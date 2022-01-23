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

	m_fSizeX = 150.f;
	m_fSizeY = 150.f;

	m_fX = g_iWinCX - m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());

	m_TransformMatrix._11 = m_fSizeX;
	m_TransformMatrix._22 = m_fSizeY;

	m_TransformMatrix._41 = -550.f;
	m_TransformMatrix._42 = -260.f;
	m_TransformMatrix._43 =  0.2f;

	XMStoreFloat4x4(&m_OrthMatrix, XMMatrixOrthographicLH((float)g_iWinCX, (float)g_iWinCY, 0.0f, 1.f));

	return S_OK;
}

_int CStickObjUI::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	m_iModelIndex = m_pModel->GetModelDesc().iModelIndexNum;

	_vector vYAxis;
	vYAxis = { 0.f, 1.f, 1.f, 0.f };
	m_pTransform->RotateAxis(vYAxis, DeltaTime );

	Scale_adj(m_iModelIndex);

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

	if (m_iModelIndex != 32)
	{
		if (FAILED(__super::Render()))
			return E_FAIL;

		if (FAILED(SetUp_ConstantTable()))
			return E_FAIL;

		_uint		iNumMaterials = (_uint)m_pModelArray[m_iModelIndex]->Get_NumMaterials();

		m_pModelArray[m_iModelIndex]->Bind_Buffers();

		for (_uint i = 0; i < iNumMaterials; ++i)
		{
			m_pModelArray[m_iModelIndex]->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
			m_pModelArray[m_iModelIndex]->Render(i, 0);
		}
	}

	return S_OK;
}

HRESULT CStickObjUI::CheckModel(_uint iModelIndex)
{
	return S_OK;
}

void CStickObjUI::Scale_adj(_uint iModelIndex)
{
	_float	fScale = m_pModel->GetModelDesc().fModelScale;
	if (iModelIndex == 0 || iModelIndex == 2 || iModelIndex == 13 || iModelIndex == 14 || iModelIndex == 15 || iModelIndex == 16 || iModelIndex == 19 ||
		iModelIndex == 23 || iModelIndex == 25 || iModelIndex == 26 || iModelIndex == 12)
	{
		m_pTransform->Set_Scale(XMVectorSet(fScale / 2.f, fScale / 2.f, fScale / 2.f, 0.f));
	}
	else if(iModelIndex == 1 || iModelIndex == 8 || iModelIndex == 3 || iModelIndex == 18 || iModelIndex == 11)
		m_pTransform->Set_Scale(XMVectorSet(fScale / 7.f , fScale / 7.f , fScale / 7.f, 0.f));
	else
		m_pTransform->Set_Scale(XMVectorSet(fScale / 5.f, fScale / 5.f, fScale / 5.f, 0.f));

}

