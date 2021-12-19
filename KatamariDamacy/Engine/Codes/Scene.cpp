#include "..\Headers\Scene.h"
#include "Management.h"

CScene::CScene(ID3D11Device* pDevice, ID3D11DeviceContext*	pDeviceContext, _int iSceneIndex)
	:m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_iSceneIndex(iSceneIndex)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CScene::Initialize()
{
	return S_OK;
}

_int CScene::Update(_double DeltaTime)
{
	return _int();
}

HRESULT CScene::Render()
{
	return S_OK;
}

void CScene::Clear()
{
	CManagement* pManagement = GET_INSTANCE(CManagement);

	pManagement->Clear_ObjMgr(m_iSceneIndex);
	pManagement->Clear_ComponentMgr(m_iSceneIndex);

	RELEASE_INSTANCE(CManagement);
}

void CScene::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
