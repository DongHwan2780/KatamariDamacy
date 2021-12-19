#pragma once

#ifndef __VIBUFFER_MODEL_H__
#define __VIBUFFER_MODEL_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Model final : public CVIBuffer
{
private:
	explicit CVIBuffer_Model(DEVICES);
	explicit CVIBuffer_Model(const CVIBuffer_Model& other);
	virtual ~CVIBuffer_Model() = default;
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT Initialize_Clone(void * pArg) override;

public:
	static CVIBuffer_Model* Create(DEVICES, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END

#endif // !__VIBUFFER_MODEL_H__
