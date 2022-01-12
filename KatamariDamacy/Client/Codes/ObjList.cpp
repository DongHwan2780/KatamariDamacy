#include "stdafx.h"
#include "..\Headers\ObjList.h"

#include "PlayerBall.h"
#include "Management.h"

CObjList::CObjList(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CObjList::CObjList(const CObjList & other)
	:CObj(other)
{
}

HRESULT CObjList::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CObjList::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CObjList::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	return _int();
}

_int CObjList::Late_Update(_double DeltaTime)
{
	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return _int();
}

HRESULT CObjList::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CObjList::Gravity(_double DeltaTime)
{
}

void CObjList::OnLandCheck()
{
}

void CObjList::Coll_PlayerBall()
{
}

CObj * CObjList::Clone(void * pArg)
{
	return nullptr;
}

void CObjList::Free()
{
	__super::Free();
}
