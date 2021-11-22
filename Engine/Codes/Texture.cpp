#include "..\Headers\Texture.h"


CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CTexture::CTexture(const CTexture & other)
	: CComponent(other)
	, m_Textures(other.m_Textures)
	, m_iTextureNum(other.m_iTextureNum)
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::Initialize_Prototype(TYPE eType, const _tchar * pTexturePath, _uint iNumTextures)
{
	_tchar			szFullPath[MAX_PATH] = TEXT("");

	m_iTextureNum = iNumTextures;

	m_Textures.reserve(iNumTextures);

	for (_uint i = 0; i < m_iTextureNum; ++i)
	{
		wsprintf(szFullPath, pTexturePath, i);

		DirectX::ScratchImage			ScratchImage;

		HRESULT			hr = 0;

		switch (eType)
		{
		case DDS:
			hr = DirectX::LoadFromDDSFile(szFullPath, DirectX::DDS_FLAGS_NONE, nullptr, ScratchImage);
			break;
		case TGA:
			hr = DirectX::LoadFromTGAFile(szFullPath, nullptr, ScratchImage);
			break;
		case WIC:
			hr = DirectX::LoadFromWICFile(szFullPath, DirectX::WIC_FLAGS_NONE, nullptr, ScratchImage);
			break;
		}

		if (FAILED(hr))
			return E_FAIL;

		ID3D11Resource*				pTexture = nullptr;
		if (FAILED(DirectX::CreateTexture(m_pDevice, ScratchImage.GetImages(), ScratchImage.GetImageCount(), ScratchImage.GetMetadata(), &pTexture)))
			return E_FAIL;

		ID3D11ShaderResourceView*		pShaderResourceView = nullptr;
		if (FAILED(m_pDevice->CreateShaderResourceView(pTexture, nullptr, &pShaderResourceView)))
			return E_FAIL;

		m_Textures.push_back(pShaderResourceView);
	}


	return S_OK;
}

HRESULT CTexture::Initialize_Clone(void * pArg)
{
	return S_OK;
}

ID3D11ShaderResourceView * CTexture::Get_ShaderResourceView(_uint iTextureIndex)
{
	if (iTextureIndex >= m_iTextureNum)
		return nullptr;

	return m_Textures[iTextureIndex];
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, TYPE eType, const _tchar * pTexturePath, _uint iNumTextures)
{
	CTexture*	pInstance = new CTexture(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pTexturePath, iNumTextures)))
	{
		MSG_BOX("Failed To Creating CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture*	pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed To Creating CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();
}
