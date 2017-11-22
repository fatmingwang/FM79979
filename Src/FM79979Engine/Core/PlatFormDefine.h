#ifndef _PLATFORM_DEFINE_H_
#define _PLATFORM_DEFINE_H_
// If we cause an exception in JNI, we print the exception info to the log, we clear the exception to avoid a pending-exception crash, and we force the function to return.
#ifdef ANDROID
#define EXCEPTION_RETURN(env)if (env->ExceptionOccurred()) {env->ExceptionDescribe();env->ExceptionClear();return false;}
#define EXCEPTION_RETURN_NULL(env)if (env->ExceptionOccurred()) {env->ExceptionDescribe();env->ExceptionClear();return 0;}
#endif
//===============================================
// Cross-platform type definitions
#ifdef WIN32
    typedef signed char int8;
    typedef short int16;
    typedef long int32;
    typedef __int64 int64;
    typedef unsigned char uint8;
    typedef unsigned short uint16;
    typedef unsigned long uint32;
    typedef unsigned __int64 uint64;
    //#define	MYWCSTOK(y,z)	wcstok(y,z);
#else // For LINUX and IOS
	#pragma GCC diagnostic ignored "-Wformat"
	//#pragma GCC diagnostic ignored "-Wformat-contain-nul"
	//#pragma GCC diagnostic ignored "-Wformat-extra-args"
	#pragma GCC diagnostic ignored "-Wformat-zero-length"
	#pragma GCC diagnostic ignored "-Wunused-variable"
	#pragma GCC diagnostic ignored "-Wwrite-strings"
    //windows type
	#define CONST				const
    #define VOID void
    typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
    typedef int                 INT;
    typedef unsigned int        UINT;
	typedef unsigned long       DWORD;
    typedef unsigned char       BYTE;
    typedef unsigned short      WORD;
    typedef float               FLOAT;
    typedef unsigned int        UINT;
    typedef char                CHAR;
    typedef long                LONG;
    typedef unsigned short      WORD;
    typedef short				SHORT;
#ifndef IOS
//	typedef bool				BOOL;
#endif
    typedef int8_t				int8;
    typedef int16_t				int16;
    typedef int32_t				int32;
    typedef int64_t				int64;
    typedef unsigned char		uint8;
    typedef unsigned int		uint16;
    typedef unsigned long int	uint32;
    typedef unsigned long long int uint64;
	typedef uint64				UINT64;
    typedef uint8 byte;
	#define	 TRUE				1
	#define	 FALSE				0
	//#define	 nullptr				0

    typedef struct tagRECT
    {
        LONG    left;
        LONG    top;
        LONG    right;
        LONG    bottom;
    } RECT;
    typedef struct tagPOINT
    {
        LONG  x;
        LONG  y;
    } POINT;


    static wchar_t* g_pStringForWCSTOK = 0;
    //#define	MYWCSTOK(y,z)   wcstok(y,z,&g_pStringForWCSTOK)
	#define	wcstok(y,z)   wcstok(y,z,&g_pStringForWCSTOK)
	#define	_atoi64	atoll//for string to uint64
    #ifndef _CLIMITS_
    #include <climits>
    #endif // _CLIMITS_
