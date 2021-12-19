#pragma once

#ifndef __VIBUFFER_UI_H__
#define __VIBUFFER_UI_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_UI final : public CVIBuffer
{
private:
	explicit CVIBuffer_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_UI(const CVIBuffer_UI& other);
	virtual ~CVIBuffer_UI() = default;
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT Initialize_Clone(void * pArg) override;

public:
	static CVIBuffer_UI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END

#endif // !__VIBUFFER_UI_H__
