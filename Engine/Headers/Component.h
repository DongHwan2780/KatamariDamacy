#pragma once

#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CComponent(const CComponent& other);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void * pArg);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
	ID3D11Device*		 m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

};

END
#endif // !__COMPONENT_H__
