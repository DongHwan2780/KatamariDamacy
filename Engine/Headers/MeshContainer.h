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
	static CMeshContainer*	Create();
	virtual void Free() override;

private:
	_uint				m_iNumVertices;
	_uint				m_iStartVertexIndex;
};

END
#endif // !__MESHCONTAINER_H__
