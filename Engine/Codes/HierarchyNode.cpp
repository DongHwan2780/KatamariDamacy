#include "..\Headers\HierarchyNode.h"



CHierarchyNode::CHierarchyNode()
{
}

HRESULT CHierarchyNode::Initialize_Clone(const char * pNodeName, _fmatrix TransformationMatrix, CHierarchyNode * pParent, _uint iDepth)
{
	strcpy_s(m_szNodeName, pNodeName);
	XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	m_pParent = pParent;
	m_iDepth = iDepth;

	return S_OK;
}

CHierarchyNode * CHierarchyNode::Create(const char * pNodeName, _fmatrix TransformationMatrix, CHierarchyNode * pParent, _uint iDepth)
{
	CHierarchyNode*		pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize_Clone(pNodeName, TransformationMatrix, pParent, iDepth)))
	{
		MSG_BOX("Failed to Creating CHierarchyNode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHierarchyNode::Free()
{
}

