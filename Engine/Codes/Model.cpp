#include "..\Headers\Model.h"

#include "MeshContainer.h"
#include "Texture.h"

#include "Animation.h"
#include "Channel.h"

#include "HierarchyNode.h"

CModel::CModel(DEVICES)
	:CVIBuffer(m_pDevice, m_pDeviceContext)
{
}

CModel::CModel(const CModel & other)
	: CVIBuffer(other)
	, m_iNumVertices(other.m_iNumVertices)
	, m_iNumFaces(other.m_iNumFaces)
	, m_pVertices(other.m_pVertices)
	, m_pPolygonIndices32(other.m_pPolygonIndices32)
	, m_MeshContainers(m_MeshContainers)
	, m_SortByMaterialMesh(other.m_SortByMaterialMesh)
	, m_ModelTextures(other.m_ModelTextures)
{
	for (auto& pMeshContainer : m_MeshContainers)
		Safe_AddRef(pMeshContainer);

	for (_uint i = 0; i < m_SortByMaterialMesh.size(); ++i)
	{
		for (auto& pSortByMaterialMesh : m_SortByMaterialMesh[i])
			Safe_AddRef(pSortByMaterialMesh);
	}

	for (auto& pMeshTextues : m_ModelTextures)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(pMeshTextues->pModelTexture[i]);
	}
}

HRESULT CModel::Initialize_Prototype(const char * pMeshFilePath, const char * pMeshFileName, const _tchar* pShaderFilePath, _fmatrix PivotMatrix)
{
	char		szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pMeshFilePath);
	strcat_s(szFullPath, pMeshFileName);

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
		if (FAILED(Create_MeshContainer(m_pScene->mMeshes[i], &iStartVertexIndex, &iStartFaceIndex, PivotMatrix)))	// 메쉬의 개수만큼 컨테이너 생성하고 메쉬, 시작정점인덱스, 폴리곤시작인덱스를 넘겨줌
			return E_FAIL;
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
	if (FAILED(Sort_MeshesByMaterial()))
		return E_FAIL;

	if (false == m_pScene->HasAnimations())
	{
		// 버텍스, 인덱스 버퍼 생성
		if (FAILED(Create_AllBuffer(pShaderFilePath)))
			return E_FAIL;

		return S_OK;
	}

	/* 노드들의 계층구조를 내가 원하는 형태로 보관한다. */
	Create_HierarchyNodes(m_pScene->mRootNode);

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
	return S_OK;
}

HRESULT CModel::Render(_uint iMaterialIndex, _uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	m_pDeviceContext->IASetInputLayout(m_EffectDescs[iPassIndex].pLayOut);

	if (FAILED(m_EffectDescs[iPassIndex].pPass->Apply(0, m_pDeviceContext)))
		return E_FAIL;

	_matrix			BoneMatrices[128];

	for (auto& pMeshContainer : m_SortByMaterialMesh[iMaterialIndex])
	{
		m_pDeviceContext->DrawIndexed(pMeshContainer->Get_NumFaces() * 3,
			pMeshContainer->Get_StartFaceIndex() * 3,
			pMeshContainer->Get_StartVertexIndex());
		// m_pDeviceContext->DrawIndexed(/* 인덱스 개수*/ /*인덱스버퍼의 어디에서부터그릴건지*/ /*정점버퍼의 어디에서부터 그리면되는지. */)
	}

	return S_OK;
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
	/* For.VertexBuffer */
	D3D11_BUFFER_DESC			BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

	BufferDesc.ByteWidth = sizeof(VTXMESH) * m_iNumVertices;	// 정점의 크기 * 정점의 개수
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = sizeof(VTXMESH);

	D3D11_SUBRESOURCE_DATA		SubResourceData;
	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	SubResourceData.pSysMem = m_pVertices;

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

	if (FAILED(__super::Create_Buffers()))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC		ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(__super::Compile_Shader(ElementDesc, 4, pShaderFilePath)))
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

HRESULT CModel::Create_HierarchyNodes(aiNode * pNode, CHierarchyNode * pParent, _uint iDepth)
{
	_matrix		TransformationMatrix;
	memcpy(&TransformationMatrix, &pNode->mTransformation, sizeof(_matrix));

	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pNode->mName.data, TransformationMatrix, pParent, iDepth);	// 첫 실행 시 생성되는 노드는 가장 최상위 부모 노드가 된다.
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);		// 생성한 노드를 넣어주고

	for (_uint i = 0; i < pNode->mNumChildren; ++i)		
		Create_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth + 1);	// 

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

			pBoneDesc->pHierarchyNode = Find_HierarchyNode(pBone->mName.data);
			memcpy(&pBoneDesc->OffsetMatrix, &pBone->mOffsetMatrix, sizeof(_matrix));

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
		if (strcmp(pNode->Get_Name(), pBoneName) == 0)
			return true;
	});

	return *iter;
}

HRESULT CModel::Update_CombinedTransformationMatrices(_double DeltaTime)
{
	if (m_Animations.empty())
		return E_FAIL;

	/* 현재 애니메이션을 재생하고 있는 시간을 계산하고 그 시간값에 따른 뼈의 상태행렬을 만들어서 m_TrnasformationMAtrix에 대입해놓느다. */
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

	//(*iter)->Add_Channel();
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
}
