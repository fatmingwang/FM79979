#pragma once
#include "../../Common/Utility.h"
#include "../../XML/XMLLoader.h"
namespace FATMING_CORE
{
	//<cListValueChanger DataType="int" Data="0,1,2,3,4,5" />
	//<cListValueChanger DataType="wstring" Data="test string,second string" />
	//<cListValueChanger DataType="string" Data="test string,second string" />
	//<cListValueChanger DataType="wstring" Data="·F,§A¦Ñ®v,2,3,4,5" Loop="0" StartIndex="0" />

	class	cListValuChangerBase
	{
		virtual	void		Clear(){}
	protected:
		int					m_iSelectedIndex;
		GET_SET_DEC(bool,m_bLoop,IsLoop,SetLoop);
		eDataType			m_eDataType;
		wchar_t*			m_strType;
	public:
		cListValuChangerBase()
		{
			m_strType = 0;
			m_iSelectedIndex = -1;
			m_bLoop = true;
			m_eDataType = eDT_MAX;
		}
		cListValuChangerBase(cListValuChangerBase*e_pListValuChangerBase)
		{
			m_iSelectedIndex = e_pListValuChangerBase->m_iSelectedIndex;
			m_bLoop = e_pListValuChangerBase->m_bLoop;
			m_eDataType = e_pListValuChangerBase->m_eDataType;
		}
		virtual~cListValuChangerBase()
		{
			Clear();
		}
		virtual	cListValuChangerBase*	Clone(){ return 0; }
		int								GetSelectedIndex(){ return m_iSelectedIndex; }
		virtual	void					Next() = 0;
		virtual	void					Prior() = 0;
		virtual	void					SetSelectedIndex(int e_iSelectedIndex) = 0;
		virtual	void					SetSelectedIndexByCompareDataString(const wchar_t*e_str) = 0;
		virtual	int						GetRandomValue() = 0;
		static cListValuChangerBase*	GetListValuChangerBaseByElement(TiXmlElement*e_pElement);
		virtual	std::wstring			ConvertSelectedDataToString() = 0;
		TiXmlElement*					ToTiXmlElement();
		virtual char*					GetSelectedDataPonter() = 0;
		virtual	void					SetAttribute(TiXmlElement*e_pTiXmlElement) = 0;
		eDataType						GetDataType() { return m_eDataType; }
	};

