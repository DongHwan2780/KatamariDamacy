#include "stdafx.h"
#include "..\Headers\PlayerBall.h"

#include "Management.h"

CPlayerBall::CPlayerBall(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CPlayerBall::CPlayerBall(const CPlayerBall & other)
	:CObj(other)
{
}

HRESULT CPlayerBall::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerBall::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CManagement*	pManagement = GET_INSTANCE(CManagement);
	m_pVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_Terrain", L"Com_VIBuffer"));
	RELEASE_INSTANCE(CManagement);

	m_fBallSize = 100.f;
	return S_OK;
}

_int CPlayerBall::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	CManagement*	pManagement = GET_INSTANCE(CManagement);
	m_pPlayerTransform = dynamic_cast<CTransform*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_Player", L"Com_Transform"));

	RELEASE_INSTANCE(CManagement);

	Gravity(DeltaTime);

	if (m_bSyncCheck)
	{
		SetTransform();
	}
	m_pCollider->Update_State(m_pTransform->Get_WorldMatrix());
	m_pColliderSphere->Update_State(m_pTransform->Get_WorldMatrix());

	m_fCollSize = m_fBallSize;
	return _int();
}

_int CPlayerBall::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	CObj*	pObj = nullptr;
	_float3 vPos;



	if (m_pColliderSphere->Collision_Sphere(this, L"Layer_StageObj", pObj, vPos))
	{
		if (pObj->Get_ObjCollSize() >= m_fBallSize)
		{

			_vector vPrePos = m_pPlayerTransform->Get_State(CTransform::POSITION);
			m_pPlayerTransform->Set_State(CTransform::POSITION, vPrePos);

			_vector ReturnDir;
			ReturnDir = m_pPlayerTransform->Get_State(CTransform::LOOK);

			m_pPlayerTransform->Set_TransformDescSpeed(m_pPlayerTransform->GetTransformDesc().fSpeedPerSec * -0.3f);
			m_pPlayerTransform->MoveToDir(-ReturnDir, m_pPlayerTransform->GetTransformDesc().fSpeedPerSec * 2.f, DeltaTime);


			CManagement*		pManagement = GET_INSTANCE(CManagement);
			pManagement->StopSound(CSoundMgr::SOUNDCHANNEL::COLL);
			pManagement->PlaySounds(L"Coll.wav", CSoundMgr::SOUNDCHANNEL::COLL);
			RELEASE_INSTANCE(CManagement);

		}
		else
		{ 
			this->Add_PlayerBallSize(pObj->Get_ObjCollSize() / 20.f);

			CCollider* m_pObjCollider = dynamic_cast<CCollider*>(pObj->GetComponent(L"Com_SPHERE"));
			CModel*		pModel = dynamic_cast<CModel*>(pObj->GetComponent(L"Com_Model"));

			m_pObjCollider->Set_CollState(CCollider::OBJ_STICK);
			m_pObjCollider->Set_CollPos(vPos);

			CModel*	pUIModel = dynamic_cast<CModel*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_StickObjUI", L"Com_Model"));

			pUIModel->SetModelIndex(pModel->GetModelDesc().iModelIndexNum);
			pUIModel->SetModelScale(pModel->GetModelDesc().fModelScale);


			_float3 vScale;
			vScale = { ((m_fBallSize / 100.f)  / 100.f),  ((m_fBallSize / 100.f) / 100.f),  ((m_fBallSize / 100.f)/ 100.f) };		// 큐브콜라이더 사이즈 조정
			m_pCollider->Set_Points(vScale);
			m_pColliderSphere->Set_Radius(this, (m_fBallSize / 100.f) / 200.f);		// 구 콜라이더 사이즈 조정
			m_StickList.emplace_back(pObj);

			CManagement*		pManagement = GET_INSTANCE(CManagement);
			pManagement->StopSound(CSoundMgr::SOUNDCHANNEL::STICK);
			pManagement->PlaySounds(L"Stick.wav", CSoundMgr::SOUNDCHANNEL::STICK);
			RELEASE_INSTANCE(CManagement);
		}

	}


	m_pRenderer->Add_RenderGroup(CRenderer::NONALPHA, this);

	RELEASE_INSTANCE(CManagement);

	return 0;
}

