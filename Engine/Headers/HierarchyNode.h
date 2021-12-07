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
