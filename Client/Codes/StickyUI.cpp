#include "stdafx.h"
#include "..\Headers\StickyUI.h"

#include "Management.h"

CStickyUI::CStickyUI(DEVICES)
	:CUIList(pDevice, pDeviceContext)
{
}

CStickyUI::CStickyUI(const CStickyUI & other)
	: CUIList(other)
{
}

HRESULT CStickyUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStickyUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_ComponentsOrthUI()))
		return E_FAIL;


	return S_OK;
}

_int CStickyUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	return _int();
}

_int CStickyUI::Late_Update(_double DeltaTime)
{
	if (m_pRenderer == nullptr)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::PRIORITY, this);
}

HRESULT CStickyUI::Render()
{
	return S_OK;
}

CStickyUI * CStickyUI::Create(DEVICES)
{
	CStickyUI*		pInstance = new CStickyUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CStickyUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CStickyUI::Clone(void * pArg)
{
	CStickyUI*		pInstance = new CStickyUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CStickyUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStickyUI::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
}
