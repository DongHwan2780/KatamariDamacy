#pragma once

#ifndef __HIERARCHYNODE_H__
#define __HIERARCHYNODE_H__

#include "Base.h"

// aiNode�� ������ ������ �ִ� Ŭ���� == aiNode���� ������ �ϳ��� �����͸� �ǹ���
// aiScene�� �ִ� RootNode�� �ֻ��� �θ�� �ϴ� Ŭ����

BEGIN(Engine)

class CHierarchyNode final : public CBase
{
public:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;

public:
	_uint Get_Depth() const { return m_iDepth; }
	const char* Get_Name() const { return m_szNodeName; }
	_matrix Get_CombinedTransformationMatrix() { return XMLoadFloat4x4(&m_CombinedTransformationMatrix); }

	_matrix Get_OffsetMatrix() { return XMLoadFloat4x4(&m_OffSetMatrix); }

	void Set_OffSetMatrix(_fmatrix OffSetMatrix) { XMStoreFloat4x4(&m_OffSetMatrix, OffSetMatrix); }

public:
	HRESULT Initialize_Clone(const char* pNodeName, _fmatrix TransformationMatrix, CHierarchyNode* pParent, _uint iDepth);
	HRESULT Add_Channel(_uint iAnimationIndex, class CChannel* pChannel);
	void Update_CombinedTransformationMatrix(_uint iAnimationIndex);
	void Resize_Channels(_uint iNumAnimation);

public:
	static CHierarchyNode* Create(const char* pNodeName, _fmatrix TransformationMatrix, CHierarchyNode* pParent, _uint iDepth);
	CHierarchyNode* Clone();
	virtual void Free() override;

private:
	char				m_szNodeName[MAX_PATH] = "";
	_float4x4			m_OffSetMatrix;
	_float4x4			m_TransformationMatrix;
	_float4x4			m_CombinedTransformationMatrix; /* �θ������ ��������� �������Ѹ������. */
	CHierarchyNode*		m_pParent = nullptr;
	_uint				m_iDepth = 0;

private:
	/* ���� ��尡 ����Ű�� �ִ� ���� �������� �ִϸ��̼ǿ��� ä�ημ� ����� �ȴ�. */
	/* ������ �ִϸ��̼ǿ��� ���ǰ� �ִ� ä�ε��� �ִϸ��̼� �迭�� �����Ѵ�. */
	vector<class CChannel*>			m_Channels;
};


END
#endif // !__HIERARCHYNODE_H__
