#include "../stdafx.h"
#include "EventValueChangerActiver.h"
#include "EventVariable.h"
#include "EventManager.h"
namespace FATMING_CORE
{
	const  wchar_t*        cEventListValueSelectionActiver::TypeID = L"cEventListValueSelectionActiver";
	const  wchar_t*        cEventValueChangeActiver::TypeID = L"cEventValueChangeActiver";

	eValueeChangeType	GetValueeChangeType(const WCHAR*e_strValue)
	{
		if( !wcscmp(e_strValue,L"Increase") )
			return eCVT_INCREASE;
		else
		if( !wcscmp(e_strValue,L"Decrease") )
			return eCVT_DECREASE;
		else
		if( !wcscmp(e_strValue,EVENT_TARGET_VALUE) )
			return eCVT_TARGET_VALUE;
		else
		if( !wcscmp(e_strValue,L"RandomValue") )
			return eCVT_RANDOM_VALUE;
		//else
		//if( !wcscmp(e_strValue,L"AddString") )
		//	return eCVT_STRING_ADD
		return eCVT_MAX;
	}

	WCHAR*	ValueToStringW(eValueeChangeType e_eValueeChangeType)
	{
		if( e_eValueeChangeType == eCVT_INCREASE )
			return L"Increase";
		if( e_eValueeChangeType == eCVT_DECREASE )
			return L"Decrease";
		if( e_eValueeChangeType == eCVT_TARGET_VALUE )
			return EVENT_TARGET_VALUE;
		if( e_eValueeChangeType == eCVT_RANDOM_VALUE )
			return L"RandomValue";
		return L"unknow";
	}
	cEventValueChangeActiver::cEventValueChangeActiver(TiXmlElement*e_pTiXmlElement)
	{
		m_eValueeChangeType = eCVT_MAX;
		m_pTargetEventVariable = 0;
		m_pRestrictValue = 0;
		m_pEventVariable = 0;
		m_pstrTargetString = 0;
		if( !e_pTiXmlElement )
			return;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventValueChangeActiver::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(VALUE_CHANGE_TYPE)
			{
				m_eValueeChangeType = GetValueeChangeType(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_TARGET_VALUE)
			{
				m_pstrTargetString = new std::wstring;
				*m_pstrTargetString = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
			{
				m_pEventVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
				if( !m_pEventVariable )
				{
					UT::ErrorMsg(L"event varialbe not find",l_strValue);
				}
			}
			else
			COMPARE_NAME("TargetEventVariable")
			{
				m_pTargetEventVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
			}
		PARSE_NAME_VALUE_END
		if( e_pTiXmlElement->FirstChildElement() )
		{
			m_pRestrictValue = cRestrictValueBase::GetRestrictValueBaseByElement(e_pTiXmlElement->FirstChildElement());
			if( m_pEventVariable )
			{
				if( m_pRestrictValue->m_eDataType != m_pEventVariable->GetDataType() )
				{
					UT::ErrorMsg(L"data type not same,please use cEventValueChangeActiver instead",L"Error!!");
				}
				m_pRestrictValue->SetTargetData(m_pEventVariable->GetData());
			}
		}
	}

	//cEventValueChangeActiver::cEventValueChangeActiver(WCHAR*e_strName,eValueeChangeType e_eValueeChangeType,cRestrictValueBase*e_pRestrictValue,WCHAR*e_strTargetValue)
	//{
	//	m_pstrTargetString = 0;
	//	this->SetName(e_strName);
	//	m_pEventVariable = m_pEventVariable;
	//	m_eValueeChangeType = e_eValueeChangeType;
	//	m_pRestrictValue = e_pRestrictValue->Clone();
	//	if( e_strTargetValue )
	//	{
	//		m_pstrTargetString = new std::wstring;
	//		*m_pstrTargetString = e_strTargetValue;
	//	}
	//}

	cEventValueChangeActiver::cEventValueChangeActiver(cEventValueChangeActiver*e_pRestrictValueActiver)
	{
		m_pstrTargetString = 0;
		m_pRestrictValue = 0;
		this->SetName(e_pRestrictValueActiver->GetName());
		m_pEventVariable = e_pRestrictValueActiver->m_pEventVariable;
		m_pTargetEventVariable = e_pRestrictValueActiver->m_pTargetEventVariable;
		m_eValueeChangeType = e_pRestrictValueActiver->m_eValueeChangeType;
		if( e_pRestrictValueActiver->m_pRestrictValue )
			m_pRestrictValue = e_pRestrictValueActiver->m_pRestrictValue->Clone();;
		if( e_pRestrictValueActiver->m_pstrTargetString )
		{
			m_pstrTargetString = new std::wstring;
			*m_pstrTargetString = *e_pRestrictValueActiver->m_pstrTargetString;
		}
	}

	cEventValueChangeActiver::~cEventValueChangeActiver()
	{
		SAFE_DELETE(m_pRestrictValue);
		SAFE_DELETE(m_pstrTargetString);
	}

	void	cEventValueChangeActiver::InternalInit()
	{
		if( m_pEventVariable && !m_pRestrictValue && !m_pTargetEventVariable )
		{
			if( !this->m_pstrTargetString )
			{
				UT::ErrorMsg(L"cEventValueChangeActiver data error",L"no target value to change variable data");
			}
		}
		if( !m_pRestrictValue && !this->m_pEventVariable )
		{
			UT::ErrorMsg(L"cEventValueChangeActiver data error",L"there is no any data to setup!?");
		}
		if( m_pTargetEventVariable && !m_pEventVariable )
		{
			UT::ErrorMsg(L"cEventValueChangeActiver data error",L"there is no EventVariable but contain a TargetEventVariable!?");
		}
	}
	void	cEventValueChangeActiver::InternalUpdate(float e_fElpasedTime)
	{
		this->m_bSatisfiedCondition = true;
		if( m_pRestrictValue )
		{
			switch(m_eValueeChangeType)
			{
				case eCVT_INCREASE:
						m_pRestrictValue->IncreaseValue();
					break;
				case eCVT_DECREASE:
						m_pRestrictValue->DecreaseValue();
					break;
				case eCVT_TARGET_VALUE:
						m_pRestrictValue->SetValue(m_pstrTargetString->c_str());
					break;
				case eCVT_RANDOM_VALUE:
					{
						int	l_iValue = 0;
						m_pRestrictValue->GetRandomValue((void*)&l_iValue);
						m_pRestrictValue->SetValue(ValueToStringW(l_iValue));
					}
					break;
			}
		}
		else
		if( m_pEventVariable )
		{
			if( this->m_pstrTargetString )
				m_pEventVariable->SetDataByString(this->m_pstrTargetString->c_str());
			else
			if( m_pTargetEventVariable )
			{
				std::wstring	l_str = m_pTargetEventVariable->ConvertToString();
				m_pEventVariable->SetDataByString(l_str.c_str());
			}
		}
	}
	void	cEventValueChangeActiver::InternalRender(){}
	//<cEventValueChangeActiver Name="Test" ValueChangeType="TargetValue" cEventVariable="PasswordLength" TargetValue="0" />
	//<cEventValueChangeActiver Name="Test2" ValueChangeType="TargetValue" cEventVariable="VendorMenuSelectedIndex" TargetValue="VendorMenu_RevenueRecord_1" />
	//<cEventValueChangeActiver Name="Test" cEventVariable="VendorPassword" TargetEventVariable="InputPassword" />
	//<cEventValueChangeActiver Name="Test2" ValueChangeType="Increase" cEventVariable="CurrentShowIndex" >
	//	<cRestrictValue DataType="int" Max="1000" Min="-1" IncreaseValue="1" />
	//</cEventValueChangeActiver>
	TiXmlElement*		cEventValueChangeActiver::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		if( m_eValueeChangeType != eCVT_MAX)
		{
			l_pEvent->SetAttribute(VALUE_CHANGE_TYPE,ValueToStringW(m_eValueeChangeType));
		}
		l_pEvent->SetAttribute(cEventVariable::TypeID,this->m_pEventVariable->GetName());
		if( m_pstrTargetString )
		{
			l_pEvent->SetAttribute(EVENT_TARGET_VALUE,m_pstrTargetString->c_str());
		}
		if(m_pTargetEventVariable)
		{
			l_pEvent->SetAttribute(L"TargetEventVariable",m_pTargetEventVariable->GetName());
		}
		if( m_pRestrictValue )
		{
			TiXmlElement*l_pTiXmlElement = m_pRestrictValue->ToTiXmlElement();
			l_pEvent->LinkEndChild(l_pTiXmlElement);
		}
		return l_pEvent;
	}

	cEventListValueSelectionActiver::cEventListValueSelectionActiver(TiXmlElement*e_pTiXmlElement)
	{
		m_eValueeChangeType = eCVT_MAX;
		m_pstrTargetValue = 0;
		m_pEventVariable = 0;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventListValueSelectionActiver::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(VALUE_CHANGE_TYPE)
			{
				m_eValueeChangeType = GetValueeChangeType(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_TARGET_VALUE)
			{
				m_pstrTargetValue = new std::wstring();
				*m_pstrTargetValue = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
			{
				m_pEventVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
			}
		PARSE_NAME_VALUE_END
		m_pListValuChangerBase = cListValuChangerBase::GetListValuChangerBaseByElement(e_pTiXmlElement->FirstChildElement());		
	}

	//cEventListValueSelectionActiver::cEventListValueSelectionActiver(cListValuChangerBase*e_pListValuChangerBase,eValueeChangeType e_eValueeChangeType,WCHAR*e_strValue)
	//{
	//	m_eValueeChangeType = e_eValueeChangeType;
	//	m_pListValuChangerBase = 0;
	//	m_pstrTargetValue = 0;
	//	if( e_pListValuChangerBase )
	//		m_pListValuChangerBase = e_pListValuChangerBase->Clone();
	//	if( m_eValueeChangeType == eCVT_TARGET_VALUE )
	//	{
	//		m_pstrTargetValue = new std::wstring();
	//		*m_pstrTargetValue = e_strValue;
	//	}
	//}

	cEventListValueSelectionActiver::cEventListValueSelectionActiver(cEventListValueSelectionActiver*e_pListValueChangerActiver)
	{
		m_pEventVariable = e_pListValueChangerActiver->m_pEventVariable;
		m_eValueeChangeType = e_pListValueChangerActiver->m_eValueeChangeType;
		m_pListValuChangerBase = 0;
		m_pstrTargetValue = 0;
		if( e_pListValueChangerActiver->m_pListValuChangerBase )
			m_pListValuChangerBase = e_pListValueChangerActiver->m_pListValuChangerBase->Clone();
		if( m_eValueeChangeType == eCVT_TARGET_VALUE )
		{
			m_pstrTargetValue = new std::wstring();
			*m_pstrTargetValue = *e_pListValueChangerActiver->m_pstrTargetValue;
		}	
	}

	cEventListValueSelectionActiver::~cEventListValueSelectionActiver()
	{
		SAFE_DELETE(m_pListValuChangerBase);
		SAFE_DELETE(m_pstrTargetValue);
	}

	void	cEventListValueSelectionActiver::InternalInit()
	{

	}
	void	cEventListValueSelectionActiver::InternalUpdate(float e_fElpasedTime)
	{
		this->m_bSatisfiedCondition = true;
		if( m_pEventVariable )
		{
			m_pListValuChangerBase->SetSelectedValue(m_pEventVariable->ConvertToString().c_str());
		}
		switch(m_eValueeChangeType)
		{
			case eCVT_INCREASE:
				m_pListValuChangerBase->Next();
				break;
			case eCVT_DECREASE:
				m_pListValuChangerBase->Prior();
				break;
			case eCVT_TARGET_VALUE:
				m_pListValuChangerBase->SetSelectedValue(m_pstrTargetValue->c_str());
				break;
			case eCVT_RANDOM_VALUE:
				m_pListValuChangerBase->SetSelectedValue(m_pListValuChangerBase->GetRandomValue());
				break;
		}
		WCHAR*	l_str = m_pListValuChangerBase->ConvertSelectedDataToString();
		if( m_pEventVariable )
			m_pEventVariable->SetDataByString(l_str);
	}
	void	cEventListValueSelectionActiver::InternalRender()
	{

	}

	cListValuChangerBase*		cEventListValueSelectionActiver::GetListValueChanger()
	{
		return this->m_pListValuChangerBase;
	}

	cIntListValueChanger*		cEventListValueSelectionActiver::GetIntListValueChanger()
	{
		return (cIntListValueChanger*)this->m_pListValuChangerBase;
	}

	cFloatListValueChanger*		cEventListValueSelectionActiver::GetFloatListValueChanger()
	{
		return (cFloatListValueChanger*)this->m_pListValuChangerBase;
	}

	cWStringListValueChanger*	cEventListValueSelectionActiver::GetWStringListValueChanger()
	{
		return (cWStringListValueChanger*)this->m_pListValuChangerBase;
	}

	cStringListValueChanger*		cEventListValueSelectionActiver::GetStringListValueChanger()
	{
		return (cStringListValueChanger*)this->m_pListValuChangerBase;
	}

	TiXmlElement*		cEventListValueSelectionActiver::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		if( m_eValueeChangeType != eCVT_MAX)
		{
			l_pEvent->SetAttribute(VALUE_CHANGE_TYPE,ValueToStringW(m_eValueeChangeType));
		}
		if( m_pstrTargetValue )
		{
			l_pEvent->SetAttribute(EVENT_TARGET_VALUE,m_pstrTargetValue->c_str());
		}
		if(m_pEventVariable)
		{
			l_pEvent->SetAttribute(cEventVariable::TypeID,m_pEventVariable->GetName());
		}
		if(m_pListValuChangerBase)
		{
			TiXmlElement*l_pListValuChangerBase = m_pListValuChangerBase->ToTiXmlElement();
			l_pEvent->LinkEndChild(l_pListValuChangerBase);
		}
		return l_pEvent;
	}
}