#include "stdafx.h"
#include "..\Headers\MainCamera.h"

#include "Management.h"

CMainCamera::CMainCamera(DEVICES)
	:CCamera(pDevice, pDeviceContext)
{
}

CMainCamera::CMainCamera(const CMainCamera & other)
	:CCamera(other)
{
}

HRESULT CMainCamera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainCamera::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CMainCamera::Update(_double DeltaTime)
{
	return _int();
}

_int CMainCamera::Late_Update(_double DeltaTime)
{
	return _int();
}

HRESULT CMainCamera::Render()
{
	return S_OK;
}

HRESULT CMainCamera::SetUp_Components()
{
	return S_OK;
}

CMainCamera * CMainCamera::Create(DEVICES)
{
	CMainCamera*	pInstance = new CMainCamera(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CMainCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CMainCamera::Clone(void * pArg)
{
	CMainCamera*	pInstance = new CMainCamera(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CMainCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainCamera::Free()
{
	__super::Free();
}
