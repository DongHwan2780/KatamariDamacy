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
		_vector		vPosition;
		_float3		vScale;
		_float		fScale;
		_float		fSpeedPerSec;
		_float		fRotatePerSec;
		_float		fMySize;
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
	_vector Get_State(STATE eState) { return XMLoadFloat4((_float4*)&m_MatWorld.m[(_uint)eState][0]); }
	_float Get_Scale(STATE eState) { return XMVectorGetX(XMVector3Length(Get_State(eState))); }
	_matrix Get_WorldMatrixInverse() const { return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_MatWorld)); }
	_matrix Get_WorldMatrix() const { return XMLoadFloat4x4(&m_MatWorld); }

	_matrix Get_WorldMatrixNoConst() { return XMLoadFloat4x4(&m_MatWorld); }

	void Set_State(STATE eState, _fvector vData) { memcpy(&m_MatWorld.m[eState][0], &vData, sizeof(_float3)); } // � ����(eState)�� � ��(vData)�� ��������
	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc) { m_TransformDesc = TransformDesc; }
	void Set_TransformDescSpeed(_float fixSpeed) { m_TransformDesc.fSpeedPerSec = fixSpeed; }

	void Set_Scale(_fvector vScale);

	void Set_PosX(STATE eState, _float PosX) { memcpy(&m_MatWorld.m[eState][0], &PosX, sizeof(_float)); }
	void Set_PosY(STATE eState, _float PosY) { memcpy(&m_MatWorld.m[eState][1], &PosY, sizeof(_float)); }
	void Set_PosZ(STATE eState, _float PosZ) { memcpy(&m_MatWorld.m[eState][2], &PosZ, sizeof(_float)); }

	void Set_WorldMatrix(_fmatrix WorldMatrix) { XMStoreFloat4x4(&m_MatWorld, WorldMatrix); };

	const _vector Get_State_Const(STATE eState) const { return *(_vector*)&m_MatWorld.m[eState][0]; }
	const _float Get_Scale_Const(STATE eState) const { return XMVectorGetX(XMVector3Length(Get_State_Const(eState))); }
	const TRANSFORMDESC& GetTransformDesc() const { return m_TransformDesc;	}
	//_matrix Get_RotatinMatrix() const { return RotationMatrix; }

public:
	void Move_Straight(_double DeltaTime, class CNavigation* pNavigation = nullptr);
	void Move_Strafe(_double DeltaTime, class CNavigation* pNavigation = nullptr);
	void Chase_Target(CTransform* pTarget);
	void Chase_Target(_fvector vTargetPos, _double DeltaTime);
	void LookAtLandObj(_fvector vTargetPos);
	void LookAt(_fvector vTargetPos);
	void SetUp_Rotate(_fvector vAxis, _float fDegree);
	void RotateAxis(_fvector vAxis, _double DeltaTime);

	void MoveToDir(_vector Look, _float fSpeed, _double DeltaTime);

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float4x4		m_MatWorld;
	TRANSFORMDESC	m_TransformDesc;

	//_matrix			RotationMatrix;
};

END
#endif // !__TRANSFORM_H__
