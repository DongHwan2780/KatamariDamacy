#pragma once

#include "ClientDefines.h"
#include "Scene.h"

BEGIN(Client)

class CLoading final : public CScene
{
private:
	explicit CLoading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLoading() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual _int	Update(_double DeltaTime) override;
	virtual HRESULT Render() override;

public:
	static CLoading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

END