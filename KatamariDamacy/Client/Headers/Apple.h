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
class CApple final : public CObjList
{
private:
	explicit CApple(DEVICES);
	explicit CApple(const CApple& other);
	virtual ~CApple() = default;

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
	static CApple*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CRenderer*		m_pRenderer = nullptr;
	CModel*			m_pModel = nullptr;

	CCollider*		pTargetCollider = nullptr;
	CTransform*		m_pPlayerBallTransform = nullptr;
};

