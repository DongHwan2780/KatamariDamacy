#pragma once

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;

namespace Client {}

using namespace Client;

enum SCENE {STATIC_SCENE, LOGO_SCENE, LOADING_SCENE, STAGEONE_SCENE, END_SCENE };