#include "stdafx.h"
#include "..\Headers\SizeUI.h"

#include "Management.h"

CSizeUI::CSizeUI(DEVICES)
	:CUIList(pDevice, pDeviceContext)
{
}

CSizeUI::CSizeUI(const CSizeUI & other)
	:CUIList(other)
{
}

HRESULT CSizeUI::Initialize_Prototype() 
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSizeUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_ComponentsOrthUI()))
		return E_FAIL;


	return S_OK;
}

_int CSizeUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	return _int();
}

_int CSizeUI::Late_Update(_double DeltaTime)
{
	if (m_pRenderer == nullptr)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::PRIORITY, this);
}

HRESULT CSizeUI::Render()
{
	return S_OK;
}

CSizeUI * CSizeUI::Create(DEVICES)
{
	CSizeUI*		pInstance = new CSizeUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CSizeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CSizeUI::Clone(void * pArg)
{
	CSizeUI*		pInstance = new CSizeUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CSizeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSizeUI::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
}
