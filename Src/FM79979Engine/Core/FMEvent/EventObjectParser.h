#ifndef _EVENT_OBJECT_PARSER_H_
#define _EVENT_OBJECT_PARSER_H_

#include "EventImage.h"
#include "EventMPDI.h"
#include "EventParticle.h"
#include "EventSound.h"
#include "EventText.h"

namespace   FATMING_CORE
{
	cEventObject*	GetcEventObject(TiXmlElement*e_pTiXmlElement);
//end namespace   FATMING_CORE
}
#endif