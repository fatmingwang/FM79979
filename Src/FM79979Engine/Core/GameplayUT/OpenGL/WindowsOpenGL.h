#pragma once
#ifdef WIN32
#include "windows.h"

bool					InitMultisample(HDC e_HdcMV);
HGLRC					InitOpenGL(HWND e_pHwnd, bool e_bInitGlewInit, HDC e_HdcMV, bool e_bEnableMultisample = false);
bool					SetOpenGLSwapInterval(bool e_bSwapInterval);
bool 					IsExtensionSupported(char* szTargetExtension);

#endif