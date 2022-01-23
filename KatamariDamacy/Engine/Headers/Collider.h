#pragma once

#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Component.h"
#include "DebugDraw.h"

//#include "../../Client/Headers/ClientDefines.h"

BEGIN(Engine)

class CLayer;
class CObj;

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum COLLTYPE { COLL_AABB, COLL_OBB, COLL_SPHERE, COLL_END} ;
	enum OBJSTATE { OBJ_NONE, OBJ_PLAYERBALL, OBJ_WALL, OBJ_STICK, OBJ_END};

	// OBJ_NONE 붙기전, OBJ_PLAYERBALL 플레이어공, OBJ_STICK 붙어있는오브젝트

public:
	typedef struct tagColliderDesc
	{
		OBJSTATE	eObjState = OBJ_END;
		_float3		vSize = _float3(1.f, 1.f, 1.f);
		_float3		vPos;
		_float		fRadius;
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
	_bool Collision_Sphere(CCollider* pTargetCollider);


	_bool Collision_OBB(CObj * _pObj, const wstring & _Layertag);
	_bool Collision_Sphere(CObj * _pObj, const wstring & _Layertag, CObj*& Out, _float3& OutPos);



	const COLLIDERDESC& GetColliderDesc() const { return m_ColliderDesc; }
	void Set_Size(const _float3& ColSize) { m_ColliderDesc.vSize = ColSize; }
	void Set_Scale(_fvector vScale);
	void Set_Points(const _float3& ColSize);
	void Set_Radius(CObj* _pObj, const _float fRadius);

	void Set_CollState(const OBJSTATE ObjState) { m_ColliderDesc.eObjState = ObjState; }
	void Set_CollPos(const _float3 CollPos) { m_ColliderDesc.vPos = CollPos; }

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


protected:
	unordered_map<wstring, class CLayer*>*			m_pGameObjects = nullptr;		// 클론
	typedef unordered_map<wstring, class CLayer*>		GAMEOBJECTS;

public:
	void SetLayers(unordered_map<wstring, class CLayer*>* _pLayer) { m_pGameObjects = _pLayer; }


	CLayer*	m_pLayer = nullptr;
};
END
#endif // !__COLLIDER_H__
