#include "stdafx.h"
#include "..\Headers\Logo.h"

#include "Management.h"

CLogo::CLogo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex)
	:CScene(pDevice, pDeviceContext, iSceneIndex)
{
}


HRESULT CLogo::Initialize()
{
	__super::Initialize();

	CManagement*		pManagement = GET_INSTANCE(CManagement);

	//pManagement->PlayBGM(L"Village.ogg");

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

_int CLogo::Update(_double DeltaTime)
{
	__super::Update(DeltaTime);

	CManagement*		pManagement = GET_INSTANCE(CManagement);
	PxTransform			pxTransform = PxTransform(PxVec3(0.f, 0.f, -10.f), PxQuat(1.f));

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		pManagement->CreateDynamicBall(pxTransform);
	}
	RELEASE_INSTANCE(CManagement);
	return _int();
}

HRESULT CLogo::Render()
{
	__super::Render();

	return S_OK;
}

CLogo * CLogo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSceneIndex)
{
	CLogo*	pInstance = new CLogo(pDevice, pDeviceContext, iSceneIndex);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLogo::Free()
{
	__super::Free();
}
