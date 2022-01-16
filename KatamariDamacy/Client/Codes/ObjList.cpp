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


	//CTransform* m_pObjTransform = dynamic_cast<CTransform*>(pObj->GetComponent(L"Com_Transform"));

	//_vector		vRight, vUp, vLook, vPos;
	//_matrix		TransMatrix;
	//TransMatrix = XMMatrixTranslation(1.f, 1.f, 1.f);

	//vRight = m_pObjTransform->Get_State(CTransform::RIGHT);
	//vUp = m_pObjTransform->Get_State(CTransform::UP);
	//vLook = m_pObjTransform->Get_State(CTransform::LOOK);
	//vPos = m_pObjTransform->Get_State(CTransform::POSITION);

	//_matrix ScaleMatrix;
	//_float	fObjScale;

	//fObjScale = m_pObjTransform->GetTransformDesc().fScale;
	//ScaleMatrix = XMMatrixTranslation(fObjScale, fObjScale, fObjScale);

	////m_pObjTransform->Set_State(CTransform::RIGHT, XMVector4Transform(vRight, m_pTransform->Get_WorldMatrix()));
	////m_pObjTransform->Set_State(CTransform::UP, XMVector4Transform(vUp, m_pTransform->Get_WorldMatrix()));
	////m_pObjTransform->Set_State(CTransform::LOOK, XMVector4Transform(vLook, m_pTransform->Get_WorldMatrix()));


	//_matrix OffsetMatrix;
	//_vector vBallPos = m_pTransform->Get_State(CTransform::POSITION);

	//OffsetMatrix = XMMatrixTranslationFromVector(vPos - vBallPos);

	//_matrix			NonScaleMatrix = m_pTransform->Get_WorldMatrix();

	//NonScaleMatrix.r[0] = XMVector3Normalize(m_pTransform->Get_WorldMatrix().r[0]);
	//NonScaleMatrix.r[1] = XMVector3Normalize(m_pTransform->Get_WorldMatrix().r[1]);
	//NonScaleMatrix.r[2] = XMVector3Normalize(m_pTransform->Get_WorldMatrix().r[2]);

	//// 오브 포지션  - 공 포지션 == 오프셋

	//// 오프셋 * 스케일 뺀 공
	//m_pObjTransform->Set_WorldMatrix(OffsetMatrix * NonScaleMatrix);


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
