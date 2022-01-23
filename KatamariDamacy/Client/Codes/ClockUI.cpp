#include "stdafx.h"
#include "..\Headers\ClockUI.h"

#include "Management.h"

CClockUI::CClockUI(DEVICES)
	:CUIList(pDevice, pDeviceContext)
{
}

CClockUI::CClockUI(const CClockUI & other)
	:CUIList(other)
{
}

HRESULT CClockUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CClockUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_ComponentsOrthUI()))
		return E_FAIL;

	m_fSizeX = 176.f;
	m_fSizeY = 176.f;

	m_fX = g_iWinCX - m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());

	m_TransformMatrix._11 = m_fSizeX;
	m_TransformMatrix._22 = m_fSizeY;

	m_TransformMatrix._41 = 550.f;
	m_TransformMatrix._42 = 270.f;
	m_TransformMatrix._43 = 0.2f;
	XMStoreFloat4x4(&m_OrthMatrix, XMMatrixOrthographicLH((float)g_iWinCX, (float)g_iWinCY, 0.0f, 1.f));

	return S_OK;
}

_int CClockUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	_vector vYAxis;

	vYAxis = { 0.f, 0.f, 1.f, 0.f };
	m_pTransform->RotateAxis(vYAxis, -DeltaTime);

	return _int();
}

_int CClockUI::Late_Update(_double DeltaTime)
{
	if (m_pRenderer == nullptr)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::UI, this);
}

HRESULT CClockUI::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(m_pTransform->Get_WorldMatrix() * XMLoadFloat4x4(&m_TransformMatrix)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	return S_OK;
}

HRESULT CClockUI::SetUp_ComponentsOrthUI()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_VIBuffer_UI"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, TEXT("Component_Texture_ClockUI"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC	TransformDesc;
	TransformDesc.fRotatePerSec = 360.f / 300.f;
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CClockUI * CClockUI::Create(DEVICES)
{
	CClockUI*		pInstance = new CClockUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CClockUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CClockUI::Clone(void * pArg)
{
	CClockUI*		pInstance = new CClockUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CClockUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClockUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
}
