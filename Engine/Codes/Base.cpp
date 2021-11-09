#include "..\Headers\Base.h"

USING(Engine)

CBase::CBase()
	: m_dwRefCnt(0)
{
}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;
}

unsigned long CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
		return m_dwRefCnt--;
}

