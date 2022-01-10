#include "stdafx.h"
#include "..\Headers\Dummy.h"

#include "Management.h"
#include "ClientDefines.h"

CDummy::CDummy(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CDummy::CDummy(const CDummy & other)
	: CObj(other)
{
}

HRESULT CDummy::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDummy::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	CTransform::TRANSFORMDESC TransformDesc;

	if (pArg)
	{
		memcpy(&TransformDesc, pArg, sizeof(CTransform::TRANSFORMDESC));
	}

	if (FAILED(SetUp_Components(TransformDesc.vScale)))
		return E_FAIL;

	m_pTransform->Set_State(CTransform::POSITION, TransformDesc.vPosition);

	return S_OK;
}

_int CDummy::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	m_pOBBCom->Update_State(m_pTransform->Get_WorldMatrix());

	return _int();
}

_int CDummy::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	CManagement*	pGameInstance = GET_INSTANCE(CManagement);

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::NONALPHA, this);

	RELEASE_INSTANCE(CManagement);

	return 0;
}

HRESULT CDummy::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

#ifdef _DEBUG
	m_pOBBCom->Render();
#endif

	return S_OK;
}

HRESULT CDummy::SetUp_Components(_float3 vScale)
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */
	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(vScale.x, vScale.y, vScale.z);

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Collider_OBB"), L"Com_Collider", (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;
	/* For.Com_Transform */

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDummy::SetUp_ConstantTable()
{
	return S_OK;
}

CDummy * CDummy::Create(DEVICES)
{
	CDummy*		pInstance = new CDummy(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CDummy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CDummy::Clone(void * pArg)
{
	CDummy*		pInstance = new CDummy(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CDummy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDummy::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pRenderer);
}
