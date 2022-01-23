#include "stdafx.h"
#include "..\Headers\CollEffect.h"

#include "Management.h"

CCollEffect::CCollEffect(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CCollEffect::CCollEffect(const CCollEffect & other)
	:CObj(other)
{
}

HRESULT CCollEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollEffect::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&vPos, pArg, sizeof(_vector));

	m_pTransform->Set_State(CTransform::POSITION, vPos);

	return S_OK;
}

_int CCollEffect::Update(_double DeltaTime)
{
	//if (m_bDead)
	//	OBJ_DEAD;

	if (0 > __super::Update(DeltaTime))
		return -1;

	//m_fFrame += 90.0f * DeltaTime;
	//if (m_fFrame >= 90)
	//	m_bDead = true;

	return _uint();
}

_int CCollEffect::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::ALPHA, this);
}

HRESULT CCollEffect::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	CManagement*		pManagement = GET_INSTANCE(CManagement);

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(m_pTransform->Get_WorldMatrix()), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(pManagement->Get_Transform(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(pManagement->Get_Transform(CPipeLine::D3DTS_PROJ)), sizeof(_matrix));
	//m_pVIBuffer->SetUp_ValueOnShader("g_vCamPosition", &pManagement->Get_CamPosition(), sizeof(_vector));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CCollEffect::SetUp_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, TEXT("Component_Texture_CollEffect"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;


	return S_OK;
}

CCollEffect * CCollEffect::Create(DEVICES)
{
	CCollEffect*		pInstance = new CCollEffect(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CCollEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CCollEffect::Clone(void * pArg)
{
	CCollEffect*		pInstance = new CCollEffect(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CCollEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
}
