#include "stdafx.h"
#include "..\Headers\StageOne.h"

#include "Management.h"
#include "MainCamera.h"

CStageOne::CStageOne(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex)
	:CScene(pDevice, pDeviceContext, iSceneIndex)
{
}


HRESULT CStageOne::Initialize()
{
	__super::Initialize();

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

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

HRESULT CStageOne::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("GameObject_Player"), pLayerTag)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);
	return S_OK;
}

HRESULT CStageOne::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 5.f, -6.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.TransformDesc.fSpeedPerSec = 7.0f;
	CameraDesc.TransformDesc.fRotatePerSec = XMConvertToRadians(90.0f);

	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("GameObject_MainCamera"), pLayerTag, &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);
	return S_OK;
}

HRESULT CStageOne::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("GameObject_Terrain"), pLayerTag)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);
	return S_OK;
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
