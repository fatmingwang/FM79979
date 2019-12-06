#ifndef StdAfx_H
#define StdAfx_H

#ifndef _UNICODE
#define _UNICODE
#endif

#if defined(WIN32)
	#pragma warning( disable : 4996 )
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <winsock.h>
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
#ifdef DEBUG
	#include <crtdbg.h>
#endif
#elif defined(ANDROID) 
#elif defined(IOS)
#endif

#include <math.h>
#include <wchar.h>

#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <assert.h>
//#include "GameplayUT/GameUTDefine.h"
#include "Common/CommonDefine.h"
#include "Common/NamedTypedObject.h"
#include "Common/Template/TemplateClass.h"
//#include "XML/StringToStructure.h"
#include "AllMathInclude.h"
#include "Common/Utility.h"
//#include "AllCoreInclude.h"
//#include "Sound/SoundManager.h"
//#if defined(WIN32)
//	typedef std::basic_string<unsigned char> MyStringForMVSC;
//	#define string MyStringForMVSC
//	typedef std::basic_string<wchar_t> MyStringForMVSCW;
//	#define wstring MyStringForMVSCW
//#endif

#endif
