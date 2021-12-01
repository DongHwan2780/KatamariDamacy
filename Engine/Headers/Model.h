#pragma once

#ifndef __MODEL_H__
#define __MODEL_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
private:
	explicit CModel(DEVICES);
	explicit CModel(const CModel& other);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Prototype(const char* pMeshFilePath, const char* pMeshFileName);
	virtual HRESULT Initialize_Clone(void * pArg);


private:
	HRESULT Create_MeshContainer(aiMesh* pMesh, _uint* pStartVertexIndex, _uint* pStartFaceIndex);

public:
	static CModel*	Create(DEVICES, const char* pMeshFilePath, const char* pMeshFileName);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	const aiScene*		m_pScene = nullptr;
	Assimp::Importer	m_Importer;

private:
	_uint				m_iNumVertices = 0;		// 정점 개수
	_uint				m_iNumFaces = 0;		// 삼각형(면)의 개수
	VTXMESH*			m_pVertices = nullptr;
	POLYGONINDICES32*	m_pPolygonIndices32 = nullptr;

	vector<class CMeshContainer*>		m_MeshContainers;


};
END
#endif // !__MODEL_H__
