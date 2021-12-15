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
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CObj* pPrototype);
	HRESULT Add_GameObj(_int iSceneIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg);
	_int	Update(_double DeltaTime);
	_int	Late_Update(_double DeltaTime);
	void	Clear(_int	iSceneIndex);

public:
	class CComponent* GetComponent(const _tchar * LayerTag, const _tchar * ComponentTag, _int iSceneIndex) const;

private:
	class CObj*	Find_Prototype(const _tchar* pPrototypeTag );
	class CLayer* Find_Layer(_int iSceneIndex , const _tchar* pLayerTag);

public:
	virtual void Free() override;

private:
	_uint	m_iNumScenes = 0;

private:
	unordered_map<const _tchar*, class CObj*>				m_Prototypes;		// ������ü

	unordered_map<const _tchar*, class CLayer*>*			m_pGameObjects = nullptr;		// Ŭ��
	typedef unordered_map<const _tchar*, class CLayer*>		GAMEOBJECTS;
};

END
#endif // !__OBJMGR_H__
