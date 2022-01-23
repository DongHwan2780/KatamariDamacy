#include "stdafx.h"
#include "..\Headers\Player.h"

#include "Management.h"

CPlayer::CPlayer(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CPlayer::CPlayer(const CPlayer & other)
	:CObj(other)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CManagement*	pManagement = GET_INSTANCE(CManagement);
	m_pVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_Terrain", L"Com_VIBuffer"));
	RELEASE_INSTANCE(CManagement);

	m_pTransform->Set_State(CTransform::POSITION, XMVectorSet(320.f, 0.f, 40.f, 1.f));

	m_pModel->SetUp_AnimationIndex(20);

	m_fAccel = 5.f;
	m_fMaxSpeed = 10.f;
	m_fBackAccel = 5.f;
	m_fBackMaxSpeed = 10.f;

	m_fLeftAccel = 5.f;
	m_fRightAccel = 5.f;
	m_fLeftMaxSpeed = 10.f;
	m_fRightMaxSpeed = 10.f;

	return S_OK;
}

_int CPlayer::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (m_bFistCreate)
	{
		Ready_Layer_PlayerBall(L"Layer_PlayerBall");


		m_pPlayerBallTransform = dynamic_cast<CTransform*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_PlayerBall", L"Com_Transform"));


		m_bFistCreate = false;
	}

	CObj* m_pPlayerBall = pManagement->GetGameObject(STAGEONE_SCENE, L"Layer_PlayerBall");

	CPlayerBall* pPlayerBall = dynamic_cast<CPlayerBall*>(m_pPlayerBall);
	_float fBallSize = pPlayerBall->Get_PlayerBallSize();

	Movement(DeltaTime);
	Gravity(DeltaTime);
	Acceleration(DeltaTime, fBallSize);

	m_pCollider->Update_State(m_pTransform->Get_WorldMatrix());
	RELEASE_INSTANCE(CManagement);

	return _int();
}

_int CPlayer::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;


	m_pModel->Play_Animation(DeltaTime);
	return m_pRenderer->Add_RenderGroup(CRenderer::NONALPHA, this);
}

HRESULT CPlayer::Render()
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
		m_pModel->Render(i, 1);
	}

#ifdef _DEBUG
	m_pCollider->Render();
#endif

	return S_OK;
}

