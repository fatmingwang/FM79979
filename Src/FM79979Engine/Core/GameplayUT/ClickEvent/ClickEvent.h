#pragma once
//
//
//		April/22/2015
//
//		for click event.
//
#include <functional>

class Frame;

namespace FATMING_CORE
{
	//although click event might order by tree view,
	//but it's too hard to implement,
	//so I make a cClickEventVector to make a set as a Event,
	//and please use cClickEventVector as a group,because usgae of moulti touch,
	//current working object might not a good idea under cClickEventDispatcher
	//although multouch event is good but too complex for the user,so use cClickEventVector
	//for a one touch event is better.
	class cObjectClickRespond
	{
		cMatrix44		m_OnClickMatrix;
		bool	m_bIsOnClick;
	public:
		cObjectClickRespond(){m_bIsOnClick = false;}
		~cObjectClickRespond(){}
		bool			IsOnClick(){return m_bIsOnClick;}
		virtual	void	OnClick(Frame*e_pFrame);
		virtual	void	OnLeave(Frame*e_pFrame);
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
	};
	struct sMouseMoveData;
	class cClickEventDispatcher;
	typedef	std::function<bool(Frame*,int,int)>	ClickFunction;
	typedef	std::function<bool(int,int)>		CollideFunction;
	//sometimes we wanna do collide in our MouseClickEvent(ClickFunction) function;
	bool	AlwaysHitCollideFunction(int e_iPosX,int e_iPosY){ return true; }
	class cClickEvent:public NamedTypedObject
	{
		CollideFunction		m_CollideFunction;
		virtual		bool	DoCollide(int e_iPosX,int e_iPosY);
		friend class cClickEventDispatcher;
		//means could be always click even the button is hiden.
		//so please ensure overlap is not happen.
		GET_SET_DEC(bool,m_bAllowParallelClick,IsAllowParallelClick,SetAllowParallelClick);
		sMouseMoveData*			m_pMouseMoveData;
		//to show the object clicked
		cObjectClickRespond*	m_pObjectClickRespond;
		//
		//please set this at InternalCllided or mouse down or the plac u expaected.
        bool					m_bCollided;
		eObjectMouseBehavior    m_eObjectMouseBehavior;
		Frame*					m_pTargetFrame;
		UT::sTimeCounter		m_TCForMouseUp;
		//UT::sTimeCounter		m_TCForRepeat;
		//static	float			m_sfRepeatTime;
		//if false mouse won't work,because its possible game set this object click event will open later.
		GET_SET_DEC(bool,m_bEnable,IsEnable,SetEnable);
		//for button event.
		ClickFunction			m_MouseDownFunction;
		ClickFunction			m_MouseMoveFunction;
		ClickFunction			m_MouseUpFunction;
		ClickFunction			m_MouseLeaveFunction;
		ClickFunction			m_ClickedFunction;
		//not working for now,because I thought stamp click time is not help at all.
		//so I am lazy to implement it now.
		ClickFunction			m_DoubleClickedFunction;
        //first time into
        virtual bool    		MouseDown(int e_iPosX,int e_iPosY);
        //horver and move
        virtual bool    		MouseMove(int e_iPosX,int e_iPosY);
        //
        virtual bool    		MouseUp(int e_iPosX,int e_iPosY);
	public:
		cClickEvent(bool e_bUseDefaultClickEffect = true);
		~cClickEvent();
		void					Init();
        virtual void    		Update(float e_fElpaseTime);
		//void					SetRepeatTime(float e_fRepeatTime);
		bool					IsCollided(){ return m_bCollided; }
		void					SetTargetFrame(Frame*e_pFrame){m_pTargetFrame = e_pFrame;}
        //
		void					SetClickFunction(ClickFunction	e_MouseDownFunction,
												ClickFunction	e_MouseMoveFunction,
												ClickFunction	e_ClickedFunction,
												ClickFunction	e_MouseUpFunction,
												ClickFunction	e_MouseLeaveFunction,
												ClickFunction	e_DoubleClickedFunction);
		void					CopyClickFunctionFromTaget(cClickEvent*e_pClickEvent);
		void					SetCollideFunction(CollideFunction	e_CollideFunction);
		sMouseMoveData*			GetMouseMoveData(){return m_pMouseMoveData;}
	};
	//for a single touch group,a current working object to save performance
	class cClickEventGroup:public cClickEvent,public cNamedTypedObjectVector<cClickEvent>
	{
	protected:
		cClickEvent*				m_pCurrentWorkingEvent;
        //first time into
        virtual bool    			MouseDown(int e_iPosX,int e_iPosY);
        //horver and move
        virtual bool    			MouseMove(int e_iPosX,int e_iPosY);
        //
        virtual bool    			MouseUp(int e_iPosX,int e_iPosY);
	public:
		cClickEventGroup();
		~cClickEventGroup();
		void						Init();
        virtual void    			Update(float e_fElpaseTime);
	};

	class cClickEventDispatcher:public cClickEventGroup
	{
		//while this exists m_ClickEventVector won't work.
		cClickEvent*				m_pTopMenu;
	public:
		cClickEventDispatcher();
		~cClickEventDispatcher();
        virtual bool    		MouseDown(int e_iPosX,int e_iPosY);
        virtual bool    		MouseMove(int e_iPosX,int e_iPosY);
        virtual bool    		MouseUp(int e_iPosX,int e_iPosY);
		bool					AddClickEvent(cClickEvent*e_pClickEvent);
		bool					RemoveClickEvent(cClickEvent*e_pClickEvent);
		void					SetTopClickEvent(cClickEvent*e_pEvent);
	};
}