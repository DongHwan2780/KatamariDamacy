#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ClientDefines.h"
#include "Obj.h"

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
	void Movement(_double TimeDelta);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

public:
	static CPlayer*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CModel*			m_pModel = nullptr;
	CCollider*			m_pCollider = nullptr;
};
END
#endif // !__PLAYER_H__