void CPlayer::Movement(_double DeltaTime)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	CObj* m_pPlayerBall = pManagement->GetGameObject(STAGEONE_SCENE, L"Layer_PlayerBall");

	CPlayerBall* pPlayerBall = dynamic_cast<CPlayerBall*>(m_pPlayerBall);


	if (pManagement->Get_DIKState(DIK_UP) & 0x80)
	{
		pPlayerBall->Set_SyncCheck(true);
		m_pTransform->Set_TransformDescSpeedRight(0.f);

		m_pTransform->Set_TransformDescAddSpeed(m_fAccel * DeltaTime);

		if (m_pTransform->GetTransformDesc().fSpeedPerSec >= m_fMaxSpeed)
			m_pTransform->Set_TransformDescSpeed(m_fMaxSpeed);

		if (m_pTransform->GetTransformDesc().fSpeedPerSec <= 0)
		{
			CManagement*		pManagement = GET_INSTANCE(CManagement);
			pManagement->StopSound(CSoundMgr::SOUNDCHANNEL::BREAK);
			pManagement->PlaySounds(L"Break.wav", CSoundMgr::SOUNDCHANNEL::BREAK);
			RELEASE_INSTANCE(CManagement);
			m_pModel->SetUp_AnimationIndex(10);
		}
		else
		{
			m_pModel->SetUp_AnimationIndex(1);
		}

		m_pTransform->Move_Straight(DeltaTime);
		m_pPlayerBallTransform->RotateAxis(m_pTransform->Get_State(CTransform::RIGHT) , DeltaTime);

		if (pManagement->Get_DIKState(DIK_LEFT) & 0x80)
		{
			m_pTransform->Move_Strafe(-DeltaTime);
			m_pTransform->RotateAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), -DeltaTime);
			m_pPlayerBallTransform->RotateAxis(m_pTransform->Get_State(CTransform::LOOK), DeltaTime );
			
		}
		else if (pManagement->Get_DIKState(DIK_RIGHT) & 0x80)
		{
			m_pTransform->Move_Strafe(DeltaTime);
			m_pTransform->RotateAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), DeltaTime);
			m_pPlayerBallTransform->RotateAxis(m_pTransform->Get_State(CTransform::LOOK), -DeltaTime);

		}
	}

	else if (pManagement->Get_DIKState(DIK_DOWN) & 0x80)
	{
		pPlayerBall->Set_SyncCheck(true);
		m_pTransform->Set_TransformDescSpeedRight(0.f);

		m_pTransform->Set_TransformDescAddSpeed(-m_fBackAccel * DeltaTime);

		if (m_pTransform->GetTransformDesc().fSpeedPerSec <= -m_fBackMaxSpeed)
			m_pTransform->Set_TransformDescSpeed(-m_fBackMaxSpeed);

		if (m_pTransform->GetTransformDesc().fSpeedPerSec >= 0)
		{
			CManagement*		pManagement = GET_INSTANCE(CManagement);
			pManagement->StopSound(CSoundMgr::SOUNDCHANNEL::BREAK);
			pManagement->PlaySounds(L"Break.wav", CSoundMgr::SOUNDCHANNEL::BREAK);
			RELEASE_INSTANCE(CManagement);
			m_pModel->SetUp_AnimationIndex(9);
		}
		else
		{
			m_pModel->SetUp_AnimationIndex(12);
		}
		m_pTransform->Move_Straight(DeltaTime);
		m_pPlayerBallTransform->RotateAxis(m_pTransform->Get_State(CTransform::RIGHT), -DeltaTime);

		if (pManagement->Get_DIKState(DIK_LEFT) & 0x80)
		{
			m_pTransform->Move_Strafe(DeltaTime);
			m_pTransform->RotateAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), DeltaTime);
			m_pPlayerBallTransform->RotateAxis(m_pTransform->Get_State(CTransform::LOOK), DeltaTime);

		}
		else if (pManagement->Get_DIKState(DIK_RIGHT) & 0x80)
		{
			m_pTransform->Move_Strafe(-DeltaTime);
			m_pTransform->RotateAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), -DeltaTime);
			m_pPlayerBallTransform->RotateAxis(m_pTransform->Get_State(CTransform::LOOK), -DeltaTime);

		}
	}

	else if (pManagement->Get_DIKState(DIK_LEFT) & 0x80)
	{
		pPlayerBall->Set_SyncCheck(true);
		m_pTransform->Set_TransformDescSpeed(0.f);
		m_pTransform->Set_TransformDescAddSpeedRight(-m_fLeftAccel * DeltaTime);

		if (m_pTransform->GetTransformDesc().fSpeedPerRightSec <= -m_fLeftMaxSpeed)
			m_pTransform->Set_TransformDescSpeedRight(-m_fLeftMaxSpeed);

		if (m_pTransform->GetTransformDesc().fSpeedPerRightSec >= 0)
		{
			CManagement*		pManagement = GET_INSTANCE(CManagement);
			pManagement->StopSound(CSoundMgr::SOUNDCHANNEL::BREAK);
			pManagement->PlaySounds(L"Break.wav", CSoundMgr::SOUNDCHANNEL::BREAK);
			RELEASE_INSTANCE(CManagement);
			m_pModel->SetUp_AnimationIndex(6);
		}
		else
		{
			m_pModel->SetUp_AnimationIndex(43);
		}

			m_pTransform->Move_Strafe(DeltaTime);
			m_pPlayerBallTransform->RotateAxis(m_pTransform->Get_State(CTransform::LOOK), DeltaTime);

			if (pManagement->Get_DIKState(DIK_UP) & 0x80)
			{
				m_pTransform->Move_Straight(DeltaTime);
			}
			else if (pManagement->Get_DIKState(DIK_DOWN) & 0x80)
			{
				m_pTransform->Move_Straight(-DeltaTime);
			}
	}

	else if (pManagement->Get_DIKState(DIK_RIGHT) & 0x80)
	{
		m_pTransform->Set_TransformDescSpeed(0.f);
		pPlayerBall->Set_SyncCheck(true);

		m_pTransform->Set_TransformDescAddSpeedRight(m_fRightAccel * DeltaTime);

		if (m_pTransform->GetTransformDesc().fSpeedPerRightSec >= m_fRightMaxSpeed)
			m_pTransform->Set_TransformDescSpeedRight(m_fRightMaxSpeed);

		if (m_pTransform->GetTransformDesc().fSpeedPerRightSec <= 0)
		{
			CManagement*		pManagement = GET_INSTANCE(CManagement);
			pManagement->StopSound(CSoundMgr::SOUNDCHANNEL::BREAK);
			pManagement->PlaySounds(L"Break.wav", CSoundMgr::SOUNDCHANNEL::BREAK);
			RELEASE_INSTANCE(CManagement);
			m_pModel->SetUp_AnimationIndex(7);
		}
		else
		{
			m_pModel->SetUp_AnimationIndex(42);
		}

			m_pTransform->Move_Strafe(DeltaTime);
			m_pPlayerBallTransform->RotateAxis(m_pTransform->Get_State(CTransform::LOOK), -DeltaTime);

			if (pManagement->Get_DIKState(DIK_UP) & 0x80)
			{
				m_pTransform->Move_Straight(DeltaTime);
			}
			else if (pManagement->Get_DIKState(DIK_DOWN) & 0x80)
			{
				m_pTransform->Move_Straight(-DeltaTime);
			}
	}
	else if (pManagement->Get_DIKState(DIK_Q) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(3);
		Around_Ball(2.f);
	}
	else if (pManagement->Get_DIKState(DIK_E) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(2);
		Around_Ball(-2.f);
	}
	else
	{
		m_pModel->SetUp_AnimationIndex(20);
		m_pTransform->Set_TransformDescAddSpeed(-2.5f * DeltaTime);

		if (m_pTransform->GetTransformDesc().fSpeedPerSec <= 0.f)
			m_pTransform->Set_TransformDescSpeed(0.f);

		m_pTransform->Set_TransformDescAddSpeedRight(-2.5f * DeltaTime);

		if (m_pTransform->GetTransformDesc().fSpeedPerRightSec <= 0.f)
			m_pTransform->Set_TransformDescSpeedRight(0.f);
	}

	//ResistAccel(DeltaTime);

	RELEASE_INSTANCE(CManagement);
}

