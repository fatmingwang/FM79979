#pragma once

#include "CommonDefine.h"
#include "Utility.h"
#include "VS_TO_GNN.h"

#include <vector>
#include <string>
#include <wchar.h>
#include "../XML/tinyxml.h"
#include "../Math/Vector4.h"
#include "../Math/Matrix44.h"
class	TiXmlElement;
namespace FATMING_CORE
{
	//x,y
	//"100,200"
	POINT	GetPoint(const char* e_str);
	POINT	GetPoint(const wchar_t* e_str);
	POINT	GetPoint(std::wstring e_str);
	POINT	GetPoint(std::string e_str);

	float	GetFloat(const char* e_str);
	float	GetFloat(const wchar_t* e_str);
	float	GetFloat(std::wstring e_str);
	float	GetFloat(std::string e_str);

	float	GetDouble(const char* e_str);
	float	GetDouble(const wchar_t* e_str);
	float	GetDouble(std::wstring e_str);
	float	GetDouble(std::string e_str);

	bool	GetBoolean(const char* e_str);
	bool	GetBoolean(const wchar_t* e_str);
	bool	GetBoolean(std::wstring e_str);
	bool	GetBoolean(std::string e_str);


	int		GetInt(const char* e_str);
	int		GetInt(const wchar_t* e_str);
	int		GetInt(std::wstring e_str);
	int		GetInt(std::string e_str);

	uint64	GetUint64(const wchar_t* e_str);
	uint64	GetUint64(const char* e_str);
	uint64	GetUint64(std::wstring e_str);
	uint64	GetUint64(std::string e_str);

	int64	GetInt64(const wchar_t* e_str);
	int64	GetInt64(const char* e_str);
	int64	GetInt64(std::wstring e_str);
	int64	GetInt64(std::string e_str);
	//left top right bottom
	//"0,0,960,540"
	RECT	GetRectangle(const wchar_t*e_pString);
	RECT	GetRectangle(const char*e_pString);
	RECT	GetRectangle(std::wstring e_str);
	RECT	GetRectangle(std::string e_str);
	RECT*	GetRectangle(int e_iNumRect,wchar_t*e_pString);

	Vector4	GetVector4(const char*e_strValue);
	Vector3	GetVector3(const char*e_strValue);
	Vector2	GetVector2(const char*e_strValue);
	Vector4	GetVector4(const wchar_t*e_strValue);
	Vector3	GetVector3(const wchar_t*e_strValue);
	Vector2	GetVector2(const wchar_t*e_strValue);

	std::vector<Vector2>	GetVector2Vector(TiXmlElement*e_pTiXmlElement);
	std::vector<Vector3>	GetVector3Vector(TiXmlElement*e_pTiXmlElement);
	std::vector<Vector4>	GetVector4Vector(TiXmlElement*e_pTiXmlElement);

	//ensure column order or row order
	cMatrix44	GetMatrix(const char*e_str,bool e_bTranspose = false);
	cMatrix44	GetMatrix(const wchar_t*e_str,bool e_bTranspose = false);
	//this might lost data if parse string again!
	cMatrix44	*GetMatrcies(const char*e_str,int e_iSize,bool e_bTranspose = false);
	//this might lost data if parse string again!
	cMatrix44	*GetMatrcies(const wchar_t*e_str,int e_iSize,bool e_bTranspose = false);


	void	GetUV(char*e_pData,float*e_pUVBuffer);
	void	GetUV(const char*e_pData,float*e_pUVBuffer);
	void	GetUV(const wchar_t*e_pData,float*e_pUVBuffer);


	//input size if u know it,it will reserve the size for vector faster
	std::vector<int>	GetIntegerListByCommaDivide(const char*,int e_iSize);
	std::vector<float>	GetFloatListByCommaDivide(const char*,int e_iSize);
	std::vector<double>	GetDoubleListByCommaDivide(const char*,int e_iSize);
	std::vector<int64>	GetInt64ListByCommaDivide(const char*, int e_iSize);

