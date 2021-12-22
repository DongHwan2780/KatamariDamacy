#include "..\Headers\VIBuffer_Triangle.h"

CVIBuffer_Triangle::CVIBuffer_Triangle(DEVICES)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Triangle::CVIBuffer_Triangle(const CVIBuffer_Triangle & other)
	: CVIBuffer(other)
{
}

HRESULT CVIBuffer_Triangle::Initialize_Prototype(_fvector* pPoints, const _tchar * pShaderFilePath)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

#pragma region VERTEXBUFFER

	m_iStride = sizeof(VTXCOL);		// 버텍스 사이즈
	m_iNumVertices = 3;		// 버텍스 갯수
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

	XMStoreFloat3(&((VTXCOL*)m_pVertices)[0].vPosition, pPoints[0]);
	((VTXCOL*)m_pVertices)[0].vColor = _float4(1.f, 1.f, 1.f, 1.f);

	XMStoreFloat3(&((VTXCOL*)m_pVertices)[1].vPosition, pPoints[1]);
	((VTXCOL*)m_pVertices)[1].vColor = _float4(1.f, 1.f, 1.f, 1.f);

	XMStoreFloat3(&((VTXCOL*)m_pVertices)[2].vPosition, pPoints[2]);
	((VTXCOL*)m_pVertices)[2].vColor = _float4(1.f, 1.f, 1.f, 1.f);

	/* For.D3D11_SUBRESOURCE_DATA */
	m_VBSubResourceData.pSysMem = m_pVertices;

#pragma endregion VERTEXBUFFER

#pragma region INDEXBUFFER

	m_iNumPrimitive = 3;
	m_iNumVerticesPerPrimitive = 2;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	/* For.D3D11_BUFFER_DESC */
	m_IBDesc.ByteWidth = sizeof(LINEINDICES16) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	LINEINDICES16*		pIndices = new LINEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(LINEINDICES16) * m_iNumPrimitive);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;

	pIndices[1]._0 = 1;
	pIndices[1]._1 = 2;

	pIndices[2]._0 = 2;
	pIndices[2]._1 = 0;

	m_IBSubResourceData.pSysMem = pIndices;

#pragma endregion INDEXBUFFER

	if (FAILED(__super::Create_Buffers()))
		return E_FAIL;

	/* 현재 정점이 어떤 멤버들을 가지고 있어! */
	D3D11_INPUT_ELEMENT_DESC		ElmentDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(__super::Compile_Shader(ElmentDesc, 2, pShaderFilePath)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Triangle::Initialize_Clone(void * pArg)
{
	return S_OK;
}

CVIBuffer_Triangle * CVIBuffer_Triangle::Create(DEVICES, _fvector* pPoints, const _tchar * pShaderFilePath)
{
	CVIBuffer_Triangle*	pInstance = new CVIBuffer_Triangle(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pPoints, pShaderFilePath)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Triangle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Triangle::Clone(void * pArg)
{
	CVIBuffer_Triangle*	pInstance = new CVIBuffer_Triangle(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Triangle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Triangle::Free()
{
	__super::Free();
}
