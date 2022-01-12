#pragma once

#ifndef __OBJLIST_H__
#define __OBJLIST_H__

#include "ClientDefines.h"
#include "Obj.h"
#include "Player.h"
#include "PlayerBall.h"
#include "Camera.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CObjList abstract : public CObj
{
public:
	explicit CObjList(DEVICES);
	explicit CObjList(const CObjList& other);
	virtual ~CObjList() = default;

public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize_Clone(void* pArg) PURE;
	virtual _int	Update(_double DeltaTime) PURE;
	virtual _int	Late_Update(_double DeltaTime) PURE;
	virtual HRESULT	Render() PURE;

public:
	void Gravity(_double DeltaTime);
	void OnLandCheck();

public:
	void Coll_PlayerBall();

public:
	_float	Get_PlayerBallSize() {}

public:
	virtual CObj*	Clone(void* pArg) PURE;
	virtual void Free() PURE;

protected:
	// 오브젝트 공통 사용 변수
	_float		m_fObjSize = 0.f;					// 각 오브젝트 사이즈, 비교해서 붙을건지 튕겨나가게할건지
	_bool		m_bStickCheck = false;

	//중력
	_float m_fGravityTime = 0.f;
	_float m_fGravityY = 0.f;

	CTransform*		m_pPlayerBallTransform = nullptr;
};


END
#endif // !__OBJLIST_H__
