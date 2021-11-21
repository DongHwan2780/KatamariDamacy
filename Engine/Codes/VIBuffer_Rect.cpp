#include "..\Headers\VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & other)
	: CVIBuffer(other)
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype(const _tchar * pShaderFilePath)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

#pragma region VERTEXBUFFER

	m_iStride = sizeof(VTXINFO);		// 버텍스 사이즈
	m_iNumVertices = 4;		// 버텍스 갯수
	m_iNumVertexBuffers = 1;		// 버퍼개수

									/* For.D3D11_BUFFER_DESC */
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXINFO[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXINFO) * m_iNumVertices);

	((VTXINFO*)m_pVertices)[0].vPos = _float3(-0.5f, 0.5f, 0.f);
	((VTXINFO*)m_pVertices)[0].vTexUV = _float2(0.f, 0.f);

	((VTXINFO*)m_pVertices)[1].vPos = _float3(0.5f, 0.5f, 0.f);
	((VTXINFO*)m_pVertices)[1].vTexUV = _float2(1.f, 0.f);

	((VTXINFO*)m_pVertices)[2].vPos = _float3(0.5f, -0.5f, 0.f);
	((VTXINFO*)m_pVertices)[2].vTexUV = _float2(1.f, 1.f);

	((VTXINFO*)m_pVertices)[3].vPos = _float3(-0.5f, -0.5f, 0.f);
	((VTXINFO*)m_pVertices)[3].vTexUV = _float2(0.f, 1.f);

	/* For.D3D11_SUBRESOURCE_DATA */
	m_VBSubResourceData.pSysMem = m_pVertices;

#pragma endregion VERTEXBUFFER

#pragma region INDEXBUFFER

	m_iNumPrimitive = 2;
	m_iNumVerticesPerPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/* For.D3D11_BUFFER_DESC */
	m_IBDesc.ByteWidth = sizeof(POLYGONINDICES16) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	POLYGONINDICES16*		pIndices = new POLYGONINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(POLYGONINDICES16) * m_iNumPrimitive);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_IBSubResourceData.pSysMem = pIndices;

#pragma endregion INDEXBUFFER

	if (FAILED(__super::Create_Buffers()))
		return E_FAIL;

	/* 현재 정점이 어떤 멤버들을 가지고 있어! */
	D3D11_INPUT_ELEMENT_DESC		ElmentDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(__super::Compile_Shader(ElmentDesc, 2, pShaderFilePath)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize_Clone(void * pArg)
{
	return S_OK;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pShaderFilePath)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
}
