#include "stdafx.h"
#include "Utility.h"
#include "GameplayUT/GameApp.h"
#include <errno.h>
#ifdef WIN32
#include <direct.h>
#endif

#if defined(IOS)
#include <mach/mach_time.h>
#include <sys/time.h>
#elif defined(WIN32)
#include "strsafe.h"
#pragma warning( disable : 4793 )
#pragma warning( disable : 4995 )
#include "../../Include/wglext.h"
//#include "../../Include/glExt.h"
//#define WGL_SAMPLE_BUFFERS_ARB	0x2041
//#define WGL_SAMPLES_ARB		0x2042
bool	arbMultisampleSupported	= false;
int		arbMultisampleFormat	= 0;
#elif defined(IOS)
#include <stdio.h>
#include <sys/stat.h>
#elif defined(ANDROID)
#include "jni.h"
#include <sys/stat.h>
#endif

namespace FATMING_CORE
{
	extern bool	g_bVBOSupported;
}
namespace UT
{
	char	g_sstrForStringToStructure[TEMP_SIZE];
	wchar_t	g_sstrForStringToStructureW[TEMP_SIZE];
#ifdef DEBUG
	void	MyGlErrorTest()
	{
		int	l_i = glGetError();
		if( l_i != 0 )
		{
			//bool	l_b0 = glIsEnabled(GL_VERTEX_ARRAY)==GL_TRUE;
			//bool	l_b1 = glIsEnabled(GL_TEXTURE_COORD_ARRAY)==GL_TRUE;
			//bool	l_b2 = glIsEnabled(GL_NORMAL_ARRAY)==GL_TRUE;
			//bool	l_b3 = glIsEnabled(GL_COLOR_ARRAY)==GL_TRUE;
			cGameApp::OutputDebugInfoString(L"GLError occur");
			int a=0;
			//GL_INVALID_ENUM 1280
			//GL_INVALID_VALUE 1281
			//GL_INVALID_OPERATION 1282
		}
	}
#endif
#ifdef WIN32
	HGLRC	InitOpenGL(HWND e_pHwnd,bool e_bInitGlewInit,HDC e_HdcMV)
	{
		static	bool	l_b =false;
		GLuint PixelFormat;													// Will Hold The Selected Pixel Format
		PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
		{
			sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
			1,																// Version Number
			PFD_DRAW_TO_WINDOW |											// Format Must Support Window
			PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,												// Must Support Double Buffering
			PFD_TYPE_RGBA,													// Request An RGBA Format
			32,																// Select Our Color Depth
			0, 0, 0, 0, 0, 0,												// Color Bits Ignored
			1,																// No Alpha Buffer
			0,																// Shift Bit Ignored
			0,																// No Accumulation Buffer
			0, 0, 0, 0,														// Accumulation Bits Ignored
			16,																// 16Bit Z-Buffer (Depth Buffer)  
			0,																// No Stencil Buffer
			0,																// No Auxiliary Buffer
			PFD_MAIN_PLANE,													// Main Drawing Layer
			0,																// Reserved
			0, 0, 0															// Layer Masks Ignored
		};
		//fuck multi windows occur error
		//e_HdcMV = GetDC (e_pHwnd);									// Grab A Device Context For This Window
		if (e_HdcMV == 0)												// Did We Get A Device Context?
		{
			assert(0);
		}
		PixelFormat = ChoosePixelFormat (e_HdcMV, &pfd);				// Find A Compatible Pixel Format
		if (PixelFormat == 0)												// Did We Find A Compatible Format?
		{
			assert(0);
		}
		if( arbMultisampleFormat != 0 )//if u want to setup multi sample u have to ensure arbMultisampleFormat have been assigned
			PixelFormat = arbMultisampleFormat;
		if (SetPixelFormat (e_HdcMV, PixelFormat, &pfd) == FALSE)		// Try To Set The Pixel Format
		{
			assert(0);
		}
		HGLRC	l_HGLRC = wglCreateContext (e_HdcMV);						// Try To Get A Rendering Context
		if (l_HGLRC == 0)												// Did We Get A Rendering Context?
		{
			assert(0);												// Return False
		}

		// Make The Rendering Context Our Current Rendering Context
		if (wglMakeCurrent (e_HdcMV, l_HGLRC) == FALSE)
		{
			assert(0);
		}
		if( e_bInitGlewInit && !l_b )
		{
			l_b  =true;
			GLenum	l_eErrorID = glewInit();
			assert(l_eErrorID == 0);
		}
		FATMING_CORE::g_bVBOSupported = IsExtensionSupported( "GL_ARB_vertex_buffer_object" );
		MyGlErrorTest();
		return l_HGLRC;
	}
	bool WGLisExtensionSupported(const char *extension)
	{
		const size_t extlen = strlen(extension);
		const char *supported = nullptr;

		// Try To Use wglGetExtensionStringARB On Current DC, If Possible
		PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

		if (wglGetExtString)
			supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

		// If That Failed, Try Standard Opengl Extensions String
		if (supported == nullptr)
			supported = (char*)glGetString(GL_EXTENSIONS);

		// If That Failed Too, Must Be No Extensions Supported
		if (supported == nullptr)
			return false;

		// Begin Examination At Start Of String, Increment By 1 On False Match
		for (const char* p = supported; ; p++)
		{
			// Advance p Up To The Next Possible Match
			p = strstr(p, extension);

			if (p == nullptr)
				return false;						// No Match

			// Make Sure That Match Is At The Start Of The String Or That
			// The Previous Char Is A Space, Or Else We Could Accidentally
			// Match "wglFunkywglExtension" With "wglExtension"

			// Also, Make Sure That The Following Character Is Space Or nullptr
			// Or Else "wglExtensionTwo" Might Match "wglExtension"
			if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
				return true;						// Match
		}
	}
			
