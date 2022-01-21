#include "..\Headers\VIBuffer_PointInstance.h"
#include "PipeLine.h"

CVIBuffer_PointInstance::CVIBuffer_PointInstance(DEVICES)
	: CVIBuffer(pDevice, pDeviceContext)	
{

}

CVIBuffer_PointInstance::CVIBuffer_PointInstance(const CVIBuffer_PointInstance & other)
	: CVIBuffer(other)
	, m_VBInstanceDesc(other.m_VBInstanceDesc)
	, m_VBInstanceSubResourceData(other.m_VBInstanceSubResourceData)
	, m_pVBInstance(other.m_pVBInstance)
	, m_iNumInstance(other.m_iNumInstance)
	, m_InstanceMatrices(other.m_InstanceMatrices)
{
	Safe_AddRef(m_pVBInstance);

}

HRESULT CVIBuffer_PointInstance::Initialize_Prototype(const _tchar* pShaderFilePath, _uint iNumInstance)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;	

	m_iNumInstance = iNumInstance;
	m_iNumVertexBuffers = 2;

#pragma region VERTEXBUFFER

	m_iStride = sizeof(VTXPOINT);
	m_iNumVertices = m_iNumInstance;	
	
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXPOINT[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXPOINT) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{ 
		((VTXPOINT*)m_pVertices)[i].vPosition = _float3(0.0f, 0.0f, 0.f);
		((VTXPOINT*)m_pVertices)[i].vSize = _float2(1.0f, 1.0f);
	}

	/* For.D3D11_SUBRESOURCE_DATA */
	m_VBSubResourceData.pSysMem = m_pVertices;

#pragma endregion VERTEXBUFFER

	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubResourceData, &m_pVB)))
		return E_FAIL;

#pragma region VERTEXINSTANCEBUFFER

	ZeroMemory(&m_VBInstanceDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBInstanceDesc.ByteWidth = m_iNumInstance * sizeof(VTXINSTANCE);
	m_VBInstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstanceDesc.MiscFlags = 0;
	m_VBInstanceDesc.StructureByteStride = sizeof(VTXINSTANCE);

	VTXINSTANCE*			pInstanceVertices = new VTXINSTANCE[m_iNumInstance];


	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pInstanceVertices[i].vPosition = _float4(550.f, -280.f, 0.7f, 1.f);
		m_InstanceMatrices.push_back(pInstanceVertices[i]);
	}
	m_VBInstanceSubResourceData.pSysMem = pInstanceVertices;	

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstanceDesc, &m_VBInstanceSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVertices);

#pragma endregion 
	
	D3D11_INPUT_ELEMENT_DESC		ElmentDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "PSIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	if (FAILED(__super::Compile_Shader(ElmentDesc, 6, pShaderFilePath)))
		return E_FAIL;
	

	return S_OK;
}



HRESULT CVIBuffer_PointInstance::Initialize_Clone(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_PointInstance::Update(_double TimeDelta, _float4 vPos)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	if (FAILED(m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource)))
		return E_FAIL;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{

		m_InstanceMatrices[i].vPosition = vPos;

		((VTXINSTANCE*)SubResource.pData)[i] = m_InstanceMatrices[i];
	}	

	m_pDeviceContext->Unmap(m_pVBInstance, 0);


	return S_OK;
}

HRESULT CVIBuffer_PointInstance::Render(_uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	ID3D11Buffer*	pVBBuffers[] = {
		m_pVB, 		
		m_pVBInstance
	};

	_uint	iStrides[] = {
		sizeof(VTXPOINT),
		sizeof(VTXINSTANCE)
	};

	_uint		iOffset[] = {
		0, 0
	};

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVBBuffers, iStrides, iOffset);	
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitive);
	m_pDeviceContext->IASetInputLayout(m_EffectDescs[iPassIndex].pLayOut);

	if (FAILED(m_EffectDescs[iPassIndex].pPass->Apply(0, m_pDeviceContext)))
		return E_FAIL;

	/* 0 : 인스턴스하나당 인덱스버퍼의 인덱슬르 몇개 활용하여 그리는지? */
	m_pDeviceContext->DrawInstanced(1, m_iNumInstance, 0, 0);	

	return S_OK;
}

CVIBuffer_PointInstance * CVIBuffer_PointInstance::Create(DEVICES, const _tchar* pShaderFilePath, _uint iNumInstance)
{
	CVIBuffer_PointInstance*	pInstance = new CVIBuffer_PointInstance(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, iNumInstance)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_PointInstance::Clone(void * pArg)
{
	CVIBuffer_PointInstance*	pInstance = new CVIBuffer_PointInstance(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_PointInstance::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);
}
