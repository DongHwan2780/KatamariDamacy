#pragma once

#include "Obj.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
class CTransform;
END
BEGIN(Client)
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

	HRESULT Ready_Layer_CollEffect(const wstring& pLayerTag, _fvector vPos);

public:
	static CDummy*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CCollider*		m_pCollider = nullptr;

	CCollider*		pTargetCollider = nullptr;
};

END
