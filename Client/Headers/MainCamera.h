#pragma once

#include "ClientDefines.h"
#include "Camera.h"

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

public:
	static CMainCamera* Create(DEVICES);
	virtual CObj* Clone(void* pArg) override;
	virtual void Free() override;

};

END