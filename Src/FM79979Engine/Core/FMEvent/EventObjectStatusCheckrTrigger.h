#ifndef _EVENT_OBJECT_STATUS_CHECKER_TRIGER_H_
#define _EVENT_OBJECT_STATUS_CHECKER_TRIGER_H_

#include "EventBase.h"

namespace FATMING_CORE
{
//====================
//no wonly check status is done
//and u could indicate statusName or ignore to froce to check current status is done or not
//<cEventObjectStatusCheckerTrigger cEventObjectInstance="Name" />
//<cEventObjectStatusCheckerTrigger cEventObjectInstance="Name" StatusName="" />
//====================
	class	cEventObjectInstance;
	class	cEventObjectStatusCheckerTrigger:public cEventBase
	{
		cEventObjectInstance*	m_pEventObjectInstance;
		std::wstring			m_strEventObjectInstanceName;
		std::wstring*			m_pstrStatusName;
		//
		virtual	void	InternalInit()override;
		virtual	void	InternalUpdate(float e_fElpasedTime)override;
	public:
		DEFINE_TYPE_INFO();
		//cEventObjectStatusCheckerTrigger();
		cEventObjectStatusCheckerTrigger(TiXmlElement*e_pTiXmlElement);
		cEventObjectStatusCheckerTrigger(cEventObjectStatusCheckerTrigger*e_pEventObjectStatusCheckerTrigger);
		EVENT_CLONE_DEFINE(cEventObjectStatusCheckerTrigger);
		virtual ~cEventObjectStatusCheckerTrigger();
		virtual	TiXmlElement*		ToTiXmlElement()override;

	};
//end namespace
}
#endif