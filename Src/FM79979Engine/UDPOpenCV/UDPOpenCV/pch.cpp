#include "pch.h"
#pragma comment(lib, "../../../lib/OpenAL32.lib")
#pragma comment(lib, "../../../lib/alut.lib")
#ifdef _DEBUG
#ifdef USE_MU_PARSER
#pragma comment(lib, "../../../lib/Debug/MathParser.lib")
#endif
#pragma comment(lib, "../../../lib/Debug/Bullet.lib")
#ifndef UWP
#pragma comment(lib, "../../../lib/Debug/Core.lib")
#endif
#pragma comment(lib, "../../../lib/Debug/Freetype.lib")
//#pragma comment(lib, "../../../lib/Debug/FatmingCollada.lib")
#pragma comment(lib, "../../../lib/Debug/ogg.lib")
#pragma comment(lib, "../../../lib/Debug/UWPNetwork.lib")
#pragma comment(lib, "../../../lib/Debug/Win32UDPOpenCV.lib")
#pragma comment(lib, "../../../lib/opencv_world440d.lib")
#else
#ifdef USE_MU_PARSER
#pragma comment(lib, "../../../lib/Release/MathParser.lib")
#endif
#pragma comment(lib, "../../../lib/Release/Bullet.lib")
#ifndef UWP
#pragma comment(lib, "../../../lib/Release/Core.lib")
#endif
//#pragma comment(lib, "../../../lib/Release/FatmingCollada.lib")
#pragma comment(lib, "../../../lib/Release/Freetype.lib")
#pragma comment(lib, "../../../lib/Release/ogg.lib")
#pragma comment(lib, "../../../lib/Release/UWPNetwork.lib")
#pragma comment(lib, "../../../lib/Release/Win32UDPOpenCV.lib")
#pragma comment(lib, "../../../lib/opencv_world440.lib")

#endif
//#pragma comment(lib, "../x64/Debug/Win32Network.lib")

#pragma comment(lib, "wsock32.lib")
