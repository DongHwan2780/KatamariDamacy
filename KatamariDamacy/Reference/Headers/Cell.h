#pragma once

#ifndef __CELL_H__
#define __CELL_H__

#include "Base.h"

BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };

private:
	explicit CCell(DEVICES);
	virtual ~CCell() = default;

public:
	_fvector Get_Point(POINT ePoint) { return XMLoadFloat3(&m_vPoints[ePoint]); }
	_uint Get_Index() const { return m_iIndex; }
	void Set_Neighbor(LINE eLine, CCell* pNeighbor) { m_pNeighbor[eLine] = pNeighbor; }

public:
	virtual HRESULT Initialize_Clone(_fvector* pPoints, _uint iIndex);
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);
	_bool isIn(_fvector vPoint, CCell** ppNeighbor);

#ifdef _DEBUG
public:
	HRESULT Ready_VIBuffer(_fvector * pPoints);
	HRESULT Render();
#endif

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _fvector* pPoints, _uint iIndex);
	virtual void Free() override;

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;
private:
	_float3			m_vPoints[POINT_END];
	_float3			m_vLines[LINE_END];
	_float3			m_vNormal[LINE_END];

	CCell*			m_pNeighbor[LINE_END];
	_uint			m_iIndex = 0;

#ifdef _DEBUG
private:
	class CVIBuffer_Triangle*	m_pVIBuffer = nullptr;
#endif

};
END
#endif // !__CELL_H__
