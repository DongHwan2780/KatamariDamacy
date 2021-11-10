#pragma once

#ifndef __ENGINE_DEFINES_H__
#define __ENGINE_DEFINES_H__

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

#endif // !__ENGINE_DEFINES_H__
