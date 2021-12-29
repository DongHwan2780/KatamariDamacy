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

	m_pGameObjects = new GAMEOBJECTS[iNumScenes];		// ���� ������ŭ ���̾�迭 ����

	m_iNumScenes = iNumScenes;

	return S_OK;
}

HRESULT CObjMgr::Add_Prototype(const wstring& pPrototypeTag, CObj * pPrototype)
{
	if (pPrototype == nullptr || Find_Prototype(pPrototypeTag) != nullptr)	// �޾ƿ� ��ü�� nullptr�̰ų� �̹� �ִٸ� FAIL��ȯ
		return E_FAIL;

	m_Prototypes.emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObjMgr::Add_GameObj(_int iSceneIndex, const wstring& pPrototypeTag, const wstring& pLayerTag, void * pArg)
{
	CObj* pPrototype = Find_Prototype(pPrototypeTag);		
	if (pPrototype == nullptr)	// �±׷� ������ ã�Ҵµ� ������ ���ٸ�
		return E_FAIL;

	CObj* pGameObj = pPrototype->Clone(pArg);
	if (pGameObj == nullptr)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iSceneIndex, pLayerTag);		// ���̾� �±׷� ã���� �� ������ �������� �ʴ´ٸ�

	if (pLayer == nullptr)
	{
		CLayer* pNewLayer = CLayer::Create();			// ���̾ ���� �����ؼ�

		if (nullptr == pNewLayer)
			return E_FAIL;

		pNewLayer->Add_GameObjectToLayer(pGameObj);		// ���̾ ����ְ�

		m_pGameObjects[iSceneIndex].emplace(pLayerTag, pNewLayer);		// �����Ŵ����� �����Ѵ�.
	}
	else
		pLayer->Add_GameObjectToLayer(pGameObj);		// ���̾ ã���� �� ������ �����Ѵٸ� �ٷ� �־���

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

CComponent * CObjMgr::GetComponent(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pComponentTag, _uint iIndex)
{
	if (iLevelIndex >= m_iNumScenes)
		return nullptr;

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->GetComponent(pComponentTag, iIndex);
}

_uint CObjMgr::GetGameObjectListSize(_uint iLevelIndex, const wstring& LayerTag) const
{
	auto iter = m_pGameObjects[iLevelIndex].find(LayerTag);

	//if (m_pGameObjects[iLevelIndex].end() == iter)
	//{
	//	MSG_BOX("Failed to GetObjListSize");
	//	return 0;
	//}

	return iter->second->GetGameObjectListSize();
}

CObj * CObjMgr::GetGameObject(_uint iLevelIndex, const wstring & LayerTag, _uint iIndex) const
{
	auto iter_find = m_pGameObjects[iLevelIndex].find(LayerTag);
	if (m_pGameObjects[iLevelIndex].end() == iter_find)
	{
		wstring ErrMsg = LayerTag + L" is not found";
		return nullptr;
	}
	return iter_find->second->GetGameObject(iIndex);
}

CObj * CObjMgr::Find_Prototype(const wstring& pPrototypeTag)
{
	auto iter_find = m_Prototypes.find(pPrototypeTag);

	if (m_Prototypes.end() == iter_find)
		return nullptr;

	return iter_find->second;
}

CLayer * CObjMgr::Find_Layer(_int iSceneIndex, const wstring&pLayerTag)
{
	auto iter = m_pGameObjects[iSceneIndex].find(pLayerTag);

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
