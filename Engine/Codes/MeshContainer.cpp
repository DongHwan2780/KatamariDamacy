#include "..\Headers\MeshContainer.h"
#include "HierarchyNode.h"


CMeshContainer::CMeshContainer()
{
}

void CMeshContainer::Get_BoneMatrices(_matrix * pBoneMatrices)
{
	_uint		iNumBones = m_Bones.size();

	for (_uint i = 0; i < iNumBones; ++i)
	{
		pBoneMatrices[i] = XMMatrixTranspose(XMLoadFloat4x4(&m_Bones[i]->OffsetMatrix) * m_Bones[i]->pHierarchyNode->Get_CombinedTransformationMatrix());
	}
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

HRESULT CMeshContainer::Clone_BoneDesc()
{
	vector<BONEDESC*>		OriginalBones = m_Bones;
	m_Bones.clear();

	for (auto& pOriginalBone : OriginalBones)
	{
		BONEDESC*		pBone = new BONEDESC();
		ZeroMemory(pBone, sizeof(BONEDESC));

		pBone->OffsetMatrix = pOriginalBone->OffsetMatrix;
		pBone->pName = pOriginalBone->pName;
		m_Bones.push_back(pBone);
	}

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

CMeshContainer * CMeshContainer:: Clone()
{
	CMeshContainer*	pInstance = new CMeshContainer(*this);

	if (FAILED(Clone_BoneDesc()))
	{
		Safe_Release(pInstance);
	}


	return pInstance;
}

void CMeshContainer::Free()
{
	for (auto& pBoneDesc : m_Bones)
		Safe_Delete(pBoneDesc);
	m_Bones.clear();
}
