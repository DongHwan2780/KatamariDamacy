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

private:
	HRESULT Ready_Prototype_Component();
	HRESULT Ready_Prototype_GameObject(); /* 로고에서 사용할 객체의 원형을 생성한다. (다른 씬의 경ㅇ우, 로더에서 구현될 내용.) */
	HRESULT Ready_Layer_BackGround(const wstring& pLayerTag, _uint iSceneIndex); /* 복제본을 생성하여 레이어에 추가하는 형태 .*/

public:
	static CLogo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex);
	virtual void Free() override;
};
END
