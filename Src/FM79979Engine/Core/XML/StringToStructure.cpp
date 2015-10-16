#include "../stdafx.h"
#include "StringToStructure.h"
#include "../Utility.h"

#ifdef WIN32
#pragma warning( disable : 4996 )
#endif
namespace FATMING_CORE
{
	POINT	GetPoint(const wchar_t* e_str){return GetPoint(UT::WcharToChar(e_str).c_str());}

	POINT	GetPoint(const char* e_str)
	{
		return GetPoint((char*)e_str);
	}

	POINT	GetPoint(char* e_str)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%s\0",e_str);
		POINT	l_Point;
		char*	l_str = strtok(l_temp,",: ");
		assert(l_str&&"parse POINT data error");
		l_Point.x = atol(l_str);
		l_str = strtok(0,",: ");
		assert(l_str&&"parse POINT data error");
		l_Point.y = atol(l_str);
		return l_Point;
	}
	POINT	GetPoint(std::string e_str){return GetPoint(e_str.c_str());}
	POINT	GetPoint(std::wstring e_str){return GetPoint(e_str.c_str());}

	float	GetFloat(const char* e_str){return (float)atof(e_str);}
	float	GetFloat(const wchar_t* e_str){return GetFloat(UT::WcharToChar(e_str).c_str());}
	float	GetFloat(std::string e_str){ return GetFloat(e_str.c_str()); }
	float	GetFloat(std::wstring e_str){ return GetFloat(e_str.c_str()); }

	bool	GetBoolean(const char* e_str){ return atoi(e_str)>0?true:false; }
	bool	GetBoolean(const wchar_t* e_str){ return GetBoolean(UT::WcharToChar(e_str).c_str()); }
	bool	GetBoolean(std::wstring e_str){ return GetBoolean(e_str.c_str()); }
	bool	GetBoolean(std::string e_str){ return GetBoolean(e_str.c_str()); }


	int		GetInt(const char* e_str){return atoi(e_str);}
	int		GetInt(const wchar_t* e_str){return GetInt(UT::WcharToChar(e_str).c_str());}
	int		GetInt(std::wstring e_str){ return GetInt(e_str.c_str()); }
	int		GetInt(std::string e_str){ return GetInt(e_str.c_str()); }

	uint64	GetUint64(const wchar_t* e_str){return GetUint64(UT::WcharToChar(e_str).c_str());}
	uint64	GetUint64(const char* e_str){return static_cast<uint64>(_atoi64(e_str));}
	uint64	GetUint64(std::wstring e_str){ return GetUint64(e_str.c_str()); }
	uint64	GetUint64(std::string e_str){ return GetUint64(e_str.c_str()); }


	int64	GetInt64(const wchar_t* e_str){return GetInt64(UT::WcharToChar(e_str).c_str());}
	int64	GetInt64(const char* e_str){	return _atoi64(e_str);	}
	int64	GetInt64(std::wstring e_str){ return GetInt64(e_str.c_str()); }
	int64	GetInt64(std::string e_str){ return GetInt64(e_str.c_str()); }


	RECT	GetRectangle(const char*e_pString){	return GetRectangle(CharToWchar(e_pString).c_str());}
	RECT	GetRectangle(const wchar_t*e_str){return GetRectangle(UT::WcharToChar(e_str).c_str());}
	RECT	GetRectangle(std::wstring e_str){ return GetRectangle(e_str.c_str()); }
	RECT	GetRectangle(std::string e_str){ return GetRectangle(e_str.c_str()); }

	//L,R,D,T  left right down top
	//							  here is secomd
	//L:0,0,R:1,0,D:0,1,T:1,1,L:0,0,R:1,0,D:0,1,T:1,1
	RECT*	GetRectangle(int e_iNumRect,wchar_t*e_pString)
	{
		wchar_t*  l_strTemp = 0;
		wchar_t*	l_pCurrent = wcstok(e_pString,L",:");
		RECT*	l_pRect = new RECT[e_iNumRect];
		for(int i=0;i<e_iNumRect;++i)
		{
			RECT	l_rect;
			int	l_i = 0;
			while( l_i != 4 )
			{
				if( !wcscmp(l_pCurrent,L"L") )
				{
					l_rect.left = wcstol(l_pCurrent,&l_strTemp,10);
				}
				else
				if( !wcscmp(l_pCurrent,L"R") )
				{
					l_rect.right = wcstol(l_pCurrent,&l_strTemp,10);
				}
				else
				if( !wcscmp(l_pCurrent,L"D") )
				{
					l_rect.bottom = wcstol(l_pCurrent,&l_strTemp,10);
				}
				else
				if( !wcscmp(l_pCurrent,L"T") )
				{
					l_rect.top = wcstol(l_pCurrent,&l_strTemp,10);
				}
				l_i++;
				l_pCurrent = wcstok(0,L",;");
			}
			l_pRect[i] = l_rect;
		}
		assert(!l_pCurrent&&"rect data is wrong in the TextureLoad");
		return l_pRect;
	}


	Vector4	GetVector4(const char*e_strValue)
	{
	    char    l_strValue[TEMP_SIZE];
	    sprintf(l_strValue,"%s\0",e_strValue);
		Vector4	l_Vector4;
		char*	l_str = strtok(l_strValue,",: ");
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.x = (float)atof(l_str);
		l_str = strtok(0,",: ");
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.y = (float)atof(l_str);
		l_str = strtok(0,",:" );
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.z = (float)atof(l_str);
		l_str = strtok(0,",:" );
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.w = (float)atof(l_str);
		return l_Vector4;
	}

	Vector3	GetVector3(const char*e_str)
	{
	    char    l_strValue[TEMP_SIZE];
	    sprintf(l_strValue,"%s\0",e_str);
		Vector3	l_Vector3(0.f,0.f,0.f);
		char*	l_str = strtok(l_strValue,",: ");
		assert(l_str&&"parse Vector3 data error");
		l_Vector3.x = (float)atof(l_str);
		l_str = strtok(0,",: ");
		assert(l_str&&"parse Vector3 data error");
		l_Vector3.y = (float)atof(l_str);
		l_str = strtok(0,",:" );
		if( l_str )//because we may only need xy no z
		{
			assert(l_str&&"parse Vector3 data error");
			l_Vector3.z = (float)atof(l_str);
		}
		return l_Vector3;
	}

	Vector2	GetVector2(const char*e_strValue)
	{
	    char    l_strValue[TEMP_SIZE];
	    sprintf(l_strValue,"%s\0",e_strValue);
		Vector2	l_Vector2;
		char*	l_str = strtok(l_strValue,",: ");
		assert(l_str&&"parse Vector2 data error");
		l_Vector2.x = (float)atof(l_str);
		l_str = strtok(0,",: ");
		assert(l_str&&"parse Vector2 data error");
		l_Vector2.y = (float)atof(l_str);
		return l_Vector2;
	}


	Vector4	GetVector4(const wchar_t*e_strValue){return GetVector4(UT::WcharToChar(e_strValue).c_str());}	

	Vector3	GetVector3(const wchar_t*e_strValue){return GetVector3(UT::WcharToChar(e_strValue).c_str());}

	Vector2	GetVector2(const wchar_t*e_strValue){return GetVector2(UT::WcharToChar(e_strValue).c_str());}	

	std::vector<Vector2>	GetVector2Vector(TiXmlElement*e_pTiXmlElement)
	{
		std::vector<Vector2>	l_ResultVector;
		return l_ResultVector;
	}

	std::vector<Vector3>	GetVector3Vector(TiXmlElement*e_pTiXmlElement)
	{
		std::vector<Vector3>	l_ResultVector;
		return l_ResultVector;	
	}

	std::vector<Vector4>	GetVector4Vector(TiXmlElement*e_pTiXmlElement)
	{
		std::vector<Vector4>	l_ResultVector;
		return l_ResultVector;	
	}

	cMatrix44	GetMatrix(const wchar_t*e_str,bool e_bTranspose)
	{
		std::string l_str = ::WcharToChar(e_str);
		return GetMatrix(l_str.c_str(),e_bTranspose);
	}

	cMatrix44	GetMatrix(const char*e_str,bool e_bTranspose)
	{
	    char    l_strValue[TEMP_SIZE];
	    sprintf(l_strValue,"%s\0",e_str);	
		cMatrix44	l_mat;
		char*	l_str = strtok((char*)l_strValue,",: ");
		for( int i=0;i<4;++i )
		{
			for( int j=0;j<4;++j )
			{
				l_mat.m[j][i] = (float)atof(l_str);
				l_str = strtok(0,",: ");
			}
		}
		assert(!l_str);
		if( e_bTranspose )
			return l_mat.Transposed();
		return l_mat;
	}
    //this might lost data if parse string again!
	cMatrix44	*GetMatrcies(const wchar_t*e_str,int e_iSize,bool e_bTranspose)
	{
		size_t	l_iLength = wcslen(e_str);
		char*l_str = new char[l_iLength+1];
		for(size_t i=0;i<l_iLength;++i)
			l_str[i] = (char)(e_str[i]);
		l_str[l_iLength] = '\0';
		cMatrix44	*l_pMartices = GetMatrcies(l_str,e_iSize,e_bTranspose);
		delete l_str;
		return l_pMartices;
	}
    //this might lost data if parse string again!
	cMatrix44	*GetMatrcies(const char*e_str,int e_iSize,bool e_bTranspose)
	{
		cMatrix44*l_pMat = new cMatrix44[e_iSize];
		char*	l_str = strtok((char*)e_str,",: ");
		for( int i=0;i<e_iSize;++i )
		{
			for( int j=0;j<4;++j )
			{
				for( int k=0;k<4;++k )
				{
					(l_pMat[i]).m[k][j] = (float)atof(l_str);
					l_str = strtok(0,",: ");
				}
			}
			if(e_bTranspose)
				(l_pMat[i]) = (l_pMat[i]).Transposed();
		}
		assert(!l_str);
		return l_pMat;
	}

	void	GetUV(const wchar_t*e_pData,float*e_pUVBuffer){return GetUV(UT::WcharToChar(e_pData).c_str(),e_pUVBuffer);}

	void	GetUV(char*e_pData,float*e_pUVBuffer)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%s\0",e_pData);
		char*	l_str = strtok(l_temp,",: ");
		for( int i=0;i<4;++i )
		{
			e_pUVBuffer[i] = (float)atof(l_str);
			l_str = strtok(0,",: ");
		}
	}

	void	GetUV(const char*e_pData,float*e_pUVBuffer)
	{
		GetUV((char*)e_pData,e_pUVBuffer);
	}

	std::vector<int>	GetIntegerListByCommaDivide(const wchar_t*e_str,int e_iSize)
	{
		std::vector<int>	l_NumeralList;
		if( e_iSize )
			l_NumeralList.reserve(e_iSize);
		wchar_t*	l_str = wcstok((wchar_t*)e_str,L", ");
		while(l_str)
		{
	#ifdef DEBUG
			int	l_i = _wtoi(l_str);
			l_NumeralList.push_back(l_i);
	#else
			l_NumeralList.push_back(_wtoi(l_str));
	#endif
			l_str = wcstok(0,L", ");
		}
		return l_NumeralList;	
	}


	std::vector<float>	GetFloatListByCommaDivide(const wchar_t*e_str,int e_iSize)
	{
		std::vector<float>	l_NumeralList;
		if( e_iSize )
			l_NumeralList.reserve(e_iSize);
		wchar_t*	l_str = wcstok((wchar_t*)e_str,L", ");
		while(l_str)
		{
	#ifdef DEBUG
			float	l_f = (float)_wtof(l_str);
			l_NumeralList.push_back(l_f);
	#else
			l_NumeralList.push_back((float)_wtof(l_str));
	#endif
			l_str = wcstok(0,L", ");
		}
#ifdef DEBUG
		if( e_iSize )
		{
			assert(l_NumeralList.size()==e_iSize);
		}
#endif
		return l_NumeralList;
	}

	std::vector<double>	GetDoubleListByCommaDivide(const wchar_t*e_str,int e_iSize)
	{
		std::vector<double>	l_NumeralList;
		if( e_iSize )
			l_NumeralList.reserve(e_iSize);
		wchar_t*	l_str = wcstok((wchar_t*)e_str,L", ");
		while(l_str)
		{
	#ifdef DEBUG
			double	l_db = _wtof(l_str);
			l_NumeralList.push_back(l_db);
	#else
			l_NumeralList.push_back(_wtof(l_str));
	#endif
			l_str = wcstok(0,L", ");
		}
		return l_NumeralList;
	}

	std::vector<int>	GetIntegerListByCommaDivide(char*e_str,int e_iSize)
	{
		std::vector<int>	l_NumeralList;
		if( e_iSize )
			l_NumeralList.reserve(e_iSize);
		char*	l_str = strtok(e_str,", ");
		while(l_str)
		{
	#ifdef DEBUG
			int	l_i = atoi(l_str);
			l_NumeralList.push_back(l_i);
	#else
			l_NumeralList.push_back(atoi(l_str));
	#endif
			l_str = strtok(0,", ");
		}
		return l_NumeralList;
	}

	std::vector<double>	GetDoubleListByCommaDivide(char*e_str,int e_iSize)
	{
		std::vector<double>	l_NumeralList;
		if( e_iSize )
			l_NumeralList.reserve(e_iSize);
		char*	l_str = strtok(e_str,", ");
		while(l_str)
		{
	#ifdef DEBUG
			double	l_db = atof(l_str);
			l_NumeralList.push_back(l_db);
	#else
			l_NumeralList.push_back(atof(l_str));
	#endif
			l_str = strtok(0,", ");
		}
		return l_NumeralList;
	}


	std::vector<float>	GetFloatListByCommaDivide(char*e_str,int e_iSize)
	{
		std::vector<float>	l_NumeralList;
		if( e_iSize )
			l_NumeralList.reserve(e_iSize);
		char*	l_str = strtok(e_str,", ");
		while(l_str)
		{
	#ifdef DEBUG
			float	l_f = (float)atof(l_str);
			l_NumeralList.push_back(l_f);
	#else
			l_NumeralList.push_back((float)atof(l_str));
	#endif
			l_str = strtok(0,", ");
		}
		return l_NumeralList;
	}

	std::vector<std::wstring>	GetWStringListByCommaDivide(const wchar_t*e_str)
	{
		size_t	l_iLength = wcslen(e_str);
		wchar_t*l_TempData = new wchar_t[l_iLength];
		memcpy(l_TempData,e_str,sizeof(wchar_t)*l_iLength);
		std::vector<std::wstring>	l_List;
		wchar_t*	l_str = wcstok(l_TempData,L", _");
		while(l_str)
		{
			l_List.push_back(l_str);
			l_str = wcstok(0,L", _");
		}
		delete l_TempData;
		return l_List;	
	}

	std::vector<std::string>	GetStringListByCommaDivide(const wchar_t*e_str)
	{
		size_t	l_iLength = wcslen(e_str);
		wchar_t*l_TempData = new wchar_t[l_iLength];
		memcpy(l_TempData,e_str,sizeof(wchar_t)*l_iLength);
		std::vector<std::string>	l_List;
		wchar_t*	l_str = wcstok(l_TempData,L", _");
		while(l_str)
		{
			l_List.push_back(UT::WcharToChar(l_str));
			l_str = wcstok(0,L", _");
		}
		return l_List;
	}

	std::wstring	ValueToStringW(eDataType e_eDataType)
	{
		if( e_eDataType == eDT_INT )
			return L"int";
		if( e_eDataType == eDT_FLOAT )
			return L"float";
		if( e_eDataType == eDT_BYTE )
			return L"byte";
		if( e_eDataType == eDT_DOUBLE  )
			return L"double";
		if( e_eDataType == eDT_WSTRING )
			return L"wstring";
		if( e_eDataType == eDT_STRING )
			return L"string";
		if( e_eDataType == eDT_VOID )
			return L"void";
		if( e_eDataType == eDT_POINT )
			return L"Point";
		if( e_eDataType == eDT_VECTOR2 )
			return L"Vector2";
		if( e_eDataType == eDT_VECTOR3 )
			return L"Vector3";
		if( e_eDataType == eDT_VECTOR4 )
			return L"Vector4";
		if( e_eDataType == eDT_INT64 )
			return L"int64";
		return L"unknow";
	}

	std::wstring	ValueToStringW(std::wstring	e_strValue)
	{
		return e_strValue;
	}
	std::wstring	ValueToStringW(const wchar_t*e_strValue)
	{
//#ifdef ANDROID
//		int	l_iLength = _countof(e_strValue);
//#else
//		int	l_iLength = wcslen(e_strValue);
//#endif
		std::wstring	l_str = e_strValue;
		return l_str;
	}
	std::wstring	ValueToStringW(std::string	e_strValue)
	{
		return ValueToStringW(e_strValue.c_str());
	}

	std::wstring	ValueToStringW(const char*e_strValue)
	{
		return ValueToStringW(UT::CharToWchar(e_strValue));
	}

	std::wstring	ValueToStringW(char e_cValue)
	{
		return UT::CharToWchar(ValueToString(e_cValue));
	}

	std::wstring	ValueToStringW(int e_iValue)
	{
		return UT::CharToWchar(ValueToString(e_iValue));
	}

	std::wstring	ValueToStringW(size_t e_iValue)
	{
		return UT::CharToWchar(ValueToString(e_iValue));
	}

	std::wstring	ValueToStringW(float e_fValue)
	{
		return UT::CharToWchar(ValueToString(e_fValue));
	}

	std::wstring	ValueToStringW(double e_fValue)
	{
		return UT::CharToWchar(ValueToString(e_fValue));
	}
	std::wstring	ValueToStringW(Vector2 e_vValue)
	{
		return UT::CharToWchar(ValueToString(e_vValue));
	}

	std::wstring	ValueToStringW(Vector3 e_vValue)
	{
		return UT::CharToWchar(ValueToString(e_vValue));
	}

	std::wstring	ValueToStringW(Vector4 e_vValue)
	{
		return UT::CharToWchar(ValueToString(e_vValue));
	}

	std::wstring	ValueToStringW(POINT e_Value)
	{
		return UT::CharToWchar(ValueToString(e_Value));
	}

	std::wstring	ValueToStringW(RECT e_Rect)
	{
		return UT::CharToWchar(ValueToString(e_Rect));
	}


	std::wstring	ValueToStringW(cMatrix44 e_mat)
	{
		return UT::CharToWchar(ValueToString(e_mat));
	}

	std::wstring	ValueToStringW(int64 e_uiValue )
	{
		return UT::CharToWchar(ValueToString(e_uiValue));
	}
