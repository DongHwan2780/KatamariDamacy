
#include "Shader_Defines.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;	
}

cbuffer LightDesc			// 터레인에 들어오는 빛 정보
{
	vector		g_vLightDir;
	vector		g_vLightDiffuse;
	vector		g_vLightAmbient;
	vector		g_vLightSpecular;
}

cbuffer MaterialDesc		// 터레인 재질 정보
{
	vector		g_vMtrlDiffuse;
	vector		g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
	vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
	float		g_fPower = 30.f;		// 스펙큘러값을 몇 승(g_fPower) 해서 쓸것인지
}

cbuffer EtcDesc
{
	vector		g_vBrushPos = vector(10.f, 0.f, 10.f, 1.f);			// 마법진 이미지 위치
	float		g_fRange = 5.f;			// 마법진 이미지 반지름(크기) 이고

	vector		g_vCamPosition;			// 캠 포지션
}


texture2D		g_DiffuseSourTexture;
texture2D		g_DiffuseDestTexture;
texture2D		g_BrushTexture;

SamplerState	g_DiffuseSampler
{
	Filter = min_mag_mip_linear;

	AddressU = wrap;
	AddressV = wrap;
};


texture2D		g_FilterTexture;

SamplerState	g_FilterSampler
{
	Filter = min_mag_mip_point;

	AddressU = wrap;
	AddressV = wrap;
};






struct VS_IN
{
	float3	vPosition : POSITION; /* 로컬스페이스 */
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
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

/* VS_OUT 구조체의 SV_POSITION이라는 시멘틱을 가진 데이터에 한해서만  */
/* W값으로 XYZW모두를 나눈다. */
/* 뷰포트로 정점의 위치를 변환한다. */
/* 래스터라이즈 : 정점으로 둘러쌓여진 픽셀의 정보를 생성하낟.(둘러쌓여진 정점의 정보를 기반으로하여)  */

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;
};

vector	PS_MAIN(PS_IN In) : SV_TARGET0
{
	vector		vColor = (vector)0;

	vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, In.vTexUV);

	return vColor;
}



technique11		DefaultDevice
{
	pass DefaultPass
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}	
}