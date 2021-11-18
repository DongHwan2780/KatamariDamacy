#include "..\Headers\SoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
}

void CSoundMgr::Initialize_Sound()
{
	FMOD::System_Create(&m_pSystem);

	m_pSystem->init(32, FMOD_INIT_NORMAL, nullptr);

	m_pSystem->createSound("../Sound/MiniGameBGM.ogg", FMOD_LOOP_OFF, 0, &m_pSound);

}

void CSoundMgr::Free()
{
}
