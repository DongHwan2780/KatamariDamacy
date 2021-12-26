#include "..\Headers\Model.h"

#include "MeshContainer.h"
#include "Texture.h"

#include "Animation.h"
#include "Channel.h"

#include "HierarchyNode.h"

CModel::CModel(DEVICES)
	:CComponent(pDevice, pDeviceContext)
{
}

CModel::CModel(const CModel & other)
	: CComponent(other)
	, m_pScene(other.m_pScene)
	, m_iNumVertices(other.m_iNumVertices)
	, m_iNumFaces(other.m_iNumFaces)
	, m_pVertices(other.m_pVertices)
	, m_pPolygonIndices32(other.m_pPolygonIndices32)
	, m_ModelTextures(other.m_ModelTextures)
	, m_pVB(other.m_pVB)
	, m_pIB(other.m_pIB)
	, m_iStride(other.m_iStride)
	, m_EffectDescs(other.m_EffectDescs)
	, m_pEffect(other.m_pEffect)
	, m_iAnimationIndex(other.m_iAnimationIndex)
	, m_PivotMatrix(other.m_PivotMatrix)
{
	/* 메시컨테이너의 뼈들 중, HierarchyNode*의 주소에는 아직 값을 채우지 않았다. */
	for (auto& pPrototypeMeshContainer : other.m_MeshContainers)
	{
		CMeshContainer*	pMeshContainer = pPrototypeMeshContainer->Clone();

		m_MeshContainers.push_back(pMeshContainer);
	}

	for (auto& pMeshTextues : m_ModelTextures)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(pMeshTextues->pModelTexture[i]);
	}

	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);

	for (auto& pEffectDesc : m_EffectDescs)
	{
		Safe_AddRef(pEffectDesc.pLayOut);
		Safe_AddRef(pEffectDesc.pPass);
	}

	Safe_AddRef(m_pEffect);
}

_fmatrix CModel::Get_BoneMatrix(const char * pBoneName)
{
	CHierarchyNode*		pNode = Find_HierarchyNode(pBoneName);
	if (nullptr == pNode)
		return XMMatrixIdentity();

	return pNode->Get_OffsetMatrix() * pNode->Get_CombinedTransformationMatrix();
}

