#include "..\Headers\Collider.h"

#include "PipeLine.h"
#include "Layer.h"
#include "Obj.h"
#include "Transform.h"
#include "Management.h"

CCollider::CCollider(DEVICES)
	:CComponent(pDevice, pDeviceContext)
{
}

CCollider::CCollider(const CCollider & other)
	:CComponent(other)
	, m_eType(other.m_eType)
	, m_pEffect(other.m_pEffect)
	, m_pBatch(other.m_pBatch)
	, m_pBB(other.m_pBB)
	, m_pSphere(other.m_pSphere)
	, m_pInputLayOut(other.m_pInputLayOut)
	, m_isCollision(other.m_isCollision)
{
	Safe_AddRef(m_pInputLayOut);
}

HRESULT CCollider::Initialize_Prototype(COLLTYPE eType)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_eType = eType;

	switch (m_eType)
	{
	case COLL_AABB: case COLL_OBB:
		m_pBB = new BoundingBox(_float3(0.0f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f));
		break;
	case COLL_SPHERE:
		m_pSphere = new BoundingSphere(_float3(0.0f, 0.f, 0.f), 0.5f);
		break;
	}

	m_pEffect = new BasicEffect(m_pDevice);
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->SetVertexColorEnabled(true);

	m_pBatch = new BATCH(m_pDeviceContext);
	if (nullptr == m_pBatch)
		return E_FAIL;

	const void*	pShaderByteCodes = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &ShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, 
		DirectX::VertexPositionColor::InputElementCount, pShaderByteCodes, ShaderByteCodeLength, &m_pInputLayOut)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCollider::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

	m_isCollision = false;

	_float3		vExtents, vCenter;

	_float3		vSphereCenter;
	_float		fRadius;

	switch (m_eType)
	{
	case CCollider::COLL_AABB:
		vExtents = _float3(m_ColliderDesc.vSize.x * 0.5f, m_ColliderDesc.vSize.y * 0.5f, m_ColliderDesc.vSize.z * 0.5f);
		vCenter = _float3(0.f, vExtents.y, 0.f);
		m_pBB = new BoundingBox(vCenter, vExtents);

		m_vMin = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vMax = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);;
		break;

	case CCollider::COLL_OBB:
		vExtents = _float3(m_ColliderDesc.vSize.x * 0.5f, m_ColliderDesc.vSize.y * 0.5f, m_ColliderDesc.vSize.z * 0.5f);
		vCenter = _float3(0.f, vExtents.y, 0.f);
		m_pBB = new BoundingBox(vCenter, vExtents);

		m_vPoint[0] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vPoint[1] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vPoint[2] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vPoint[3] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);

		m_vPoint[4] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vPoint[5] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vPoint[6] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vPoint[7] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		break;

	case CCollider::COLL_SPHERE:
		vSphereCenter = _float3(0.f,0.f, 0.f);
		fRadius = m_ColliderDesc.vSize.x;
		m_pSphere = new BoundingSphere(vSphereCenter, fRadius);
		break;
	}

	return S_OK;
}

HRESULT CCollider::Render()
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	m_pEffect->SetWorld(XMLoadFloat4x4(&m_TransformMatrix));

	m_pEffect->SetView(pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ));

	m_pDeviceContext->IASetInputLayout(m_pInputLayOut);
	m_pEffect->Apply(m_pDeviceContext);

	_vector		vColor = m_isCollision == true ? DirectX::Colors::Red : DirectX::Colors::Green;

	m_pBatch->Begin();

	switch (m_eType)
	{
	case CCollider::COLL_AABB: case CCollider::COLL_OBB:
		DX::Draw(m_pBatch, *m_pBB, vColor);
		break;

	case CCollider::COLL_SPHERE:
		DX::Draw(m_pBatch, *m_pSphere, vColor);
		break;
	}

	m_pBatch->End();

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}

_bool CCollider::Update_State(_fmatrix TransformMatrix)
{
	if (m_eType == CCollider::COLL_AABB || m_eType == CCollider::COLL_SPHERE)
		XMStoreFloat4x4(&m_TransformMatrix, Remove_ScaleRotation(TransformMatrix));

	else if (m_eType == CCollider::COLL_OBB )
		XMStoreFloat4x4(&m_TransformMatrix, Remove_Scale(TransformMatrix));

	return _bool();
}

