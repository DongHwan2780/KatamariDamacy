#pragma once

#ifndef __NAVIGATION_H__
#define __NAVIGATION_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
	typedef struct tagNavigationDesc
	{
		_uint		iCurrentCellIndex = 0;
	}NAVIDESC;

private:
	explicit CNavigation(DEVICES);
	explicit CNavigation(const CNavigation& other);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pDataFiles);
	virtual HRESULT Initialize_Clone(void * pArg) override;

public:
	_bool isMove(_fvector vPoint);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif

private:
	HRESULT SetUp_NeighborCell();

public:
	static CNavigation* Create(DEVICES, const _tchar* pDataFiles);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	vector<class CCell*>				m_Cells;
	typedef vector<class CCell*>		CELLS;

private:
	NAVIDESC	m_NaviDesc;
	_uint		m_iNumCells = 0;

};

END

#endif // !__NAVIGATION_H__
