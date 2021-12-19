#pragma once

#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "Base.h"
/* 특정애니메이션에 소속되어있는 뼈. */
BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize_Clone(const char* pChannelName);
	HRESULT Add_KeyFrameDesc(KEYFRAMEDESC* pKeyFrameDesc);

public:
	vector<KEYFRAMEDESC*> Get_KeyFrames() {return m_KeyFrame;}

	const char* Get_Name() const {return m_szChannelName;}

	_matrix Get_TransformationMatrix() {return XMLoadFloat4x4(&m_TransformationMatrix);}

	_uint Get_CurrentKeyFrame() const { return m_iCurrentKeyFrame; }

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {	XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);}
	void Set_CurrentKeyFrame(_uint iCurrentKeyFrame) { m_iCurrentKeyFrame = iCurrentKeyFrame; }


public:
	static CChannel* Create(const char* pChannelName);
	virtual void Free() override;

private:
	char		m_szChannelName[MAX_PATH] = "";
	vector<KEYFRAMEDESC*>		m_KeyFrame;		/* 현재 뼈가 각각의 키프레임 동작때마다 표현해야할 상태정보를 보관해둔다. */
	_float4x4					m_TransformationMatrix;
	_uint						m_iCurrentKeyFrame = 0;
};
END
#endif // !__CHANNEL_H__