	bool InitMultisample(HINSTANCE hInstance,HWND hWnd)
	{  
		// See If The String Exists In WGL!
		if (!WGLisExtensionSupported("WGL_ARB_multisample"))
		{
			arbMultisampleSupported=false;
			return false;
		}

		// Get Our Pixel Format
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB =
			(PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

		if (!wglChoosePixelFormatARB)
		{
			// We Didn't Find Support For Multisampling, Set Our Flag And Exit Out.
			arbMultisampleSupported=false;
			return false;
		}

		// Get Our Current Device Context. We Need This In Order To Ask The OpenGL Window What Attributes We Have
		HDC hDC = GetDC(hWnd);

		int pixelFormat;
		bool valid;
		UINT numFormats;
		float fAttributes[] = {0,0};

		// These Attributes Are The Bits We Want To Test For In Our Sample
		// Everything Is Pretty Standard, The Only One We Want To 
		// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
		// These Two Are Going To Do The Main Testing For Whether Or Not
		// We Support Multisampling On This Hardware
		int iAttributes[] = { WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
			WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,24,
			WGL_ALPHA_BITS_ARB,8,
			WGL_DEPTH_BITS_ARB,16,
			WGL_STENCIL_BITS_ARB,0,
			WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
			WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
			WGL_SAMPLES_ARB, 4 ,						// Check For 4x Multisampling
			0,0};

		// First We Check To See If We Can Get A Pixel Format For 4 Samples
		valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats)?true:false;
	 
		// if We Returned True, And Our Format Count Is Greater Than 1
		if (valid && numFormats >= 1)
		{
			arbMultisampleSupported	= true;
			arbMultisampleFormat	= pixelFormat;	
			return arbMultisampleSupported;
		}

		// Our Pixel Format With 4 Samples Failed, Test For 2 Samples
		iAttributes[19] = 2;
		valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats)?true:false;
		if (valid && numFormats >= 1)
		{
			arbMultisampleSupported	= true;
			arbMultisampleFormat	= pixelFormat;	 
			return arbMultisampleSupported;
		}
		  
		// Return The Valid Format
		return  arbMultisampleSupported;
	}
	bool IsExtensionSupported( char* szTargetExtension )
	{
		const unsigned char *pszExtensions = nullptr;
		const unsigned char *pszStart;
		unsigned char *pszWhere, *pszTerminator;

		// Extension names should not have spaces
		pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
		if( pszWhere || *szTargetExtension == '\0' )
			return false;

		// Get Extensions String
		pszExtensions = glGetString( GL_EXTENSIONS );

		// Search The Extensions String For An Exact Copy
		pszStart = pszExtensions;
		for(;;)
		{
			pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
			if( !pszWhere )
				break;
			pszTerminator = pszWhere + strlen( szTargetExtension );
			if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
				if( *pszTerminator == ' ' || *pszTerminator == '\0' )
					return true;
			pszStart = pszTerminator;
		}
		return false;
	}