#ifndef _WIN64
	std::wstring	ValueToStringW(uint64 e_uiValue )
	{
		return UT::CharToWchar(ValueToString(e_uiValue));
	}
#endif
	std::string	ValueToString(std::string e_strValue)
	{
		return ValueToString(e_strValue.c_str());
	}

	std::string	ValueToString(const char*e_strValue)
	{
		std::string	l_str = e_strValue;
		return l_str;
	}
	std::string	ValueToString(const char e_cValue)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%d\0",(int)e_cValue);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	ValueToString(int e_iValue)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%d\0",e_iValue);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	ValueToString(size_t e_iValue)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%d\0",e_iValue);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	ValueToString(float e_fValue)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%.2f\0",e_fValue);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	ValueToString(double e_fValue)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%.2f\0",(float)e_fValue);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	ValueToString(std::wstring e_strValue)
	{
		return UT::WcharToChar(e_strValue);
	}

	std::string	ValueToString(const wchar_t*e_strValue)
	{
		return UT::WcharToChar(e_strValue);
	}

	std::string	ValueToString(char*e_strValue)
	{
		std::string	l_str = e_strValue;
		return l_str;
	}

	std::string	ValueToString(Vector2 e_vValue)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%.2f,%.2f\0",e_vValue.x,e_vValue.y);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	ValueToString(Vector3 e_vValue)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%.2f,%.2f,%.2f\0",e_vValue.x,e_vValue.y,e_vValue.z);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	ValueToString(Vector4 e_vValue)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%.2f,%.2f,%.2f,%.2f\0",e_vValue.x,e_vValue.y,e_vValue.z,e_vValue.w);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	ValueToString(POINT e_Pos)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%d,%d\0",(int)e_Pos.x,(int)e_Pos.y);
		std::string	l_str = l_temp;
		return l_str;
	}
	std::string	ValueToString(RECT e_Rect)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%d,%d,%d,%d\0",e_Rect.left,e_Rect.top,e_Rect.right,e_Rect.bottom);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	ValueToString(cMatrix44 e_mat)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\0",
			e_mat.m[0][0],e_mat.m[1][0],e_mat.m[2][0],e_mat.m[3][0],
			e_mat.m[0][1],e_mat.m[1][1],e_mat.m[2][1],e_mat.m[3][1],
			e_mat.m[0][2],e_mat.m[1][2],e_mat.m[2][2],e_mat.m[3][2],
			e_mat.m[0][3],e_mat.m[1][3],e_mat.m[2][3],e_mat.m[3][3]);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	ValueToString(int64 e_uiValue )
	{
		//%x is 16.
		//%d is 10.
		//%u is unsigned int
		char l_temp[MAX_PATH];
		itoa64(e_uiValue,l_temp,10);
		std::string l_str = l_temp;
		return l_str;
	}
