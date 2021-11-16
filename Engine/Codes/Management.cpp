#include "..\Headers\Management.h"
#include "TimeMgr.h"
#include "Graphic.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	:m_pTimeMgr(CTimeMgr::GetInstance())
	, m_pGraphicDevice(CGraphic::GetInstance())
	, m_pSceneMgr(CSceneMgr::GetInstance())
	, m_pObjMgr(CObjMgr::GetInstance())
	, m_pComponentMgr(CComponentMgr::GetInstance())
{
	Safe_AddRef(m_pComponentMgr);
	Safe_AddRef(m_pObjMgr);
	Safe_AddRef(m_pSceneMgr);
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pTimeMgr);
}
#pragma region TIMER_MANAGER
HRESULT CManagement::Add_Timers(const _tchar * TimeTag)
{
	if (nullptr == m_pTimeMgr)
		return E_FAIL;


	return m_pTimeMgr->Add_Timers(TimeTag);
}

_double CManagement::Compute_DeltaTime(const _tchar * TimeTag)
{
	if (nullptr == m_pTimeMgr)
		return 0.0;

	return m_pTimeMgr->Compute_DeltaTime(TimeTag);
}
#pragma endregion

#pragma region GRAPHIC_DEVICE
HRESULT CManagement::Ready_GraphicDevice(HWND hWnd, _uint iWidth, _uint iHeight, ID3D11Device** ppOutDevice, ID3D11DeviceContext** ppOutDeviceContext)
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Ready_GraphicDevice(hWnd, iWidth, iHeight, ppOutDevice, ppOutDeviceContext);
}

HRESULT CManagement::Clear_BackBufferView(_float4 vColor)
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Clear_BackBufferView(vColor);
}

HRESULT CManagement::Clear_DepthStencilView(_float fDepth, _uint iStencil)
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Clear_DepthStencilView(fDepth, iStencil);
}

HRESULT CManagement::Present()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Present();
}
#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CManagement::Add_Prototype(const _tchar * pPrototypeTag, class CObj * pPrototype)
{
	if (m_pObjMgr == nullptr)
		return E_FAIL;

	return m_pObjMgr->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CManagement::Add_GameObj(_int iSceneIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg)
{
	if (m_pObjMgr == nullptr)
		return E_FAIL;

	return m_pObjMgr->Add_GameObj(iSceneIndex, pPrototypeTag, pLayerTag, pArg);
}

void CManagement::Clear_ObjMgr(_int iSceneIndex)
{
	if (m_pObjMgr == nullptr)
		return;

	m_pObjMgr->Clear(iSceneIndex);
}
#pragma endregion

#pragma region COMPONENT_MANAGER
HRESULT CManagement::Add_Prototype(_int iSceneIndex, const _tchar * pPrototypeTag, class CComponent * pPrototype)
{
	if (m_pComponentMgr == nullptr)
		return E_FAIL;

	return m_pComponentMgr->Add_Prototype(iSceneIndex, pPrototypeTag, pPrototype);
}

CComponent * CManagement::Clone_Component(_int iSceneIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (m_pComponentMgr == nullptr)
		return nullptr;

	return m_pComponentMgr->Clone_Component(iSceneIndex, pPrototypeTag, pArg);
}

void CManagement::Clear_ComponentMgr(_int iSceneIndex)
{
	if (m_pComponentMgr == nullptr)
		return;

	m_pComponentMgr->Clear(iSceneIndex);
}
#pragma endregion

HRESULT CManagement::Set_CurScene(CScene * pCurScene)
{
	if (nullptr == m_pSceneMgr)
		return E_FAIL;

	return m_pSceneMgr->Set_CurScene(pCurScene);
}

_int CManagement::Update_Scene(_double DeltaTime)
{
	if (nullptr == m_pSceneMgr)
		return -1;

	return m_pSceneMgr->Update_Scene(DeltaTime);
}

HRESULT CManagement::Render_Scene()
{
	if (nullptr == m_pSceneMgr)
		return E_FAIL;

	return m_pSceneMgr->Render_Scene();
}

HRESULT CManagement::Initialize_Engine(_int iNumScenes)
{
	if (m_pObjMgr == nullptr || m_pComponentMgr == nullptr)
		return E_FAIL;

	if (FAILED(m_pObjMgr->Reserve_Manager(iNumScenes)))
		return E_FAIL;

	if (FAILED(m_pComponentMgr->Reserve_Manager(iNumScenes)))
		return E_FAIL;

	return S_OK;
}

_int CManagement::Update(_double DeltaTime)
{
	if (m_pObjMgr == nullptr)
		return -1;

	_int iProgress = 0;

	if (0 > (iProgress = m_pObjMgr->Update(DeltaTime)))
		return -1;


	if (0 > (iProgress = m_pObjMgr->Late_Update(DeltaTime)))
		return -1;


	return _int();
}

void CManagement::Release_Engine()
{
	if (0 != CManagement::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CGameInstance");

	if (0 != CTimeMgr::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CTimer_Manager");

	if(0 != CSceneMgr::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CSceneMgr");

	if (0 != CObjMgr::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CObjMgr");

	if (0 != CComponentMgr::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CComponentMgr");

	if (0 != CGraphic::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CGraphic");
}

void CManagement::UpdateTool()
{
	//m_pTimeMgr->Add_Timers(TEXT("Tool_Timer"));
	//_double DeltaTime = m_pTimeMgr->Compute_DeltaTime(TEXT("Tool_Timer"));

	//m_pObjMgr->Update(DeltaTime);
	//m_pObjMgr->Late_Update(DeltaTime);

	m_pGraphicDevice->Clear_BackBufferView(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGraphicDevice->Clear_DepthStencilView(1.f, 0);
	
	//m_pRenderer->Draw_RenderGroup();

	m_pGraphicDevice->Present();
}

void CManagement::Free()
{
	Safe_Release(m_pComponentMgr);
	Safe_Release(m_pObjMgr);
	Safe_Release(m_pSceneMgr);
	Safe_Release(m_pTimeMgr);
	Safe_Release(m_pGraphicDevice);
}
