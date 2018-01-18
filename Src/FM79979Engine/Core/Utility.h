#ifndef UTILITY_H
#define UTILITY_H

#include "NamedTypedObject.h"
#include <map>
#include "Math/Collision.h"
#include "Glh.h"
#include "TimeUT.h"
#include "Android/nv_file.h"
#include "TimeUT.h"
#if defined(WIN32)
	#pragma warning( disable : 4996 )
#endif

namespace UT
{
#ifdef DEBUG
	void					MyGlErrorTest(const char*e_strMessage);
	void					MyGLEnable(GLenum e_GLenum);
	void					MyGLDisable(GLenum e_GLenum);
	void					MyGLGetIntegerv(GLenum e_GLenum, GLint *params);
#else
	#define	MyGlErrorTest(q)
	#define	MyGLEnable(p)			glEnable(p)
	#define	MyGLDisable(p)			glDisable(p)
	#define	MyGLGetIntegerv(p,q)	glGetIntegerv(p,q)
#endif
#ifdef WIN32
	HGLRC					InitOpenGL(HWND e_pHwnd,bool e_bInitGlewInit,HDC e_HdcMV,bool e_bEnableMultisample = false);
	bool 					IsExtensionSupported( char* szTargetExtension );
#endif
	eDataType				GetDataType(const wchar_t*e_str);
	eDataType				GetDataType(const char*e_str);
	//inline char*			ConstCharToChar(const char* e_str){ static char l_strTemp[TEMP_SIZE]; sprintf(l_strTemp,"%s\0",e_str);return l_strTemp; }
	std::wstring			ComposeMsgByFormat(wchar_t*e_strErrMsg1,...);
	std::string				ComposeMsgByFormat(char*e_strErrMsg1,...);
	void					ErrorMsg(const wchar_t*e_strErrMsg1,const wchar_t*e_strErrMsg2);
	void					ErrorMsg(std::wstring  e_strErrMsg1,std::wstring e_strErrMsg2);
	void					ErrorMsg(const wchar_t*  e_strErrMsg1,std::wstring e_strErrMsg2);
	void					ErrorMsg(std::wstring e_strErrMsg1,const wchar_t*  e_strErrMsg2);

