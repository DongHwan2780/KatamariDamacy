#pragma once

#ifndef __OBJMGR_H__
#define __OBJMGR_H__


#include "Base.h"

BEGIN(Engine)

class CObjMgr final : public CBase		//	객체들을 기준에 맞춰서 보관하기위한 용도
{
	DECLARE_SINGLETON(CObjMgr)
private:
	CObjMgr();
	virtual ~CObjMgr() = default;

public:
	HRESULT Reserve_Manager(_uint iNumScenes);

public:
	virtual void Free() override;

private:
	_uint	m_iNumScene = 0;

private:
	//unordered_map<const _tchar*, class CObj*>				m_Prototypes;

	//unordered_map<const _tchar*, class CLayer*>*			m_pGameObjects = nullptr;
	//typedef unordered_map<const _tchar*, class CLayer*>		GAMEOBJECTS;
};

END
#endif // !__OBJMGR_H__
