#pragma once

#ifndef __OBJMGR_H__
#define __OBJMGR_H__


#include "Base.h"

BEGIN(Engine)

class CObjMgr final : public CBase		//	��ü���� ���ؿ� ���缭 �����ϱ����� �뵵
{
	DECLARE_SINGLETON(CObjMgr)
private:
	CObjMgr();
	virtual ~CObjMgr() = default;

public:
	HRESULT Reserve_Manager(_uint iNumScenes);
	HRESULT Add_Prototype(const wstring& pPrototypeTag, class CObj* pPrototype);
	HRESULT Add_GameObj(_int iSceneIndex, const wstring& pPrototypeTag, const wstring& pLayerTag, void* pArg);
	_int	Update(_double DeltaTime);
	_int	Late_Update(_double DeltaTime);
	void	Clear(_int	iSceneIndex);

public:
	class CComponent* GetComponent(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pComponentTag, _uint iIndex = 0) ;
	size_t GetGameObjectListSize(_uint iLevelIndex, const wstring& LayerTag) const;
	class CObj* GetGameObject(_uint iLevelIndex, const wstring& LayerTag, _uint iIndex = 0) const;

private:
	class CObj*	Find_Prototype(const wstring& pPrototypeTag );
	class CLayer* Find_Layer(_int iSceneIndex , const wstring& pLayerTag);

public:
	virtual void Free() override;

private:
	_uint	m_iNumScenes = 0;

private:
	unordered_map<wstring, class CObj*>				m_Prototypes;		// ������ü

	unordered_map<wstring, class CLayer*>*			m_pGameObjects = nullptr;		// Ŭ��
	typedef unordered_map<wstring, class CLayer*>		GAMEOBJECTS;
};

END
#endif // !__OBJMGR_H__
