#pragma once

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { DDS, TGA, WIC, TYPE_END};


protected:
	explicit CTexture(ID3D11Device* pDevice, ID3D11DeviceContext*	pDeviceContext);
	explicit CTexture(const CTexture& other);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const _tchar* pTexturePath, _uint iNumTextures);
	virtual HRESULT Initialize_Clone(void * pArg);

public:
	ID3D11ShaderResourceView* Get_ShaderResourceView(_uint iTextureIndex = 0);

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext*	pDeviceContext, TYPE eType, const _tchar* pTexturePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	vector<ID3D11ShaderResourceView*>			m_Textures;
	typedef vector<ID3D11ShaderResourceView*>	TEXTURES;

private:
	_uint		m_iTextureNum = 0;
};

END

#endif // !__TEXTURE_H__
