#pragma once

#include "Camera.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
class CTransform;
class CModel;
END

class CToolCamera final : public CCamera
{
private:
	explicit CToolCamera(DEVICES);
	explicit CToolCamera(const CToolCamera& other);
	virtual ~CToolCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;

public:
	static CToolCamera* Create(DEVICES);
	virtual CObj * Clone(void * pArg) override;
	virtual void Free() override;


private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CModel*			m_pModel = nullptr;
	CCollider*		m_pCollider = nullptr;
};

