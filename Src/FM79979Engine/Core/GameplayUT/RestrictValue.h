#ifndef _RESTRICT_VALUE_H_
#define _RESTRICT_VALUE_H_

#include "../XML/tinyxml.h"

namespace FATMING_CORE
{
	//make a value go between max and min.
	class	cRestrictValueBase
	{
	protected:
		GET_SET_DEC(bool,m_bValueLoop,IsValueLoop,SetValueLoop);
		eDataType			m_eDataType;
		wchar_t*				m_strType;
	public:
		cRestrictValueBase()
		{
			m_strType = 0;
			m_bValueLoop = true;
			m_eDataType = eDT_MAX;
		}
		virtual	cRestrictValueBase*	Clone() = 0;
		virtual	bool				IncreaseValue() = 0;
		virtual	bool				DecreaseValue() = 0;
		virtual	bool				SetValue(const wchar_t*e_strValue) = 0;
		virtual	bool				SetValue(std::wstring e_strValue) = 0;
		virtual	void				SetTargetData(void*e_pValue) = 0;
		virtual	void				GetRandomValue(void*e_pValue) = 0;
		static cRestrictValueBase*	GetRestrictValueBaseByElement(TiXmlElement*e_pElement);
		eDataType					GetDataType(){return m_eDataType;}
		TiXmlElement*				ToTiXmlElement();
		virtual	void				SetAttribute(TiXmlElement*e_pTiXmlElement) = 0;
	};
	//<cRestrictValue DataType="float" Max="100" Min="0" IncreaseValue="1" Value="1" />
	template<class T> class	cRestrictValue:public cRestrictValueBase
	{
		T	m_Value;
		GET_SET_DEC(T,m_IncreaseValue,GetIncreaseValue,SetIncreaseValue);
		GET_SET_DEC(T,m_MaximumValue,GetMaximumValue,SetMaximumValue);
		GET_SET_DEC(T,m_MinimumValue,GetMinimumValue,SetMinimumValue);
		//set this pointer so it will automaticly to set new value
		T*				m_pTargetData;
		inline	void	SetTargetValue(T e_Value)
		{
			if( m_pTargetData )*m_pTargetData = e_Value;// m_Value = e_Value;
		}
        inline	bool	ValueChangeVaild(T e_Old,T e_New,bool e_bForceToLimit)
        {
            if (e_Old != e_New)
            {
                if (e_New <= m_MaximumValue && e_New >= m_MinimumValue)
                {
                    m_Value = e_New;
					SetTargetValue(m_Value);
					return true;
                }
				if( m_bValueLoop )
				{
					if( e_New > m_MaximumValue )
						m_Value = m_MinimumValue;
					if( e_New < m_MinimumValue )
						m_Value = m_MaximumValue;
					SetTargetValue(m_Value);
					return true;
				}
				if( e_bForceToLimit )
				{
					if( e_New > m_MaximumValue )
						m_Value = m_MaximumValue;
					if( e_New < m_MinimumValue )
						m_Value = m_MinimumValue;
					SetTargetValue(m_Value);
					return true;
				}
				return false;
            }
			return true;
        }
	public:
		cRestrictValue()
		{
			m_strType = L"cRestrictValue";
			m_pTargetData = 0;
			m_Value = (T)0;
			m_IncreaseValue = (T)1;
			m_MaximumValue =  (T)100;
			m_MinimumValue =  (T)0;
		}
		cRestrictValue(T e_Value,T e_IncreaseValue,T e_MaximumValue,T e_MinimumValue)
		{
			m_strType = L"cRestrictValue";
			m_pTargetData = 0;
			m_Value = e_Value;
			m_IncreaseValue = e_IncreaseValue;
			m_MaximumValue = e_MaximumValue;
			m_MinimumValue = e_MinimumValue;
		}
		cRestrictValue(cRestrictValue<T> *e_pRestrictValue)
		{
			m_strType = e_pRestrictValue->m_strType;
			m_pTargetData = e_pRestrictValue->m_pTargetData;
			m_Value = e_pRestrictValue->m_Value;
			m_IncreaseValue = e_pRestrictValue->m_IncreaseValue;
			m_MaximumValue = e_pRestrictValue->m_MaximumValue;
			m_MinimumValue = e_pRestrictValue->m_MinimumValue;
		}
		cRestrictValueBase*	Clone(){ return new cRestrictValue<T>(this); }
		cRestrictValue(TiXmlElement*e_pElement)
		{
			m_Value = 0;
			m_strType = L"cRestrictValue";
			m_pTargetData = 0;
			ELEMENT_VALUE_ASSERT_CHECK(e_pElement,L"cRestrictValue");
			PARSE_ELEMENT_START(e_pElement)
				COMPARE_NAME_WITH_DEFINE(DATA_TYPE)
				{
					this->m_eDataType = UT::GetDataType(l_strValue);
				}
				else
				COMPARE_NAME("IncreaseValue")
				{
					if( m_eDataType == eDT_INT )
					{
						this->m_IncreaseValue = (T)VALUE_TO_INT;
					}
					else
					{
						this->m_IncreaseValue = (T)VALUE_TO_FLOAT;
					}
				}
				else
				COMPARE_NAME("Value")
				{
					if( m_eDataType == eDT_INT )
					{
						this->m_Value = (T)VALUE_TO_INT;
					}
					else
					{
						this->m_Value = (T)VALUE_TO_FLOAT;
					}
				}
				else
				COMPARE_NAME("Max")
				{
					if( m_eDataType == eDT_INT )
					{
						this->m_MaximumValue = (T)VALUE_TO_INT;
					}
					else
					{
						this->m_MaximumValue = (T)VALUE_TO_FLOAT;
					}
				}
				else
				COMPARE_NAME("Min")
				{
					if( m_eDataType == eDT_INT )
					{
						this->m_MinimumValue = (T)VALUE_TO_INT;
					}
					else
					{
						this->m_MinimumValue = (T)VALUE_TO_FLOAT;
					}
				}
				else
				COMPARE_NAME("Loop")
				{
					m_bValueLoop = VALUE_TO_BOOLEAN;
				}
				
			PARSE_NAME_VALUE_END
		}
		virtual ~cRestrictValue()
		{
			SAFE_DELETE(m_pTargetData);
		}
		void	SetTargetData(T*e_pValue)
		{
			m_pTargetData = e_pValue;
			if( m_pTargetData )
			{
				m_Value = *m_pTargetData;
			}
		}
		virtual	void	SetTargetData(void*e_pTargetValue)
		{
			SetTargetData((T*)e_pTargetValue);
		}
		virtual	bool	IncreaseValue()
		{
			if( m_pTargetData )
			{
				return ValueChangeVaild(*m_pTargetData,*m_pTargetData+m_IncreaseValue,true);
			}
			return ValueChangeVaild(m_Value,m_Value+m_IncreaseValue,true); 
		}
		virtual	bool	DecreaseValue()
		{
			if( m_pTargetData )
			{
				return ValueChangeVaild(*m_pTargetData,*m_pTargetData-m_IncreaseValue,true);
			}
			return ValueChangeVaild(m_Value,m_Value-m_IncreaseValue,true); 
		}

		virtual	bool	SetValue(T  e_Value,bool e_bForceToLimit = false){ return  ValueChangeVaild(m_Value,e_Value,e_bForceToLimit); }
		virtual	bool	SetValue(std::wstring e_strValue){ return SetValue(e_strValue.c_str()); }
		virtual	bool	SetValue(const wchar_t*e_strValue)
		{
			switch( m_eDataType )
			{
				case eDT_BYTE:
				case eDT_INT:
					return SetValue((T)_wtoi(e_strValue));
					break;
				case eDT_FLOAT:
				case eDT_DOUBLE:
					return SetValue((T)_wtof(e_strValue));
					break;
				default:
					assert(0 &&"lazy to implement cRestrictValue::SetValue");
					break;
			}
			return false;
		}
		T*				GetTargetData()
		{
			return m_pTargetData;
		}
		virtual	void		GetRandomValue(void*e_pValue)
		{
			T*l_pValue = (T*)e_pValue;
			*l_pValue = (T)frand((float)m_MinimumValue,(float)m_MaximumValue);		
		}
		T	GetValue(){return m_Value;}
		virtual	void	SetAttribute(TiXmlElement*e_pTiXmlElement)
		{
			e_pTiXmlElement->SetAttribute(L"IncreaseValue",ValueToStringW(m_IncreaseValue));
			e_pTiXmlElement->SetAttribute(L"Value",ValueToStringW(m_Value));
			e_pTiXmlElement->SetAttribute(L"Max",ValueToStringW(m_MaximumValue));
			e_pTiXmlElement->SetAttribute(L"Min",ValueToStringW(m_MinimumValue));		
		}
	};
//end namespace FATMING_CORE
}
#endif