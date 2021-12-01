#pragma once

#ifndef __BASE_H__
#define __BASE_H__

#include "EngineDefines.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract		// 레퍼런스 카운트 관리
{
protected:
	explicit CBase();
	virtual ~CBase() = default;

public:
	unsigned long AddRef();
	unsigned long Release();

public:
	virtual void Free() = 0;

private:
	unsigned long m_dwRefCnt = 0;
};

END

#endif // !__BASE_H__