#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ClientDefines.h"
#include "Obj.h"
#include "PlayerBall.h"
#include "Management.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
class CTransform;
class CModel;
END

BEGIN(Client)

class CPlayer final : public CObj
{
public:
	explicit CPlayer(DEVICES);
	explicit CPlayer(const CPlayer& other);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;

private:
	void Movement(_double DeltaTime);
	void Acceleration(_double DeltaTime, _float Ballsize);
	void ResistAccel(_double DeltaTime);
	void Gravity(_double DeltaTime);

	void Around_Ball(_float Orbit);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	HRESULT Ready_Layer_PlayerBall(const wstring& pLayerTag);

public:
	static CPlayer*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CModel*			m_pModel = nullptr;
	CCollider*		m_pCollider = nullptr;

	CTransform*		m_pPlayerBallTransform = nullptr;
	CVIBuffer_Terrain* m_pVIBuffer = nullptr;
private:

	// 속도  m_pTransform.speed = ballsize * accel
	_float			m_fAccel = 0.f;		// 공 크기에 따라 값이 변경되어야함 -> 공이 크면 클수록 작게, 공이 작을수록 크게
	_float			m_fBackAccel = 0.f;
	_float			m_fMaxSpeed = 0.f;
	_float			m_fBackMaxSpeed = 0.f;

	_float			m_fLeftAccel = 0.f;		// 공 크기에 따라 값이 변경되어야함 -> 공이 크면 클수록 작게, 공이 작을수록 크게
	_float			m_fRightAccel = 0.f;
	_float			m_fLeftMaxSpeed = 0.f;
	_float			m_fRightMaxSpeed = 0.f;



	_float			m_fResist = 0.f;		// 저항값 == 공 크기 * 중력 가속 + 플레이어 속도
	_float			m_fResistance = 2.f;

	_float			m_fBallSize = 0.f;

	//중력
	_float			m_fGravityTime = 0.f;
	_float			m_fGravityY = 0.f;

	//충돌
	_float3			m_vReactionDir = { 0.f, 0.f, 0.f };
	_float			m_fReactionPower = 0.f;

	_bool			m_bFistCreate = true;

};
END
#endif // !__PLAYER_H__