#endif
	//--------------------------------------------------------------------------------------
	// Name: DebugSpewV()
	// Desc: Internal helper function
	//--------------------------------------------------------------------------------------
	void DebugSpewV( const CHAR* strFormat, const va_list pArgList )
	{
		CHAR str[2048];
		// Use the secure CRT to avoid buffer overruns. Specify a count of
		// _TRUNCATE so that too long strings will be silently truncated
		// rather than triggering an error.
#ifndef IOS
		vsnprintf( str, sizeof(str), strFormat, pArgList );
		cGameApp::OutputDebugInfoString( str );
#endif
	}
	void DebugSpewV( const wchar_t* strFormat, const va_list pArgList )
	{
		wchar_t str[2048];
		// Use the secure CRT to avoid buffer overruns. Specify a count of
		// _TRUNCATE so that too long strings will be silently truncated
		// rather than triggering an error.
#ifdef WIN32
		_vsnwprintf( str, sizeof(str), strFormat, pArgList );
#elif defined(ANDROID)
		vswprintf( str, sizeof(str), strFormat, pArgList );
#endif

		cGameApp::OutputDebugInfoString( str );
	}

	void	ErrorMsg(char*e_strErrMsg1,char*e_strErrMsg2)
	{
		cGameApp::WriteLog(e_strErrMsg1);
		cGameApp::WriteLog(e_strErrMsg2);
#if defined(WIN32)
		wchar_t	l_temp1[TEMP_SIZE];
		wchar_t	l_temp2[TEMP_SIZE];
		CHAR_TO_WCHAR(e_strErrMsg1,l_temp1);
		CHAR_TO_WCHAR(e_strErrMsg2,l_temp2);
		ErrorMsg(l_temp1,l_temp2);
#else
		//printf("%s, %s\n\0",e_strErrMsg1,e_strErrMsg2);
		std::string	l_strMessage = e_strErrMsg1;
		l_strMessage += "___";
		l_strMessage += e_strErrMsg2;
		cGameApp::OutputDebugInfoString(l_strMessage.c_str());
#endif
	}
	eDataType	GetDataType(const wchar_t*e_str)
	{
		if( !e_str )
			return eDT_MAX;
		if( !wcscmp(e_str,L"int") )
			return eDT_INT;
		if( !wcscmp(e_str,L"float") )
			return eDT_FLOAT;
		if( !wcscmp(e_str,L"byte") )
			return eDT_BYTE;
		if( !wcscmp(e_str,L"double") )
			return eDT_DOUBLE;
		if( !wcscmp(e_str,L"wstring") )
			return eDT_WSTRING;
		if( !wcscmp(e_str,L"string") )
			return eDT_STRING;
		if( !wcscmp(e_str,L"void") )
			return eDT_VOID;
		if( !wcscmp(e_str,L"Point") )
			return eDT_POINT;
		if( !wcscmp(e_str,L"Vector2") )
			return eDT_VECTOR2;
		if( !wcscmp(e_str,L"Vector3") )
			return eDT_VECTOR3;
		if( !wcscmp(e_str,L"Vector4") )
			return eDT_VECTOR4;
		if( !wcscmp(e_str,L"int64") )
			return eDT_INT64;
		if( !wcscmp(e_str,L"uint64") )
			return eDT_UINT64;
		return eDT_MAX;
	}
	eDataType	GetDataType(const char*e_str)
	{
		if( !strcmp(e_str,"int") )
			return eDT_INT;
		if( !strcmp(e_str,"float") )
			return eDT_FLOAT;
		if( !strcmp(e_str,"byte") )
			return eDT_BYTE;
		if( !strcmp(e_str,"double") )
			return eDT_DOUBLE;
		if( !strcmp(e_str,"wstring") )
			return eDT_WSTRING;
		if( !strcmp(e_str,"string") )
			return eDT_STRING;
		if( !strcmp(e_str,"void") )
			return eDT_VOID;
		if( !strcmp(e_str,"int64") )
			return eDT_INT64;
		if( !strcmp(e_str,"uint64") )
			return eDT_UINT64;
		return eDT_MAX;
	}
	void	ErrorMsg(const char*e_strErrMsg1,char*e_strErrMsg2){ ErrorMsg((char*)e_strErrMsg1,(char*)e_strErrMsg2);  }
	void	ErrorMsg(char*e_strErrMsg1,const char*e_strErrMsg2){ ErrorMsg((char*)e_strErrMsg1,(char*)e_strErrMsg2); }
	void	ErrorMsg(const char*e_strErrMsg1,const char*e_strErrMsg2){ ErrorMsg((char*)e_strErrMsg1,(char*)e_strErrMsg2); }
	void	ErrorMsg(std::wstring  e_strErrMsg1,std::wstring e_strErrMsg2){ ErrorMsg(e_strErrMsg1.c_str(),e_strErrMsg2.c_str()); }
	void	ErrorMsg(const wchar_t*  e_strErrMsg1,std::wstring e_strErrMsg2){ErrorMsg(e_strErrMsg1,e_strErrMsg2.c_str());}
	void	ErrorMsg(std::wstring e_strErrMsg1,const wchar_t*  e_strErrMsg2){ ErrorMsg(e_strErrMsg1.c_str(),e_strErrMsg2); }

	void	ErrorMsg(const wchar_t*e_strErrMsg1,const wchar_t*e_strErrMsg2)
	{
#if defined(WIN32)
		if( FATMING_CORE::cGameApp::m_sbShowErrorMsgBox )
		{
			try
			{
				std::wstring	l_str = e_strErrMsg1;
				std::wstring	l_str2 = e_strErrMsg2;
				if(MessageBox(0,e_strErrMsg1,e_strErrMsg2,MB_YESNO)==6)
				{
					assert(0);
					//exit(0);
				}
			}
			catch(const std::exception& ex)
			{
				const char*l_str = ex.what();
				int a=0;
			}
		}
		else
		if( FATMING_CORE::cGameApp::m_spstrErrorMsgString )
		{
			*FATMING_CORE::cGameApp::m_spstrErrorMsgString += e_strErrMsg1;
			*FATMING_CORE::cGameApp::m_spstrErrorMsgString += L"  ";
			*FATMING_CORE::cGameApp::m_spstrErrorMsgString += e_strErrMsg2;
			*FATMING_CORE::cGameApp::m_spstrErrorMsgString += L"\n";
			SystemErrorCheck();
		}
#else
		std::wstring	l_str = e_strErrMsg1;
		l_str += e_strErrMsg2;
		cGameApp::OutputDebugInfoString(l_str.c_str());
#endif
	}
	void	SystemErrorCheck()
	{
#ifdef _WIN32
#ifdef DEBUG
		static	bool	l_b = false;
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		unsigned long dw = GetLastError(); 
		if( dw && !l_b )
		{
			l_b = true;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, nullptr );

			// Display the error message and exit the process

			lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
				(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)L"Error")+40)*sizeof(TCHAR)); 
			StringCchPrintf((LPTSTR)lpDisplayBuf, 
				LocalSize(lpDisplayBuf),
				TEXT("%s failed with error %d: %s"), 
				L"Error", dw, lpMsgBuf);
			cGameApp::OutputDebugInfoString((wchar_t*)lpDisplayBuf);
			cGameApp::OutputDebugInfoString(L"\n");
			//MessageBox(nullptr, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

			LocalFree(lpMsgBuf);
			LocalFree(lpDisplayBuf);
		}
#endif
#endif
		return;
	}
	//--------------------------------------------------------------------------------------
	// Name: DebugSpew()
	// Desc: Prints formatted debug spew
	//--------------------------------------------------------------------------------------
	// variadic_macros.cpp
	//#include <stdio.h>
	//#define EMPTY
	//
	//#define CHECK1(x, ...) if (!(x)) { printf(__VA_ARGS__); }
	//#define CHECK2(x, ...) if ((x)) { printf(__VA_ARGS__); }
	//#define CHECK3(...) { printf(__VA_ARGS__); }
	//#define MACRO(s, ...) printf(s, __VA_ARGS__)
	//
	//int main() {
	//   CHECK1(0, "here %s %s %s", "are", "some", "varargs1(1)\n");
	//   CHECK1(1, "here %s %s %s", "are", "some", "varargs1(2)\n");   // won't print
	//
	//   CHECK2(0, "here %s %s %s", "are", "some", "varargs2(3)\n");   // won't print
	//   CHECK2(1, "here %s %s %s", "are", "some", "varargs2(4)\n");
	//
	//   // always invokes printf in the macro
	//   CHECK3("here %s %s %s", "are", "some", "varargs3(5)\n");
	//
	//   MACRO("hello, world\n");
	//   // MACRO("error\n", EMPTY);   would cause C2059
	//}
	// 
	//  Copy Code 
	//here are some varargs1(1)
	//here are some varargs2(4)
	//here are some varargs3(5)
	//hello, world
	// 
	void	ErrorMsgByFormat(char*e_strErrMsg1,...)
	{
		va_list pArgList;
		va_start( pArgList, e_strErrMsg1 );
		DebugSpewV( e_strErrMsg1, pArgList );
		va_end( pArgList );		
	}

	std::wstring			ComposeMsgByFormat(wchar_t*e_strErrMsg1,...)
	{
//#ifdef ANDROID
//		std::wstring	l_str;
//		assert(0);
//		return l_str;
//#endif
		std::wstring	l_str;
		va_list pArgList;
		va_start( pArgList, e_strErrMsg1 );
		wchar_t str[2048];
#ifdef _WIN32
		_vsnwprintf( str, sizeof(str), e_strErrMsg1, pArgList );
#else
		vswprintf( str, sizeof(str), e_strErrMsg1, pArgList );
#endif
		va_end( pArgList );
		l_str = str;
		return l_str;
	}
	std::string			ComposeMsgByFormat(char*e_strErrMsg1,...)
	{
		std::string	l_str;
		va_list pArgList;
		va_start( pArgList, e_strErrMsg1 );
		char str[2048];
		vsnprintf( str, sizeof(str), e_strErrMsg1, pArgList );
		va_end( pArgList );
		l_str = str;
		return l_str;	
	}
	void					ErrorMsgByFormat(wchar_t*e_strErrMsg1,...)
	{
		va_list pArgList;
		va_start( pArgList, e_strErrMsg1 );
		DebugSpewV( e_strErrMsg1, pArgList );
		va_end( pArgList );			
	}
