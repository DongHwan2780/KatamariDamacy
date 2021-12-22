#pragma once

#ifndef __VIBUFFER_TRIANGLE_H__
#define __VIBUFFER_TRIANGLE_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Triangle final : public CVIBuffer
{
private:
	explicit CVIBuffer_Triangle(DEVICES);
	explicit CVIBuffer_Triangle(const CVIBuffer_Triangle& other);
	virtual ~CVIBuffer_Triangle() = default;
public:
	virtual HRESULT Initialize_Prototype(_fvector* pPoints, const _tchar* pShaderFilePath);
	virtual HRESULT Initialize_Clone(void * pArg) override;

public:
	static CVIBuffer_Triangle* Create(DEVICES, _fvector* pPoints, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END

#endif // !__VIBUFFER_TRIANGLE_H__
