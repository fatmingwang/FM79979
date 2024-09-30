// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifdef _UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#ifdef WIN32
#include "targetver.h"
#include <winsock2.h>
#include <ws2bth.h>
#include <Bthsdpdef.h>
#include <Setupapi.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#endif
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently



#include "../../Core/AllCoreInclude.h"
#include "../../Core/FatmingCollada/AllColladaInclude.h"
#include "../../imgui/ImGuiRender.h"
#include "../../imgui/imgui.h"