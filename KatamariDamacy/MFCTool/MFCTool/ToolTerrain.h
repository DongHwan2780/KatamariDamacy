#pragma once

#include "Obj.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
END

class CToolTerrain final : public CObj
{
private:
	explicit CToolTerrain(DEVICES);
	explicit CToolTerrain(const CToolTerrain& other);
	virtual ~CToolTerrain() = default;

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
	static CToolTerrain*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CVIBuffer_Terrain* m_pVIBuffer = nullptr;
};

