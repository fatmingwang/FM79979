#ifndef _EVENT_VARIABLE_H_
#define _EVENT_VARIABLE_H_

#include "EventHeaderDefine.h"

namespace FATMING_CORE
{
	class	cEventVariableList;
	//<VariableData>
	//	<cEventVariable Name="LinePlayable" Type="int" Value="17" />
	//	<cEventVariable Name="Language" Type="wstring" Value="EN" />
	//	<cEventVariable Name="CurrentPackageName" Type="wstring" Value="FirstPage" />
	//</VariableData>
	class	cEventVariable:public NamedTypedObject
	{
		friend class		cEventVariableList;
		void*				m_pData;
		eDataType			m_eDataType;
	public:
		DEFINE_TYPE_INFO();
		cEventVariable();
		cEventVariable(const WCHAR*e_strName,eDataType e_eDataType,const WCHAR*e_strValue);
		cEventVariable(TiXmlElement*e_pTiXmlElement);
		virtual ~cEventVariable();
		eDataType				GetDataType(){return  m_eDataType;}
		void					SetDataByString(const WCHAR*e_strValue,eDataType e_eDataType);
		void					SetDataByString(std::wstring e_strValue,eDataType e_eDataType);
		//if data is eDT_VOID,ensure momory control
		void					SetData(void*e_pData,eDataType e_eDataType);
		//it will delete old data and directly assign data to it,so ensure the data is not share or meomry control is fine.
		//if data is eDT_VOID,ensure momory control
		void					SetData(void*e_pData);
		//convert string to data type
		void					SetDataByString(const WCHAR*e_strValue);
		void					SetDataByString(std::wstring e_strValue);

		void					SetString(std::string e_string);
		void					SetWString(std::wstring e_string);
		void					SetInt(int e_iValue);
		void					SetFloat(float e_fValue);
		void					SetDouble(double e_fdbValue);
		void					SetChar(char e_cValue);
		void					SetPoint(POINT e_Point);
		void					SetVector2(Vector2 e_vValue);
		void					SetVector3(Vector3 e_vValue);
		void					SetVector4(Vector4 e_vValue);
		void					SetInt64(int64 e_uiValue);
		void					SetUint64(uint64 e_uiValue);
		//
		bool					IsSame(std::wstring e_strValue);
		bool					IsSame(const WCHAR*e_strValue);
		bool					IsSame(char e_cValue);
		bool					IsSame(double e_dbValue);
		bool					IsSame(float e_fValue);
		bool					IsSame(int e_iValue);
		bool					IsSame(POINT e_Point);
		bool					IsSame(Vector2 e_vValue);
		bool					IsSame(Vector3 e_vValue);
		bool					IsSame(Vector4 e_vValue);
		bool					IsSame(int64 e_iValue);
		bool					IsSame(uint64 e_uiValue);
		bool					IsSame(cEventVariable*e_pEventVariable);

		std::string*			GetString();
		std::wstring*			GetWString();
		int*					GetInt();
		float*					GetFloat();
		double*					GetDouble();
		char*					GetChar();
		POINT*					GetPoint();
		Vector2*				GetVector2();
		Vector3*				GetVector3();
		Vector4*				GetVector4();
		int64*					GetInt64();
		uint64*					GetUint64();
		void*					GetData();
		int						GetDataSize();
		std::wstring			ConvertToString();
		virtual	void			Destroy();
		virtual	TiXmlElement*	ToTiXmlElement();
	};

    //<cEventVariableList Name="Player1Data" Type="void" Value="30" />
	   // <cEventVariable Name="InMoney" Type="int" Value="6" />
	   // <cEventVariable Name="OutMoney" Type="int" Value="6" />
    //</cEventVariableList/>
	//all cNamedTypedObjectVector<cEventVariable>::m_pData come from cEventVariableList::m_pData,so we could call SetData.get data to access out goal.
	//data not allow wstring and string.
	//only support AddObject and RemoveObject
	class	cEventVariableList:public cNamedTypedObjectVector<cEventVariable>
	{
		void*				m_pData;
		eDataType			m_eDataType;
	public:
		cEventVariableList();
		virtual ~cEventVariableList();
		DEFINE_TYPE_INFO();
		//
		virtual	bool			RemoveObject(int e_iIndex);
		virtual	bool			AddObjectNeglectExist(cEventVariable* e_pcEventVariable);
		//after add or remove object call this
		void					DataReorganize();
		//
		void					SetData(void*e_pData,int e_iSize);
		void*					GetData();
		int						DataSize();
		virtual	void			Destroy();
		virtual	TiXmlElement*	ToTiXmlElement();
	};
    //<cEventVariableList Name="Player1Data" Type="void" Value="30" >
	   // <cEventVariable Name="InMoney" Type="int" Value="6" />
	   // <cEventVariable Name="OutMoney" Type="int" Value="6" />
    //</cEventVariableList>
	class	cEventVariableManager:public cNamedTypedObjectVector<cEventVariable>,public ISAXCallback
	{
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	public:
		cEventVariableManager();
		virtual ~cEventVariableManager();
		//if variable is exists won't add again.
		bool			AddVariable(TiXmlElement*e_pTiXmlElement);
		bool			AddVariable(WCHAR*e_strName,eDataType e_eDataType,const WCHAR*e_strData);
		void			Export(const char*e_strFileName,bool e_bBinary = false);
	};

	//typedef cEventVariable	cEventVariable;
	//typedef cEventVariableManager	cEventVariableManager;
//end namespace FATMING_CORE
}
#endif