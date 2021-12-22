#pragma once

#ifndef __ENGINESTRUCT_H__
#define __ENGINESTRUCT_H__

namespace Engine
{
	typedef struct tagKeyFrameDesc		// 중간동작을 표현하기위한 모션(키프레임)을 구성하는 뼈 하나의 상태
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
		double			Time; /* 현재 키프레임이 표현되어야할 시간. */
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
		XMFLOAT4		vLightDir;		// 빛의 방향

		XMFLOAT4		vDiffuse;		// 난반사광원 == 빛의 색상 정보(물체의 기본적인 색상)
		XMFLOAT4		vAmbient;		// 환경광에 의한 색상 == 다른 오브젝트에 의해 튕겨 들어온 빛에 의해 보이는 색상, 명암값에 더해서 사용함
		XMFLOAT4		vSpecular;		// 정반사광원 == 번들거리는 하이라이트 효과를 줄때 사용
	}LIGHTDESC;

	typedef struct tagMaterialDesc		// 빛이 들어오면 물체가 반사해야하는 정보
	{
		XMFLOAT4		vDiffuse;		// 픽셀 단위로 반사되야 하는 색상을 담은 정보
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
