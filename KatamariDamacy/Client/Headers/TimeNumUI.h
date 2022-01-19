#pragma once


#ifndef __TIMENUMUI_H__
#define __TIMENUMUI_H__

#include "ClientDefines.h"
#include "UIList.h"

BEGIN(Client)

class CTimeNumUI final : public CUIList
{
public:
	explicit CTimeNumUI(DEVICES);
	explicit CTimeNumUI(const CTimeNumUI& other);
	virtual ~CTimeNumUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;

private:
	void Calculate_Time(_double DeltaTime);


private:
	HRESULT SetUp_ComponentsOrthUI();

public:
	static CTimeNumUI*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float4x4			m_TransformMatrixTwo;

	_uint				m_iTextureIndexFirst = 0;
	_uint				m_iTextureIndexSecond = 0;

	_double				m_dNowTime = 0.0;
};

END
#endif // !__TIMENUMUI_H__