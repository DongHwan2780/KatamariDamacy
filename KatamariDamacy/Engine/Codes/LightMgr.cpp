#include "..\Headers\LightMgr.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr()
{
}

const LIGHTDESC * CLightMgr::Get_LightDesc(_uint Index) const
{
	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < Index; ++i)
		++iter;

	return (*iter)->Get_LightDesc();
}

HRESULT CLightMgr::Add_Light(DEVICES, const LIGHTDESC & LightDesc)
{
	CLight*		pLight = CLight::Create(pDevice, pDeviceContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	return S_OK;
}


void CLightMgr::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
