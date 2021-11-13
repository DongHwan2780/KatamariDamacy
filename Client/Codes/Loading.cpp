#include "stdafx.h"
#include "..\Headers\Loading.h"

#include "Management.h"
#include "Loader.h"
#include "StageOne.h"

CLoading::CLoading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}

HRESULT CLoading::Initialize(SCENE eScene)
{
	__super::Initialize();

	m_pLoader = CLoader::Create(eScene);

	if (m_pLoader == nullptr)
		return E_FAIL;

	m_eNextScene = eScene;

	return S_OK;
}

_int CLoading::Update(_double DeltaTime)
{
	__super::Update(DeltaTime);

	if (m_pLoader->Get_Finish())	// �ε��� ������ == �����尡 �� ����
	{
		CManagement*		pManagement = GET_INSTANCE(CManagement);

		SetWindowText(g_hWnd, TEXT("�ε� �Ϸ�"));		// �ؽ�Ʈ ����ϰ�

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)	// �ε��Ϸ� ���¿��� Ű�� ������
		{
			CScene*	pScene = nullptr;

			switch (m_eNextScene)		// �ε��� �̴ϼȶ������ �޾ƿ� enumID���� ���� ���������� �̵�������
			{
			case STAGEONE_SCENE:
				pScene = CStageOne::Create(m_pDevice, m_pDeviceContext);
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

CLoading * CLoading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, SCENE eScene)
{
	CLoading*	pInstance = new CLoading(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize(eScene)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading::Free()
{
	__super::Free();
}
