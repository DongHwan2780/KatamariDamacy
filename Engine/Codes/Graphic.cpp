#include "..\Headers\Graphic.h"

IMPLEMENT_SINGLETON(CGraphic)

CGraphic::CGraphic()
{
}

HRESULT CGraphic::Ready_GraphicDevice(HWND hWnd, _uint iWidth, _uint iHeight, ID3D11Device** ppOutDevice, ID3D11DeviceContext** ppOutDeviceContext)
{
	_uint iDebug = 0;

#ifdef _DEBUG
	iDebug = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	D3D_FEATURE_LEVEL	Feature_Level;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, iDebug, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &Feature_Level, &m_pDeviceContext)))
		return E_FAIL;

	if (FAILED(Ready_SwapChain(hWnd, iWidth, iHeight)))
		return E_FAIL;

	if (FAILED(Ready_BackBuffer_RenderTargetView()))
		return E_FAIL;

	if (FAILED(Ready_DepthStencil_RenderTargetView(iWidth, iHeight)))
		return E_FAIL;

	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBufferRTV, m_pDepthStencilRTV);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	ViewPortDesc.Width = (float)iWidth;
	ViewPortDesc.Height = (float)iHeight;
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.MinDepth = 0.0f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pDeviceContext->RSSetViewports(1, &ViewPortDesc);

	*ppOutDevice = m_pDevice;
	*ppOutDeviceContext = m_pDeviceContext;

	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);

	return S_OK;
}

HRESULT CGraphic::Clear_BackBufferView(_float4 vColor)
{
	if (nullptr == m_pBackBufferRTV)
		return E_FAIL;

	m_pDeviceContext->ClearRenderTargetView(m_pBackBufferRTV, (_float*)&vColor);

	return S_OK;
}

HRESULT CGraphic::Clear_DepthStencilView(_float fDepth, _uint iStencil)
{
	if (nullptr == m_pDepthStencilRTV)
		return E_FAIL;

	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilRTV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, fDepth, iStencil);

	return S_OK;
}

HRESULT CGraphic::Ready_SwapChain(HWND hWnd, _uint iWidth, _uint iHeight)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	IDXGIFactory*			pDXGIFactory = nullptr;
	IDXGIAdapter*			pDXGIAdapter = nullptr;
	IDXGIDevice*			pDXGIDevice = nullptr;

	if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice)))
		return E_FAIL;
	if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter)))
		return E_FAIL;
	if (FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDXGIFactory)))
		return E_FAIL;

	DXGI_SWAP_CHAIN_DESC			SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	SwapChainDesc.BufferDesc.Width = iWidth;
	SwapChainDesc.BufferDesc.Height = iHeight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.SampleDesc.Count = 1;

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = hWnd;
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(pDXGIFactory->CreateSwapChain(m_pDevice, &SwapChainDesc, &m_pSwapChain)))	// CreateSwapChain 함수를 쓰기 위해서는 IDXGIDevice, IDXGIAdapter 변수를 만들어서 정보를 받아오는 과정이 필요.
		return E_FAIL;

	Safe_Release(pDXGIFactory);
	Safe_Release(pDXGIAdapter);
	Safe_Release(pDXGIDevice);

	return S_OK;
}

HRESULT CGraphic::Ready_BackBuffer_RenderTargetView()
{
	ID3D11Texture2D*			pBackBufferTexture = nullptr;

	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);

	if (FAILED(m_pDevice->CreateRenderTargetView(pBackBufferTexture, nullptr, &m_pBackBufferRTV)))
		return E_FAIL;

	Safe_Release(pBackBufferTexture);

	return S_OK;
}

HRESULT CGraphic::Ready_DepthStencil_RenderTargetView(_uint iWidth, _uint iHeight)
{
	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(TextureDesc));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;

	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	ID3D11Texture2D*		pDepthStencilTexture = nullptr;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDepthStencilRTV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;

}

HRESULT CGraphic::Present()
{
	if (nullptr == m_pSwapChain)
		return E_FAIL;

	m_pSwapChain->Present(0, 0);

	return S_OK;
}

void CGraphic::Free()
{
	Safe_Release(m_pDepthStencilRTV);
	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pSwapChain);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
