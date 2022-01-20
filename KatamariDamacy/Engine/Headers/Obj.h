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
	HRESULT SetUp_Components(_int iSceneIndex, const wstring& pPrototypeTag, const wstring& pComponentTag,  class CComponent** pOut, void* pArg = nullptr);

public:
	CComponent*	GetComponent(const wstring& pComponentTag) const;

	CObj*		GetThis() { return this; }
	virtual void SetDead() { m_bDead = true; }


	_float Get_ObjCollSize() { return m_fCollSize; }
	void Add_PlayerBallSize(_float fBallSize) { m_fBallSize += fBallSize; }
public:
	virtual CObj*	Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	//PxTransform				m_pxTransform = PxTransform(PxVec3(), PxQuat());

protected:
	unordered_map<wstring, class CComponent*>			m_Components;
	typedef unordered_map<wstring, class CComponent*>	COMPONENTS;

	_bool	m_bDead = false;
	_float	m_fCollSize = 0.f;

	_float	m_fBallSize = 100.f;
};

END
#endif // !__OBJ_H__
