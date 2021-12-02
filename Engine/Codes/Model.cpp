#include "..\Headers\Model.h"

#include "MeshContainer.h"
#include "Texture.h"

CModel::CModel(DEVICES)
	:CVIBuffer(m_pDevice, m_pDeviceContext)
{
}

CModel::CModel(const CModel & other)
	: CVIBuffer(other)
{
}

HRESULT CModel::Initialize_Prototype(const char * pMeshFilePath, const char * pMeshFileName, const _tchar* pShaderFilePath)
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


	m_MeshContainers.reserve(m_pScene->mNumMeshes);

	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		if (FAILED(Create_MeshContainer(m_pScene->mMeshes[i], &iStartVertexIndex, &iStartFaceIndex)))
			return E_FAIL;
	}

	m_iStride = sizeof(VTXMESH);

	/* 정점버퍼, 인덱스 버퍼를 생성하낟. */
	if (FAILED(Create_AllBuffer(pShaderFilePath)))
		return E_FAIL;

	/* 현재 모델의 머테리얼들을 로드한다. */
	m_ModelTextures.reserve(m_pScene->mNumMaterials);

	for (_uint i = 0; i < m_pScene->mNumMaterials; ++i)
	{
		if (FAILED(Create_Materials(m_pScene->mMaterials[i], pMeshFilePath)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Initialize_Clone(void * pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	_uint		iOffSet = 0;

	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVB, &m_iStride, &iOffSet);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
	m_pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDeviceContext->IASetInputLayout(m_EffectDescs[iPassIndex].pLayOut);

	if (FAILED(m_EffectDescs[iPassIndex].pPass->Apply(0, m_pDeviceContext)))
		return E_FAIL;

	for (auto& pMeshContainer : m_MeshContainers)
	{
		// m_pDeviceContext->DrawIndexed(/* 인덱스카운드*/ /*인덱스버퍼의 어디에서부터그릴건지*/ /*정점버퍼의 어디에서부터 그리면되는지. */)
	}

	return S_OK;
}

HRESULT CModel::Create_MeshContainer(aiMesh * pMesh, _uint * pStartVertexIndex, _uint * pStartFaceIndex)
{
	_uint		iStartVertexIndex = *pStartVertexIndex;

	for (_uint i = 0; i < pMesh->mNumVertices; ++i)
	{
		/* 정점의 위치를 가지고 온다. */
		memcpy(&m_pVertices[*pStartVertexIndex].vPosition, &pMesh->mVertices[i], sizeof(_float3));

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

	CMeshContainer*			pMeshContainer = CMeshContainer::Create(pMesh->mName.data, pMesh->mNumFaces, iStartFaceIndex, iStartVertexIndex);
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

	BufferDesc.ByteWidth = sizeof(VTXMESH) * m_iNumVertices;
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
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

		char	szTextureFileName[MAX_PATH] = "";
		char	szExt[MAX_PATH] = "";

		_splitpath(strTexturePath.data, nullptr, nullptr, szTextureFileName, szExt);
		strcat_s(szTextureFileName, szExt);

		char		szFullPath[MAX_PATH] = "";
		strcpy_s(szFullPath, pMeshFilePath);
		strcat_s(szFullPath, szTextureFileName);

		_tchar		szWideFullPath[MAX_PATH] = TEXT("");

		MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szWideFullPath, MAX_PATH);

		if (!strcmp(szExt, ".dds"))
			pModelTexture->pModelTexture[i] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::DDS, szWideFullPath);
		else if (!strcmp(szExt, ".tga"))
			pModelTexture->pModelTexture[i] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TGA, szWideFullPath);
		else
			pModelTexture->pModelTexture[i] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::WIC, szWideFullPath);
	}

	m_ModelTextures.push_back(pModelTexture);

	return S_OK;
}

CModel * CModel::Create(DEVICES, const char * pMeshFilePath, const char * pMeshFileName, const _tchar* pShaderFilePath)
{
	CModel*	pInstance = new CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pMeshFilePath, pMeshFileName, pShaderFilePath)))
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
