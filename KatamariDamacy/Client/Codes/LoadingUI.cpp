#include "stdafx.h"
#include "..\Headers\LoadingUI.h"

#include "Management.h"

CLoadingUI::CLoadingUI(DEVICES)
	:CUIList(pDevice, pDeviceContext)
{
}

CLoadingUI::CLoadingUI(const CLoadingUI & other)
	: CUIList(other)
{
}

HRESULT CLoadingUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 354.f;
	m_fSizeY = 376.f;

	m_fX = g_iWinCX - m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());

	m_TransformMatrix._11 = m_fSizeX / 2.f;
	m_TransformMatrix._22 = m_fSizeY / 2.f;

	m_TransformMatrix._41 = 500.f;
	m_TransformMatrix._42 = -230.f;

	XMStoreFloat4x4(&m_OrthMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.0f, 1.f));

	return S_OK;
}

_int CLoadingUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	if (m_iIdx < 2.0)
		m_iIdx += DeltaTime * 2.0;
	else
		m_iIdx = 0.0;

	return _int();
}

_int CLoadingUI::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;


	if (m_iIdx >= 2.0)
		m_iIdx = 0.0;

	return m_pRenderer->Add_RenderGroup(CRenderer::PRIORITY, this);
}

HRESULT CLoadingUI::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, m_iIdx)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	return S_OK;
}

HRESULT CLoadingUI::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_VIBuffer_UI"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(LOADING_SCENE, TEXT("Component_Texture_LoadingUI"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;

	return S_OK;
}

CLoadingUI * CLoadingUI::Create(DEVICES)
{
	CLoadingUI*		pInstance = new CLoadingUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CLoadingUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CLoadingUI::Clone(void * pArg)
{
	CLoadingUI*		pInstance = new CLoadingUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CLoadingUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingUI::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
}
