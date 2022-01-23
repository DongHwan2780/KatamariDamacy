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
		BGM, LOGO, BREAK, COLL, STICK, TIMER, SOUND_END
	};

private:
	CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	void Initialize_Sound();

public:
	void PlaySounds(_tchar* pSoundKey, SOUNDCHANNEL eID);
	void PlayBGM(_tchar* pSoundKey);
	void StopSound(SOUNDCHANNEL eID);
	void StopAll();
	void Pause(SOUNDCHANNEL eID);
	void Resume(SOUNDCHANNEL eID);
	void Set_Volume(SOUNDCHANNEL eID, float _fVolume); // 1.0f -> 100% // 0.1f -> 10% // Normalize

private:
	void LoadSoundFile();

public:
	virtual void Free() override;

private:
	FMOD::System*	m_pSystem = nullptr;
	FMOD::Channel*	m_pSound_Channel[SOUND_END];
	FMOD::Sound*	m_pNormalSound;
	unordered_map<_tchar*, FMOD::Sound*> m_pSound;
	typedef unordered_map<_tchar*, FMOD::Sound*> SOUNDS;
};

END
#endif // !__SOUNDMGR_H__
