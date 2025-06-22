#include "StringToStructure.h"
#include "assert.h"
#include "Utility.h"
#include "Log/FMLog.h"
#include "../Common/CommonApp.h"
//#include "../GameplayUT/GameApp.h"
//#include "../GameplayUT/Log/FMLog.h"
#include <errno.h>
#include <locale>
//#include <locale>
#ifdef WIN32
#include <direct.h>
#endif

#if defined(IOS)
#include <mach/mach_time.h>
#include <sys/time.h>
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIImage.h>
#include <stdio.h>
#include <sys/stat.h>
#elif defined(WIN32)
#include "strsafe.h"
#include <time.h>
//========for WcharToChar(const wchar_t* e_strString) and CharToWchar
//#include <locale>
//#include <utility>
//#include <codecvt>
//========
#pragma warning( disable : 4793 )
#pragma warning( disable : 4995 )
//#include "../../Include/glExt.h"
//#define WGL_SAMPLE_BUFFERS_ARB	0x2041
//#define WGL_SAMPLES_ARB		0x2042
#elif defined(ANDROID)
#include "jni.h"
#include <sys/stat.h>
#include <time.h>
//#include <locale>
#elif defined(WASM)
#include <stdarg.h>
#include <sys/stat.h>
#include <locale>
#elif defined(LINUX)
#include <sys/stat.h>
#endif

#include <sstream>
#include <regex>

#ifdef UWP

	using namespace Windows::UI::Popups;

