#pragma once

#include "Camera.h"
#include "Management.h"
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
	void Movement(_double DeltaTime);
	_uint Get_VertexIndex() { return m_iVertexIndex; }
	_float3 Get_VertexPos() { return vOut; }
public:
	static CToolCamera* Create(DEVICES);
	virtual CObj * Clone(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer_Terrain*		m_pVIBuffer = nullptr;
	_uint m_iVertexIndex = 0;
	_float3 vOut;
};

