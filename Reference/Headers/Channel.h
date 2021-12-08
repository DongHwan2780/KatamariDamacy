#pragma once

#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "Base.h"
/* Ư���ִϸ��̼ǿ� �ҼӵǾ��ִ� ��. */
BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize_Clone(const char* pChannelName);
	HRESULT Add_KeyFrameDesc(KEYFRAMEDESC* pKeyFrameDesc);

public:
	static CChannel* Create(const char* pChannelName);
	virtual void Free() override;

private:
	char		m_szChannelName[MAX_PATH] = "";
	vector<KEYFRAMEDESC*>		m_KeyFrame;		/* ���� ���� ������ Ű������ ���۶����� ǥ���ؾ��� ���������� �����صд�. */
};
END
#endif // !__CHANNEL_H__
