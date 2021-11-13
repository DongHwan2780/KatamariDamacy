#include "stdafx.h"
#include "..\Headers\MainApp.h"

#include "Management.h"
#include "Logo.h"
#include "Loading.h"


CMainApp::CMainApp()
	:m_pManagement(CManagement::GetInstance())
{
	Safe_AddRef(m_pManagement);
}

HRESULT CMainApp::Initialize()
{
	if (m_pManagement == nullptr)
		return E_FAIL;

	if (FAILED(m_pManagement->Ready_GraphicDevice(g_hWnd, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;

	if (FAILED(Ready_Scene(STAGEONE_SCENE)))
		return E_FAIL;

	return S_OK;
}

_uint CMainApp::Update(_double dDeltaTime)
{

	m_fDeltaTime += dDeltaTime;

	m_pManagement->Update_Scene(dDeltaTime);

	return 0;
}

_uint CMainApp::Late_Update()
{
	return _uint();
}

HRESULT CMainApp::Render()
{
	m_pManagement->Clear_BackBufferView(_float4(0.f, 0.f, 1.f, 1.f));
	m_pManagement->Clear_DepthStencilView(1.f, 0);

	m_pManagement->Present();

#ifdef _DEBUG
	++m_iNumDraw;

	if (m_fDeltaTime >= 1.0)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumDraw);
		SetWindowText(g_hWnd, m_szFPS);

		m_fDeltaTime = 0.0;
		m_iNumDraw = 0;
	}
#endif

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);

	CManagement::Release_Engine();
}

HRESULT CMainApp::Ready_Scene(SCENE eScene)
{
	CScene*		pScene = nullptr;

	switch (eScene)
	{
	case LOGO_SCENE:
		pScene = CLogo::Create(m_pDevice, m_pDeviceContext);
		break;
	case STAGEONE_SCENE:
		pScene = CLoading::Create(m_pDevice, m_pDeviceContext, eScene);		// 로딩씬에서 옵젝생성하고 스테이지로 넘어가줄거임 == 로딩이후에 넘어갈 스테이지 enumID값을 인자로 넘겨줘서
		break;
	}

	if (pScene == nullptr)
		return E_FAIL;

	if (FAILED(m_pManagement->Set_CurScene(pScene)))
		return E_FAIL;

	return S_OK;
}

