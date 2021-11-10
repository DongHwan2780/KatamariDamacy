#include "..\Headers\Scene.h"

CScene::CScene(ID3D11Device* pDevice, ID3D11DeviceContext*	pDeviceContext)
	:m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CScene::Initialize()
{
	return S_OK;
}

_int CScene::Update(_double DeltaTime)
{
	return _int();
}

HRESULT CScene::Render()
{
	return S_OK;
}

void CScene::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
