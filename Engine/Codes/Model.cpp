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