HRESULT CModel::Initialize_Prototype(const char * pMeshFilePath, const char * pMeshFileName, const _tchar* pShaderFilePath, _fmatrix PivotMatrix)
{
	char		szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pMeshFilePath);
	strcat_s(szFullPath, pMeshFileName);

	m_PivotMatrix = PivotMatrix;
	// Readfile == 메쉬의 정보를 읽어온다.
	// aiProcess_ConvertToLeftHanded == 메쉬를 왼손좌표계 기준으로 변경해준다.
	// aiProcess_Triangulate == 메쉬가 가지고 있는 정점들이 삼각형을 이루고 있지 않을 경우 삼각형으로 정점들을 연결해줌
	m_pScene = m_Importer.ReadFile(szFullPath, aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (nullptr == m_pScene)
		return E_FAIL;

	/* 내가 사용하기 좋도록 정리한다. */
	/* 모델을 구성하는 전체 정점의 갯수를 얻어온다. */
	/* 전체 폴리곤 갯수를 얻어온다. */
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)			// 씬이 들고있는 메쉬의 개수만큼 돌려서
	{
		m_iNumVertices += m_pScene->mMeshes[i]->mNumVertices;	// i번째 메쉬의 Vertices개수를 얻어와서 멤버변수에 넣어준다.
		m_iNumFaces += m_pScene->mMeshes[i]->mNumFaces;			// i번째 메쉬의 Faces개수를 얻어와서 멤버변수에 넣어준다.
	}

	m_pVertices = new VTXMESH[m_iNumVertices];					// Vertices 개수만큼 동적할당
	ZeroMemory(m_pVertices, sizeof(VTXMESH) * m_iNumVertices);

	m_pPolygonIndices32 = new POLYGONINDICES32[m_iNumFaces];
	ZeroMemory(m_pPolygonIndices32, sizeof(POLYGONINDICES32) * m_iNumFaces);

	_uint		iStartVertexIndex = 0;
	_uint		iStartFaceIndex = 0;


	m_MeshContainers.reserve(m_pScene->mNumMeshes);		// 메쉬의 개수만큼 컨테이너 공간 예약

	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		if (false == m_pScene->HasAnimations())
		{
			if (FAILED(Create_MeshContainer(m_pScene->mMeshes[i], &iStartVertexIndex, &iStartFaceIndex, XMMatrixIdentity() * PivotMatrix)))	// 메쉬의 개수만큼 컨테이너 생성하고 메쉬, 시작정점인덱스, 폴리곤시작인덱스를 넘겨줌
				return E_FAIL;
		}
		else
		{
			if (FAILED(Create_MeshContainer(m_pScene->mMeshes[i], &iStartVertexIndex, &iStartFaceIndex, XMMatrixIdentity())))	// 메쉬의 개수만큼 컨테이너 생성하고 메쉬, 시작정점인덱스, 폴리곤시작인덱스를 넘겨줌
				return E_FAIL;
		}
	}

	m_iStride = sizeof(VTXMESH);		// 정점 구조체의 크기

	/* 현재 모델의 머테리얼들을 로드한다. */
	m_ModelTextures.reserve(m_pScene->mNumMaterials);

	for (_uint i = 0; i < m_pScene->mNumMaterials; ++i)
	{
		if (FAILED(Create_Materials(m_pScene->mMaterials[i], pMeshFilePath)))
			return E_FAIL;
	}

	/* 메시컨테이너들을 머테리얼 기준으로 묶고 정렬한다. */

	if (false == m_pScene->HasAnimations())
	{
		// 버텍스, 인덱스 버퍼 생성
		if (FAILED(Create_AllBuffer(pShaderFilePath)))
			return E_FAIL;

		return S_OK;
	}

	/* 노드들의 계층구조를 내가 원하는 형태로 보관한다. */
	Create_HierarchyNodes(m_pScene->mRootNode, nullptr, 0, PivotMatrix);

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest) {
		return pSour->Get_Depth() < pDest->Get_Depth();
	});


	if (FAILED(SetUp_SkinnedInfo()))
		return E_FAIL;

	if (FAILED(Create_AllBuffer(pShaderFilePath)))
		return E_FAIL;

	if (FAILED(SetUp_AnimationInfo()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize_Clone(void * pArg)
{
	Create_HierarchyNodes(m_pScene->mRootNode, nullptr, 0, m_PivotMatrix);

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	if (FAILED(SetUp_SkinnedInfo()))
		return E_FAIL;

	Sort_MeshesByMaterial();

	if (FAILED(SetUp_AnimationInfo()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Render(_uint iMaterialIndex, _uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	m_pDeviceContext->IASetInputLayout(m_EffectDescs[iPassIndex].pLayOut);


	_matrix			BoneMatrices[128];

	for (auto& pMeshContainer : m_SortByMaterialMesh[iMaterialIndex])
	{
		ZeroMemory(BoneMatrices, sizeof(_matrix) * 128);

		/* 현재 메시컨테이너 영향을 주는 뼈대들의 Combined행렬을 배열에 저장한다. */
		pMeshContainer->Get_BoneMatrices(BoneMatrices);

		/* 배열을 셰이더에 던진다. */
		SetUp_ValueOnShader("g_BoneMatrices", BoneMatrices, sizeof(_matrix) * 128);

		if (FAILED(m_EffectDescs[iPassIndex].pPass->Apply(0, m_pDeviceContext)))
			return E_FAIL;

		m_pDeviceContext->DrawIndexed(pMeshContainer->Get_NumFaces() * 3,
			pMeshContainer->Get_StartFaceIndex() * 3,
			pMeshContainer->Get_StartVertexIndex());
		// m_pDeviceContext->DrawIndexed(/* 인덱스 개수*/ /*인덱스버퍼의 어디에서부터그릴건지*/ /*정점버퍼의 어디에서부터 그리면되는지. */)
	}

	return S_OK;
}

_bool CModel::RayCast(_float3 & out, HWND hWnd, _uint iWinCX, _uint iWinCY, const _float4x4 & matWorld)
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(hWnd, &pt);

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	// 뷰포트 -> 투영스페이스
	_float3 vMouse = _float3(0.f, 0.f, 0.f);
	vMouse.x = pt.x / (iWinCX * 0.5f) - 1.f;
	vMouse.y = 1.f - pt.y / (iWinCY * 0.5f);

	// 투영스페이스 -> 뷰스페이스
	_float4x4 matProj;

	XMStoreFloat4x4(&matProj, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ));
	XMMatrixInverse( nullptr, XMLoadFloat4x4(&matProj));
	XMVector3TransformCoord(XMLoadFloat3(&vMouse), XMLoadFloat4x4(&matProj));

	// 뷰스페이스에서 광선(ray)의 정보를 설정한다.
	_float3 vRayPos = _float3(0.f, 0.f, 0.f);
	_float3 vRayDir = _float3(vMouse.x - vRayPos.x, vMouse.y - vRayPos.y, - vMouse.z - vRayPos.z);

	// 뷰스페이스 -> 월드스페이스
	_float4x4 matView;
	XMStoreFloat4x4(&matView, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW));
	XMMatrixInverse(nullptr, XMLoadFloat4x4(&matView));
	XMVector3TransformCoord(XMLoadFloat3(&vRayPos), XMLoadFloat4x4(&matView));
	XMVector3TransformNormal(XMLoadFloat3(&vRayDir), XMLoadFloat4x4(&matView));

	// 월드스페이스 -> 로컬스페이스
	_float4x4 matInverse;
	XMStoreFloat4x4(&matInverse, XMMatrixInverse(nullptr, XMLoadFloat4x4(&matWorld)));
	XMVector3TransformCoord(XMLoadFloat3(&vRayPos), XMLoadFloat4x4(&matInverse));
	XMVector3TransformNormal(XMLoadFloat3(&vRayDir), XMLoadFloat4x4(&matInverse));

	_uint _1 = 0, _2 = 0, _3 = 0;
	_uint m_iIndexSize = sizeof(POLYGONINDICES32);
	_uint iSize = m_iIndexSize / 3;
	_float3 v1, v2, v3;
	_float dist;

	for (_uint i = 0; i < m_iNumFaces; ++i)
	{
		memcpy(&_1, ((_byte*)m_pPolygonIndices32) + i * m_iIndexSize, iSize);
		memcpy(&_2, ((_byte*)m_pPolygonIndices32) + i * m_iIndexSize + (iSize), iSize);
		memcpy(&_3, ((_byte*)m_pPolygonIndices32) + i * m_iIndexSize + (iSize * 2), iSize);

		memcpy(&v1, ((_byte*)m_pVertices) + _1 * m_iStride, sizeof(_float3));
		memcpy(&v2, ((_byte*)m_pVertices) + _2 * m_iStride, sizeof(_float3));
		memcpy(&v3, ((_byte*)m_pVertices) + _3 * m_iStride, sizeof(_float3));

		if (DX::Intersects(XMLoadFloat3(&vRayPos), XMLoadFloat3(&vRayDir), XMLoadFloat3(&v1), XMLoadFloat3(&v2), XMLoadFloat3(&v3), dist))		//광선이랑 폴리곤이랑 충돌헀으면
		{
			XMStoreFloat3(&out, dist * XMLoadFloat3(&vRayDir));
			XMVector3TransformCoord(XMLoadFloat3(&out), XMLoadFloat4x4(&matWorld));
			return true;
		}
	}

	RELEASE_INSTANCE(CPipeLine);

	return false;
}

HRESULT CModel::Bind_Buffers()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	_uint		iOffSet = 0;

	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVB, &m_iStride, &iOffSet);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
	m_pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return S_OK;
}