	template<class T>class	cListValueChanger:public cListValuChangerBase
	{
		void	AssignDataToDataList(eDataType	l_eDataType, const wchar_t*e_strValue, void*e_pData)
		{
			if (l_eDataType == eDT_INT)
			{
				std::vector<int>l_DataList = GetIntegerListByCommaDivide(e_strValue, 1);
				*(std::vector<int>*)e_pData = l_DataList;
			}
			else
			if (l_eDataType == eDT_INT64)
			{
				std::vector<int64>l_DataList = GetInt64ListByCommaDivide(e_strValue, 1);
				*(std::vector<int64>*)e_pData = l_DataList;
			}
			else
			if (l_eDataType == eDT_FLOAT)
			{
				std::vector<float>l_DataList = GetFloatListByCommaDivide(e_strValue, 1);
				*(std::vector<float>*)e_pData = l_DataList;
			}
			else
			if (l_eDataType == eDT_STRING)
			{
				std::vector<std::string>l_DataList = GetStringListByCommaDivide(e_strValue);
				*(std::vector<string>*)e_pData = l_DataList;
			}
			else
			if (l_eDataType == eDT_WSTRING)
			{
				std::vector<std::wstring>l_DataList = GetWStringListByCommaDivide(e_strValue);
				*(std::vector<wstring>*)e_pData = l_DataList;
			}
		}
		virtual	void		Clear()override { SAFE_DELETE(m_pDataList); }
		std::vector<T>		*m_pDataList;
	public:
		cListValueChanger(std::vector<T>e_DataList,bool e_bLoop = false)
		{
			m_strType = L"cListValueChanger";
			SetDataList(e_DataList); this->m_bLoopfalse = e_bLoop; 
		}
		cListValueChanger(TiXmlElement*e_pElement)
		{
			m_strType = L"cListValueChanger";
			m_pDataList = new std::vector<T>;
			ELEMENT_VALUE_ASSERT_CHECK(e_pElement,L"cListValueChanger");
			m_eDataType = eDT_INT;
			int	l_iSelectedIndex = 0;
			PARSE_ELEMENT_START(e_pElement)
				COMPARE_NAME_WITH_DEFINE(DATA_TYPE)
				{
					m_eDataType = UT::GetDataType(l_strValue);
				}
				else
				COMPARE_NAME("Data")
				{
					AssignDataToDataList(m_eDataType,l_strValue,m_pDataList);
				}
				else
				COMPARE_NAME("Loop")
				{
					m_bLoop = VALUE_TO_BOOLEAN;
				}
				else
				COMPARE_NAME("StartIndex")
				{
					l_iSelectedIndex = VALUE_TO_INT;
				}
			PARSE_NAME_VALUE_END
			m_iSelectedIndex = l_iSelectedIndex ;
		}
		cListValueChanger(cListValueChanger*e_pListValueChanger):cListValuChangerBase(e_pListValueChanger)
		{
			m_pDataList = new std::vector<T>;
			*m_pDataList = *e_pListValueChanger->m_pDataList;
		}
		virtual~cListValueChanger()
		{
			this->Clear();
		}
		virtual	cListValuChangerBase*	Clone()override
		{
			cListValuChangerBase*l_pListValuChangerBase = new cListValueChanger(this);
			return l_pListValuChangerBase;
		}
		std::vector<T>*		GetDataList(){return m_pDataList;}
		virtual	void		Next()override
		{
			int	l_iNewIndex = m_iSelectedIndex+1;
			int	l_iSzie = (int)m_pDataList->size();
			if( l_iNewIndex >= l_iSzie  )
			{
				if( m_bLoop )
					l_iNewIndex = 0;
				else
					l_iNewIndex = l_iSzie-1;
			}
			m_iSelectedIndex = l_iNewIndex;
		}
		virtual	void		Prior()override
		{
			int	l_iNewIndex = m_iSelectedIndex-1;
			if( l_iNewIndex <= -1  )
			{
				if( m_bLoop )
				{
					l_iNewIndex = (int)m_pDataList->size()-1;
				}
				else
					l_iNewIndex = 0;
			}
			m_iSelectedIndex = l_iNewIndex;		
		}
		virtual	void	SetSelectedIndex(int e_iSelectedIndex)override
		{
			if(e_iSelectedIndex<(int)m_pDataList->size() && e_iSelectedIndex > -1)
				m_iSelectedIndex = e_iSelectedIndex;
			else
				m_iSelectedIndex = -1;
		}
		virtual	void		SetSelectedIndexByCompareDataString(const wchar_t*e_str)override
		{
			SetSelectedIndex(FindIndexByValue(e_str));
		}
		virtual	int			GetRandomValue()override
		{
			if( m_pDataList->size() == 0 )
				return -1;
			return rand()%m_pDataList->size();
		}

		virtual char*					GetSelectedDataPonter()override
		{
			if (this->m_iSelectedIndex == -1)
				return nullptr;
			if (m_pDataList)
			{
				return (char*)&(*m_pDataList)[m_iSelectedIndex];
			}
			return nullptr;
		}

		void				SetDataList(std::vector<T> e_pDataList)
		{
			*m_pDataList = e_pDataList;
			if( m_pDataList->size() == 0 )
				m_iSelectedIndex = -1;
			else
				m_iSelectedIndex = 0;
		}

		T*		GetSelectedValue()
		{
			if( m_iSelectedIndex != -1 )
				return &(*m_pDataList)[m_iSelectedIndex];
			return nullptr;
		}
		virtual	std::wstring				ConvertSelectedDataToString()override
		{
			if( m_iSelectedIndex != -1 )
			{
				return ValueToStringW((*m_pDataList)[m_iSelectedIndex]).c_str();
			}
			std::wstring l_strTemp;
			return l_strTemp;
		}
		int			FindIndexByValue(const wchar_t*e_strValue)
		{
			size_t luiSize = m_pDataList->size();
			void*l_pData = 0;
			switch (m_eDataType)
			{
			case eDT_INT:
			{
				int	l_iValue = GetInt(e_strValue);
				for (size_t i = 0; i<luiSize; ++i)
				{
					l_pData = (void*)&(*m_pDataList)[i];
					if (*(int*)l_pData == l_iValue)
						return (int)i;
				}
			}
			break;
			case eDT_INT64:
			{
				int64	l_i64Value = GetInt64(e_strValue);
				for (size_t i = 0; i<luiSize; ++i)
				{
					l_pData = (void*)&(*m_pDataList)[i];
					if (*(int64*)l_pData == l_i64Value)
						return (int)i;
				}
			}
			break;
			case eDT_STRING:
			{
				std::string	l_strTemp = UT::WcharToChar(e_strValue);
				for (size_t i = 0; i<luiSize; ++i)
				{
					std::string*l_str = (std::string*)&(*m_pDataList)[i];
					//wchar_t*l_str = (wchar_t*)l_pData;
					if (!strcmp(l_str->c_str(), l_strTemp.c_str()))
						return (int)i;
				}
			}
			break;
			case eDT_WSTRING:
				for (size_t i = 0; i<luiSize; ++i)
				{
					std::wstring*l_str = (std::wstring*)&(*m_pDataList)[i];
					//wchar_t*l_str = (wchar_t*)l_pData;
					if (!wcscmp(l_str->c_str(), e_strValue))
						return (int)i;
				}
				break;
				//case eDT_BYTE:
				//case eDT_DOUBLE:
				//case eDT_FLOAT:
			default:
				UT::ErrorMsg(L"currently I am lazy so only support int", L"Warning");
				break;
			}
			return -1;
		}
//<cListValueChanger  DataType="wstring" Data=",AuditMenu_Revenue_4,AuditMenu_JPRecord_5" Loop="1" />
		virtual	void	SetAttribute(TiXmlElement*e_pTiXmlElement)override
		{
			if( m_pDataList )
			{
				size_t	l_uiSize = m_pDataList->size();
				std::wstring	l_str;
				for( size_t i=0;i<l_uiSize;++i )
				{
					l_str += ValueToStringW((T)(*m_pDataList)[i]);
					if( i != l_uiSize-1 )
					{
						l_str += L",";
					}
				}
				e_pTiXmlElement->SetAttribute(L"Data",l_str.c_str());
			}
		}
	};

	typedef	cListValueChanger<int>			cIntListValueChanger;
	typedef	cListValueChanger<float>		cFloatListValueChanger;
	typedef	cListValueChanger<std::string>	cStringListValueChanger;
	typedef	cListValueChanger<std::wstring>	cWStringListValueChanger;

//end FATMING_CORE
}