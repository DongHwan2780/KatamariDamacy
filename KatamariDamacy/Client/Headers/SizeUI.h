#pragma once

#ifndef __SIZEUI_H__
#define __SIZEUI_H__

#include "ClientDefines.h"
#include "UIList.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CSizeUI final : public CUIList
{
public:
	explicit CSizeUI(DEVICES);
	explicit CSizeUI(const CSizeUI& other);
	virtual ~CSizeUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;

private:
	HRESULT SetUp_ComponentsOrthUI();

private:
	void Calculate_Size(_double DeltaTime);

public:
	static CSizeUI*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float4x4			m_TransformMatrixCM;
	_float4x4			m_TransformMatrixMM;

	_float4x4			m_TransformMatrixTensCM;
	_float4x4			m_TransformMatrixOnesCM;
	_float4x4			m_TransformMatrixNumMM;
	
	_uint				m_iIndexTensCM = 0;
	_uint				m_iIndexOnesCM = 0;

	_uint				m_iIndexOnesMM = 0;


	CTexture*			m_pTextureCM = nullptr;
	CTexture*			m_pTextureMM = nullptr;
	CTexture*			m_pTextureNum = nullptr;
	CTransform*			m_pTransform = nullptr;

private:
	CObj*				m_pPlayerBall = nullptr;
};
END
#endif // !__SIZEUI_H__

