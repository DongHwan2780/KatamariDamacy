#include "..\Headers\TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
{
}


HRESULT CTimeMgr::Add_Timers(const _tchar * TimeTag)
{
	if (nullptr != Find_Timers(TimeTag))		//	Ű������ Ÿ�̸Ӹ� ã���� �� �̹� ������ ���� ��ȯ
		return E_FAIL;

	CTimer*		pTimer = CTimer::Create();		// ������ ���������ְ�
	if (nullptr == pTimer)						// ������ �ȉ����� ���� ��ȯ
		return E_FAIL;

	m_pTimers.insert(TIMERS::value_type(TimeTag, pTimer));  // ���������� �־���

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
