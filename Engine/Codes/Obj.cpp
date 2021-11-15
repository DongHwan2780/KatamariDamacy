#include "..\Headers\Obj.h"


CObj::CObj(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:m_pDevice(pDevice)
	,m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(pDeviceContext);
	Safe_AddRef(pDevice);
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

CObj * CObj::Clone(void * pArg)
{
	return nullptr;
}

void CObj::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
