#ifndef _EVENT_TRIGGER_PARSER_H_
#define _EVENT_TRIGGER_PARSER_H_

#include "EventButtonTrigger.h"
#include "EventMouseTrigger.h"
#include "EventValueCheckerTrigger.h"
namespace   FATMING_CORE
{
	cEventBase*	GetTriggerEvent(TiXmlElement*e_pTiXmlElement);
//end namespace   FATMING_CORE
}
#endif