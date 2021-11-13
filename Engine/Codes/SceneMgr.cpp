#include "..\Headers\SceneMgr.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
{
}

HRESULT CSceneMgr::Set_CurScene(CScene * pCurScene)
{
	if (pCurScene == nullptr)
		return E_FAIL;

	if (Safe_Release(m_pCurScene) != 0)		// 지웠는데 레퍼카운트가 남아있으면
		return E_FAIL;

	m_pCurScene = pCurScene;

	return S_OK;
}

_int CSceneMgr::Update_Scene(_double DeltaTime)
{
	if (m_pCurScene == nullptr)
		return -1;


	return m_pCurScene->Update(DeltaTime);
}

HRESULT CSceneMgr::Render_Scene()
{
	if (m_pCurScene == nullptr)
		return E_FAIL;

	return m_pCurScene->Render();
}

void CSceneMgr::Free()
{
	Safe_Release(m_pCurScene);
}
