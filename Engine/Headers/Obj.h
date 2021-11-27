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

protected:
	HRESULT SetUp_Components(_int iSceneIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag,  class CComponent** pOut, void* pArg = nullptr);

public:
	virtual CObj*	Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	PxTransform				m_pxTransform = PxTransform(PxVec3(), PxQuat());

protected:
	unordered_map<const _tchar*, class CComponent*>			m_Components;
	typedef unordered_map<const _tchar*, class CComponent*>	COMPONENTS;
};

END
#endif // !__OBJ_H__
