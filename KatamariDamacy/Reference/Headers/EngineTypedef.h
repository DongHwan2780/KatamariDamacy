#pragma once

#ifndef __ENGINE_TYPEDEF_H__
#define __ENGINE_TYPEDEF_H__

namespace Engine
{

	typedef unsigned char			_ubyte;
	typedef signed char				_byte;

	typedef unsigned short			_ushort;
	typedef signed short			_short;

	typedef unsigned int			_uint;
	typedef signed int				_int;

	typedef unsigned long			_ulong;
	typedef signed long				_long;

	typedef float					_float;
	typedef double					_double;

	typedef bool					_bool;

	typedef wchar_t					_tchar;

	typedef XMFLOAT2				_float2;
	typedef XMFLOAT3				_float3;
	typedef XMFLOAT4				_float4;

	typedef XMVECTOR				_vector;		// 멤버변수가 따로 정의되어있지 않은 통짜 벡터 -> 멤버변수에 담아서 쓰려면 _float형식으로 캐스팅해야함
	typedef FXMVECTOR				_fvector;		// XMVECTOR형식에 매개변수를 사용하기 위한 벡터 == _float3?

	typedef XMFLOAT4X4				_float4x4;		// WorldMatrix,  4x4구조체 행렬

	typedef XMMATRIX				_matrix;
	typedef FXMMATRIX				_fmatrix;

}

#endif // !__ENGINE_TYPEDEF_H__