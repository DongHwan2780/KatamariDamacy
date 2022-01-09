#include "stdafx.h"
#include "ToolDummy.h"

#include "Management.h"
#include "ClientDefines.h"

CToolDummy::CToolDummy(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CToolDummy::CToolDummy(const CToolDummy & other)
	: CObj(other)
{
}

HRESULT CToolDummy::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolDummy::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_float3 vPickingPos = _float3{0.f ,0.f, 0.f};
	memcpy(&vPickingPos, pArg, sizeof(_float3));

	m_pTransform->Set_State(CTransform::POSITION, XMLoadFloat3(&vPickingPos));

	return S_OK;
}

_int CToolDummy::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	m_pOBBCom->Update_State(m_pTransform->Get_WorldMatrix());

	return _int();
}

_int CToolDummy::Late_Update(_double DeltaTime)
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

HRESULT CToolDummy::Render()
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

HRESULT CToolDummy::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */
	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(2.f, 2.f, 2.f);

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;
	/* For.Com_Transform */

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolDummy::SetUp_ConstantTable()
{
	return S_OK;
}

CToolDummy * CToolDummy::Create(DEVICES)
{
	CToolDummy*		pInstance = new CToolDummy(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CStageMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CToolDummy::Clone(void * pArg)
{
	CToolDummy*		pInstance = new CToolDummy(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CStageMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CToolDummy::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pRenderer);
}
