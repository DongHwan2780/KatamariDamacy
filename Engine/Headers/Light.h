#pragma once

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
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
