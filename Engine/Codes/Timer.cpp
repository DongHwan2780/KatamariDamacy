#include "..\Headers\Timer.h"



CTimer::CTimer()
{
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FixTime);		// 시작 지점의 CPU의 틱을 받아옴
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_LastTime);

	QueryPerformanceFrequency(&m_CpuTick);		// 타이머의 주파수 반환

	return S_OK;
}

_float CTimer::Compute_DeltaTime()
{
	QueryPerformanceCounter(&m_FrameTime);		// 지금(함수가 불린 시점)의 틱값을 받아옴

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)	
	{
		m_FixTime = m_FrameTime;
		QueryPerformanceFrequency(&m_CpuTick);
	}

	m_fDeltaTime = (_float)(m_FrameTime.QuadPart) - m_LastTime.QuadPart / m_CpuTick.QuadPart;		// 작업하는데 걸린 시간 / 초당 진동수

	m_LastTime = m_FrameTime;

	return _float(m_fDeltaTime);
}

CTimer * CTimer::Create()
{
	CTimer* pInstance = new CTimer();

	if (FAILED(pInstance->Ready_Timer()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimer::Free()
{
}

