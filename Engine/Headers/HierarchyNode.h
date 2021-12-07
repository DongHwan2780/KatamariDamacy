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

public:
	HRESULT Initialize_Clone(const char* pNodeName, _fmatrix TransformationMatrix, CHierarchyNode* pParent, _uint iDepth);

public:
	static CHierarchyNode* Create(const char* pNodeName, _fmatrix TransformationMatrix, CHierarchyNode* pParent, _uint iDepth);
	virtual void Free() override;

private:
	char				m_szNodeName[MAX_PATH] = "";
	_float4x4			m_TransformationMatrix;
	CHierarchyNode*		m_pParent = nullptr;
	_uint				m_iDepth = 0;
};


END
#endif // !__HIERARCHYNODE_H__
