#include "..\Headers\MeshContainer.h"



CMeshContainer::CMeshContainer()
{
}

HRESULT CMeshContainer::Initialize_Clone(const char * pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex, _uint iMaterialIndex)
{
	strcpy_s(m_szName, pName);
	m_iNumFaces = iNumFaces;
	m_iStartFaceIndex = iStartFaceIndex;
	m_iStartVertexIndex = iStartVertexIndex;
	m_iMaterialIndex = iMaterialIndex;

	return S_OK;
}

HRESULT CMeshContainer::Add_Bones(BONEDESC * pBoneDesc)
{
	m_Bones.push_back(pBoneDesc);

	return S_OK;
}

CMeshContainer * CMeshContainer::Create(const char* pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex, _uint iMaterialIndex)
{
	CMeshContainer*	pInstance = new CMeshContainer();

	if (FAILED(pInstance->Initialize_Clone(pName, iNumFaces, iStartFaceIndex, iStartVertexIndex, iMaterialIndex)))
	{
		MSG_BOX("Failed To Creating CMeshContainer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshContainer::Free()
{
}
