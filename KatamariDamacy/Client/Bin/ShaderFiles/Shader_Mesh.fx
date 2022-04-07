
#include "Shader_Defines.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;	
}

struct MeshBoneMatrices
{
	matrix		BoneMatrices[128];
};

MeshBoneMatrices		g_BoneMatrices;

texture2D		g_DiffuseTexture;

SamplerState	g_DiffuseSampler
{
	AddressU = mirror;
	AddressV = mirror;
};


struct VS_IN
{
	float3	vPosition : POSITION; /* 로컬스페이스 */
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float3	vTangent : TANGENT;
	uint4	vBlendIndex : BLENDINDEX; /* 현재정점에 영향ㅇ르 주는 네개의 뼈인덱슬르 보관하고 있다. */
	float4	vBlendWeight : BLENDWEIGHT; /* 각 뼈인덱스의 해당하는 영향비율(0.0ㄹ\ ~ 1) */ /*네개 값의 합은 1 */
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;	
};

/* 정점의 스페이스 변환. (월드, 뷰, 투영행렬의 곱.)*/
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}


VS_OUT VS_MAIN_ANIM(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	matrix		BoneMatrix = g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x + 
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y + 
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * In.vBlendWeight.w;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;

}

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;
};

vector	PS_MAIN(PS_IN In) : SV_TARGET
{
	vector		vColor = (vector)0;

	vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, In.vTexUV);

	return vColor;
}



technique11		DefaultDevice
{
	pass NonAnimation
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}	
	pass Animation
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}