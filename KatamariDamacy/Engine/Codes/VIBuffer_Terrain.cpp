#include "..\Headers\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(DEVICES, _uint iVertexCountX, _uint iVertexCountZ, _float fVertexInterval)
	: CVIBuffer(pDevice, pDeviceContext)
	, m_iVertexCountX(iVertexCountX)
	, m_iVertexCountZ(iVertexCountZ)
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

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pShaderFilePath)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	//HANDLE		hFile = CreateFile(pHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if (0 == hFile)
	//	return E_FAIL;

	//_ulong					dwByte = 0;

	//BITMAPFILEHEADER		fh;
	//ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

	//BITMAPINFOHEADER		ih;
	//ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	//_ulong*		pPixel = new _ulong[ih.biWidth * ih.biHeight];		// 픽셀을 비트맵사이즈의 x * y만큼 만들어줌
	//ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	//CloseHandle(hFile);

#pragma region VERTEXBUFFER

	m_iStride = sizeof(VTXINFO);		// 버텍스 사이즈
	//m_iVertexCountX = ih.biWidth;			// 가로 개수 == 비트맵 가로 크기
//	m_iVertexCountZ = ih.biHeight;			// 세로 개수 == 비트맵 세로 크기
	m_iNumVertices = m_iVertexCountX * m_iVertexCountZ;		// 버텍스 갯수
	m_iNumVertexBuffers = 1;		// 버퍼개수

	/* For.D3D11_BUFFER_DESC */
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXINFO[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXINFO) * m_iNumVertices);

	for (_uint i = 0; i < m_iVertexCountZ; ++i)
	{
		for (_uint j = 0; j < m_iVertexCountX; ++j)
		{
			_uint		iIndex = i * m_iVertexCountX + j;

			((VTXINFO*)m_pVertices)[iIndex].vPos = _float3(j * m_fVertexInterval, 0.f, i * m_fVertexInterval);
			((VTXINFO*)m_pVertices)[iIndex].vTexUV = _float2(j / (m_iVertexCountX - 1.f), i / (m_iVertexCountZ - 1.f));
		}
	}

	/* For.D3D11_SUBRESOURCE_DATA */
	m_VBSubResourceData.pSysMem = m_pVertices;

#pragma endregion VERTEXBUFFER

#pragma region INDEXBUFFER

	m_iNumPrimitive = (m_iVertexCountX - 1) * (m_iVertexCountZ - 1) * 2;
	m_iNumVerticesPerPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/* For.D3D11_BUFFER_DESC */
	m_IBDesc.ByteWidth = sizeof(POLYGONINDICES32) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	m_pIndices = new POLYGONINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(POLYGONINDICES32) * m_iNumPrimitive);

	_uint		iNumPrimitive = 0;

	for (_uint i = 0; i < m_iVertexCountZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iVertexCountX - 1; ++j)
		{
			_uint		iIndex = i * m_iVertexCountX + j;

			if (m_iNumVertices <= iIndex)
				continue;

			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive]._0 = iIndex + m_iVertexCountX;
			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive]._1 = iIndex + m_iVertexCountX + 1;
			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive++]._2 = iIndex + 1;

			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive]._0 = iIndex + m_iVertexCountX;
			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive]._1 = iIndex + 1;
			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive++]._2 = iIndex;
		}
	}

	m_IBSubResourceData.pSysMem = m_pIndices;

#pragma endregion INDEXBUFFER

	if (FAILED(__super::Create_Buffers()))
		return E_FAIL;

