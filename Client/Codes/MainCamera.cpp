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

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	m_pPlayerTransform = dynamic_cast<CTransform*>(pManagement->GetComponent(TEXT("Layer_Player"), TEXT("Com_Transform"), STAGEONE_SCENE));

	//Safe_AddRef(m_pPlayerTransform);

	RELEASE_INSTANCE(CManagement);
	return S_OK;
}

_int CMainCamera::Update(_double DeltaTime)
{
	if (nullptr == m_pTransform)
		return -1;

	POINT		ptMouse = { g_iWinCX >> 1, g_iWinCY >> 1 };

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (pManagement->Get_DIKState(DIK_W) & 0x80)
	{
		m_pTransform->Move_Straight(DeltaTime);
	}

	if (pManagement->Get_DIKState(DIK_S) & 0x80)
	{
		m_pTransform->Move_Straight(DeltaTime * -1.0);
	}

	if (pManagement->Get_DIKState(DIK_A) & 0x80)
	{
		m_pTransform->Move_Strafe(DeltaTime * -1.0);
	}

	if (pManagement->Get_DIKState(DIK_D) & 0x80)
	{
		m_pTransform->Move_Strafe(DeltaTime);
	}

	_long			MouseMove = 0;

	if (MouseMove = pManagement->Get_MouseMoveState(CInput::MOUSE_MOVEX))
	{
		m_pTransform->RotateAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), DeltaTime * MouseMove * 2.f);
	}

	if (MouseMove = pManagement->Get_MouseMoveState(CInput::MOUSE_MOVEY))
	{
		m_pTransform->RotateAxis(m_pTransform->Get_State(CTransform::RIGHT), DeltaTime * MouseMove * 2.0f);
	}

	RELEASE_INSTANCE(CManagement);

	Movement(DeltaTime);

	return __super::Update(DeltaTime);
}

_int CMainCamera::Late_Update(_double DeltaTime)
{
	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return _uint();
}

HRESULT CMainCamera::Render()
{
	return S_OK;
}

HRESULT CMainCamera::SetUp_Components()
{
	return S_OK;
}

_uint CMainCamera::Movement(_double DeltaTime)
{
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::POSITION);

	_vector vTargetLook = m_pPlayerTransform->Get_State(CTransform::LOOK);
	XMVector3Normalize(vTargetLook);

	_vector vInTargetLook = vTargetLook * -m_fDistanceToTarget;

	XMStoreFloat3(&m_CameraDesc.vEye,  vTargetPos + vInTargetLook);
	XMStoreFloat3(&m_CameraDesc.vAt,  vTargetPos);

	return _uint();
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
