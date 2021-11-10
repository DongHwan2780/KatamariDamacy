#include "stdafx.h"
#include "..\Headers\Loading.h"


CLoading::CLoading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}

HRESULT CLoading::Initialize()
{
	__super::Initialize();
	return S_OK;
}

_int CLoading::Update(_double DeltaTime)
{
	__super::Update(DeltaTime);
	return _int();
}

HRESULT CLoading::Render()
{
	__super::Render();

	return S_OK;
}

CLoading * CLoading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLoading*	pInstance = new CLoading(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading::Free()
{
	__super::Free();
}
