#include "..\Headers\SoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
	m_pSystem = nullptr;
}

void CSoundMgr::Initialize_Sound()
{
	FMOD::System_Create(&m_pSystem);

	m_pSystem->init(64, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
}

void CSoundMgr::PlaySounds(_tchar * pSoundKey, SOUNDCHANNEL eID)
{
	SOUNDS::iterator iter;

	iter = find_if(m_pSound.begin(), m_pSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_pSound.end())
		return;

	bool bPlay = false;
	if (m_pSound_Channel[eID]->isPlaying(&bPlay))
	{
		m_pSystem->playSound(iter->second, 0, FALSE, &m_pSound_Channel[eID]);
	}


	m_pSystem->update();
}

void CSoundMgr::PlayBGM(_tchar * pSoundKey)
{
	SOUNDS::iterator iter;

	iter = find_if(m_pSound.begin(), m_pSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_pSound.end())
		return;

	m_pSystem->playSound(iter->second, 0, FALSE, &m_pSound_Channel[BGM]);
	m_pSound_Channel[BGM]->setMode(FMOD_LOOP_NORMAL);
	m_pSystem->update();
}

void CSoundMgr::StopSound(SOUNDCHANNEL eID)
{
	m_pSound_Channel[eID]->stop();
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < SOUND_END; ++i)
		m_pSound_Channel[i]->stop();
}

void CSoundMgr::Pause(SOUNDCHANNEL eID)
{
	m_pSound_Channel[eID]->setPaused(true);
}

void CSoundMgr::Resume(SOUNDCHANNEL eID)
{
	m_pSound_Channel[eID]->setPaused(false);
}

void CSoundMgr::Set_Volume(SOUNDCHANNEL eID, float _fVolume)
{
	m_pSound_Channel[eID]->setVolume(_fVolume);
}

void CSoundMgr::LoadSoundFile()
{
	_finddatai64_t fd;

	intptr_t handle = _findfirsti64("../../Sound/*.*", &fd);	// 

	if (handle == 0)
		return;

	intptr_t iResult = 0;

	char szCurPath[128] = "../../Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);
		FMOD::Sound* pSound = nullptr;

		FMOD_RESULT eRes = m_pSystem->createSound(szFullPath, FMOD_LOOP_OFF, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = (int)strlen(fd.name) + 1;

			_tchar* pSoundKey = new _tchar[iLength];
			ZeroMemory(pSoundKey, sizeof(_tchar) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_pSound.emplace(pSoundKey, pSound);
		}
		iResult = _findnexti64(handle, &fd);
	}
	m_pSystem->update();
	_findclose(handle);
}

void CSoundMgr::Free()
{
	for (auto& Mypair : m_pSound)
	{
		delete[] Mypair.first;
		Mypair.second->release();
	}
	m_pSound.clear();
	m_pSystem->release();
	m_pSystem->close();
}
