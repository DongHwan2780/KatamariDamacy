#include "..\Headers\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iVertexCountX, _uint IVertexCountY, _float fVertexInterval)
	: CVIBuffer(pDevice, pDeviceContext)
	, m_iVertexCountX(iVertexCountX)
	, m_iVertexCountZ(IVertexCountY)
	, m_fVertexInterval(fVertexInterval)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & other)
	: CVIBuffer(other)
	, m_iVertexCountX(other.m_iVertexCountX)
	, m_iVertexCountZ(other.m_iVertexCountZ)
	, m_fVertexInterval(other.m_fVertexInterval)
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar * pShaderFilePath)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

#pragma region VERTEXBUFFER

	m_iStride = sizeof(VTXINFO);		// 버텍스 사이즈
	m_iNumVertices = m_iVertexCountX * m_iVertexCountZ;		// 버텍스 갯수
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

	_uint iIndex = 0;

	for (_uint z = 0; z < m_iVertexCountZ; ++z) /*행*/
	{
		for (_uint x = 0; x < m_iVertexCountX; ++x) /*열*/
		{
			iIndex = z * m_iVertexCountX + x;

			if (m_iNumVertices <= iIndex)
				continue;

			((VTXINFO*)m_pVertices)[iIndex].vPos = _float3(x * m_fVertexInterval, 0.f, z * m_fVertexInterval);
		}
	}

	/* For.D3D11_SUBRESOURCE_DATA */
	m_VBSubResourceData.pSysMem = m_pVertices;

#pragma endregion VERTEXBUFFER

#pragma region INDEXBUFFER

	m_iNumPrimitive = (m_iVertexCountX - 1) * (m_iVertexCountZ - 1) * 2;
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

	iIndex = 0;
	_uint iTriIndex = 0;

	for (_uint z = 0; z < m_iVertexCountZ - 1; ++z) /*행*/
	{
		for (_uint x = 0; x < m_iVertexCountX - 1; ++x) /*열*/
		{
			iIndex = z * m_iVertexCountX + x;

			if (m_iNumVertices <= iIndex)
				continue;

			pIndices[iTriIndex]._0 = iIndex + m_iVertexCountX;
			pIndices[iTriIndex]._1 = iIndex + m_iVertexCountX + 1;
			pIndices[iTriIndex++]._2 = iIndex + 1;

			pIndices[iTriIndex]._0 = iIndex + m_iVertexCountX;
			pIndices[iTriIndex]._1 = iIndex + 1;
			pIndices[iTriIndex++]._2 = iIndex;
		}
	}

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

HRESULT CVIBuffer_Terrain::Initialize_Clone(void * pArg)
{
	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pShaderFilePath, _uint iVertexCountX, _uint IVertexCountY, _float fVertexInterval)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pDevice, pDeviceContext, iVertexCountX, IVertexCountY, fVertexInterval);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}
