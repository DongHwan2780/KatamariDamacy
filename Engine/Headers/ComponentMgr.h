#pragma once

#ifndef __COMPONENTMGR_H__
#define __COMPONENTMGR_H__

#include "Base.h"

class CComponentMgr final : public CBase
{
	DECLARE_SINGLETON(CComponentMgr)

private:
	CComponentMgr();
	virtual ~CComponentMgr() = default;

public:
	virtual void Free() override;

private:
	//unordered_map<const _tchar*, CComponent*>*	m_pPrototypes = nullptr;
};

#endif // !__COMPONENTMGR_H__
