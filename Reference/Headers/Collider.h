#pragma once

#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Component.h"

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

private:
	explicit CCollider(DEVICES);
	explicit CCollider(const CCollider& other);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(COLLTYPE eType);
	virtual HRESULT Initialize_Clone(void * pArg) override;

public:
	static CCollider* Create(DEVICES, COLLTYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	COLLTYPE				m_eType = COLL_END;
	COLLIDERDESC			m_ColliderDesc;

	BoundingBox*			m_pAABB = nullptr;
	BoundingOrientedBox*	m_pOBB = nullptr;
	BoundingSphere*			m_pSphere = nullptr;
};
END
#endif // !__COLLIDER_H__
