#pragma once

#ifndef __SOUNDMGR_H__
#define __SOUNDMGR_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CSoundMgr final : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)
public:
	enum SOUNDCHANNEL
	{
		BGM, LOGO, SOUND_END
	};

private:
	CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	void Initialize_Sound();

public:
	virtual void Free() override;

private:
	FMOD::System*	m_pSystem = nullptr;
	FMOD::Channel*	m_pSound_Channel[SOUND_END];
	FMOD::Sound*	m_pSound = nullptr;
};

END
#endif // !__SOUNDMGR_H__
