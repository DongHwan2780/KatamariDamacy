#include "..\Headers\Timer.h"



CTimer::CTimer()
{
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FixTime);		// ���� ������ CPU�� ƽ�� �޾ƿ�
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_LastTime);

	QueryPerformanceFrequency(&m_CpuTick);		// Ÿ�̸��� ���ļ� ��ȯ

	return S_OK;
}

_float CTimer::Compute_DeltaTime()
{
	QueryPerformanceCounter(&m_FrameTime);		// ����(�Լ��� �Ҹ� ����)�� ƽ���� �޾ƿ�

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)	
	{
		m_FixTime = m_FrameTime;
		QueryPerformanceFrequency(&m_CpuTick);
	}

	m_fDeltaTime = (_float)(m_FrameTime.QuadPart) - m_LastTime.QuadPart / m_CpuTick.QuadPart;		// �۾��ϴµ� �ɸ� �ð� / �ʴ� ������

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

