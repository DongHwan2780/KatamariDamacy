#pragma once

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase		// 빛의 정보를 보관하기 위한 클래스, 빛 객체 하나당 하나의 정보만 가지고 있을거임
{
private:
	explicit CLight(DEVICES);
	virtual ~CLight() = default;

public:
	const LIGHTDESC* Get_LightDesc() const { return &m_LightDesc; }

public:
	HRESULT Initialize_Clone(const LIGHTDESC& LightDesc);

public:
	static CLight* Create(DEVICES, const LIGHTDESC& LightDesc);
	virtual void Free() override;

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	LIGHTDESC				m_LightDesc;
};

END
#endif // !__LIGHT_H__
