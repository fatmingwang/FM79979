#ifndef _EVENT_INSTANCE_SWITCHER_H_
#define _EVENT_INSTANCE_SWITCHER_H_

#include "EventBase.h"
namespace FATMING_CORE
{
//<cEventInstanceSwitcher>
//	<ConditionAndNextEventInstance TargetResult="0" CinditionVarialbe="ooxx" TargetEventInstanceName="aabb">
//	<ConditionAndNextEventInstance TargetResult="1" CinditionVarialbe="ooxx2" TargetEventInstanceName="aabb2">
//	<ConditionAndNextEventInstance TargetResult="2" CinditionVarialbe="ooxx3" TargetEventInstanceName="aabb3">
//</cEventInstanceSwitcher>

	//get parent cEventInstanceObject to set m_pCurrentEventInstance
	class	cEventObjectInstance;
	class	cEventInstanceSwitcherActiver:public cEventBase
	{
		struct	sConditionAndNextEventInstance
		{
			std::wstring	strTargetResult;
			std::wstring	strConditionVarialbe;
			std::wstring	strTargetEventInstanceName;
		};
		std::vector<sConditionAndNextEventInstance>		m_ConditionAndNextEventInstanceVector;
		//Activer->EventInstance->EventObjectInstance
		cEventObjectInstance*			m_pParentEventObjectInstance;
		virtual	void					InternalInit()override;
		virtual	void					InternalUpdate(float e_fElpasedTime)override;
		virtual	void					InternalRender()override;
	public:
		DEFINE_TYPE_INFO();
		cEventInstanceSwitcherActiver(TiXmlElement*e_pTiXmlElement);
		cEventInstanceSwitcherActiver(cEventInstanceSwitcherActiver*e_pEventInstanceSwitcher);
		EVENT_CLONE_DEFINE(cEventInstanceSwitcherActiver);
		virtual ~cEventInstanceSwitcherActiver();
		virtual	TiXmlElement*		ToTiXmlElement()override;
	};
//end namespace
}
//end _EVENT_INSTANCE_SWITCHER_H_
#endif