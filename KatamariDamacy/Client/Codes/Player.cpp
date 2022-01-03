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

	m_pModel->SetUp_AnimationIndex(20);

	return S_OK;
}

_int CPlayer::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	if (m_bFistCreate)
	{
		Ready_Layer_PlayerBall(L"Layer_PlayerBall");

		CManagement*	pManagement = GET_INSTANCE(CManagement);

		m_pPlayerBallTransform = dynamic_cast<CTransform*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_PlayerBall", L"Com_Transform"));

		RELEASE_INSTANCE(CManagement);

		m_bFistCreate = false;
	}

	Movement(DeltaTime);

	m_pCollider->Update_State(m_pTransform->Get_WorldMatrix());

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

	_uint		iNumMaterials = m_pModel->Get_NumMaterials();

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

	if (pManagement->Get_DIKState(DIK_UP) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(1);
		m_pTransform->Move_Straight(DeltaTime);
		m_pPlayerBallTransform->RotateAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), DeltaTime * 20.f);

		if (pManagement->Get_DIKState(DIK_LEFT) & 0x80)
		{
			m_pTransform->Move_Strafe(-DeltaTime);
			m_pPlayerBallTransform->RotateAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), DeltaTime * 20.f);
		}
		else if (pManagement->Get_DIKState(DIK_RIGHT) & 0x80)
		{
			m_pTransform->Move_Strafe(DeltaTime);
			m_pPlayerBallTransform->RotateAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), DeltaTime * -20.f);
		}
	}

	else if (pManagement->Get_DIKState(DIK_DOWN) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(12);
		m_pTransform->Move_Straight(-DeltaTime);
		m_pPlayerBallTransform->RotateAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), DeltaTime * -20.f);

		if (pManagement->Get_DIKState(DIK_LEFT) & 0x80)
		{
			m_pTransform->Move_Strafe(-DeltaTime);
			m_pPlayerBallTransform->RotateAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), DeltaTime * 20.f);
		}
		else if (pManagement->Get_DIKState(DIK_RIGHT) & 0x80)
		{
			m_pTransform->Move_Strafe(DeltaTime);
			m_pPlayerBallTransform->RotateAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), DeltaTime * -20.f);
		}
	}

	else if (pManagement->Get_DIKState(DIK_LEFT) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(3);
		m_pTransform->Move_Strafe(-DeltaTime);
		m_pPlayerBallTransform->RotateAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), DeltaTime * 20.f);

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
		m_pModel->SetUp_AnimationIndex(2);
		m_pTransform->Move_Strafe(DeltaTime);
		m_pPlayerBallTransform->RotateAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), DeltaTime * -20.f);

		if (pManagement->Get_DIKState(DIK_UP) & 0x80)
		{
			m_pTransform->Move_Straight(DeltaTime);
		}
		else if (pManagement->Get_DIKState(DIK_DOWN) & 0x80)
		{
			m_pTransform->Move_Straight(-DeltaTime);
		}
	}
	else
		m_pModel->SetUp_AnimationIndex(20);

	RELEASE_INSTANCE(CManagement);
}

void CPlayer::Acceleration(_double DeltaTime, _float Ballsize)
{
	_float fPlayerSpeed = m_fAccel * Ballsize;


		if (fPlayerSpeed > m_fMaxSpeed - 20.f)
		{
			m_fAccel = 7.f;
		}
		else if (m_fAccel < 15.f && fPlayerSpeed <= m_fMaxSpeed - 20.f)
		{
			m_fAccel += DeltaTime*25.f;
		}

		if (fPlayerSpeed > 0.f)
		{
			m_fBackAccel = 15.f;
		}
		else if (fPlayerSpeed < -m_fBackMaxSpeed + 20.f)
		{
			m_fBackAccel = 7.f;
		}
}

void CPlayer::ResistAccel(_double DeltaTime, _float Ballsize)
{
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
	TransformDesc.fSpeedPerSec = 3.f;

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Collider_AABB", L"Com_Collider", (CComponent**)&m_pCollider, &ColliderDesc)))
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

	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRenderer);
}
