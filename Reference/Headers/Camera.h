#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Obj.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CObj
{
public:
	typedef struct CameraDesc
	{
		_float3		vEye;
		_float3		vAt;
		_float3		vAxisY = _float3(0.f, 1.f, 0.f);
		CTransform::TRANSFORMDESC		TransformDesc;
	}CAMERADESC;
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

protected:
	class CTransform*		m_pTransform = nullptr;
	CAMERADESC				m_CameraDesc;
	class CPipeLine*		m_pPipeline = nullptr;
};

END
#endif // !__CAMERA_H__