void CPlayer::Acceleration(_double DeltaTime, _float Ballsize)
{
	_float fPlayerSpeed = m_pTransform->GetTransformDesc().fSpeedPerSec;
	_float fPlayerSpeedRight = m_pTransform->GetTransformDesc().fSpeedPerRightSec;

	if (fPlayerSpeed > m_fMaxSpeed - 4.f)
	{
		m_fAccel = 2.f;
	}
	else if (fPlayerSpeed <= m_fMaxSpeed)
	{
		m_fAccel += DeltaTime * 5.f;
	}

	if (fPlayerSpeed > 0.f)
	{
		m_fBackAccel = 15.f;
	}
	else if (fPlayerSpeed < -m_fBackMaxSpeed + 5.f)
	{
		m_fBackAccel = 5.f;
	}
	////////////////////////////////////////////////////////////////////////////////////
	if (fPlayerSpeedRight > m_fRightMaxSpeed - 4.f)
	{
		m_fRightAccel = 2.f;
	}
	else if (fPlayerSpeedRight <= m_fRightMaxSpeed)
	{
		m_fRightAccel += DeltaTime * 5.f;
	}

	if (fPlayerSpeedRight > 0.f)
	{
		m_fLeftAccel = 15.f;
	}
	else if (fPlayerSpeedRight < -m_fLeftMaxSpeed + 5.f)
	{
		m_fLeftAccel = 5.f;
	}

}

