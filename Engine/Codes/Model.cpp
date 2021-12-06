#include "..\Headers\Model.h"

#include "MeshContainer.h"
#include "Texture.h"

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

HRESULT CModel::Initialize_Prototype(const char * pMeshFilePath, const char * pMeshFileName, const _tchar* pShaderFilePath)
{
	char		szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pMeshFilePath);
	strcat_s(szFullPath, pMeshFileName);

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
		if (FAILED(Create_MeshContainer(m_pScene->mMeshes[i], &iStartVertexIndex, &iStartFaceIndex)))	// �޽��� ������ŭ �����̳� �����ϰ� �޽�, ���������ε���, ����������ε����� �Ѱ���
			return E_FAIL;
	}

	m_iStride = sizeof(VTXMESH);		// ���� ����ü�� ũ��

	// ���ؽ�, �ε��� ���� ����
	if (FAILED(Create_AllBuffer(pShaderFilePath)))
		return E_FAIL;

	/* ���� ���� ���׸������ �ε��Ѵ�. */
	m_ModelTextures.reserve(m_pScene->mNumMaterials);

	for (_uint i = 0; i < m_pScene->mNumMaterials; ++i)
	{
		if (FAILED(Create_Materials(m_pScene->mMaterials[i], pMeshFilePath)))
			return E_FAIL;
	}

	/* �޽������̳ʵ��� ���׸��� �������� ���� �����Ѵ�. */
	if (FAILED(Sort_MeshesByMaterial()))
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

	for (auto& pMeshContainer : m_SortByMaterialMesh[iMaterialIndex])
	{
		m_pDeviceContext->DrawIndexed(pMeshContainer->Get_NumFaces() * 3,
			pMeshContainer->Get_StartFaceIndex() * 3,
			pMeshContainer->Get_StartVertexIndex());
		// m_pDeviceContext->DrawIndexed(/* �ε��� ����*/ /*�ε��������� ��𿡼����ͱ׸�����*/ /*���������� ��𿡼����� �׸���Ǵ���. */)
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

HRESULT CModel::Create_MeshContainer(aiMesh * pMesh, _uint * pStartVertexIndex, _uint * pStartFaceIndex)
{
	_uint		iStartVertexIndex = *pStartVertexIndex;

	for (_uint i = 0; i < pMesh->mNumVertices; ++i)
	{
		/* ������ ��ġ�� ������ �´�. */
		memcpy(&m_pVertices[*pStartVertexIndex].vPosition, &pMesh->mVertices[i], sizeof(_float3));

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
	/* For.VertexBuffer */
	D3D11_BUFFER_DESC			BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

	BufferDesc.ByteWidth = sizeof(VTXMESH) * m_iNumVertices;	// ������ ũ�� * ������ ����
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

	m_SortByMaterialMesh.resize(iNumMaterials);	// ���׸����� ������ŭ 

	for (auto& pMeshContainer : m_MeshContainers)
	{
		_uint	iMeshMaterialIndex = pMeshContainer->Get_MeshMaterialIndex();
		if (iMeshMaterialIndex >= m_pScene->mNumMaterials)
			return E_FAIL;

		m_SortByMaterialMesh[iMeshMaterialIndex].push_back(pMeshContainer);
		Safe_AddRef(pMeshContainer);
	}
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
