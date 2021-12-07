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
	return E_NOTIMPL;
}

HRESULT CObjList::Initialize_Clone(void * pArg)
{
	return E_NOTIMPL;
}

_int CObjList::Update(_double DeltaTime)
{
	return _int();
}

_int CObjList::Late_Update(_double DeltaTime)
{
	return _int();
}

HRESULT CObjList::Render()
{
	return E_NOTIMPL;
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
}
