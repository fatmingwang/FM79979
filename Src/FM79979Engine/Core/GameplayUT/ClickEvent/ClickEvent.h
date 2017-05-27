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
	struct sMouseMoveData;
	class cBasicSound;
	class cClickEvent;
	class cClickEventDispatcher;
	class cObjectClickRespond;

	typedef	std::function<void(cObjectClickRespond*)>			ClickFunction;

	//for move object or something slese
	class cObjectClickRespond:public NamedTypedObject
	{
		friend class	cClickEvent;
		//
		cBasicSound*	m_pClickSound;
		cMatrix44		m_OnClickMatrix;
		cMatrix44		m_OnHorverMatrix;
		bool			m_bIsOnClick; 
		//this won't be delete,just a reference,because render is keep i another render tree.
		GET_SET_DEC(cRenderObject*,m_pRenderObject,GetRenderObject,SetRenderObject);
		GET_SET_DEC(ClickFunction,m_pClickFunction,GetClickFunction,SetClickFunction);
		cClickEvent*	m_pClickEventParent;
	public:
		DEFINE_TYPE_INFO();
		cObjectClickRespond();
		~cObjectClickRespond();
		bool			IsOnClick(){return m_bIsOnClick;}
		virtual	void	OnClick();
		virtual	void	OnLeave();
		virtual	void	OnMouseUp();
		virtual	void	OnDoubleClick();
		virtual	void	OnHorver();
		virtual	void	Update(float e_fElpaseTime,eObjectMouseBehavior e_eObjectMouseBehavior);
		bool			Collide(int e_iPosX,int e_iPosY);
		cBasicSound*	GetClickSound();
		void			SetClickSound(cBasicSound*e_pBasicSound);
	};

	class cClickEvent:public NamedTypedObject
	{
		friend	class					cClickEventDispatcher;
		//
        bool							m_bCollided;
		eObjectMouseBehavior			m_eObjectMouseBehavior;
		UT::sTimeCounter				m_TCForMouseUp;
		virtual		bool				DoCollide(int e_iPosX,int e_iPosY);
		sMouseMoveData*					m_pMouseMoveData;
		//to show the object clicked
		cObjectClickRespond*			m_pObjectClickRespond;
		//if false mouse won't work,because its possible game set this object click event will open later.
		GET_SET_DEC(bool,m_bEnable,IsEnable,SetEnable);
		//if true, the touch will be eat
		GET_SET_DEC(bool,m_bSwallowedTouch,IsSwallowedTouch,SetSwallowedTouch);
        //first time into
        virtual cClickEvent*			MouseDown(int e_iPosX,int e_iPosY);
        //horver and move
        virtual cClickEvent*			MouseMove(int e_iPosX,int e_iPosY);
        //
        virtual cClickEvent*			MouseUp(int e_iPosX,int e_iPosY);
	public:
		cClickEvent(bool e_bUseDefaultClickEffect = true);
		cClickEvent(cObjectClickRespond*e_pObjectClickRespond);
		~cClickEvent();
		DEFINE_TYPE_INFO();
		//
		//static cClickEvent*				LazyCreate(cRenderObject*e_pRenderObject,bool e_bUseDefaultClickEffect = true);
		void							Init();
        virtual void    				Update(float e_fElpaseTime);
		//void							SetRepeatTime(float e_fRepeatTime);
		bool							IsCollided(){ return m_bCollided; }
		sMouseMoveData*					GetMouseMoveData(){return m_pMouseMoveData;}
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
		DEFINE_TYPE_INFO();
		void						Init();
        virtual void    			Update(float e_fElpaseTime);
		bool						AddButton(cObjectClickRespond*e_pObjectClickRespond);
		bool						AddButton(cRenderObject*e_pRenderObject,ClickFunction e_ClickFunction,cBasicSound*e_pClickSound);
		
	};
	//its higher than cClickEventGroup,because it has top menu and always need to work click event vector
	class cClickEventDispatcher:public cClickEventGroup
	{
		//while this exists m_ClickEventVector won't work.
		cClickEvent*				m_pCurrentWorkingEvent;
		cClickEventGroup			m_AlwaysNeedToWorkClickEventGroup;
	public:
		cClickEventDispatcher();
		~cClickEventDispatcher();
		DEFINE_TYPE_INFO();
		virtual void    			Update(float e_fElpaseTime);
        virtual cClickEvent*    	MouseDown(int e_iPosX,int e_iPosY);
        virtual cClickEvent*    	MouseMove(int e_iPosX,int e_iPosY);
        virtual cClickEvent*	   	MouseUp(int e_iPosX,int e_iPosY);
		bool						AddAlwaysNeedToWorkClickEvent(cClickEvent*e_pClickEvent);
		bool						RemoveAlwaysNeedToWorkClickEvent(cClickEvent*e_pClickEvent);
		void						SetTopClickEvent(cClickEvent*e_pEvent);
	};
}