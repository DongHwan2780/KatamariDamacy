#pragma once

#ifndef __MANAGEMENT_H__
#define __MANAGEMENT_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CManagement final : public CBase		// Management == GameInstance
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement() = default;

#pragma region TIMEMGR
public:
	HRESULT Add_Timers(const _tchar* TimeTag);
	_double	Compute_DeltaTime(const _tchar* TimeTag);
#pragma endregion

#pragma region GRAPHIC_DEVICE
	HRESULT Ready_GraphicDevice(HWND hWnd, _uint iWidth, _uint iHeight, ID3D11Device** ppOutDevice, ID3D11DeviceContext** ppOutDeviceContext);
	HRESULT Clear_BackBufferView(_float4 vColor);
	HRESULT Clear_DepthStencilView(_float fDepth, _uint iStencil);
	HRESULT Present();
#pragma endregion

#pragma region OBJMGR

#pragma endregion

#pragma region COMPONENT

#pragma endregion

#pragma region KEYMGR

#pragma endregion

#pragma region COLLISION

#pragma endregion

#pragma region SCENE
	HRESULT	Set_CurScene(class CScene* pCurScene);
	_int	Update_Scene(_double DeltaTime);
	HRESULT	Render_Scene();
#pragma endregion

public:
	static void Release_Engine();

public:
	virtual void Free() override;

private:
	class CTimeMgr*		m_pTimeMgr = nullptr;
	class CGraphic*		m_pGraphicDevice = nullptr;
	class CSceneMgr*	m_pSceneMgr = nullptr;
};

END
#endif // !__MANAGEMENT_H__