//===============================
//
//===============================
	std::wstring					GetFileNameWithoutFullPath(std::wstring e_pString,bool e_bStripExtensionFileName)
	{
		return GetFileNameWithoutFullPath(e_pString.c_str(),e_bStripExtensionFileName);
	}
	std::string					GetFileNameWithoutFullPath(std::string e_pString,bool e_bStripExtensionFileName)
	{
		return GetFileNameWithoutFullPath(e_pString.c_str(),e_bStripExtensionFileName);
	}
	std::wstring	GetFileNameWithoutFullPath(const wchar_t*e_pString,bool e_bStripExtensionFileName)
	{
		wchar_t	l_temp[TEMP_SIZE];
		memset(l_temp,0,sizeof(l_temp));
		int l_iForStripExtensionFileName = 0;
		int	l_iStringLen = (int)wcslen(e_pString);
		for(int i=l_iStringLen-1;i!=0;--i)
		{
			if( e_bStripExtensionFileName )
			{
				if( e_pString[i] == L'.' )
					l_iForStripExtensionFileName = l_iStringLen-i;
			}

			if( e_pString[i] == L'\\' || e_pString[i] == L'/' )
			{
				int l_iNameLEngth = l_iStringLen-i-1-l_iForStripExtensionFileName;
				memcpy(l_temp,e_pString+i+1,sizeof(wchar_t)*l_iNameLEngth);
				l_temp[l_iNameLEngth] = L'\0';
				return l_temp;
			}
		}
		int l_iNameLEngth = l_iStringLen-l_iForStripExtensionFileName;
		memcpy(l_temp,e_pString,sizeof(wchar_t)*l_iNameLEngth);
		l_temp[l_iNameLEngth] = L'\0';
		std::wstring	l_strResult = l_temp;
				
		return l_strResult;
	}
//===============================
//
	//const char* file = "79979.fat";
	// const char* extension;
    //extension = strrchr( file, '.');

    //if (extension)
    //    extension++; //start looking after the .
    //else
    //    return false; //don't try to place guess the file type

    //int formatCount = sizeof(Model::formatTable) / sizeof(Model::FormatInfo);

    ////try to match by format first
    //for ( int ii = 0; ii < formatCount; ii++) {
    //    if ( ! strcasecmp( formatTable[ii].extension, extension)) {
    //        //extension matches, load it
    //        //return formatTable[ii].reader( file, *this);
    //    }
    //}
//===============================
	std::wstring		GetFileExtensionName(const wchar_t*e_pString)
	{
		std::wstring	l_strResult;
		wchar_t l_stemp[TEMP_SIZE];
		memset(l_stemp,0,sizeof(l_stemp));
		int	l_iExtensionNameSize = 0;
		int	l_iFileNameLength = (int)wcslen(e_pString)-1;
		//until find L'.'
		for( int i = l_iFileNameLength;i>0;--i )
		{
			if( e_pString[i] == L'.' )
			{//set all extension name
				for( int j = l_iExtensionNameSize;j>0 ;--j )
					l_stemp[l_iExtensionNameSize-j] = e_pString[l_iFileNameLength-j+1];
				l_stemp[l_iExtensionNameSize] = L'\0';
				l_strResult = l_stemp;
				return l_strResult;
			}
			l_iExtensionNameSize++;
		}
		return l_strResult;
	}

	std::string	GetFileNameWithoutFullPath(const char*e_pString,bool e_bStripExtensionFileName)
	{
		char	l_temp[TEMP_SIZE];
		memset(l_temp,0,sizeof(l_temp));
		int l_iForStripExtensionFileName = 0;
		int	l_iStringLen = (int)strlen(e_pString);
		for(int i=l_iStringLen-1;i!=0;--i)
		{
			if( e_bStripExtensionFileName )
			{
				if( e_pString[i] == '.' &&l_iForStripExtensionFileName==0)
					l_iForStripExtensionFileName = l_iStringLen-i;
			}

			if( e_pString[i] == '\\' || e_pString[i] == '/' )
			{
				int l_iNameLEngth = l_iStringLen-i-1-l_iForStripExtensionFileName;
				memcpy(l_temp,e_pString+i+1,sizeof(char)*l_iNameLEngth);
				l_temp[l_iNameLEngth] = '\0';
				return l_temp;
			}
		}
		int l_iNameLEngth = l_iStringLen-l_iForStripExtensionFileName;
		memcpy(l_temp,e_pString,sizeof(char)*l_iNameLEngth);
		l_temp[l_iNameLEngth] ='\0';
		std::string	l_strResult = l_temp;
		return l_strResult;
	}

	std::string	GetFileExtensionName(const char*e_pString)
	{
		std::string	l_strExtensionName;
		char l_stemp[TEMP_SIZE];
		memset(l_stemp,0,sizeof(l_stemp));
		int	l_iExtensionNameSize = 0;
		int	l_iFileNameLength = (int)strlen(e_pString)-1;
		//until find '.'
		for( int i = l_iFileNameLength;i>0;--i )
		{
			if( e_pString[i] == '.' )
			{//set all extension name
				for( int j = l_iExtensionNameSize;j>0 ;--j )
					l_stemp[l_iExtensionNameSize-j] = e_pString[l_iFileNameLength-j+1];
				l_stemp[l_iExtensionNameSize] = '\0';
				l_strExtensionName = l_stemp;
				return l_strExtensionName;
			}
			l_iExtensionNameSize++;
		}
		return l_strExtensionName;
	}

	std::string		ChangeFileExtensionName(const char*e_pFileNameString,const char*e_strNewExtensionName)
	{
		char l_stemp[TEMP_SIZE];
		int	l_iExtensionNameStartIndex = -1;
		int	l_iFileNameLength = (int)strlen(e_pFileNameString);
		memcpy(l_stemp,e_pFileNameString,sizeof(char)*l_iFileNameLength);
		//until find '.'
		for( int i = l_iFileNameLength-1;i>0;--i )
		{
			if( e_pFileNameString[i] == '.' )
			{
				l_iExtensionNameStartIndex = i;
				break;
			}
		}
		if( l_iExtensionNameStartIndex != -1 )
		{
			memcpy(l_stemp,e_pFileNameString,sizeof(char)*l_iExtensionNameStartIndex);
			l_stemp[l_iExtensionNameStartIndex] = 0;
		}
		std::string	l_strNewFileName = l_stemp;
		l_strNewFileName += ".";
		l_strNewFileName += e_strNewExtensionName;
		return l_strNewFileName;
	}

	std::string	GetDirectoryWithoutFileName(const char*e_str)
	{
		std::string	l_strResult;
		char	l_strTemp[TEMP_SIZE];
		int	l_i = (int)strlen(e_str);
		for(int i= l_i-1;i>0;--i)
		{
			if( e_str[i] == '/'||e_str[i] == '\\' )
			{
				sprintf(l_strTemp,"%s\n",e_str);
				l_strTemp[i+1] = '\0';
				l_strResult = l_strTemp;
				return l_strResult;
			}
		}
#ifdef	ANDROID
		l_strResult = "";
#else
		l_strResult = "./";
#endif
		return l_strResult;
	}
