#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Obj.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CObj
{
protected:
	explicit CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamera(const CCamera& other);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual _int	Update(_double DeltaTime);
	virtual _int	Late_Update(_double DeltaTime);
	virtual HRESULT	Render();

public:
	virtual CObj*	Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
#endif // !__CAMERA_H__