void CPlayer::ResistAccel(_double DeltaTime)
{
	_float fVel = m_pTransform->GetTransformDesc().fSpeedPerSec;
	// 속도에 따라 저항을 다르게 주어야 함
	if (m_pTransform->GetTransformDesc().fSpeedPerSec > 0)
	{
		m_pTransform->Set_TransformDescAddSpeed(-DeltaTime * m_fResistance);//*fVel*0.002f*5);
		if (m_pTransform->GetTransformDesc().fSpeedPerSec <= 0)
			m_pTransform->Set_TransformDescSpeed(0.f);
	}
	else if (m_pTransform->GetTransformDesc().fSpeedPerSec < 0)
	{
		m_pTransform->Set_TransformDescAddSpeed(DeltaTime * m_fResistance);//*fVel*0.002f*5);
		if (m_pTransform->GetTransformDesc().fSpeedPerSec >= 0)
			m_pTransform->Set_TransformDescSpeed(0.f);
	}
}

void CPlayer::Gravity(_double DeltaTime)
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
		fPos.y = m_fGravityY - 9.8f * m_fGravityTime * m_fGravityTime ;
		m_pTransform->Set_State(CTransform::POSITION, XMVectorSet(fPos.x, fPos.y, fPos.z, 1.f));
		m_fGravityTime += (float)DeltaTime;
	}
}

void CPlayer::Around_Ball(_float Orbit)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);
	CObj* m_pPlayerBall = pManagement->GetGameObject(STAGEONE_SCENE, L"Layer_PlayerBall");

	CPlayerBall* pPlayerBall = dynamic_cast<CPlayerBall*>(m_pPlayerBall);
	pPlayerBall->Set_SyncCheck(false);

	_vector vPos = m_pTransform->Get_State(CTransform::POSITION);
	_vector vBallPos = m_pPlayerBallTransform->Get_State(CTransform::POSITION);

	_vector		vRight, vUp, vLook;

	_matrix OffsetMatrix, OrbitMat, WorldMat;
	OffsetMatrix = XMMatrixIdentity();

	_matrix			NonRotateMatrix = XMMatrixIdentity();
	NonRotateMatrix.r[3] = m_pPlayerBallTransform->Get_WorldMatrix().r[3];		// 부모 == 공 회전값 뺀 위치 매트릭스

	OrbitMat = XMMatrixRotationY(XMConvertToRadians(Orbit));		// 공전 매트릭스
	OffsetMatrix = XMMatrixTranslationFromVector(vPos - vBallPos);		// 공전 위치
	WorldMat = OffsetMatrix * OrbitMat * NonRotateMatrix;
	m_pTransform->Set_WorldMatrix(WorldMat);

	vRight = m_pTransform->Get_State(CTransform::RIGHT);
	vUp = m_pTransform->Get_State(CTransform::UP);
	vLook = m_pTransform->Get_State(CTransform::LOOK);

	vLook = m_pPlayerBallTransform->Get_State(CTransform::POSITION) - m_pTransform->Get_State(CTransform::POSITION);
	m_pTransform->Set_State(CTransform::LOOK, XMVector3Normalize(vLook));

	_float3 vAxis = _float3(0.f, 1.f, 0.f);
	vRight = XMVector3Cross(XMLoadFloat3(&vAxis), vLook);
	m_pTransform->Set_State(CTransform::RIGHT, XMVector3Normalize(vRight));

	vUp = XMVector3Cross(vLook, vRight);
	m_pTransform->Set_State(CTransform::UP, XMVector3Normalize(vUp));

	RELEASE_INSTANCE(CManagement);
}

HRESULT CPlayer::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Player", L"Com_Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	/* For.Com_Transform */

	CTransform::TRANSFORMDESC	TransformDesc;
	TransformDesc.fRotatePerSec = 10.0f;

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);

	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Collider_OBB", L"Com_OBB", (CComponent**)&m_pCollider, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::SetUp_ConstantTable()
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

HRESULT CPlayer::Ready_Layer_PlayerBall(const wstring & pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("GameObject_PlayerBall"), pLayerTag)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

CPlayer * CPlayer::Create(DEVICES)
{
	CPlayer*		pInstance = new CPlayer(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CPlayer::Clone(void * pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	//Safe_Release(m_pVIBuffer);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRenderer);
}
