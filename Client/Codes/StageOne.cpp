#include "stdafx.h"
#include "..\Headers\StageOne.h"


CStageOne::CStageOne(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}


HRESULT CStageOne::Initialize()
{
	return S_OK;
}

_int CStageOne::Update(_double DeltaTime)
{
	return _int();
}

HRESULT CStageOne::Render()
{
	return S_OK;
}

CStageOne * CStageOne::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CStageOne* pInstance = new CStageOne(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageOne::Free()
{
}
