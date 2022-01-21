#pragma once

#include "ClientDefines.h"
#include "Obj.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_PointInstance;
END

BEGIN(Client)

class CCollEffect final : public CObj
{
public:
	explicit CCollEffect(DEVICES);
	explicit CCollEffect(const CCollEffect& other);
	virtual ~CCollEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void * pArg) override;
	virtual _int Update(_double DeltaTime) override;
	virtual _int Late_Update(_double DeltaTime) override;
	virtual HRESULT Render() override;

public:
	static CCollEffect*	Create(DEVICES);
	virtual CObj * Clone(void * pArg) override;
	virtual void Free() override;

private:
	HRESULT SetUp_Components();

private:
	CTexture*	m_pTexture = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CTransform*	m_pTransform = nullptr;
	CVIBuffer_PointInstance* m_pVIBuffer = nullptr;

private:
	_float		m_fFrame = 0.f;
};
END
