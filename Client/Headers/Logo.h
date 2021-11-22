#pragma once

#include "ClientDefines.h"
#include "Scene.h"

BEGIN(Client)
class CLogo final : public CScene
{
private:
	explicit CLogo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex);
	virtual ~CLogo() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual _int	Update(_double DeltaTime) override;
	virtual HRESULT Render() override;

public:
	static CLogo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex);
	virtual void Free() override;
};
END
