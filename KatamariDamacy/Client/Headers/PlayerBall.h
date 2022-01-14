#pragma once

#ifndef __PLAYERBALL_H__
#define __PLAYERBALL_H__

#include "ClientDefines.h"
#include "Obj.h"
#include "Player.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
class CTransform;
class CModel;
END

BEGIN(Client)

class CPlayerBall final : public CObj
{
public:
	explicit CPlayerBall(DEVICES);
	explicit CPlayerBall(const CPlayerBall& other);
	virtual ~CPlayerBall() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	void SetTransform();

public:
	static CPlayerBall*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CModel*			m_pModel = nullptr;
	CCollider*		m_pCollider = nullptr;
	CCollider*		m_pColliderSphere = nullptr;

	CCollider*		pTargetCollider = nullptr;
private:
	_float			m_fBallSize = 0.f;
	CTransform*		m_pPlayerTransform = nullptr;

	_bool			m_bFirst = true;

};
END
#endif // !__PLAYERBALL_H__
