#include "stdafx.h"
#include "..\Headers\Loader.h"

#include "Management.h"


#include "Player.h"
#include "PlayerBall.h"
#include "Terrain.h"
#include "MainCamera.h"
#include "StageMap.h"
#include "Terrain.h"

#include "Dummy.h"

#include "Apple.h"
#include "Bag.h"
#include "Banana.h"
#include "Bat.h"
#include "Bansai.h"
#include "Book0.h"
#include "Book1.h"
#include "Bowling.h"
#include "Cake.h"
#include "Can.h"
#include "Candy.h"
#include "Clock.h"

#include "Dice.h"
#include "DrinkCan0.h"
#include "DrinkCan1.h"
#include "Egg.h"
#include "Eraser.h"
#include "Fork.h"
#include "FryPan.h"
#include "LipStick.h"
#include "Magnet.h"
#include "Melon.h"
#include "MilkPack.h"
#include "Orange.h"
#include "Pilon.h"
#include "Pin0.h"
#include "Pin1.h"
#include "Radio.h"
#include "Spoon.h"
#include "Stand.h"
#include "SunFlower.h"

#include "Tulip.h"

CLoader::CLoader(DEVICES)
	:m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

unsigned int APIENTRY ThreadMain(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	HRESULT			hr = 0;

	switch (pLoader->Get_NextScene())
	{
	case STAGEONE_SCENE:
		hr = pLoader->StageLoader();
		break;
	}

	if (FAILED(hr))
	{
		return 0;
	}

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}


HRESULT CLoader::Initialize_Thread(SCENE eScene)
{
	InitializeCriticalSection(&m_CS);

	m_eScene = eScene;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadMain, this, 0, nullptr);

	if (m_hThread == 0)
		return E_FAIL;


	return S_OK;
}

