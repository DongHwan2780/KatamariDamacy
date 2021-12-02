#pragma once

#ifndef __MODEL_H__
#define __MODEL_H__

#include "Component.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CVIBuffer
{
private:
	explicit CModel(DEVICES);
	explicit CModel(const CModel& other);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Prototype(const char* pMeshFilePath, const char* pMeshFileName, const _tchar* pShaderFilePath);
	virtual HRESULT Initialize_Clone(void * pArg);
	HRESULT Render(_uint iPassIndex);


private:
	HRESULT Create_MeshContainer(aiMesh* pMesh, _uint* pStartVertexIndex, _uint* pStartFaceIndex);
	HRESULT Create_AllBuffer(const _tchar* pShaderFilePath);
	HRESULT Create_Materials(aiMaterial*	pMaterial, const char* pMeshFilePath);

public:
	static CModel*	Create(DEVICES, const char* pMeshFilePath, const char* pMeshFileName, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	const aiScene*		m_pScene = nullptr;		// ��� �𵨵��� �޽�, ���׸��� ������ ������ ���� ����
	Assimp::Importer	m_Importer;

private:
	_uint				m_iNumVertices = 0;		// ���� ����
	_uint				m_iNumFaces = 0;		// �ﰢ��(������)�� ����
	VTXMESH*			m_pVertices = nullptr;
	POLYGONINDICES32*	m_pPolygonIndices32 = nullptr;

	vector<class CMeshContainer*>		m_MeshContainers;
	vector<MODELTEXTURES*>				m_ModelTextures;


};
END
#endif // !__MODEL_H__
