#include "..\Headers\VIBuffer_Model.h"

CVIBuffer_Model::CVIBuffer_Model(DEVICES)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Model::CVIBuffer_Model(const CVIBuffer_Model & other)
	: CVIBuffer(other)
{
}

HRESULT CVIBuffer_Model::Initialize_Prototype(const _tchar * pShaderFilePath)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

#pragma region VERTEXBUFFER

	D3D11_BUFFER_DESC			BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

	BufferDesc.ByteWidth = sizeof(VTXMESH) * m_iNumVertices;
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = sizeof(VTXMESH);

	D3D11_SUBRESOURCE_DATA		SubResourceData;
	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	SubResourceData.pSysMem = m_pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &SubResourceData, &m_pVB)))
		return E_FAIL;

#pragma endregion VERTEXBUFFER

#pragma region INDEXBUFFER

	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

	BufferDesc.ByteWidth = sizeof(POLYGONINDICES32) * m_iNumFaces;
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubResourceData.pSysMem = m_pPolygonIndices;

	if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &SubResourceData, &m_pIB)))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC		ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(Compile_Shader(ElementDesc, 4, pShaderFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Model::Initialize_Clone(void * pArg)
{
	return S_OK;
}

CVIBuffer_Model * CVIBuffer_Model::Create(DEVICES, const _tchar * pShaderFilePath)
{
	CVIBuffer_Model*	pInstance = new CVIBuffer_Model(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Model");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Model::Clone(void * pArg)
{
	CVIBuffer_Model*	pInstance = new CVIBuffer_Model(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Model");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Model::Free()
{
	__super::Free();
}
