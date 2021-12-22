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
	size_t	Get_NumMaterials() const { return (_uint)m_ModelTextures.size(); }
	_fmatrix Get_BoneMatrix(const char* pBoneName);

public:
	virtual HRESULT Initialize_Prototype(const char* pMeshFilePath, const char* pMeshFileName, const _tchar* pShaderFilePath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize_Clone(void * pArg);
	HRESULT Render(_uint iMaterialIndex, _uint iPassIndex);

public:
	HRESULT Bind_Buffers();
	HRESULT SetUp_ValueOnShader(const char* pConstantName, void* pData, _uint iByteSize);
	HRESULT SetUp_TextureOnShader(const char* pConstantName, _uint iMaterialIndex, aiTextureType eTextureType);
	HRESULT SetUp_AnimationIndex(_uint iAnimationIndex);
	HRESULT Play_Animation(_double DeltaTime);

private:
	HRESULT Create_MeshContainer(aiMesh* pMesh, _uint* pStartVertexIndex, _uint* pStartFaceIndex, _fmatrix PivotMatrix);
	HRESULT Create_AllBuffer(const _tchar* pShaderFilePath);
	HRESULT Create_Materials(aiMaterial*	pMaterial, const char* pMeshFilePath);
	HRESULT Create_HierarchyNodes(aiNode* pNode, class CHierarchyNode* pParent = nullptr, _uint iDepth = 0, _fmatrix PivotMatrix = XMMatrixIdentity());


	HRESULT Compile_Shader(D3D11_INPUT_ELEMENT_DESC* pElementDescs, _uint iNumElement, const _tchar* pShaderFilePath, _uint iTechniqueIndex = 0);
	HRESULT Sort_MeshesByMaterial();
	HRESULT SetUp_SkinnedInfo();
	HRESULT SetUp_AnimationInfo();

	CHierarchyNode* Find_HierarchyNode(const char* pBoneName);

	HRESULT Update_CombinedTransformationMatrices(_double DeltaTime);

	void Add_Channel_To_HierarchyNode(_uint iAnimationindex, class CChannel* pChannel);


public:
	static CModel*	Create(DEVICES, const char* pMeshFilePath, const char* pMeshFileName, const _tchar* pShaderFilePath, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

protected:
	vector<class CAnimation*>		m_Animations;
	_uint							m_iAnimationIndex = 0;
	_matrix							m_PivotMatrix;

private:
	const aiScene*		m_pScene = nullptr;		// 모든 모델들의 메쉬, 머테리얼 정보를 가지고 있을 공간
	Assimp::Importer	m_Importer;

private:
	_uint				m_iNumVertices = 0;		// 정점 개수
	_uint				m_iNumFaces = 0;		// 삼각형(폴리곤)의 개수
	VTXMESH*			m_pVertices = nullptr;
	POLYGONINDICES32*	m_pPolygonIndices32 = nullptr;

	vector<class CMeshContainer*>			m_MeshContainers;

	vector<vector<class CMeshContainer*>>	m_SortByMaterialMesh;		// 같은 재질을 가진 메쉬컨테이너끼리 묶어서 보관한다.
	vector<MODELTEXTURES*>					m_ModelTextures;

	vector<class CHierarchyNode*>			m_HierarchyNodes;

private:
	ID3D11Buffer*				m_pVB = nullptr;
	ID3D11Buffer*				m_pIB = nullptr;
	_uint						m_iStride = 0;

protected:
	vector<EFFECTDESC>			m_EffectDescs;
	ID3DX11Effect*				m_pEffect = nullptr;
};
END
#endif // !__MODEL_H__
