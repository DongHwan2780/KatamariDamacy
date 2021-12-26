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
	/* �޽������̳��� ���� ��, HierarchyNode*�� �ּҿ��� ���� ���� ä���� �ʾҴ�. */
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
	// Readfile == �޽��� ������ �о�´�.
	// aiProcess_ConvertToLeftHanded == �޽��� �޼���ǥ�� �������� �������ش�.
	// aiProcess_Triangulate == �޽��� ������ �ִ� �������� �ﰢ���� �̷�� ���� ���� ��� �ﰢ������ �������� ��������
	m_pScene = m_Importer.ReadFile(szFullPath, aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (nullptr == m_pScene)
		return E_FAIL;

	/* ���� ����ϱ� ������ �����Ѵ�. */
	/* ���� �����ϴ� ��ü ������ ������ ���´�. */
	/* ��ü ������ ������ ���´�. */
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)			// ���� ����ִ� �޽��� ������ŭ ������
	{
		m_iNumVertices += m_pScene->mMeshes[i]->mNumVertices;	// i��° �޽��� Vertices������ ���ͼ� ��������� �־��ش�.
		m_iNumFaces += m_pScene->mMeshes[i]->mNumFaces;			// i��° �޽��� Faces������ ���ͼ� ��������� �־��ش�.
	}

	m_pVertices = new VTXMESH[m_iNumVertices];					// Vertices ������ŭ �����Ҵ�
	ZeroMemory(m_pVertices, sizeof(VTXMESH) * m_iNumVertices);

	m_pPolygonIndices32 = new POLYGONINDICES32[m_iNumFaces];
	ZeroMemory(m_pPolygonIndices32, sizeof(POLYGONINDICES32) * m_iNumFaces);

	_uint		iStartVertexIndex = 0;
	_uint		iStartFaceIndex = 0;


	m_MeshContainers.reserve(m_pScene->mNumMeshes);		// �޽��� ������ŭ �����̳� ���� ����

	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		if (false == m_pScene->HasAnimations())
		{
			if (FAILED(Create_MeshContainer(m_pScene->mMeshes[i], &iStartVertexIndex, &iStartFaceIndex, XMMatrixIdentity() * PivotMatrix)))	// �޽��� ������ŭ �����̳� �����ϰ� �޽�, ���������ε���, ����������ε����� �Ѱ���
				return E_FAIL;
		}
		else
		{
			if (FAILED(Create_MeshContainer(m_pScene->mMeshes[i], &iStartVertexIndex, &iStartFaceIndex, XMMatrixIdentity())))	// �޽��� ������ŭ �����̳� �����ϰ� �޽�, ���������ε���, ����������ε����� �Ѱ���
				return E_FAIL;
		}
	}

	m_iStride = sizeof(VTXMESH);		// ���� ����ü�� ũ��

	/* ���� ���� ���׸������ �ε��Ѵ�. */
	m_ModelTextures.reserve(m_pScene->mNumMaterials);

	for (_uint i = 0; i < m_pScene->mNumMaterials; ++i)
	{
		if (FAILED(Create_Materials(m_pScene->mMaterials[i], pMeshFilePath)))
			return E_FAIL;
	}

	/* �޽������̳ʵ��� ���׸��� �������� ���� �����Ѵ�. */

	if (false == m_pScene->HasAnimations())
	{
		// ���ؽ�, �ε��� ���� ����
		if (FAILED(Create_AllBuffer(pShaderFilePath)))
			return E_FAIL;

		return S_OK;
	}

	/* ������ ���������� ���� ���ϴ� ���·� �����Ѵ�. */
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

		/* ���� �޽������̳� ������ �ִ� ������� Combined����� �迭�� �����Ѵ�. */
		pMeshContainer->Get_BoneMatrices(BoneMatrices);

		/* �迭�� ���̴��� ������. */
		SetUp_ValueOnShader("g_BoneMatrices", BoneMatrices, sizeof(_matrix) * 128);

		if (FAILED(m_EffectDescs[iPassIndex].pPass->Apply(0, m_pDeviceContext)))
			return E_FAIL;

		m_pDeviceContext->DrawIndexed(pMeshContainer->Get_NumFaces() * 3,
			pMeshContainer->Get_StartFaceIndex() * 3,
			pMeshContainer->Get_StartVertexIndex());
		// m_pDeviceContext->DrawIndexed(/* �ε��� ����*/ /*�ε��������� ��𿡼����ͱ׸�����*/ /*���������� ��𿡼����� �׸���Ǵ���. */)
	}

	return S_OK;
}

