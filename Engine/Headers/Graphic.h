#pragma once

#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include "Base.h"

BEGIN(Engine)

class CGraphic final : public CBase
{
	DECLARE_SINGLETON(CGraphic)

private:
	CGraphic();
	virtual ~CGraphic() = default;

public:
	HRESULT Ready_GraphicDevice(HWND hWnd, _uint iWidth, _uint iHeight);
	HRESULT Clear_BackBufferView(_float4 vColor);
	HRESULT Clear_DepthStencilView(_float fDepth, _uint iStencil);
	HRESULT Present();

public:
	HRESULT Ready_SwapChain(HWND hWnd, _uint iWidth, _uint iHeight);
	HRESULT Ready_BackBuffer_RenderTargetView();
	HRESULT Ready_DepthStencil_RenderTargetView(_uint iWidth, _uint iHeight);

public:
	virtual void Free() override;
private:
	ID3D11Device*			m_pDevice = nullptr;		// 장치 초기화 관련 및 메모리 할당
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;	// 렌더링에 필요한 바인딩 작업 및 관련 작업 수행(버퍼를 그릴때)			//  LPDIRECT3DDEVICE9이 하던 기능을 둘이서 나눠서 함
	IDXGISwapChain*			m_pSwapChain = nullptr;
	ID3D11RenderTargetView*	m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView*	m_pDepthStencilRTV = nullptr;

};

END
#endif // !__GRAPHIC_H__
