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

	m_pPlayerTransform = dynamic_cast<CTransform*>(pManagement->GetComponent(STAGEONE_SCENE, TEXT("Layer_Player"), TEXT("Com_Transform")));

	//Safe_AddRef(m_pPlayerTransform);

	RELEASE_INSTANCE(CManagement);


	return S_OK;
}

_int CMainCamera::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	Movement(DeltaTime);

	return _int();
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

	_vector	vLook = m_pPlayerTransform->Get_State(CTransform::LOOK);
	m_pTransform->Set_State(CTransform::LOOK, XMVector3Normalize(vLook));

	_vector vInTargetLook = vLook * -m_fDistanceToTarget;

	_vector vUp = XMVectorSet(0.f, 5.f, 0.f, 0.f);

	_vector vRight = m_pTransform->Get_State(CTransform::RIGHT);
	_matrix	matRot = XMMatrixRotationAxis(vRight, m_fCamAngle);

	XMStoreFloat3(&m_CameraDesc.vEye, vTargetPos + vInTargetLook + vUp);
	XMStoreFloat3(&m_CameraDesc.vAt, vTargetPos);

	vLook = XMLoadFloat3(&m_CameraDesc.vAt) - XMLoadFloat3(&m_CameraDesc.vEye);
	m_pTransform->Set_State(CTransform::LOOK, XMVector3Normalize(vLook));

	vRight = XMVector3Cross(XMLoadFloat3(&m_CameraDesc.vAxisY), vLook);
	m_pTransform->Set_State(CTransform::RIGHT, XMVector3Normalize(vRight));

	vUp = XMVector3Cross(vLook, vRight);
	m_pTransform->Set_State(CTransform::UP, XMVector3Normalize(vUp));

	m_pTransform->Set_State(CTransform::POSITION, XMLoadFloat3(&m_CameraDesc.vEye));

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
