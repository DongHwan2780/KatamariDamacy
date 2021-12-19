#pragma once

#ifndef __STICKYUI_H__
#define __STICKYUI_H__


#include "ClientDefines.h"
#include "UIList.h"

BEGIN(Client)

class CStickyUI final : public CUIList
{
public:
	explicit CStickyUI(DEVICES);
	explicit CStickyUI(const CStickyUI& other);
	virtual ~CStickyUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;


private:
	HRESULT SetUp_ComponentsOrthUI();

public:
	static CStickyUI*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;
};
END
#endif // !__STICKYUI_H__
