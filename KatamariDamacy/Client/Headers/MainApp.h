#pragma once

#include "ClientDefines.h"
#include "Base.h"

BEGIN(Engine)
class CManagement;
class CRenderer;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	_uint	Update(_double dDeltaTime);
	_uint	Late_Update();
	HRESULT Render();

public:
	static CMainApp*	Create();
	virtual void Free() override;

private:
	HRESULT Ready_Scene(SCENE eScene);
	HRESULT Ready_Prototype_Component();


private:
	CManagement*			m_pManagement = nullptr;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	CRenderer*				m_pRenderer = nullptr;

#ifdef _DEBUG
private:
	_tchar			m_szFPS[MAX_PATH] = TEXT("");
	_uint			m_iNumDraw = 0;
	_double			m_fDeltaTime = 0.0;
#endif
};

END