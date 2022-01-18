#include "stdafx.h"
#include "..\Headers\StageOne.h"

#include "Management.h"
#include "MainCamera.h"

#include "SizeUI.h"
#include "StickyUI.h"
#include "TimeUI.h"
#include "PlayerMoveUI.h"
#include "StickObjUI.h"
#include "ClockUI.h"

CStageOne::CStageOne(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex)
	:CScene(pDevice, pDeviceContext, iSceneIndex)
{
}


HRESULT CStageOne::Initialize()
{
	__super::Initialize();

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_StageMap(TEXT("Layer_StageMap"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_SizeUI(TEXT("Layer_SizeUI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_StickyUI(TEXT("Layer_StickUI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_ClockUI(TEXT("Layer_ClockUI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_TimeUI(TEXT("Layer_TimeUI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_PlayerMoveUI(TEXT("Layer_PlayerMoveUI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_StickObjUI(TEXT("Layer_StickObjUI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Dummy(TEXT("Layer_Dummy"))))
		return E_FAIL;

	Ready_Layer_StageObj();

	//CManagement*		pManagement = GET_INSTANCE(CManagement);
	//pManagement->PlayBGM(L"Stage.wav");
	//RELEASE_INSTANCE(CManagement);

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

HRESULT CStageOne::Ready_Layer_Terrain(const wstring & pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("GameObject_Terrain"), pLayerTag)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);
	return S_OK;
}

HRESULT CStageOne::Ready_Layer_Dummy(const wstring & pLayerTag)
{
	HANDLE hFile = CreateFile(L"../../MFCTool/Data/Object/Collider.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		E_FAIL;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	DWORD dwByte = 0;
	CTransform::TRANSFORMDESC TransformDesc;

	while (true)
	{
		ReadFile(hFile, &TransformDesc.vPosition, sizeof(_vector), &dwByte, nullptr);
		ReadFile(hFile, &TransformDesc.vScale, sizeof(_float3), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("GameObject_Dummy"), pLayerTag, &TransformDesc)))
			return E_FAIL;	
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CStageOne::Ready_Layer_StageObj()
{
	wstring wstrFilePath[32] = { L"", L"", L"", L"", L"", L"", L"", L"", L"", L""
		, L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L""
		, L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L""
		, L"" , L"" }


		, wstrProtoTag[32] = { L"", L"", L"", L"", L"", L"", L"", L"", L"", L""
		, L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L""
		, L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L""
		, L"" , L"" }

		, wstrLayerTag[32] = { L"", L"", L"", L"", L"", L"", L"", L"", L"", L""
			, L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L""
			, L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L""
			, L"" , L"" };

	wstrFilePath[0] = L"../../MFCTool/Data/Object/Apple.dat";
	wstrFilePath[1] = L"../../MFCTool/Data/Object/Bag.dat";
	wstrFilePath[2] = L"../../MFCTool/Data/Object/Banana.dat";
	wstrFilePath[3] = L"../../MFCTool/Data/Object/Bansai.dat";
	wstrFilePath[4] = L"../../MFCTool/Data/Object/Bat.dat";
	wstrFilePath[5] = L"../../MFCTool/Data/Object/Book0.dat";
	wstrFilePath[6] = L"../../MFCTool/Data/Object/Book1.dat";
	wstrFilePath[7] = L"../../MFCTool/Data/Object/Bowling.dat";
	wstrFilePath[8] = L"../../MFCTool/Data/Object/Cake.dat";
	wstrFilePath[9] = L"../../MFCTool/Data/Object/Can.dat";
	wstrFilePath[10] = L"../../MFCTool/Data/Object/Candy.dat";
	wstrFilePath[11] = L"../../MFCTool/Data/Object/Clock.dat";
	wstrFilePath[12] = L"../../MFCTool/Data/Object/Dice.dat";
	wstrFilePath[13] = L"../../MFCTool/Data/Object/DrinkCan0.dat";
	wstrFilePath[14] = L"../../MFCTool/Data/Object/DrinkCan1.dat";
	wstrFilePath[15] = L"../../MFCTool/Data/Object/Egg.dat";
	wstrFilePath[16] = L"../../MFCTool/Data/Object/Eraser.dat";
	wstrFilePath[17] = L"../../MFCTool/Data/Object/Fork.dat";
	wstrFilePath[18] = L"../../MFCTool/Data/Object/FryPan.dat";
	wstrFilePath[19] = L"../../MFCTool/Data/Object/LipStick.dat";
	wstrFilePath[20] = L"../../MFCTool/Data/Object/Magnet.dat";
	wstrFilePath[21] = L"../../MFCTool/Data/Object/Melon.dat";
	wstrFilePath[22] = L"../../MFCTool/Data/Object/MilkPack.dat";
	wstrFilePath[23] = L"../../MFCTool/Data/Object/Orange.dat";
	wstrFilePath[24] = L"../../MFCTool/Data/Object/Pilon.dat";
	wstrFilePath[25] = L"../../MFCTool/Data/Object/Pin0.dat";
	wstrFilePath[26] = L"../../MFCTool/Data/Object/Pin1.dat";
	wstrFilePath[27] = L"../../MFCTool/Data/Object/Radio.dat";
	wstrFilePath[28] = L"../../MFCTool/Data/Object/Spoon.dat";
	wstrFilePath[29] = L"../../MFCTool/Data/Object/Stand.dat";
	wstrFilePath[30] = L"../../MFCTool/Data/Object/SunFlower.dat";
	wstrFilePath[31] = L"../../MFCTool/Data/Object/Tulip.dat";

	wstrProtoTag[0] =		L"GameObject_Apple";
	wstrProtoTag[1] =		L"GameObject_Bag";
	wstrProtoTag[2] =		L"GameObject_Banana";
	wstrProtoTag[3] =		L"GameObject_Bat";
	wstrProtoTag[4] =		L"GameObject_Bansai";
	wstrProtoTag[5] =		L"GameObject_Book0";
	wstrProtoTag[6] =		L"GameObject_Book1";
	wstrProtoTag[7] =		L"GameObject_Bowling";
	wstrProtoTag[8] =		L"GameObject_Cake";
	wstrProtoTag[9] =		L"GameObject_Can";
	wstrProtoTag[10] =		L"GameObject_Candy";
	wstrProtoTag[11] =		L"GameObject_Clock";
	wstrProtoTag[12] =		L"GameObject_Dice";
	wstrProtoTag[13] =		L"GameObject_DrinkCan0";
	wstrProtoTag[14] =		L"GameObject_DrinkCan1";
	wstrProtoTag[15] =		L"GameObject_Egg";
	wstrProtoTag[16] =		L"GameObject_Eraser";
	wstrProtoTag[17] =		L"GameObject_Fork";
	wstrProtoTag[18] =		L"GameObject_FryPan";
	wstrProtoTag[19] =		L"GameObject_LipStick";
	wstrProtoTag[20] =		L"GameObject_Magnet";
	wstrProtoTag[21] =		L"GameObject_Melon";
	wstrProtoTag[22] =		L"GameObject_MilkPack";
	wstrProtoTag[23] =		L"GameObject_Orange";
	wstrProtoTag[24] =		L"GameObject_Pilon";
	wstrProtoTag[25] =		L"GameObject_Pin0";
	wstrProtoTag[26] =		L"GameObject_Pin1";
	wstrProtoTag[27] =		L"GameObject_Radio";
	wstrProtoTag[28] =		L"GameObject_Spoon";
	wstrProtoTag[29] =		L"GameObject_Stand";
	wstrProtoTag[30] =		L"GameObject_SunFlower";
	wstrProtoTag[31] =		L"GameObject_Tulip";

	wstrLayerTag[0] =	L"Layer_Apple";
	wstrLayerTag[1] =	L"Layer_Bag";
	wstrLayerTag[2] =	L"Layer_Banana";
	wstrLayerTag[3] =	L"Layer_Bat";
	wstrLayerTag[4] =	L"Layer_Bansai";
	wstrLayerTag[5] =	L"Layer_Book0";
	wstrLayerTag[6] =	L"Layer_Book1";
	wstrLayerTag[7] =	L"Layer_Bowling";
	wstrLayerTag[8] =	L"Layer_Cake";
	wstrLayerTag[9] =	L"Layer_Can";
	wstrLayerTag[10] =	L"Layer_Candy";
	wstrLayerTag[11] =	L"Layer_Clock";
	wstrLayerTag[12] =	L"Layer_Dice";
	wstrLayerTag[13] =	L"Layer_DrinkCan0";
	wstrLayerTag[14] =	L"Layer_DrinkCan1";
	wstrLayerTag[15] =	L"Layer_Egg";
	wstrLayerTag[16] =	L"Layer_Eraser";
	wstrLayerTag[17] =	L"Layer_Fork";
	wstrLayerTag[18] =	L"Layer_FryPan";
	wstrLayerTag[19] =	L"Layer_LipStick";
	wstrLayerTag[20] =	L"Layer_Magnet";
	wstrLayerTag[21] =	L"Layer_Melon";
	wstrLayerTag[22] =	L"Layer_MilkPack";
	wstrLayerTag[23] =	L"Layer_Orange";
	wstrLayerTag[24] =	L"Layer_Pilon";
	wstrLayerTag[25] =	L"Layer_Pin0";
	wstrLayerTag[26] =	L"Layer_Pin1";
	wstrLayerTag[27] =	L"Layer_Radio";
	wstrLayerTag[28] =	L"Layer_Spoon";
	wstrLayerTag[29] =	L"Layer_Stand";
	wstrLayerTag[30] =	L"Layer_SunFlower";
	wstrLayerTag[31] =	L"Layer_Tulip";

	for (int i = 0; i < 32; ++i)
	{

		HANDLE hFile = CreateFile(wstrFilePath[i].c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			E_FAIL;

		CManagement*	pManagement = GET_INSTANCE(CManagement);

		DWORD dwByte = 0;
		CTransform::TRANSFORMDESC TransformDesc;

		while (true)
		{
			ReadFile(hFile, &TransformDesc.vPosition, sizeof(_vector), &dwByte, nullptr);
			ReadFile(hFile, &TransformDesc.fScale, sizeof(_float), &dwByte, nullptr);

			if (dwByte == 0)
				break;

			if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, wstrProtoTag[i], L"Layer_StageObj", &TransformDesc)))
				return E_FAIL;
		}

		RELEASE_INSTANCE(CManagement);
		CloseHandle(hFile);
	}



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

	/* Prototype_Texture_TimeUI */
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Texture_TimeUI"), CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::WIC, TEXT("../Bin/Resources/Textures/KatamariUITexture/TimeUI/timer%d.png"), 2))))
		return E_FAIL;

	/* Prototype_Texture_TimeUI */
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Texture_ClockUI"), CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::WIC, TEXT("../Bin/Resources/Textures/KatamariUITexture/TimeUI/timerPin.png"), 1))))
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

	/* Prototype_TimeUI */
	if (FAILED(pManagement->Add_Prototype(TEXT("Prototype_TimeUI"), CTimeUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* Prototype_PlayerMoveUI */
	if (FAILED(pManagement->Add_Prototype(TEXT("Prototype_PlayerMoveUI"), CPlayerMoveUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* Prototype_StickObjUI */
	if (FAILED(pManagement->Add_Prototype(TEXT("Prototype_StickObjUI"), CStickObjUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* Prototype_ClockUI */
	if (FAILED(pManagement->Add_Prototype(TEXT("Prototype_ClockUI"), CClockUI::Create(m_pDevice, m_pDeviceContext))))
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
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	/* For.GameObject_StickUI */
	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("Prototype_TimeUI"), pLayerTag)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CStageOne::Ready_Layer_PlayerMoveUI(const wstring& pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	/* Prototype_PlayerMoveUI */
	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("Prototype_PlayerMoveUI"), pLayerTag)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CStageOne::Ready_Layer_StickObjUI(const wstring & pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	_uint i = 32;

	/* Prototype_PlayerMoveUI */
	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("Prototype_StickObjUI"), pLayerTag, &i)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CStageOne::Ready_Layer_ClockUI(const wstring & pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	/* Prototype_PlayerMoveUI */
	if (FAILED(pManagement->Add_GameObj(STAGEONE_SCENE, TEXT("Prototype_ClockUI"), pLayerTag)))
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
	//CManagement*		pManagement = GET_INSTANCE(CManagement);
	//pManagement->StopAll();
	//RELEASE_INSTANCE(CManagement);
	__super::Free();
}
