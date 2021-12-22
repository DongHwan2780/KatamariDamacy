#pragma once

#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum COLLTYPE { COLL_AABB, COLL_OBB, COLL_SPHERE, COLL_END} ;
public:
	typedef struct tagColliderDesc
	{
		_float3		vSize = _float3(1.f, 1.f, 1.f);
	} COLLIDERDESC;

	typedef struct tagOBBDesc
	{
		_float3		vCenter;
		_float3		vAlignAxis[3];
		_float3		vCenterAxis[3];
	}OBBDESC;

private:
	explicit CCollider(DEVICES);
	explicit CCollider(const CCollider& other);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(COLLTYPE eType);
	virtual HRESULT Initialize_Clone(void * pArg) override;
	HRESULT Render();

public:
	_bool Update_State(_fmatrix TransformMatrix);
	_bool Collision_AABB(CCollider* pTargetCollider);
	_bool Collision_OBB(CCollider* pTargetCollider);

private:
	_fmatrix Remove_ScaleRotation(_fmatrix TransformMatrix);
	_fmatrix Remove_Scale(_fmatrix TransformMatrix);

	OBBDESC Compute_OBB(_fvector* pPoints);

public:
	static CCollider* Create(DEVICES, COLLTYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	COLLTYPE				m_eType = COLL_END;
	COLLIDERDESC			m_ColliderDesc;

	BoundingBox*			m_pBB = nullptr;
	BoundingSphere*			m_pSphere = nullptr;
	_bool					m_isCollision = false;

	_float4x4				m_TransformMatrix;

	_float3					m_vMin, m_vMax;
	_float3					m_vPoint[8];

private:
	BasicEffect*			m_pEffect = nullptr;
	ID3D11InputLayout*		m_pInputLayOut = nullptr;

private:
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>*				m_pBatch = nullptr;
	typedef DirectX::PrimitiveBatch<DirectX::VertexPositionColor>		BATCH;
};
END
#endif // !__COLLIDER_H__
