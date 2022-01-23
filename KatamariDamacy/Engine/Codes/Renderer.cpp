#include "..\Headers\Renderer.h"
#include "Obj.h"


CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CRenderer::CRenderer(const CRenderer & other)
	: CComponent(other)
{
}

HRESULT CRenderer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDER eRenderID, CObj * pRenderObj)
{
	if (eRenderID >= RENDER_END || pRenderObj == nullptr)
		return E_FAIL;

	m_RenderGroups[eRenderID].push_back(pRenderObj);

	Safe_AddRef(pRenderObj);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonAlpha()))
		return E_FAIL;

	if (FAILED(Render_Alpha()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(Render_UI2()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pObj : m_RenderGroups[PRIORITY])
	{
		if (pObj != nullptr)
		{
			if (FAILED(pObj->Render()))
				return E_FAIL;

			Safe_Release(pObj);
		}
	}
	m_RenderGroups[PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonAlpha()
{
	for (auto& pObj : m_RenderGroups[NONALPHA])
	{
		if (pObj != nullptr)
		{
			if (FAILED(pObj->Render()))
				return E_FAIL;

			Safe_Release(pObj);
		}
	}
	m_RenderGroups[NONALPHA].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Alpha()
{
	for (auto& pObj : m_RenderGroups[ALPHA])
	{
		if (pObj != nullptr)
		{
			if (FAILED(pObj->Render()))
				return E_FAIL;

			Safe_Release(pObj);
		}
	}
	m_RenderGroups[ALPHA].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pObj : m_RenderGroups[UI])
	{
		if (pObj != nullptr)
		{
			if (FAILED(pObj->Render()))
				return E_FAIL;

			Safe_Release(pObj);
		}
	}
	m_RenderGroups[UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI2()
{
	for (auto& pObj : m_RenderGroups[UI2])
	{
		if (pObj != nullptr)
		{
			if (FAILED(pObj->Render()))
				return E_FAIL;

			Safe_Release(pObj);
		}
	}
	m_RenderGroups[UI2].clear();

	return S_OK;
}

CRenderer * CRenderer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CRenderer*	pInstance = new CRenderer(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Creating CRenderer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();
}
