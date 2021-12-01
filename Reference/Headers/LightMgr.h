#pragma once

#ifndef __LIGHTMGR_H__
#define __LIGHTMGR_H__

#include "Base.h"

BEGIN(Engine)

class CLightMgr final : public CBase		// ºû °´Ã¼µé º¸°ü¿ë
{
	DECLARE_SINGLETON(CLightMgr)
public:
	CLightMgr();
	virtual ~CLightMgr() = default;

public:
	const LIGHTDESC*	Get_LightDesc(_uint Index = 0) const;

public:
	HRESULT Add_Light(DEVICES, const LIGHTDESC& LightDesc);

public:
	virtual void Free() override;

private:
	list<class CLight*>			m_Lights;
	typedef list<class CLight*>	LIGHTS;
};
END
#endif // !__LIGHTMGR_H__
