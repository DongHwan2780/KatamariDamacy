#pragma once

#include "ClientDefines.h"
#include "Scene.h"

BEGIN(Client)

class CStageOne final : public CScene
{
private:
	explicit CStageOne(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex);
	virtual ~CStageOne() = default;

public:
	virtual HRESULT Initialize() override;
	virtual _int	Update(_double DeltaTime) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const wstring& pLayerTag);
	HRESULT Ready_Layer_Camera(const wstring& pLayerTag);
	HRESULT Ready_Layer_StageMap(const wstring& pLayerTag);
	HRESULT Ready_Layer_Terrain(const wstring& pLayerTag);

	HRESULT Ready_Layer_Dummy(const wstring& pLayerTag);

	HRESULT Ready_Layer_StageObj();

private:
	HRESULT Ready_Prototype_Component();
	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Layer_SizeUI(const wstring& pLayerTag); 
	HRESULT Ready_Layer_StickyUI(const wstring& pLayerTag);
	HRESULT Ready_Layer_TimeUI(const wstring& pLayerTag);
	HRESULT Ready_Layer_PlayerMoveUI(const wstring& pLayerTag);

public:
	static CStageOne* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex);
	virtual void Free() override;
};

END