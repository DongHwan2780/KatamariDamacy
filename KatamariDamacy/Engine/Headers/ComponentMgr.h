#pragma once

#ifndef __COMPONENTMGR_H__
#define __COMPONENTMGR_H__

#include "Model.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transform.h"
#include "Collider.h"

#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_UI.h"
#include "VIBuffer_PointInstance.h"
BEGIN(Engine)

class CComponentMgr final : public CBase
{
	DECLARE_SINGLETON(CComponentMgr)

private:
	CComponentMgr();
	virtual ~CComponentMgr() = default;

public:
	HRESULT Reserve_Manager(_int iNumScenes);
	HRESULT Add_Prototype(_int iSceneIndex, const wstring& pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_int iSceneIndex, const wstring& pPrototypeTag, void* pArg);
	void Clear(_int iSceneIndex);

public:
	CComponent* Find_Prototype(_int iSceneIndex, const wstring& pPrototypeTag);

public:
	virtual void Free() override;

private:
	unordered_map<wstring, CComponent*>*			m_pPrototypes = nullptr;
	typedef unordered_map<wstring, CComponent*>	PROTOTYPES;

private:
	_int	m_iNumScenes = 0;
};

END
#endif // !__COMPONENTMGR_H__
