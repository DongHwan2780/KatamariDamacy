#include "..\Headers\Management.h"
#include "TimeMgr.h"
#include "Graphic.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	:m_pTimeMgr(CTimeMgr::GetInstance())
	, m_pGraphicDevice(CGraphic::GetInstance())
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pTimeMgr);
}

HRESULT CManagement::Add_Timers(const _tchar * TimeTag)
{
	if (nullptr == m_pTimeMgr)
		return E_FAIL;


	return m_pTimeMgr->Add_Timers(TimeTag);
}

_double CManagement::Compute_DeltaTime(const _tchar * TimeTag)
{
	if (nullptr == m_pTimeMgr)
		return 0.0;

	return m_pTimeMgr->Compute_DeltaTime(TimeTag);
}

HRESULT CManagement::Ready_GraphicDevice(HWND hWnd, _uint iWidth, _uint iHeight, ID3D11Device** ppOutDevice, ID3D11DeviceContext** ppOutDeviceContext)
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Ready_GraphicDevice(hWnd, iWidth, iHeight, ppOutDevice, ppOutDeviceContext);
}

HRESULT CManagement::Clear_BackBufferView(_float4 vColor)
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Clear_BackBufferView(vColor);
}

HRESULT CManagement::Clear_DepthStencilView(_float fDepth, _uint iStencil)
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Clear_DepthStencilView(fDepth, iStencil);
}

HRESULT CManagement::Present()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Present();
}

void CManagement::Release_Engine()
{
	if (0 != CManagement::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CGameInstance");

	if (0 != CTimeMgr::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CTimer_Manager");

	if (0 != CGraphic::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CGraphic");
}

void CManagement::Free()
{
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pTimeMgr);
}
