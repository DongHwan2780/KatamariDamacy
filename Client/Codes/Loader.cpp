#include "stdafx.h"
#include "..\Headers\Loader.h"

#include "Management.h"
#include "Player.h"
#include "Terrain.h"
#include "MainCamera.h"

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

	/* For.VIBuffer Component */
	/* Component_VIBuffer_Terrain */
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Height.bmp"), TEXT("../Bin/ShaderFiles/Shader_Terrain.fx")))))
		return E_FAIL;

	/* For.Texture Component */
	/* Component_Texture_Terrain */
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Texture_Terrain"), CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TGA, TEXT("../Bin/Resources/Textures/Grass_%d.tga"), 2))))
		return E_FAIL;

	/* Component_Texture_Filter*/
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Texture_Filter"), CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::WIC, TEXT("../Bin/Resources/Textures/Filter.bmp")))))
		return E_FAIL;
	/* Component_Texture_Brush*/
	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Texture_Brush"), CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TGA, TEXT("../Bin/Resources/Textures/Brush.tga")))))
		return E_FAIL;


	/* For.Model Component */
	/* Component_Model_Player*/
	_matrix		ScaleMatrix, RotationMatrix, TranslationMatrix;
	_matrix		ModelPivotMatrix;
	ScaleMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	RotationMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	ModelPivotMatrix = ScaleMatrix * RotationMatrix;

	if (FAILED(pManagement->Add_Prototype(STAGEONE_SCENE, TEXT("Component_Model_Player"), CModel::Create(m_pDevice, m_pDeviceContext, "../Bin/Resources/Meshes/Katamari/", "OUJI01.fbx", TEXT("../Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix))))
		return E_FAIL;

	/* For.Collider Component */
	if (FAILED(pManagement->Add_Prototype(STATIC_SCENE, TEXT("Component_Collider_AABB"), CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::COLL_AABB))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* GameObject_Player*/
	if (FAILED(pManagement->Add_Prototype(TEXT("GameObject_Player"), CPlayer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* GameObject_Terrain */
	if (FAILED(pManagement->Add_Prototype(TEXT("GameObject_Terrain"), CTerrain::Create(m_pDevice, m_pDeviceContext))))
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
