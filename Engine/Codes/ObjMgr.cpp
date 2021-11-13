#include "..\Headers\ObjMgr.h"

IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr()
{
}

HRESULT CObjMgr::Reserve_Manager(_uint iNumScenes)
{
	//if (nullptr != m_pGameObjects)
	//	return E_FAIL;

	//m_pGameObjects = new GAMEOBJECTS[iNumScenes];		// 씬의 개수만큼 레이어배열 생성

	//m_iNumScene = iNumScenes;

	return S_OK;
}

void CObjMgr::Free()
{
	//for (_uint i = 0; i < m_iNumScene; ++i)
	//{
	//	for (auto& Pair : m_pGameObjects[i])
	//		Safe_Release(Pair.second);

	//	m_pGameObjects[i].clear();
	//}

	//Safe_Delete_Array(m_pGameObjects);
}
