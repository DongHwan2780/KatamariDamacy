#include "stdafx.h"
#include "..\Headers\StickObjUI.h"

#include "Management.h"

CStickObjUI::CStickObjUI(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CStickObjUI::CStickObjUI(const CStickObjUI & other)
	: CObj(other)
{
}

HRESULT CStickObjUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStickObjUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_iModelIndex, pArg, sizeof(_uint));

	if (FAILED(SetUp_ComponentsOrthUI(m_iModelIndex)))
		return E_FAIL;

	m_fSizeX = 250.f;
	m_fSizeY = 250.f;

	m_fX = g_iWinCX - m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());

	m_TransformMatrix._11 = m_fSizeX;
	m_TransformMatrix._22 = m_fSizeY;

	m_TransformMatrix._41 = -550.f;
	m_TransformMatrix._42 = -260.f;
	m_TransformMatrix._43 = 0.2f;

	XMStoreFloat4x4(&m_OrthMatrix, XMMatrixOrthographicLH((float)g_iWinCX, (float)g_iWinCY, 0.0f, 1.f));

	return S_OK;
}

_int CStickObjUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;


	//m_pTransform->RotateAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), DeltaTime );

	CheckModel(m_iModelIndex);

	return _int();
}

_int CStickObjUI::Late_Update(_double DeltaTime)
{
	if (m_pRenderer == nullptr)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::PRIORITY, this);
}

HRESULT CStickObjUI::Render()
{
	if (nullptr == m_pModel)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint		iNumMaterials = (_uint)m_pModel->Get_NumMaterials();

	m_pModel->Bind_Buffers();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModel->Render(i, 0);
	}

	return S_OK;
}

HRESULT CStickObjUI::CheckModel(_uint iModelIndex)
{
	switch (iModelIndex)
	{
	case 0:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Apple", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Bag", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Banana", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 3:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Bat", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 4:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Bansai", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 5:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Book0", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 6:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Book1", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 7:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Bowling", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 8:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Cake", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 9:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Can", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 10:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Candy", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 11:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Clock", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 12:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Dice", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 13:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_DrinkCan0", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 14:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_DrinkCan1", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 15:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Egg", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 16:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Eraser", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 17:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Fork", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 18:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_FryPan", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 19:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_LipStick", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 20:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Magnet", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 21:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Melon", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 22:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_MilkPack", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 23:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Orange", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 24:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Pilon", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 25:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Pin0", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 26:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Pin1", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 27:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Radio", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 28:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Spoon", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 29:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Stand", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 30:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_SunFlower", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	case 31:
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Tulip", L"Com_Model", (CComponent**)&m_pModel)))
			return E_FAIL;
		break;
	}
}

HRESULT CStickObjUI::SetUp_ComponentsOrthUI(_uint iModelIndex)
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC	TransformDesc;
	TransformDesc.fRotatePerSec = 360.0f;
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStickObjUI::SetUp_ConstantTable()
{
	if (nullptr == m_pModel)
		return E_FAIL;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)), sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

CStickObjUI * CStickObjUI::Create(DEVICES)
{
	CStickObjUI*		pInstance = new CStickObjUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CStickObjUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CStickObjUI::Clone(void * pArg)
{
	CStickObjUI*		pInstance = new CStickObjUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CStickObjUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStickObjUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRenderer);
}
