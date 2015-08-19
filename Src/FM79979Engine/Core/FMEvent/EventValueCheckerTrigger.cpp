#include "../stdafx.h"
#include "EventHeaderDefine.h"
#include "EventValueCheckerTrigger.h"
#include "EventVariable.h"
#include "EventManager.h"
namespace FATMING_CORE
{
	const wchar_t*         cEventKeyUpDectect::TypeID( L"cEventKeyUpDectect" );
	const wchar_t*         cEventValueCheck::TypeID( L"cEventValueCheck" );

	cEventKeyUpDectect::cEventKeyUpDectect(TiXmlElement*e_pTiXmlElement)
	{
		m_ucKeyData = -1;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventKeyUpDectect::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(EVENT_KEY_DATA)
			{
				this->m_ucKeyData = (unsigned char)l_strValue[0];
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_KEY_VALUE)
			{
				m_ucKeyData = (unsigned char)VALUE_TO_INT;
			}
			else
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
		PARSE_NAME_VALUE_END
	}
	//<cEventKeyUpDectect KeyData="M" />
	TiXmlElement*		cEventKeyUpDectect::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		if( m_ucKeyData != -1 )
		{
			l_pEvent->SetAttribute(EVENT_KEY_DATA,(unsigned char)this->m_ucKeyData);
		}
		return l_pEvent;
	}

	cEventKeyUpDectect::cEventKeyUpDectect(cEventKeyUpDectect*e_pEventKeyUpDectect)
	{
		m_ucKeyData = e_pEventKeyUpDectect->m_ucKeyData;
	}

	cEventKeyUpDectect::~cEventKeyUpDectect()
	{
	
	}

	void	cEventKeyUpDectect::InternalInit()
	{
		cClickMouseBehavior::Init();
	}

	void	cEventKeyUpDectect::InternalUpdate(float e_fElpasedTime)
	{
		if( !this->m_bKeyDown )
		{
			if(cGameApp::m_sucKeyData[this->m_ucKeyData])
			{
				this->m_bKeyDown = true;
			}
		}
		else
		if(!cGameApp::m_sucKeyData[this->m_ucKeyData])
		{
			this->m_bSatisfiedCondition = true;
		}
	}

	//<cEventValueCheck Value="" cEventVariable="" Same="0" DataType="int" Value="1"  >
	//<cEventValueCheck cEventVariable=""  CompareEventVariable="" Same="0" DataType="int" Value="1"  >
	cEventValueCheck::cEventValueCheck(TiXmlElement*e_pTiXmlElement)
	{
		m_pstrCheckValue = 0;
		m_pstrInitCompareEventVariableString = 0;
		m_bSame = true;
		m_pCheckTargetData = 0;
		m_pCompareEventVariable = 0;
		bool	l_bInitCompareEventVariable = false;
		std::wstring	l_strInitCompareEventVariable;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventValueCheck::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(DATA_TYPE)
			{
				//m_eDataType = GetDataType(l_strValue);
			}
			else
			COMPARE_NAME("Same")
			{
				m_bSame = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("Value")
			{
				m_pstrCheckValue = new std::wstring;
				*m_pstrCheckValue = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
			{
				m_pCheckTargetData = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
				if( !m_pCheckTargetData )
				{
					UT::ErrorMsg(l_strValue,L"cEventValueCheck variable not exists");
				}
			}
			else
			COMPARE_NAME("CompareEventVariable")
			{
				m_pCompareEventVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
				if( !m_pCheckTargetData )
				{
					UT::ErrorMsg(l_strValue,L"cEventValueCheck CompareEventVariable variable not exists");
				}
			}
			else
			COMPARE_NAME("InitCompareEventVariable")
			{
				m_pstrInitCompareEventVariableString = new std::wstring;
				*m_pstrInitCompareEventVariableString = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
		PARSE_NAME_VALUE_END
		if( m_pstrInitCompareEventVariableString&&m_pCompareEventVariable )
		{
			m_pCompareEventVariable->SetDataByString(m_pstrInitCompareEventVariableString->c_str());
		}
	}

	//cEventValueCheck::cEventValueCheck(const WCHAR*e_strCheckValue,eDataType e_eDataType,cEventVariable*e_pCheckTargetData,bool	e_bSame)
	//{
	//	m_bSame = e_bSame;
	//	m_strCheckValue = e_strCheckValue;
	//	m_eDataType = e_eDataType;
	//	m_pCheckTargetData = e_pCheckTargetData;
	//}

	cEventValueCheck::cEventValueCheck(cEventValueCheck*e_pEventValueCheck)
	{
		m_pstrCheckValue = 0;
		m_bSame = e_pEventValueCheck->m_bSame;
		m_pstrInitCompareEventVariableString = 0;
		if(e_pEventValueCheck->m_pstrInitCompareEventVariableString)
		{
			m_pstrInitCompareEventVariableString = new std::wstring;
			*m_pstrInitCompareEventVariableString = *e_pEventValueCheck->m_pstrInitCompareEventVariableString;
		}
		if( e_pEventValueCheck->m_pstrCheckValue )
		{
			m_pstrCheckValue = new std::wstring;
			*m_pstrCheckValue = *e_pEventValueCheck->m_pstrCheckValue;
		}
		//m_eDataType = e_pEventValueCheck->m_eDataType;
		m_pCheckTargetData = e_pEventValueCheck->m_pCheckTargetData;
		m_pCompareEventVariable = e_pEventValueCheck->m_pCompareEventVariable;
		if( m_pstrInitCompareEventVariableString )
		{
			m_pCompareEventVariable->SetDataByString(m_pstrInitCompareEventVariableString->c_str());		
		}
	}

	cEventValueCheck::~cEventValueCheck()
	{
		SAFE_DELETE(m_pstrInitCompareEventVariableString);
		SAFE_DELETE(m_pstrCheckValue);
	}

	void	cEventValueCheck::InternalInit()
	{

	}

	void	cEventValueCheck::InternalUpdate(float e_fElpasedTime)
	{
		if( m_pCheckTargetData )
		{
			if( m_pCompareEventVariable )
			{
				bool	l_bSame = m_pCheckTargetData->IsSame(m_pCompareEventVariable->ConvertToString().c_str());
				if( l_bSame )
					m_bSatisfiedCondition = m_bSame?true:false;
				else
					m_bSatisfiedCondition = m_bSame?false:true;
			}
			else
			{
				bool	l_bSame = m_pCheckTargetData->IsSame(m_pstrCheckValue->c_str());
				if( l_bSame )
					m_bSatisfiedCondition = m_bSame?true:false;
				else
					m_bSatisfiedCondition = m_bSame?false:true;
			}
		}
	}

	void	cEventValueCheck::InternalRender()
	{

	}
	//<cEventValueCheck  cEventVariable="2266" Same="0" DataType="int" Value="1"  >
	//<cEventValueCheck cEventVariable="79979"  CompareEventVariable="2266" Same="0"  >
	TiXmlElement*		cEventValueCheck::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		if(m_pCheckTargetData)
		{
			l_pEvent->SetAttribute(cEventVariable::TypeID,m_pCheckTargetData->GetName());
		}
		if( m_pCompareEventVariable )
		{
			l_pEvent->SetAttribute(L"CompareEventVariable",m_pCompareEventVariable->GetName());
		}
		if( m_pstrCheckValue )
		{
			l_pEvent->SetAttribute(L"Value",m_pstrCheckValue->c_str());
		}
		l_pEvent->SetAttribute(L"Same",m_bSame?1:0);
		if( m_pstrInitCompareEventVariableString )
		{
			l_pEvent->SetAttribute(L"InitCompareEventVariable",m_pstrInitCompareEventVariableString->c_str());
		}
		return l_pEvent;
	}
//end namespace FATMING_CORE
}