HRESULT CPlayerBall::Render()
{
	if (nullptr == m_pModel)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint		iNumMaterials = (_uint)m_pModel->Get_NumMaterials();

	m_pModel->Bind_Buffers();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModel->Render(i, 0);
	}

#ifdef _DEBUG
	m_pCollider->Render();
	m_pColliderSphere->Render();
#endif

	return S_OK;
}

void CPlayerBall::Gravity(_double DeltaTime)
{
	_vector vPosition = m_pTransform->Get_State(CTransform::POSITION);
	_float3 fPos;
	XMStoreFloat3(&fPos, vPosition);

	if (m_pVIBuffer->IsGround(fPos)) // 바닥 충돌
	{
		m_fGravityTime = 0.f;
		m_fGravityY = fPos.y;
		m_pTransform->Set_State(CTransform::POSITION, XMVectorSet(fPos.x, fPos.y, fPos.z, 1.f));
	}
	else// 중력 적용
	{
		fPos.y = m_fGravityY - 9.8f * m_fGravityTime * m_fGravityTime;
		m_pTransform->Set_State(CTransform::POSITION, XMVectorSet(fPos.x, fPos.y, fPos.z, 1.f));
		m_fGravityTime += (float)DeltaTime;
	}
}

HRESULT CPlayerBall::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_PlayerBall", L"Com_Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	/* For.Com_Transform */

	CTransform::TRANSFORMDESC	TransformDesc;
	TransformDesc.fRotatePerSec = 180.0f;

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);

	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Collider_OBB", L"Com_OBB", (CComponent**)&m_pCollider, &ColliderDesc)))
		return E_FAIL;


	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);

	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Collider_SPHERE", L"Com_SPHERE", (CComponent**)&m_pColliderSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerBall::SetUp_ConstantTable()
{
	if (nullptr == m_pModel)
		return E_FAIL;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(m_pTransform->Get_WorldMatrix()), sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(pManagement->Get_Transform(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(pManagement->Get_Transform(CPipeLine::D3DTS_PROJ)), sizeof(_matrix));

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

void CPlayerBall::SetTransform()
{
	_vector		vRight, vUp, vLook;
	vRight = m_pTransform->Get_State(CTransform::RIGHT);
	vUp = m_pTransform->Get_State(CTransform::UP);
	vLook = m_pTransform->Get_State(CTransform::LOOK);

	_matrix			NonRotateMatrix = XMMatrixIdentity();
	NonRotateMatrix.r[3] = m_pPlayerTransform->Get_WorldMatrix().r[3];

	m_pTransform->Set_State(CTransform::RIGHT, XMVector4Transform(vRight, NonRotateMatrix));
	m_pTransform->Set_State(CTransform::UP, XMVector4Transform(vUp, NonRotateMatrix));
	m_pTransform->Set_State(CTransform::LOOK, XMVector4Transform(vLook, NonRotateMatrix));

	////////// 포지션 보정
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::POSITION);

	_vector	vPlayerLook = m_pPlayerTransform->Get_State(CTransform::LOOK);
	_vector vInTargetLook = vPlayerLook * 3.f;

	_float3 vSyncPos;

	_vector vSyncYPos;
	vSyncYPos = XMVectorSet(0.f, (m_fBallSize / 100.f) / 4.f, 0.f, 1.f);

	XMStoreFloat3(&vSyncPos, vTargetPos + vInTargetLook + vSyncYPos);
	m_pTransform->Set_State(CTransform::POSITION, XMVector4Transform(XMLoadFloat3(&vSyncPos), NonRotateMatrix));

}

void CPlayerBall::Create_StickObjUI(_uint iModelIndex)
{

}

HRESULT CPlayerBall::Ready_Layer_CollEffect(const wstring & pLayerTag, _float3 vPos)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	/* Prototype_PlayerMoveUI */
	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("GameObject_CollEffect"), pLayerTag, &vPos)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}


CPlayerBall * CPlayerBall::Create(DEVICES)
{
	CPlayerBall*		pInstance = new CPlayerBall(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CPlayerBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CPlayerBall::Clone(void * pArg)
{
	CPlayerBall*		pInstance = new CPlayerBall(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CPlayerBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerBall::Free()
{
	__super::Free();
	//Safe_Release(m_pVIBuffer);
	Safe_Release(m_pColliderSphere);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRenderer);
}
