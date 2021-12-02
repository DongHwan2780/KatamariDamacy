#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ClientDefines.h"
#include "Obj.h"

BEGIN(Engine)
class CTransform;
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
	HRESULT SetUp_Components();

public:
	static CPlayer*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
};
END
#endif // !__PLAYER_H__
