#pragma once

#include "ClientDefines.h"
#include "Camera.h"
#include "Player.h"

BEGIN(Client)

class CMainCamera final : public CCamera
{
public:
	explicit CMainCamera(DEVICES);
	explicit CMainCamera(const CMainCamera&	other);
	virtual ~CMainCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;

private:
	HRESULT SetUp_Components();

private:
	_uint Movement(_double DeltaTime);

public:
	static CMainCamera* Create(DEVICES);
	virtual CObj* Clone(void* pArg) override;
	virtual void Free() override;

private:
	CPlayer*		m_pPlayer = nullptr;
	CTransform*		m_pPlayerTransform = nullptr;

	_float			m_fDistanceToTarget = 5.f;
	_float			m_fCamAngle = XMConvertToRadians(15.f);
};

END