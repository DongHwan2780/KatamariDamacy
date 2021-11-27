#include "stdafx.h"
#include "..\Headers\StageOne.h"

#include "Management.h"

CStageOne::CStageOne(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex)
	:CScene(pDevice, pDeviceContext, iSceneIndex)
{
}


HRESULT CStageOne::Initialize()
{
	__super::Initialize();

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

CStageOne * CStageOne::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex)
{
	CStageOne* pInstance = new CStageOne(pDevice, pDeviceContext, iSceneIndex);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageOne::Free()
{
	__super::Free();
}
