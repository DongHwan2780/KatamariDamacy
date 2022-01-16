#include "..\Headers\Transform.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CTransform::CTransform(const CTransform & other)
	:CComponent(other)
	, m_MatWorld(other.m_MatWorld)
	, m_TransformDesc(other.m_TransformDesc)
{
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_MatWorld, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize_Clone(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

void CTransform::Set_Scale(_fvector vScale)
{
	_vector		vRight = XMVector3Normalize(Get_State(CTransform::RIGHT)) * XMVectorGetX(vScale);
	_vector		vUp = XMVector3Normalize(Get_State(CTransform::UP)) * XMVectorGetY(vScale);
	_vector		vLook = XMVector3Normalize(Get_State(CTransform::LOOK)) * XMVectorGetZ(vScale);

	Set_State(CTransform::RIGHT, vRight);
	Set_State(CTransform::UP, vUp);
	Set_State(CTransform::LOOK, vLook);
}

void CTransform::Move_Straight(_double DeltaTime, class CNavigation* pNavigation)
{
	_vector vLook = Get_State(CTransform::LOOK);
	_vector vPos = Get_State(CTransform::POSITION);

	vPos += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * _float(DeltaTime);

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::POSITION, vPos);
		return;
	}

	if (true == pNavigation->isMove(vPos))
		Set_State(CTransform::POSITION, vPos);
	else
	{

	}

}

void CTransform::Move_Strafe(_double DeltaTime, class CNavigation* pNavigation)
{
	_vector vRight = Get_State(CTransform::RIGHT);
	_vector vPos = Get_State(CTransform::POSITION);

	vPos += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * _float(DeltaTime);

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::POSITION, vPos);
		return;
	}

	if (true == pNavigation->isMove(vPos))
		Set_State(CTransform::POSITION, vPos);
	else
	{

	}
}

void CTransform::Chase_Target(CTransform * pTarget)
{
}

void CTransform::Chase_Target(_fvector vTargetPos, _double DeltaTime)
{
	_vector		vPos = Get_State(CTransform::POSITION);

	_vector		vDirection = vTargetPos - vPos;

	vPos += XMVector3Normalize(vDirection) * m_TransformDesc.fSpeedPerSec * (_float)DeltaTime;

	Set_State(CTransform::POSITION, vPos);

	LookAtLandObj(vTargetPos);
}

void CTransform::LookAtLandObj(_fvector vTargetPos)
{
	_vector		vPos = Get_State(CTransform::POSITION);
	_vector		vUp = Get_State(CTransform::UP);

	_vector		vDirection = vTargetPos - vPos;

	_vector		vRight = XMVector3Cross(vUp, vDirection);

	vRight = XMVector3Normalize(vRight) * Get_Scale(CTransform::RIGHT);
	_vector		vLook = XMVector3Cross(vRight, vUp);
	vLook = XMVector3Normalize(vLook) * Get_Scale(CTransform::LOOK);

	Set_State(CTransform::RIGHT, vRight);
	Set_State(CTransform::LOOK, vLook);
}

void CTransform::LookAt(_fvector vTargetPos)
{
}

void CTransform::SetUp_Rotate(_fvector vAxis, _float fDegree)
{
	_vector		vRight, vUp, vLook;

	vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scale(CTransform::RIGHT);		// XMVectorSet == 벡터값 초기화할때 사용
	vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scale(CTransform::UP);
	vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scale(CTransform::LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(fDegree));	// 회전축(vAxis)과 회전값(fDegree)를 받아서 회전축으로 회전값만큼 고정 회전시킨다.

	Set_State(CTransform::RIGHT, XMVector4Transform(vRight, RotationMatrix));
	Set_State(CTransform::UP, XMVector4Transform(vUp, RotationMatrix));
	Set_State(CTransform::LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void CTransform::RotateAxis(_fvector vAxis, _double DeltaTime)
{
	_vector		vRight, vUp, vLook;

	vRight = Get_State(CTransform::RIGHT);
	vUp = Get_State(CTransform::UP);
	vLook = Get_State(CTransform::LOOK);

	_matrix  RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(m_TransformDesc.fRotatePerSec) * (_float)DeltaTime);

	Set_State(CTransform::RIGHT, XMVector4Transform(vRight, RotationMatrix));
	Set_State(CTransform::UP, XMVector4Transform(vUp, RotationMatrix));
	Set_State(CTransform::LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void CTransform::MoveToDir(_vector Look, _float fSpeed, _double DeltaTime)
{
	_vector vLook = Get_State(CTransform::LOOK);
	_vector vPos = Get_State(CTransform::POSITION);

	vPos += XMVector3Normalize(Look) * m_TransformDesc.fSpeedPerSec * _double(DeltaTime) * fSpeed;

	Set_State(CTransform::POSITION, vPos);
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTransform*	pInstance = new CTransform(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed To Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
