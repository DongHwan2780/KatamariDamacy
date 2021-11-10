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
	LARGE_INTEGER	m_FrameTime;		// LARGE_INTEGER == 64��Ʈ ������ �����͸� �����ϱ� ���� ����� ����� ���� �ڷ���
	LARGE_INTEGER	m_FixTime;
	LARGE_INTEGER	m_LastTime;
	LARGE_INTEGER	m_CpuTick;
};

END
#endif // !__TIMER_H__
