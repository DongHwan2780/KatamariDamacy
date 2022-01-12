#pragma once

#ifndef __VIBUFFER_TERRAIN_H__
#define __VIBUFFER_TERRAIN_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	explicit CVIBuffer_Terrain(DEVICES, _uint iVertexCountX, _uint iVertexCountZ, _float fVertexInterval = 1.f);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& other);
	virtual ~CVIBuffer_Terrain() = default;
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT Initialize_Clone(void * pArg) override;

public:
	void Set_TerrainPosY(_uint _iIndex, _float _fPosY);
	virtual HRESULT ReadyWithSave(const _tchar * pShaderFilePath, const HANDLE& hFile);

	_bool IsGround(_float3& inOut) const;

public:
	static CVIBuffer_Terrain* Create(DEVICES, const _tchar* pShaderFilePath, _uint iVertexCountX, _uint iVertexCountZ, _float fVertexInterval = 1.f);
	static CVIBuffer_Terrain* CreateLoadData(DEVICES, const _tchar* pShaderFilePath, const HANDLE& hFile, _uint iVertexCountX, _uint iVertexCountZ, _float fVertexInterval = 1.f);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_uint m_iVertexCountX = 0;
	_uint m_iVertexCountZ = 0;
	_float m_fVertexInterval = 1.f;

	//VTXNORMALINFO* pVertices = nullptr;
};

END

#endif // !__VIBUFFER_TERRAIN_H__