#endif // PLATFORMS
//===============================================
//math
#ifndef D3DXToRadian
#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))
#endif
//===============================================
#ifndef D3DXToDegree
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))
#endif
//===============================================
#ifndef D3DX_PI
#define D3DX_PI    (3.14159265358979323846f)
#endif
//===============================================
#if defined WIN32
#ifndef GL_COLOR_ATTACHMENT0
#define GL_COLOR_ATTACHMENT0                            GL_COLOR_ATTACHMENT0_EXT
#endif
#ifndef GL_DEPTH_ATTACHMENT
#define GL_DEPTH_ATTACHMENT                             GL_DEPTH_ATTACHMENT_EXT
#endif
#ifndef GL_FRAMEBUFFER_COMPLETE
#define GL_FRAMEBUFFER_COMPLETE                         GL_FRAMEBUFFER_COMPLETE_EXT
#endif
#ifndef GL_FRAMEBUFFER_UNSUPPORTED
#define GL_FRAMEBUFFER_UNSUPPORTED                      GL_FRAMEBUFFER_UNSUPPORTED_EXT
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT    GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT            GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS            GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
#endif
#ifndef GL_FRAMEBUFFER
#define GL_FRAMEBUFFER                                  GL_FRAMEBUFFER_EXT
#endif
#ifndef GL_RENDERBUFFER
#define GL_RENDERBUFFER                                 GL_RENDERBUFFER_EXT
#endif
#ifndef glGenFramebuffers
#define glGenFramebuffers                               glGenFramebuffersEXT
#endif
#ifndef glBindFramebuffer
#define glBindFramebuffer                               glBindFramebufferEXT
#endif
#ifndef glBindRenderbuffer
#define glBindRenderbuffer                              glBindRenderbufferEXT
#endif
#ifndef glCheckFramebufferStatus
#define glCheckFramebufferStatus                        glCheckFramebufferStatusEXT
#endif
#ifndef glFramebufferRenderbuffer
#define glFramebufferRenderbuffer                       glFramebufferRenderbufferEXT
#endif
#ifndef glFramebufferTexture2D
#define glFramebufferTexture2D                          glFramebufferTexture2DEXT
#endif
#ifndef glGenRenderbuffers
#define glGenRenderbuffers                              glGenRenderbuffersEXT
#endif
#ifndef glRenderbufferStorage
#define glRenderbufferStorage                           glRenderbufferStorageEXT
#endif
#ifndef glDeleteFramebuffers
#define glDeleteFramebuffers                            glDeleteFramebuffersEXT
#endif
#ifndef glDeleteRenderbuffers
#define glDeleteRenderbuffers                           glDeleteRenderbuffersEXT
#endif
#ifndef glClearDepthf
#define glClearDepthf                                   glClearDepth
#endif
#ifndef GL_RGBA32F
#define GL_RGBA32F										GL_RGBA32F_ARB
#endif
#ifndef GL_RGB32F
#define GL_RGB32F										GL_RGB32F_ARB
#endif
#ifndef GL_RGBA16F
#define GL_RGBA16F										GL_RGBA16F_ARB
#endif
#ifndef GL_RGB16F
#define GL_RGB16F										GL_RGB16F_ARB
#endif
#endif
//===============================================
#ifdef WIN32
#pragma warning(disable:4702)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#else
#ifdef IOS
#include <ctype.h>
#include <wctype.h>
#include <unistd.h>
#include <string>
#include <wchar.h>
#include <stdint.h>
#else // IOS
#if defined(LINUX) || defined(__PPU__)
#include <ctype.h>
#include <wctype.h>
#include <unistd.h>
#include <string>
#include <wchar.h>
#include <stdarg.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#else // OTHER...                 only for linux and windows!!
//#error "Unsupported platform."
#else
#if defined(ANDROID)	
#include "unistd.h"
#endif //ANDROID
#endif // LINUX || __PPU__
#endif // IOS
#endif // WIN32
//===============================================
#ifndef MAX_PATH
#define MAX_PATH			260
#endif


#define IPAD_RESOLUTION_WIDTH		768
#define IPAD_RESOLUTION_HEIGHT		1024

#define NEW_IPAD_RESOLUTION_WIDTH   1536
#define NEW_IPAD_RESOLUTION_HEIGHT  2048

#define IPHONE_RESOLUTION_WIDTH		320
#define IPHONE_RESOLUTION_HEIGHT	480

#define IPHONE_4_RESOLUTION_WIDTH   640
#define IPHONE_4_RESOLUTION_HEIGHT  960

#define IPHONE_5_RESOLUTION_WIDTH   640
#define IPHONE_5_RESOLUTION_HEIGHT  1136

#define	FULL_HD_RESOLUTION_WIDTH	1920
#define	FULL_HD_RESOLUTION_HEIGHT	1080

#define	DATA_TYPE	L"DataType"

enum	eDataType
{
	eDT_BYTE = 0,
	eDT_SHORT,
	eDT_FLOAT,
	eDT_DOUBLE,
	eDT_INT,
	eDT_UINT,
	eDT_STRING, //string(std::string)
	eDT_WSTRING,//wstring(std::wstring)
	eDT_VECTOR2,
	eDT_VECTOR3,
	eDT_VECTOR4,
	eDT_POINT,
	eDT_VOID,	//could be any type
	eDT_INT64,
	eDT_UINT64,
	eDT_MAX,
};

//original name eUVDirection
enum	eMoveDirection
{
	eMD_LEFT_TO_RIGHT = 0,
	eMD_RIGHT_TO_LEFT,
	eMD_UP_TO_DOWN,
	eMD_DOWN_TO_UP,
	eMD_STAY,
	eMD_MAX,
};

static int	g_iDataTypeSize[eDT_MAX] = {
										sizeof(char),//byte
										sizeof(float),//float
										sizeof(double),//double
										sizeof(int),//int
										-1,//string
										-1,//wstring
										sizeof(float)*2,//vector2
										sizeof(float)*3,//vector3
										sizeof(float)*4,//vector4
										sizeof(long)*2,//point
										-1,//void
										sizeof(int64),//int64
										sizeof(uint64)//UINT64
										};

#endif