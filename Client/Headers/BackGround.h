#pragma once

#include "ClientDefines.h"
#include "Obj.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBackGround final : public CObj
{
public:
	explicit CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBackGround(const CBackGround& other);
	virtual ~CBackGround() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT SetUp_Components();

public:
	static CBackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CObj* Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTexture*			m_pTexture = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	CVIBuffer_Rect*		m_pVIBuffer = nullptr;


private:
	/* 직교투영으롬 ㅏㄴ든다. */
	_float4x4			m_ProjMatrix;
	_float4x4			m_TransformMatrix;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
};

END