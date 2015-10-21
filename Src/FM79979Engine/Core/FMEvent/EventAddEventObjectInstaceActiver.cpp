#include "../stdafx.h"
#include "EventVariable.h"
#include "EventManager.h"
#include "EventAddEventObjectInstaceActiver.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cEventAddEventObjectInstaceActiver);
	cEventAddEventObjectInstaceActiver::cEventAddEventObjectInstaceActiver(TiXmlElement*e_pTiXmlElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventAddEventObjectInstaceActiver::TypeID)
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(cEventObjectInstance::TypeID)
			{
				m_strEventObjectInstanceName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				SetName(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventInstance::TypeID)
			{
				m_strEventInstanceName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventObjectStatus::TypeID)
			{
				m_strEventStatusName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_OBJECTINSTANCE_ALL_STATUS_WORJING)
			{
				m_bAllEventObjectStatusWorking = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
			{
				m_strEventVariableName = l_strValue;
			}
			
		PARSE_NAME_VALUE_END
	}

	cEventAddEventObjectInstaceActiver::cEventAddEventObjectInstaceActiver(cEventAddEventObjectInstaceActiver*e_pEventAddEventObjectInstace)
	{
		m_strEventObjectInstanceName = e_pEventAddEventObjectInstace->m_strEventObjectInstanceName;
		m_strEventInstanceName = e_pEventAddEventObjectInstace->m_strEventInstanceName;
		m_strEventStatusName = e_pEventAddEventObjectInstace->m_strEventStatusName;
		m_bAllEventObjectStatusWorking = e_pEventAddEventObjectInstace->m_bAllEventObjectStatusWorking;
	}

	cEventAddEventObjectInstaceActiver::~cEventAddEventObjectInstaceActiver()
	{
	
	}

	void	cEventAddEventObjectInstaceActiver::InternalInit()
	{
		if(!g_pCurrentEventManager->IsEventInstanceElementExist(m_strEventInstanceName.c_str()))
		{
			UT::ErrorMsg(m_strEventInstanceName.c_str(),L"EventInstance is not exists");
		}
		if(!g_pCurrentEventManager->IsEventObjectStatusElementExist(m_strEventStatusName.c_str()))
		{
			UT::ErrorMsg(m_strEventStatusName.c_str(),L"EventObjectStatus is not exists");
		}
		if( g_pCurrentEventManager->m_pEventVariableManager->GetObject(m_strEventVariableName.c_str()) )
		{
			UT::ErrorMsg(m_strEventVariableName.c_str(),L"Eventvariable is not exists");
		}
	}

	void	cEventAddEventObjectInstaceActiver::InternalUpdate(float e_fElpasedTime)
	{
		g_pCurrentEventManager->AddEventObjectInstanceToCurrentPackage(m_strEventObjectInstanceName.c_str(),m_strEventInstanceName.c_str(),m_strEventStatusName.c_str(),this->m_bAllEventObjectStatusWorking,m_strEventVariableName.c_str());
		this->m_bSatisfiedCondition = true;
	}
	//<cEventAddEventObjectInstaceActiver cEventObjectInstance="" cEventInstance="" cEventObjectStatus="" cEventVariable="" AllStatusWorking="" />
	TiXmlElement*		cEventAddEventObjectInstaceActiver::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		l_pEvent->SetAttribute(cEventObjectInstance::TypeID,m_strEventObjectInstanceName.c_str());
		l_pEvent->SetAttribute(cEventInstance::TypeID,m_strEventInstanceName.c_str());
		l_pEvent->SetAttribute(cEventObjectStatus::TypeID,m_strEventStatusName.c_str());
		l_pEvent->SetAttribute(cEventVariable::TypeID,m_strEventVariableName.c_str());
		l_pEvent->SetAttribute(EVENT_OBJECTINSTANCE_ALL_STATUS_WORJING,m_bAllEventObjectStatusWorking?1:0);
		return l_pEvent;
	}
//end namespace FATMING_CORE
}