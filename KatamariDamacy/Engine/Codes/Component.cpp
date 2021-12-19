#include "..\Headers\Component.h"

CComponent::CComponent(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_isCloned(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CComponent::CComponent(const CComponent & other)
	: m_pDevice(other.m_pDevice)
	, m_pDeviceContext(other.m_pDeviceContext)
	, m_isCloned(true)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}
HRESULT CComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CComponent::Initialize_Clone(void * pArg)
{
	return S_OK;
}

void CComponent::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
