#include "StringToStructure.h"
#include <assert.h>
#ifdef LINUX
#include <alloca.h>
#endif
#ifdef WIN32
#pragma warning( disable : 4996 )
#endif
wchar_t* g_pStringForWCSTOK = nullptr;
namespace FATMING_CORE
{
	POINT	GetPoint(const wchar_t* e_str)
	{
		auto l_str = UT::WcharToChar(e_str);
		return GetPoint(l_str.c_str());
	}

	POINT	GetPoint(const char* e_str)
	{
		char	l_temp[TEMP_SIZE];
		char*	l_pForStrtok_s = nullptr;
		sprintf(l_temp, "%s", e_str);
		POINT	l_Point;
		char*	l_str = strtok_s(l_temp, ",: ",&l_pForStrtok_s);
		assert(l_str&&"parse POINT data error 1");
		l_Point.x = atol(l_str);
		l_str = strtok_s(nullptr, ",: ", &l_pForStrtok_s);
		assert(l_str&&"parse POINT data error 2");
		l_Point.y = atol(l_str);
		return l_Point;
	}

	POINT	GetPoint(std::string e_str){return GetPoint(e_str.c_str());}
	POINT	GetPoint(std::wstring e_str){return GetPoint(e_str.c_str());}

	float	GetFloat(const char* e_str){return (float)atof(e_str);}
	float	GetFloat(const wchar_t* e_str){return GetFloat(UT::WcharToChar(e_str).c_str());}
	float	GetFloat(std::string e_str){ return GetFloat(e_str.c_str()); }
	float	GetFloat(std::wstring e_str){ return GetFloat(e_str.c_str()); }

