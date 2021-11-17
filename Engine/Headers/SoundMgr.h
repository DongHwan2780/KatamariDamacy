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
	FMOD_SYSTEM*	pSystem;
	FMOD_CHANNEL*	pSound_Channel[SOUND_END];
	FMOD_SOUND*		pSound;
};

END
#endif // !__SOUNDMGR_H__
