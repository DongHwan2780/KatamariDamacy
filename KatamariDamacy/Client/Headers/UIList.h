#pragma once

#ifndef __UILIST_H__
#define __UILIST_H__

#include "ClientDefines.h"
#include "Obj.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CModel;
END

BEGIN(Client)

class CUIList abstract : public CObj
{
public:
	explicit CUIList(DEVICES);
	explicit CUIList(const CUIList& other);
	virtual ~CUIList() = default;

public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize_Clone(void* pArg) PURE;
	virtual _int	Update(_double DeltaTime) PURE;
	virtual _int	Late_Update(_double DeltaTime) PURE;
	virtual HRESULT	Render() PURE;

public:
	void Set_LimitTime(_double MinTime) { m_dLimitTime -= MinTime; }
	_double Get_LimitTime() { return m_dLimitTime; }

public:
	virtual CObj*	Clone(void* pArg) PURE;
	virtual void Free() PURE;

protected:
	CTexture*			m_pTexture = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	CVIBuffer_Rect*		m_pVIBuffer = nullptr;


protected:			// 직교투영에서 쓸 변수들
	_float4x4			m_OrthMatrix;
	_float4x4			m_TransformMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_double				m_iIdx = 0;

protected:
	_double				m_dNowTime = 0.0;
	_double				m_dLimitTime = 0.0;
};
END
#endif // !__UILIST_H__
