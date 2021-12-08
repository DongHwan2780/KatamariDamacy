#include "..\Headers\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize_Clone(const char * pAnimationName, _double Duration, _double TickPerSecond)
{
	strcpy_s(m_AnimationName, pAnimationName);
	m_Duration = Duration;
	m_TickPerSecond = TickPerSecond;

	return S_OK;
}

HRESULT CAnimation::Add_Channel(CChannel * pChannel)
{
	if (nullptr == pChannel)
		return E_FAIL;

	m_Channels.push_back(pChannel);

	return S_OK;
}

CAnimation * CAnimation::Create(const char * pAnimationName, _double Duration, _double TickPerSecond)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize_Clone(pAnimationName, Duration, TickPerSecond)))
	{
		MSG_BOX("Failed to Creating CHierarchyNode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
}
