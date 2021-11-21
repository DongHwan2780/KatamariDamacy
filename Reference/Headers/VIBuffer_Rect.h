#pragma once

#ifndef __VIBUFFER_RECT_H__
#define __VIBUFFER_RECT_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private:
	explicit CVIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_Rect(const CVIBuffer_Rect& other);
	virtual ~CVIBuffer_Rect() = default;
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT Initialize_Clone(void * pArg) override;

public:
	static CVIBuffer_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_uint m_iVertexCountX = 0;
	_uint m_iVertexCountZ = 0;
	_float m_fVertexInterval = 1.f;
};

END

#endif // !__VIBUFFER_RECT_H__
