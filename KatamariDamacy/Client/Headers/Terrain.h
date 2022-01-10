#pragma once

#include "ClientDefines.h"
#include "Obj.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Engine)

class CTerrain final : public CObj
{
public:
	explicit CTerrain(DEVICES);
	explicit CTerrain(const CTerrain& other);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual _int	Update(_double DeltaTime)override;
	virtual _int	Late_Update(_double DeltaTime)override;
	virtual HRESULT	Render()override;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	void LoadData(void* pArg = nullptr);

public:
	static CTerrain* Create(DEVICES);
	virtual CObj* Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CVIBuffer_Terrain* m_pVIBuffer = nullptr;
};

END