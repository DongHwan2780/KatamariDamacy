#pragma once

#ifndef __ENGINE_DEFINES_H__
#define __ENGINE_DEFINES_H__

#pragma warning (disable : 4251)

#pragma region KEY_DEFINE
#define KEY_LEFT	 0x00000001
#define KEY_UP		 0x00000002
#define KEY_RIGHT	 0x00000004
#define KEY_DOWN	 0x00000008
#define KEY_RETURN	 0x00000010
#pragma endregion


#include <d3d11.h>
#include <unordered_map>
#include <algorithm>
#include <typeinfo>
#include <process.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

#include "EngineMacro.h"
#include "EngineTypedef.h"
#include "EngineFunction.h"

using namespace std;
using namespace Engine;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#pragma region SOUND
#include <io.h>
#include <stdio.h>
#include "fmod.h"
#pragma comment (lib, "fmod_vc.lib")
#pragma comment (lib, "fmodL_vc.lib")
#pragma endregion	

#endif // !__ENGINE_DEFINES_H__
