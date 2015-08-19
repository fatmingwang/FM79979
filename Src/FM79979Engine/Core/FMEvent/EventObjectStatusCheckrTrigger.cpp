#include "../stdafx.h"
#include "EventObjectStatusCheckrTrigger.h"
#include "EventObjectInstance.h"
#include "EventManager.h"
namespace FATMING_CORE
{
	const wchar_t*         cEventObjectStatusCheckerTrigger::TypeID( L"cEventObjectStatusCheckerTrigger" );

	//cEventObjectStatusCheckerTrigger::cEventObjectStatusCheckerTrigger()
	//{
	//
	//}
	//<cEventObjectStatusCheckerTrigger cEventObjectInstance="Name" />
	//<cEventObjectStatusCheckerTrigger cEventObjectInstance="Name" StatusName="" />
	cEventObjectStatusCheckerTrigger::cEventObjectStatusCheckerTrigger(TiXmlElement*e_pTiXmlElement)
	{
		m_pEventObjectInstance = 0;
		m_pstrStatusName = 0;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_STATUS_NAME)
			{
				m_pstrStatusName = new std::wstring;
				*m_pstrStatusName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventObjectInstance::TypeID)
			{
				m_strEventObjectInstanceName = l_strValue;
			}
		PARSE_NAME_VALUE_END
	}

	cEventObjectStatusCheckerTrigger::cEventObjectStatusCheckerTrigger(cEventObjectStatusCheckerTrigger*e_pEventObjectStatusCheckerTrigger)
	{
		m_pEventObjectInstance = 0;
		m_pstrStatusName = 0;
		if( e_pEventObjectStatusCheckerTrigger->m_pstrStatusName )
		{
			*m_pstrStatusName = *e_pEventObjectStatusCheckerTrigger->m_pstrStatusName;
		}
		m_strEventObjectInstanceName = e_pEventObjectStatusCheckerTrigger->m_strEventObjectInstanceName;
	}

	cEventObjectStatusCheckerTrigger::~cEventObjectStatusCheckerTrigger()
	{
		SAFE_DELETE(m_pstrStatusName);
	}
	//<cEventObjectStatusCheckerTrigger cEventObjectInstance="Name" />
	//<cEventObjectStatusCheckerTrigger cEventObjectInstance="Name" StatusName="" />
	TiXmlElement*		cEventObjectStatusCheckerTrigger::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		l_pEvent->SetAttribute(cEventObjectInstance::TypeID,m_strEventObjectInstanceName.c_str());
		if( m_pstrStatusName )
		{
			l_pEvent->SetAttribute(EVENT_STATUS_NAME,m_pstrStatusName->c_str());
		}
		return l_pEvent;
	}

	void	cEventObjectStatusCheckerTrigger::InternalInit()
	{
		m_pEventObjectInstance = g_pCurrentEventManager->GetEventObjectInstanceFromCurrentEventPackage(m_strEventObjectInstanceName.c_str());
		if( !m_pEventObjectInstance )
		{
			UT::ErrorMsg(L"cEventObjectStatusCheckerTrigger can not find cEventObjectInstance",m_strEventObjectInstanceName.c_str());
		}
	}

	void	cEventObjectStatusCheckerTrigger::InternalUpdate(float e_fElpasedTime)
	{
		if( !m_pEventObjectInstance )
			return;
		cEventObject*l_pEventObject = m_pEventObjectInstance->GetCurrentWorkingObject()->GetObject();
		if( m_pstrStatusName )
		{
			if( wcscmp(l_pEventObject->GetName(),m_pstrStatusName->c_str()) )
			{
				return;
			}
		}
		if(l_pEventObject->IsPlayDone())
			this->m_bSatisfiedCondition = true;
	}
}