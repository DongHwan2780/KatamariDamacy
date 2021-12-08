#pragma once

#include "ClientDefines.h"
#include "Obj.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CModel;
END

BEGIN(Client)

class CPlayerMoveUI final : public CObj
{
public:
	explicit CPlayerMoveUI(DEVICES);
	explicit CPlayerMoveUI(const CPlayerMoveUI& other);
	virtual ~CPlayerMoveUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;

private:
	HRESULT SetUp_Components();

public:
	static CPlayerMoveUI*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CModel*			m_pModel = nulltpr;
};
END
