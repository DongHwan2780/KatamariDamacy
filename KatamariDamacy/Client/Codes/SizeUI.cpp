#include "stdafx.h"
#include "..\Headers\SizeUI.h"

#include "Management.h"

CSizeUI::CSizeUI(DEVICES)
	:CUIList(pDevice, pDeviceContext)
{
}

CSizeUI::CSizeUI(const CSizeUI & other)
	:CUIList(other)
{
}

HRESULT CSizeUI::Initialize_Prototype() 
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSizeUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_ComponentsOrthUI()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;

	m_fX = g_iWinCX - m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());

	m_TransformMatrix._11 = m_fSizeX;
	m_TransformMatrix._22 = m_fSizeY;

	m_TransformMatrix._41 = m_fX - (g_iWinCX >> 1);
	m_TransformMatrix._42 = -m_fY + (g_iWinCY >> 1);

	XMStoreFloat4x4(&m_OrthMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.0f, 1.f));

	return S_OK;
}

_int CSizeUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	return _int();
}

_int CSizeUI::Late_Update(_double DeltaTime)
{
	if (m_pRenderer == nullptr)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::PRIORITY, this);
}

HRESULT CSizeUI::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	return S_OK;
}

HRESULT CSizeUI::SetUp_ComponentsOrthUI()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_VIBuffer_UI"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, TEXT("Component_Texture_SizeUI"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;


	return S_OK;
}

CSizeUI * CSizeUI::Create(DEVICES)
{
	CSizeUI*		pInstance = new CSizeUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CSizeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CSizeUI::Clone(void * pArg)
{
	CSizeUI*		pInstance = new CSizeUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CSizeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSizeUI::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
}