#ifndef IOS
	bool mkpath( std::string path )
	{
		const char*l_str = path.c_str();
		int	l_iIndexForDot = UT::FindChacterIndexFromBackBuffer(path.c_str(),'.');
		int	l_iIndexForSlash = UT::FindChacterIndexFromBackBuffer(path.c_str(),'/');
		if( l_iIndexForSlash == -1 )
			return false;
		if( l_iIndexForDot != -1 && l_iIndexForSlash != -1)
		{
			path = path.substr(0, path.find_last_of('/'));
			const char*l_str3 = path.c_str();
			int a=0;
		}
		bool bSuccess = false;
#ifdef WIN32
		int nRC = ::mkdir( path.c_str());
#else
		int nRC = ::mkdir( path.c_str(), 0775 );
#endif
		if( nRC == -1 )
		{
			switch( errno )
			{
				case ENOENT:
					//parent didn't exist, try to create it
					if( mkpath( path.substr(0, path.find_last_of('/')) ) )
						//Now, try to create again.
#ifdef WIN32
						bSuccess = 0 == ::mkdir( path.c_str());
#else
						bSuccess = 0 == mkdir( path.c_str(), 0775 );
#endif
					else
						bSuccess = false;
					break;
				case EEXIST:
					//Done!
					bSuccess = true;
					break;
				default:
					bSuccess = false;
					break;
			}
		}
		else
			bSuccess = true;
		return bSuccess;
	}
#endif
	std::string				GetTxtFileContent(const char* e_strFileName)
	{
		std::string	l_strContent;
		NvFile*l_pFile = MyFileOpen(e_strFileName,"r");
		if(!l_pFile)
		{
			l_strContent = e_strFileName;
			l_strContent += " (file is not exists)";
			return l_strContent;
		}
		long	l_uiFileSize = UT::GetFileSize(l_pFile);
		char*l_Temp = new char[l_uiFileSize+1];//1 MB
		int	l_iNumRead = NvFRead(l_Temp,1,l_uiFileSize,l_pFile);
		l_Temp[l_uiFileSize] = 0;
		l_strContent = l_Temp;
		delete l_Temp;
		NvFClose(l_pFile);
		return l_strContent;
	}

	bool					SaveTxtToFile(const char* e_strFileName,const char*e_strContent,int e_iByteCount)
	{
		std::string	l_strContent;
		//force to replace
		NvFile*l_pFile = MyFileOpen(e_strFileName,"w");
		if(!l_pFile)
		{
			return false;
		}
		long	l_uiFileSize = e_iByteCount;//strlen(e_strFileName);
		NvFWrite(e_strContent,sizeof(char),l_uiFileSize,l_pFile);
		NvFClose(l_pFile);
		return true;
	}

	NvFile*	MyFileOpen( const char* FileName,const char*e_strMode )
	{
		NvFile* fp = nullptr;
		bool	l_bWrite = false;
		for(size_t i=0;i<strlen(e_strMode);++i)
		{
			if( e_strMode[i] == 'w' )
			{
				l_bWrite = true;
			}
		}
#if defined(IOS)
		char	l_strFileName[MAX_PATH];
		GetAppleBundelResourcePathByObjectPath(FileName,l_strFileName);
		fp = fopen( l_strFileName, e_strMode );
#elif defined (ANDROID)
		//http://blog.sephiroth.it/2010/10/24/reading-resource-files-from-native-code/
		//http://androgeek.info/?p=275
		if( l_bWrite )
			mkpath( std::string(FileName) );
		fp = NvFOpen(FileName,e_strMode);
		if( !fp )
		{
			//try external sd card first
			if(cGameApp::m_spExternalSDDirectory)
			{
				std::string l_strFileName = *cGameApp::m_spExternalSDDirectory;
				l_strFileName += *cGameApp::m_psstrGameAppName;
				l_strFileName += "/";
				l_strFileName += FileName;
				if( l_bWrite )
					mkpath( l_strFileName );
				fp = NvFOpen(l_strFileName.c_str(),e_strMode);
				if( fp )
					return fp;
			}
			//try write file into
			std::string l_strFileName = "/sdcard/";
			l_strFileName += *cGameApp::m_psstrGameAppName;
			l_strFileName += "/";
			l_strFileName += FileName;
			//I am lazy to do recursive mkdir,so here may not working
			if( l_bWrite )
				mkpath(l_strFileName);
			fp = NvFOpen(l_strFileName.c_str(),e_strMode);
			if( !fp )
			{//write into internal memory
				l_strFileName = cGameApp::m_spInternalDirectory->c_str();
				l_strFileName += "/";	
				l_strFileName += *cGameApp::m_psstrGameAppName;
				l_strFileName += "/";
				l_strFileName += FileName;
				if( l_bWrite )
					mkpath(l_strFileName);
				fp = NvFOpen(l_strFileName.c_str(),e_strMode);
			}
#ifdef DEBUG
			const char*ll = l_strFileName.c_str();
			if(!fp)
			{
				std::string	l_str = ll;l_str += " openfile failed! ";
				cGameApp::OutputDebugInfoString(ll);
			}
			else
			{
				std::string	l_str = ll;l_str += " openfile ok !";
				cGameApp::OutputDebugInfoString(l_str.c_str());			
			}
#endif
		}
#else
		if( l_bWrite )
			mkpath( std::string(FileName) );
		fp = fopen( FileName, e_strMode );
#endif
		return fp;
	}


	bool		MyFileCopy( const char* e_strSrcFileName,const char* e_strDestFileName)
	{
		if(e_strSrcFileName)
		{
			NvFile*l_pSrcFile = UT::MyFileOpen(e_strSrcFileName,"rb");
			if( l_pSrcFile )
			{
				long	l_uiFileSize = UT::GetFileSize(l_pSrcFile);
				std::string	l_CopyImageFileName = e_strDestFileName;
				NvFile*l_pCopyImageFile = UT::MyFileOpen(l_CopyImageFileName.c_str(),"wb");
				if( l_pCopyImageFile )
				{
					int	l_iBuffer = 1024*1024;
					if( l_iBuffer > l_uiFileSize)
						l_iBuffer = l_uiFileSize;
					char*l_Temp = new char[l_iBuffer];//1 MB
					int	l_iRestBuffer = l_uiFileSize;
					while( l_iRestBuffer )
					{
						int	l_iNumRead = NvFRead(l_Temp,1,l_iBuffer,l_pSrcFile);
						if( l_iNumRead <= 0 )//all read done
							break;
						l_iRestBuffer -= l_iNumRead;
						NvFWrite(l_Temp,1,l_iNumRead,l_pCopyImageFile);					
					}
					NvFClose(l_pCopyImageFile);
					delete l_Temp;
				}
				NvFClose(l_pSrcFile);
			}
		}
		return true;
	}
	bool IsFileExists( const char* FileName )
	{
		
		NvFile*l_pFile = MyFileOpen(FileName,"r");
		if( l_pFile )
		{
			NvFClose(l_pFile);
			return true;
		}
		return false;
	}

	bool	MyRemoveFile( const char* FileName )
	{
		int	l_iResult = remove(FileName);
		return l_iResult==0?true:false;
	}

