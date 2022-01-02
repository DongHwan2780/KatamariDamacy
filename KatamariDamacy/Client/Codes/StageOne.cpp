#include "stdafx.h"
#include "..\Headers\StageOne.h"

#include "Management.h"
#include "MainCamera.h"

#include "SizeUI.h"
#include "StickyUI.h"
#include "TimeUI.h"
#include "PlayerMoveUI.h"

CStageOne::CStageOne(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex)
	:CScene(pDevice, pDeviceContext, iSceneIndex)
{
}


HRESULT CStageOne::Initialize()
{
	__super::Initialize();

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	//if (FAILED(Ready_Prototype_Component()))
	//	return E_FAIL;

	//if (FAILED(Ready_Prototype_GameObject()))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_StageMap(TEXT("Layer_StageMap"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_SizeUI(TEXT("Layer_SizeUI"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_StickyUI(TEXT("Layer_StickUI"))))
	//	return E_FAIL;

	return S_OK;
}

_int CStageOne::Update(_double DeltaTime)
{
	__super::Update(DeltaTime);

	return _int();
}

HRESULT CStageOne::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CStageOne::Ready_Lights()
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.vLightDir = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pManagement->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CStageOne::Ready_Layer_Player(const wstring& pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("GameObject_Player"), pLayerTag)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CStageOne::Ready_Layer_Camera(const wstring& pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 3.f, -6.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f); 

	CameraDesc.TransformDesc.fSpeedPerSec = 5.0f;
	CameraDesc.TransformDesc.fRotatePerSec = XMConvertToRadians(90.0f);

	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("GameObject_MainCamera"), pLayerTag, &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);
	return S_OK;
}

HRESULT CStageOne::Ready_Layer_StageMap(const wstring& pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("GameObject_StageMap"), pLayerTag)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);
	return S_OK;
}

HRESULT CStageOne::Ready_Prototype_Component()
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);


	/* Prototype_Texture_SizeUI */
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Texture_SizeUI"), CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::WIC, TEXT("../Bin/Resources/Textures/KatamariUITexture/BallSizeUI/Size%d.png"), 5))))
		return E_FAIL;

	/* Prototype_Texture_StickUI */
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Texture_StickUI"), CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::WIC, TEXT("../Bin/Resources/Textures/KatamariUITexture/LastStickUI/ps4_stickR.png"), 1))))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CStageOne::Ready_Prototype_GameObject()
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);


	/* Prototype_SizeUI */
	if (FAILED(pManagement->Add_Prototype(TEXT("Prototype_SizeUI"), CSizeUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* Prototype_StickUI */
	if (FAILED(pManagement->Add_Prototype(TEXT("Prototype_StickUI"), CStickyUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CStageOne::Ready_Layer_SizeUI(const wstring& pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	/* For.GameObject_SizeUI */
	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("Prototype_SizeUI"), pLayerTag)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CStageOne::Ready_Layer_StickyUI(const wstring& pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	/* For.GameObject_StickUI */
	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("Prototype_StickUI"), pLayerTag)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CStageOne::Ready_Layer_TimeUI(const wstring& pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CStageOne::Ready_Layer_PlayerMoveUI(const wstring& pLayerTag)
{
	return E_NOTIMPL;
}


CStageOne * CStageOne::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex)
{
	CStageOne* pInstance = new CStageOne(pDevice, pDeviceContext, iSceneIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CStageOne");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageOne::Free()
{
	__super::Free();
}
