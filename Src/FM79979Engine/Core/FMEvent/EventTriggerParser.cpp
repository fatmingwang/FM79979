#include "../stdafx.h"
#include "EventButtonTrigger.h"
#include "EventMouseTrigger.h"
#include "EventValueCheckerTrigger.h"
#include "EventManager.h"
#include "EventSelectionCheckerTrigger.h"
#include "EventObjectStatusCheckrTrigger.h"
namespace   FATMING_CORE
{
	cEventBase*	cEventManager::GetTriggerEvent(TiXmlElement*e_pTiXmlElement)
	{
		cEventBase*l_pEventBase = 0;
		const WCHAR*l_strValue = e_pTiXmlElement->Value();
		NEW_EVENT_BT_XML_ELEMENT(cEventButton,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventMultiButton,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventMouseTrigger,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventValueCheck,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventKeyUpDectect,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventSelectionCheckerTrigger,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventObjectStatusCheckerTrigger,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventMouseTrigger,e_pTiXmlElement)
		{
			UT::ErrorMsg(l_strValue,L"unsupport value");
		}
		//else
		//COMPARE_VALUE_WITH_DEFINE(cEventMouseTrigger::TypeID)
		//{

		//}}
		return l_pEventBase;
	}
//end namespace   FATMING_CORE
}