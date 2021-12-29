#include "..\Headers\TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
{
}


HRESULT CTimeMgr::Add_Timers(const wstring& TimeTag)
{
	if (nullptr != Find_Timers(TimeTag))		//	Ű������ Ÿ�̸Ӹ� ã���� �� �̹� ������ ���� ��ȯ
		return E_FAIL;

	CTimer*		pTimer = CTimer::Create();		// ������ ���������ְ�
	if (nullptr == pTimer)						// ������ �ȉ����� ���� ��ȯ
		return E_FAIL;

	m_pTimers.insert(TIMERS::value_type(TimeTag, pTimer));  // ���������� �־���

	return S_OK;
}

_double CTimeMgr::Compute_DeltaTime(const wstring& TimeTag)
{
	CTimer*	pTimer = Find_Timers(TimeTag);

	if (nullptr == pTimer)
		return (float)E_FAIL;

	return pTimer->Compute_DeltaTime();
}

CTimer * CTimeMgr::Find_Timers(const wstring& TimeTag)
{
	auto iter_find = m_pTimers.find(TimeTag);

	if (iter_find == m_pTimers.end())
		return nullptr;

	return iter_find->second;
}

void CTimeMgr::Free()
{
	for (auto& Pair : m_pTimers)
		Safe_Release(Pair.second);

	m_pTimers.clear();
}
