#pragma once

#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE {RIGHT, UP, LOOK, POSITION, STATE_END};

public:
	typedef struct tagTransformDesc
	{
		_float fSpeedPerSec;
		_float fRotatePerSec;
	}TRANSFORMDESC;

private:
	explicit CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTransform(const CTransform& other);
	virtual ~CTransform() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void * pArg);
	//{ return *(_vector*)&m_MatWorld.m[eState][0]; }
	//{ return *(static_cast<_vector>(&m_MatWorld).m[eState][0]); }
public:
	_vector Get_State(STATE eState) { return *(_vector*)&m_MatWorld.m[eState][0]; }
	_float Get_Scale(STATE eState) { return XMVectorGetX(XMVector3Length(Get_State(eState))); }
	_matrix Get_WorldMatrixInverse() const { return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_MatWorld)); }
	_matrix Get_WorldMatrix() const { return XMLoadFloat4x4(&m_MatWorld); }

	void Set_State(STATE eState, _fvector vData) { memcpy(&m_MatWorld.m[eState][0], &vData, sizeof(_float3)); } // 어떤 벡터(eState)에 어떤 값(vData)를 넣을건지
	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc) { m_TransformDesc = TransformDesc; }

public:
	void Move_Straight(_double DeltaTime);
	void Move_Strafe(_double DeltaTime);
	void Chase_Target(CTransform* pTarget);
	void Chase_Target(_fvector vTargetPos, _double DeltaTime);
	void LookAtLandObj(_fvector vTargetPos);
	void LookAt(_fvector vTargetPos);
	void SetUp_Rotate(_fvector vAxis, _float fDegree);
	void RotateAxis(_fvector vAxis, _double DeltaTime);

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float4x4		m_MatWorld;
	TRANSFORMDESC	m_TransformDesc;
};

END
#endif // !__TRANSFORM_H__