#ifdef WIN32
	int DeleteDirectory(const std::wstring &refcstrRootDirectory,bool  bDeleteSubdirectories)
	{
	  bool            bSubdirectory = false;       // Flag, indicating whether
												   // subdirectories have been found
	  HANDLE          hFile;                       // Handle to directory
	  std::wstring     strFilePath;                 // Filepath
	  std::wstring     strPattern;                  // Pattern
	  WIN32_FIND_DATA FileInformation;             // File information


	  strPattern = refcstrRootDirectory + L"\\*.*";
	  hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
	  if(hFile != INVALID_HANDLE_VALUE)
	  {
		do
		{
		  if(FileInformation.cFileName[0] != '.')
		  {
			strFilePath.erase();
			strFilePath = refcstrRootDirectory + L"\\" + FileInformation.cFileName;

			if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			  if(bDeleteSubdirectories)
			  {
				// Delete subdirectory
				int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
				if(iRC)
				  return iRC;
			  }
			  else
				bSubdirectory = true;
			}
			else
			{
			  // Set file attributes
			  if(::SetFileAttributes(strFilePath.c_str(),
									 FILE_ATTRIBUTE_NORMAL) == FALSE)
				return ::GetLastError();

			  // Delete file
			  if(::DeleteFile(strFilePath.c_str()) == FALSE)
				return ::GetLastError();
			}
		  }
		} while(::FindNextFile(hFile, &FileInformation) == TRUE);

		// Close handle
		::FindClose(hFile);

		unsigned long dwError = ::GetLastError();
		if(dwError != ERROR_NO_MORE_FILES)
		  return dwError;
		else
		{
		  if(!bSubdirectory)
		  {
			// Set directory attributes
			if(::SetFileAttributes(refcstrRootDirectory.c_str(),
								   FILE_ATTRIBUTE_NORMAL) == FALSE)
			  return ::GetLastError();

			// Delete directory
			if(::RemoveDirectory(refcstrRootDirectory.c_str()) == FALSE)
			  return ::GetLastError();
		  }
		}
	  }

	  return 0;
	}
