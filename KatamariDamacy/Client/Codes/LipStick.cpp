#include "stdafx.h"
#include "..\Headers\LipStick.h"

#include "Management.h"

CLipStick::CLipStick(DEVICES)
	:CObjList(pDevice, pDeviceContext)
{
}

CLipStick::CLipStick(const CLipStick & other)
	: CObjList(other)
{
}

HRESULT CLipStick::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLipStick::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CTransform::TRANSFORMDESC TransformDesc;

	if (pArg)
	{
		memcpy(&TransformDesc, pArg, sizeof(CTransform::TRANSFORMDESC));
	}


	m_pTransform->Set_State(CTransform::POSITION, TransformDesc.vPosition);

	m_pTransform->Set_Scale(XMVectorSet(TransformDesc.fScale, TransformDesc.fScale, TransformDesc.fScale, 0.f));

	return S_OK;
}

_int CLipStick::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	return _int();
}

_int CLipStick::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::NONALPHA, this);
}

HRESULT CLipStick::Render()
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
		m_pModel->Render(i, 0);
	}

	return S_OK;
}

HRESULT CLipStick::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_LipStick", L"Com_Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	/* For.Com_Transform */

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLipStick::SetUp_ConstantTable()
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

CLipStick * CLipStick::Create(DEVICES)
{
	CLipStick*		pInstance = new CLipStick(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CStageMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CLipStick::Clone(void * pArg)
{
	CLipStick*		pInstance = new CLipStick(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CStageMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLipStick::Free()
{

	//Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRenderer);
	__super::Free();
}
