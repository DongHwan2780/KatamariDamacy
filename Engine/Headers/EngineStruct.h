#pragma once

#ifndef __ENGINESTRUCT_H__
#define __ENGINESTRUCT_H__

namespace Engine
{
	typedef struct tagLightDesc
	{
		XMFLOAT4		vLightDir;		// ���� ����

		XMFLOAT4		vDiffuse;		// ���� ���� ����(��ü�� �⺻���� ����)
		XMFLOAT4		vAmbient;		// ȯ�汤�� ���� ���� == �ٸ� ������Ʈ�� ���� ƨ�� ���� ���� ���� ���̴� ����, ��ϰ��� ���ؼ� �����
		XMFLOAT4		vSpecular;		// ����Ÿ��� ���̶���Ʈ ȿ���� �ٶ� ���
	}LIGHTDESC;

	typedef struct tagMaterialDesc		// ���� ������ ��ü�� �ݻ��ؾ��ϴ� ����
	{
		XMFLOAT4		vDiffuse;		// �ȼ� ������ �ݻ�Ǿ� �ϴ� ������ ���� ����
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
	}MATERIALDESC;

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
