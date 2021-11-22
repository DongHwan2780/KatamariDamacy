#include "..\Headers\VIBuffer.h"
#include "Texture.h"


CVIBuffer::CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & other)
	: CComponent(other)
	, m_pVB(other.m_pVB)
	, m_pIB(other.m_pIB)
	, m_VBDesc(other.m_VBDesc)
	, m_IBDesc(other.m_IBDesc)
	, m_VBSubResourceData(other.m_VBSubResourceData)
	, m_IBSubResourceData(other.m_IBSubResourceData)
	, m_iStride(other.m_iStride)
	, m_iNumVertices(other.m_iNumVertices)
	, m_iNumPrimitive(other.m_iNumPrimitive)
	, m_pVertices(other.m_pVertices)
	, m_EffectDescs(other.m_EffectDescs)
	, m_pEffect(other.m_pEffect)
	, m_iNumVerticesPerPrimitive(other.m_iNumVerticesPerPrimitive)
	, m_iNumVertexBuffers(other.m_iNumVertexBuffers)
	, m_ePrimitive(other.m_ePrimitive)
	, m_eIndexFormat(other.m_eIndexFormat)
{
	for (auto& EffectDesc : m_EffectDescs)
	{
		Safe_AddRef(EffectDesc.pPass);
		Safe_AddRef(EffectDesc.pLayOut);
	}

	Safe_AddRef(m_pEffect);
	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	return S_OK;
}

HRESULT CVIBuffer::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Render(_uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	_uint		iOffset = 0;

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, &m_pVB, &m_iStride, &iOffset);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitive);
	m_pDeviceContext->IASetInputLayout(m_EffectDescs[iPassIndex].pLayOut);

	if (FAILED(m_EffectDescs[iPassIndex].pPass->Apply(0, m_pDeviceContext)))
		return E_FAIL;

	m_pDeviceContext->DrawIndexed(m_iNumPrimitive * m_iNumVerticesPerPrimitive, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::SetUp_ValueOnShader(const char * pConstantName, void * pData, _uint iByteSize)	// 쉐이더 전역변수에 값을 전달해주기 위한 함수
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	if (FAILED(pVariable->SetRawValue(pData, 0, iByteSize)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::SetUp_TextureOnShader(const char * pConstantName, CTexture * pTextureComponent, _uint iTextureIndex)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName)->AsShaderResource();
	if (nullptr == pVariable)
		return E_FAIL;

	ID3D11ShaderResourceView*		pShaderResourceView = pTextureComponent->Get_ShaderResourceView(iTextureIndex);
	if (nullptr == pShaderResourceView)
		return E_FAIL;

	if (FAILED(pVariable->SetResource(pShaderResourceView)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Create_Buffers()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubResourceData, &m_pVB)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_IBDesc, &m_IBSubResourceData, &m_pIB)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Compile_Shader(D3D11_INPUT_ELEMENT_DESC * pElementDescs, _uint iNumElement, const _tchar * pShaderFilePath, _uint iTechniqueIndex)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	ID3DBlob*		pCompileShader = nullptr;
	ID3DBlob*		pCompileShaderErrorMessage = nullptr;

	// 외부에서 가져온 쉐이더 파일을 번역하여 바이너리화하였고 메모리영역안에 담았다.
	if (FAILED(D3DCompileFromFile(pShaderFilePath, nullptr, nullptr, nullptr, "fx_5_0", iFlag, 0, &pCompileShader, &pCompileShaderErrorMessage)))
		return E_FAIL;

	if (FAILED(D3DX11CreateEffectFromMemory(pCompileShader->GetBufferPointer(), pCompileShader->GetBufferSize(), 0, m_pDevice, &m_pEffect)))
		return E_FAIL;

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByIndex(iTechniqueIndex);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;

	m_EffectDescs.reserve(TechniqueDesc.Passes);

	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		EFFECTDESC			EffectDesc;

		EffectDesc.pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == EffectDesc.pPass)
			return E_FAIL;

		D3DX11_PASS_DESC			PassDesc;

		if (FAILED(EffectDesc.pPass->GetDesc(&PassDesc)))
			return E_FAIL;

		if (FAILED(m_pDevice->CreateInputLayout(pElementDescs, iNumElement, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &EffectDesc.pLayOut)))
			return E_FAIL;

		m_EffectDescs.push_back(EffectDesc);
	}

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVertices);

	for (auto& EffectDesc : m_EffectDescs)
	{
		Safe_Release(EffectDesc.pPass);
		Safe_Release(EffectDesc.pLayOut);
	}
	m_EffectDescs.clear();

	Safe_Release(m_pEffect);
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}
