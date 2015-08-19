#include "../stdafx.h"
#include "EventValueChangerActiver.h"
#include "EventObjecInstanceStatusChangeActiver.h"
#include "EventButtonSelectedActiver.h"
#include "EventExternalFunctionCallActiver.h"
#include "EventVariableCheckYesAndNoActiver.h"
#include "EventInstanceSwitcherActiver.h"
#include "EventManager.h"
namespace   FATMING_CORE
{
	cEventBase*	cEventManager::GetActiverEvent(TiXmlElement*e_pTiXmlElement)
	{
		cEventBase*l_pEventBase = 0;
		const WCHAR*l_strValue = e_pTiXmlElement->Value();
		NEW_EVENT_BT_XML_ELEMENT(cEventValueChangeActiver,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventListValueSelectionActiver,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventObjectInstanceStatusChangeActiver,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventButtonSelectedActiver,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventExternalFunctionCallActiver,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventVariableCheckYesAndNoActiver,e_pTiXmlElement)
		NEW_EVENT_BT_XML_ELEMENT(cEventInstanceSwitcherActiver,e_pTiXmlElement)
		{
			UT::ErrorMsg(l_strValue,L"unsupport value");
		}
		return l_pEventBase;
	}
//end namespace   FATMING_CORE
}