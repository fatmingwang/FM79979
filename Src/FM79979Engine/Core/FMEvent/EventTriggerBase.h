#ifndef _EVENT_INSTANCE_H_
#define _EVENT_INSTANCE_H_

#include "EventBase.h"

namespace FATMING_CORE
{
	//time's up some thing not complete
	//time evnet could contain anything to its list
	//class	eEventTriggerWithTime
	//{
	//	UT::sTimeCounter	m_Time;
	//	cDoAllMouseBehaviorList<cEventBase>	m_TriggerEventList;
	//};

	class	cEventTriggerBase:public cEventBase
	{
	public:
		cEventTriggerBase():cEventBase();
		~cEventTriggerBase();
		//event tirgger could be satisfied but result could be failed(ex:input pw but not same)
		virtual	bool			IsSuccess(){ return true; }
	};
//end namespace FATMING_CORE
}