_bool CCollider::Collision_AABB(CCollider * pTargetCollider)
{
	if (COLL_AABB != m_eType ||
		COLL_AABB != pTargetCollider->m_eType)
		return false;

	_vector		vSourMin, vSourMax, vDestMin, vDestMax;

	vSourMin = XMVector3TransformCoord(XMLoadFloat3(&m_vMin), XMLoadFloat4x4(&m_TransformMatrix));
	vSourMax = XMVector3TransformCoord(XMLoadFloat3(&m_vMax), XMLoadFloat4x4(&m_TransformMatrix));

	vDestMin = XMVector3TransformCoord(XMLoadFloat3(&pTargetCollider->m_vMin), XMLoadFloat4x4(&pTargetCollider->m_TransformMatrix));
	vDestMax = XMVector3TransformCoord(XMLoadFloat3(&pTargetCollider->m_vMax), XMLoadFloat4x4(&pTargetCollider->m_TransformMatrix));


	/* 너비비교 */
	if (max(XMVectorGetX(vSourMin), XMVectorGetX(vDestMin)) >
		min(XMVectorGetX(vSourMax), XMVectorGetX(vDestMax)))
	{
		m_isCollision = false;
		pTargetCollider->m_isCollision = false;
		return false;
	}

	/* 높이비교 */
	if (max(XMVectorGetY(vSourMin), XMVectorGetY(vDestMin)) >
		min(XMVectorGetY(vSourMax), XMVectorGetY(vDestMax)))
	{
		m_isCollision = false;
		pTargetCollider->m_isCollision = false;
		return false;
	}

	/* 깊이비교 */
	if (max(XMVectorGetZ(vSourMin), XMVectorGetZ(vDestMin)) >
		min(XMVectorGetZ(vSourMax), XMVectorGetZ(vDestMax)))
	{
		m_isCollision = false;
		pTargetCollider->m_isCollision = false;
		return false;
	}

	m_isCollision = true;
	pTargetCollider->m_isCollision = true;

	return true;
}

_bool CCollider::Collision_OBB(CCollider * pTargetCollider)
{
	_vector			vSourPoint[8], vDestPoint[8];

	for (_uint i = 0; i < 8; ++i)
	{
		vSourPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vPoint[i]), XMLoadFloat4x4(&m_TransformMatrix));
		vDestPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&pTargetCollider->m_vPoint[i]), XMLoadFloat4x4(&pTargetCollider->m_TransformMatrix));
	}

	OBBDESC		ObbDesc[2];
	ObbDesc[0] = Compute_OBB(vSourPoint);
	ObbDesc[1] = Compute_OBB(vDestPoint);

	_float		fDistance[3] = { 0.0f };

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[0]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[1]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[2]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[0]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[1]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[2]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenter) - XMLoadFloat3(&ObbDesc[0].vCenter), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			if (fDistance[0] + fDistance[1] < fDistance[2])
			{
				m_isCollision = false;
				pTargetCollider->m_isCollision = false;
				return false;
			}
		}
	}

	m_isCollision = true;
	pTargetCollider->m_isCollision = true;

	return true;
}

_bool CCollider::Collision_Sphere(CCollider * pTargetCollider)
{
	if (COLL_SPHERE != m_eType ||
		COLL_SPHERE != pTargetCollider->m_eType)
		return false;

	_vector		vMyCenter, vTargetCenter;
	_float		fMyRadius, fTargetRadius;

	_float3		vDiff;
	_float		fDistance;


	vMyCenter = XMVector3TransformCoord(XMLoadFloat3(&m_pSphere->Center), XMLoadFloat4x4(&m_TransformMatrix));
	vTargetCenter = XMVector3TransformCoord(XMLoadFloat3(&pTargetCollider->m_pSphere->Center), XMLoadFloat4x4(&pTargetCollider->m_TransformMatrix));

	fMyRadius = m_pSphere->Radius;
	fTargetRadius = pTargetCollider->m_pSphere->Radius;


	XMStoreFloat3(&vDiff, (vTargetCenter - vMyCenter));
	fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vDiff)));

	if (fDistance <= (fMyRadius + fTargetRadius))
	{
		m_isCollision = true;
		pTargetCollider->m_isCollision = true;

		return true;
	}
	else
	{
		m_isCollision = false;
		pTargetCollider->m_isCollision = false;

		return false;
	}

}