HRESULT CLoader::StageLoader()
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	/* For.Model Component */
	/* Component_Model_Player*/
	_matrix		ScaleMatrix, RotationMatrix, TranslationMatrix;
	_matrix		ModelPivotMatrix;
	ScaleMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	RotationMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	ModelPivotMatrix = ScaleMatrix * RotationMatrix;
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Model_Player"), CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/Katamari/", "OUJI01.fbx", TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix))))
		return E_FAIL;

	ModelPivotMatrix = ScaleMatrix;
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Model_PlayerUI"), CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/Katamari/", "OUJI01.fbx", TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix))))
		return E_FAIL;

	ScaleMatrix = XMMatrixScaling(30.f, 30.f, 30.f);
	ModelPivotMatrix = ScaleMatrix;
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Model_PlayerBall"), CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/Katamari/", "PlayerBall.fbx", TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix))))
		return E_FAIL;

	/* Component_Model_StageMap*/
	ScaleMatrix = XMMatrixScaling(15.f, 15.f,15.f);
	RotationMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	ModelPivotMatrix = ScaleMatrix /** RotationMatrix*/;
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Model_StageMap"), CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/StageMap/", "StageSkyMap.fbx", TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix))))
		return E_FAIL;



	ScaleMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	RotationMatrix = XMMatrixRotationZ(XMConvertToRadians(180.0f));
	ModelPivotMatrix = ScaleMatrix * RotationMatrix;
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Apple", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Apple.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Bag", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Bag.fbx",				TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Banana", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Banana.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Bat", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Bat.fbx",				TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Bansai", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Bansai.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Book0", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Book0.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Book1", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Book1.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Bowling", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Bowling.fbx",		TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Cake", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Cake.fbx",				TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Can", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Can.fbx",				TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Candy", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Candy.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Clock", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Clock.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Dice", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Dice.fbx",				TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_DrinkCan0", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "DrinkCan0.fbx",	TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_DrinkCan1", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "DrinkCan1.fbx",	TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Egg", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Egg.fbx",				TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Eraser", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Eraser.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Fork", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Fork.fbx",				TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_FryPan", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "FryPan.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_LipStick", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "LipStick.fbx",		TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Magnet", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Magnet.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Melon", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Melon.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_MilkPack", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "MilkPack.fbx",		TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Orange", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Orange.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Pilon", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Pilon.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Pin0", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Pin0.fbx",				TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Pin1", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Pin1.fbx",				TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Radio", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Radio.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Spoon", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Spoon.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Stand", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Stand.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_SunFlower", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "SunFlower.fbx",	TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	pManagement->Add_Prototype(STAGEONE_SCENE, L"Component_Model_Tulip", CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/Object/", "Tulip.fbx",			TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* For.Collider Component */
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Collider_AABB"), CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::COLL_AABB))))
		return E_FAIL;

	/* For.Collider Component */
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Collider_OBB"), CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::COLL_OBB))))
		return E_FAIL;

	/* For.Collider Component */
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Collider_SPHERE"), CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::COLL_SPHERE))))
		return E_FAIL;

	/* For.Terrain Buffer */
	LoadDataTerrain();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* GameObject_Player*/
	if (FAILED(pManagement->Add_Prototype(TEXT("GameObject_Player"), CPlayer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* GameObject_PlayerBall*/
	if (FAILED(pManagement->Add_Prototype(TEXT("GameObject_PlayerBall"), CPlayerBall::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* GameObject_StageMap */
	if (FAILED(pManagement->Add_Prototype(TEXT("GameObject_StageMap"), CStageMap::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* GameObject_MainCamera*/
	if (FAILED(pManagement->Add_Prototype(TEXT("GameObject_MainCamera"), CMainCamera::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* GameObject_Terrain*/
	if (FAILED(pManagement->Add_Prototype(TEXT("GameObject_Terrain"), CTerrain::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	pManagement->Add_Prototype(L"GameObject_Dummy",		CDummy::Create(m_pDevice, m_pDeviceContext));

	pManagement->Add_Prototype(L"GameObject_Apple",		CApple::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Bag",		CBag::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Banana",	CBanana::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Bat",		CBat::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Bansai",	CBansai::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Book0",		CBook0::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Book1",		CBook1::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Bowling",	CBowling::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Cake",		CCake::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Can",		CCan::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Candy",		CCandy::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Clock",		CClock::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Dice",		CDice::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_DrinkCan0",	CDrinkCan0::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_DrinkCan1",	CDrinkCan1::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Egg",		CEgg::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Eraser",	CEraser::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Fork",		CFork::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_FryPan",	CFryPan::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_LipStick",	CLipStick::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Magnet",	CMagnet::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Melon",		CMelon::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_MilkPack",	CMilkPack::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Orange",	COrange::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Pilon",		CPilon::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Pin0",		CPin0::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Pin1",		CPin1::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Radio",		CRadio::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Spoon",		CSpoon::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Stand",		CStand::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_SunFlower",	CSunFlower::Create(m_pDevice, m_pDeviceContext));
	pManagement->Add_Prototype(L"GameObject_Tulip",		CTulip::Create(m_pDevice, m_pDeviceContext));

	RELEASE_INSTANCE(CManagement);

	m_bFinish = true;

	return S_OK;
}

HRESULT CLoader::LoadDataTerrain(void * pArg)
{
	HANDLE hFile = CreateFile(L"../../MFCTool/Data/Object/TerrainMap.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		E_FAIL;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	DWORD dwByte = 0;
	_float3 pVertexPos;

	_uint m_iTileX;
	_uint m_iTileY;
	_float m_fTileInterval;

	ReadFile(hFile, &m_iTileX, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iTileY, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_fTileInterval, sizeof(_float), &dwByte, nullptr);
	int i = 0;

	pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_VIBuffer_Terrain"), CVIBuffer_Terrain::CreateLoadData(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_Terrain.fx"), hFile, m_iTileX, m_iTileY, m_fTileInterval));

	CloseHandle(hFile);

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

CLoader * CLoader::Create(DEVICES, SCENE eScene)
{
	CLoader* pInstance = new CLoader(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Thread(eScene)))
	{
		MSG_BOX("Failed to Creating CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CS);

	DeleteObject(m_hThread);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
