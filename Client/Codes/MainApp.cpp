#include "stdafx.h"
#include "..\Headers\MainApp.h"

#include "Management.h"


CMainApp::CMainApp()
	:m_pManagement(CManagement::GetInstance())
{
	Safe_AddRef(m_pManagement);
}

HRESULT CMainApp::Initialize()
{
	if (m_pManagement == nullptr)
		return E_FAIL;

	if (FAILED(m_pManagement->Ready_GraphicDevice(g_hWnd, g_iWinCX, g_iWinCY)))
		return E_FAIL;

	return S_OK;
}

_uint CMainApp::Update(_float dDeltaTime)
{

	m_fDeltaTime += dDeltaTime;

	return _uint();
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
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pManagement);

	CManagement::Release_Engine();
}