//	Safe_Delete_Array(pPixel);

	/* 현재 정점이 어떤 멤버들을 가지고 있어! */
	D3D11_INPUT_ELEMENT_DESC		ElmentDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(__super::Compile_Shader(ElmentDesc, 2, pShaderFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize_Clone(void * pArg)
{
	return S_OK;
}

void CVIBuffer_Terrain::Set_TerrainPosY(_uint _iIndex, _float _fPosY)
{
	D3D11_MAPPED_SUBRESOURCE mapresource;

	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapresource);

	((VTXINFO*)mapresource.pData)[_iIndex].vPos.y = _fPosY;

	memcpy(m_pVertices, mapresource.pData, m_iStride * m_iNumVertices);

	m_pDeviceContext->Unmap(m_pVB, 0);
}

HRESULT CVIBuffer_Terrain::ReadyWithSave(const _tchar * pShaderFilePath, const HANDLE & hFile)
{
	m_iStride = sizeof(VTXINFO);		// 버텍스 사이즈
	m_iNumVertices = m_iVertexCountX * m_iVertexCountZ;		// 버텍스 갯수
	m_iNumVertexBuffers = 1;		// 버퍼개수

									/* For.D3D11_BUFFER_DESC */
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXINFO[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXINFO) * m_iNumVertices);

	DWORD dwByte = 0;

	for (_uint i = 0; i < m_iVertexCountZ; ++i)
	{
		for (_uint j = 0; j < m_iVertexCountX; ++j)
		{
			_uint		iIndex = i * m_iVertexCountX + j;

			if (m_iNumVertices <= iIndex)
				continue;

			_float3 pVertexPos;
			ReadFile(hFile, &pVertexPos, sizeof(_float3), &dwByte, nullptr);

			((VTXINFO*)m_pVertices)[iIndex].vPos = _float3(j * m_fVertexInterval, pVertexPos.y, i * m_fVertexInterval);
			((VTXINFO*)m_pVertices)[iIndex].vTexUV = _float2(j / (m_iVertexCountX - 1.f), i / (m_iVertexCountZ - 1.f));
		}
	}

	/* For.D3D11_SUBRESOURCE_DATA */
	m_VBSubResourceData.pSysMem = m_pVertices;

#pragma endregion VERTEXBUFFER

#pragma region INDEXBUFFER

	m_iNumPrimitive = (m_iVertexCountX - 1) * (m_iVertexCountZ - 1) * 2;
	m_iNumVerticesPerPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/* For.D3D11_BUFFER_DESC */
	m_IBDesc.ByteWidth = sizeof(POLYGONINDICES32) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	m_pIndices = new POLYGONINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(POLYGONINDICES32) * m_iNumPrimitive);

	_uint		iNumPrimitive = 0;

	for (_uint i = 0; i < m_iVertexCountZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iVertexCountX - 1; ++j)
		{
			_uint		iIndex = i * m_iVertexCountX + j;

			if (m_iNumVertices <= iIndex)
				continue;

			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive]._0 = iIndex + m_iVertexCountX;
			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive]._1 = iIndex + m_iVertexCountX + 1;
			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive++]._2 = iIndex + 1;

			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive]._0 = iIndex + m_iVertexCountX;
			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive]._1 = iIndex + 1;
			((POLYGONINDICES32*)m_pIndices)[iNumPrimitive++]._2 = iIndex;
		}
	}

	m_IBSubResourceData.pSysMem = m_pIndices;

#pragma endregion INDEXBUFFER

	if (FAILED(__super::Create_Buffers()))
		return E_FAIL;

	//	Safe_Delete_Array(pPixel);

	/* 현재 정점이 어떤 멤버들을 가지고 있어! */
	D3D11_INPUT_ELEMENT_DESC		ElmentDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(__super::Compile_Shader(ElmentDesc, 2, pShaderFilePath)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(DEVICES,  const _tchar* pShaderFilePath, _uint iVertexCountX, _uint iVertexCountZ, _float fVertexInterval)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pDevice, pDeviceContext, iVertexCountX, iVertexCountZ, fVertexInterval);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::CreateLoadData(DEVICES, const _tchar * pShaderFilePath, const HANDLE & hFile, _uint iVertexCountX, _uint iVertexCountZ, _float fVertexInterval)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pDevice, pDeviceContext, iVertexCountX, iVertexCountZ, fVertexInterval);

	if (FAILED(pInstance->ReadyWithSave(pShaderFilePath, hFile)))
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