HRESULT CModel::SetUp_ValueOnShader(const char * pConstantName, void * pData, _uint iByteSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	if (FAILED(pVariable->SetRawValue(pData, 0, iByteSize)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::SetUp_TextureOnShader(const char * pConstantName, _uint iMaterialIndex, aiTextureType eTextureType)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName)->AsShaderResource();
	if (nullptr == pVariable)
		return E_FAIL;


	if (nullptr != m_ModelTextures[iMaterialIndex]->pModelTexture[eTextureType])
	{
		ID3D11ShaderResourceView*		pShaderResourceView =
			m_ModelTextures[iMaterialIndex]->pModelTexture[eTextureType]->Get_ShaderResourceView(0);
		if (nullptr == pShaderResourceView)
			return E_FAIL;
		if (FAILED(pVariable->SetResource(pShaderResourceView)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::SetUp_AnimationIndex(_uint iAnimationIndex)
{
	if (m_Animations.size() <= iAnimationIndex)
		return E_FAIL;

	m_iAnimationIndex = iAnimationIndex;

	return S_OK;
}

HRESULT CModel::Play_Animation(_double DeltaTime)
{
	Update_CombinedTransformationMatrices(DeltaTime);

	return S_OK;
}

HRESULT CModel::Create_MeshContainer(aiMesh * pMesh, _uint * pStartVertexIndex, _uint * pStartFaceIndex, _fmatrix PivotMatrix)
{
	_uint		iStartVertexIndex = *pStartVertexIndex;

	for (_uint i = 0; i < pMesh->mNumVertices; ++i)
	{
		/* 정점의 위치를 가지고 온다. */
		memcpy(&m_pVertices[*pStartVertexIndex].vPosition, &pMesh->mVertices[i], sizeof(_float3));

		_vector		vPosition;
		vPosition = XMLoadFloat3(&m_pVertices[*pStartVertexIndex].vPosition);
		vPosition = XMVector3TransformCoord(vPosition, PivotMatrix);
		XMStoreFloat3(&m_pVertices[*pStartVertexIndex].vPosition, vPosition);

		/* 정점의 노멀를 가지고 온다. */
		memcpy(&m_pVertices[*pStartVertexIndex].vNormal, &pMesh->mNormals[i], sizeof(_float3));

		/* 정점의 텍스쳐 유브이좌표를 가지고 온다. */
		memcpy(&m_pVertices[*pStartVertexIndex].vTexUV, &pMesh->mTextureCoords[0][i], sizeof(_float2));

		/* 정점의 탄젠트 정보를 가지고 온다. */
		memcpy(&m_pVertices[*pStartVertexIndex].vTangent, &pMesh->mTangents[i], sizeof(_float3));

		++(*pStartVertexIndex);
	}

	_uint		iStartFaceIndex = *pStartFaceIndex;

	for (_uint i = 0; i < pMesh->mNumFaces; ++i)
	{
		m_pPolygonIndices32[*pStartFaceIndex]._0 = pMesh->mFaces[i].mIndices[0];
		m_pPolygonIndices32[*pStartFaceIndex]._1 = pMesh->mFaces[i].mIndices[1];
		m_pPolygonIndices32[*pStartFaceIndex]._2 = pMesh->mFaces[i].mIndices[2];
		++(*pStartFaceIndex);
	}

	CMeshContainer*			pMeshContainer = CMeshContainer::Create(pMesh->mName.data, pMesh->mNumFaces, iStartFaceIndex, iStartVertexIndex, pMesh->mMaterialIndex);
	if (nullptr == pMeshContainer)
		return E_FAIL;

	m_MeshContainers.push_back(pMeshContainer);

	return S_OK;
}

HRESULT CModel::Create_AllBuffer(const _tchar * pShaderFilePath)
{
	D3D11_BUFFER_DESC			BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

	BufferDesc.ByteWidth = sizeof(VTXMESH) * m_iNumVertices;
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = sizeof(VTXMESH);

	D3D11_SUBRESOURCE_DATA		SubResourceData;
	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	SubResourceData.pSysMem = m_pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &SubResourceData, &m_pVB)))
		return E_FAIL;


	/* For.IndexBuffer */
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

	BufferDesc.ByteWidth = sizeof(POLYGONINDICES32) * m_iNumFaces;
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubResourceData.pSysMem = m_pPolygonIndices32;

	if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &SubResourceData, &m_pIB)))
		return E_FAIL;

	/*if (m_pScene->HasAnimations())
	{

	}*/
	D3D11_INPUT_ELEMENT_DESC		ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(Compile_Shader(ElementDesc, 6, pShaderFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Create_Materials(aiMaterial * pMaterial, const char * pMeshFilePath)
{
	MODELTEXTURES*		pModelTexture = new MODELTEXTURES;
	ZeroMemory(pModelTexture, sizeof(MODELTEXTURES));

	for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
	{
		aiString		strTexturePath;

		if (FAILED(pMaterial->GetTexture(aiTextureType(i), 0, &strTexturePath)))
			continue;

		char	szTextureFileName[MAX_PATH] = "";		// 파일명
		char	szExt[MAX_PATH] = "";					// 확장자명

		_splitpath(strTexturePath.data, nullptr, nullptr, szTextureFileName, szExt);	// 텍스쳐주소에서 텍스쳐이름, 확장자명만 가져와서 붙여줌
		strcat_s(szTextureFileName, szExt);		

		char		szFullPath[MAX_PATH] = "";
		strcpy_s(szFullPath, pMeshFilePath);		// 받아온 메쉬파일의 주소와
		strcat_s(szFullPath, szTextureFileName);	// 텍스쳐이름 + 확장자명을 szFullPath에 붙여줌

		_tchar		szWideFullPath[MAX_PATH] = TEXT("");

		MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szWideFullPath, MAX_PATH);

		if (!strcmp(szExt, ".dds"))				// 확장자명에 따라서 타입에 따른 텍스쳐 생성
			pModelTexture->pModelTexture[i] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::DDS, szWideFullPath);		
		else if (!strcmp(szExt, ".tga"))
			pModelTexture->pModelTexture[i] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TGA, szWideFullPath);
		else
			pModelTexture->pModelTexture[i] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::WIC, szWideFullPath);
	}

	m_ModelTextures.push_back(pModelTexture);

	return S_OK;
}

