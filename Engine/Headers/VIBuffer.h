#pragma once

#ifndef __VIBUFFER_H__
#define __VIBUFFER_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
public:
	typedef struct tagEffectDesc
	{
		ID3DX11EffectPass*		pPass = nullptr;
		ID3D11InputLayout*		pLayOut = nullptr;
	}EFFECTDESC;

protected:
	explicit CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer(const CVIBuffer& other);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void * pArg);
	virtual HRESULT Render(_uint iPassIndex);
public:
	HRESULT SetUp_ValueOnShader(const char* pConstantName, void* pData, _uint iByteSize);

protected:
	HRESULT Create_Buffers();
	HRESULT Compile_Shader(D3D11_INPUT_ELEMENT_DESC* pElementDescs, _uint iNumElement, const _tchar* pShaderFilePath, _uint iTechniqueIndex = 0);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;

protected: //버텍스 버퍼 정보
	ID3D11Buffer*				m_pVB = nullptr;
	D3D11_BUFFER_DESC			m_VBDesc;
	D3D11_SUBRESOURCE_DATA		m_VBSubResourceData;
	_uint						m_iStride; // 버텍스사이즈
	_uint						m_iNumVertices = 0;	// 버텍스 개수
	_uint						m_iNumVertexBuffers = 0;		// 버퍼개수


protected: // 인덱스 버퍼 정보
	ID3D11Buffer*				m_pIB = nullptr;
	D3D11_BUFFER_DESC			m_IBDesc;
	D3D11_SUBRESOURCE_DATA		m_IBSubResourceData;
	_uint						m_iNumPrimitive = 0;		// 만들 삼각형 개수
	DXGI_FORMAT					m_eIndexFormat;
	D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitive;
	_uint						m_iNumVerticesPerPrimitive = 0;

protected:
	vector<EFFECTDESC>			m_EffectDescs;
	ID3DX11Effect*				m_pEffect = nullptr;

protected:
	void*				m_pVertices = nullptr;
};
END
#endif // !__VIBUFFER_H__
