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
	HRESULT Ready_Prototype_GameObject(); /* �ΰ��� ����� ��ü�� ������ �����Ѵ�. (�ٸ� ���� �椷��, �δ����� ������ ����.) */
	HRESULT Ready_Layer_BackGround(const wstring& pLayerTag, _uint iSceneIndex); /* �������� �����Ͽ� ���̾ �߰��ϴ� ���� .*/

public:
	static CLogo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex);
	virtual void Free() override;
};
END
