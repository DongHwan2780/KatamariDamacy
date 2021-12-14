#include "stdafx.h"
#include "..\Headers\PlayerMoveUI.h"

#include "Management.h"

CPlayerMoveUI::CPlayerMoveUI(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CPlayerMoveUI::CPlayerMoveUI(const CPlayerMoveUI & other)
	: CObj(other)
{
}

HRESULT CPlayerMoveUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerMoveUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;


	return S_OK;
}

_int CPlayerMoveUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	return _int();
}

_int CPlayerMoveUI::Late_Update(_double DeltaTime)
{
	if (m_pRenderer == nullptr)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::PRIORITY, this);
}

HRESULT CPlayerMoveUI::Render()
{
	return E_NOTIMPL;
}

HRESULT CPlayerMoveUI::SetUp_Components()
{
	return E_NOTIMPL;
}

CPlayerMoveUI * CPlayerMoveUI::Create(DEVICES)
{
	CPlayerMoveUI*		pInstance = new CPlayerMoveUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CPlayerMoveUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CPlayerMoveUI::Clone(void * pArg)
{
	CPlayerMoveUI*		pInstance = new CPlayerMoveUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CPlayerMoveUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerMoveUI::Free()
{
	__super::Free();
}
