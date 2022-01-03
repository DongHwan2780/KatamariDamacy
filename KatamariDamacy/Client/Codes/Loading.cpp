#include "stdafx.h"
#include "..\Headers\Loading.h"

#include "Management.h"
#include "Loader.h"
#include "StageOne.h"
#include "LoadingUI.h"

CLoading::CLoading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex)
	:CScene(pDevice, pDeviceContext, iSceneIndex)
{
}

HRESULT CLoading::Initialize(SCENE eScene)
{
	__super::Initialize();

	m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, eScene);

	if (m_pLoader == nullptr)
		return E_FAIL;

	m_eNextScene = eScene;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Layer_LoadingUI(TEXT("Layer_LoadingUI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"), 1)))
		return E_FAIL;

	return S_OK;
}
_int CLoading::Update(_double DeltaTime)
{
	__super::Update(DeltaTime);

	if (m_pLoader->Get_Finish() == true)	// 로딩이 끝나면 == 쓰레드가 다 돌면
	{
		CManagement*		pManagement = GET_INSTANCE(CManagement);

		SetWindowText(g_hWnd, TEXT("로딩 완료"));		// 텍스트 출력하고

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)	// 로딩완료 상태에서 키를 누르면
		{
			CScene*	pScene = nullptr;

			switch (m_eNextScene)		// 로딩씬 이니셜라이즈에서 받아온 enumID값에 따라 다음씬으로 이동시켜줌
			{
			case STAGEONE_SCENE:
				pScene = CStageOne::Create(m_pDevice, m_pDeviceContext, STAGEONE_SCENE);
				break;
			}

			if (FAILED(pManagement->Set_CurScene(pScene)))
				return E_FAIL;
		}

		RELEASE_INSTANCE(CManagement);
	}

	return _int();
}

HRESULT CLoading::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CLoading::Ready_Prototype_Component()
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);


	/* Prototype_Texture_BackGround */
	if (FAILED(pManagement->Add_Prototype(LOADING_SCENE, TEXT("Component_Texture_LoadingUI"), CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::WIC, TEXT("../Bin/Resources/Textures/KatamariUITexture/Loading/Loading%d.png"), 2))))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CLoading::Ready_Prototype_GameObject()
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);


	/* Prototype_BackGround */
	if (FAILED(pManagement->Add_Prototype(TEXT("Prototype_LoadingUI"), CLoadingUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CLoading::Ready_Layer_BackGround(const wstring & pLayerTag, _uint iSceneIndex)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	/* For.GameObject_BackGround */
	if (FAILED(pManagement->Add_GameObj(LOADING_SCENE, TEXT("Prototype_BackGround"), pLayerTag, &iSceneIndex)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

HRESULT CLoading::Ready_Layer_LoadingUI(const wstring & pLayerTag)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);

	/* For.GameObject_BackGround */
	if (FAILED(pManagement->Add_GameObj(LOADING_SCENE, TEXT("Prototype_LoadingUI"), pLayerTag)))
		return E_FAIL;

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

CLoading * CLoading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, SCENE eScene, _uint iSceneIndex)
{
	CLoading*	pInstance = new CLoading(pDevice, pDeviceContext, iSceneIndex);

	if (FAILED(pInstance->Initialize(eScene)))
	{
		MSG_BOX("Failed to Creating CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
