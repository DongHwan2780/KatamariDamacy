#include "..\Headers\keyMgr.h"

IMPLEMENT_SINGLETON(CkeyMgr)

CkeyMgr::CkeyMgr()
	:m_dwKey(0), m_dwKeyUp(0), m_dwKeyDown(0)
{
}

void CkeyMgr::Update_KeyMgr()
{
}

_bool CkeyMgr::Key_Up(DWORD _dwKey)
{
	return _bool();
}

_bool CkeyMgr::Key_Down(DWORD _dwKey)
{
	return _bool();
}

_bool CkeyMgr::Key_Pressing(DWORD _dwKey)
{
	return _bool();
}

void CkeyMgr::Free()
{
}


CkeyMgr::~CkeyMgr()
{
}
