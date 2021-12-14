#include "..\Headers\Collider.h"


CCollider::CCollider(DEVICES)
	:CComponent(pDevice, pDeviceContext)
{
}

CCollider::CCollider(const CCollider & other)
	:CComponent(other)
{
}

HRESULT CCollider::Initialize_Prototype(COLLTYPE eType)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_eType = eType;

	switch (m_eType)
	{
	case COLL_AABB:
		m_pAABB = new BoundingBox(_float3(0.0f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f));
		break;
	case COLL_OBB:
		m_pOBB = new BoundingOrientedBox(_float3(0.0f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f), _float4(0.f, 0.f, 0.f, 0.f));
		break;
	case COLL_SPHERE:
		m_pSphere = new BoundingSphere(_float3(0.0f, 0.f, 0.f), 0.5f);
		break;
	}
	return S_OK;
}

HRESULT CCollider::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

	switch (m_eType)
	{
	case CCollider::COLL_AABB: case CCollider::COLL_OBB:
		m_pAABB = new BoundingBox(*m_pAABB);
		m_pAABB->Extents = _float3(m_ColliderDesc.vSize.x, m_ColliderDesc.vSize.y, m_ColliderDesc.vSize.z);
		break;

	case CCollider::COLL_SPHERE:
		break;
	}

	return S_OK;
}

CCollider * CCollider::Create(DEVICES, COLLTYPE eType)
{
	CCollider*	pInstance = new CCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		MSG_BOX("Failed To Creating CCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider*	pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed To Creating CCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOBB);
	Safe_Delete(m_pSphere);
}
