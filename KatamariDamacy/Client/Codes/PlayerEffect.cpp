#include "stdafx.h"
#include "..\Headers\PlayerEffect.h"

#include "Management.h"

CPlayerEffect::CPlayerEffect(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CPlayerEffect::CPlayerEffect(const CPlayerEffect & other)
	: CObj(other)
{
}

HRESULT CPlayerEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerEffect::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 540.f;
	m_fSizeY = 380.f;

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());

	m_TransformMatrix._11 = m_fSizeX;
	m_TransformMatrix._22 = m_fSizeY;

	m_TransformMatrix._41 = 350.f;
	m_TransformMatrix._42 = -280.f;
	m_TransformMatrix._43 = 0.1f;

	XMStoreFloat4x4(&m_OrthMatrix, XMMatrixOrthographicLH((float)g_iWinCX, (float)g_iWinCY, 0.0f, 1.f));


	//_vector vPos = XMVectorSet(m_TransformMatrix._41, m_TransformMatrix._42, m_TransformMatrix._43, 1.f);

	//m_pTransform->Set_State(CTransform::POSITION, vPos);

	return S_OK;
}

_int CPlayerEffect::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	//m_TransformMatrix._41 -= 0.5f;
	//m_TransformMatrix._42 += 0.5f;

	return _uint();
}

_int CPlayerEffect::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	//MoveDir(DeltaTime);

	return m_pRenderer->Add_RenderGroup(CRenderer::PRIORITY, this);
}

HRESULT CPlayerEffect::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	int i = rand() % 4;

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, 0)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	return S_OK;
}

CPlayerEffect * CPlayerEffect::Create(DEVICES)
{
	CPlayerEffect*		pInstance = new CPlayerEffect(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CPlayerEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CPlayerEffect::Clone(void * pArg)
{
	CPlayerEffect*		pInstance = new CPlayerEffect(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CPlayerEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
}

void CPlayerEffect::MoveDir(_double DeltaTime)
{
	_vector vPos = XMVectorZero();

	_uint i = rand() % 10;

	switch (i)
	{
	case 0:
		vPos = XMVectorSet(500.f , -280.f , 0.f, 1.f);
		break;					
	case 1:						
		vPos = XMVectorSet(510.f , -270.f , 0.f, 1.f);
		break;					 
	case 2:						 
		vPos = XMVectorSet(520.f , -260.f , 0.f, 1.f);
		break;					
	case 3:						
		vPos = XMVectorSet(530.f , -250.f , 0.f, 1.f);
		break;					 
	case 4:						 
		vPos = XMVectorSet(540.f , -240.f , 0.f, 1.f);
		break;					
	case 5:						
		vPos = XMVectorSet(550.f , -240.f, 0.f, 1.f);
		break;					 
	case 6:						 
		vPos = XMVectorSet(560.f , -250.f, 0.f, 1.f);
		break;					
	case 7:						
		vPos = XMVectorSet(570.f , -260.f, 0.f, 1.f);
		break;					
	case 8:						
		vPos = XMVectorSet(580.f, -270.f , 0.f, 1.f);
		break;					
	case 9:						
		vPos = XMVectorSet(590.f , -280.f, 0.f, 1.f);
		break;
	}

	m_pTransform->Chase_Target(vPos ,DeltaTime);
}

HRESULT CPlayerEffect::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_VIBuffer_UI"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, TEXT("Component_Texture_PlayerEffect"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC	TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}
