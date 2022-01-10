#pragma once

#include "Obj.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
class CTransform;
END

class CDummy final : public CObj
{
private:
	explicit CDummy(DEVICES);
	explicit CDummy(const CDummy& other);
	virtual ~CDummy() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;

private:
	HRESULT SetUp_Components(_float3 vScale);
	HRESULT SetUp_ConstantTable();

public:
	static CDummy*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CCollider*		m_pOBBCom = nullptr;
};

