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
	ID3D11Device*			m_pDevice = nullptr;		// ��ġ �ʱ�ȭ ���� �� �޸� �Ҵ�
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;	// �������� �ʿ��� ���ε� �۾� �� ���� �۾� ����(���۸� �׸���)			//  LPDIRECT3DDEVICE9�� �ϴ� ����� ���̼� ������ ��
	IDXGISwapChain*			m_pSwapChain = nullptr;
	ID3D11RenderTargetView*	m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView*	m_pDepthStencilRTV = nullptr;

};

END
#endif // !__GRAPHIC_H__