HRESULT CModel::Sort_MeshesByMaterial()
{
	_uint		iNumMaterials = m_ModelTextures.size();

	m_SortByMaterialMesh.resize(iNumMaterials);	// 머테리얼의 개수만큼 공간과 원소를 잡아준다.
												// reserve는 공간만 잡아줄 뿐 원소는 채워주지않고, 인자가 capacity보다 클때 iter를 사용할수없고 무효화되는 주의점이 있음.
												// 그래서 resize를 통해 공간도 잡아주고 배열원소도 null값으로 사용.
												
	for (auto& pMeshContainer : m_MeshContainers)
	{
		_uint	iMeshMaterialIndex = pMeshContainer->Get_MeshMaterialIndex();	// 메쉬컨테이너에 보관된 메쉬의 머테리얼 인덱스를 받아서
		if (iMeshMaterialIndex >= m_pScene->mNumMaterials)
			return E_FAIL;

		m_SortByMaterialMesh[iMeshMaterialIndex].push_back(pMeshContainer);		// 해당 머테리얼 인덱스에 맞는 배열에 원소로 넣어줌
		Safe_AddRef(pMeshContainer);
	}
	return S_OK;
}

HRESULT CModel::Create_HierarchyNodes(aiNode * pNode, CHierarchyNode * pParent, _uint iDepth, _fmatrix PivotMatrix)
{
	_matrix		TransformationMatrix;
	memcpy(&TransformationMatrix, &pNode->mTransformation, sizeof(_matrix));		/* 현재 이 노드가 대변하고있는 뼈의 mTransformation를 가지고 온다. */

	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pNode->mName.data, TransformationMatrix * PivotMatrix, pParent, iDepth);	// 첫 실행 시 생성되는 노드는 가장 최상위 부모 노드가 된다.
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);		// 생성한 노드를 넣어주고

	for (_uint i = 0; i < pNode->mNumChildren; ++i)		
		Create_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth + 1);	// 

	return S_OK;
}

