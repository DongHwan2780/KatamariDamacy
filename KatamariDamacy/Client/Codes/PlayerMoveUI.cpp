#include "stdafx.h"
#include "..\Headers\PlayerMoveUI.h"

#include "Management.h"

CPlayerMoveUI::CPlayerMoveUI(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CPlayerMoveUI::CPlayerMoveUI(const CPlayerMoveUI & other)
	: CObj(other)
{
}

HRESULT CPlayerMoveUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerMoveUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pModel->SetUp_AnimationIndex(20);

	m_fSizeX = 176.f;
	m_fSizeY = 176.f;

	m_fX = g_iWinCX - m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());

	m_TransformMatrix._11 = m_fSizeX;
	m_TransformMatrix._22 = m_fSizeY;

	m_TransformMatrix._41 = 550.f;
	m_TransformMatrix._42 = -280.f;
	m_TransformMatrix._43 = 0.5f;

	XMStoreFloat4x4(&m_OrthMatrix, XMMatrixOrthographicLH((float)g_iWinCX, (float)g_iWinCY, 0.0f, 1.f));

	return S_OK;
}

_int CPlayerMoveUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	Movement(DeltaTime);

	return _int();
}

_int CPlayerMoveUI::Late_Update(_double DeltaTime)
{
	if (m_pRenderer == nullptr)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	m_pModel->Play_Animation(DeltaTime);

	return m_pRenderer->Add_RenderGroup(CRenderer::NONALPHA, this);
}

HRESULT CPlayerMoveUI::Render()
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

	return S_OK;
}

void CPlayerMoveUI::Movement(_double DeltaTime)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (pManagement->Get_DIKState(DIK_UP) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(1);
	}

	else if (pManagement->Get_DIKState(DIK_DOWN) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(12);
	}

	else if (pManagement->Get_DIKState(DIK_LEFT) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(43);
	}

	else if (pManagement->Get_DIKState(DIK_RIGHT) & 0x80)
	{
		m_pModel->SetUp_AnimationIndex(42);
	}
	else
		m_pModel->SetUp_AnimationIndex(20);

	RELEASE_INSTANCE(CManagement);
}

HRESULT CPlayerMoveUI::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_PlayerUI", L"Com_Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	/* For.Com_Transform */

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerMoveUI::SetUp_ConstantTable()
{
	if (nullptr == m_pModel)
		return E_FAIL;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)), sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

CPlayerMoveUI * CPlayerMoveUI::Create(DEVICES)
{
	CPlayerMoveUI*		pInstance = new CPlayerMoveUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CPlayerMoveUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CPlayerMoveUI::Clone(void * pArg)
{
	CPlayerMoveUI*		pInstance = new CPlayerMoveUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CPlayerMoveUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerMoveUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRenderer);
}