#ifndef _WIN64
	std::string	ValueToString(uint64 e_uiValue )
	{
		//%x is 16.
		//%d is 10.
		//%u is unsigned int
		char l_temp[MAX_PATH];
		itoa64(e_uiValue,l_temp,10);
		std::string l_str = l_temp;
		return l_str;
	}
#endif
	std::string	UVToString(float*e_pfUV)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%.7f,%.7f,%.7f,%.7f\0",e_pfUV[0],e_pfUV[1],e_pfUV[2],e_pfUV[3]);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	MatrixToStringWithIndex(cMatrix44 e_mat)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"0:%.5f,1:%.5f,2:%.5f,3:%.5f,4:%.5f,5:%.5f,6:%.5f,7:%.5f,8:%.5f,9:%.5f,10:%.5f,11:%.5f,12:%.5f,13:%.5f,14:%.5f,15:%.5f\0",
			e_mat.m[0][0],e_mat.m[0][1],e_mat.m[0][2],e_mat.m[0][3],
			e_mat.m[1][0],e_mat.m[1][1],e_mat.m[1][2],e_mat.m[1][3],
			e_mat.m[2][0],e_mat.m[2][1],e_mat.m[2][2],e_mat.m[2][3],
			e_mat.m[3][0],e_mat.m[3][1],e_mat.m[3][2],e_mat.m[3][3]);
		std::string	l_str = l_temp;
		return l_str;
	}

	std::string	Vector3VectorToString(std::vector<Vector3> *e_pvPosVector)
	{
		std::string	l_str;
		size_t	l_uiSize = e_pvPosVector->size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			l_str += ValueToString((*e_pvPosVector)[i]);
			if( i+1 != l_uiSize )
				l_str+=",";
		}
		return l_str;
	}

	std::string	Vector2VectorToString(std::vector<Vector2> *e_pvPosVector,int e_iSize)
	{
		std::string	l_str;
		size_t	l_uiSize = e_pvPosVector->size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			l_str += ValueToString((*e_pvPosVector)[i]);
			if( i+1 != l_uiSize )
				l_str+=",";
		}
		return l_str;	
	}

	std::vector<Vector2>	StringToVector2Vector(wchar_t*e_str,int e_iSize)
	{
		std::vector<Vector2>	l_NumeralList;
		if( e_iSize )
			l_NumeralList.reserve(e_iSize);
		wchar_t*	l_str = wcstok(e_str,L", ");
		while(l_str)
		{
			Vector2	l_v;
			l_v.x = (float)_wtof(l_str);
			l_str = wcstok(0,L", ");
			l_v.y = (float)_wtof(l_str);
			l_NumeralList.push_back(l_v);
			l_str = wcstok(0,L", ");
		}
		return l_NumeralList;
	}

	std::vector<Vector3>	StringToVector3Vector(wchar_t*e_str,int e_iSize)
	{
		std::vector<Vector3>	l_NumeralList;
		if( e_iSize )
			l_NumeralList.reserve(e_iSize);
		wchar_t*	l_str = wcstok(e_str,L", ");
		while(l_str)
		{
			Vector3	l_v;
			l_v.x = (float)_wtof(l_str);
			l_str = wcstok(0,L", ");
			l_v.y = (float)_wtof(l_str);
			l_str = wcstok(0,L", ");
			l_v.z = (float)_wtof(l_str);
			l_NumeralList.push_back(l_v);
			l_str = wcstok(0,L", ");
		}
		return l_NumeralList;
	}

	std::vector<Vector2>	StringToVector2Vector(char*e_str,int e_iSize)
	{
		std::vector<Vector2>	l_NumeralList;
		if( e_iSize )
			l_NumeralList.reserve(e_iSize);
		char*	l_str = strtok(e_str,", ");
		while(l_str)
		{
			Vector2	l_v;
			l_v.x = (float)atof(l_str);
			l_str = strtok(0,", ");
			l_v.y = (float)atof(l_str);
			l_NumeralList.push_back(l_v);
			l_str = strtok(0,", ");
		}
		return l_NumeralList;	
	}

	std::vector<Vector3>	StringToVector3Vector(char*e_str,int e_iSize)
	{
		std::vector<Vector3>	l_NumeralList;
		if( e_iSize )
			l_NumeralList.reserve(e_iSize);
		char*	l_str = strtok(e_str,", ");
		while(l_str)
		{
			Vector3	l_v;
			l_v.x = (float)atof(l_str);
			l_str = strtok(0,", ");
			l_v.y = (float)atof(l_str);
			l_str = strtok(0,", ");
			l_v.z = (float)atof(l_str);
			l_NumeralList.push_back(l_v);
			l_str = strtok(0,", ");
		}
		return l_NumeralList;	
	}


	eShapeType	GetShapeType(const char*e_str)
	{
		if( !strcmp(e_str,"RECT") )
		{
			return eST_RECT;
		}
		else
		if( !strcmp(e_str,"Sphere") )
		{
			return eST_Sphere;
		}
		return eST_MAX;
	}

	eShapeType				GetShapeType(const wchar_t*e_str)
	{
		return GetShapeType(UT::WcharToChar(e_str).c_str());
	}

	eFMAnimationRuleType	GetFMAnimationRuleType(const char*e_str)
	{
		if( !strcmp(e_str,"MPDI") )
		{
			return eFM_MPDI;
		}
		else
		if( !strcmp(e_str,"SubMPDI") )
		{
			return eFM_SUB_MPDI;
		}
		else
		if( !strcmp(e_str,"MPDIList") )
		{
			return eFM_MDPI_LIST;
		}
		else
		if( !strcmp(e_str,"Particle") )
		{
			return eFM_PARTICLE;
		}
		else
		if( !strcmp(e_str,"PRTG") )
		{
			return eFM_PARTICLE_GROUP;
		}
		return eFM_MAX;
	}
	eFMAnimationRuleType	GetFMAnimationRuleType( const wchar_t*e_str )
	{
		return GetFMAnimationRuleType( UT::WcharToChar(e_str).c_str() );
	}

	GLenum	IndexToBlendingIndex( int e_iIndex )
	{
		switch( e_iIndex )
		{
			case 0:
				return GL_ZERO;
			case 1:
				return GL_ONE;
			case 2:
				return GL_ONE_MINUS_SRC_COLOR;
			case 3:
				return GL_DST_COLOR;
			case 4:
				return GL_ONE_MINUS_DST_COLOR;
			case 5:
				return GL_SRC_ALPHA;
			case 6:
				return GL_ONE_MINUS_SRC_ALPHA;
			case 7:
				return GL_DST_ALPHA;
			case 8:
				return GL_ONE_MINUS_DST_ALPHA;
		}
		return e_iIndex;
		//return 0;
	}

	int BlendingIndexToIndex( GLenum e_eBlendingIndex)
	{
		switch( e_eBlendingIndex )
		{
			case GL_ZERO:
				return 0;
			case GL_ONE:
				return 1;
			case GL_ONE_MINUS_SRC_COLOR:
				return 2;
			case GL_DST_COLOR:
				return 3;
			case GL_ONE_MINUS_DST_COLOR:
				return 4;
			case GL_SRC_ALPHA:
				return 5;
			case GL_ONE_MINUS_SRC_ALPHA:
				return 6;
			case GL_DST_ALPHA:
				return 7;
			case GL_ONE_MINUS_DST_ALPHA:
				return 8;
		}
		assert(0&&"combox has not follow rule");
		return -1;
	}

}