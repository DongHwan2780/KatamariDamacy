#include "stdafx.h"
#include "..\Headers\Player.h"

#include "Management.h"

CPlayer::CPlayer(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CPlayer::CPlayer(const CPlayer & other)
	:CObj(other)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CPlayer::Initialize_Clone(void * pArg)
{
	return E_NOTIMPL;
}

_int CPlayer::Update(_double DeltaTime)
{
	return _int();
}

_int CPlayer::Late_Update(_double DeltaTime)
{
	return _int();
}

HRESULT CPlayer::Render()
{
	return E_NOTIMPL;
}

HRESULT CPlayer::SetUp_Components()
{
	return E_NOTIMPL;
}

CPlayer * CPlayer::Create(DEVICES)
{
	return nullptr;
}

CObj * CPlayer::Clone(void * pArg)
{
	return nullptr;
}

void CPlayer::Free()
{
}
