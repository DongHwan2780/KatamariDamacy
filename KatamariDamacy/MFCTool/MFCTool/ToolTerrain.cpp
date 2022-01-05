#include "stdafx.h"
#include "ToolTerrain.h"

#include "Management.h"
#include "ClientDefines.h"

CToolTerrain::CToolTerrain(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CToolTerrain::CToolTerrain(const CToolTerrain & other)
	:CObj(other)
{
}

HRESULT CToolTerrain::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolTerrain::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CToolTerrain::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	return _int();
}

_int CToolTerrain::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::NONALPHA, this);
}

HRESULT CToolTerrain::Render()
{

	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	return S_OK;
}

HRESULT CToolTerrain::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_VIBuffer_Terrain"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolTerrain::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(m_pTransform->Get_WorldMatrix()), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(pManagement->Get_Transform(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(pManagement->Get_Transform(CPipeLine::D3DTS_PROJ)), sizeof(_matrix));

	//_matrix		ViewMatrix = pManagement->Get_Transform(CPipeLine::D3DTS_VIEW);

	///* 카메라의 월드변환행렬. */
	//ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	//m_pVIBuffer->SetUp_ValueOnShader("g_vCamPosition", &ViewMatrix.r[3], sizeof(_vector));

	//if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseSourTexture", m_pTexture, 0)))
	//	return E_FAIL;
	//if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseDestTexture", m_pTexture, 1)))
	//	return E_FAIL;
	//if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_FilterTexture", m_pFilter)))
	//	return E_FAIL;
	//if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BrushTexture", m_pBrush)))
	//	return E_FAIL;

	//LIGHTDESC		LightDesc = *pManagement->Get_LightDesc();

	//m_pVIBuffer->SetUp_ValueOnShader("g_vLightDir", &LightDesc.vLightDir, sizeof(_vector));
	//m_pVIBuffer->SetUp_ValueOnShader("g_vLightDiffuse", &LightDesc.vDiffuse, sizeof(_vector));
	//m_pVIBuffer->SetUp_ValueOnShader("g_vLightAmbient", &LightDesc.vAmbient, sizeof(_vector));
	//m_pVIBuffer->SetUp_ValueOnShader("g_vLightSpecular", &LightDesc.vSpecular, sizeof(_vector));

	RELEASE_INSTANCE(CManagement);


	return S_OK;
}

CToolTerrain * CToolTerrain::Create(DEVICES)
{
	CToolTerrain*		pInstance = new CToolTerrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CToolTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CToolTerrain::Clone(void * pArg)
{
	CToolTerrain*		pInstance = new CToolTerrain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CToolTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CToolTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
}
