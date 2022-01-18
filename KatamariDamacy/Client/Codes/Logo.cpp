#include "stdafx.h"
#include "..\Headers\Logo.h"

#include "Management.h"
#include "BackGround.h"
#include "Loading.h"

CLogo::CLogo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex)
	:CScene(pDevice, pDeviceContext, iSceneIndex)
{
}


HRESULT CLogo::Initialize()
{
	__super::Initialize();
	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"), 0)))
		return E_FAIL;


	//CManagement*		pManagement = GET_INSTANCE(CManagement);
	//pManagement->PlayBGM(L"Logo.wav");
	//RELEASE_INSTANCE(CManagement);

	return S_OK;
}

_int CLogo::Update(_double DeltaTime)
{
	__super::Update(DeltaTime);

	CManagement*		pManagement = GET_INSTANCE(CManagement);

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (FAILED(pManagement->Set_CurScene(CLoading::Create(m_pDevice, m_pDeviceContext, STAGEONE_SCENE, LOADING_SCENE))))
			return E_FAIL;
	}

	RELEASE_INSTANCE(CManagement);

	return _int();
}

HRESULT CLogo::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CLogo::Ready_Prototype_Component()
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);


	/* Prototype_Texture_BackGround */
	if (FAILED(pManagement->Add_Prototype(LOGO_SCENE, TEXT("Component_Texture_BackGround"), CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::WIC, TEXT("../Bin/Resources/Textures/KatamariUITexture/Logo/BackGround%d.png"), 2))))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CLogo::Ready_Prototype_GameObject()
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);


	/* Prototype_BackGround */
	if (FAILED(pManagement->Add_Prototype(TEXT("Prototype_BackGround"), CBackGround::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CLogo::Ready_Layer_BackGround(const wstring& pLayerTag, _uint iSceneIndex)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	/* For.GameObject_BackGround */
	if (FAILED(pManagement->Add_GameObj(LOGO_SCENE, TEXT("Prototype_BackGround"), pLayerTag, &iSceneIndex)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

CLogo * CLogo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSceneIndex)
{
	CLogo*	pInstance = new CLogo(pDevice, pDeviceContext, iSceneIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CLevel_Logo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLogo::Free()
{
	//CManagement*		pManagement = GET_INSTANCE(CManagement);
	//pManagement->StopAll();
	//RELEASE_INSTANCE(CManagement);
	__super::Free();
}
