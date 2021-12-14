#include "..\Headers\Obj.h"
#include "Management.h"

CObj::CObj(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:m_pDevice(pDevice)
	,m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

CObj::CObj(const CObj & other)
	:m_pDevice(other.m_pDevice)
	, m_pDeviceContext(other.m_pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CObj::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CObj::Initialize_Clone(void * pArg)
{
	return S_OK;
}

_int CObj::Update(_double DeltaTime)
{
	return _int();
}

_int CObj::Late_Update(_double DeltaTime)
{
	return _int();
}

HRESULT CObj::Render()
{
	return S_OK;
}

HRESULT CObj::SetUp_Components(_int iSceneIndex, const _tchar * pPrototypeTag, const _tchar * pComponentTag,  CComponent ** pOut, void * pArg)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	CComponent*		pComponent = pManagement->Clone_Component(iSceneIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	if (m_Components.end() != find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag)))
		return E_FAIL;
	else
	{
		m_Components.emplace(pComponentTag, pComponent);
		*pOut = pComponent;
		Safe_AddRef(pComponent);
	}

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

void CObj::Free()
{
	for (auto& pair : m_Components)
	{
		Safe_Release(pair.second);
	}

	m_Components.clear();

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
