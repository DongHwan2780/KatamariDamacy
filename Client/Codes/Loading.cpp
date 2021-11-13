#include "stdafx.h"
#include "..\Headers\Loading.h"

#include "Management.h"
#include "Loader.h"
#include "StageOne.h"

CLoading::CLoading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}

HRESULT CLoading::Initialize(SCENE eScene)
{
	__super::Initialize();

	m_pLoader = CLoader::Create(eScene);

	if (m_pLoader == nullptr)
		return E_FAIL;

	m_eNextScene = eScene;

	return S_OK;
}

_int CLoading::Update(_double DeltaTime)
{
	__super::Update(DeltaTime);

	if (m_pLoader->Get_Finish())	// 로딩이 끝나면 == 쓰레드가 다 돌면
	{
		CManagement*		pManagement = GET_INSTANCE(CManagement);

		SetWindowText(g_hWnd, TEXT("로딩 완료"));		// 텍스트 출력하고

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)	// 로딩완료 상태에서 키를 누르면
		{
			CScene*	pScene = nullptr;

			switch (m_eNextScene)		// 로딩씬 이니셜라이즈에서 받아온 enumID값에 따라 다음씬으로 이동시켜줌
			{
			case STAGEONE_SCENE:
				pScene = CStageOne::Create(m_pDevice, m_pDeviceContext);
				break;
			}

			if (FAILED(pManagement->Set_CurScene(pScene)))
				return E_FAIL;
		}

		RELEASE_INSTANCE(CManagement);
	}

	return _int();
}

HRESULT CLoading::Render()
{
	__super::Render();

	return S_OK;
}

CLoading * CLoading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, SCENE eScene)
{
	CLoading*	pInstance = new CLoading(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize(eScene)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading::Free()
{
	__super::Free();
}
