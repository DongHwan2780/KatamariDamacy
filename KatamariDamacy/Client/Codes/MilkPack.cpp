#include "stdafx.h"
#include "..\Headers\MilkPack.h"

#include "Management.h"

CMilkPack::CMilkPack(DEVICES)
	:CObjList(pDevice, pDeviceContext)
{
}

CMilkPack::CMilkPack(const CMilkPack & other)
	: CObjList(other)
{
}

HRESULT CMilkPack::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMilkPack::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg)
	{
		memcpy(&TransformDesc, pArg, sizeof(CTransform::TRANSFORMDESC));
	}

	m_pTransform->Set_State(CTransform::POSITION, TransformDesc.vPosition);
	m_pTransform->Set_Scale(XMVectorSet(TransformDesc.fScale, TransformDesc.fScale, TransformDesc.fScale, 0.f));
	m_pTransform->Set_TransformDesc(TransformDesc);
	m_pModel->SetModelScale(TransformDesc.fScale);
	m_fCollSize = 130.f;
	return S_OK;
}

_int CMilkPack::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;
	Coll_PlayerBall();

	m_pColliderSphere->Update_State(m_pTransform->Get_WorldMatrix());
	return _int();
}

_int CMilkPack::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::NONALPHA, this);
}

HRESULT CMilkPack::Render()
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

#ifdef _DEBUG
	m_pColliderSphere->Render();
#endif
	return S_OK;
}

HRESULT CMilkPack::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */
	CModel::MODELDESC	ModelDesc;
	ZeroMemory(&ModelDesc, sizeof(CModel::MODELDESC));
	ModelDesc.iModelIndexNum = 22;
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_MilkPack", L"Com_Model", (CComponent**)&m_pModel, &ModelDesc)))
		return E_FAIL;

	/* For.Com_Transform */

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
		return E_FAIL;
	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(2.f, 2.f, 2.f);
	ColliderDesc.eObjState = CCollider::OBJ_NONE;
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Collider_SPHERE", L"Com_SPHERE", (CComponent**)&m_pColliderSphere, &ColliderDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CMilkPack::SetUp_ConstantTable()
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

CMilkPack * CMilkPack::Create(DEVICES)
{
	CMilkPack*		pInstance = new CMilkPack(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CMilkPack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CMilkPack::Clone(void * pArg)
{
	CMilkPack*		pInstance = new CMilkPack(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CMilkPack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMilkPack::Free()
{

	Safe_Release(m_pColliderSphere);
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRenderer);
	__super::Free();
}
