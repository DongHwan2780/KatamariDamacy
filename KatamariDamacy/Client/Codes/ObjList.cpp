#include "stdafx.h"
#include "..\Headers\ObjList.h"

#include "PlayerBall.h"
#include "Management.h"

#include "StickObjUI.h"

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
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (m_pColliderSphere->GetColliderDesc().eObjState == CCollider::OBJ_STICK && m_bStickCheck == true)
	{
		m_pPlayerBallTransform = dynamic_cast<CTransform*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_PlayerBall", L"Com_Transform"));

		_vector vPos = m_pTransform->Get_State(CTransform::POSITION);
		_vector vBallPos = m_pPlayerBallTransform->Get_State(CTransform::POSITION);

		OffsetMatrix = XMMatrixIdentity();

		// 포지션을 구해주고 구한 포지션에 공의 회전값만큼 공전을 해줘라

		_vector  vScale, vQuat, vTrans;
		XMMatrixDecompose(&vScale, &vQuat, &vTrans, m_pPlayerBallTransform->Get_WorldMatrix());

		// 렝스에다가 0 0 1 곱한값을 쿼터니언에 곱해줌

		OffsetMatrix = XMMatrixTranslationFromVector(vPos - vBallPos);
		OffsetMatrix = OffsetMatrix * XMMatrixRotationQuaternion(vQuat);

		m_pTransform->Set_WorldMatrix(OffsetMatrix * m_pPlayerBallTransform->Get_WorldMatrix());
		m_pTransform->Set_Scale(XMVectorSet(m_pTransform->GetTransformDesc().fScale, m_pTransform->GetTransformDesc().fScale, m_pTransform->GetTransformDesc().fScale, 0.f));

		m_bStickCheck = false;
	}
	else if (m_pColliderSphere->GetColliderDesc().eObjState == CCollider::OBJ_STICK && m_bStickCheck == false)
	{
		m_pPlayerBallTransform = dynamic_cast<CTransform*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_PlayerBall", L"Com_Transform"));

		// 오브 포지션  - 공 포지션 == 오프셋

		// 오프셋 * 스케일 뺀 공
		m_pTransform->Set_WorldMatrix(OffsetMatrix * m_pPlayerBallTransform->Get_WorldMatrix());
		m_pTransform->Set_Scale(XMVectorSet(m_pTransform->GetTransformDesc().fScale, m_pTransform->GetTransformDesc().fScale, m_pTransform->GetTransformDesc().fScale, 0.f));
	}

	RELEASE_INSTANCE(CManagement);
}

void CObjList::Create_StickObjUI(_uint iModelIndex)
{
	//CManagement*	pManagement = GET_INSTANCE(CManagement);


	//pLayer = pManagement->Get_GameObj(L"Layer_StickObjUI");

	//for (auto& iter : pLayer->GetGameObjList())
	//{
	//	if (iter != nullptr)
	//	{
	//		Safe_Release((*iter));
	//	}
	//}

	//pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("Prototype_SizeUI"), L"Layer_StickObjUI", &iModelIndex);

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
