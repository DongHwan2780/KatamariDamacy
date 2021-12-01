#include "..\Headers\Model.h"


CModel::CModel(DEVICES)
	:CComponent(m_pDevice, m_pDeviceContext)
{
}

CModel::CModel(const CModel & other)
	:CComponent(other)
{
}

HRESULT CModel::Initialize_Prototype(const char * pMeshFilePath, const char * pMeshFileName)
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


	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		if (FAILED(Create_MeshContainer(m_pScene->mMeshes[i], &iStartVertexIndex, &iStartFaceIndex)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Initialize_Clone(void * pArg)
{
	return E_NOTIMPL;
}

HRESULT CModel::Create_MeshContainer(aiMesh * pMesh, _uint * pStartVertexIndex, _uint * pStartFaceIndex)
{
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

	for (_uint i = 0; i < pMesh->mNumFaces; ++i)
	{
		m_pPolygonIndices32[*pStartFaceIndex]._0 = pMesh->mFaces[i].mIndices[0];
		m_pPolygonIndices32[*pStartFaceIndex]._1 = pMesh->mFaces[i].mIndices[1];
		m_pPolygonIndices32[*pStartFaceIndex]._2 = pMesh->mFaces[i].mIndices[2];
		++(*pStartFaceIndex);
	}

	return S_OK;
}

CModel * CModel::Create(DEVICES, const char * pMeshFilePath, const char * pMeshFileName)
{
	CModel*	pInstance = new CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pMeshFilePath, pMeshFileName)))
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
