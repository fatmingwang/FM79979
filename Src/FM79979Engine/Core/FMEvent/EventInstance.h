#ifndef _EVENT_INSTANCE_H_
#define _EVENT_INSTANCE_H_

#include "EventBase.h"

namespace FATMING_CORE
{
	//==================
	//event instance coudl to be have more than one,
	//so we could switch current event instance by state ID
	//ex:
	//<EventInstanceList>
	//	<cEventInstance Name="EventTest" Repeat="1" >
	//		<EventTrigger>
	//			<cEventButton Name="up.png" BaseImage="Up" Pos="150,50,0" />
	//		</EventTrigger>
	//		<EventActiver>
	//			<cEventValueChangeActiver Name="Test1" ValueChangeType="Increase" TargetValue="1" cEventVariable="LinePlayable" >
	//				<cRestrictValue DataType="int" Max="25" Min="0" IncreaseValue="1" Value="1" />
	//			</cEventValueChangeActiver>
	//			<cEventObjectInstanceStatusChangeActiver cEventObjectInstance="GeneratorTest" StatusName="World" />
	//		</EventActiver>
	//	</cEventInstance>
	//	<cEventInstance Name="MinusBetLine" Repeat="1" >
	//		<EventTrigger>
	//			<cEventButton Name="" BaseImage="Up" Pos="0,50,0" />
	//		</EventTrigger>
	//		<EventActiver>
	//			<cEventValueChangeActiver Name="Test2" ValueChangeType="Decrease" TargetValue="1" cEventVariable="LinePlayable" >
	//				<cRestrictValue DataType="int" Max="25" Min="0" IncreaseValue="1" Value="1" />
	//			</cEventValueChangeActiver>
	//			<cEventObjectInstanceStatusChangeActiver cEventObjectInstance="GeneratorTest" StatusName="Hello" />
	//		</EventActiver>
	//	</cEventInstance>
	//	<cEventInstance Name="EventTest2" Repeat="1" >
	//		<EventTrigger>
	//			<cEventMultiButton Name="" ButtonSelectionType="One" StayAtLastSelect="1" >
	//				<cEventButton Name="11" BaseImage="Down" Pos="300,500,0" />
	//				<cEventButton Name="22" PI="Element" PIUnit="Gate" Pos="500,500,0" />
	//				<cEventButton Name="33" Text="79979 test" BGColor="1,0,0.5,1" Pos="400,500,0" />
	//			</cEventMultiButton>
	//		</EventTrigger>
	//		<EventActiver>
	//			<cEventButtonSelectedActiver Name="79979" cEventVariable="CurrentPackageName" Result="SecondPage,ThirdPage,FourthPage"/>
	//		</EventActiver>
	//	</cEventInstance>
	//</EventInstanceList>
	//the object own those two event instance must have a state name
	//so while state is changed,we can excute different event,even delete unnecessary event
	//==================
	//instance will send self into all childrend by fetch data.
	class	cEventInstance:public cEventBase
	{
		//I want to be activer forver so make always hit!.
		virtual bool    InternalCollide(int e_iPosX,int e_iPosY){ return true; }
		virtual	void	InternalMouseDown(int e_iX,int e_iY);
		virtual	void	InternalMouseUp(int e_iX,int e_iY);
		virtual	void	InternalMouseMove(int e_iX,int e_iY);
		virtual	void	InternalInit();
		virtual	void	InternalUpdate(float e_fElpasedTime);
		virtual	void	InternalRender();
		bool	m_bTriggerAllDone;
		bool	m_bActiveAllDone;
		//trigger and activer all done init itself again
		GET_SET_DEC(bool,m_bRepeat,IsRepeat,SetRepeat);
		//if true all trigger must fill condition at same time or it will initial trigger status
		GET_SET_DEC(bool,m_bTriggerAllDoneAtSameTime,IsTriggerAllDoneAtSameTime,SetTriggerAllDoneAtSameTime);
	public:
		DEFINE_TYPE_INFO();
		cDoAllMouseBehaviorList<cEventBase>	m_TriggerEventList;
		cDoAllMouseBehaviorList<cEventBase>	m_ActiveEventList;
		//
		bool	UpdateEventList(cNamedTypedObjectVector<cEventBase>*e_EventList,float e_fElpasedTime);
		void	RenderEventList(cNamedTypedObjectVector<cEventBase>*e_EventList);
		//if the event is from resource delete instance it won't delete event
		cEventInstance();
		cEventInstance(cEventInstance*e_pEventInstance);
		cEventInstance*Clone();
		virtual ~cEventInstance();
		//to get current info
		virtual	void	DebugRender();
		//now you could get all resource in m_TriggerEventList and m_ActiveEventList
		virtual	void	CollectResourceList(cNamedTypedObjectVector<NamedTypedObject>*e_pObjectListByName);
		virtual	TiXmlElement*	ToTiXmlElement();
	};
 /*!
   *  A function
   */

	/*!
	*object could have more than one event instance and switch them to update.
	*it could be a hireachy.
	*											 __[]
	*											/
	*									 __[]---___[]
	*									/__[]
	*               ---------------[]____[]
	*
	*each [] is a EventInstanceOwner
	*/
	//class	cEventInstanceOwner:public cNamedTypedObjectVector<cEventInstance>
	//{
	//	GET_SET_DEC(cEventInstance*,m_pCurrentEventInstance,GetCurrentEventInstance,SetCurrentEventInstance);
	//	WCHAR	m_strCurrentEventInstanceName[MAX_PATH];
	//	void	EventUpdate(float e_fElpasedTime);
	//	//void	EventRender();
	//	cEventInstanceOwner*m_pEventInstanceOwner;
	//public:
	//	cEventInstanceOwner(cEventInstanceOwner*e_pEventInstanceOwner,bool e_bEventInstanceFromResource = false);
	//	virtual ~cEventInstanceOwner();
	//	WCHAR	*GetCurrentEventInstanceName(){ return m_strCurrentEventInstanceName; }
	//	bool	SetEnevtInstanceByName(WCHAR*e_strEventInstanceName);
	//};
}
#endif