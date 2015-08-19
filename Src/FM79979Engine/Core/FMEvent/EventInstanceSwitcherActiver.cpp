#include "../stdafx.h"
#include "EventVariable.h"
#include "EventInstanceSwitcherActiver.h"
#include "EventManager.h"
namespace FATMING_CORE
{
	const wchar_t*         cEventInstanceSwitcherActiver::TypeID( L"cEventInstanceSwitcherActiver" );
//<cEventInstanceSwitcher>
//	<ConditionAndNextEventInstance TargetResult="0" ConditionVarialbe="ooxx" TargetEventInstanceName="aabb">
//	<ConditionAndNextEventInstance TargetResult="1" ConditionVarialbe="ooxx2" TargetEventInstanceName="aabb2">
//	<ConditionAndNextEventInstance TargetResult="2" ConditionVarialbe="ooxx3" TargetEventInstanceName="aabb3">
//</cEventInstanceSwitcher>
	cEventInstanceSwitcherActiver::cEventInstanceSwitcherActiver(TiXmlElement*e_pTiXmlElement)
	{
		m_pParentEventObjectInstance = 0;
		if( !e_pTiXmlElement )
			return;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventInstanceSwitcherActiver::TypeID);
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while(e_pTiXmlElement)
		{
			sConditionAndNextEventInstance	l_sConditionAndNextEventInstance;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
				{
					l_sConditionAndNextEventInstance.strConditionVarialbe = l_strValue;
				}
				else
				COMPARE_NAME("TargetEventInstanceName")
				{
					l_sConditionAndNextEventInstance.strTargetEventInstanceName = l_strValue;
				}
				else
				COMPARE_NAME("Result")
				{
					l_sConditionAndNextEventInstance.strTargetResult = l_strValue;
				}
			PARSE_NAME_VALUE_END
			m_ConditionAndNextEventInstanceVector.push_back(l_sConditionAndNextEventInstance);
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
	}

	cEventInstanceSwitcherActiver::cEventInstanceSwitcherActiver(cEventInstanceSwitcherActiver*e_pEventMultiButtonSelectedActiver)
	{
		m_ConditionAndNextEventInstanceVector = e_pEventMultiButtonSelectedActiver->m_ConditionAndNextEventInstanceVector;
		m_pParentEventObjectInstance = 0;
	}

	cEventInstanceSwitcherActiver::~cEventInstanceSwitcherActiver()
	{
	
	}

	void		cEventInstanceSwitcherActiver::InternalInit()
	{
		assert(m_pEventInstance&&"no m_pEventInstance,cEventInstanceSwitcherActiver::InternalInit()");;
		m_pParentEventObjectInstance = dynamic_cast<cEventObjectInstance*>(this->m_pEventInstance->GetOwner());
		assert(m_pParentEventObjectInstance&&"no m_pParentEventObjectInstance?cEventInstanceSwitcherActiver::InternalInit()");
		size_t	l_ui = m_ConditionAndNextEventInstanceVector.size();
		for( size_t i = 0;i<l_ui;++i )
		{
			if(!g_pCurrentEventManager->GetEventVariable(this->m_ConditionAndNextEventInstanceVector[i].strConditionVarialbe.c_str()))
			{
				UT::ErrorMsg(this->m_ConditionAndNextEventInstanceVector[i].strConditionVarialbe.c_str(),L"there is no such variable");
			}
			if(m_pParentEventObjectInstance->GetEventInstanceList()->GetObjectIndexByName(m_ConditionAndNextEventInstanceVector[i].strTargetEventInstanceName.c_str()) == -1 )
			{
				UT::ErrorMsg(this->m_ConditionAndNextEventInstanceVector[i].strConditionVarialbe.c_str(),L"there is no such EventInstance");
			}
		}
	}

	void		cEventInstanceSwitcherActiver::InternalUpdate(float e_fElpasedTime)
	{
		size_t	l_ui = m_ConditionAndNextEventInstanceVector.size();
		for( size_t i = 0;i<l_ui;++i )
		{
			cEventInstance*l_pEventInstance = m_pParentEventObjectInstance->GetEventInstanceList()->GetObject(m_ConditionAndNextEventInstanceVector[i].strTargetEventInstanceName.c_str());
			if( !l_pEventInstance->IsSatisfiedCondition() )
			{
				cEventVariable*l_pEventVariable = g_pCurrentEventManager->GetEventVariable(this->m_ConditionAndNextEventInstanceVector[i].strConditionVarialbe.c_str());
				if(l_pEventVariable->IsSame(m_ConditionAndNextEventInstanceVector[i].strTargetResult.c_str()))
				{
					cEventInstance*l_pEventInstance = m_pParentEventObjectInstance->GetEventInstanceList()->GetObject(m_ConditionAndNextEventInstanceVector[i].strTargetEventInstanceName.c_str());
					m_pParentEventObjectInstance->SetCurrentEventInstance(l_pEventInstance);
				}	
			}
		}
		this->m_bSatisfiedCondition = true;
	}

	void		cEventInstanceSwitcherActiver::InternalRender()
	{
	
	}

//<cEventInstanceSwitcher>
//	<ConditionAndNextEventInstance TargetResult="0" ConditionVarialbe="ooxx" TargetEventInstanceName="aabb">
//	<ConditionAndNextEventInstance TargetResult="1" ConditionVarialbe="ooxx2" TargetEventInstanceName="aabb2">
//	<ConditionAndNextEventInstance TargetResult="2" ConditionVarialbe="ooxx3" TargetEventInstanceName="aabb3">
//</cEventInstanceSwitcher>
	TiXmlElement*		cEventInstanceSwitcherActiver::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		size_t	l_ui = m_ConditionAndNextEventInstanceVector.size();
		for( size_t i = 0;i<l_ui;++i )
		{
			TiXmlElement*l_pChild = new TiXmlElement(L"ConditionAndNextEventInstance");
			l_pChild->SetAttribute(L"TargetResult",this->m_ConditionAndNextEventInstanceVector[i].strTargetResult.c_str());
			l_pChild->SetAttribute(L"ConditionVarialbe",this->m_ConditionAndNextEventInstanceVector[i].strConditionVarialbe.c_str());
			l_pChild->SetAttribute(L"TargetEventInstanceName",this->m_ConditionAndNextEventInstanceVector[i].strTargetEventInstanceName.c_str());
			l_pEvent->LinkEndChild(l_pChild);
		}
		return l_pEvent;
	}

//end namespace
}