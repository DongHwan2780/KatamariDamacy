#include "..\Headers\MeshContainer.h"



CMeshContainer::CMeshContainer()
{
}

HRESULT CMeshContainer::Initialize_Clone(const char * pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex)
{
	strcpy(m_szName, pName);
	m_iNumFaces = iNumFaces;
	m_iStartFaceIndex = iStartFaceIndex;
	m_iStartVertexIndex = iStartVertexIndex;

	return S_OK;
}

CMeshContainer * CMeshContainer::Create(const char* pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex)
{
	CMeshContainer*	pInstance = new CMeshContainer();

	if (FAILED(pInstance->Initialize_Clone(pName, iNumFaces, iStartFaceIndex, iStartVertexIndex)))
	{
		MSG_BOX("Failed To Creating CMeshContainer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshContainer::Free()
{
}
