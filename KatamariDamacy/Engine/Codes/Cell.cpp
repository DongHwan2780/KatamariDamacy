#include "..\Headers\Cell.h"

#include "VIBuffer_Triangle.h"
#include "PipeLine.h"

CCell::CCell(DEVICES)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	ZeroMemory(m_pNeighbor, sizeof(CCell*) * LINE_END);
}

HRESULT CCell::Initialize_Clone(_fvector * pPoints, _uint iIndex)
{
	for (_uint i = 0; i < POINT_END; ++i)
		XMStoreFloat3(&m_vPoints[i], pPoints[i]);

	XMStoreFloat3(&m_vLines[LINE_AB], pPoints[POINT_B] - pPoints[POINT_A]);
	XMStoreFloat3(&m_vLines[LINE_BC], pPoints[POINT_C] - pPoints[POINT_B]);
	XMStoreFloat3(&m_vLines[LINE_CA], pPoints[POINT_A] - pPoints[POINT_C]);

	XMStoreFloat3(&m_vNormal[LINE_AB], XMVector3Normalize(XMVectorSet(m_vLines[LINE_AB].z * -1.f, m_vLines[LINE_AB].y, m_vLines[LINE_AB].x, 1.f)));
	XMStoreFloat3(&m_vNormal[LINE_BC], XMVector3Normalize(XMVectorSet(m_vLines[LINE_BC].z * -1.f, m_vLines[LINE_BC].y, m_vLines[LINE_BC].x, 1.f)));
	XMStoreFloat3(&m_vNormal[LINE_CA], XMVector3Normalize(XMVectorSet(m_vLines[LINE_CA].z * -1.f, m_vLines[LINE_CA].y, m_vLines[LINE_CA].x, 1.f)));

	m_iIndex = iIndex;

#ifdef _DEBUG
	if (FAILED(Ready_VIBuffer(pPoints)))
		return E_FAIL;
#endif

	return S_OK;
}

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
	}

	return false;
}

_bool CCell::isIn(_fvector vPoint, CCell ** ppNeighbor)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector		vSour = XMVector3Normalize(vPoint - XMLoadFloat3(&m_vPoints[i]));
		_vector		vDest = XMLoadFloat3(&m_vNormal[i]);

		if (0 < XMVectorGetX(XMVector3Dot(vSour, vDest)))
		{
			*ppNeighbor = m_pNeighbor[i];
			return false;
		}
	}

	return true;
}
#ifdef _DEBUG
HRESULT CCell::Ready_VIBuffer(_fvector * pPoints)
{
	m_pVIBuffer = CVIBuffer_Triangle::Create(m_pDevice, m_pDeviceContext, pPoints, TEXT("../Bin/ShaderFiles/Shader_TriangleDBG.fx"));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	return S_OK;
}

HRESULT CCell::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ)), sizeof(_matrix));

	m_pVIBuffer->Render(0);

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}
#endif
CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _fvector * pPoints, _uint iIndex)
{
	CCell*		pInstance = new CCell(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Clone(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Creating CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
