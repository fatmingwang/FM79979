#pragma once
#include <wchar.h>
#include <stdint.h>
// If we cause an exception in JNI, we print the exception info to the log, we clear the exception to avoid a pending-exception crash, and we force the function to return.
#ifdef ANDROID
#define EXCEPTION_RETURN(env)if (env->ExceptionOccurred()) {env->ExceptionDescribe();env->ExceptionClear();return false;}
#define EXCEPTION_RETURN_NULL(env)if (env->ExceptionOccurred()) {env->ExceptionDescribe();env->ExceptionClear();return 0;}
#include "stdint.h"
#endif
//===============================================
// Cross-platform type definitions
#if defined(WIN32) || defined(UWP)
    typedef signed char int8;
    typedef short int16;
    typedef long int32;
    typedef __int64 int64;
    typedef unsigned char uint8;
    typedef unsigned short uint16;
    typedef unsigned long uint32;
    typedef unsigned __int64 uint64;
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

	#define	_atoi64	atoll//for string to uint64
    #ifndef _CLIMITS_
    #include <climits>
	#include <unistd.h>//for usleep
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

static int	g_iDataTypeSize[eDT_MAX] = {
										sizeof(char),					//eDT_BYTE = 0,
										sizeof(short),					//eDT_SHORT,
										sizeof(float),					//eDT_FLOAT,
										sizeof(double),					//eDT_DOUBLE,
										sizeof(int),					//eDT_INT,
										sizeof(unsigned int),			//eDT_UINT,
										0,								//eDT_STRING,
										0,								//eDT_WSTRING,
										sizeof(float) * 2,				//eDT_VECTOR2,
										sizeof(float) * 3,				//eDT_VECTOR3,
										sizeof(float) * 4,				//eDT_VECTOR4,
										sizeof(long) * 2,				//eDT_POINT,
										0,								//eDT_VOID,
										sizeof(int64),					//eDT_INT64,
										sizeof(uint64)					//UINT64,
};