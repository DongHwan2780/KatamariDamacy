#include "stdafx.h"
#include "..\Headers\Logo.h"


CLogo::CLogo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}


HRESULT CLogo::Initialize()
{
	return S_OK;
}

_int CLogo::Update(_double DeltaTime)
{
	return _int();
}

HRESULT CLogo::Render()
{
	return S_OK;
}

CLogo * CLogo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLogo*	pInstance = new CLogo(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLogo::Free()
{
}
