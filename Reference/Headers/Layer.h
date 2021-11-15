#pragma once

#ifndef __LAYER_H__
#define __LAYER_H__

#include "Base.h"

BEGIN(Engine)

class CLayer final : public CBase
{
public:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObjectToLayer(class CObj*	pGameObj);
	_int	Update(_double DeltaTime);
	_int	Late_Update(_double DeltaTime);

public:
	static CLayer*	Create();
	virtual void Free() override;

private:
	list<class CObj*>			m_ObjList;
	typedef list<class CObj*>	OBJECTLIST;
};

END
#endif // !__LAYER_H__