_bool CCollider::Collision_OBB(CObj * _pObj, const wstring & _Layertag)
{
	_vector			vSourPoint[8], vDestPoint[8];

	CManagement* pManagement = GET_INSTANCE(CManagement);
	auto iter_find = pManagement->Get_GameObj(_Layertag);

	for (auto& iter : iter_find->GetGameObjList())		// 레이어에 있는 클론객체리스트
	{

		CCollider*	pTargetCollider = static_cast<CCollider*>(iter->GetComponent(L"Com_OBB"));

		if (COLL_OBB != m_eType ||
			COLL_OBB != pTargetCollider->m_eType)
			continue;

		for (_uint i = 0; i < 8; ++i)
		{
			vSourPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vPoint[i]), XMLoadFloat4x4(&m_TransformMatrix));
			vDestPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&pTargetCollider->m_vPoint[i]), XMLoadFloat4x4(&pTargetCollider->m_TransformMatrix));
		}

		OBBDESC		ObbDesc[2];
		ObbDesc[0] = Compute_OBB(vSourPoint);
		ObbDesc[1] = Compute_OBB(vDestPoint);

		_float		fDistance[3] = { 0.0f };

		for (_uint i = 0; i < 2; ++i)
		{
			for (_uint j = 0; j < 3; ++j)
			{
				fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[0]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
					fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[1]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
					fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[2]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

				fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[0]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
					fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[1]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
					fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[2]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

				fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenter) - XMLoadFloat3(&ObbDesc[0].vCenter), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

				if (fDistance[0] + fDistance[1] < fDistance[2])
				{
					m_isCollision = false;
					pTargetCollider->m_isCollision = false;

					RELEASE_INSTANCE(CManagement);
					return false;
				}
			}
		}
		m_isCollision = true;
		pTargetCollider->m_isCollision = true;
		RELEASE_INSTANCE(CManagement);
		return true;
	}

}

_bool CCollider::Collision_Sphere(CObj * _pObj, const wstring & _Layertag, CObj *& Out, _float3& OutPos)
{
	CManagement* pManagement = GET_INSTANCE(CManagement);
	auto iter_find = pManagement->Get_GameObj(_Layertag);

	for (auto& iter : iter_find->GetGameObjList())		// 레이어에 있는 클론객체리스트
	{

		CCollider*	pTargetCollider = static_cast<CCollider*>(iter->GetComponent(L"Com_SPHERE"));

		if (COLL_SPHERE != m_eType ||
			COLL_SPHERE != pTargetCollider->m_eType)
			 return false;

		if (pTargetCollider->GetColliderDesc().eObjState == OBJ_STICK)
		{
			m_isCollision = false;
			continue;
		}

		_vector		vMyCenter, vTargetCenter;
		_float		fMyRadius, fTargetRadius;

		_float3		vDiff;		// 
		_float		fDistance;

		vMyCenter = XMVector3TransformCoord(XMLoadFloat3(&m_pSphere->Center), XMLoadFloat4x4(&m_TransformMatrix));
		vTargetCenter = XMVector3TransformCoord(XMLoadFloat3(&pTargetCollider->m_pSphere->Center), XMLoadFloat4x4(&pTargetCollider->m_TransformMatrix));

		fMyRadius = m_pSphere->Radius;
		fTargetRadius = pTargetCollider->m_pSphere->Radius;


		XMStoreFloat3(&vDiff, (vTargetCenter - vMyCenter));
		fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vDiff)));

		_vector vDir, vCenter;

		vDir = XMLoadFloat3(&vDiff);
		vCenter = XMLoadFloat3(&m_pSphere->Center);

		if (fDistance <= (fMyRadius + fTargetRadius))
		{
			m_isCollision = true;
			pTargetCollider->m_isCollision = true;

			Out = iter->GetThis();
			XMStoreFloat3(&OutPos , -fMyRadius * vDir + vCenter);

			RELEASE_INSTANCE(CManagement);
			return true;
		}
		else
		{
			pTargetCollider->m_isCollision = false;
		}
	}
	m_isCollision = false;
	RELEASE_INSTANCE(CManagement);
	return false;
}