#endif
	bool	MyRemoveDirectory( const char* e_strDirectoryName )
	{
#ifdef WIN32
		std::wstring	l_str = UT::CharToWchar(e_strDirectoryName);
		return DeleteDirectory(l_str,true)?true:false;
#else
		return false;
#endif
	}

	bool	MyCreateDirectory( const char* e_strDirectoryName )
	{
#ifdef IOS
		return mkpath_np(e_strDirectoryName, 0775);
#else
		return mkpath(std::string(e_strDirectoryName));
#endif
//#ifdef WIN32
//		return _mkdir(e_strDirectoryName) > 0;
//#else
//		mkdir(e_strDirectoryName,0770);
//#endif
//		return false;
	}

	Vector4	ViewRectToOpenGLScissor(Vector4 e_v2DViewRange,Vector4 e_vViewPort,Vector2 e_vGameResolution,eDeviceDirection e_eDeviceDirection)
	{
		Vector2 l_vViewSize(e_vViewPort.Width(),e_vViewPort.Height());
		Vector2	l_vScale(1.f,1.f);
		switch(e_eDeviceDirection)
		{
			case eDD_PORTRAIT:
			case eDD_UPSIDE_DOWN:
				l_vScale = Vector2(e_vGameResolution.x/l_vViewSize.x,e_vGameResolution.y/l_vViewSize.y);
				break;
			case eDD_LANDSCAPE_LEFT:
			case eDD_LANDSCAPE_RIGHT:
				l_vScale = Vector2(e_vGameResolution.x/l_vViewSize.y,e_vGameResolution.y/l_vViewSize.x);
				break;
		}
		e_v2DViewRange.x /= l_vScale.x;
		e_v2DViewRange.y /= l_vScale.y;
		e_v2DViewRange.z /= l_vScale.x;
		e_v2DViewRange.w /= l_vScale.y;
		//add offset
		if(cGameApp::m_svViewPortSize.x != 0)
		{
			e_v2DViewRange.x += cGameApp::m_svViewPortSize.x;
			e_v2DViewRange.z += cGameApp::m_svViewPortSize.x;
		}
		//add offset
		if(cGameApp::m_svViewPortSize.y != 0)
		{
			e_v2DViewRange.y -= cGameApp::m_svViewPortSize.y;
			e_v2DViewRange.w -= cGameApp::m_svViewPortSize.y;
		}
		Vector4	l_vScissor;
		float	l_fWidth = (e_v2DViewRange.z-e_v2DViewRange.x);
		float	l_fHeight = (e_v2DViewRange.w-e_v2DViewRange.y);
		switch(e_eDeviceDirection)
		{
			case eDD_PORTRAIT:
				l_vScissor.x = e_v2DViewRange.x;
				l_vScissor.y = l_vViewSize.y-e_v2DViewRange.y-l_fHeight;
				break;
			case eDD_UPSIDE_DOWN:
				l_vScissor.x = l_vViewSize.x-(e_v2DViewRange.x+l_fWidth);
				l_vScissor.y = e_v2DViewRange.y;
				break;
			case eDD_LANDSCAPE_LEFT:
			{
				l_vScissor.x = e_v2DViewRange.y;
				l_vScissor.y = e_v2DViewRange.x;
			}
				break;
			case eDD_LANDSCAPE_RIGHT:
				l_vScissor.x = l_vViewSize.x-e_v2DViewRange.w;
				l_vScissor.y = l_vViewSize.y-e_v2DViewRange.z;
				break;
		}
		switch(e_eDeviceDirection)
		{
			case eDD_PORTRAIT:
			case eDD_UPSIDE_DOWN:
				l_vScissor.z = l_fWidth;
				l_vScissor.w = l_fHeight;
				break;
			case eDD_LANDSCAPE_LEFT:
			case eDD_LANDSCAPE_RIGHT:
				l_vScissor.w = l_fWidth;
				l_vScissor.z = l_fHeight;
				break;
		}
		return l_vScissor;
	}

	Vector4	ViewRectToOpenGLScissor(Vector4 e_v2DViewRange)
	{
		return ViewRectToOpenGLScissor(e_v2DViewRange,cGameApp::m_svViewPortSize,FATMING_CORE::cGameApp::m_svGameResolution,FATMING_CORE::cGameApp::m_seDeviceDirection);
	}
//==================math
	//101~106	A~F
	//48~57		0~9
	//97~102	a~f
	//http://social.msdn.microsoft.com/forums/en-US/vcgeneral/thread/d56a52f0-f9e2-45e5-be16-7e5ebcf7169e
	char	CharToHex(char e_c)
	{
		char	l_cValue = -1;
		if( e_c >='0' && e_c<= '9' )
		{
			l_cValue = e_c-'0';
		}
		else
		if( e_c >='a' && e_c<= 'f' )
		{
			l_cValue = e_c-'a'+10;
		}
		else
		if( e_c >='A' && e_c<= 'F' )
		{
			l_cValue = e_c-'A'+10;
		}
		else
		{
			assert(0);
			return -1;
		}
		return l_cValue;
	}

	int		HexadecimalStringToDecimal(char*e_strValue)
	{
		int	l_iNum = (int)strlen(e_strValue);
		if( l_iNum <= 0  )
			return 0;
		int	l_iValue = 0;
		int	l_iHexValue = 16;
		char	l_cOrder[8] = {0,0,0,0,0,0,0,0};//0x00000000
		for( int i=0;i<l_iNum;++i )//set order from units start
			l_cOrder[l_iNum-i-1] = e_strValue[i];

		l_iValue += CharToHex(l_cOrder[0]);
		for( int i = 1; i < l_iNum ; ++i )
		{
			l_iValue += CharToHex(l_cOrder[i])*l_iHexValue;
			l_iHexValue*=16;
		}
		return l_iValue;
	}
	//hex copy to string
	//char Address[100];
	//unsigned int Add = 0x000CA;
	//sprintf(Address, "%05X", Add);
	int		DecimalStringToHexadcimal(char*e_strValue)
	{
		return strtol(e_strValue,nullptr,16);
	}
    int	FindChacterIndexFromStartBuffer(const wchar_t*e_String,wchar_t e_char,int e_iLookForIndex)
    {
		int j = (int)wcslen(e_String);
		//int l_iNum = 0;
		for( int i=0;i<j;++i )
		{
			if( e_String[i] == e_char )
			{
			    //fuck.....what is this for.....
			    //if( e_iLookForIndex == e_iLookForIndex )
				    return i;
				//++l_iNum;
			}
		}
		return -1;
    }
	int	FindChacterIndexFromBackBuffer(const wchar_t*e_String,wchar_t e_char)
	{
		int	l_iLastSlash = -1;
		int j = (int)wcslen(e_String)-1;
		for( ;j>0;--j )
		{
			if( e_String[j] == e_char )
			{
				l_iLastSlash = j;
				return l_iLastSlash;
			}
		}
		return l_iLastSlash;	
	}

	int	FindChacterIndexFromBackBuffer(const char*e_String,char e_char)
	{
		int	l_iLastSlash = -1;
		int j = (int)strlen(e_String)-1;
		for( ;j>0;--j )
		{
			if( e_String[j] == e_char )
			{
				l_iLastSlash = j;
				return l_iLastSlash;
			}
		}
		return l_iLastSlash;
	}

	long				GetFileSize(NvFile*e_pFile)
	{
		if( e_pFile == nullptr )
			return 0;
		long l_lFilesize = 0;
#ifdef ANDROID
		l_lFilesize = NvFSize(e_pFile);		
#else
		//store curent position
		long l_lCurrentPosition = NvFTell( e_pFile );
		//go to end
		int	l_i = NvFSeek(e_pFile,0,SEEK_END);
		assert(l_i == 0&&"file search size error");
		l_lFilesize = NvFTell( e_pFile );
		//restore position
		NvFSeek(e_pFile,l_lCurrentPosition,SEEK_SET);
#endif
		return l_lFilesize;
	}


	std::wstring			GetSystemTimeForFile(bool e_bCN)
	{
		std::wstring	l_str;
#ifdef WIN32
		SYSTEMTIME l_st;
//		GetSystemTime(&l_st);
		GetLocalTime(&l_st);
		if( e_bCN )
		{
			l_str += ValueToStringW(l_st.wYear);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wMonth);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wDay);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wHour);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wMinute);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wSecond);
		}
		else
		{
			l_str += ValueToStringW(l_st.wMonth);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wDay);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wYear);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wHour);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wMinute);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wSecond);	
		}
