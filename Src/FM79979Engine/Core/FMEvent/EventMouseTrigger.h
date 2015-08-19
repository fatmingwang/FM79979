#ifndef _EVENT_MOUSE_TRIGGER_H_
#define _EVENT_MOUSE_TRIGGER_H_

#include "EventBase.h"
#include "../AllCoreInclude.h"
namespace FATMING_CORE
{
	//it also combine key data?
	//<cEventMouseTrigger Gesture="ClickUp" >
	//<cEventMouseTrigger Gesture="eGE_SLIDE_UP" Distance="100">
	class	cEventMouseTrigger:public cEventBase
	{
		eGestureEnum    m_eCurrentGestureEnum;
		//if gesture need to be moved,m_fMoveDistance will be checked
		float			m_fMoveDistance;
		//ex:horver or slide
		//sTimeCounter	m_LeastTimeTC;
		GET_SET_DEC(eGestureEnum,m_eGestureEnum,GetGestureEnum,SetGestureEnum);
		virtual	void	InternalInit();
		virtual	void	InternalUpdate(float e_fElpasedTime);
		virtual	void	InternalRender();

        virtual void	InternalMouseMove(int e_iPosX,int e_iPosY);
        virtual void	InternalMouseDown(int e_iPosX,int e_iPosY);
        virtual void    InternalMouseUp(int e_iPosX,int e_iPosY);
        virtual void    InternalMouseDoubleClickUp(int e_iPosX,int e_iPosY);
		virtual bool    InternalCollide(int e_iPosX,int e_iPosY){ return true; }
	public:
		DEFINE_TYPE_INFO();
		cEventMouseTrigger(TiXmlElement*e_pTiXmlElement);
		cEventMouseTrigger(cEventMouseTrigger*e_pEventTest);
		EVENT_CLONE_DEFINE(cEventMouseTrigger);
		virtual ~cEventMouseTrigger();
		virtual	TiXmlElement*		ToTiXmlElement();
	};


	//m_pParentEventObjectInstance,get it from hirechay
	//
	//<cEventObjectMouseUpDectet cEventObjectInstance="GeneratorTest" />
	class	cEventObjectInstance;
	class	cEventObjectMouseUpDectet:public cEventBase
	{
		cEventObjectInstance*	m_pParentEventObjectInstance;
		std::wstring			m_strEventObjectInstanceName;
		bool					m_bWaitForPlayDone;
		bool					m_bEventObjectInstanceMouseUp;
		//
		virtual	void		InternalInit();
		virtual	void		InternalUpdate(float e_fElpasedTime);
	public:
		DEFINE_TYPE_INFO();
		cEventObjectMouseUpDectet();
		virtual ~cEventObjectMouseUpDectet();
		virtual	TiXmlElement*		ToTiXmlElement();
	};

//end namespace FATMING_CORE
}
#endif