#include "stdafx.h"
#include "..\Headers\UIList.h"

CUIList::CUIList(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CUIList::CUIList(const CUIList & other)
	:CObj(other)
{
}

HRESULT CUIList::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUIList::Initialize_Clone(void * pArg)
{
	__super::Initialize_Clone(pArg);

	return S_OK;
}

_int CUIList::Update(_double DeltaTime)
{
	__super::Update(DeltaTime);

	return _int();
}

_int CUIList::Late_Update(_double DeltaTime)
{
	__super::Late_Update(DeltaTime);

	return _int();
}

HRESULT CUIList::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CUIList::SetUp_ComponentsOrthUI()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(LOGO_SCENE, TEXT("Component_Texture_BackGround"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUIList::SetUp_ComponentsProjUI()
{
	return S_OK;
}
