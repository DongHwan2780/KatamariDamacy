#include "..\Headers\Management.h"
#include "TimeMgr.h"
#include "Graphic.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pTimeMgr(CTimeMgr::GetInstance())
	, m_pGraphicDevice(CGraphic::GetInstance())
	, m_pInputDevice(CInput::GetInstance())
	, m_pSceneMgr(CSceneMgr::GetInstance())
	, m_pObjMgr(CObjMgr::GetInstance())
	, m_pComponentMgr(CComponentMgr::GetInstance())
	, m_pSoundMgr(CSoundMgr::GetInstance())
	, m_pPipeline(CPipeLine::GetInstance())
	, m_pLightMgr(CLightMgr::GetInstance())
{
	Safe_AddRef(m_pLightMgr);
	Safe_AddRef(m_pPipeline);
	Safe_AddRef(m_pComponentMgr);
	Safe_AddRef(m_pObjMgr);
	Safe_AddRef(m_pSceneMgr);
	Safe_AddRef(m_pInputDevice);
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pTimeMgr);
	Safe_AddRef(m_pSoundMgr);
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
CComponent * CManagement::GetComponent(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex)
{
	return m_pObjMgr->GetComponent(iLevelIndex, pLayerTag, pComponentTag, iIndex);;
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

#pragma region SCENE_MGR
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
#pragma endregion

#pragma region SOUND_MGR
void CManagement::PlaySounds(_tchar * pSoundKey, CSoundMgr::SOUNDCHANNEL eID)
{
	m_pSoundMgr->PlaySounds(pSoundKey, eID);
}

void CManagement::PlayBGM(_tchar * pSoundKey)
{
	m_pSoundMgr->PlayBGM(pSoundKey);
}

void CManagement::StopSound(CSoundMgr::SOUNDCHANNEL eID)
{
	m_pSoundMgr->StopSound(eID);
}

void CManagement::StopAll()
{
	m_pSoundMgr->StopAll();
}

void CManagement::Pause(CSoundMgr::SOUNDCHANNEL eID)
{
	m_pSoundMgr->Pause(eID);
}

void CManagement::Resume(CSoundMgr::SOUNDCHANNEL eID)
{
	m_pSoundMgr->Resume(eID);
}

void CManagement::Set_Volume(CSoundMgr::SOUNDCHANNEL eID, float _fVolume)
{
	m_pSoundMgr->Set_Volume(eID, _fVolume);
}


#pragma endregion

#pragma region PIPELINE
_matrix CManagement::Get_Transform(CPipeLine::TYPE eType)
{
	if (nullptr == m_pPipeline)
		return XMMatrixIdentity();

	return m_pPipeline->Get_Transform(eType);
}

_vector CManagement::Get_CamPosition()
{
	if (nullptr == m_pPipeline)
		return XMVectorZero();

	return m_pPipeline->Get_CamPosition();
}
HRESULT CManagement::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	if (m_pInputDevice == nullptr)
		return E_FAIL;

	return m_pInputDevice->Ready_InputDevice(hInst, hWnd);
}
_byte CManagement::Get_DIKState(_ubyte byKeyID)
{
	if (m_pInputDevice == nullptr)
		return 0;

	return m_pInputDevice->Get_DIKState(byKeyID);
}
_long CManagement::Get_MouseMoveState(CInput::MOUSEMOVESTATE eMoveState)
{
	if (m_pInputDevice == nullptr)
		return 0;

	return m_pInputDevice->Get_MouseMoveState(eMoveState);
}
_byte CManagement::Get_MouseButtonState(CInput::MOUSEBUTTONSTATE eButtonState)
{
	if (m_pInputDevice == nullptr)
		return 0;

	return m_pInputDevice->Get_MouseButtonState(eButtonState);
}
#pragma endregion

#pragma region LIGHT_MGR

const LIGHTDESC * CManagement::Get_LightDesc(_uint Index) const
{
	if(m_pLightMgr == nullptr)
		return nullptr;

	return m_pLightMgr->Get_LightDesc(Index);
}
HRESULT CManagement::Add_Light(DEVICES, const LIGHTDESC & LightDesc)
{
	if (m_pLightMgr == nullptr)
		return E_FAIL;

	return m_pLightMgr->Add_Light(pDevice, pDeviceContext, LightDesc);
}
#pragma endregion


HRESULT CManagement::Initialize_Engine(HINSTANCE hInst, HWND hWnd, _int iNumScenes)
{
	if (m_pObjMgr == nullptr || m_pComponentMgr == nullptr || m_pInputDevice == nullptr)
		return E_FAIL;

	if (FAILED(m_pInputDevice->Ready_InputDevice(hInst, hWnd)))
		return E_FAIL;

	if (FAILED(m_pObjMgr->Reserve_Manager(iNumScenes)))
		return E_FAIL;

	if (FAILED(m_pComponentMgr->Reserve_Manager(iNumScenes)))
		return E_FAIL;

	//m_physx->Initialize_Physx();

	//m_pSoundMgr->Initialize_Sound();

	return S_OK;
}

_int CManagement::Update(_double DeltaTime)
{
	if (m_pObjMgr == nullptr || m_pInputDevice == nullptr)
		return -1;

	_int iProgress = 0;
	
	if (FAILED(m_pInputDevice->SetUp_InputDeviceState()))
		return -1;


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

	if (0 != CSoundMgr::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CSoundMgr");

	if (0 != CPipeLine::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CPipeLine");

	if (0 != CLightMgr::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CLightMgr");

	//if (0 != CPhysX::GetInstance()->DestroyInstance())
	//	MSG_BOX("Failed to Deleting CPhysX");

	if (0 != CInput::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CInput");

	if (0 != CGraphic::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Deleting CGraphic");
}

void CManagement::UpdateTool()
{
	m_pGraphicDevice->Clear_BackBufferView(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGraphicDevice->Clear_DepthStencilView(1.f, 0);


	m_pGraphicDevice->Present();
}

void CManagement::Free()
{
	Safe_Release(m_pLightMgr);
	Safe_Release(m_pPipeline);
	//Safe_Release(m_physx);
	Safe_Release(m_pSoundMgr);
	Safe_Release(m_pComponentMgr);
	Safe_Release(m_pObjMgr);
	Safe_Release(m_pSceneMgr);
	Safe_Release(m_pTimeMgr);
	Safe_Release(m_pInputDevice);
	Safe_Release(m_pGraphicDevice);
}
