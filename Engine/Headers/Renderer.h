#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDER {PRIORITY, NONALPHA, ALPHA, UI, RENDER_END};

protected:
	explicit CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CRenderer(const CRenderer& other);
	virtual ~CRenderer() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void* pArg);

public:
	HRESULT Add_RenderGroup(RENDER eRenderID, class CObj* pRenderObj);
	HRESULT Draw_RenderGroup();

private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();


public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	list<class CObj*>				m_RenderGroups[RENDER_END];
	typedef list<class CObj*>		RENDERGROUPS;
};

END
#endif // !__RENDERER_H__