	double	GetDouble(const char* e_str) { return atof(e_str); }
	double	GetDouble(const wchar_t* e_str) { return GetDouble(UT::WcharToChar(e_str).c_str()); }
	double	GetDouble(std::string e_str) { return GetDouble(e_str.c_str()); }
	double	GetDouble(std::wstring e_str) { return GetDouble(e_str.c_str()); }

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
		size_t		l_iLength = wcslen(e_pString);
		wchar_t*	l_strTemp = nullptr;
		wchar_t*	l_pForwcstok_s = nullptr;
		wchar_t*	l_pTempWString = (wchar_t*)alloca(sizeof(wchar_t) * (l_iLength + 1));
		memcpy(l_pTempWString, e_pString, sizeof(wchar_t) * l_iLength);
		l_pTempWString[l_iLength] = 0;
		wchar_t*	l_pCurrent = wcstok_s(l_pTempWString,L",:",&l_pForwcstok_s);
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
				l_pCurrent = wcstok_s(nullptr,L",;", &l_pForwcstok_s);
			}
			l_pRect[i] = l_rect;
		}
		assert(!l_pCurrent&&"rect data is wrong in the TextureLoad");
		return l_pRect;
	}


	Vector4	GetVector4(const char*e_strValue)
	{
	    char    l_strValue[TEMP_SIZE];
		char*	l_pForStrtok_s = nullptr;
	    sprintf(l_strValue,"%s",e_strValue);
		Vector4	l_Vector4;
		char*	l_str = strtok_s(l_strValue,",: ",&l_pForStrtok_s);
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.x = (float)atof(l_str);
		l_str = strtok_s(nullptr,",: ", &l_pForStrtok_s);
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.y = (float)atof(l_str);
		l_str = strtok_s(nullptr,",:", &l_pForStrtok_s);
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.z = (float)atof(l_str);
		l_str = strtok_s(nullptr, ",:", &l_pForStrtok_s);
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.w = (float)atof(l_str);
		return l_Vector4;
	}

	Vector3	GetVector3(const char*e_str)
	{
	    char    l_strValue[TEMP_SIZE];
		char*	l_pForStrtok_s = nullptr;
	    sprintf(l_strValue,"%s",e_str);
		Vector3	l_Vector3(0.f,0.f,0.f);
		char*	l_str = strtok_s(l_strValue,",: ",&l_pForStrtok_s);
		assert(l_str&&"parse Vector3 data error");
		l_Vector3.x = (float)atof(l_str);
		l_str = strtok_s(nullptr, ",: ", &l_pForStrtok_s);
		assert(l_str&&"parse Vector3 data error");
		l_Vector3.y = (float)atof(l_str);
		l_str = strtok_s(nullptr, ",:", &l_pForStrtok_s);
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
		char*	l_pForStrtok_s = nullptr;
	    sprintf(l_strValue,"%s",e_strValue);
		Vector2	l_Vector2;
		char*	l_str = strtok_s(l_strValue,",: ", &l_pForStrtok_s);
		assert(l_str&&"parse Vector2 data error");
		l_Vector2.x = (float)atof(l_str);
		l_str = strtok_s(nullptr, ",: ", &l_pForStrtok_s);
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
		char*	l_pForStrtok_s = nullptr;
	    sprintf(l_strValue,"%s",e_str);	
		cMatrix44	l_mat;
		char*	l_str = strtok_s((char*)l_strValue,",: ", &l_pForStrtok_s);
		for( int i=0;i<4;++i )
		{
			for( int j=0;j<4;++j )
			{
				l_mat.m[j][i] = (float)atof(l_str);
				l_str = strtok_s(nullptr, ",: ", &l_pForStrtok_s);
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
		delete[] l_str;
		return l_pMartices;
	}
    //this might lost data if parse string again!
	cMatrix44	*GetMatrcies(const char*e_str,int e_iSize,bool e_bTranspose)
	{
		char    l_strValue[TEMP_SIZE];
		char*	l_pForStrtok_s = nullptr;
		sprintf(l_strValue, "%s", e_str);
		cMatrix44*l_pMat = new cMatrix44[e_iSize];
		char*	l_str = strtok_s(l_strValue,",: ", &l_pForStrtok_s);
		for( int i=0;i<e_iSize;++i )
		{
			for( int j=0;j<4;++j )
			{
				for( int k=0;k<4;++k )
				{
					(l_pMat[i]).m[k][j] = (float)atof(l_str);
					l_str = strtok_s(nullptr, ",: ", &l_pForStrtok_s);
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
		char*	l_pForStrtok_s = nullptr;
		sprintf(l_temp,"%s",e_pData);
		char*	l_str = strtok_s(l_temp,",: ", &l_pForStrtok_s);
		for( int i=0;i<4;++i )
		{
			e_pUVBuffer[i] = (float)atof(l_str);
			l_str = strtok_s(nullptr, ",: ", &l_pForStrtok_s);
		}
	}

	void	GetUV(const char*e_pData,float*e_pUVBuffer)
	{
		GetUV((char*)e_pData,e_pUVBuffer);
	}

	std::vector<int>	GetIntegerListByCommaDivide(const wchar_t*e_str,int e_iSize)
	{
		wchar_t*					l_pForwcstok_s = nullptr;
		std::vector<int>			l_NumeralList;
		size_t	l_iLength =			wcslen(e_str);
		wchar_t* l_pTempWString = (wchar_t*)alloca(sizeof(wchar_t) * (l_iLength + 1));
		memcpy(l_pTempWString, e_str, sizeof(wchar_t) * l_iLength);
		l_pTempWString[l_iLength] = 0;
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		wchar_t*	l_str = wcstok_s(l_pTempWString,L", ",&l_pForwcstok_s);
		while(l_str)
		{
	#ifdef DEBUG
			int	l_i = _wtoi(l_str);
			l_NumeralList.push_back(l_i);
	#else
			l_NumeralList.push_back(_wtoi(l_str));
	#endif
			l_str = wcstok_s(nullptr,L", ", &l_pForwcstok_s);
		}
		return l_NumeralList;	
	}


	std::vector<float>	GetFloatListByCommaDivide(const wchar_t*e_str,int e_iSize)
	{
		size_t	l_iLength = wcslen(e_str);
		wchar_t* l_pForwcstok_s = nullptr;
		wchar_t* l_pTempWString = (wchar_t*)alloca(sizeof(wchar_t) * (l_iLength + 1));
		memcpy(l_pTempWString, e_str, sizeof(wchar_t) * l_iLength);
		l_pTempWString[l_iLength] = 0;
		std::vector<float>	l_NumeralList;
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		wchar_t*	l_str = wcstok_s(l_pTempWString,L", ",&l_pForwcstok_s);
		while(l_str)
		{
	#ifdef DEBUG
			float	l_f = (float)_wtof(l_str);
			l_NumeralList.push_back(l_f);
	#else
			l_NumeralList.push_back((float)_wtof(l_str));
	#endif
			l_str = wcstok_s(nullptr,L", ",&l_pForwcstok_s);
		}
		return l_NumeralList;
	}

	std::vector<double>	GetDoubleListByCommaDivide(const wchar_t*e_str,int e_iSize)
	{
		size_t	l_iLength = wcslen(e_str);
		wchar_t* l_pForwcstok_s = nullptr;
		wchar_t* l_pTempWString = (wchar_t*)alloca(sizeof(wchar_t) * (l_iLength + 1));
		memcpy(l_pTempWString, e_str, sizeof(wchar_t) * l_iLength);
		l_pTempWString[l_iLength] = 0;
		std::vector<double>	l_NumeralList;
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		wchar_t*	l_str = wcstok_s(l_pTempWString,L", ",&l_pForwcstok_s);
		while(l_str)
		{
	#ifdef DEBUG
			double	l_db = _wtof(l_str);
			l_NumeralList.push_back(l_db);
	#else
			l_NumeralList.push_back(_wtof(l_str));
	#endif
			l_str = wcstok_s(nullptr,L", " , &l_pForwcstok_s);
		}
		return l_NumeralList;
	}

	std::vector<int64>	GetInt64ListByCommaDivide(const wchar_t*e_str, int e_iSize)
	{
		size_t	l_iLength = wcslen(e_str);
		wchar_t* l_pForwcstok_s = nullptr;
		wchar_t* l_pTempWString = (wchar_t*)alloca(sizeof(wchar_t) * (l_iLength + 1));
		memcpy(l_pTempWString, e_str, sizeof(wchar_t) * l_iLength);
		l_pTempWString[l_iLength] = 0;
		std::vector<int64>	l_NumeralList;
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		wchar_t*	l_str = wcstok_s(l_pTempWString, L", ",&l_pForwcstok_s);
		while (l_str)
		{
			int64 l_i64Value =  GetInt64(l_str);
			l_NumeralList.push_back(l_i64Value);
			l_str = wcstok_s(nullptr , L", ", &l_pForwcstok_s);
		}
		return l_NumeralList;
	}

	std::vector<int>	GetIntegerListByCommaDivide(const char*e_str,int e_iSize)
	{
		std::vector<int>	l_NumeralList;
		char*				l_pForStrtok_s = nullptr;
		char*				l_strValue = (char*)alloca(strlen(e_str));
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		sprintf(l_strValue, "%s", e_str);
		char*	l_str = strtok_s(l_strValue,", ",&l_pForStrtok_s);
		while(l_str)
		{
	#ifdef DEBUG
			int	l_i = atoi(l_str);
			l_NumeralList.push_back(l_i);
	#else
			l_NumeralList.push_back(atoi(l_str));
	#endif
			l_str = strtok_s(nullptr, ", ", &l_pForStrtok_s);
		}
		return l_NumeralList;
	}

	std::vector<double>	GetDoubleListByCommaDivide(const char*e_str,int e_iSize)
	{
		std::vector<double>	l_NumeralList;
		char* l_pForStrtok_s = nullptr;
		char* l_strValue = (char*)alloca(strlen(e_str));
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		sprintf(l_strValue, "%s", e_str);
		char*	l_str = strtok_s(l_strValue,", ",&l_pForStrtok_s);
		while(l_str)
		{
	#ifdef DEBUG
			double	l_db = atof(l_str);
			l_NumeralList.push_back(l_db);
	#else
			l_NumeralList.push_back(atof(l_str));
	#endif
			l_str = strtok_s(nullptr, ", ", &l_pForStrtok_s);
		}
		return l_NumeralList;
	}

	std::vector<int64>	GetInt64ListByCommaDivide(const char*e_str, int e_iSize)
	{
		std::vector<int64>	l_NumeralList;
		char*				l_strValue = (char*)alloca(strlen(e_str));
		char*				l_pForStrtok_s = nullptr;
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		sprintf(l_strValue, "%s", e_str);
		char*	l_str = strtok_s(l_strValue, ", ", &l_pForStrtok_s);
		while (l_str)
		{
			int64 l_i64 = GetInt64(l_str);
			l_NumeralList.push_back(l_i64);
			l_str = strtok_s(nullptr, ", ", &l_pForStrtok_s);
		}
		return l_NumeralList;
	}

	std::vector<float>	GetFloatListByCommaDivide(const char*e_str,int e_iSize)
	{
		std::vector<float>	l_NumeralList;
		char*				l_strValue = (char*)alloca(strlen(e_str));
		char*				l_pForStrtok_s = nullptr;
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		sprintf(l_strValue, "%s", e_str);
		char*	l_str = strtok_s(l_strValue, ", ", &l_pForStrtok_s);
		while(l_str)
		{
	#ifdef DEBUG
			float	l_f = (float)atof(l_str);
			l_NumeralList.push_back(l_f);
	#else
			l_NumeralList.push_back((float)atof(l_str));
	#endif
			l_str = strtok_s(nullptr, ", ", &l_pForStrtok_s);
		}
		return l_NumeralList;
	}

	std::vector<std::wstring>	GetWStringListByCommaDivide(const wchar_t*e_str)
	{
		std::vector<std::wstring>	l_List;
		size_t	l_iLength = wcslen(e_str);
		wchar_t*	l_pForwcstok_s = nullptr;
		wchar_t*	l_pTempWString = (wchar_t*)alloca(sizeof(wchar_t) * (l_iLength +1));
		memcpy(l_pTempWString,e_str,sizeof(wchar_t)*l_iLength);
		l_pTempWString[l_iLength] = 0;
		wchar_t*	l_str = wcstok_s(l_pTempWString,L", ",&l_pForwcstok_s);
		while(l_str)
		{
			l_List.push_back(l_str);
			l_str = wcstok_s(nullptr,L", ",&l_pForwcstok_s);
		}
		return l_List;	
	}

	std::vector<std::string>	GetStringListByCommaDivide(const wchar_t*e_str)
	{
		size_t	l_iLength = wcslen(e_str);
		wchar_t* l_pForwcstok_s = nullptr;
		wchar_t*l_TempData = new wchar_t[l_iLength+1];
		memcpy(l_TempData,e_str,sizeof(wchar_t)*l_iLength);
		l_TempData[l_iLength] = 0;
		std::vector<std::string>	l_List;
		wchar_t*	l_str = wcstok_s(l_TempData,L", ", &l_pForwcstok_s);
		while(l_str)
		{
			l_List.push_back(UT::WcharToChar(l_str));
			l_str = wcstok_s(nullptr,L", ", &l_pForwcstok_s);
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
		if( e_eDataType == eDT_UINT64 )
			return L"uint64";
		return L"unknow";
	}


	eDataType		StringToDataType(const char*e_str)
	{
		if( !strcmp("char",e_str) || !strcmp("byte",e_str) )
			return eDT_BYTE;
		if( !strcmp("float",e_str) )
			return eDT_FLOAT;
		if( !strcmp("double",e_str) )
			return eDT_DOUBLE;
		if( !strcmp("int",e_str) )
			return eDT_INT;
		if( !strcmp("string",e_str) )
			return eDT_STRING;
		if( !strcmp("wstring",e_str) )
			return eDT_WSTRING;
		if( !strcmp("Vector2",e_str) )
			return eDT_VECTOR2;
		if( !strcmp("Vector3",e_str) )
			return eDT_VECTOR3;
		if( !strcmp("Vector4",e_str) )
			return eDT_VECTOR4;
		if( !strcmp("Point",e_str) )
			return eDT_POINT;
		if( !strcmp("void",e_str) )
			return eDT_VOID;
		if( !strcmp("int64",e_str) )
			return eDT_INT64;
		if( !strcmp("uint64",e_str) )
			return eDT_UINT64;
		return eDT_MAX;
	}

	std::wstring	ValueToStringW(eMoveDirection e_eMoveDirection)
	{
		std::wstring l_strResult;
		switch(e_eMoveDirection)
		{
			case eMD_DOWN_TO_UP:
				l_strResult = L"DownToUp";
			break;
			case eMD_LEFT_TO_RIGHT:
				l_strResult = L"LeftToRight";
			break;
			case eMD_RIGHT_TO_LEFT:
				l_strResult = L"RightToLeft";
			break;
			case eMD_UP_TO_DOWN:
				l_strResult = L"UpToDown";
			break;
			case eMD_STAY:
				l_strResult = L"Stay";
			break;
			default:
				l_strResult = L"unknow move direction";
			break;
		}
		return l_strResult;
	}

	eMoveDirection	StringToMoveDirection(const wchar_t*e_str)
	{
		return StringToMoveDirection(UT::WcharToChar(e_str).c_str());
	}

	eMoveDirection	StringToMoveDirection(const char*e_str)
	{
		if( !strcmp("DownToUp",e_str))
			return eMD_DOWN_TO_UP;
		if( !strcmp("LeftToRight",e_str))
			return eMD_LEFT_TO_RIGHT;
		if( !strcmp("RightToLeft",e_str) )
			return eMD_RIGHT_TO_LEFT;
		if( !strcmp("UpToDown",e_str) )
			return eMD_UP_TO_DOWN;
		if( !strcmp("Stay",e_str) )
			return 		eMD_STAY;
		return eMD_MAX;
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
		return UT::CharToWchar(e_strValue);
	}

	std::wstring	ValueToStringW(char e_cValue)
	{
		return UT::CharToWchar(ValueToString(e_cValue));
	}

	std::wstring	ValueToStringW(int e_iValue)
	{
		return UT::CharToWchar(ValueToString(e_iValue));
	}

	std::wstring	ValueToStringW(unsigned int e_iValue)
	{
		return UT::CharToWchar(ValueToString(e_iValue));
	}
#if !defined(ANDROID) && !defined(LINUX)
	std::wstring	ValueToStringW(size_t e_iValue)
	{
		return UT::CharToWchar(ValueToString(e_iValue));
	}
#endif
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
	//http://www.cplusplus.com/reference/cstdio/printf/
	std::string		ValueToString(unsigned int e_iValue)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp, "%u\0", e_iValue);
		std::string	l_str = l_temp;
		return l_str;
	}
#if !defined(ANDROID) && !defined(LINUX)
	std::string	ValueToString(size_t e_iValue)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%zu\0",e_iValue);
		std::string	l_str = l_temp;
		return l_str;
	}
#endif
	std::string	ValueToString(float e_fValue)
	{
		char	l_temp[TEMP_SIZE];
		sprintf(l_temp,"%.5f\0",e_fValue);
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

	std::vector<Vector2>	StringToVector2Vector(const wchar_t*e_str,int e_iSize)
	{
		size_t	l_iLength = wcslen(e_str);
		wchar_t* l_pForwcstok_s = nullptr;
		wchar_t* l_pTempWString = (wchar_t*)alloca(sizeof(wchar_t) * (l_iLength + 1));
		memcpy(l_pTempWString, e_str, sizeof(wchar_t) * l_iLength);
		l_pTempWString[l_iLength] = 0;
		std::vector<Vector2>	l_NumeralList;
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		wchar_t*	l_str = wcstok_s(l_pTempWString,L", ",&l_pForwcstok_s);
		while(l_str)
		{
			Vector2	l_v;
			l_v.x = (float)_wtof(l_str);
			l_str = wcstok_s(nullptr,L", ", &l_pForwcstok_s);
			l_v.y = (float)_wtof(l_str);
			l_NumeralList.push_back(l_v);
			l_str = wcstok_s(nullptr,L", ", &l_pForwcstok_s);
		}
		return l_NumeralList;
	}

	std::vector<Vector3>	StringToVector3Vector(const wchar_t*e_str,int e_iSize)
	{
		size_t	l_iLength = wcslen(e_str);
		wchar_t* l_pForwcstok_s = nullptr;
		wchar_t* l_pTempWString = (wchar_t*)alloca(sizeof(wchar_t) * (l_iLength + 1));
		memcpy(l_pTempWString, e_str, sizeof(wchar_t) * l_iLength);
		l_pTempWString[l_iLength] = 0;
		std::vector<Vector3>	l_NumeralList;
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		wchar_t*	l_str = wcstok_s(l_pTempWString,L", ",&l_pForwcstok_s);
		while(l_str)
		{
			Vector3	l_v;
			l_v.x = (float)_wtof(l_str);
			l_str = wcstok_s(nullptr,L", ", &l_pForwcstok_s);
			l_v.y = (float)_wtof(l_str);
			l_str = wcstok_s(nullptr,L", ", &l_pForwcstok_s);
			l_v.z = (float)_wtof(l_str);
			l_NumeralList.push_back(l_v);
			l_str = wcstok_s(nullptr , L", ", &l_pForwcstok_s);
		}
		return l_NumeralList;
	}

	std::vector<Vector2>	StringToVector2Vector(const char*e_str,int e_iSize)
	{
		std::vector<Vector2>	l_NumeralList;
		char* l_strValue = (char*)alloca(strlen(e_str));
		char* l_pForStrtok_s = nullptr;
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		sprintf(l_strValue, "%s", e_str);
		char*	l_str = strtok_s(l_strValue, ", ", &l_pForStrtok_s);
		while(l_str)
		{
			Vector2	l_v;
			l_v.x = (float)atof(l_str);
			l_str = strtok_s(nullptr, ", ", &l_pForStrtok_s);
			l_v.y = (float)atof(l_str);
			l_NumeralList.push_back(l_v);
			l_str = strtok_s(nullptr, ", ", &l_pForStrtok_s);
		}
		return l_NumeralList;	
	}

	std::vector<Vector3>	StringToVector3Vector(const char*e_str,int e_iSize)
	{
		std::vector<Vector3>	l_NumeralList;
		char*					l_strValue = (char*)alloca(strlen(e_str));
		char*					l_pForStrtok_s = nullptr;
		if (e_iSize)
		{
			l_NumeralList.reserve(e_iSize);
		}
		sprintf(l_strValue, "%s", e_str);
		char*	l_str = strtok_s(l_strValue, ", ", &l_pForStrtok_s);
		while(l_str)
		{
			Vector3	l_v;
			l_v.x = (float)atof(l_str);
			l_str = strtok_s(nullptr, ", ", &l_pForStrtok_s);
			l_v.y = (float)atof(l_str);
			l_str = strtok_s(nullptr, ", ", &l_pForStrtok_s);
			l_v.z = (float)atof(l_str);
			l_NumeralList.push_back(l_v);
			l_str = strtok_s(nullptr, ", ", &l_pForStrtok_s);
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
}