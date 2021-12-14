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

	//m_pTransformCom->Set_Scale(XMVectorSet(0.01f, 0.01f, 0.01f, 1.f));

	CTransform::TRANSFORMDESC	TransformDesc;
	TransformDesc.fSpeedPerSec = 3.f;

	m_pTransform->Set_TransformDesc(TransformDesc);

	m_pModel->SetUp_AnimationIndex(20);

	return S_OK;
}

_int CPlayer::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	Movement(DeltaTime);

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

	return S_OK;
}

void CPlayer::Movement(_double TimeDelta)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (pManagement->Get_DIKState(DIK_W) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(1);
		m_pTransform->Move_Straight(TimeDelta);
	}

	if (pManagement->Get_DIKState(DIK_S) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(12);
		m_pTransform->Move_Straight(-TimeDelta);
	}

	if (pManagement->Get_DIKState(DIK_A) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(3);
		m_pTransform->Move_Strafe(-TimeDelta);
	}

	if (pManagement->Get_DIKState(DIK_D) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(2);
		m_pTransform->Move_Strafe(TimeDelta);
	}

	RELEASE_INSTANCE(CManagement);
}

HRESULT CPlayer::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, TEXT("Component_Model_Player"), TEXT("Com_Model"), (CComponent**)&m_pModel)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform)))
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

	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRenderer);
}