#endif

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace UT
{
	//--------------------------------------------------------------------------------------
	// Name: DebugSpewV()
	// Desc: Internal helper function
	//--------------------------------------------------------------------------------------
	void DebugSpewV( const char* strFormat, va_list pArgList )
	{
		char str[2048];
		// Use the secure CRT to avoid buffer overruns. Specify a count of
		// _TRUNCATE so that too long strings will be silently truncated
		// rather than triggering an error.
#ifndef IOS
		vsnprintf( str, sizeof(str), strFormat, pArgList );
		FMLog::LogWithFlag(str, CORE_LOG_FLAG);
#endif
	}
	void DebugSpewV( const wchar_t* strFormat, va_list pArgList )
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
		FMLog::LogWithFlag(str, CORE_LOG_FLAG);
	}

	void	ErrorMsg(const char*e_strErrMsg1, const char*e_strErrMsg2)
	{
		FMLog::Log(e_strErrMsg1,true);
		FMLog::Log(e_strErrMsg2, true);
#if defined(WIN32)
		std::wstring	l_temp1 = FATMING_CORE::ValueToStringW(e_strErrMsg1);
		std::wstring	l_temp2 = FATMING_CORE::ValueToStringW(e_strErrMsg2);
		ErrorMsg(l_temp1.c_str(),l_temp2.c_str());
#else
		//printf("%s, %s\n\0",e_strErrMsg1,e_strErrMsg2);
		std::string	l_strMessage = e_strErrMsg1;
		l_strMessage += "___";
		l_strMessage += e_strErrMsg2;
		FMLog::LogWithFlag(l_strMessage.c_str(), CORE_LOG_FLAG);
#endif
	}
	eDataType	GetDataType(const wchar_t*e_str)
	{
		return GetDataType(FATMING_CORE::ValueToString(e_str).c_str());
		//if( !e_str )
		//	return eDT_MAX;
		//if( !wcscmp(e_str,L"int") )
		//	return eDT_INT;
		//if( !wcscmp(e_str,L"float") )
		//	return eDT_FLOAT;
		//if( !wcscmp(e_str,L"byte") )
		//	return eDT_BYTE;
		//if( !wcscmp(e_str,L"double") )
		//	return eDT_DOUBLE;
		//if( !wcscmp(e_str,L"wstring") )
		//	return eDT_WSTRING;
		//if( !wcscmp(e_str,L"string") )
		//	return eDT_STRING;
		//if( !wcscmp(e_str,L"void") )
		//	return eDT_VOID;
		//if( !wcscmp(e_str,L"Point") )
		//	return eDT_POINT;
		//if( !wcscmp(e_str,L"Vector2") )
		//	return eDT_VECTOR2;
		//if( !wcscmp(e_str,L"Vector3") )
		//	return eDT_VECTOR3;
		//if( !wcscmp(e_str,L"Vector4") )
		//	return eDT_VECTOR4;
		//if( !wcscmp(e_str,L"int64") )
		//	return eDT_INT64;
		//if( !wcscmp(e_str,L"uint64") )
		//	return eDT_UINT64;
		//return eDT_MAX;
	}
	eDataType	GetDataType(const char*e_str)
	{
		if (!strcmp(e_str, "char") || !strcmp(e_str, "byte"))
			return eDT_BYTE;
		if (!strcmp(e_str, "short"))
			return eDT_SHORT;
		if (!strcmp(e_str, "float"))
			return eDT_FLOAT;
		if (!strcmp(e_str, "double"))
			return eDT_DOUBLE;
		if( !strcmp(e_str,"int") )
			return eDT_INT;
		if (!strcmp(e_str, "uint"))
			return eDT_UINT;
		if (!strcmp(e_str, "string"))
			return eDT_STRING;
		if( !strcmp(e_str,"wstring") )
			return eDT_WSTRING;
		if (!strcmp(e_str, "vector2"))
			return eDT_VECTOR2;
		if (!strcmp(e_str, "vector3"))
			return eDT_VECTOR3;
		if (!strcmp(e_str, "vector4"))
			return eDT_VECTOR4;
		if (!strcmp(e_str, "point"))
			return eDT_POINT;
		if( !strcmp(e_str,"void") )
			return eDT_VOID;
		if( !strcmp(e_str,"int64") )
			return eDT_INT64;
		if( !strcmp(e_str,"uint64") )
			return eDT_UINT64;
		return eDT_MAX;
	}

	void	ErrorMsg(std::wstring  e_strErrMsg1,std::wstring e_strErrMsg2){ ErrorMsg(e_strErrMsg1.c_str(),e_strErrMsg2.c_str()); }
	void	ErrorMsg(const wchar_t*  e_strErrMsg1,std::wstring e_strErrMsg2){ErrorMsg(e_strErrMsg1,e_strErrMsg2.c_str());}
	void	ErrorMsg(std::wstring e_strErrMsg1,const wchar_t*  e_strErrMsg2){ ErrorMsg(e_strErrMsg1.c_str(),e_strErrMsg2); }

	void	ErrorMsg(const wchar_t*e_strErrMsg1,const wchar_t*e_strErrMsg2)
	{
#if defined(WIN32)
		if( FMLog::g_siLogShowErrorType == 1)
		{
#ifndef UWP
			try
			{
				static bool l_bMessageBoxLeave = false;
				if (!l_bMessageBoxLeave)
				{
					std::wstring	l_str = e_strErrMsg1;
					std::wstring	l_str2 = e_strErrMsg2;
					if (MessageBox(0, e_strErrMsg1, e_strErrMsg2, MB_YESNO) == 6)
					{
						assert(0);
						//exit(0);
					}
					l_bMessageBoxLeave = true;
				}
			}
			catch(const std::exception& ex)
			{
				const char*l_str = ex.what();
				int a=0;
			}
#else
			// Create the message dialog and set its content
			MessageDialog^ msg = ref new MessageDialog("No internet connection has been found.");

			// Add commands and set their callbacks.
			// Both commands use the same callback function instead of inline event handlers.
			UICommand^ continueCommand = ref new UICommand(
				"Try again",
				ref new UICommandInvokedHandler([](Windows::UI::Popups::IUICommand^ command) {}));
			UICommand^ upgradeCommand = ref new UICommand(
				"Close",
				ref new UICommandInvokedHandler([](Windows::UI::Popups::IUICommand^ command) {}));

			// Add the commands to the dialog
			msg->Commands->Append(continueCommand);
			msg->Commands->Append(upgradeCommand);

			// Set the command that will be invoked by default
			msg->DefaultCommandIndex = 0;

			// Set the command to be invoked when escape is pressed
			msg->CancelCommandIndex = 1;

			// Show the message dialog
			msg->ShowAsync();
#endif
		}
		if (FMLog::g_siLogShowErrorType == 2)
		{
			std::wstring	l_str = L"Error:";
			l_str += e_strErrMsg1;
			l_str += L" : ";
			l_str += e_strErrMsg2;
			FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		}
		else
		if( FMLog::g_spstrLogErrorMsgString )
		{
			*FMLog::g_spstrLogErrorMsgString += e_strErrMsg1;
			*FMLog::g_spstrLogErrorMsgString += L"  ";
			*FMLog::g_spstrLogErrorMsgString += e_strErrMsg2;
			*FMLog::g_spstrLogErrorMsgString += L"\n";
			SystemErrorCheck();
		}
#else
		std::wstring	l_str = e_strErrMsg1;
		l_str += e_strErrMsg2;
		FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
#endif
	}
	void	SystemErrorCheck()
	{
#ifdef _WIN32
#ifdef DEBUG
		//static	bool	l_b = false;
		//LPVOID lpMsgBuf;
		//LPVOID lpDisplayBuf;
		//unsigned long dw = GetLastError(); 
		//if( dw && !l_b )
		//{
		//	l_b = true;
		//	FormatMessage(
		//		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		//		FORMAT_MESSAGE_FROM_SYSTEM |
		//		FORMAT_MESSAGE_IGNORE_INSERTS,
		//		nullptr,
		//		dw,
		//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		//		(LPTSTR) &lpMsgBuf,
		//		0, nullptr );

		//	// Display the error message and exit the process

		//	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		//		(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)L"Error")+40)*sizeof(TCHAR)); 
		//	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		//		LocalSize(lpDisplayBuf),
		//		TEXT("%ls failed with error %d: %ls"), 
		//		L"Error", dw, lpMsgBuf);
		//	FMLog::LogWithFlag((const wchar_t*)lpDisplayBuf, CORE_LOG_FLAG);
		//	//MessageBox(nullptr, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

		//	LocalFree(lpMsgBuf);
		//	LocalFree(lpDisplayBuf);
		//}
#endif
#endif
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
	//https://stackoverflow.com/questions/15836392/c-passing-variable-number-of-arguments-from-one-function-to-another
	//void modifyAndPrintMessage(char* message, ...)
	//{
	//	// do somehthing custom

	//	va_list args;
	//	va_start(args, message);

	//	vprintf(newMessage, args);

	//	va_end(args);
	//}
	//variable arguments
	void	ErrorMsgByFormat(const char*e_strErrMsg1,...)
	{
		std::string	l_str;
		char str[2048];
		va_list pArgList;
		va_start(pArgList, e_strErrMsg1);
		vsnprintf(str, sizeof(str), e_strErrMsg1, pArgList);
		va_end(pArgList);
		l_str = str;
		UT::ErrorMsg(l_str.c_str(), "Error");
	}

	std::wstring			ComposeMsgByFormat(const wchar_t*e_strErrMsg1,...)
	{
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
	std::string			ComposeMsgByFormat(const char*e_strErrMsg1,...)
	{
		std::string	l_str;
		va_list pArgList;
		va_start( pArgList, e_strErrMsg1 );
		char str[20480];
		vsnprintf( str, sizeof(str), e_strErrMsg1, pArgList );
		va_end( pArgList );
		l_str = str;
		return l_str;	
	}
	void					ErrorMsgByFormat(const wchar_t*e_strErrMsg1,...)
	{
		std::wstring	l_str;
		va_list pArgList;
		va_start(pArgList, e_strErrMsg1);
		wchar_t str[2048];
#ifdef _WIN32
		_vsnwprintf(str, sizeof(str), e_strErrMsg1, pArgList);
#else
		vswprintf(str, sizeof(str), e_strErrMsg1, pArgList);
#endif
		va_end(pArgList);
		l_str = str;
		UT::ErrorMsg(l_str.c_str(), L"Error");
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
		memset(l_temp, 0, sizeof(l_temp));
		if (e_pString)
		{
			int l_iForStripExtensionFileName = 0;
			int	l_iStringLen = (int)wcslen(e_pString);
			for (int i = l_iStringLen - 1; i != 0; --i)
			{
				if (e_bStripExtensionFileName)
				{
					if (e_pString[i] == L'.')
						l_iForStripExtensionFileName = l_iStringLen - i;
				}

				if (e_pString[i] == L'\\' || e_pString[i] == L'/')
				{
					int l_iNameLEngth = l_iStringLen - i - 1 - l_iForStripExtensionFileName;
					memcpy(l_temp, &e_pString[i + 1], sizeof(wchar_t)*l_iNameLEngth);
					l_temp[l_iNameLEngth] = L'\0';
					return l_temp;
				}
			}
			int l_iNameLEngth = l_iStringLen - l_iForStripExtensionFileName;
			memcpy(l_temp, e_pString, sizeof(wchar_t)*l_iNameLEngth);
			l_temp[l_iNameLEngth] = L'\0';
		}
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
		std::string	l_strResult;
		if (!e_pString || strlen(e_pString) == 0)
		{
			return l_strResult;
		}
		char	l_temp[TEMP_SIZE];
		memset(l_temp,0,sizeof(l_temp));
		if (e_pString)
		{
			int l_iForStripExtensionFileName = 0;
			int	l_iStringLen = (int)strlen(e_pString);
			for (int i = l_iStringLen - 1; i != 0; --i)
			{
				if (e_bStripExtensionFileName)
				{
					if (e_pString[i] == '.' &&l_iForStripExtensionFileName == 0)
						l_iForStripExtensionFileName = l_iStringLen - i;
				}

				if (e_pString[i] == '\\' || e_pString[i] == '/')
				{
					int l_iNameLEngth = l_iStringLen - i - 1 - l_iForStripExtensionFileName;
					memcpy(l_temp, &e_pString[i + 1], sizeof(char)*l_iNameLEngth);
					l_temp[l_iNameLEngth] = '\0';
					l_strResult = l_temp;
					return l_strResult;
				}
			}
			int l_iNameLEngth = l_iStringLen - l_iForStripExtensionFileName;
			memcpy(l_temp, e_pString, sizeof(char)*l_iNameLEngth);
			l_temp[l_iNameLEngth] = '\0';
		}
		l_strResult = l_temp;
		return l_strResult;
	}

	std::string		ConvertFileNameWithoutFullPath(const char*e_strFileName, char e_cSlashReplaceCharacter)
	{
		std::string l_strResult = e_strFileName;
		size_t	l_iFileNameLength = strlen(e_strFileName);
		//skip the file name start with ./
		for (size_t i = 2; i < l_iFileNameLength; ++i)
		{
			if (e_strFileName[i] == '/' || e_strFileName[i] == '\\')
			{
				l_strResult[i] = e_cSlashReplaceCharacter;
			}
		}
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

	bool					IsStringContain(const char*e_strSource,const char*e_strCompare)
	{
		std::string l_str = e_strSource;
		if(l_str.find(e_strCompare) != std::string::npos)
			return true;
		return false;
	}

	bool					IsStringContain(const wchar_t*e_strSource,const wchar_t*e_strCompare)
	{
		std::wstring l_str = e_strSource;
		if(l_str.find(e_strCompare) != std::wstring::npos)
			return true;
		return false;
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

	std::wstring			GetDirectoryWithoutFileName(const wchar_t* e_str)
	{
		std::wstring l_FileName = e_str;
		size_t lastSlash;
		if ((lastSlash = l_FileName.rfind(L'/')) != std::wstring::npos)
			return l_FileName.substr(0, lastSlash + 1);
		else if ((lastSlash = l_FileName.rfind(L'\\')) != std::wstring::npos)
			return l_FileName.substr(0, lastSlash + 1);
		else
			return L"";
	}

	std::string AddExtenStringForFileName(const char * e_str, const char * e_strExtendString)
	{
		auto l_strExtensionFileName = GetFileExtensionName(e_str);
		auto l_strFileName = GetDirectoryWithoutFileName(e_str);
		l_strFileName += GetFileNameWithoutFullPath(e_str);
		l_strFileName += e_strExtendString;
		l_strFileName += ".";
		l_strFileName += l_strExtensionFileName;
		return l_strFileName;
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

	char*					GetFileContent(const char* e_strFileName, int&e_iFileLength, const char* e_strMode)
	{
 		char*l_pTemp = nullptr;
		NvFile*l_pFile = MyFileOpen(e_strFileName, e_strMode);
		if (!l_pFile)
		{
			return nullptr;
		}
		int	l_iFileSize = UT::GetFileSize(l_pFile);
		if (l_iFileSize != 0)
		{
			int l_ExtraceEndCharacterLength = 1;
			l_pTemp = new char[l_iFileSize+ l_ExtraceEndCharacterLength];
			size_t	l_iNumRead = NvFRead(l_pTemp, 1, l_iFileSize, l_pFile);
			l_pTemp[l_iNumRead] = 0;
			e_iFileLength = l_iFileSize+ l_ExtraceEndCharacterLength;
		}
		NvFClose(l_pFile);
		return l_pTemp;
	}
	std::string				GetTxtFileContent(const char* e_strFileName)
	{
		std::string	l_strContent = e_strFileName;
		int l_iFileLength = 0;
		auto l_strFileContent = GetFileContent(e_strFileName, l_iFileLength, "r");
		if (l_strFileContent)
		{
			l_strContent = l_strFileContent;
			delete l_strFileContent;
		}
		else
		{
			l_strContent = e_strFileName;
			l_strContent += " (file is not exists)";
		}
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
		//auto l_uiFileSize2 = strlen(e_strContent);
		auto l_iLength = NvFWrite(e_strContent,sizeof(char),l_uiFileSize,l_pFile);
		NvFClose(l_pFile);
		return true;
	}

	bool		SaveStringToFile(const std::string& filePath, const std::string& content)
	{
		//try
		//{
			// Ensure the directory exists
			fs::path path(filePath);
			if (path.has_parent_path())
			{
				fs::create_directories(path.parent_path());
			}

			// Open file for writing
			std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
			if (!outFile)
			{
				std::cerr << "Error: Could not open file for writing: " << filePath << '\n';
				return false;
			}

			// Write content to the file
			outFile << content;
			outFile.close();

			if (!outFile.good())
			{
				std::cerr << "Error: Failed to write content to file: " << filePath << '\n';
				return false;
			}

			return true; // Success
		//}
		//catch (const std::exception& e)
		//{
		//	std::cerr << "Exception: " << e.what() << '\n';
		//	return false;
		//}
	}
#ifdef IOS
	//because iphone using bundle resource,so the resource store in its specific path.
	void GetAppleBundelResourcePathByObjectPath( const char*e_strSrc,char*e_strDest)
	{
		char	l_strFileName[1024];
		sprintf(l_strFileName,"%s",GetFileNameWithoutFullPath(e_strSrc,false).c_str());
		NSString* l_str = 0;
		NSString*l_NSDirectory = 0;
		NSString*l_strName = [[NSString alloc]initWithUTF8String:l_strFileName];
		char	l_strDirectory[1024];
		std::string l_strDirectoryName = GetDirectoryWithoutFileName(e_strSrc);
		if( l_strDirectoryName.length() )
		{
			sprintf(l_strDirectory,"%s",l_strDirectoryName.c_str());
			l_NSDirectory = [[NSString alloc]initWithUTF8String:l_strDirectory];
		}
		l_str = [[NSBundle mainBundle] pathForResource:l_strName ofType:nil inDirectory:l_NSDirectory ];
		//ensure u add the right type in the XCode
		const char* datechar = [l_str UTF8String];
		if( datechar )
		{
			assert(e_strDest);
			sprintf(e_strDest,"%s",datechar);
		}
	}

	void GetIphoneAppFilePath(const char* e_strFileName, char*e_strOutputFileName)
	{
		NSArray *Paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *documentsDirectory = [Paths objectAtIndex:0];
		sprintf(e_strOutputFileName, "%s/%s", [documentsDirectory UTF8String], e_strFileName);
	}
#endif

	NvFile*	MyFileOpen( const char* e_strFileName,const char*e_strMode )
	{
#if defined(WIN32) && defined(DEBUG)
		WCHAR l_dwCurrenctDirectory[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, l_dwCurrenctDirectory);
#endif
		NvFile* l_pFile = nullptr;
		bool	l_bWrite = false;
		for(size_t i=0;i<strlen(e_strMode);++i)
		{
			if( e_strMode[i] == 'w' )
			{
				l_bWrite = true;
				break;
			}
		}
#if defined(IOS)
		char	l_strFileName[MAX_PATH];
		GetAppleBundelResourcePathByObjectPath(e_strFileName,l_strFileName);
		l_pFile = fopen( l_strFileName, e_strMode );
		if( !l_pFile)
		{
			GetIphoneAppFilePath(e_strFileName,l_strFileName);
			l_pFile = fopen( l_strFileName, e_strMode );
		}
#elif defined (ANDROID)
		//http://blog.sephiroth.it/2010/10/24/reading-resource-files-from-native-code/
		//http://androgeek.info/?p=275
 		if( l_bWrite )
			mkpath( std::string(e_strFileName) );
		//now make original file read at last,because we want to read new file first.?
		//l_pFile = NvFOpen(e_strFileName, e_strMode);
		if( !l_pFile)
		{
			const int l_iNumDirectory = 5;
			std::string l_strSDCard = "/sdcard/";
			//try external sd card first
			std::string* l_strDirectory[l_iNumDirectory] =
			{
				cCommonApp::m_spExternalSDDirectory,
				cCommonApp::m_spExternalDirectory,
				& l_strSDCard,
				cCommonApp::m_spobbPath,
				cCommonApp::m_spInternalDirectory
			};
			for (int i = 0; i < l_iNumDirectory; ++i)
			{
				if (l_strDirectory[i])
				{
					std::string l_strFileName = *l_strDirectory[i];
					if (l_strFileName[l_strFileName.length() - 1] != '/')
					{
						l_strFileName += "/";
					}
					//external add GameAppName for folder.
					if (i < 3)
					{
						if (cCommonApp::m_psstrGameAppName)
						{
							l_strFileName += *cCommonApp::m_psstrGameAppName;
							l_strFileName += "/";
						}
					}
					l_strFileName += e_strFileName;
					if (l_bWrite)
					{
						mkpath(l_strFileName);
					}
					l_pFile = NvFOpen(l_strFileName.c_str(), e_strMode);
					if (l_pFile)
					{
//#ifdef DEBUG
//						std::string l_strTempFileNameForShow = l_strFileName;
//						l_strTempFileNameForShow += " opened";
//						FMLog::Log(l_strTempFileNameForShow.c_str(),false);
//#endif
						return l_pFile;
					}
				}
			}
			if (!l_pFile)
			{
				l_pFile = NvFOpen(e_strFileName, e_strMode);
			}
#ifdef DEBUG
			const char*ll = e_strFileName;
			if(!l_pFile)
			{
				std::string	l_str = ll;l_str += " openfile failed! ";
				FMLog::LogWithFlag(ll, CORE_LOG_FLAG);
			}
			else
			{
				std::string	l_str = ll;l_str += " openfile ok !";
				FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
			}
#endif
		}
#elif defined(WASM)
		{
			std::string l_strWASMFile = "/GameApp/" + UT::ConvertFileNameWithoutFullPath(e_strFileName);
			l_pFile = fopen(l_strWASMFile.c_str(), e_strMode);
			std::string l_strFileName = e_strFileName;
			l_strFileName += " :0-";
			l_strFileName += l_strWASMFile;
			if (!l_pFile)
			{
				l_strWASMFile = e_strFileName;
				l_strFileName += " :1-";
				l_strFileName += l_strWASMFile;
				l_pFile = fopen(l_strWASMFile.c_str(), e_strMode);
				if (!l_pFile)
				{
					l_strWASMFile = "assets/";
					l_strWASMFile += e_strFileName;
					l_pFile = fopen(l_strWASMFile.c_str(), e_strMode);
					l_strFileName += " :2-";
					l_strFileName += l_strWASMFile;
				}
				if (!l_pFile)
				{
					l_strWASMFile = "/GameApp/";
					l_strWASMFile += e_strFileName;
					l_pFile = fopen(l_strWASMFile.c_str(), e_strMode);
					l_strFileName += " :3-";
					l_strFileName += l_strWASMFile;
				}
				if (!l_pFile)
				{
					l_strWASMFile = UT::ConvertFileNameWithoutFullPath(e_strFileName);
					l_pFile = fopen(l_strWASMFile.c_str(), e_strMode);
					l_strFileName += " :4-";
					l_strFileName += l_strWASMFile;
				}
			}
			if (l_pFile)
			{
				l_strFileName += " open file okay";
			}
			else
			{
				l_strFileName += " open file failed";
			}
			FMLog::LogWithFlag(l_strFileName.c_str(), CORE_LOG_FLAG);
		}
#elif defined(UWP)
		std::string l_strUWPFullpath;
		std::string* l_PathArray[] = { cCommonApp::m_spUWPAppDataLocalDirectory,cCommonApp::m_spUWPAssetsDirectory };
		for (int i = 0; i < 2; ++i)
		{
			if (!l_PathArray[i])
				continue;
			l_strUWPFullpath = *l_PathArray[i];
			bool l_bSkipDotSlash = false;
			if (strlen(e_strFileName) >= 2)
			{
				if (e_strFileName[0] == '.' && e_strFileName[1] == '/')
				{
					l_bSkipDotSlash = true;
				}
			}
			if (l_bSkipDotSlash)
			{
				l_strUWPFullpath += std::string(&e_strFileName[2]);
			}
			else
			{
				l_strUWPFullpath += e_strFileName;
			}
			if (l_bWrite)
				mkpath(std::string(UT::GetDirectoryWithoutFileName(l_strUWPFullpath.c_str()).c_str()));
			l_pFile = fopen(l_strUWPFullpath.c_str(), e_strMode);
			if (l_pFile)
				break;
		}
		if (!l_pFile)
		{
			l_pFile = fopen(e_strFileName, e_strMode);
		}
#else
		if (l_bWrite)
		{
			mkpath(std::string(UT::GetDirectoryWithoutFileName(e_strFileName).c_str()));
		}
		l_pFile = fopen( e_strFileName, e_strMode );
#endif
		return l_pFile;
	}

	bool GetJsonFromFile(const char* e_strFileName, nlohmann::json& e_JsonData)
	{
		int l_iFileLength = 0;
		auto l_strFileContent = GetFileContent(e_strFileName, l_iFileLength, "r");
		if (l_strFileContent)
		{
			try
			{
				e_JsonData = nlohmann::json::parse(l_strFileContent);
			}
			catch (const nlohmann::json::parse_error& e)
			{
				delete l_strFileContent;
				FMLOG(e.what());
				return false;
			}
			delete l_strFileContent;
			return true;
		}
		return false;
		//MyFileOpen(e_strFileName, "r");
		// Open file stream
		std::ifstream l_JsonStream(e_strFileName);
		if (!l_JsonStream.is_open())
		{
			FMLOG("Failed to open file:%s ", e_strFileName);
			return false;
		}
		// Parse JSON data
		try
		{
			l_JsonStream >> e_JsonData;
		}
		catch (const nlohmann::json::parse_error& e)
		{
			FMLOG(e.what());
			return false;
		}

		// Close the stream
		l_JsonStream.close();
		return true;
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
						int	l_iNumRead = (int)NvFRead(l_Temp,1,l_iBuffer,l_pSrcFile);
						if( l_iNumRead <= 0 )//all read done
							break;
						l_iRestBuffer -= l_iNumRead;
						NvFWrite(l_Temp,1,l_iNumRead,l_pCopyImageFile);					
					}
					NvFClose(l_pCopyImageFile);
					delete[] l_Temp;
				}
				NvFClose(l_pSrcFile);
			}
		}
		return true;
	}
	bool IsFileExists( const char* e_strFileName)
	{
		NvFile*l_pFile = MyFileOpen(e_strFileName,"r");
		if( l_pFile )
		{
			NvFClose(l_pFile);
			return true;
		}
		return false;
	}

	bool	MyRemoveFile( const char* e_strFileName)
	{
		int	l_iResult = remove(e_strFileName);
		if (l_iResult != 0)
		{
#if defined(WASM)
			std::string l_strWASMFile = UT::ConvertFileNameWithoutFullPath(e_strFileName);
			l_iResult = remove(l_strWASMFile.c_str());

#elif defined(ANDROID)
			//do somethig with ANDROID path.
#endif
		}
		if (l_iResult != 0)
		{
			std::string l_strInfo = e_strFileName;
			l_strInfo += " remove file failed";
			FMLog::LogWithFlag(l_strInfo.c_str(), CORE_LOG_FLAG);
		}
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
			l_cValue = (char)(e_c-'0');
		}
		else
		if( e_c >='a' && e_c<= 'f' )
		{
			l_cValue = (char)(e_c-'a'+10);
		}
		else
		if( e_c >='A' && e_c<= 'F' )
		{
			l_cValue = (char)(e_c-'A'+10);
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

	int64		HexStringToInt64(const char*e_strValue)
	{
		int	l_iNum = (int)strlen(e_strValue);
		if (l_iNum <= 0)
			return 0;
		int64	l_i64Value = 0;
		int64	l_i64HexValue = 16;
		char	l_cOrder[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };//0x00000000 00000000
		for (int i = 0; i<l_iNum; ++i)//set order from units start
			l_cOrder[l_iNum - i - 1] = e_strValue[i];
		l_i64Value += CharToHex(l_cOrder[0]);
		for (int i = 1; i < l_iNum; ++i)
		{
			l_i64Value += CharToHex(l_cOrder[i])*l_i64HexValue;
			l_i64HexValue *= 16;
		}
		return l_i64Value;
	}
	std::string DataToHexString(unsigned char*e_pData, int e_iInLength)
	{
		std::string l_strResult;
		for (int i = 0; i < e_iInLength; i++)
		{
			char l_str[3];
			sprintf(l_str, "%x", e_pData[i]);
			l_strResult += l_str;
		}
		return l_strResult;
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
		return FATMING_CORE::ValueToStringW(GetSystemTimeForFile());
//#ifdef WIN32
//		SYSTEMTIME l_st;
////		GetSystemTime(&l_st);
//		GetLocalTime(&l_st);
//		if( e_bCN )
//		{
//			l_str += ValueToStringW(l_st.wYear);
//			l_str += L"_";
//			l_str += ValueToStringW(l_st.wMonth);
//			l_str += L"_";
//			l_str += ValueToStringW(l_st.wDay);
//			l_str += L"_";
//			l_str += ValueToStringW(l_st.wHour);
//			l_str += L"_";
//			l_str += ValueToStringW(l_st.wMinute);
//			l_str += L"_";
//			l_str += ValueToStringW(l_st.wSecond);
//		}
//		else
//		{
//			l_str += ValueToStringW(l_st.wMonth);
//			l_str += L"_";
//			l_str += ValueToStringW(l_st.wDay);
//			l_str += L"_";
//			l_str += ValueToStringW(l_st.wYear);
//			l_str += L"_";
//			l_str += ValueToStringW(l_st.wHour);
//			l_str += L"_";
//			l_str += ValueToStringW(l_st.wMinute);
//			l_str += L"_";
//			l_str += ValueToStringW(l_st.wSecond);	
//		}
//#endif	
//		return l_str;
	}

	std::wstring			GetSystemTime(bool e_bCN)
	{
		return FATMING_CORE::ValueToStringW(GetSystemTime());
	}
	std::string GetSystemTimeForFile()
	{
		struct tm* tm;
		char buf[200];
		time_t ltime; time(&ltime);
		/* convert time_t to broken-down time representation */
		tm = localtime(&ltime);
		/* format time days.month.year hour:minute:seconds */
		strftime(buf, sizeof(buf), "%d_%m_%Y_%H_%M_%S", tm);

		return std::string(buf);
	}

	std::string	GetSystemTimeOnlyUnderHour()
	{
		struct tm* tm;
		char buf[200];
		time_t ltime; time(&ltime);
		/* convert time_t to broken-down time representation */
		tm = localtime(&ltime);
		/* format time days.month.year hour:minute:seconds */
		sprintf(buf, "%d:%d:%d", tm->tm_hour, tm->tm_min, tm->tm_sec);
		return std::string(buf);
	}
	std::string GetSystemTime()
	{
		struct tm* tm;
		char buf[200];
		time_t ltime; time(&ltime);
		/* convert time_t to broken-down time representation */
		tm = localtime(&ltime);
		/* format time days.month.year hour:minute:seconds */
		strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", tm);
		return std::string(buf);
	}
#if defined(WIN32)
//#define	WCHAR_TO_WCHAR( p,q ){ int l_iLength = wcslen(p);memcpy(q,p,sizeof(wchar_t)*l_iLength);q[l_iLength] = L'\0';  }
//#if defined(ANDROID)
//#define WCHAR_TO_CHAR(wchar_t_,char_){	int	l_iSize = wcslen(wchar_t_);	for(int i=0;i<l_iSize;++i){	char_[i] = (char)wchar_t_[i];}char_[l_iSize] = '\0'; }
//#define CHAR_TO_WCHAR(char_,wchar_t_){	int	l_iSize = strlen(char_);for(int i=0;i<l_iSize;++i){	wchar_t_[i] = (wchar_t)char_[i];}wchar_t_[l_iSize] = L'\0';}
//#else
//#define WCHAR_TO_CHAR(wchar_t_,char_){wcstombs(char_,wchar_t_,TEMP_SIZE);}
//#define CHAR_TO_WCHAR(char_,wchar_t_){ mbstowcs(wchar_t_,char_,TEMP_SIZE);}
//#endif
// Convert a wide Unicode string to an UTF8 string
//https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte
	//inline std::string STDWStringToString(std::wstring e_String)
	//{
	//	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//	std::string narrow = converter.to_bytes(e_String);
	//	return narrow;
	//}
	//inline std::wstring STDStringToWString(std::string e_String)
	//{
	//	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//	std::wstring wide = converter.from_bytes(e_String);
	//	return wide;
	//}

	std::string WcharToChar(const wchar_t* e_strString)
	{
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//std::string narrow = converter.to_bytes(e_strString);
		//return narrow;
		int l_iSize = (int)wcslen(e_strString);
		if (!e_strString || l_iSize == 0) return std::string();
		//int size_needed = WideCharToMultiByte(CP_UTF8, 0, e_strString, l_iSize, NULL, 0, NULL, NULL);
		int size_needed = WideCharToMultiByte(CP_ACP, 0, e_strString, l_iSize, NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_ACP, 0, e_strString, l_iSize, &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}

	// Convert an UTF8 string to a wide Unicode String
	std::wstring CharToWchar(const char* e_strString)
	{
		int l_iSize = (int)strlen(e_strString);
		if (!e_strString || l_iSize == 0) return std::wstring();
		int size_needed = MultiByteToWideChar(CP_ACP, 0, e_strString, l_iSize, NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_ACP, 0, e_strString, l_iSize, &wstrTo[0], size_needed);
		return wstrTo;
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//std::wstring wide = converter.from_bytes(e_strString);
		//return wide;
	}
//#elif defined(ANDROID) 
//	std::string	WcharToChar(const wchar_t *e_strWchar)
//	{
//		////https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
//		////std::setlocale(LC_ALL, "");
//		std::string		l_strResult;
//		std::wstring	l_strForCopy = e_strWchar;
//		l_strResult.assign(l_strForCopy.begin(), l_strForCopy.end());
//		//}
//		return l_strResult;
//	}
//	// Convert an UTF8 string to a wide Unicode String
//	std::wstring CharToWchar(const char* e_strString)
//	{
//		int l_iSize = (int)strlen(e_strString);
//		if (!e_strString || l_iSize == 0) return std::wstring();
//		int size_needed = MultiByteToWideChar(CP_UTF8, 0, e_strString, l_iSize, NULL, 0);
//		std::wstring wstrTo(size_needed, 0);
//		MultiByteToWideChar(CP_UTF8, 0, e_strString, l_iSize, &wstrTo[0], size_needed);
//		return wstrTo;
//	}
#elif defined(ANDROID)
	//#define WCHAR_TO_CHAR(wchar_t_,char_){wcstombs(char_,wchar_t_,TEMP_SIZE);}
	//#define CHAR_TO_WCHAR(char_,wchar_t_){ mbstowcs(wchar_t_,char_,TEMP_SIZE);}
	std::string	WcharToChar(const wchar_t *e_strWchar)
	{
		//fuck because old NDK has serious problem with wcstombs so copy char one by one
		std::string l_strResult;
		auto l_iSize = wcslen(e_strWchar);
		l_strResult.resize(l_iSize);
		for (auto i = 0; i < l_iSize; ++i)
		{
			l_strResult[i] = (char)e_strWchar[i];
		}
		return l_strResult;
		//size_t l_uiSize = wcslen(e_strWchar);
		//char * dBuf = new char[l_uiSize+1];
		//dBuf[l_uiSize] = 0;
		//wcstombs(dBuf, e_strWchar, l_uiSize);
		//std::string wstrTo = dBuf;
		//delete[] dBuf;
		//return wstrTo;
	}


	//#define CHAR_TO_WCHAR(char_,wchar_t_){ mbstowcs(wchar_t_,char_,TEMP_SIZE);}
	// Convert an UTF8 string to a wide Unicode String
	std::wstring CharToWchar(const char* e_strString)
	{
		//fuck because old NDK has serious problem with wcstombs so copy char one by one
		std::wstring l_strResult;
		auto l_iSize = strlen(e_strString);
		l_strResult.resize(l_iSize);
		for (auto i = 0; i < l_iSize; ++i)
		{
			l_strResult[i] = e_strString[i];
		}
		return l_strResult;
		//size_t l_uiSize = (int)strlen(e_strString);
		//if (!e_strString || l_uiSize == 0) return std::wstring();
		//wchar_t * dBuf = new wchar_t[l_uiSize+1];
		//dBuf[l_uiSize] = 0;
		//mbstowcs(dBuf, e_strString, l_uiSize);
		//std::wstring wstrTo = dBuf;
		//delete[] dBuf;
		//return wstrTo;
	}
#else

	//#define WCHAR_TO_CHAR(wchar_t_,char_){wcstombs(char_,wchar_t_,TEMP_SIZE);}
	//#define CHAR_TO_WCHAR(char_,wchar_t_){ mbstowcs(wchar_t_,char_,TEMP_SIZE);}
	std::string	WcharToChar(const wchar_t *e_strWchar)
	{
		size_t l_uiSize = wcslen(e_strWchar);
		char * dBuf = new char[l_uiSize+1];
		dBuf[l_uiSize] = 0;
		wcstombs(dBuf, e_strWchar, l_uiSize);
		std::string wstrTo = dBuf;
		delete[] dBuf;
		return wstrTo;
	}


	//#define CHAR_TO_WCHAR(char_,wchar_t_){ mbstowcs(wchar_t_,char_,TEMP_SIZE);}
	// Convert an UTF8 string to a wide Unicode String
	std::wstring CharToWchar(const char* e_strString)
	{
		size_t l_uiSize = (int)strlen(e_strString);
		if (!e_strString || l_uiSize == 0) return std::wstring();
		wchar_t * dBuf = new wchar_t[l_uiSize+1];
		dBuf[l_uiSize] = 0;
		mbstowcs(dBuf, e_strString, l_uiSize);
		std::wstring wstrTo = dBuf;
		delete[] dBuf;
		return wstrTo;
	}
#endif

	std::string	WcharToChar(std::wstring e_strWchar)
	{
		auto l_str = e_strWchar.c_str();
		return WcharToChar(l_str);
	}

	std::wstring	CharToWchar(std::string	e_strChar)
	{
		auto l_str = e_strChar.c_str();
		return CharToWchar(l_str);
	}
	//for nlohmann json string to wchar.
	std::wstring			UTF8ToWchar(const char* e_str)
	{
		std::wstring out;
		if (!e_str)
		{
			return out;
		}
		unsigned int codepoint;
		while (*e_str != 0)
		{
			unsigned char ch = static_cast<unsigned char>(*e_str);
			if (ch <= 0x7f)
				codepoint = ch;
			else if (ch <= 0xbf)
				codepoint = (codepoint << 6) | (ch & 0x3f);
			else if (ch <= 0xdf)
				codepoint = ch & 0x1f;
			else if (ch <= 0xef)
				codepoint = ch & 0x0f;
			else
				codepoint = ch & 0x07;
			++e_str;
			if (((*e_str & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
			{
				if (sizeof(wchar_t) > 2)
					out.append(1, static_cast<wchar_t>(codepoint));
				else if (codepoint > 0xffff)
				{
					codepoint -= 0x10000;
					out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
					out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
				}
				else if (codepoint < 0xd800 || codepoint >= 0xe000)
					out.append(1, static_cast<wchar_t>(codepoint));
			}
		}
		return out;
	}

	std::vector<std::string> StringSplit(const std::string& s, char delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(s);
		while (std::getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}

	bool IsNumber(std::string& s)
	{
		//from chatgpt
		// Remove leading and trailing whitespace and newlines
		s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char c)
			{
				return std::isspace(c);
			}), s.end());

		// Regex to match a valid number
		return std::regex_match(s, std::regex("[+-]?([0-9]*[.])?[0-9]+"));
	}

	std::string ToLower(const std::string& str)
	{
		std::string lower_case = str;
		std::locale loc;
		for (char& s : lower_case)
			s = std::tolower(s, loc);
		return lower_case;
	}

	std::wstring ToLower(const std::wstring& str)
	{
		std::wstring lower_case = str;
		std::locale loc;
		for (wchar_t& s : lower_case)
			s = std::tolower(s, loc);
		return lower_case;
	}

	int		GetLoopIndex(int e_iIndex,int e_iTotalCount)
	{
		if( e_iTotalCount == 0 )
			return -1; 
		if( e_iIndex < 0 )
		{
			int	l_iIndex = e_iIndex%e_iTotalCount;
			if (l_iIndex == 0)
				return 0;
			return e_iTotalCount+l_iIndex;
		}
		if( e_iIndex >= e_iTotalCount )
		{
			int	l_iIndex = e_iIndex%e_iTotalCount;
			return l_iIndex;
		}
		return e_iIndex;
	}

	void			itoa64(int64 e_i64Val, char*e_strBuffer, int base)
	{
		char buf[64] = { 0 };
		memset(buf, 0, sizeof(char) * 64);
		int64	l_i64OriginalValue = e_i64Val;
		if (l_i64OriginalValue != 0)
		{
			if (l_i64OriginalValue < 0)
				l_i64OriginalValue = -l_i64OriginalValue;
			int i = 62;//last one is 0,value start from last 2.
			for (; l_i64OriginalValue && i; --i, l_i64OriginalValue /= base)
				buf[i] = "0123456789abcdef"[l_i64OriginalValue % base];
			if (e_i64Val < 0)
			{
				memcpy(&e_strBuffer[1], &buf[i + 1], sizeof(char) * 64 - i - 1);
				e_strBuffer[0] = L'-';
			}
			else
			{
				memcpy(e_strBuffer, &buf[i + 1], sizeof(char) * 64 - i - 1);
			}
		}
		else
		{
			e_strBuffer[0] = '0';
			e_strBuffer[1] = '\0';
		}
	}

	sDataContainer::sDataContainer()
	{
		pData = nullptr;
		uiDataSize = 0;
		uiDataCount = 0;
		DataType = eDataType::eDT_MAX;
	}

	sDataContainer::~sDataContainer()
	{
		SAFE_DELETE_ARRAY(pData);
	}

	bool	sDataContainer::CreateData(unsigned int e_uiDataSize, eDataType e_eDataType, int e_iDataCount)
	{
		if (g_iDataTypeSize[e_eDataType] * e_iDataCount == (int)e_uiDataSize)
		{
			SAFE_DELETE_ARRAY(pData);
			pData = new char[e_uiDataSize];
			DataType = e_eDataType;
			this->uiDataSize = e_uiDataSize;
			this->uiDataCount = e_iDataCount;
			return true;
		}
		return false;
	}

	bool	sDataContainer::CopyData(char * e_pData, unsigned int e_uiDataSize, eDataType e_eDataType, int e_iDataCount)
	{
		if (g_iDataTypeSize[e_eDataType] * e_iDataCount == (int)e_uiDataSize && e_pData != nullptr)
		{
			SAFE_DELETE_ARRAY(pData);
			pData = new char[e_uiDataSize];
			memcpy(pData, e_pData, e_uiDataSize);
			uiDataSize = e_uiDataSize;
			uiDataCount = e_iDataCount;
			DataType = e_eDataType;
			return true;
		}
		return false;
	}

	bool	sDataContainer::AssignData(char * e_pData, unsigned int e_uiDataSize, eDataType e_eDataType, int e_iDataCount)
	{
		if (g_iDataTypeSize[e_eDataType] * e_iDataCount == (int)e_uiDataSize)
		{
			SAFE_DELETE_ARRAY(pData);
			pData = e_pData;
			uiDataSize = e_uiDataSize;
			uiDataCount = e_iDataCount;
			DataType = e_eDataType;
			return true;
		}
		return false;
	}

	//end namespace UT
}
