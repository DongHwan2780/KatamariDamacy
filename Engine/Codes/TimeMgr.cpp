#include "..\Headers\TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
{
}


HRESULT CTimeMgr::Add_Timers(const _tchar * TimeTag)
{
	if (nullptr != Find_Timers(TimeTag))		//	키값으로 타이머를 찾았을 때 이미 있으면 실패 반환
		return E_FAIL;

	CTimer*		pTimer = CTimer::Create();		// 없으면 생성시켜주고
	if (nullptr == pTimer)						// 생성이 안됬으면 실패 반환
		return E_FAIL;

	m_pTimers.insert(TIMERS::value_type(TimeTag, pTimer));  // 생성했으면 넣어줌

	return S_OK;
}

_float CTimeMgr::Compute_DeltaTime(const _tchar * TimeTag)
{
	CTimer*	pTimer = Find_Timers(TimeTag);

	if (nullptr == pTimer)
		return (float)E_FAIL;

	return pTimer->Compute_DeltaTime();
}

CTimer * CTimeMgr::Find_Timers(const _tchar * TimeTag)
{
	auto	iter = find_if(m_pTimers.begin(), m_pTimers.end(), CTagFinder(TimeTag));

	if (iter == m_pTimers.end())
		return nullptr;

	return iter->second;
}

void CTimeMgr::Free()
{
	for (auto& Pair : m_pTimers)
		Safe_Release(Pair.second);

	m_pTimers.clear();
}
