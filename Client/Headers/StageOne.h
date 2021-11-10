#pragma once

#include "ClientDefines.h"
#include "Scene.h"

class CStageOne final : public CScene
{
private:
	explicit CStageOne(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CStageOne() = default;

public:
	virtual HRESULT Initialize() override;
	virtual _int	Update(_double DeltaTime) override;
	virtual HRESULT Render() override;

public:
	static CStageOne* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

