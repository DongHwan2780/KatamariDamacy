#pragma once

#ifndef __TIMEMGR_H__
#define __TIMEMGR_H__

#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class CTimeMgr final : public CBase
{
	DECLARE_SINGLETON(CTimeMgr)
private:
	explicit CTimeMgr();
	virtual ~CTimeMgr() = default;

public:
	HRESULT Add_Timers(const _tchar* TimeTag);
	_double	Compute_DeltaTime(const _tchar* TimeTag);

private:
	CTimer* Find_Timers(const _tchar* TimeTag);

public:
	virtual void Free();

private:
	typedef unordered_map<const _tchar*, CTimer*> TIMERS;		//TIMERS	m_pTimers;  -> 키값이 const라서 못쓰는거같음
	unordered_map<const _tchar*, CTimer*>	m_pTimers;

	//TIMERS		m_pTimers;
};

END

#endif // !__TIMEMGR_H__
