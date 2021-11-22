#pragma once

#include "ClientDefines.h"
#include "Scene.h"

BEGIN(Client)

class CLoading final : public CScene
{
private:
	explicit CLoading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex);
	virtual ~CLoading() = default;

public:
	virtual HRESULT	Initialize(SCENE eScene);
	virtual _int	Update(_double DeltaTime) override;
	virtual HRESULT Render() override;

public:
	static CLoading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, SCENE eScene, _uint iSceneIndex);
	virtual void Free() override;

private:
	class CLoader*		m_pLoader = nullptr;

private:
	SCENE	m_eNextScene;
};

END