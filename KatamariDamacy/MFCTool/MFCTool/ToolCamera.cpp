#include "stdafx.h"
#include "ToolCamera.h"



CToolCamera::CToolCamera(DEVICES)
	:CCamera(pDevice, pDeviceContext)
{
}

CToolCamera::CToolCamera(const CToolCamera & other)
	:CCamera(other)
{
}

HRESULT CToolCamera::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CToolCamera::Initialize_Clone(void * pArg)
{
	__super::Initialize_Clone(pArg);

	return S_OK;
}

_int CToolCamera::Update(_double DeltaTime)
{
	__super::Update(DeltaTime);


	return _int();
}

_int CToolCamera::Late_Update(_double DeltaTime)
{
	__super::Late_Update(DeltaTime);

	return _int();
}

HRESULT CToolCamera::Render()
{
	return S_OK;
}

CToolCamera * CToolCamera::Create(DEVICES)
{
	CToolCamera* pInstance = new CToolCamera(pDevice, pDeviceContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CToolCamera::Clone(void * pArg)
{
	CToolCamera* pInstance = new CToolCamera(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CToolCamera::Free()
{
	__super::Free();
}
