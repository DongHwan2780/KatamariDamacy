#pragma once

#ifndef __HIERARCHYNODE_H__
#define __HIERARCHYNODE_H__

#include "Base.h"

// aiNode의 정보를 가지고 있는 클래스 == aiNode계층 구조의 하나의 데이터를 의미함
// aiScene에 있는 RootNode를 최상위 부모로 하는 클래스

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
	_float4x4			m_CombinedTransformationMatrix; /* 부모뼈들의 상태행렬을 누적시켜만든행렬. */
	CHierarchyNode*		m_pParent = nullptr;
	_uint				m_iDepth = 0;

private:
	/* 현재 노드가 가리키고 있는 뼈는 여러개의 애니메이션에서 채널로서 사용이 된다. */
	/* 각각의 애니메이션에서 사용되고 있는 채널들을 애니메이션 배열로 저장한다. */
	vector<class CChannel*>			m_Channels;
};


END
#endif // !__HIERARCHYNODE_H__
