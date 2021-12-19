#include "..\Headers\Channel.h"



CChannel::CChannel()
{
}

HRESULT CChannel::Initialize_Clone(const char * pChannelName)
{
	strcpy_s(m_szChannelName, pChannelName);

	return S_OK;
}

HRESULT CChannel::Add_KeyFrameDesc(KEYFRAMEDESC * pKeyFrameDesc)
{
	if (nullptr == pKeyFrameDesc)
		return E_FAIL;

	m_KeyFrame.push_back(pKeyFrameDesc);

	return S_OK;
}

CChannel * CChannel::Create(const char * pChannelName)
{
	CChannel*		pInstance = new CChannel();

	if (FAILED(pInstance->Initialize_Clone(pChannelName)))
	{
		MSG_BOX("Failed to Creating CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	for (auto& pKeyFrame : m_KeyFrame)
		Safe_Delete(pKeyFrame);

	m_KeyFrame.clear();
}
