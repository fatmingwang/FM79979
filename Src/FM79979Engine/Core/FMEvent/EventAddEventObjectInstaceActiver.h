#ifndef _EVENT_ADD_EVENT_OBJECT_INSTANCE_H_
#define _EVENT_ADD_EVENT_OBJECT_INSTANCE_H_

namespace FATMING_CORE
{
	//<cEventAddEventObjectInstaceActiver Name="" cEventObjectInstance="" cEventInstance="" cEventObjectStatus="" AllStatusWorking="0" cEventVariable="" />
	class	cEventAddEventObjectInstaceActiver:public cEventBase
	{
		std::wstring					m_strEventObjectInstanceName;
		std::wstring					m_strEventInstanceName;
		std::wstring					m_strEventStatusName;
		std::wstring					m_strEventVariableName;
		bool							m_bAllEventObjectStatusWorking;
		virtual	void					InternalInit()override;
		virtual	void					InternalUpdate(float e_fElpasedTime)override;
	public:
		DEFINE_TYPE_INFO();
		cEventAddEventObjectInstaceActiver(TiXmlElement*e_pTiXmlElement);
		cEventAddEventObjectInstaceActiver(cEventAddEventObjectInstaceActiver*e_pEventAddEventObjectInstace);
		EVENT_CLONE_DEFINE(cEventAddEventObjectInstaceActiver);
		virtual ~cEventAddEventObjectInstaceActiver();
		virtual	TiXmlElement*		ToTiXmlElement()override;
	};
//end FATMING_CORE
}

#endif