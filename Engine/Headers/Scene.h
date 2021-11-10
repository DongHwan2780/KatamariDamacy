#pragma once

#ifndef __SCENE_H__
#define __SCENE_H__


#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract : public CBase
{
protected:
	explicit CScene(ID3D11Device* pDevice, ID3D11DeviceContext*	pDeviceContext);
	virtual ~CScene() = default;

public:
	virtual HRESULT Initialize();
	virtual _int	Update(_double DeltaTime);
	virtual HRESULT Render();

public:
	virtual void Free() override;

protected:
	ID3D11Device*		m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
}; 

END
#endif // !__SCENE_H__
