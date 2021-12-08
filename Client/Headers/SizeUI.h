#pragma once

#ifndef __SIZEUI_H__
#define __SIZEUI_H__

#include "ClientDefines.h"
#include "UIList.h"

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

public:
	static CSizeUI*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

};
END
#endif // !__SIZEUI_H__

