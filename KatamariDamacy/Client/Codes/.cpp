#include "stdafx.h"
#include "ToolCamera.h"

#include "Model.h"
#include "Management.h"
#include "ClientDefines.h"

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
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolCamera::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CToolCamera::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	Movement(DeltaTime);

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

void CToolCamera::Movement(_double DeltaTime)
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_pTransform->Move_Straight(DeltaTime);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pTransform->Move_Straight(-DeltaTime);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pTransform->Move_Strafe(-DeltaTime);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pTransform->Move_Strafe(DeltaTime);
	}
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		m_pTransform->RotateAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), DeltaTime * -2.f);
	}

	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_pTransform->RotateAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), DeltaTime * 2.f);
	}
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		m_pTransform->RotateAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), DeltaTime * -2.f);
	}

	if (GetAsyncKeyState('C') & 0x8000)
	{
		m_pTransform->RotateAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), DeltaTime * 2.f);
	}
}

CToolCamera * CToolCamera::Create(DEVICES)
{
	CToolCamera* pInstance = new CToolCamera(pDevice, pDeviceContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating ToolCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CToolCamera::Clone(void * pArg)
{
	CToolCamera* pInstance = new CToolCamera(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating ToolCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CToolCamera::Free()
{
	__super::Free();
}
