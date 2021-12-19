#pragma once


#ifndef __TIMEUI_H__
#define __TIMEUI_H__

#include "ClientDefines.h"
#include "UIList.h"

BEGIN(Client)

class CTimeUI final : public CUIList
{
public:
	explicit CTimeUI(DEVICES);
	explicit CTimeUI(const CTimeUI& other);
	virtual ~CTimeUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;

public:
	static CTimeUI*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;
};

END
#endif // !__TIMEUI_H__