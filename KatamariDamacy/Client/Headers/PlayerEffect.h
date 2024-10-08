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

class CPlayerEffect final : public CObj
{
public:
	explicit CPlayerEffect(DEVICES);
	explicit CPlayerEffect(const CPlayerEffect& other);
	virtual ~CPlayerEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void * pArg) override;
	virtual _int Update(_double DeltaTime) override;
	virtual _int Late_Update(_double DeltaTime) override;
	virtual HRESULT Render() override;

public:
	static CPlayerEffect*	Create(DEVICES);
	virtual CObj * Clone(void * pArg) override;
	virtual void Free() override;

public:
	void MoveDir(_double DeltaTime);

private:
	HRESULT SetUp_Components();

private:
	CTexture*	m_pTexture = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CTransform*	m_pTransform = nullptr;
	CVIBuffer_PointInstance* m_pVIBuffer = nullptr;

private:
	_float		m_fFrame = 0.f;

	_float4x4			m_OrthMatrix;
	_float4x4			m_TransformMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_double				m_iIdx = 0;
};
END
