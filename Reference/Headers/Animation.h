#pragma once

#ifndef __ANIMATION_H__
#define __ANIMATION_H__


#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize_Clone(const char* pAnimationName, _double Duration, _double TickPerSecond);
	HRESULT Add_Channel(class CChannel* pChannel);

public:
	static CAnimation* Create(const char* pAnimationName, _double Duration, _double TickPerSecond);
	virtual void Free() override;

private:
	char				m_AnimationName[MAX_PATH];
	_double				m_Duration = 0.0; /* 애니메이션 하나를 재생하기위한 시간 */
	_double				m_TickPerSecond = 0.0;

	/* 현재 애니메이션 표현을 위한 채널(뼈)들의 집합. */
	vector<class CChannel*>	m_Channels;
};
END
#endif // ! __ANIMATION_H__
