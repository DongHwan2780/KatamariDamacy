#pragma once

#include "ObjList.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
class CTransform;
class CModel;
END
BEGIN(Client)
END
class CDrinkCan0 final : public CObjList
{
private:
	explicit CDrinkCan0(DEVICES);
	explicit CDrinkCan0(const CDrinkCan0& other);
	virtual ~CDrinkCan0() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

public:
	static CDrinkCan0*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CModel*			m_pModel = nullptr;
	CCollider*		m_pCollider = nullptr;
};

