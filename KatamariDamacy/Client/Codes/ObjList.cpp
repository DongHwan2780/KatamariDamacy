#include "stdafx.h"
#include "..\Headers\ObjList.h"

#include "PlayerBall.h"
#include "Management.h"

CObjList::CObjList(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CObjList::CObjList(const CObjList & other)
	:CObj(other)
{
}

HRESULT CObjList::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CObjList::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CObjList::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	return _int();
}

_int CObjList::Late_Update(_double DeltaTime)
{
	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return _int();
}

HRESULT CObjList::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CObjList::Gravity(_double DeltaTime)
{
}

void CObjList::OnLandCheck()
{
}

void CObjList::Coll_PlayerBall()
{
	//CManagement*	pManagement = GET_INSTANCE(CManagement);


	//if (m_pColliderSphere->GetColliderDesc().eObjState == CCollider::OBJ_NONE)
	//{
	//	CCollider*	pTargetCollider = dynamic_cast<CCollider*>(pManagement->GetComponent(STAGEONE_SCENE, TEXT("Layer_PlayerBall"), TEXT("Com_SPHERE")));

	//	if (m_pColliderSphere->Collision_Sphere(pTargetCollider))
	//	{
	//		m_pPlayerBallTransform = dynamic_cast<CTransform*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_PlayerBall", L"Com_Transform"));

	//		_vector		vRight, vUp, vLook;
	//		_matrix		TransMatrix;
	//		TransMatrix = XMMatrixTranslation(0.f, 0.f, 2.f);

	//		vRight = m_pTransform->Get_State(CTransform::RIGHT);
	//		vUp = m_pTransform->Get_State(CTransform::UP);
	//		vLook = m_pTransform->Get_State(CTransform::LOOK);

	//		m_pTransform->Set_WorldMatrix(m_pPlayerBallTransform->Get_WorldMatrix() * TransMatrix);

	//		m_pTransform->Set_State(CTransform::RIGHT, XMVector4Transform(vRight, m_pPlayerBallTransform->Get_WorldMatrix()));
	//		m_pTransform->Set_State(CTransform::UP, XMVector4Transform(vUp, m_pPlayerBallTransform->Get_WorldMatrix()));
	//		m_pTransform->Set_State(CTransform::LOOK, XMVector4Transform(vLook, m_pPlayerBallTransform->Get_WorldMatrix()));
	//	}
	//}


	//RELEASE_INSTANCE(CManagement);
}

CObj * CObjList::Clone(void * pArg)
{
	return nullptr;
}

void CObjList::Free()
{
	__super::Free();
}
