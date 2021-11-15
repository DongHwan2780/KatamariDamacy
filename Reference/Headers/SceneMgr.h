#pragma once

#ifndef __SCENEMGR_H__
#define __SCENEMGR_H__

#include "Base.h"
#include "Scene.h"

BEGIN(Engine)

class CSceneMgr final : public CBase
{
	DECLARE_SINGLETON(CSceneMgr)
public:
	CSceneMgr();
	virtual ~CSceneMgr() = default;

public:
	HRESULT	Set_CurScene(CScene* pCurScene);
	_int	Update_Scene(_double DeltaTime);
	HRESULT	Render_Scene();


public:
	virtual void Free() override;


private:
	CScene*		m_pCurScene = nullptr;
};

END
#endif // !__SCENEMGR_H__
