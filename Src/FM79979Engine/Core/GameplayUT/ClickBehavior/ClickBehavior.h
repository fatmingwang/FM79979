#pragma once
//
//
//		April/22/2015
//
//		for click event.
//		May/26/2017
//		add cClickBehaviorButtonRenderStatus
#include <functional>

class Frame;

namespace FATMING_CORE
{
	struct sMouseMoveData;
	class cBasicSound;
	class cClickBehavior;
	class cClickBehaviorDispatcher;
	class cCueToStartCurveWithTime;

	typedef	std::function<void(int,int,cClickBehavior*)>		ClickFunction;
	typedef	std::function<bool(int,int)>						CollideFunction;
	inline	bool	FullscreenCollide(int e_iPodX,int e_iPodY){ return true; }

	class cClickBehavior:public virtual NamedTypedObject
	{
		friend	class					cClickBehaviorDispatcher;
	protected:
		eObjectMouseBehavior			m_eObjectMouseBehavior;
		UT::sTimeCounter				m_TCForMouseUp;
		cBasicSound*					m_pClickSound;
        bool							m_bCollided;
		sMouseMoveData*					m_pMouseMoveData;
		//if false mouse won't work,because its possible game set this object click event will open later.
		GET_SET_DEC(bool,m_bEnable,IsEnable,SetEnable);
		//if true, the touch will be eat
		GET_SET_DEC(bool,m_bSwallowedTouch,IsSwallowedTouch,SetSwallowedTouch);
		//
		virtual bool					Collide(int e_iPosX,int e_iPosY);
        //first time into
        virtual cClickBehavior*			MouseDown(int e_iPosX,int e_iPosY);
        //horver and move
        virtual cClickBehavior*			MouseMove(int e_iPosX,int e_iPosY);
        //
        virtual cClickBehavior*			MouseUp(int e_iPosX,int e_iPosY);
		//while first click collide is true always horver.
		bool							m_bAllowDrag;
		CollideFunction					m_CollideFunction;
		ClickFunction					m_MouseDownFunction;
		ClickFunction					m_MouseHorverFunction;
		ClickFunction					m_MouseUpFunction;
		ClickFunction					m_MouseDoubleClickFunction;
		ClickFunction					m_MouseLeaveFunction;
	public:
		cClickBehavior();
		~cClickBehavior();
		DEFINE_TYPE_INFO()
		GET_SET(bool,m_bAllowDrag,IsAllowDrag,SetAllowDrag);
		//
		//static cClickBehavior*				LazyCreate(cRenderObject*e_pRenderObject,bool e_bUseDefaultClickEffect = true);
		void							Init();
        virtual void    				Update(float e_fElpaseTime);
		//void							SetRepeatTime(float e_fRepeatTime);
		bool							IsCollided(){ return m_bCollided; }
		sMouseMoveData*					GetMouseMoveData(){return m_pMouseMoveData;}
		cBasicSound*					GetClickSound();
		void							SetClickSound(cBasicSound*e_pBasicSound);
		eObjectMouseBehavior			GetObjectMouseBehavior(){return m_eObjectMouseBehavior;}
		void							SetMouseFunction(CollideFunction	e_CollideFunction,
														ClickFunction		e_MouseDownFunction,
														ClickFunction		e_MouseHorverFunction,
														ClickFunction		e_MouseUpFunction,
														ClickFunction		e_MouseDoubleClickFunction,
														ClickFunction		e_MouseLeaveFunction);
		//I am a mask to avoid click other buttons.
		void							CreateFullScreenCollide();
	};
	//for a single touch group,a current working object to save performance
	class cClickBehaviorGroup:public cClickBehavior,public cNamedTypedObjectVector<cClickBehavior>
	{
		friend	class 				cClickBehaviorDispatcher;
	protected:
        //first time into
        virtual cClickBehavior*		MouseDown(int e_iPosX,int e_iPosY)override;
        //horver and move
        virtual cClickBehavior*   	MouseMove(int e_iPosX,int e_iPosY)override;
        //
        virtual cClickBehavior*    	MouseUp(int e_iPosX,int e_iPosY)override;
	public:
		cClickBehaviorGroup();
		~cClickBehaviorGroup();
		DEFINE_TYPE_INFO()
		void													Init();
        virtual void    										Update(float e_fElpaseTime)override;
		cClickBehavior*											AddDefaultRenderClickBehaviorButton(cRenderObject*e_pRenderObject,ClickFunction e_ClickFunction,cBasicSound*e_pBasicSound);
		std::tuple<cClickBehavior*,cRenderObject*>				AddDefaultRenderClickBehaviorButton(cCueToStartCurveWithTime*e_pSubMPDI,ClickFunction e_ClickFunction,cBasicSound*e_pBasicSound);
		virtual	NamedTypedObject*	Clone()override { return nullptr; }
	};
	//its higher than cClickBehaviorGroup,because it has top menu and always need to work click event vector
	class cClickBehaviorDispatcher:public cClickBehaviorGroup
	{
		//while this exists m_ClickEventVector won't work.
		cClickBehavior*				m_pCurrentWorkingEvent;
		cClickBehaviorGroup			m_AlwaysNeedToWorkClickEventGroup;
	public:
		cClickBehaviorDispatcher();
		~cClickBehaviorDispatcher();
		DEFINE_TYPE_INFO()
		virtual void    			Update(float e_fElpaseTime)override;
        virtual cClickBehavior*    	MouseDown(int e_iPosX,int e_iPosY)override;
        virtual cClickBehavior*    	MouseMove(int e_iPosX,int e_iPosY)override;
        virtual cClickBehavior*	   	MouseUp(int e_iPosX,int e_iPosY)override;
		bool						AddAlwaysNeedToWorkClickEvent(cClickBehavior*e_pClickEvent);
		bool						RemoveAlwaysNeedToWorkClickEvent(cClickBehavior*e_pClickEvent);
		void						SetTopClickEvent(cClickBehavior*e_pEvent);
	};


	class cLazyClickBehaviorAndRenderObject:public NamedTypedObject
	{
	public:
		cClickBehaviorGroup*	m_pClickBehaviorGroup;
		cRenderObject*			m_pRenderObject;
		cLazyClickBehaviorAndRenderObject(cClickBehaviorGroup*e_pClickBehaviorGroup,cRenderObject*e_pRenderObject);
		cLazyClickBehaviorAndRenderObject();
		~cLazyClickBehaviorAndRenderObject();
		void	AddChild(cRenderObject*e_pRenderObject);
		void	AddObject(cClickBehavior*e_pClickBehavior);
		void	SetEnable(bool e_bEnable);
		bool	IsEnable();
		void	Init();
		void	SetNameToAll(const wchar_t*e_strName);
	};
}