_bool CModel::RayCast(_float3 & out, HWND hWnd, _uint iWinCX, _uint iWinCY, const _float4x4 & matWorld)
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(hWnd, &pt);

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	// ����Ʈ -> ���������̽�
	_float3 vMouse = _float3(0.f, 0.f, 0.f);
	vMouse.x = pt.x / (iWinCX * 0.5f) - 1.f;
	vMouse.y = 1.f - pt.y / (iWinCY * 0.5f);

	// ���������̽� -> �佺���̽�
	_float4x4 matProj;

	XMStoreFloat4x4(&matProj, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ));
	XMMatrixInverse( nullptr, XMLoadFloat4x4(&matProj));
	XMVector3TransformCoord(XMLoadFloat3(&vMouse), XMLoadFloat4x4(&matProj));

	// �佺���̽����� ����(ray)�� ������ �����Ѵ�.
	_float3 vRayPos = _float3(0.f, 0.f, 0.f);
	_float3 vRayDir = _float3(vMouse.x - vRayPos.x, vMouse.y - vRayPos.y, - vMouse.z - vRayPos.z);

	// �佺���̽� -> ���彺���̽�
	_float4x4 matView;
	XMStoreFloat4x4(&matView, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW));
	XMMatrixInverse(nullptr, XMLoadFloat4x4(&matView));
	XMVector3TransformCoord(XMLoadFloat3(&vRayPos), XMLoadFloat4x4(&matView));
	XMVector3TransformNormal(XMLoadFloat3(&vRayDir), XMLoadFloat4x4(&matView));

	// ���彺���̽� -> ���ý����̽�
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

		if (DX::Intersects(XMLoadFloat3(&vRayPos), XMLoadFloat3(&vRayDir), XMLoadFloat3(&v1), XMLoadFloat3(&v2), XMLoadFloat3(&v3), dist))		//�����̶� �������̶� �浹������
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
		/* ������ ��ġ�� ������ �´�. */
		memcpy(&m_pVertices[*pStartVertexIndex].vPosition, &pMesh->mVertices[i], sizeof(_float3));

		_vector		vPosition;
		vPosition = XMLoadFloat3(&m_pVertices[*pStartVertexIndex].vPosition);
		vPosition = XMVector3TransformCoord(vPosition, PivotMatrix);
		XMStoreFloat3(&m_pVertices[*pStartVertexIndex].vPosition, vPosition);

		/* ������ ��ָ� ������ �´�. */
		memcpy(&m_pVertices[*pStartVertexIndex].vNormal, &pMesh->mNormals[i], sizeof(_float3));

		/* ������ �ؽ��� ��������ǥ�� ������ �´�. */
		memcpy(&m_pVertices[*pStartVertexIndex].vTexUV, &pMesh->mTextureCoords[0][i], sizeof(_float2));

		/* ������ ź��Ʈ ������ ������ �´�. */
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

		char	szTextureFileName[MAX_PATH] = "";		// ���ϸ�
		char	szExt[MAX_PATH] = "";					// Ȯ���ڸ�

		_splitpath(strTexturePath.data, nullptr, nullptr, szTextureFileName, szExt);	// �ؽ����ּҿ��� �ؽ����̸�, Ȯ���ڸ� �����ͼ� �ٿ���
		strcat_s(szTextureFileName, szExt);		

		char		szFullPath[MAX_PATH] = "";
		strcpy_s(szFullPath, pMeshFilePath);		// �޾ƿ� �޽������� �ּҿ�
		strcat_s(szFullPath, szTextureFileName);	// �ؽ����̸� + Ȯ���ڸ��� szFullPath�� �ٿ���

		_tchar		szWideFullPath[MAX_PATH] = TEXT("");

		MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szWideFullPath, MAX_PATH);

		if (!strcmp(szExt, ".dds"))				// Ȯ���ڸ� ���� Ÿ�Կ� ���� �ؽ��� ����
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

	m_SortByMaterialMesh.resize(iNumMaterials);	// ���׸����� ������ŭ ������ ���Ҹ� ����ش�.
												// reserve�� ������ ����� �� ���Ҵ� ä�������ʰ�, ���ڰ� capacity���� Ŭ�� iter�� ����Ҽ����� ��ȿȭ�Ǵ� �������� ����.
												// �׷��� resize�� ���� ������ ����ְ� �迭���ҵ� null������ ���.
												
	for (auto& pMeshContainer : m_MeshContainers)
	{
		_uint	iMeshMaterialIndex = pMeshContainer->Get_MeshMaterialIndex();	// �޽������̳ʿ� ������ �޽��� ���׸��� �ε����� �޾Ƽ�
		if (iMeshMaterialIndex >= m_pScene->mNumMaterials)
			return E_FAIL;

		m_SortByMaterialMesh[iMeshMaterialIndex].push_back(pMeshContainer);		// �ش� ���׸��� �ε����� �´� �迭�� ���ҷ� �־���
		Safe_AddRef(pMeshContainer);
	}
	return S_OK;
}

