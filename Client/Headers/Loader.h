#pragma once

#include "ClientDefines.h"
#include "Base.h"

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader();
	virtual ~CLoader() = default;

public:
	HRESULT Initialize_Thread(SCENE eScene);
	HRESULT StageLoader();

public:
	CRITICAL_SECTION Get_CS() { return m_CS; }
	SCENE Get_NextScene() const { return m_eScene; }
	_bool Get_Finish() const { return m_bFinish; }


public:
	static CLoader* Create(SCENE eScene);
	virtual void Free() override;

private:
	SCENE		m_eScene = END_SCENE;
	HANDLE		m_hThread = 0;
	CRITICAL_SECTION	m_CS;
	_bool		m_bFinish;
};

END