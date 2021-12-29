#include "..\Headers\ComponentMgr.h"

IMPLEMENT_SINGLETON(CComponentMgr)

CComponentMgr::CComponentMgr()
{
}

HRESULT CComponentMgr::Reserve_Manager(_int iNumScenes)
{
	if (nullptr != m_pPrototypes)
		return E_FAIL;

	m_pPrototypes = new PROTOTYPES[iNumScenes];		// 씬의 개수만큼 레이어배열 생성

	m_iNumScenes = iNumScenes;

	return S_OK;
}

HRESULT CComponentMgr::Add_Prototype(_int iSceneIndex, const wstring& pPrototypeTag, CComponent * pPrototype)
{
	if (pPrototype == nullptr || Find_Prototype(iSceneIndex, pPrototypeTag) != nullptr || iSceneIndex >= m_iNumScenes)	// 받아온 객체가 nullptr이거나 이미 있다면 FAIL반환
		return E_FAIL;

	m_pPrototypes[iSceneIndex].emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

CComponent * CComponentMgr::Clone_Component(_int iSceneIndex, const wstring& pPrototypeTag, void * pArg)
{
	if(m_iNumScenes <= iSceneIndex)
		return nullptr;

	CComponent* pPrototype = Find_Prototype(iSceneIndex, pPrototypeTag);
	if (pPrototype == nullptr)
		return nullptr;

	return pPrototype->Clone(pArg);
}

void CComponentMgr::Clear(_int iSceneIndex)
{
	if (m_iNumScenes <= iSceneIndex)
		return;

	for (auto& pair : m_pPrototypes[iSceneIndex])
	{
		Safe_Release(pair.second);
	}
	m_pPrototypes[iSceneIndex].clear();
}

CComponent * CComponentMgr::Find_Prototype(_int iSceneIndex, const wstring& pPrototypeTag)
{
	auto iter = m_pPrototypes[iSceneIndex].find(pPrototypeTag);

	if (iter == m_pPrototypes[iSceneIndex].end())
		return nullptr;

	return iter->second;
}

void CComponentMgr::Free()
{
	for (size_t i = 0; i < m_iNumScenes; ++i)
	{
		for (auto& pair : m_pPrototypes[i])
		{
			Safe_Release(pair.second);
		}
		m_pPrototypes[i].clear();
	}
	Safe_Delete_Array(m_pPrototypes);
}
