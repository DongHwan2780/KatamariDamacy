#pragma once

#ifndef __ENGINESTRUCT_H__
#define __ENGINESTRUCT_H__

namespace Engine
{
	typedef struct tagKeyFrameDesc		// �߰������� ǥ���ϱ����� ���(Ű������)�� �����ϴ� �� �ϳ��� ����
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
		double			Time; /* ���� Ű�������� ǥ���Ǿ���� �ð�. */
	}KEYFRAMEDESC;

	typedef struct tagBoneDesc
	{
		char*		pName = nullptr;
		class CHierarchyNode*	pHierarchyNode = nullptr;
		// XMFLOAT4X4		OffsetMatrix * CombindMatrix(TransformationMatrix * pParent->CombindMatrix);
		XMFLOAT4X4		OffsetMatrix;
	}BONEDESC;

	typedef struct TagModelTextures
	{
		class CTexture*		pModelTexture[AI_TEXTURE_TYPE_MAX];
	}MODELTEXTURES;

	typedef struct tagEffectDesc
	{
		ID3DX11EffectPass*		pPass = nullptr;
		ID3D11InputLayout*		pLayOut = nullptr;
	}EFFECTDESC;

	typedef struct tagLightDesc
	{
		XMFLOAT4		vLightDir;		// ���� ����

		XMFLOAT4		vDiffuse;		// ���ݻ籤�� == ���� ���� ����(��ü�� �⺻���� ����)
		XMFLOAT4		vAmbient;		// ȯ�汤�� ���� ���� == �ٸ� ������Ʈ�� ���� ƨ�� ���� ���� ���� ���̴� ����, ��ϰ��� ���ؼ� �����
		XMFLOAT4		vSpecular;		// ���ݻ籤�� == ����Ÿ��� ���̶���Ʈ ȿ���� �ٶ� ���
	}LIGHTDESC;

	typedef struct tagMaterialDesc		// ���� ������ ��ü�� �ݻ��ؾ��ϴ� ����
	{
		XMFLOAT4		vDiffuse;		// �ȼ� ������ �ݻ�Ǿ� �ϴ� ������ ���� ����
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
	}MATERIALDESC;

	typedef struct tagVertex_Mesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex;
		XMFLOAT4		vBlendWeight;
	}VTXMESH;

	typedef struct tagVertex_Color
	{
		XMFLOAT3		vPosition;
		XMFLOAT4		vColor;
	}VTXCOL;

	typedef struct tagVertexInfo
	{
		XMFLOAT3	vPos;
		XMFLOAT2	vTexUV;
	}VTXINFO;

	typedef struct tagVertex_NormalInfo
	{
		XMFLOAT3		vPos;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
	}VTXNORMALINFO;

	typedef struct tagVertex_Instancing
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vPosition;
	}VTXINSTANCE;

	typedef struct taglineIndices16
	{
		unsigned short	_0, _1;
	}LINEINDICES16;

	typedef struct tagPolygonIndices16
	{
		_ushort	_0, _1, _2;
	}POLYGONINDICES16;

	typedef struct tagPolygonIndices32
	{
		_ulong	_0, _1, _2;
	}POLYGONINDICES32;
}


#endif // ! __ENGINESTRUCT__H_-
