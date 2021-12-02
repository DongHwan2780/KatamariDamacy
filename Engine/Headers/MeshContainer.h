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
	virtual HRESULT Initialize_Clone(const char* pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex);

public:
	static CMeshContainer* Create(const char* pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex);
	virtual void Free() override;

private:
	char				m_szName[MAX_PATH] = "";
	_uint				m_iNumFaces = 0;
	_uint				m_iStartFaceIndex = 0;
	_uint				m_iStartVertexIndex = 0;
};

END
#endif // !__MESHCONTAINER_H__
