#include "..\Headers\Collider.h"

#include "PipeLine.h"

CCollider::CCollider(DEVICES)
	:CComponent(pDevice, pDeviceContext)
{
}

CCollider::CCollider(const CCollider & other)
	:CComponent(other)
	, m_eType(other.m_eType)
	, m_pEffect(other.m_pEffect)
	, m_pBatch(other.m_pBatch)
	, m_pAABB(other.m_pAABB)
	, m_pOBB(other.m_pOBB)
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

	m_isCollision = true;

	switch (m_eType)
	{
	case CCollider::COLL_AABB: case CCollider::COLL_OBB:
		m_pAABB->Extents = _float3(m_ColliderDesc.vSize.x * 0.5f, m_ColliderDesc.vSize.y * 0.5f, m_ColliderDesc.vSize.z * 0.5f);
		m_pAABB->Center = _float3(0.f, m_pAABB->Extents.y, 0.f);	// 높이 보정
		break;

	case CCollider::COLL_SPHERE:
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
	case CCollider::COLL_AABB:
		DX::Draw(m_pBatch, *m_pAABB, vColor);
		break;
	case CCollider::COLL_OBB:
		DX::Draw(m_pBatch, *m_pOBB, vColor);
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
	if (m_eType == CCollider::COLL_AABB)
		XMStoreFloat4x4(&m_TransformMatrix, Remove_Rotation(TransformMatrix));

	return _bool();
}

_bool CCollider::Collision_AABB(const CCollider * pTargetCollider)
{
	return _bool();
}

_fmatrix CCollider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix			NonRotateMatrix = XMMatrixIdentity();

	NonRotateMatrix.r[3] = TransformMatrix.r[3];

	return NonRotateMatrix;
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
		Safe_Delete(m_pAABB);
		Safe_Delete(m_pOBB);
		Safe_Delete(m_pSphere);

		Safe_Delete(m_pEffect);
		Safe_Delete(m_pBatch);

	}
	Safe_Release(m_pInputLayOut);
}
