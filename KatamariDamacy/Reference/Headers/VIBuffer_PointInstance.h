#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_PointInstance final : public CVIBuffer
{
private:
	explicit CVIBuffer_PointInstance(DEVICES);
	explicit CVIBuffer_PointInstance(const CVIBuffer_PointInstance& rhs);
	virtual ~CVIBuffer_PointInstance() = default;
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, _uint iNumInstance);
	virtual HRESULT Initialize_Clone(void* pArg) override;

	HRESULT Update(_double TimeDelta, _float4 vPos);
	HRESULT Render(_uint iPassIndex);

private:
	ID3D11Buffer*			m_pVBInstance = nullptr;
	D3D11_BUFFER_DESC		m_VBInstanceDesc;
	D3D11_SUBRESOURCE_DATA	m_VBInstanceSubResourceData;

	_uint			m_iNumInstance = 0;
	vector<VTXINSTANCE>		m_InstanceMatrices;

public:
	static CVIBuffer_PointInstance* Create(DEVICES, const _tchar* pShaderFilePath, _uint iNumInstance = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END