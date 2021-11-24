#pragma once

#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "Camera.h"
#include "Base.h"

BEGIN(Engine)

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)
public:
	enum TYPE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

public:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	_matrix Get_Transform(TYPE eType) {		// Ÿ�Կ� ���� ��� ��ȯ
		return eType == D3DTS_VIEW ? XMLoadFloat4x4(&m_ViewMatrix) : XMLoadFloat4x4(&m_ProjMatrix);		// ���׿���
	}

	_vector Get_CamPosition() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_ViewMatrix)).r[3];		// .r[] == �ε����� ���� �࿡ ���� ���� -> .r[3] == ������ ��
	}

	void Set_Transform(TYPE eType, _fmatrix StateMatrix) {		// Ÿ�Կ� ���� ��ļ���
		if (eType == D3DTS_VIEW)
			XMStoreFloat4x4(&m_ViewMatrix, StateMatrix);
		else
			XMStoreFloat4x4(&m_ProjMatrix, StateMatrix);
	}

public:
	virtual void Free() override;

private:
	_float4x4		m_ViewMatrix;
	_float4x4		m_ProjMatrix;
};
END
#endif // !__PIPELINE_H__
