#pragma once

#ifndef __VIBUFFER_TERRAIN_H__
#define __VIBUFFER_TERRAIN_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	explicit CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iVertexCountX, _uint IVertexCountY, _float fVertexInterval = 1.f);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& other);
	virtual ~CVIBuffer_Terrain() = default;
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT Initialize_Clone(void * pArg) override;

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath, _uint iVertexCountX, _uint IVertexCountY, _float fVertexInterval = 1.f);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_uint m_iVertexCountX = 0;
	_uint m_iVertexCountZ = 0;
	_float m_fVertexInterval = 1.f;
};

END

#endif // !__VIBUFFER_TERRAIN_H__
