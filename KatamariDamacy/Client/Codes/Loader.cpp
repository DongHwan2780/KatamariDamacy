#include "stdafx.h"
#include "..\Headers\Loader.h"

#include "Management.h"


#include "Player.h"
#include "PlayerBall.h"
#include "Terrain.h"
#include "MainCamera.h"
#include "StageMap.h"

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

	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Model_StageMap"), CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/GameObject/StageMap/", "StageMap.fbx", TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* For.Collider Component */
	if (FAILED(pManagement->Add_Prototype(STATIC_SCENE, TEXT("Component_Collider_AABB"), CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::COLL_AABB))))
		return E_FAIL;

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

	/* GameObject_Camera_Fly*/
	if (FAILED(pManagement->Add_Prototype(TEXT("GameObject_MainCamera"), CMainCamera::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	m_bFinish = true;

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