HRESULT CModel::Compile_Shader(D3D11_INPUT_ELEMENT_DESC * pElementDescs, _uint iNumElement, const _tchar * pShaderFilePath, _uint iTechniqueIndex)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	ID3DBlob*		pCompileShader = nullptr;
	ID3DBlob*		pCompileShaderErrorMessage = nullptr;

	/* 외부에서 가져온 쉐이더 파일을 번역하여 바이너리화하였고 메모리영역안에 ㄷ마았다. */
	if (FAILED(D3DCompileFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, nullptr, "fx_5_0", iFlag, 0, &pCompileShader, &pCompileShaderErrorMessage)))
		return E_FAIL;

	if (FAILED(D3DX11CreateEffectFromMemory(pCompileShader->GetBufferPointer(), pCompileShader->GetBufferSize(), 0, m_pDevice, &m_pEffect)))
		return E_FAIL;

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByIndex(iTechniqueIndex);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;

	m_EffectDescs.reserve(TechniqueDesc.Passes);

	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		EFFECTDESC			EffectDesc;

		EffectDesc.pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == EffectDesc.pPass)
			return E_FAIL;

		D3DX11_PASS_DESC			PassDesc;

		if (FAILED(EffectDesc.pPass->GetDesc(&PassDesc)))
			return E_FAIL;

		if (FAILED(m_pDevice->CreateInputLayout(pElementDescs, iNumElement, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &EffectDesc.pLayOut)))
			return E_FAIL;

		m_EffectDescs.push_back(EffectDesc);
	}

	return S_OK;
}