	std::vector<int>	GetIntegerListByCommaDivide(const wchar_t*,int e_iSize);
	std::vector<float>	GetFloatListByCommaDivide(const wchar_t*,int e_iSize);
	std::vector<double>	GetDoubleListByCommaDivide(const wchar_t*,int e_iSize);
	std::vector<int64>	GetInt64ListByCommaDivide(const wchar_t*, int e_iSize);

	std::vector<std::wstring>	GetWStringListByCommaDivide(const wchar_t*e_str);
	std::vector<std::string>	GetStringListByCommaDivide(const wchar_t*e_str);
	//inpput data and size to generation,only support common type(int float char double....not support structure)
	//although it could add a callback function make it better but slow.
	template<class T>T*	ParseDataToGenerateIntPointer(const char*e_strData,int e_iSize)
	{
		T	*l_pData = new	T[e_iSize];
		char*   l_strValue = (char*)alloca(strlen(e_strData));
		sprintf(l_strValue, "%s", e_strData);
		char*	l_str = strtok(l_strValue, ", ");
		int	l_iStep = 0;
		while(l_str)
		{
			l_pData[l_iStep] = (T)atof(l_str);
			l_str = strtok(0,", ");
			++l_iStep;
		}
		assert(l_iStep == e_iSize&&"ParseDataToGenerateIntPointer not matched!");
		return l_pData;
	}
	//for
	template<class T>T*	ParseDataToGenerateIntPointer(const wchar_t*e_strData,int e_iSize)
	{
		std::string	l_str = UT::WcharToChar(e_strData);
		return ParseDataToGenerateIntPointer<T>(l_str.c_str(),e_iSize);
//#ifdef ANDROID
//		int	l_iLength = wcslen(e_strData);
//		char*l_strData = new char[l_iLength+1];
//		for(int i=0;i<l_iLength;++i)
//			l_strData[i] = (char)(e_strData[i]);
//		l_strData[l_iLength] = '\0';
//		T	*l_pData = ParseDataToGenerateIntPointer<T>(l_strData,e_iSize);
//		delete l_strData;
//#else
//		T	*l_pData = new	T[e_iSize];
//		wchar_t*	l_str = wcstok((wchar_t*)e_strData,L", ");
//		int	l_iStep = 0;
//		while(l_str)
//		{
//			l_pData[l_iStep] = (T)_wtof(l_str);
//			l_str = wcstok(0,L", ");
//			++l_iStep;
//		}
//		//avoid size is not match
//		assert(e_iSize == l_iStep);
//#endif
//		return l_pData;
	}
	//performance is not good enough...by sometimes we just need it
	template<class T>std::vector<T>	GetValueListByCommaDivide(const wchar_t*e_str)
	{
		std::vector<T>	l_NumeralList;
		if (e_str)
		{
			size_t l_uiSize = wcslen(e_str);
			wchar_t*l_strTemp = new wchar_t[l_uiSize +1];
			memcpy(l_strTemp, e_str, sizeof(wchar_t)*l_uiSize);
 			l_strTemp[l_uiSize] = 0;
			wchar_t*	l_str = wcstok(l_strTemp, L", ");
			while (l_str)
			{
#ifdef DEBUG
				T	l_i = (T)_wtof(l_str);
				l_NumeralList.push_back(l_i);
#else
				l_NumeralList.push_back((T)_wtof(l_str));
#endif
				l_str = wcstok(0, L", ");
			}
			delete[] l_strTemp;
		}
		return l_NumeralList;
	}
	//
	std::wstring	ValueToStringW(eDataType e_eDataType);
	eDataType		StringToDataType(const char*e_str);
	std::wstring	ValueToStringW(eMoveDirection e_eMoveDirection);
	eMoveDirection	StringToMoveDirection(const char*e_str);
	eMoveDirection	StringToMoveDirection(const wchar_t*e_str);

