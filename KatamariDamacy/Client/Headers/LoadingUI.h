#pragma once

#include "ClientDefines.h"
#include "UIList.h"

BEGIN(Client)

class CLoadingUI final : public CUIList
{
public:
	explicit CLoadingUI(DEVICES);
	explicit CLoadingUI(const CLoadingUI& other);
	virtual ~CLoadingUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int	Update(_double DeltaTime) override;
	virtual _int	Late_Update(_double DeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT SetUp_Components();

public:
	static CLoadingUI* Create(DEVICES);
	virtual CObj* Clone(void* pArg) override;
	virtual void Free() override;
};

END