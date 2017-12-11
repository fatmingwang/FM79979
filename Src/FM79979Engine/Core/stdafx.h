#ifndef StdAfx_H
#define StdAfx_H

#ifndef _UNICODE
#define _UNICODE
#endif

#if defined(WIN32)
	#pragma warning( disable : 4996 )
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#elif defined(ANDROID)
 //#include <cstdarg>
#elif defined(IOS)

//#ifndef IOS
//#define	IOS
//#endif

#endif


#include <math.h>
#include <wchar.h>
#if defined(WIN32)
	#include "../../Include/glew.h"
	#include "../../Include/glext.h"
#elif defined(IOS)
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
#elif defined(ANDROID)
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES/glplatform.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <GLES2/gl2platform.h>
#elif defined(LINUX)
	//#include "glew.h"
	//#include "glext.h"
#elif defined(WASM)
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <GLES3/gl3.h>
#endif

#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <assert.h>
#include "GameplayUT/GameUTDefine.h"
#include "CommonDefine.h"
#include "NamedTypedObject.h"
#include "TemplateClass.h"
#include "XML/StringToStructure.h"
#include "AllMathInclude.h"
#include "Utility.h"
//#include "Sound/SoundManager.h"
//#if defined(WIN32)
//	typedef std::basic_string<unsigned char> MyStringForMVSC;
//	#define string MyStringForMVSC
//	typedef std::basic_string<wchar_t> MyStringForMVSCW;
//	#define wstring MyStringForMVSCW
//#endif

#endif
