#include "stdafx.h"
#include "..\Headers\PlayerBall.h"

#include "Management.h"

CPlayerBall::CPlayerBall(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CPlayerBall::CPlayerBall(const CPlayerBall & other)
	:CObj(other)
{
}

HRESULT CPlayerBall::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CPlayerBall::Initialize_Clone(void * pArg)
{
	return E_NOTIMPL;
}

_int CPlayerBall::Update(_double DeltaTime)
{
	return _int();
}

_int CPlayerBall::Late_Update(_double DeltaTime)
{
	return _int();
}

HRESULT CPlayerBall::Render()
{
	return E_NOTIMPL;
}

HRESULT CPlayerBall::SetUp_Components()
{
	return E_NOTIMPL;
}

CPlayerBall * CPlayerBall::Create(DEVICES)
{
	return nullptr;
}

CObj * CPlayerBall::Clone(void * pArg)
{
	return nullptr;
}

void CPlayerBall::Free()
{
}
