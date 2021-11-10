#pragma once

#ifndef __TIMER_H__
#define __TIMER_H__

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;

public:
	HRESULT Ready_Timer();
	_double  Compute_DeltaTime();

public:
	static CTimer* Create();
	virtual void Free();

private:
	_double	m_fDeltaTime = 0.0;

private:
	LARGE_INTEGER	m_FrameTime;		// LARGE_INTEGER == 64비트 정수형 데이터를 저장하기 위해 선언된 사용자 정의 자료형
	LARGE_INTEGER	m_FixTime;
	LARGE_INTEGER	m_LastTime;
	LARGE_INTEGER	m_CpuTick;
};

END
#endif // !__TIMER_H__
