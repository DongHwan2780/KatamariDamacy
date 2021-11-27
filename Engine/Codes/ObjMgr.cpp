#include "..\Headers\ObjMgr.h"
#include "Layer.h"
#include "Obj.h"

IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr()
{
}

HRESULT CObjMgr::Reserve_Manager(_uint iNumScenes)
{
	if (nullptr != m_pGameObjects)
		return E_FAIL;

	m_pGameObjects = new GAMEOBJECTS[iNumScenes];		// 씬의 개수만큼 레이어배열 생성

	m_iNumScenes = iNumScenes;

	return S_OK;
}

HRESULT CObjMgr::Add_Prototype(const _tchar * pPrototypeTag, CObj * pPrototype)
{
	if (pPrototype == nullptr || Find_Prototype(pPrototypeTag) != nullptr)	// 받아온 객체가 nullptr이거나 이미 있다면 FAIL반환
		return E_FAIL;

	m_Prototypes.emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObjMgr::Add_GameObj(_int iSceneIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg)
{
	CObj* pPrototype = Find_Prototype(pPrototypeTag);		
	if (pPrototype == nullptr)	// 태그로 원본을 찾았는데 원본이 없다면
		return E_FAIL;

	CObj* pGameObj = pPrototype->Clone(pArg);
	if (pGameObj == nullptr)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iSceneIndex, pLayerTag);		// 레이어 태그로 찾았을 때 기존에 존재하지 않는다면

	if (pLayer == nullptr)
	{
		CLayer* pNewLayer = CLayer::Create();			// 레이어를 새로 생성해서

		if (nullptr == pNewLayer)
			return E_FAIL;

		pNewLayer->Add_GameObjectToLayer(pGameObj);		// 레이어에 담아주고

		m_pGameObjects[iSceneIndex].emplace(pLayerTag, pNewLayer);		// 옵젝매니저도 보관한다.
	}
	else
		pLayer->Add_GameObjectToLayer(pGameObj);		// 레이어를 찾았을 때 기존에 존재한다면 바로 넣어줌

	return S_OK;
}

_int CObjMgr::Update(_double DeltaTime)
{
	_int		iProgress = 0;

	for (_uint i = 0; i < m_iNumScenes; ++i)
	{
		for (auto& pair : m_pGameObjects[i])
		{

			iProgress = pair.second->Update(DeltaTime);
			if (0 > iProgress)
				return -1;
		}
	}

	return _int();
}

_int CObjMgr::Late_Update(_double DeltaTime)
{
	_int		iProgress = 0;

	for (_uint i = 0; i < m_iNumScenes; ++i)
	{
		for (auto& pair : m_pGameObjects[i])
		{
			iProgress = pair.second->Late_Update(DeltaTime);
			if (0 > iProgress)
				return -1;
		}
	}
	return _int();
}

void CObjMgr::Clear(_int iSceneIndex)
{
	if (iSceneIndex >= m_iNumScenes)
		return;

	for (auto& pair : m_pGameObjects[iSceneIndex])
	{
		Safe_Release(pair.second);
	}
	m_pGameObjects[iSceneIndex].clear();
}

CObj * CObjMgr::Find_Prototype(const _tchar * pPrototypeTag)
{
	auto iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTagFinder(pPrototypeTag));

	if (m_Prototypes.end() == iter)
		return nullptr;

	return iter->second;
}

CLayer * CObjMgr::Find_Layer(_int iSceneIndex, const _tchar *pLayerTag)
{
	auto iter = find_if(m_pGameObjects[iSceneIndex].begin(), m_pGameObjects[iSceneIndex].end(), CTagFinder(pLayerTag));

	if (m_pGameObjects[iSceneIndex].end() == iter)
		return nullptr;

	return iter->second;
}

void CObjMgr::Free()
{
	for (_uint i = 0; i < m_iNumScenes; ++i)
	{
		for (auto& Pair : m_pGameObjects[i])
		{
			Safe_Release(Pair.second);
		}

		m_pGameObjects[i].clear();
	}

	Safe_Delete_Array(m_pGameObjects);

	for (auto& pair : m_Prototypes)
	{
		Safe_Release(pair.second);
	}
	m_Prototypes.clear();
}