HRESULT CStickObjUI::SetUp_ComponentsOrthUI(_uint iModelIndex)
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */
	wstring wstrModelTag[32] = { L"", L"", L"", L"", L"", L"", L"", L"", L"", L""
		, L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L""
		, L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L""
		, L"" , L"" };

	wstring wstrComTag[32] = { L"", L"", L"", L"", L"", L"", L"", L"", L"", L""
		, L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L""
		, L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L"" , L""
		, L"" , L"" };

	wstrModelTag[0] = L"Component_Model_Apple";
	wstrModelTag[1] = L"Component_Model_Bag";
	wstrModelTag[2] = L"Component_Model_Banana";
	wstrModelTag[3] = L"Component_Model_Bat";
	wstrModelTag[4] = L"Component_Model_Bansai";
	wstrModelTag[5] = L"Component_Model_Book0";
	wstrModelTag[6] = L"Component_Model_Book1";
	wstrModelTag[7] = L"Component_Model_Bowling";
	wstrModelTag[8] = L"Component_Model_Cake";
	wstrModelTag[9] = L"Component_Model_Can";
	wstrModelTag[10] = L"Component_Model_Candy";
	wstrModelTag[11] = L"Component_Model_Clock";
	wstrModelTag[12] = L"Component_Model_Dice";
	wstrModelTag[13] = L"Component_Model_DrinkCan0";
	wstrModelTag[14] = L"Component_Model_DrinkCan1";
	wstrModelTag[15] = L"Component_Model_Egg";
	wstrModelTag[16] = L"Component_Model_Eraser";
	wstrModelTag[17] = L"Component_Model_Fork";
	wstrModelTag[18] = L"Component_Model_FryPan";
	wstrModelTag[19] = L"Component_Model_LipStick";
	wstrModelTag[20] = L"Component_Model_Magnet";
	wstrModelTag[21] = L"Component_Model_Melon";
	wstrModelTag[22] = L"Component_Model_MilkPack";
	wstrModelTag[23] = L"Component_Model_Orange";
	wstrModelTag[24] = L"Component_Model_Pilon";
	wstrModelTag[25] = L"Component_Model_Pin0";
	wstrModelTag[26] = L"Component_Model_Pin1";
	wstrModelTag[27] = L"Component_Model_Radio";
	wstrModelTag[28] = L"Component_Model_Spoon";
	wstrModelTag[29] = L"Component_Model_Stand";
	wstrModelTag[30] = L"Component_Model_SunFlower";
	wstrModelTag[31] = L"Component_Model_Tulip";

	wstrComTag[0] =	L"Com_ModelArray0";
	wstrComTag[1] = L"Com_ModelArray1";
	wstrComTag[2] = L"Com_ModelArray2";
	wstrComTag[3] = L"Com_ModelArray3";
	wstrComTag[4] = L"Com_ModelArray4";
	wstrComTag[5] = L"Com_ModelArray5";
	wstrComTag[6] = L"Com_ModelArray6";
	wstrComTag[7] = L"Com_ModelArray7";
	wstrComTag[8] = L"Com_ModelArray8";
	wstrComTag[9] = L"Com_ModelArray9";
	wstrComTag[10] = L"Com_ModelArray10";
	wstrComTag[11] = L"Com_ModelArray11";
	wstrComTag[12] = L"Com_ModelArray12";
	wstrComTag[13] = L"Com_ModelArray13";
	wstrComTag[14] = L"Com_ModelArray14";
	wstrComTag[15] = L"Com_ModelArray15";
	wstrComTag[16] = L"Com_ModelArray16";
	wstrComTag[17] = L"Com_ModelArray17";
	wstrComTag[18] = L"Com_ModelArray18";
	wstrComTag[19] = L"Com_ModelArray19";
	wstrComTag[20] = L"Com_ModelArray20";
	wstrComTag[21] = L"Com_ModelArray21";
	wstrComTag[22] = L"Com_ModelArray22";
	wstrComTag[23] = L"Com_ModelArray23";
	wstrComTag[24] = L"Com_ModelArray24";
	wstrComTag[25] = L"Com_ModelArray25";
	wstrComTag[26] = L"Com_ModelArray26";
	wstrComTag[27] = L"Com_ModelArray27";
	wstrComTag[28] = L"Com_ModelArray28";
	wstrComTag[29] = L"Com_ModelArray29";
	wstrComTag[30] = L"Com_ModelArray30";
	wstrComTag[31] = L"Com_ModelArray31";


	for (int i = 0; i < 32; ++i)
	{
		if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, wstrModelTag[i], wstrComTag[i], (CComponent**)&m_pModelArray[i])))
			return E_FAIL;
	}

	CModel::MODELDESC ModelDesc;
	ModelDesc.iModelIndexNum = iModelIndex;

	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Tulip", L"Com_Model", (CComponent**)&m_pModel, &ModelDesc)))
		return E_FAIL;

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

	m_pModelArray[m_iModelIndex]->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(m_pTransform->Get_WorldMatrix() * XMLoadFloat4x4(&m_TransformMatrix)), sizeof(_matrix));
	m_pModelArray[m_iModelIndex]->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pModelArray[m_iModelIndex]->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_OrthMatrix)), sizeof(_matrix));

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

	for(int i = 0; i < 32; ++i)
	{
		Safe_Release(m_pModelArray[i]);
	}

	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRenderer);

}
