#pragma once

#ifndef __MESHCONTAINER_H__
#define __MESHCONTAINER_H__

#include "Base.h"

BEGIN(Engine)

class CMeshContainer final : public CBase
{
public:
	CMeshContainer();
	virtual ~CMeshContainer() = default;

public:
	_uint Get_MeshMaterialIndex() const { return m_iMaterialIndex; }
	_uint Get_NumFaces() const { return m_iNumFaces; }
	_uint Get_StartFaceIndex() const { return m_iStartFaceIndex; }
	_uint Get_StartVertexIndex() const { return m_iStartVertexIndex; }

	vector<BONEDESC*> Get_BoneDesc() { return m_Bones; }

	void Get_BoneMatrices(_matrix* pBoneMatrices);

public:
	virtual HRESULT Initialize_Clone(const char* pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex, _uint iMaterialIndex);

public:
	HRESULT Add_Bones(BONEDESC* pBoneDesc);
	HRESULT Clone_BoneDesc();

public:
	static CMeshContainer* Create(const char* pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex, _uint iMaterialIndex);
	CMeshContainer* Clone();
	virtual void Free() override;

private:
	char				m_szName[MAX_PATH] = "";
	_uint				m_iNumFaces = 0;
	_uint				m_iStartFaceIndex = 0;
	_uint				m_iStartVertexIndex = 0;
	_uint				m_iMaterialIndex = 0;

	/* 현재 메시컨테이너(정점들)는 어떤 뼈대들로부터 행렬을 적용받아야하는지. */
	vector<BONEDESC*>		m_Bones;
};

END
#endif // !__MESHCONTAINER_H__