	void					ErrorMsg(char*e_strErrMsg1,char*e_strErrMsg2);
	void					ErrorMsg(const char*e_strErrMsg1,char*e_strErrMsg2);
	void					ErrorMsg(char*e_strErrMsg1,const char*e_strErrMsg2);
	void					ErrorMsg(const char*e_strErrMsg1,const char*e_strErrMsg2);	
	void					ErrorMsgByFormat(char*e_strErrMsg1,...);
	void					ErrorMsgByFormat(wchar_t*e_strErrMsg1,...);
	//
	void					SystemErrorCheck();
	std::wstring			GetFileNameWithoutFullPath(const wchar_t*e_pString,bool e_bStripExtensionFileName = true);
	std::wstring			GetFileNameWithoutFullPath(std::wstring e_pString,bool e_bStripExtensionFileName = true);
	std::string				GetFileNameWithoutFullPath(const char*e_pString,bool e_bStripExtensionFileName = true);
	std::string				GetFileNameWithoutFullPath(std::string e_pString,bool e_bStripExtensionFileName = true);
	std::string				ConvertFileNameWithoutFullPath(const char*e_strFileName,char e_cSlashReplaceCharacter = '_');
	std::wstring			GetFileExtensionName(const wchar_t*e_pString);
	std::string				GetFileExtensionName(const char*e_pString);
	std::string				ChangeFileExtensionName(const char*e_pFileNameString,const char*e_strNewExtensionName);
	std::string				GetDirectoryWithoutFileName(const char*e_str);
	//
	bool					IsStringContain(const char*e_strSource,const char*e_strCompare);
	bool					IsStringContain(const wchar_t*e_strSource,const wchar_t*e_strCompare);
	//for cross platform
	////http://forum.codecall.net/topic/42319-what-mode-should-i-use-at-fopen-r-r-wb/
	//For creating/writing/reading use w+ For opening/appending/reading use r+
	//"r"	//Open a file for reading. The file must exist. 
	//"w"	//Create an empty file for writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file. 
	//"a"	//Append to a file. Writing operations append data at the end of the file. The file is created if it does not exist. 
	//"r+"	//Open a file for update both reading and writing. The file must exist.
	//"w+"	//Create an empty file for both reading and writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file. 
	//"a+"	//Open a file for reading and appending. All writing operations are performed at the end of the file, protecting the previous content to be overwritten. You can reposition the internal pointer to anywhere in the file for reading, but writing operations will move it back to the end of file. The file is created if it does not exist. 
	NvFile*					MyFileOpen( const char* FileName,const char*e_strMode  );
	bool					MyFileCopy( const char* e_strSrcFileName,const char* e_strDestFileName);
	std::string				GetTxtFileContent(const char* e_strFileName);
	bool					SaveTxtToFile(const char* e_strFileName,const char*e_strContent,int e_iByteCount);
	long					GetFileSize(NvFile*e_pFile);
	bool					IsFileExists( const char* FileName );
	bool					MyRemoveFile( const char* FileName );
	bool					MyRemoveDirectory( const char* e_strDirectoryName );
	bool					MyCreateDirectory( const char* e_strDirectoryName );
	//ex:  view rang is 100,100,200,200
	//                  4 factor represent x,y,x,y coordinate
	Vector4					ViewRectToOpenGLScissor(Vector4 e_v2DViewRange,Vector4 e_vViewPort,Vector2 e_vGameResolution,eDeviceDirection e_eDeviceDirection);
	//x,y showpos,zw width height
	Vector4					ViewRectToOpenGLScissor(Vector4 e_v2DViewRange);
	//============
	//only support 4 characters
	int						HexadecimalStringToDecimal(char*e_strValue);
	int						DecimalStringToHexadcimal(char*e_strValue);	
	int						FindChacterIndexFromBackBuffer(const char*e_String,char e_char);
	int						FindChacterIndexFromBackBuffer(const wchar_t*e_String,wchar_t e_char);
	int						FindChacterIndexFromStartBuffer(const wchar_t*e_String,wchar_t e_char,int e_iLookForIndex = 0);
	//
	//if e_bCN is true return chinese format or return western format
	std::wstring			GetSystemTimeForFile(bool e_bCN);
	std::wstring			GetSystemTime(bool e_bCN);
	//====================
	//this is good for immediately to get pointer,but becareful!
	//because here using static variable
	//====================
//	void					WcharToChar(const wchar_t *e_strSrc,char*e_strDest);
	//void					WcharToChar(wchar_t *e_strSrc,char*e_strDest);
	std::string				WcharToChar(const wchar_t *e_strWchar);
	std::string				WcharToChar(std::wstring e_strWchar);
	//void					CharToWchar(const char *e_strSrc,wchar_t*e_strDest);
	//void					CharToWchar(char *e_strSrc,wchar_t*e_strDest);
	std::wstring			CharToWchar(const char *e_strChar);
	std::wstring			CharToWchar(std::string	e_strChar);
	POINT					GetScreenResolution();
	//a vector contain a bunch data,now input a index it is possible bigger than last index or smaller 0,
	//so we have to find the proper one if we want it be loop
	int						GetLoopIndex(int e_iIndex,int e_iTotalCount);
	template <class T>inline	int		GetLoopIndex(int e_iIndex,std::vector<T>*e_pVector){return GetLoopIndex(e_iIndex,(int)e_pVector->size());}
	//is index legal in the vector range
	template <class T>inline	bool	IsLegalIndexInVector(std::vector<T>*e_pVector,int e_iIndex){if( e_iIndex<0 )return false;if( e_iIndex>=(int)e_pVector->size() )return false;return true;	}


	template <class T>inline	int	IsVectorContainValue(std::vector<T>&e_Vector,int e_Value){int l_iSize = (int)e_Vector.size();for( int i=0;i<l_iSize;++i ){if(e_Vector[i] == e_Value)return i;}return -1;}
	//
	void					itoa64(int64 val,char*e_strBuffer, int base);
	//
	template<class KEY,class VALUE>
	bool					IsMapContainKey(std::map<KEY,VALUE>*e_pMap,KEY e_Key)
	{
		auto l_iterator = e_pMap->find(e_Key);
		if( l_iterator == e_pMap->end() )
			return false;
		return true;
	}
//end namespace Utility
}
using namespace UT;
#endif