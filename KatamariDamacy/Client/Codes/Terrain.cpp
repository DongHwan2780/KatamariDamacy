#include "stdafx.h"
#include "..\Headers\Terrain.h"

#include "Management.h"

CTerrain::CTerrain(DEVICES)
	:CObj(pDevice, pDeviceContext)
{
}

CTerrain::CTerrain(const CTerrain & other)
	:CObj(other)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	//LoadData(pArg);

	return S_OK;
}

_int CTerrain::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	return _uint();
}

_int CTerrain::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return m_pRenderer->Add_RenderGroup(CRenderer::NONALPHA, this);
}

HRESULT CTerrain::Render()
{

#ifdef _DEBUG
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pVIBuffer->Render(0);
#endif // !#ifdef _DEBUG

	return S_OK;
}

HRESULT CTerrain::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Transform*/
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, TEXT("Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, TEXT("Component_VIBuffer_Terrain"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(m_pTransform->Get_WorldMatrix()), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(pManagement->Get_Transform(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(pManagement->Get_Transform(CPipeLine::D3DTS_PROJ)), sizeof(_matrix));

	RELEASE_INSTANCE(CManagement);


	return S_OK;
}

void CTerrain::LoadData(void * pArg)
{
	HANDLE hFile = CreateFile(L"../../MFCTool/Data/Object/TerrainMap.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	if (m_pVIBuffer != nullptr)
	{
		Safe_Release(m_pVIBuffer);
	}

	DWORD dwByte = 0;
	_float3 pVertexPos;

	int i = 0;

	while (1)
	{
		ReadFile(hFile, &pVertexPos, sizeof(_float3), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		m_pVIBuffer->Set_TerrainPosY(i, pVertexPos.y);
		++i;
	}
	CloseHandle(hFile);
}

CTerrain * CTerrain::Create(DEVICES)
{
	CTerrain*		pInstance = new CTerrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CTerrain::Clone(void * pArg)
{
	CTerrain*		pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
}
