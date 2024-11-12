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

#include "../Core/AllCoreInclude.h"
#include "../Core/FatmingCollada/AllColladaInclude.h"
#include "../imgui/ImGuiRender.h"
#include "../imgui/imgui.h"