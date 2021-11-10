#pragma once

#ifndef __LAYERMGR_H__
#define __LAYERMGR_H__

#include "Base.h"

BEGIN(Engine)

class CLayerMgr final : public CBase
{
	DECLARE_SINGLETON(CLayerMgr)
private:
	CLayerMgr();
	virtual ~CLayerMgr() = default;

public:
	virtual void Free() override;
};

END
#endif // !__LAYERMGR_H__