/* 실제 정점들이 뼈의 행렬정보를 받아와서 처리될 수 있도록 준비한다. */
HRESULT CModel::SetUp_SkinnedInfo()
{
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		aiMesh*		pMesh = m_pScene->mMeshes[i];
		CMeshContainer*	pMeshContainer = m_MeshContainers[i];

		/* 현재 메시에 영향을 미치는 뼈들의 정보를 구성하여 메시컨테이너안에 보관해둔다. */
		/* 렌더링 시에, 메시컨테이너단위로 루프를 돌면서 그리기때문에 각 메시에 영향을 미치는 뼈들을 모아둔다.  */
		for (_uint j = 0; j < pMesh->mNumBones; ++j)
		{
			aiBone*		pBone = pMesh->mBones[j];
			if (nullptr == pBone)
				return E_FAIL;

			BONEDESC*	pBoneDesc = new BONEDESC;
			ZeroMemory(pBoneDesc, sizeof(BONEDESC));

			pBoneDesc->pName = pBone->mName.data;
			pBoneDesc->pHierarchyNode = Find_HierarchyNode(pBone->mName.data);

			_matrix		OffsetMatrix;
			memcpy(&OffsetMatrix, &pBone->mOffsetMatrix, sizeof(_matrix));
			XMStoreFloat4x4(&pBoneDesc->OffsetMatrix, XMMatrixTranspose(OffsetMatrix));
			pBoneDesc->pHierarchyNode->Set_OffSetMatrix(XMMatrixTranspose(OffsetMatrix));

			/* 정점의 블렌드 인덱스와 블렌드 웨이트를 채운다. */
			/* 뼈에 포함되어있는 웨이트의 갯수는 곧 영향을 미치는 정점의 갯수.  */
			for (_uint k = 0; k < pBone->mNumWeights; ++k)
			{
				/* 하나의 웨이트값은 어떤 정점에 영향을 미치는지? 정점의 인덱스를 가져온다.   */
				/* 내가 하나의 정점 버퍼로 모두 모아둔 정점버퍼기준으로 인덱스를 가져온다. */
				_uint	iVertexIndex = pBone->mWeights[k].mVertexId + pMeshContainer->Get_StartVertexIndex();

				/* 정점은 웨이트값을 네개씩 가질수있다. */
				/* 메시를 구성하는 뼈들을 순회하며 뼈가 영향을 미치는 정점의 인덱스를 얻어왔다. */
				/* 정점에 웨이트를 채우려고했더니 이미 다른 뼈가 값을 채워놓을 수 있었다. */
				/* 다른뼈가 먼저 채워준경우 그 다음 웨이트에 값을 채운다. */
				if (0.0f == m_pVertices[iVertexIndex].vBlendWeight.x)
				{
					m_pVertices[iVertexIndex].vBlendIndex.x = j;
					m_pVertices[iVertexIndex].vBlendWeight.x = pBone->mWeights[k].mWeight;
				}

				else if (0.0f == m_pVertices[iVertexIndex].vBlendWeight.y)
				{
					m_pVertices[iVertexIndex].vBlendIndex.y = j;
					m_pVertices[iVertexIndex].vBlendWeight.y = pBone->mWeights[k].mWeight;
				}

				else if (0.0f == m_pVertices[iVertexIndex].vBlendWeight.z)
				{
					m_pVertices[iVertexIndex].vBlendIndex.z = j;
					m_pVertices[iVertexIndex].vBlendWeight.z = pBone->mWeights[k].mWeight;
				}

				else
				{
					m_pVertices[iVertexIndex].vBlendIndex.w = j;
					m_pVertices[iVertexIndex].vBlendWeight.w = pBone->mWeights[k].mWeight;
				}
			}
			pMeshContainer->Add_Bones(pBoneDesc);
		}
	}
	return S_OK;
}

