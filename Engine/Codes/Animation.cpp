#include "..\Headers\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize_Clone(const char * pAnimationName, _double Duration, _double TickPerSecond)
{
	strcpy_s(m_AnimationName, pAnimationName);
	m_Duration = Duration;
	m_TickPerSecond = TickPerSecond;

	return S_OK;
}

HRESULT CAnimation::Add_Channel(CChannel * pChannel)
{
	if (nullptr == pChannel)
		return E_FAIL;

	m_Channels.push_back(pChannel);

	return S_OK;
}

HRESULT CAnimation::Update_TransformationMatrices(_double DeltaTime)
{
	m_CurrentTime += fmod(m_TickPerSecond * DeltaTime, m_Duration);		// ���� �ִϸ��̼��� ����ð�

	if (m_CurrentTime > m_Duration) // ��ü �ִϸ��̼� ��� �ð����� ���� �ð��� Ŀ����
	{
		m_CurrentTime = m_CurrentTime - m_Duration;//fmod(m_TickPerSecond * DeltaTime, m_Duration);		// ���� �ð� �ʱ�ȭ
		m_bFinished = true;
	}
	else
		m_bFinished = false;

	_vector		vSourScale, vDestScale;
	_vector		vSourRotation, vDestRotation;
	_vector		vSourPosition, vDestPosition;

	for (auto& pAnimationChannel : m_Channels)		// �ִϸ��̼��� ��� ���¿� ���� ��ĵ��� �������ֱ� ���� �ִϸ��̼ǿ� ������ �ִ� ���� ��ȸ
	{
		vector<KEYFRAMEDESC*>		KeyFrames = pAnimationChannel->Get_KeyFrames();

		KEYFRAMEDESC*		pFirst = KeyFrames.front(); 
		KEYFRAMEDESC*		pLast = KeyFrames.back();

		_vector			vScale, vRotation, vPosition;
		vScale = XMVectorZero();
		vRotation = XMVectorZero();
		vPosition = XMVectorZero();


		_uint		iCurrentKeyFrame = 0;
		_float		fRatio = 0.f;		// ������Ű ���� ������ ������������ ��


		if (true == m_bFinished)
		{
			iCurrentKeyFrame = 0;
			pAnimationChannel->Set_CurrentKeyFrame(iCurrentKeyFrame);
		}


		if (m_CurrentTime <= pFirst->Time)		// ���� �ִϸ��̼� ��� �ð��� ù��° ������Ű���� ������
		{
			vScale = XMLoadFloat3(&pFirst->vScale);
			vRotation = XMLoadFloat4(&pFirst->vRotation);
			vPosition = XMLoadFloat3(&pFirst->vPosition);
			vPosition = XMVectorSetW(vPosition, 1.f);		// ù��° ������Ű�� ��İ����� ����
		}

		else if (m_CurrentTime > pLast->Time)
		{
			vScale = XMLoadFloat3(&pLast->vScale);
			vRotation = XMLoadFloat4(&pLast->vRotation);
			vPosition = XMLoadFloat3(&pLast->vPosition);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}

		else
		{
			if (m_CurrentTime > KeyFrames[iCurrentKeyFrame + 1]->Time)
				pAnimationChannel->Set_CurrentKeyFrame(++iCurrentKeyFrame);


			fRatio = (m_CurrentTime - KeyFrames[iCurrentKeyFrame]->Time) /
				(KeyFrames[iCurrentKeyFrame + 1]->Time - KeyFrames[iCurrentKeyFrame]->Time);

			vSourScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vScale);
			vSourRotation = XMLoadFloat4(&KeyFrames[iCurrentKeyFrame]->vRotation);
			vSourPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vPosition);
			vSourPosition = XMVectorSetW(vSourPosition, 1.f);

			vDestScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame + 1]->vScale);
			vDestRotation = XMLoadFloat4(&KeyFrames[iCurrentKeyFrame + 1]->vRotation);
			vDestPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame + 1]->vPosition);
			vDestPosition = XMVectorSetW(vDestPosition, 1.f);

			vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);		// XMVectorLerp -> �������� �Լ�
			vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
			vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}

		_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		pAnimationChannel->Set_TransformationMatrix(TransformationMatrix);

	}

	return S_OK;
}

CAnimation * CAnimation::Create(const char * pAnimationName, _double Duration, _double TickPerSecond)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize_Clone(pAnimationName, Duration, TickPerSecond)))
	{
		MSG_BOX("Failed to Creating CHierarchyNode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);
	m_Channels.clear();
}
