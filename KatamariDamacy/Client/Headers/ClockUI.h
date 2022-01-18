#pragma once


#ifndef __CLOCKUI_H__
#define __CLOCKUI_H__


#include "ClientDefines.h"
#include "UIList.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CClockUI final : public CUIList
{
public:
	explicit CClockUI(DEVICES);
	explicit CClockUI(const CClockUI& other);
	virtual ~CClockUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT	Render() override;


private:
	HRESULT SetUp_ComponentsOrthUI();

public:
	static CClockUI*	Create(DEVICES);
	virtual CObj*	Clone(void* pArg) override;
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
};

END
#endif // !__CLOCKUI_H__