#pragma once

#ifndef __OBJ_H__
#define __OBJ_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CObj abstract : public CBase 
{
protected:
	explicit CObj(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CObj(const CObj& other);
	virtual ~CObj() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual _int	Update(_double DeltaTime);
	virtual _int	Late_Update(_double DeltaTime);
	virtual HRESULT	Render();

public:
	virtual CObj*	Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
};

END
#endif // !__OBJ_H__
