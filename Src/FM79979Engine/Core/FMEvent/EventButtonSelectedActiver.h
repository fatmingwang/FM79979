#ifndef _EVENT_MULTI_BUTTON_SELECTED_ACTIVER_H_
#define _EVENT_MULTI_BUTTON_SELECTED_ACTIVER_H_

#include "EventBase.h"

namespace FATMING_CORE
{
	//===========================
	//cEventButtonSelectedActiver must exists with cEventMultiButton.
	//
	//<cEventButtonSelectedActiver Name="79979" cEventVariable="CurrentPackageName" Result="SecondPage,ThirdPage,FourthPage"/>
	//if cEventMultiButton is not eBST_ONE,cEventVariable data will be set as click button 's order to set data from Result
	//===========================
	class	cEventVariable;
	class	cEventButtonSelectedActiver:public cEventBase
	{
		GETP_SET_DEC(std::vector<std::wstring>,m_strResult,GetResult,SetResult);
		GET_SET_DEC(cEventVariable*,m_pEventVariable,GetEventVariable,SetEventVariable);
		virtual	void					InternalInit();
		virtual	void					InternalUpdate(float e_fElpasedTime);
		virtual	void					InternalRender();
	public:
		DEFINE_TYPE_INFO();
		cEventButtonSelectedActiver(TiXmlElement*e_pTiXmlElement);
		cEventButtonSelectedActiver(cEventButtonSelectedActiver*e_pEventMultiButtonSelectedActiver);
		EVENT_CLONE_DEFINE(cEventButtonSelectedActiver);
		virtual ~cEventButtonSelectedActiver();
		virtual	TiXmlElement*		ToTiXmlElement();
	};
}

#endif