HRESULT CModel::SetUp_AnimationInfo()
{
	_uint		iNumAnimation = m_pScene->mNumAnimations;

	for (auto& pHierarchyNode : m_HierarchyNodes)
		pHierarchyNode->Resize_Channels(iNumAnimation);

	for (_uint i = 0; i < iNumAnimation; ++i)
	{
		/* 하나의 애니메이션 하나의 동작 전체를 의미. */
		aiAnimation*	pAnim = m_pScene->mAnimations[i];
		if (nullptr == pAnim)
			return E_FAIL;

		CAnimation*		pAnimation = CAnimation::Create(pAnim->mName.data, pAnim->mDuration, pAnim->mTicksPerSecond);
		if (nullptr == pAnimation)
			return E_FAIL;

		/* 현재 애니메이션을 표현하기위한 뼈들. */
		for (_uint j = 0; j < pAnim->mNumChannels; ++j)
		{
			aiNodeAnim*		pNodeAnim = pAnim->mChannels[j];
			if (nullptr == pNodeAnim)
				return E_FAIL;

			CChannel*		pChannel = CChannel::Create(pNodeAnim->mNodeName.data);
			if (nullptr == pChannel)
				return E_FAIL;

			_uint		iMaxKeyFrame = max(pNodeAnim->mNumScalingKeys, pNodeAnim->mNumRotationKeys);
			iMaxKeyFrame = max(iMaxKeyFrame, pNodeAnim->mNumPositionKeys);


			for (_uint k = 0; k < iMaxKeyFrame; ++k)
			{
				KEYFRAMEDESC*		pKeyFrame = new KEYFRAMEDESC;
				ZeroMemory(pKeyFrame, sizeof(KEYFRAMEDESC));

				if (k < pNodeAnim->mNumScalingKeys)
				{
					memcpy(&pKeyFrame->vScale, &pNodeAnim->mScalingKeys[k].mValue, sizeof(_float3));
					pKeyFrame->Time = pNodeAnim->mScalingKeys[k].mTime;
				}

				if (k < pNodeAnim->mNumRotationKeys)
				{
					pKeyFrame->vRotation.x = pNodeAnim->mRotationKeys[k].mValue.x;
					pKeyFrame->vRotation.y = pNodeAnim->mRotationKeys[k].mValue.y;
					pKeyFrame->vRotation.z = pNodeAnim->mRotationKeys[k].mValue.z;
					pKeyFrame->vRotation.w = pNodeAnim->mRotationKeys[k].mValue.w;
					pKeyFrame->Time = pNodeAnim->mRotationKeys[k].mTime;
				}

				if (k < pNodeAnim->mNumPositionKeys)
				{
					memcpy(&pKeyFrame->vPosition, &pNodeAnim->mPositionKeys[k].mValue, sizeof(_float3));
					pKeyFrame->Time = pNodeAnim->mPositionKeys[k].mTime;
				}

				/* 뼈가 표현해야할 키프레임들(한애니메이션속에표현되는)의 상태행렬, 시간값을 저장해놓는다. */
				pChannel->Add_KeyFrameDesc(pKeyFrame);
			}

			Add_Channel_To_HierarchyNode(i, pChannel);

			/* 애니메이션을 표현하기위한 뼈들을 애님에ㅣ션 안에 모아놓는다. */
			pAnimation->Add_Channel(pChannel);

		}
		/* 애니메이션의 집합. */
		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

CHierarchyNode * CModel::Find_HierarchyNode(const char * pBoneName)
{
	auto iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CHierarchyNode* pNode) 
	{
		if (0 == strcmp(pNode->Get_Name(), pBoneName))
			return true;

		return false;
	});

	return *iter;
}

