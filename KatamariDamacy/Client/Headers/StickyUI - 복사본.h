#pragma once

#ifndef __STICKOBJUI_H__
#define __STICKOBJUI_H__


#include "ClientDefines.h"
#include "Obj.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CModel;
END


BEGIN(Client)

class CStickObjUI final : public CObj
{
public:
	explicit CStickObjUI(DEVICES);
	explicit CStickObjUI(const CStickObjUI& other);
	virtual ~CStickObjUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;


public:
	HRESULT CheckModel(_uint iModelIndex);

private:
	HRESULT SetUp_ComponentsOrthUI(_uint iModelIndex);
	HRESULT SetUp_ConstantTable();

public:
	static CStickObjUI*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CModel*			m_pModel = nullptr;

private:
	_float4x4			m_OrthMatrix;
	_float4x4			m_TransformMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	_uint				m_iModelIndex;
};
END
#endif // !__STICKOBJUI_H__
