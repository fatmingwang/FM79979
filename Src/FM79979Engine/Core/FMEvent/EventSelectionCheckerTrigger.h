#ifndef _EVENT_SELECTUIN_CHECKER_TRIGGER_H_
#define _EVENT_SELECTUIN_CHECKER_TRIGGER_H_

#include "EventBase.h"
#include "../AllCoreInclude.h"
namespace FATMING_CORE
{
	//ButtonResult will get from name of pressed buttons
	//<cEventSelectionCheckerTrigger Name="79979" ButtonSelectionType="Repeat" StayAtLastSelect="0" ClickCount="5" HintPos="230,560" CompareResultVariable="VendorPassword" OutputResultVariable="InputPassword" KeepDoingUntilSameResult="1"  >
	//<cEventSelectionCheckerTrigger Name="79979" ButtonSelectionType="One" StayAtLastSelect="0" HintPos="" CompareResultVariable="" OutputResultVariable="79979" >
	//	<cEventButton Name="1" Text="停一" BGColor="1,0,0.5,1" Pos="260,200,0" KeyData="C" />
	//	<cEventButton Name="2" Text="停二" BGColor="1,0,0.5,1" Pos="260,300,0" KeyData="V" />
	//	<cEventButton Name="3" Text="停三" BGColor="1,0,0.5,1" Pos="260,400,0" KeyData="B" />
	//	<cEventButton Name="4" Text="停四4" BGColor="1,0,0.5,1" Pos="260,500,0" KeyData="N" />
	//	<cEventButton Name="5" Text="停五5" BGColor="1,0,0.5,1" Pos="260,600,0" KeyData="M" />
	//	<cEventButton Name="6" Text="押注" BGColor="1,0,0.5,1" Pos="260,700,0" KeyData="L" />
	//	<cEventButton Name="7" Text="押線" BGColor="1,0,0.5,1" Pos="260,800,0" KeyData="I" />
	//	<cEventButton Name="8" Text="轉" BGColor="1,0,0.5,1" Pos="260,900,0" KeyData="S" />
	//	<cEventButton Name="9" Text="自動轉" BGColor="1,0,0.5,1" Pos="260,900,0" KeyData="A" />
	//</cEventSelectionCheckerTrigger>
	class	cEventSelectionCheckerTrigger:public cEventMultiButton
	{
		//the final result to compare with what button u have pressed
		cEventVariable*		m_pCompareResultVariable;
		cEventVariable*		m_pOutputResultVariable;
		//if input result is not match it will keep dogint this event
		bool				m_bKeepDoingUntilSameResult;
		bool				m_bShowHint;
		Vector2				m_vHintPos;
		virtual	void		InternalUpdate(float e_fElpasedTime);
		virtual	void		InternalRender();
	public:
		DEFINE_TYPE_INFO();
		cEventSelectionCheckerTrigger(TiXmlElement*e_pTiXmlElement);
		//cEventMultiButton(eButtonSelectionType e_eButtonSelectionType);
		cEventSelectionCheckerTrigger(cEventSelectionCheckerTrigger*e_pEventSelectionCheckerTrigger);
		EVENT_CLONE_DEFINE(cEventSelectionCheckerTrigger);
		virtual ~cEventSelectionCheckerTrigger();
		virtual	TiXmlElement*		ToTiXmlElement();
	};
}
//end namespace FATMING_CORE
#endif