	std::wstring	ValueToStringW(std::wstring	e_strValue);
	std::wstring	ValueToStringW(std::string	e_strValue);
	std::wstring	ValueToStringW(const wchar_t*e_strValue);
	std::wstring	ValueToStringW(char e_cValue);
	std::wstring	ValueToStringW(const char*e_strValue);
	std::wstring	ValueToStringW(int e_iValue);
	std::wstring	ValueToStringW(unsigned int e_iValue);
	std::wstring	ValueToStringW(float e_fValue);
	std::wstring	ValueToStringW(double e_fValue);
	std::wstring	ValueToStringW(Vector2 e_vValue);
	std::wstring	ValueToStringW(Vector3 e_vValue);
	std::wstring	ValueToStringW(Vector4 e_vValue);
	std::wstring	ValueToStringW(POINT e_Value);
	std::wstring	ValueToStringW(RECT e_Rect);
	std::wstring	ValueToStringW(cMatrix44 e_mat);
	std::wstring	ValueToStringW(int64 e_uiValue );
	std::wstring	ValueToStringW(uint64 e_uiValue );
	std::wstring	ValueToStringW(size_t e_iValue);
	//
	std::string		ValueToString(std::wstring e_strValue);
	std::string		ValueToString(std::string e_strValue);
	std::string		ValueToString(const wchar_t*e_strValue);
	std::string		ValueToString(const char*e_strValue);
	std::string		ValueToString(const char e_cValue);
	std::string		ValueToString(int	e_iValue);
	std::string		ValueToString(unsigned int e_iValue);
	std::string		ValueToString(float e_fValue);
	std::string		ValueToString(double e_fValue);
	std::string		ValueToString(Vector2 e_vValue);
	std::string		ValueToString(Vector3 e_vValue);
	std::string		ValueToString(Vector4 e_vValue);
	std::string		ValueToString(POINT e_Value);
	std::string		ValueToString(RECT e_Rect);
	std::string		ValueToString(cMatrix44 e_mat);
	std::string		ValueToString(int64 e_uiValue );
	std::string		ValueToString(uint64 e_uiValue );
	std::string		ValueToString(size_t e_iValue);

	std::string		UVToString(float*e_pfUV);

	std::string		MatrixToStringWithIndex(cMatrix44 e_mat);

	std::string				Vector3VectorToString(std::vector<Vector3>* e_pvPosVector);
	std::string				Vector2VectorToString(std::vector<Vector2>* e_pvPosVector);
	std::vector<Vector2>	StringToVector2Vector(const wchar_t*e_str,int e_iSize = 0);
	std::vector<Vector3>	StringToVector3Vector(const wchar_t*e_str,int e_iSize = 0);
	std::vector<Vector2>	StringToVector2Vector(const char*e_str,int e_iSize = 0);
	std::vector<Vector3>	StringToVector3Vector(const char*e_str,int e_iSize = 0);

	enum	eShapeType
	{
		eST_RECT = 0,
		eST_Sphere,
		eST_MAX,
	};

	enum	eFMAnimationRuleType
	{
		eFM_MPDI = 0,
		eFM_SUB_MPDI,
		eFM_MDPI_LIST,
		eFM_PARTICLE,
		eFM_PARTICLE_GROUP,
		eFM_MAX,
	};

	eShapeType				GetShapeType(const char*e_str);
	eShapeType				GetShapeType(const wchar_t*e_str);
	eFMAnimationRuleType	GetFMAnimationRuleType(const char*e_str);
	eFMAnimationRuleType	GetFMAnimationRuleType(const wchar_t*e_str);

	template<class T>std::string	ValueToString(std::vector<T>e_Vector)
	{
		std::string	l_strResult;
		size_t	l_iSize = e_Vector.size();
		for( size_t i=0;i<l_iSize;++i )
		{
			l_strResult += ValueToString(e_Vector[i]);
			if( i != l_iSize-1 )
				l_strResult += ",";
		}
		return l_strResult;	
	}
	template<class T>std::wstring	ValueToStringW(std::vector<T>e_Vector)
	{
		std::string		l_strResult2 = ValueToString(e_Vector);
		return ValueToStringW(l_strResult2);
	}
}
using namespace FATMING_CORE;