#endif	
		return l_str;
	}

	std::wstring			GetSystemTime(bool e_bCN)
	{
		std::wstring	l_str;
#ifdef WIN32
		SYSTEMTIME l_st;
//		GetSystemTime(&l_st);
		GetLocalTime(&l_st);
		if( e_bCN )
		{
			l_str += ValueToStringW(l_st.wYear);
			l_str += L".";
			l_str += ValueToStringW(l_st.wMonth);
			l_str += L".";
			l_str += ValueToStringW(l_st.wDay);
			l_str += L".";
			l_str += ValueToStringW(l_st.wHour);
			l_str += L":";
			l_str += ValueToStringW(l_st.wMinute);
			l_str += L":";
			l_str += ValueToStringW(l_st.wSecond);
		}
		else
		{
			l_str += ValueToStringW(l_st.wMonth);
			l_str += L".";
			l_str += ValueToStringW(l_st.wDay);
			l_str += L".";
			l_str += ValueToStringW(l_st.wYear);
			l_str += L".";
			l_str += ValueToStringW(l_st.wHour);
			l_str += L":";
			l_str += ValueToStringW(l_st.wMinute);
			l_str += L":";
			l_str += ValueToStringW(l_st.wSecond);	
		}
#endif
		return l_str;
	}

	void	WcharToChar(const wchar_t *e_strSrc,char*e_strDest)
	{
		assert(e_strDest&&"dest can't be nullptr");
		assert(e_strSrc&&"the wchar_t string is nullptr");
        memset(e_strDest,0,sizeof(char)*MAX_PATH);
		WCHAR_TO_CHAR(e_strSrc,e_strDest);
	}
	void	WcharToChar(wchar_t *e_strSrc,char*e_strDest)
	{
		assert(e_strDest&&"dest can't be nullptr");
		assert(e_strSrc&&"the wchar_t string is nullptr");
        memset(e_strDest,0,sizeof(char)*MAX_PATH);
		WCHAR_TO_CHAR(e_strSrc,e_strDest);
	}

	std::string	WcharToChar(wchar_t *e_strWchar)
	{
		return WcharToChar((const wchar_t*)e_strWchar);
	}

	std::string	WcharToChar(const wchar_t *e_strWchar)
	{
		std::string		l_strResult;
		std::wstring	l_strForCopy = e_strWchar;
		l_strResult.assign(l_strForCopy.begin(),l_strForCopy.end());
		return l_strResult;
	}

	std::string	WcharToChar(std::wstring e_strWchar)
	{
		return WcharToChar(e_strWchar.c_str());
	}

	void	CharToWchar(const char *e_strSrc,wchar_t*e_strDest)
	{
		assert(e_strDest&&"dest can't be nullptr");
		assert(e_strSrc&&"the wchar_t string is nullptr");
        memset(e_strDest,0,sizeof(wchar_t)*MAX_PATH);
		CHAR_TO_WCHAR(e_strSrc,e_strDest);
	}

	void	CharToWchar(char *e_strSrc,wchar_t*e_strDest)
	{
		assert(e_strDest&&"dest can't be nullptr");
		assert(e_strSrc&&"the wchar_t string is nullptr");
        memset(e_strDest,0,sizeof(wchar_t)*MAX_PATH);
		CHAR_TO_WCHAR(e_strSrc,e_strDest);
	}
	std::wstring	CharToWchar(char *e_strChar)
	{
		return CharToWchar((const char *)e_strChar);
	}
	std::wstring	CharToWchar(const char *e_strChar)
	{
		std::wstring	l_strResult;
		std::string		l_strForCopy = e_strChar;
		l_strResult.assign(l_strForCopy.begin(),l_strForCopy.end());
		return l_strResult;
		//if here occured crush,in the dot net that's because fucking MS
		//using critical section to lock
		//please call this after whole windos form is showed
		//or using DNCT::OpenfileGet Name.
	}

	std::wstring	CharToWchar(std::string	e_strChar)
	{
		return CharToWchar(e_strChar.c_str());
	}

	int		GetLoopIndex(int e_iIndex,int e_iTotalCount)
	{
		if( e_iTotalCount == 0 )
			return -1; 
		if( e_iIndex < 0 )
		{
			int	l_iIndex = e_iIndex%e_iTotalCount;
			return e_iTotalCount+l_iIndex;
		}
		if( e_iIndex >= e_iTotalCount )
		{
			int	l_iIndex = e_iIndex%e_iTotalCount;
			return l_iIndex;
		}
		return e_iIndex;
	}

	void			itoa64(int64 val,char*e_strBuffer, int base)
	{
		char buf[64] = {0};
		memset(buf,0,sizeof(char)*64);
		if( val!=0 )
		{
			int i = 62;//last one is 0,value start from last 2.
			for(; val && i ; --i, val /= base)
				buf[i] = "0123456789abcdef"[val % base];
			memcpy(e_strBuffer,&buf[i+1],sizeof(char)*64-i-1);
		}
		else
		{
			e_strBuffer[0] = '0';
			e_strBuffer[1] = '\0';
		}	
	}


	POINT			GetScreenResolution()
	{
#ifdef WIN32
		POINT	l_ScreenResolution = {GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN)};
		return l_ScreenResolution;
#else
		POINT	l_ScreenResolution = {(int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y};
		return l_ScreenResolution;
#endif

	}

//end namespace UT
}
