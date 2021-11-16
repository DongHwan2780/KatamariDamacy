#pragma once

#ifndef __MANAGEMENT_H__
#define __MANAGEMENT_H__

#include "Base.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "ComponentMgr.h"

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

#pragma region OBJ_MGR
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CObj* pPrototype);
	HRESULT Add_GameObj(_int iSceneIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
	void	Clear_ObjMgr(_int iSceneIndex);
#pragma endregion

#pragma region COMPONENT_MGR
	HRESULT Add_Prototype(_int iSceneIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_int iSceneIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
	void Clear_ComponentMgr(_int iSceneIndex);
#pragma endregion

#pragma region KEYMGR

#pragma endregion

#pragma region COLLISION_MGR

#pragma endregion

#pragma region SCENE_MGR
	HRESULT	Set_CurScene(class CScene* pCurScene);
	_int	Update_Scene(_double DeltaTime);
	HRESULT	Render_Scene();
#pragma endregion

public:
	HRESULT Initialize_Engine(_int iNumScenes);
	_int	Update(_double DeltaTime);
	static void Release_Engine();

public:
	virtual void Free() override;

private:
	class CTimeMgr*			m_pTimeMgr = nullptr;
	class CGraphic*			m_pGraphicDevice = nullptr;
	class CSceneMgr*		m_pSceneMgr = nullptr;
	class CObjMgr*			m_pObjMgr = nullptr;
	class CComponentMgr*	m_pComponentMgr = nullptr;
};

END
#endif // !__MANAGEMENT_H__
