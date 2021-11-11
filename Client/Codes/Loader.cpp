#include "stdafx.h"
#include "..\Headers\Loader.h"


CLoader::CLoader()
{
}

unsigned int APIENTRY ThreadMain(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	HRESULT			hr = 0;

	switch (pLoader->Get_NextScene())
	{
	case STAGEONE_SCENE:
		hr = pLoader->StageLoader();
		break;

		/*case LEVEL_GAMEPLAY1:
		break;*/
	}

	if (FAILED(hr))
	{
		return 0;
	}

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}

HRESULT CLoader::Initialize_Thread(SCENE eScene)
{
	InitializeCriticalSection(&m_CS);

	m_eScene = eScene;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadMain, this, 0, nullptr);

	if (m_hThread == 0)
		return E_FAIL;


	return S_OK;
}

HRESULT CLoader::StageLoader()
{
	m_bFinish = true;

	return S_OK;
}

CLoader * CLoader::Create(SCENE eScene)
{
	CLoader* pInstance = new CLoader();

	if (FAILED(pInstance->Initialize_Thread(eScene)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CS);

	DeleteObject(m_hThread);
}
