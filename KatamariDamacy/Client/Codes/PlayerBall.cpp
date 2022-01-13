#include "stdafx.h"
#include "..\Headers\PlayerBall.h"

#include "Management.h"

CPlayerBall::CPlayerBall(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CPlayerBall::CPlayerBall(const CPlayerBall & other)
	:CObj(other)
{
}

HRESULT CPlayerBall::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerBall::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	m_pPlayerTransform = dynamic_cast<CTransform*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_Player", L"Com_Transform"));

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

_int CPlayerBall::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;



	m_pCollider->Update_State(m_pTransform->Get_WorldMatrix());
	m_pColliderSphere->Update_State(m_pTransform->Get_WorldMatrix());

	SetTransform();

	return _int();
}

_int CPlayerBall::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	//CObj*	pObj = nullptr;
	//m_pColliderSphere->Collision_Sphere(this, L"Layer_StageObj", pObj);


	return m_pRenderer->Add_RenderGroup(CRenderer::NONALPHA, this);
}

HRESULT CPlayerBall::Render()
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
	m_pCollider->Render();
	m_pColliderSphere->Render();
#endif

	return S_OK;
}

HRESULT CPlayerBall::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_PlayerBall", L"Com_Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	/* For.Com_Transform */

	CTransform::TRANSFORMDESC	TransformDesc;
	TransformDesc.fRotatePerSec = XMConvertToRadians(360.0f);

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Collider_AABB", L"Com_AABB", (CComponent**)&m_pCollider, &ColliderDesc)))
		return E_FAIL;


	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Collider_Sphere", L"Com_SPHERE", (CComponent**)&m_pColliderSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerBall::SetUp_ConstantTable()
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

void CPlayerBall::SetTransform()
{
	_vector		vRight, vUp, vLook;
	_matrix		TransMatrix;
	TransMatrix = XMMatrixTranslation(0.f, 0.5f, 2.f);

	vRight = m_pTransform->Get_State(CTransform::RIGHT);
	vUp = m_pTransform->Get_State(CTransform::UP);
	vLook = m_pTransform->Get_State(CTransform::LOOK);

	m_pTransform->Set_WorldMatrix(m_pPlayerTransform->Get_WorldMatrix() * TransMatrix);

	m_pTransform->Set_State(CTransform::RIGHT, XMVector4Transform(vRight, m_pPlayerTransform->Get_WorldMatrix() * TransMatrix));
	m_pTransform->Set_State(CTransform::UP, XMVector4Transform(vUp, m_pPlayerTransform->Get_WorldMatrix() * TransMatrix));
	m_pTransform->Set_State(CTransform::LOOK, XMVector4Transform(vLook, m_pPlayerTransform->Get_WorldMatrix() * TransMatrix));
}

CPlayerBall * CPlayerBall::Create(DEVICES)
{
	CPlayerBall*		pInstance = new CPlayerBall(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CPlayerBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CPlayerBall::Clone(void * pArg)
{
	CPlayerBall*		pInstance = new CPlayerBall(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CPlayerBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerBall::Free()
{
	__super::Free();

	Safe_Release(m_pColliderSphere);
	Safe_Release(m_pPlayerTransform);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRenderer);
}
