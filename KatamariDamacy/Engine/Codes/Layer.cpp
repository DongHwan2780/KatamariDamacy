#include "..\Headers\Layer.h"
#include "Obj.h"


CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObjectToLayer(CObj * pGameObj)
{
	if (pGameObj == nullptr)
		return E_FAIL;

	m_ObjList.push_back(pGameObj);

	return S_OK;
}

_int CLayer::Update(_double DeltaTime)
{
	_int	iProgress = 0;

	for (auto& pGameObj : m_ObjList)
	{
		iProgress = pGameObj->Update(DeltaTime);

		if (0 > iProgress)
			return -1;
	}

	return _int();
}

_int CLayer::Late_Update(_double DeltaTime)
{
	_int	iProgress = 0;

	for (auto& pGameObj : m_ObjList)
	{
		iProgress = pGameObj->Late_Update(DeltaTime);

		if (0 > iProgress)
			return -1;
	}

	return _int();
}

CComponent * CLayer::GetComponent(const wstring& ComponentTag, _uint iIndex)
{
	auto iter = m_ObjList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->GetComponent(ComponentTag);
}

_uint CLayer::GetGameObjectListSize() const
{
	return (_uint)m_ObjList.size();
}

CObj * CLayer::GetGameObject(_uint iIndex) const
{
	auto iter = m_ObjList.begin();
	if (m_ObjList.end() == iter)
	{
		return nullptr;
	}

	for (_uint i = 0; i < iIndex; ++i, ++iter);

	return (*iter);
}

CLayer * CLayer::Create()
{
	CLayer*	pInstance = new CLayer();

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObj : m_ObjList)
		Safe_Release(pGameObj);

	m_ObjList.clear();
}
