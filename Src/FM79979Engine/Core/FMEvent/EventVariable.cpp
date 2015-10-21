#include "../stdafx.h"
#include "EventVariable.h"

namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cEventVariableList);
	TYPDE_DEFINE_MARCO(cEventVariable);
	cEventVariable::cEventVariable()
	{
		m_pData = 0;
		m_eDataType = eDT_MAX;
	}

	cEventVariable::cEventVariable(const WCHAR*e_strName,eDataType e_eDataType,const WCHAR*e_strValue)
	{
		m_pData = 0;
		SetDataByString(e_strValue,e_eDataType);
		this->SetName(e_strName);
	}
	//<Variable Name="" DataType="" Value="" />
	cEventVariable::cEventVariable(TiXmlElement*e_pTiXmlElement)
	{
		m_pData = 0;
		m_eDataType = eDT_MAX;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventVariable::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME("Type")
			{
				m_eDataType = UT::GetDataType(l_strValue);
			}
			else
			COMPARE_NAME("Value")
			{
				SetDataByString(l_strValue,m_eDataType);
			}
		PARSE_NAME_VALUE_END
	}

	cEventVariable::~cEventVariable()
	{
		Destroy();
	}

	//void	cEventVariable::SetDataType(eDataType e_eDataType)
	//{
	//
	//}

	void	cEventVariable::Destroy()
	{
		if( m_eDataType == eDT_WSTRING )
		{
			std::wstring	*l_str = (std::wstring*)m_pData;
			SAFE_DELETE(l_str);
		}
		else
		if( m_eDataType == eDT_STRING )
		{
			std::string	*l_str = (std::string*)m_pData;
			SAFE_DELETE(l_str);		
		}
		else
		{
			char*l_pData = (char*)m_pData;
			SAFE_DELETE(l_pData);
		}
		m_pData = 0;
	}

	void	cEventVariable::SetData(void*e_pData,eDataType e_eDataType)
	{
		void*	l_pData = 0;
		Destroy();
		m_eDataType = e_eDataType;
		if( m_eDataType == eDT_BYTE )
		{
			l_pData = new char;
			*(char*)l_pData = *(char*)e_pData;
		}
		else
		if( m_eDataType == eDT_INT )
		{
			l_pData = new int;
			*(int*)l_pData = *(int*)e_pData;
		}
		else
		if( m_eDataType == eDT_FLOAT )
		{
			l_pData = new float;
			*(float*)l_pData = *(float*)e_pData;
		}
		else
		if( m_eDataType == eDT_STRING )
		{
			l_pData = new std::string;
			*(std::string*)l_pData = *(std::string*)e_pData;
		}
		else
		if( m_eDataType == eDT_WSTRING )
		{
			l_pData = new std::wstring;
			*(std::wstring*)l_pData = *(std::wstring*)e_pData;
		}
		else
		if( m_eDataType == eDT_DOUBLE )
		{
			l_pData = new double;
			*(double*)l_pData = *(double*)e_pData;
		}
		else
		if( m_eDataType == eDT_VOID )
		{
			l_pData = e_pData;
		}
		else
		if( m_eDataType == eDT_POINT )
		{
			l_pData = new POINT;
			*(POINT*)l_pData = *(POINT*)e_pData;
		}
		else
		if( m_eDataType == eDT_VECTOR2 )
		{
			l_pData = new Vector2;
			*(Vector2*)l_pData = *(Vector2*)e_pData;
		}
		else
		if( m_eDataType == eDT_VECTOR3 )
		{
			l_pData = new Vector3;
			*(Vector3*)l_pData = *(Vector3*)e_pData;
		}
		else
		if( m_eDataType == eDT_VECTOR4 )
		{
			l_pData = new Vector4;
			*(Vector4*)l_pData = *(Vector4*)e_pData;
		}
		else
		if( m_eDataType == eDT_INT64 )
		{
			l_pData = new int64;
			*(int64*)l_pData = *(int64*)e_pData;
		}
		else
		if( m_eDataType == eDT_UINT64 )
		{
			l_pData = new uint64;
			*(uint64*)l_pData = *(uint64*)e_pData;
		}
		else
			m_eDataType = eDT_MAX;
		this->m_pData = l_pData;
	}

	void				cEventVariable::SetDataByString(std::wstring e_strValue)
	{
		SetDataByString(e_strValue.c_str());
	}

	void				cEventVariable::SetDataByString(const WCHAR*e_strValue)
	{
		switch( m_eDataType )
		{
			case eDT_BYTE:
				*(char*)m_pData = (char)_wtoi(e_strValue);
				break;
			case eDT_INT:
				*(int*)m_pData = _wtoi(e_strValue);
				break;
			case eDT_FLOAT:
				*(float*)m_pData = (float)_wtof(e_strValue);
				break;
			case eDT_STRING:
				*(std::string*)m_pData = UT::WcharToChar(e_strValue);
				break;
			case eDT_WSTRING:
					*(std::wstring*)m_pData = e_strValue;
				break;
			case eDT_DOUBLE:
				*(double*)m_pData = (double)_wtof(e_strValue);
				break;
			case eDT_VECTOR2:
				*(Vector4*)m_pData = FATMING_CORE::GetVector2(e_strValue);
				break;
			case eDT_VECTOR3:
				*(Vector4*)m_pData = FATMING_CORE::GetVector3(e_strValue);
				break;
			case eDT_VECTOR4:
				*(Vector4*)m_pData = FATMING_CORE::GetVector4(e_strValue);
				break;
			case eDT_POINT:
				*(POINT*)m_pData = FATMING_CORE::GetPoint(e_strValue);
				break;
			case eDT_UINT64:
				*(uint64*)m_pData = FATMING_CORE::GetUint64(e_strValue);
				break;
			case eDT_INT64:
				*(int64*)m_pData = FATMING_CORE::GetInt64(e_strValue);
				break;
			default:
				assert(0&&"SetData(WCHAR*e_strValue,eDataType e_eDataType) data type error");
				break;
		}
	}
	void				cEventVariable::SetDataByString(std::wstring e_strValue,eDataType e_eDataType)
	{
		SetDataByString(e_strValue.c_str(),e_eDataType);
	}

	void				cEventVariable::SetDataByString(const WCHAR*e_strValue,eDataType e_eDataType)
	{
		void*	l_pData = 0;
		Destroy();
		m_eDataType = e_eDataType;
		switch( m_eDataType )
		{
			case eDT_BYTE:
				l_pData = new char;
				*(char*)l_pData = (char)_wtoi(e_strValue);
				break;
			case eDT_INT:
				l_pData = new int;
				*(int*)l_pData = _wtoi(e_strValue);
				break;
			case eDT_FLOAT:
				l_pData = new float;
				*(float*)l_pData = (float)_wtof(e_strValue);
				break;
			case eDT_STRING:
				l_pData = new std::string;
				*(std::string*)l_pData = UT::WcharToChar(e_strValue);
				break;
			case eDT_WSTRING:
					l_pData = new std::wstring(e_strValue);
					*(std::wstring*)l_pData = e_strValue;
				break;
			case eDT_DOUBLE:
				l_pData = new double;
				*(double*)l_pData = (double)_wtof(e_strValue);
				break;
			case eDT_VECTOR2:
				l_pData = new Vector2;
				*(Vector4*)l_pData = FATMING_CORE::GetVector2(e_strValue);
				break;
			case eDT_VECTOR3:
				l_pData = new Vector3;
				*(Vector4*)l_pData = FATMING_CORE::GetVector3(e_strValue);
				break;
			case eDT_VECTOR4:
				l_pData = new Vector4;
				*(Vector4*)l_pData = FATMING_CORE::GetVector4(e_strValue);
				break;
			case eDT_POINT:
				l_pData = new POINT;
				*(POINT*)l_pData = FATMING_CORE::GetPoint(e_strValue);
				break;
			case eDT_INT64:
				l_pData = new int64;
				*(int64*)l_pData = FATMING_CORE::GetInt64(e_strValue);
				break;
			case eDT_UINT64:
				l_pData = new uint64;
				*(uint64*)l_pData = FATMING_CORE::GetUint64(e_strValue);
				break;
			default:
				assert(0&&"SetData(WCHAR*e_strValue,eDataType e_eDataType) data type error");
				break;
		}
		this->m_pData = l_pData;
	}

	void				cEventVariable::SetData(void*e_pData)
	{
		Destroy();
		m_pData = e_pData;
	}

	void				cEventVariable::SetString(std::string e_string)
	{
		SetData((void*)&e_string,eDT_STRING);
	}

	void				cEventVariable::SetWString(std::wstring e_string)
	{
		SetData((void*)&e_string,eDT_WSTRING);
	}

	void				cEventVariable::SetInt(int e_iValue)
	{
		SetData((void*)&e_iValue,eDT_INT);
	}

	void				cEventVariable::SetFloat(float e_fValue)
	{
		SetData((void*)&e_fValue,eDT_FLOAT);			
	}

	void				cEventVariable::SetDouble(double e_fdbValue)
	{
		SetData((void*)&e_fdbValue,eDT_DOUBLE);	
	}

	void				cEventVariable::SetChar(char e_cValue)
	{
		SetData((void*)&e_cValue,eDT_BYTE);
	}

	void				cEventVariable::SetPoint(POINT e_Point)
	{
		SetData((void*)&e_Point,eDT_POINT);
	}

	void				cEventVariable::SetVector2(Vector2 e_vValue)
	{
		SetData((void*)&e_vValue,eDT_VECTOR2);
	}

	void				cEventVariable::SetVector3(Vector3 e_vValue)
	{
		SetData((void*)&e_vValue,eDT_VECTOR3);
	}

	void				cEventVariable::SetVector4(Vector4 e_vValue)	
	{
		SetData((void*)&e_vValue,eDT_VECTOR4);
	}

	void				cEventVariable::SetInt64(int64 e_uiValue)
	{
			SetData((void*)&e_uiValue,eDT_INT64);
	}

	void				cEventVariable::SetUint64(uint64 e_uiValue)
	{
		SetData((void*)&e_uiValue,eDT_UINT64);
	}

	bool				cEventVariable::IsSame(std::wstring e_strValue)
	{
		return IsSame(e_strValue.c_str());
	}

	bool				cEventVariable::IsSame(const WCHAR*e_strValue)
	{
		if( eDT_VOID == m_eDataType )
			return false;
		std::wstring	l_str = ConvertToString();
		if(!wcscmp(l_str.c_str(),e_strValue))
			return true;
		return false;
	}

	bool				cEventVariable::IsSame(double e_dbValue)
	{
		return *this->GetDouble() == e_dbValue;
	}

	bool				cEventVariable::IsSame(char e_cValue)
	{
		return IsSame(ValueToStringW(e_cValue));
	}

	bool				cEventVariable::IsSame(float e_fValue)
	{
		return *this->GetFloat() == e_fValue;
	}

	bool				cEventVariable::IsSame(int e_iValue)
	{
		return IsSame(ValueToStringW(e_iValue));
	}

	bool				cEventVariable::IsSame(POINT e_Point)
	{
		POINT	l_Point = *this->GetPoint();
		if( l_Point.x == e_Point.x && l_Point.y == e_Point.y )
			return true;
		return false;
	}

	bool				cEventVariable::IsSame(Vector2 e_vValue)
	{
		return *this->GetVector2() == e_vValue;
	}
	bool				cEventVariable::IsSame(Vector3 e_vValue)
	{
		return *this->GetVector3() == e_vValue;
	}
	bool				cEventVariable::IsSame(Vector4 e_vValue)
	{
		return *this->GetVector4() == e_vValue;
	}

	bool				cEventVariable::IsSame(int64 e_iValue)
	{
		return *this->GetInt64() == e_iValue;
	}

	bool				cEventVariable::IsSame(uint64 e_uiValue)
	{
		return *this->GetUint64() == e_uiValue;
	}

	bool				cEventVariable::IsSame(cEventVariable*e_pEventVariable)
	{
		if( e_pEventVariable )
			return IsSame(e_pEventVariable->ConvertToString().c_str());
		return 0;
	}

	std::string*		cEventVariable::GetString()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_STRING&&"data type is not correct");
		return (std::string*)m_pData;
	} 

	std::wstring*		cEventVariable::GetWString()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_WSTRING&&"data type is not correct");
		return (std::wstring*)m_pData;
	}

	int*				cEventVariable::GetInt()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_INT&&"data type is not correct");
		return (int*)m_pData;
	}

	float*				cEventVariable::GetFloat()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_FLOAT&&"data type is not correct");
		return (float*)m_pData;
	}

	double*				cEventVariable::GetDouble()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_DOUBLE&&"data type is not correct");
		return (double*)m_pData;
	}

	char*				cEventVariable::GetChar()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_BYTE&&"data type is not correct");
		return (char*)m_pData;
	}

	POINT*				cEventVariable::GetPoint()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_POINT&&"data type is not correct");
		return (POINT*)m_pData;
	}

	Vector2*			cEventVariable::GetVector2()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_VECTOR2&&"data type is not correct");
		return (Vector2*)m_pData;	
	}

	Vector3*			cEventVariable::GetVector3()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_VECTOR3&&"data type is not correct");
		return (Vector3*)m_pData;	
	}

	Vector4*			cEventVariable::GetVector4()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_VECTOR4&&"data type is not correct");
		return (Vector4*)m_pData;	
	}

	int64*				cEventVariable::GetInt64()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_INT64&&"data type is not correct");
		return (int64*)m_pData;	
	}

	uint64*				cEventVariable::GetUint64()
	{
		if(!m_pData)
			return 0;
		assert(m_eDataType == eDT_UINT64&&"data type is not correct");
		return (uint64*)m_pData;
	}

	void*				cEventVariable::GetData()
	{
		return m_pData;
	}

	int					cEventVariable::GetDataSize()
	{
		return g_iDataTypeSize[m_eDataType];
	}

	std::wstring		cEventVariable::ConvertToString()
	{
		std::wstring	l_str;
		if(!m_pData||eDT_VOID == m_eDataType)
			return l_str;
		switch( this->m_eDataType)
		{
			case eDT_BYTE:
				l_str = ValueToStringW(*(char*)this->m_pData);
				break;
			case eDT_FLOAT:
				l_str = ValueToStringW(*(float*)this->m_pData);
				break;
			case eDT_DOUBLE:
				l_str = ValueToStringW(*(double*)this->m_pData);
				break;
			case eDT_INT:
				l_str = ValueToStringW(*(int*)this->m_pData);
				break;
			case eDT_WSTRING:
				l_str = (WCHAR*)GetWString()->c_str();
				break;
			case eDT_STRING:
				l_str = ValueToStringW(this->GetString()->c_str());
				break;
			case eDT_VECTOR2:
				l_str = ValueToStringW(*(Vector2*)this->m_pData);
				break;
			case eDT_VECTOR3:
				l_str = ValueToStringW(*(Vector3*)this->m_pData);
				break;
			case eDT_VECTOR4:
				l_str = ValueToStringW(*(Vector4*)this->m_pData);
				break;
			case eDT_POINT:
				l_str = ValueToStringW(*(POINT*)this->m_pData);
				break;
			case eDT_INT64:
				l_str = ValueToStringW(*(int64*)this->m_pData);
				break;
			case eDT_UINT64:
				l_str = ValueToStringW(*(uint64*)this->m_pData);
				break;
			default:
				UT::ErrorMsg("ConvertToString not support this type","Error");
				break;
		}
		return l_str;
	}

	TiXmlElement*	cEventVariable::ToTiXmlElement()
	{
		cEventVariable*l_pNameAndData = this;
		std::wstring	l_str = l_pNameAndData->ConvertToString();
		TiXmlElement *l_pTiXmlElement = new TiXmlElement(l_pNameAndData->Type());
		l_pTiXmlElement->SetAttribute(L"Name",l_pNameAndData->GetName());
		l_pTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,ValueToStringW(l_pNameAndData->GetDataType()));
		l_pTiXmlElement->SetAttribute(L"Value",l_str.c_str());
		return l_pTiXmlElement;
	}

	cEventVariableList::cEventVariableList()
	{
		m_pData = 0;
	}

	cEventVariableList::~cEventVariableList()
	{
		char*l_pData = (char*)m_pData;
		SAFE_DELETE(l_pData);
		m_pData = 0;
	}

	bool	cEventVariableList::RemoveObject(int e_iIndex)
	{
		return cNamedTypedObjectVector<cEventVariable>::RemoveObject(e_iIndex);
	}
	bool	cEventVariableList::AddObjectNeglectExist(cEventVariable* e_pcEventVariable)
	{
		return cNamedTypedObjectVector<cEventVariable>::AddObjectNeglectExist(e_pcEventVariable);
	}

	void	cEventVariableList::SetData(void*e_pData,int e_iSize)
	{
		Destroy();
		char*l_pData = (char*)this->m_pData;
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{//all data come from cEventVariableList::m_pData
			this->GetObject(i)->m_pData = (void*)l_pData;
			l_pData += this->GetObject(i)->GetDataSize();
		}
	}

	void*	cEventVariableList::GetData()
	{
		return m_pData;
	}

	int		cEventVariableList::DataSize()
	{//fuck
		return 0;
	}

	void	cEventVariableList::Destroy()
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{//all data come from cEventVariableList::m_pData
			this->GetObject(i)->m_pData = 0;
		}
		char*l_pData = (char*)m_pData;
		SAFE_DELETE(l_pData);
		m_pData = 0;
	}


	TiXmlElement*	cEventVariableList::ToTiXmlElement()
	{
		TiXmlElement *l_pRoot = new TiXmlElement(cEventVariableList::TypeID);
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{//all data come from cEventVariableList::m_pData
			cEventVariable*l_pNameAndData = this->GetObject(i);
			TiXmlElement *l_pTiXmlElement = l_pNameAndData->ToTiXmlElement();
			l_pRoot->LinkEndChild(l_pTiXmlElement);
		}
		return l_pRoot;
	}

	cEventVariableManager::cEventVariableManager()
	{

	}

	cEventVariableManager::~cEventVariableManager()
	{
	
	}

	void	cEventVariableManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		if( !wcscmp(e_pTiXmlElement->Value(),cEventVariable::TypeID) )
		{	
			AddVariable(e_pTiXmlElement);
		}
	}

	bool	cEventVariableManager::AddVariable(TiXmlElement*e_pTiXmlElement)
	{
		cEventVariable*l_pNameAndData = new cEventVariable(e_pTiXmlElement);
		if(!this->AddObject(l_pNameAndData))
		{
			delete l_pNameAndData;
			return false;
		}
		return true;
	}

	bool			cEventVariableManager::AddVariable(WCHAR*e_strName,eDataType e_eDataType,const WCHAR*e_strData)
	{
		if(this->GetObject(e_strName))
			return false;
		cEventVariable*l_pNameAndData = new cEventVariable(e_strName,e_eDataType,e_strData);
		this->AddObjectNeglectExist(l_pNameAndData);
		return true;
	}

	void			cEventVariableManager::Export(const char*e_strFileName,bool e_bBinary)
	{
		TiXmlElement *l_pRoot;
		TiXmlDocument l_Doc;
		l_pRoot = new TiXmlElement(L"VariableData");
		l_Doc.LinkEndChild( l_pRoot );	

		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cEventVariable*l_pNameAndData = this->m_ObjectList[i];
			TiXmlElement *l_pTiXmlElement = l_pNameAndData->ToTiXmlElement();
			l_pRoot->LinkEndChild(l_pTiXmlElement);
		}
		l_Doc.SaveFile(e_strFileName,e_bBinary);
	}
//end namesapce FATMING_CORE
}