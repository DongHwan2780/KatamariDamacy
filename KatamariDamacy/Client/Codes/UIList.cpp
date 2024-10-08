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

	m_dLimitTime = 300.0;

	return S_OK;
}

_int CUIList::Update(_double DeltaTime)
{
	__super::Update(DeltaTime);

	m_dNowTime += DeltaTime;

	if (Get_LimitTime() == 0)
	{
		m_dLimitTime = 0;
	}
	else if (m_dNowTime >= 1.0)
	{
		m_dLimitTime -= 1.0;
		m_dNowTime = 0.0;
	}

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

void CUIList::Free()
{
	__super::Free();
}

//HRESULT CUIList::SetUp_ComponentsProjUI()
//{
//	return S_OK;
//}
