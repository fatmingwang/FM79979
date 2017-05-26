#pragma once
//
//
//		April/22/2015
//
//		for click event.
//		May/26/2017
//		add cClickEventButtonRenderStatus
#include <functional>

class Frame;

namespace FATMING_CORE
{
	class cBasicSound;
	//for move object or something slese
	class cObjectClickRespond
	{
		cMatrix44		m_OnClickMatrix;
		bool			m_bIsOnClick;
	public:
		cObjectClickRespond(){m_bIsOnClick = false;}
		~cObjectClickRespond(){}
		bool			IsOnClick(){return m_bIsOnClick;}
		virtual	void	OnClick(Frame*e_pFrame);
		virtual	void	OnLeave(Frame*e_pFrame);
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
	};
	//if more dynamic animation is required,inherent this
	class cClickEventButtonRenderStatus:public NamedTypedObject
	{
		//this will be delete
		GET_SET_DEC(cRenderObject*,m_pRenderObject,GetRenderObject,SetRenderObject);
	public:
		cClickEventButtonRenderStatus();
		virtual ~cClickEventButtonRenderStatus();
		virtual void	Init();
		virtual void	Update(float e_fElpaseTime,eObjectMouseBehavior e_eObjectMouseBehavior);
		virtual void	Render(eObjectMouseBehavior e_eObjectMouseBehavior);
	};

	struct sMouseMoveData;
	class cClickEventDispatcher;
	typedef	std::function<bool(Frame*,int,int)>	ClickFunction;
	typedef	std::function<bool(int,int)>		CollideFunction;
	//sometimes we wanna do collide in our MouseClickEvent(ClickFunction) function;
	inline bool	AlwaysHitCollideFunction(int e_iPosX,int e_iPosY){ return true; }

	class cClickEvent:public NamedTypedObject
	{
		friend	class		cClickEventDispatcher;
		//
		cBasicSound*					m_pClickSound;
		//this one will be delete
		cClickEventButtonRenderStatus*	m_pClickEventButtonRenderStatus;
		//
		CollideFunction					m_CollideFunction;
		virtual		bool				DoCollide(int e_iPosX,int e_iPosY);
		//if true, the touch will be eat
		GET_SET_DEC(bool,m_bSwallowedTouch,IsSwallowedTouch,SetSwallowedTouch);
		sMouseMoveData*					m_pMouseMoveData;
		//to show the object clicked
		cObjectClickRespond*			m_pObjectClickRespond;
		//
		//please set this at InternalCllided or mouse down or the plac u expaected.
        bool							m_bCollided;
		eObjectMouseBehavior			m_eObjectMouseBehavior;
		Frame*							m_pTargetFrame;
		UT::sTimeCounter				m_TCForMouseUp;
		//UT::sTimeCounter				m_TCForRepeat;
		//static	float					m_sfRepeatTime;
		//if false mouse won't work,because its possible game set this object click event will open later.
		GET_SET_DEC(bool,m_bEnable,IsEnable,SetEnable);
		//for button event.
		ClickFunction					m_MouseDownFunction;
		ClickFunction					m_MouseMoveFunction;
		ClickFunction					m_MouseUpFunction;
		ClickFunction					m_MouseLeaveFunction;
		ClickFunction					m_ClickedFunction;
		//not working for now,because I thought stamp click time is not help at all.
		//so I am lazy to implement it now.
		ClickFunction					m_DoubleClickedFunction;
        //first time into
        virtual cClickEvent*			MouseDown(int e_iPosX,int e_iPosY);
        //horver and move
        virtual cClickEvent*			MouseMove(int e_iPosX,int e_iPosY);
        //
        virtual cClickEvent*			MouseUp(int e_iPosX,int e_iPosY);
	public:
		cClickEvent(cClickEventButtonRenderStatus*e_pClickEventButtonRenderStatus,bool e_bUseDefaultClickEffect = true);
		cClickEvent(bool e_bUseDefaultClickEffect = true);
		~cClickEvent();
		//
		static cClickEvent*				LazyCreate(cRenderObject*e_pRenderObject,bool e_bUseDefaultClickEffect = true);
		void							Init();
        virtual void    				Update(float e_fElpaseTime);
		virtual void					Render();
		//void							SetRepeatTime(float e_fRepeatTime);
		bool							IsCollided(){ return m_bCollided; }
		void							SetTargetFrame(Frame*e_pFrame){m_pTargetFrame = e_pFrame;}
        //
		void							SetClickFunction(ClickFunction	e_MouseDownFunction,
														ClickFunction	e_MouseMoveFunction,
														ClickFunction	e_ClickedFunction,
														ClickFunction	e_MouseUpFunction,
														ClickFunction	e_MouseLeaveFunction,
														ClickFunction	e_DoubleClickedFunction);
		void							CopyClickFunctionFromTaget(cClickEvent*e_pClickEvent);
		void							SetCollideFunction(CollideFunction	e_CollideFunction);
		sMouseMoveData*					GetMouseMoveData(){return m_pMouseMoveData;}
		cClickEventButtonRenderStatus*	GetClickEventButtonRenderStatus();
		void							SetClickEventButtonRenderStatus(cClickEventButtonRenderStatus*e_pClickEventButtonRenderStatus,bool e_bClone);
		cBasicSound*					GetClickSound();
		void							SetClickSound(cBasicSound*e_pBasicSound);
	};
	//for a single touch group,a current working object to save performance
	class cClickEventGroup:public cClickEvent,public cNamedTypedObjectVector<cClickEvent>
	{
		friend	class 				cClickEventDispatcher;
	protected:
        //first time into
        virtual cClickEvent*		MouseDown(int e_iPosX,int e_iPosY);
        //horver and move
        virtual cClickEvent*   		MouseMove(int e_iPosX,int e_iPosY);
        //
        virtual cClickEvent*    	MouseUp(int e_iPosX,int e_iPosY);
	public:
		cClickEventGroup();
		~cClickEventGroup();
		void						Init();
        virtual void    			Update(float e_fElpaseTime);
		virtual void				Render();
	};
	//its higher than cClickEventGroup,because it has top menu and always need to work click event vector
	class cClickEventDispatcher:public cClickEventGroup
	{
		//while this exists m_ClickEventVector won't work.
		cClickEvent*							m_pCurrentWorkingEvent;
		cClickEventGroup						m_AlwaysNeedToWorkClickEventGroup;
	public:
		cClickEventDispatcher();
		~cClickEventDispatcher();
		virtual void    				Update(float e_fElpaseTime);
        virtual cClickEvent*    		MouseDown(int e_iPosX,int e_iPosY);
        virtual cClickEvent*    		MouseMove(int e_iPosX,int e_iPosY);
        virtual cClickEvent*	   		MouseUp(int e_iPosX,int e_iPosY);
		bool							AddAlwaysNeedToWorkClickEvent(cClickEvent*e_pClickEvent);
		bool							RemoveAlwaysNeedToWorkClickEvent(cClickEvent*e_pClickEvent);
		void							SetTopClickEvent(cClickEvent*e_pEvent);
	};
}