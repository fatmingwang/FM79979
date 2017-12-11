#ifndef CommonDefine_H
#define CommonDefine_H

#include "PlatFormDefine.h"

#ifndef TEMP_SIZE
#define	TEMP_SIZE			1024
#endif
//do the thing going to happen,if cvall back is assigned
typedef void    (*DoButtonGoal_Callback)(void*e_pData);

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=nullptr; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p,s)       { if(p) { p->Release(s);     (p)=nullptr; } }
#endif
#ifndef SAFE_FREE
#define SAFE_FREE(p)       { if(p) { free (p);     (p)=nullptr; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=nullptr; } }
#endif

#define DELETE_VECTOR(List,T){for( size_t i = 0;i<List.size();++i){	T l_p = List[i];SAFE_DELETE(l_p);}List.clear();}
#define DELETE_POINTER_VECTOR(List,T){for( UINT i = 0;i<List->size();++i){	T l_p = (*List)[i];SAFE_DELETE(l_p);}SAFE_DELETE(List);}
//std::vector<std::vector<Dataooxx*>*>
#define DELETE_VECTOR_VECTORPOINTER(List,T){for( size_t i = 0;i<List.size();++i){auto l_pDataVector = List[i];if( l_pDataVector ){DELETE_VECTOR((*l_pDataVector),T);SAFE_DELETE(l_pDataVector);}}List.clear();}


//p for first q for second r for map object
#define DEL_MAP(p,q,r){											\
	std::map<p,q>::iterator i=r.begin();						\
	for(;i!=r.end();++i)										\
	{															\
		q l_Type2=i->second;									\
		SAFE_DELETE(l_Type2);									\
	}r.clear();}

#define RELEASE_MAP(p,q,r){											\
	std::map<p,q>::iterator i=r.begin();						\
	for(;i!=r.end();++i)										\
	{															\
		q l_Type2=i->second;									\
		SAFE_RELEASE(l_Type2);									\
	}r.clear();}

//Type Variable,GetName,SetName
#define GET_SET_DEC( T,V,p,q ) protected:T V;public:T p (){ return V; }void q (T value){ V = value; }

#define GET_SET( T,V,p,q )		public:T p (){ return V; }void q (T value){ V = value; }
////Type Variable,GetName,SetName
//return pointer
#define GETP_SET_DEC( T,V,p,q ) protected:T V;public:T* p (){ return &V; }void q (T value){ V = value; }
//
#define GETPP_SET_DEC( T,V,p,q ) protected:T V;public:T** p (){ return &V; }void q (T value){ V = value; }
//
#define GET_SET_DEC_QUICK( TYPE, NAME )   TYPE	m_##NAME; void Set##NAME(TYPE e_Data ){m_##NAME = e_Data;} TYPE Get##NAME(){ return m_##NAME; }
//here the limit for string length is TEMP_SIZE = 255
#ifdef WIN32
#define	CHAR_TO_WCHAR( p,q )	 MultiByteToWideChar( CP_ACP, 0, p, -1, q, 512 );//q for return p for convert
#define	WCHAR_TO_CHAR( p,q )  WideCharToMultiByte( CP_ACP, 0, p, -1, q, 256, nullptr, nullptr );//q for return p for convert
#define	WCHAR_TO_WCHAR( p,q ){ size_t l_iLength = wcslen(p);memcpy(q,p,sizeof(wchar_t)*l_iLength);q[l_iLength] = L'\0';  }
#define FM_NEW new(_CLIENT_BLOCK,__FILE__, __LINE__)
#else
//here should using
//size_t iconv(iconv_t cd, char **inbuf,
//       size_t *inbytesleft, char **outbuf,
//       size_t *outbytesleft); 
//http://www.opengroup.org/onlinepubs/009695399/functions/iconv.html
	#define	WCHAR_TO_WCHAR( p,q ){ int l_iLength = wcslen(p);memcpy(q,p,sizeof(wchar_t)*l_iLength);q[l_iLength] = L'\0';  }
	#if defined(ANDROID)
		#define WCHAR_TO_CHAR(wchar_t_,char_){	int	l_iSize = wcslen(wchar_t_);	for(int i=0;i<l_iSize;++i){	char_[i] = (char)wchar_t_[i];}char_[l_iSize] = '\0'; }
		#define CHAR_TO_WCHAR(char_,wchar_t_){	int	l_iSize = strlen(char_);for(int i=0;i<l_iSize;++i){	wchar_t_[i] = (wchar_t)char_[i];}wchar_t_[l_iSize] = L'\0';}
	#else
		#define WCHAR_TO_CHAR(wchar_t_,char_){wcstombs(char_,wchar_t_,TEMP_SIZE);}
		#define CHAR_TO_WCHAR(char_,wchar_t_){ mbstowcs(wchar_t_,char_,TEMP_SIZE);}
	#endif

#endif

#define UINT_ARGB(a,r,g,b) ((unsigned int)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define UINT_RGBA(r,g,b,a) UINT_ARGB(a,r,g,b)
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif
//the velocities dont need to be normalised. 
//1.0E-6f means 1.0f * 10^-6, so 0.000001f. It's a tolerance that you need to avoid divides by 0.
#define	EPSIONAL	0.00001f

#define	FM	FATMING_CORE

#if defined(ANDROID)||defined(IOS) || defined(LINUX) || defined(WASM)
#define	Sleep(TIME)	usleep(TIME*1000);//unistd.h
#endif
//#define CORE_DLL
#ifdef CORE_DLL
#ifdef WIN32
// Disable the "private member not available for export" warning,
// because I don't feel like writing interfaces
//#pragma warning(disable:4251) 
#define CORE_EXPORT __declspec(dllexport)
//#elif defined(IOS) || defined(LINUX)
#else
#define CORE_EXPORT __attribute__((visibility("default")))
//#define FCOLLADA_LOCAL __attribute__((visibility("hidden")))
#endif
#else // FCOLLADA_DLL
//#define FCOLLADA_EXPORT
#endif // FCOLLADA_DLL

#endif