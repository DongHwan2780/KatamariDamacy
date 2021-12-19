#include "..\Headers\keyMgr.h"

IMPLEMENT_SINGLETON(CkeyMgr)

CkeyMgr::CkeyMgr()
	:m_dwKey(0), m_dwKeyUp(0), m_dwKeyDown(0)
{
}

void CkeyMgr::Update_KeyMgr()
{
	m_dwKey = 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_RETURN;
}

_bool CkeyMgr::Key_Up(DWORD _dwKey)
{
	if (m_dwKey & _dwKey)
	{
		m_dwKeyUp |= _dwKey;
		return false;
	}
	else if (m_dwKeyUp& _dwKey)
	{
		m_dwKeyUp ^= _dwKey;
		return true;
	}
	return false;
}

_bool CkeyMgr::Key_Down(DWORD _dwKey)
{
	if ((m_dwKey & _dwKey) && !(m_dwKeyDown & _dwKey))
	{
		m_dwKeyDown |= _dwKey;
		return true;
	}
	else if (!(m_dwKey & _dwKey) && (m_dwKeyDown & _dwKey))
	{
		m_dwKeyDown ^= _dwKey;
		return false;
	}
	return false;
}

_bool CkeyMgr::Key_Pressing(DWORD _dwKey)
{
	if (m_dwKey & _dwKey)
		return true;
	return false;
}

void CkeyMgr::Free()
{
}
