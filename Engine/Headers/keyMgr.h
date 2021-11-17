#pragma once

#ifndef __KEYMGR_H__
#define __KEYMGR_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CkeyMgr : public CBase
{
	DECLARE_SINGLETON(CkeyMgr)
private:
	CkeyMgr();
	virtual ~CkeyMgr() = default;

public:
	void Update_KeyMgr();
	_bool Key_Up(DWORD _dwKey);
	_bool Key_Down(DWORD _dwKey);
	_bool Key_Pressing(DWORD _dwKey);

public:
	virtual void Free() override;

private:
	DWORD m_dwKey;
	DWORD m_dwKeyUp;
	DWORD m_dwKeyDown;
};
END
#endif // !__KEYMGR_H__
