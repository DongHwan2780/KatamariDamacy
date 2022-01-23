#include "stdafx.h"
#include "..\Headers\SizeUI.h"

#include "Management.h"
#include "PlayerBall.h"

CSizeUI::CSizeUI(DEVICES)
	:CUIList(pDevice, pDeviceContext)
{
}

CSizeUI::CSizeUI(const CSizeUI & other)
	:CUIList(other)
{
}

HRESULT CSizeUI::Initialize_Prototype() 
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSizeUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_ComponentsOrthUI()))
		return E_FAIL;

	m_fSizeX = 312.f;
	m_fSizeY = 312.f;

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());		// 백그라운드
	m_TransformMatrix._11 = m_fSizeX;
	m_TransformMatrix._22 = m_fSizeY;
	m_TransformMatrix._41 = -600.f;
	m_TransformMatrix._42 = 300.f;
	m_TransformMatrix._43 = 0.3f;

	XMStoreFloat4x4(&m_TransformMatrixCM, XMMatrixIdentity());		// CM
	m_TransformMatrixCM._11 = 70.f;
	m_TransformMatrixCM._22 = 30.f;
	m_TransformMatrixCM._41 = -510.f;
	m_TransformMatrixCM._42 = 295.f;

	XMStoreFloat4x4(&m_TransformMatrixMM, XMMatrixIdentity());		//MM
	m_TransformMatrixMM._11 = 95.f;
	m_TransformMatrixMM._22 = 30.f;
	m_TransformMatrixMM._41 = -380.f;
	m_TransformMatrixMM._42 = 295.f;

	XMStoreFloat4x4(&m_TransformMatrixTensCM, XMMatrixIdentity());		// TensNum_CM
	m_TransformMatrixTensCM._11 = 32.f;
	m_TransformMatrixTensCM._22 = 42.f;
	m_TransformMatrixTensCM._41 = -600.f;
	m_TransformMatrixTensCM._42 = 300.f;

	XMStoreFloat4x4(&m_TransformMatrixOnesCM, XMMatrixIdentity());		// OnesNum_CM
	m_TransformMatrixOnesCM._11 = 32.f;
	m_TransformMatrixOnesCM._22 = 42.f;
	m_TransformMatrixOnesCM._41 = -565.f;
	m_TransformMatrixOnesCM._42 = 300.f;

	XMStoreFloat4x4(&m_TransformMatrixNumMM, XMMatrixIdentity());		// Num_MM
	m_TransformMatrixNumMM._11 = 32.f;
	m_TransformMatrixNumMM._22 = 42.f;
	m_TransformMatrixNumMM._41 = -450.f;
	m_TransformMatrixNumMM._42 = 300.f;

	XMStoreFloat4x4(&m_OrthMatrix, XMMatrixOrthographicLH((float)g_iWinCX, (float)g_iWinCY, 0.0f, 1.f));


	return S_OK;
}

_int CSizeUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	if (m_iIdx < 14.0)
		m_iIdx += DeltaTime * 14.0;
	else
 		m_iIdx = 0.0;

	Calculate_Size(DeltaTime);

	return _int();
}

_int CSizeUI::Late_Update(_double DeltaTime)
{
	if (m_pRenderer == nullptr)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	if (m_iIdx >= 14.0)
		m_iIdx = 0.0;

	return m_pRenderer->Add_RenderGroup(CRenderer::UI, this);
}

HRESULT CSizeUI::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(/*m_pTransform->Get_WorldMatrix() * */XMLoadFloat4x4(&m_TransformMatrix)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, (_uint)m_iIdx)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrixCM)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTextureCM)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrixMM)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTextureMM)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrixTensCM)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTextureNum, m_iIndexTensCM)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrixOnesCM)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTextureNum, m_iIndexOnesCM)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrixNumMM)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTextureNum, m_iIndexOnesMM)))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	return S_OK;
}

HRESULT CSizeUI::SetUp_ComponentsOrthUI()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_VIBuffer_UI"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, TEXT("Component_Texture_SizeUI"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, TEXT("Component_Texture_CM_UI"), TEXT("Com_TextureCM"), (CComponent**)&m_pTextureCM)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, TEXT("Component_Texture_MM_UI"), TEXT("Com_TextureMM"), (CComponent**)&m_pTextureMM)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, TEXT("Component_Texture_TimeNumUI"), TEXT("Com_TextureNum"), (CComponent**)&m_pTextureNum)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
		return E_FAIL;


	return S_OK;
}

void CSizeUI::Calculate_Size(_double DeltaTime)
{
	CManagement*	pManagement = GET_INSTANCE(CManagement);
	m_pPlayerBall = pManagement->GetGameObject(STAGEONE_SCENE, L"Layer_PlayerBall");

	CPlayerBall* pPlayerBall = dynamic_cast<CPlayerBall*>(m_pPlayerBall);
	_float fBallSize = pPlayerBall->Get_PlayerBallSize();

		// m_dLimitTime == 공 사이즈값
	m_iIndexTensCM = (int)fBallSize / 100;
	m_iIndexOnesCM = ((int)fBallSize % 100) / 10;
	m_iIndexOnesMM = ((int)fBallSize % 100) % 10;

	m_TransformMatrix._11 += (fBallSize / 100.f) * 0.01f;
	m_TransformMatrix._22 += (fBallSize / 100.f) * 0.01f;

	RELEASE_INSTANCE(CManagement);

}

CSizeUI * CSizeUI::Create(DEVICES)
{
	CSizeUI*		pInstance = new CSizeUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CSizeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CSizeUI::Clone(void * pArg)
{
	CSizeUI*		pInstance = new CSizeUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CSizeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSizeUI::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
	Safe_Release(m_pTextureCM);
	Safe_Release(m_pTextureMM);
	Safe_Release(m_pTextureNum);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
}