HRESULT CModel::Create_HierarchyNodes(aiNode * pNode, CHierarchyNode * pParent, _uint iDepth, _fmatrix PivotMatrix)
{
	_matrix		TransformationMatrix;
	memcpy(&TransformationMatrix, &pNode->mTransformation, sizeof(_matrix));		/* ���� �� ��尡 �뺯�ϰ��ִ� ���� mTransformation�� ������ �´�. */

	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pNode->mName.data, TransformationMatrix * PivotMatrix, pParent, iDepth);	// ù ���� �� �����Ǵ� ���� ���� �ֻ��� �θ� ��尡 �ȴ�.
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);		// ������ ��带 �־��ְ�

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

	/* �ܺο��� ������ ���̴� ������ �����Ͽ� ���̳ʸ�ȭ�Ͽ��� �޸𸮿����ȿ� �����Ҵ�. */
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

/* ���� �������� ���� ��������� �޾ƿͼ� ó���� �� �ֵ��� �غ��Ѵ�. */
HRESULT CModel::SetUp_SkinnedInfo()
{
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		aiMesh*		pMesh = m_pScene->mMeshes[i];
		CMeshContainer*	pMeshContainer = m_MeshContainers[i];

		/* ���� �޽ÿ� ������ ��ġ�� ������ ������ �����Ͽ� �޽������̳ʾȿ� �����صд�. */
		/* ������ �ÿ�, �޽������̳ʴ����� ������ ���鼭 �׸��⶧���� �� �޽ÿ� ������ ��ġ�� ������ ��Ƶд�.  */
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

			/* ������ ���� �ε����� ���� ����Ʈ�� ä���. */
			/* ���� ���ԵǾ��ִ� ����Ʈ�� ������ �� ������ ��ġ�� ������ ����.  */
			for (_uint k = 0; k < pBone->mNumWeights; ++k)
			{
				/* �ϳ��� ����Ʈ���� � ������ ������ ��ġ����? ������ �ε����� �����´�.   */
				/* ���� �ϳ��� ���� ���۷� ��� ��Ƶ� �������۱������� �ε����� �����´�. */
				_uint	iVertexIndex = pBone->mWeights[k].mVertexId + pMeshContainer->Get_StartVertexIndex();

				/* ������ ����Ʈ���� �װ��� �������ִ�. */
				/* �޽ø� �����ϴ� ������ ��ȸ�ϸ� ���� ������ ��ġ�� ������ �ε����� ���Դ�. */
				/* ������ ����Ʈ�� ä������ߴ��� �̹� �ٸ� ���� ���� ä������ �� �־���. */
				/* �ٸ����� ���� ä���ذ�� �� ���� ����Ʈ�� ���� ä���. */
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
		/* �ϳ��� �ִϸ��̼� �ϳ��� ���� ��ü�� �ǹ�. */
		aiAnimation*	pAnim = m_pScene->mAnimations[i];
		if (nullptr == pAnim)
			return E_FAIL;

		CAnimation*		pAnimation = CAnimation::Create(pAnim->mName.data, pAnim->mDuration, pAnim->mTicksPerSecond);
		if (nullptr == pAnimation)
			return E_FAIL;

		/* ���� �ִϸ��̼��� ǥ���ϱ����� ����. */
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

				/* ���� ǥ���ؾ��� Ű�����ӵ�(�Ѿִϸ��̼Ǽӿ�ǥ���Ǵ�)�� �������, �ð����� �����س��´�. */
				pChannel->Add_KeyFrameDesc(pKeyFrame);
			}

			Add_Channel_To_HierarchyNode(i, pChannel);

			/* �ִϸ��̼��� ǥ���ϱ����� ������ �ִԿ��Ӽ� �ȿ� ��Ƴ��´�. */
			pAnimation->Add_Channel(pChannel);

		}
		/* �ִϸ��̼��� ����. */
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

	/* ���� �ִϸ��̼��� ����ϰ� �ִ� �ð��� ����ϰ� �� �ð����� ���� ���� ��������� ���� m_TrnasformationMAtrix�� �����س��´�. */
	/* Ư�� �ִϸ��̼��� ����ɶ�, �� �ִϸ��̼��� �����ϴ� ������ ����� �����Ѵ�. */
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

	(*iter)->Add_Channel(iAnimationindex, pChannel);		/* ���⼭ �߰��Ǵ� ä���� ���(��)�� ������ ���� �������� ǥ���ϴ� ä�� */
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
