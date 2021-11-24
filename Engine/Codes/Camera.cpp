#include "..\Headers\Camera.h"


CCamera::CCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CObj(pDevice, pDeviceContext)
{
}

CCamera::CCamera(const CCamera & other)
	:CObj(other)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CCamera::Initialize_Clone(void * pArg)
{
	return E_NOTIMPL;
}

_int CCamera::Update(_double DeltaTime)
{
	return _int();
}

_int CCamera::Late_Update(_double DeltaTime)
{
	return _int();
}

HRESULT CCamera::Render()
{
	return E_NOTIMPL;
}

void CCamera::Free()
{
}