void CCollider::Set_Scale(_fvector vScale)
{
	_vector		vRight = XMVector3Normalize(*(_vector*)&m_TransformMatrix.m[0][0]) * XMVectorGetX(vScale);
	_vector		vUp = XMVector3Normalize(*(_vector*)&m_TransformMatrix.m[1][0]) * XMVectorGetY(vScale);
	_vector		vLook = XMVector3Normalize(*(_vector*)&m_TransformMatrix.m[2][0]) * XMVectorGetZ(vScale);

	memcpy(&m_TransformMatrix.m[0][0], &vRight, sizeof(_float3));
	memcpy(&m_TransformMatrix.m[1][0], &vUp, sizeof(_float3));
	memcpy(&m_TransformMatrix.m[2][0], &vLook, sizeof(_float3));
}

void CCollider::Set_Points(const _float3 & ColSize)
{
	m_ColliderDesc.vSize = ColSize;

	m_pBB->Extents = _float3(m_ColliderDesc.vSize.x * 0.5f, m_ColliderDesc.vSize.y * 0.5f, m_ColliderDesc.vSize.z * 0.5f);
	m_pBB->Center = _float3(0.f, m_pBB->Extents.y, 0.f);

	m_vPoint[0] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
	m_vPoint[1] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
	m_vPoint[2] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
	m_vPoint[3] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);

	m_vPoint[4] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
	m_vPoint[5] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
	m_vPoint[6] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
	m_vPoint[7] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
}

void CCollider::Set_Radius(const _float fRadius)
{
	m_ColliderDesc.fRadius = fRadius;

	m_pSphere->Radius = m_ColliderDesc.fRadius;
}

_fmatrix CCollider::Remove_ScaleRotation(_fmatrix TransformMatrix)
{
	_matrix			NonRotateMatrix = XMMatrixIdentity();

	NonRotateMatrix.r[3] = TransformMatrix.r[3];

	return NonRotateMatrix;
}

_fmatrix CCollider::Remove_Scale(_fmatrix TransformMatrix)
{
	_matrix			NonScaleMatrix = Remove_ScaleRotation(TransformMatrix);

	NonScaleMatrix.r[0] = XMVector3Normalize(TransformMatrix.r[0]);
	NonScaleMatrix.r[1] = XMVector3Normalize(TransformMatrix.r[1]);
	NonScaleMatrix.r[2] = XMVector3Normalize(TransformMatrix.r[2]);

	return NonScaleMatrix;
}

CCollider::OBBDESC CCollider::Compute_OBB(_fvector * pPoints)
{
	OBBDESC			OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));

	XMStoreFloat3(&OBBDesc.vCenter, (pPoints[0] + pPoints[6]) * 0.5f);

	XMStoreFloat3(&OBBDesc.vAlignAxis[0], XMVector3Normalize(pPoints[5] - pPoints[4]));
	XMStoreFloat3(&OBBDesc.vAlignAxis[1], XMVector3Normalize(pPoints[5] - pPoints[6]));
	XMStoreFloat3(&OBBDesc.vAlignAxis[2], XMVector3Normalize(pPoints[5] - pPoints[1]));

	XMStoreFloat3(&OBBDesc.vCenterAxis[0], ((pPoints[5] + pPoints[2]) * 0.5f) - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[1], ((pPoints[5] + pPoints[0]) * 0.5f) - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[2], ((pPoints[5] + pPoints[7]) * 0.5f) - XMLoadFloat3(&OBBDesc.vCenter));

	return OBBDesc;
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

	if (false == m_isCloned)
	{
		Safe_Delete(m_pEffect);
		Safe_Delete(m_pBatch);
	}

	Safe_Delete(m_pBB);
	Safe_Delete(m_pSphere);

	Safe_Release(m_pInputLayOut);
}
