#pragma once

#ifndef __OBJMGR_H__
#define __OBJMGR_H__


#include "Base.h"

BEGIN(Engine)

class CObjMgr final : public CBase
{
private:
	CObjMgr();
	virtual ~CObjMgr() = default;
};

END
#endif // !__OBJMGR_H__
