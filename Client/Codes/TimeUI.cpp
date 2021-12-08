#include "stdafx.h"
#include "..\Headers\TimeUI.h"

#include "Management.h"

CTimeUI::CTimeUI(DEVICES)
	:CUIList(pDevice, pDeviceContext)
{
}

CTimeUI::CTimeUI(const CTimeUI & other)
	:CUIList(other)
{
}

HRESULT CTimeUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTimeUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_ComponentsOrthUI()))
		return E_FAIL;


	return S_OK;
}

_int CTimeUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	return _int();
}

_int CTimeUI::Late_Update(_double DeltaTime)
{
	if (m_pRenderer == nullptr)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::PRIORITY, this);
}

HRESULT CTimeUI::Render()
{
	return S_OK;
}

CTimeUI * CTimeUI::Create(DEVICES)
{
	CTimeUI*		pInstance = new CTimeUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CTimeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CTimeUI::Clone(void * pArg)
{
	CTimeUI*		pInstance = new CTimeUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CTimeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimeUI::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
}