HRESULT CModel::Update_CombinedTransformationMatrices(_double DeltaTime)
{
	if (m_Animations.empty() || m_iAnimationIndex >= m_Animations.size())
		return E_FAIL;

	/* 현재 애니메이션을 재생하고 있는 시간을 계산하고 그 시간값에 따른 뼈의 상태행렬을 만들어서 m_TrnasformationMAtrix에 대입해놓는다. */
	/* 특정 애니메이션이 재생될때, 그 애니메이션을 구성하는 뼈들의 행렬을 갱신한다. */
	m_Animations[m_iAnimationIndex]->Update_TransformationMatrices(DeltaTime);

	for (auto& pHierarchyNodes : m_HierarchyNodes)
		pHierarchyNodes->Update_CombinedTransformationMatrix(m_iAnimationIndex);

	return S_OK;
}

void CModel::Add_Channel_To_HierarchyNode(_uint iAnimationindex, CChannel * pChannel)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CHierarchyNode* pNode)->bool
	{
		return !strcmp(pNode->Get_Name(), pChannel->Get_Name());
	});

	(*iter)->Add_Channel(iAnimationindex, pChannel);		/* 여기서 추가되는 채널은 노드(뼈)의 정보와 같은 뼈정보플 표현하는 채널 */
}

CModel * CModel::Create(DEVICES, const char * pMeshFilePath, const char * pMeshFileName, const _tchar* pShaderFilePath, _fmatrix PivotMatrix)
{
	CModel*	pInstance = new CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pMeshFilePath, pMeshFileName, pShaderFilePath, PivotMatrix)))
	{	
		MSG_BOX("Failed To Creating CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*	pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed To Creating CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pVertices);
		Safe_Delete_Array(m_pPolygonIndices32);

		for (auto& pModelTextures : m_ModelTextures)
		{
			for (auto& pTexture : pModelTextures->pModelTexture)
				Safe_Release(pTexture);

			Safe_Delete(pModelTextures);
		}
		m_ModelTextures.clear();

		m_Importer.FreeScene();
	}

	for (auto& pModelTextures : m_ModelTextures)
	{
		for (auto& pTexture : pModelTextures->pModelTexture)
			Safe_Release(pTexture);
	}
	m_ModelTextures.clear();

	for (auto& pMeshContainers : m_SortByMaterialMesh)
	{
		for (auto& pMeshContainer : pMeshContainers)
			Safe_Release(pMeshContainer);
		pMeshContainers.clear();
	}
	m_SortByMaterialMesh.clear();

	for (auto& pMeshContainer : m_MeshContainers)
		Safe_Release(pMeshContainer);
	m_MeshContainers.clear();

	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);
	m_HierarchyNodes.clear();


	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	for (auto& EffectDesc : m_EffectDescs)
	{
		Safe_Release(EffectDesc.pLayOut);
		Safe_Release(EffectDesc.pPass);
	}
	m_EffectDescs.clear();

	Safe_Release(m_pEffect);



	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();
}
