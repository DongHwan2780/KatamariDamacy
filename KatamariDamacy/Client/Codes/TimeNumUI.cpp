#include "stdafx.h"
#include "..\Headers\TimeNumUI.h"

#include "Management.h"

CTimeNumUI::CTimeNumUI(DEVICES)
	:CUIList(pDevice, pDeviceContext)
{
}

CTimeNumUI::CTimeNumUI(const CTimeNumUI & other)
	:CUIList(other)
{
}

HRESULT CTimeNumUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTimeNumUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_ComponentsOrthUI()))
		return E_FAIL;

	m_fSizeX = 32.f;
	m_fSizeY = 42.f;

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());

	m_TransformMatrix._11 = m_fSizeX;
	m_TransformMatrix._22 = m_fSizeY;

	m_TransformMatrix._41 = 420.f;
	m_TransformMatrix._42 = 290.f;
	//m_TransformMatrix._43 = 0.5f;


	XMStoreFloat4x4(&m_TransformMatrixTwo, XMMatrixIdentity());

	m_TransformMatrixTwo._11 = m_fSizeX;
	m_TransformMatrixTwo._22 = m_fSizeY;

	m_TransformMatrixTwo._41 = 450.f;
	m_TransformMatrixTwo._42 = 290.f;
	//m_TransformMatrixTwo._43 = 0.5f;

	XMStoreFloat4x4(&m_OrthMatrix, XMMatrixOrthographicLH((float)g_iWinCX, (float)g_iWinCY, 0.0f, 1.f));

	return S_OK;
}

_int CTimeNumUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	Calculate_Time(DeltaTime);

	return _int();
}

_int CTimeNumUI::Late_Update(_double DeltaTime)
{
	if (m_pRenderer == nullptr)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::PRIORITY, this);
}

HRESULT CTimeNumUI::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, m_iTextureIndexFirst)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrixTwo)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, m_iTextureIndexSecond)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	return S_OK;
}

void CTimeNumUI::Calculate_Time(_double DeltaTime)
{
	m_dNowTime += DeltaTime;
	if (m_dLimitTime == 0)
	{
		m_dLimitTime = 0;
		m_iTextureIndexFirst = 0;
		m_iTextureIndexSecond = 0;
	}
	else if (m_dNowTime >= 1.0)
	{
		m_dLimitTime -= 1.0;
		m_dNowTime -= 1.0;
	}


	if (m_dLimitTime > 240.0)
	{
		m_iTextureIndexFirst = 0;
		m_iTextureIndexSecond = 5;
	}
	else if (m_dLimitTime > 180.0)
	{
		m_iTextureIndexFirst = 0;
		m_iTextureIndexSecond = 4;
	}
	else if (m_dLimitTime > 120.0)
	{
		m_iTextureIndexFirst = 0;
		m_iTextureIndexSecond = 3;
	}
	else if (m_dLimitTime > 60.0)
	{
		m_iTextureIndexFirst = 0;
		m_iTextureIndexSecond = 2;
	}
	else if (m_dLimitTime == 60.0)
	{
		m_iTextureIndexFirst = 0;
		m_iTextureIndexSecond = 1;
	}
	else if(m_dLimitTime < 60.0)
	{
		m_iTextureIndexFirst = m_dLimitTime / 10.0;
		m_iTextureIndexSecond = (int)m_dLimitTime % 10;
	}

}

HRESULT CTimeNumUI::SetUp_ComponentsOrthUI()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_VIBuffer_UI"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, TEXT("Component_Texture_TimeNumUI"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;


	return S_OK;
}

CTimeNumUI * CTimeNumUI::Create(DEVICES)
{
	CTimeNumUI*		pInstance = new CTimeNumUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CTimeNumUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CTimeNumUI::Clone(void * pArg)
{
	CTimeNumUI*		pInstance = new CTimeNumUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CTimeNumUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimeNumUI::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
}
