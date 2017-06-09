#include "../../Stdafx.h"
#include "../Mouse_Touch.h"
#include "../../Sound/BasicSound.h"
#include "ClickBehavior.h"
#include "DefaultRenderClickBehavior.h"
#include "../GameApp.h"
#define MOUSE_UP_IDLE_TIME	0.1f

namespace FATMING_CORE
{

	TYPDE_DEFINE_MARCO(cClickBehavior);
	TYPDE_DEFINE_MARCO(cClickBehaviorGroup);
	TYPDE_DEFINE_MARCO(cClickBehaviorDispatcher);

	#define LAZY_MOUSE_FUNCTION(Function,PosX,PosY) if(Function)Function(PosX,PosY,this)

	cClickBehavior::cClickBehavior()
	{
		m_bSwallowedTouch = true;
		m_TCForMouseUp.SetTargetTime(MOUSE_UP_IDLE_TIME);
		m_bEnable = true;
		m_eObjectMouseBehavior = eOMB_NONE;
		m_pMouseMoveData = new sMouseMoveData();
		m_pClickSound = nullptr;
		m_CollideFunction = nullptr;
		m_MouseDownFunction = nullptr;
		m_MouseHorverFunction = nullptr;
		m_MouseUpFunction = nullptr;
		m_MouseDoubleClickFunction = nullptr;
		m_MouseLeaveFunction = nullptr;
		m_bAllowDrag = false;
	}


	cClickBehavior::~cClickBehavior()
	{
		SAFE_DELETE(m_pMouseMoveData);
		SAFE_RELEASE(m_pClickSound,this);	
	}

	//cClickBehavior*	cClickBehavior::LazyCreate(cRenderObject*e_pRenderObject,bool e_bUseDefaultClickEffect)
	//{
	//	cClickBehaviorButtonRenderStatus*l_pClickEventButtonRenderStatus = new cClickBehaviorButtonRenderStatus();
	//	l_pClickEventButtonRenderStatus->SetRenderObject(e_pRenderObject);
	//	cClickBehavior*l_pClickEvent = new cClickBehavior(l_pClickEventButtonRenderStatus,e_bUseDefaultClickEffect);
	//	return l_pClickEvent;
	//}

	void	cClickBehavior::Init()
	{
		m_TCForMouseUp.Start();
		m_eObjectMouseBehavior = eOMB_NONE;
		m_pMouseMoveData->Init();
		m_bCollided = false;
	}

	//bool	cClickBehavior::MouseLeave(int e_iPosX,int e_iPosY)
	//{
	//	if( m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ||
	//		m_eObjectMouseBehavior == eOMB_HORVER)
	//	{
	//		m_eObjectMouseBehavior = eObjectMouseBehavior::eOMB_LEAVE;
	//		return true;
	//	}
	//}

	bool	cClickBehavior::Collide(int e_iPosX,int e_iPosY)
	{
		if( m_bAllowDrag && m_bCollided )
			return true;
		m_bCollided = false;
		if(this->m_CollideFunction && m_CollideFunction(e_iPosX,e_iPosY))
		{
			m_bCollided = true;;
		}
		return m_bCollided;
	}

    cClickBehavior*    cClickBehavior::MouseDown(int e_iPosX,int e_iPosY)
	{
		if( !m_bEnable )
			return false;
		if( m_eObjectMouseBehavior == eOMB_NONE)
		{
			if(Collide(e_iPosX,e_iPosY))
			{
				m_pMouseMoveData->MouseDown(e_iPosX,e_iPosY);
				m_eObjectMouseBehavior = eOMB_FIRST_TIME_INTO;
				LAZY_MOUSE_FUNCTION(m_MouseDownFunction,e_iPosX,e_iPosY);
				return this;
			}
		}
		m_eObjectMouseBehavior = eOMB_NONE;
		return nullptr;
	}

    cClickBehavior*    cClickBehavior::MouseMove(int e_iPosX,int e_iPosY)
	{
		if( !m_bEnable )
			return false;
		if( m_eObjectMouseBehavior == eOMB_HORVER||
			m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ||
			m_eObjectMouseBehavior == eOMB_LEAVE)
		{
			m_pMouseMoveData->MouseMove(e_iPosX,e_iPosY);
			if(Collide(e_iPosX,e_iPosY))
			{
				m_eObjectMouseBehavior = eOMB_HORVER;
				LAZY_MOUSE_FUNCTION(m_MouseHorverFunction,e_iPosX,e_iPosY);
			}
			else
			{
				m_eObjectMouseBehavior = eOMB_LEAVE;
				LAZY_MOUSE_FUNCTION(m_MouseLeaveFunction,e_iPosX,e_iPosY);
			}
			return this;
		}
		return nullptr;
	}

    cClickBehavior*    cClickBehavior::MouseUp(int e_iPosX,int e_iPosY)
	{
		if( !m_bEnable)
			return false;
		if(this->Collide(e_iPosX,e_iPosY))
		{
			m_bEnable = false;
			bool l_bDoubleClick = m_pMouseMoveData->MouseUp(e_iPosX,e_iPosY);
			if( !l_bDoubleClick )
				m_eObjectMouseBehavior = eOMB_UP;
			else
				m_eObjectMouseBehavior = eOMB_DOUBLU_CLICK_UP;
			return this;
		}
		else
		{
			m_eObjectMouseBehavior = eOMB_NONE;

		}
		return nullptr;
	}

	void    cClickBehavior::Update(float e_fElpaseTime)
	{
		if(m_eObjectMouseBehavior == eOMB_UP || m_eObjectMouseBehavior == eOMB_DOUBLU_CLICK_UP)
		{
			m_TCForMouseUp.Update(e_fElpaseTime);
			if( m_TCForMouseUp.bTragetTimrReached )
			{
				//make a time offset
				if(m_eObjectMouseBehavior == eOMB_DOUBLU_CLICK_UP)
				{
					LAZY_MOUSE_FUNCTION(m_MouseDoubleClickFunction,this->m_pMouseMoveData->UpPos.x,this->m_pMouseMoveData->UpPos.y);
				}
				else
				{
					LAZY_MOUSE_FUNCTION(m_MouseUpFunction,this->m_pMouseMoveData->UpPos.x,this->m_pMouseMoveData->UpPos.y);
				}
				if( m_pClickSound )
					m_pClickSound->Play(true);
				this->Init();
				m_bEnable = true;
			}
		}
	}

	cBasicSound*	cClickBehavior::GetClickSound()
	{
		return this->m_pClickSound;	
	}

	void	cClickBehavior::SetClickSound(cBasicSound*e_pBasicSound)
	{
		SAFE_RELEASE(m_pClickSound,this);
		if( e_pBasicSound )
		{
			m_pClickSound = e_pBasicSound;
			m_pClickSound->AddRef(this);
		}
	}

	void	cClickBehavior::SetMouseFunction(CollideFunction	e_CollideFunction,
													ClickFunction		e_MouseDownFunction,
													ClickFunction		e_MouseHorverFunction,
													ClickFunction		e_MouseUpFunction,
													ClickFunction		e_MouseDoubleClickFunction,
													ClickFunction		e_MouseLeaveFunction)
	{
		m_CollideFunction			   = e_CollideFunction;
		m_MouseDownFunction			   = e_MouseDownFunction;
		m_MouseHorverFunction		   = e_MouseHorverFunction;
		m_MouseUpFunction			   = e_MouseUpFunction;
		m_MouseDoubleClickFunction	   = e_MouseDoubleClickFunction;
		m_MouseLeaveFunction		   = e_MouseLeaveFunction;
	}


	void	cClickBehavior::CreateFullScreenCollide()
	{
		this->m_CollideFunction = FullscreenCollide;
	}

	cClickBehaviorGroup::cClickBehaviorGroup()
	{

	}

	cClickBehaviorGroup::~cClickBehaviorGroup()
	{

	}


    cClickBehavior*    cClickBehaviorGroup::MouseDown(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return false;		
		int	l_uiSize = Count();
		if( l_uiSize != 0 )
		{
			for( int i=0;i<l_uiSize;++i )
			{
				//because we will try to hit from last to first
				int	l_iTargtIndexFromBackToFront = l_uiSize-1-i;
				cClickBehavior*l_pClickEvent = this->GetObject(l_iTargtIndexFromBackToFront);
				cClickBehavior*l_pResult = l_pClickEvent->MouseDown(e_iPosX,e_iPosY);
				if( l_pResult && l_pResult->IsSwallowedTouch())
				{
					return l_pResult;
				}
			}
		}
		if(cClickBehavior::MouseDown(e_iPosX,e_iPosY))
		{
			if( this->IsSwallowedTouch() )
			{
				return this;
			}
		}
		return nullptr;
	}

    cClickBehavior*	cClickBehaviorGroup::MouseMove(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return false;
		int	l_uiSize = Count();
		for( int i=0;i<l_uiSize;++i )
		{
			int	l_iTargtIndexFromBackToFront = l_uiSize-1-i;
			cClickBehavior*l_pClickEvent = this->GetObject(l_iTargtIndexFromBackToFront);
			cClickBehavior*l_pResult = l_pClickEvent->MouseMove(e_iPosX,e_iPosY);
			if( l_pResult && l_pResult->IsSwallowedTouch())
			{
				return l_pResult;
			}
		}
		if(cClickBehavior::MouseMove(e_iPosX,e_iPosY))
		{
			if( this->IsSwallowedTouch() )
				return this;
		}
		return nullptr;
	}

    cClickBehavior*	cClickBehaviorGroup::MouseUp(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return false;
		int	l_uiSize = Count();
		for( int i=0;i<l_uiSize;++i )
		{
			int	l_iTargtIndexFromBackToFront = l_uiSize-1-i;
			cClickBehavior*l_pClickEvent = this->GetObject(l_iTargtIndexFromBackToFront);
			cClickBehavior*l_pResult = l_pClickEvent->MouseUp(e_iPosX,e_iPosY);
			if( l_pResult && l_pResult->IsSwallowedTouch())
			{
				return l_pResult;
			}
		}
		if(cClickBehavior::MouseUp(e_iPosX,e_iPosY))
		{
			if( this->IsSwallowedTouch() )
				return this;
		}
		return nullptr;
	}


	void	cClickBehaviorGroup::Init()
	{
		cClickBehavior::Init();
		for(auto l_pData :this->m_ObjectList)
		{
			l_pData->Init();
		}
	}

    void    cClickBehaviorGroup::Update(float e_fElpaseTime)
	{
		int	l_uiSize = Count();
		for( int i=0;i<l_uiSize;++i )
		{
			(*this)[i]->Update(e_fElpaseTime);
		}
	}

	cClickBehavior*	cClickBehaviorGroup::AddDefaultRenderClickBehaviorButton(cRenderObject*e_pRenderObject,ClickFunction e_ClickFunction,cBasicSound*e_pBasicSound)
	{
		if( !e_pRenderObject )
			return nullptr;
		cDefaultRenderClickBehavior*l_pClickEvent = new cDefaultRenderClickBehavior();
		l_pClickEvent->SetRenderObject(e_pRenderObject);
		if(!e_pRenderObject->GenerateBound())
		{
			std::wstring l_strDebugInfo = e_pRenderObject->GetName();
			l_strDebugInfo += L" generate bound faild,the object dont override the GenerateBound";
			cGameApp::OutputDebugInfoString(l_strDebugInfo);
		}
		l_pClickEvent->SetMouseFunction(nullptr,nullptr,nullptr,e_ClickFunction,e_ClickFunction,nullptr);
		l_pClickEvent->SetName(e_pRenderObject->GetName());
		l_pClickEvent->SetClickSound(e_pBasicSound);
		this->AddObjectNeglectExist(l_pClickEvent);
		return l_pClickEvent;
	}

	//bool	cClickBehaviorGroup::AddButton(cRenderObject*e_pRenderObject,ClickFunction e_ClickFunction,cBasicSound*e_pClickSound)
	//{
	//	cSimpleObjectClickRespond*l_pObjectClickRespond = new cSimpleObjectClickRespond();
	//	l_pObjectClickRespond->SetRenderObject(e_pRenderObject);
	//	l_pObjectClickRespond->SetName(e_pRenderObject->GetName());
	//	l_pObjectClickRespond->SetClickSound(e_pClickSound);
	//	l_pObjectClickRespond->SetClickFunction(e_ClickFunction);
	//	cClickBehavior*l_pClickEvent = new cClickBehavior(l_pObjectClickRespond);
	//	l_pClickEvent->SetName(e_pRenderObject->GetName());
	//	this->AddObject(l_pClickEvent);
	//	return true;
	//}

	cClickBehaviorDispatcher::cClickBehaviorDispatcher()
	{
		m_pCurrentWorkingEvent = 0;
	}

	cClickBehaviorDispatcher::~cClickBehaviorDispatcher()
	{
		
	}

	void    cClickBehaviorDispatcher::Update(float e_fElpaseTime)
	{
		if(!this->IsEnable())
			return;
		if( m_pCurrentWorkingEvent )
		{
			m_pCurrentWorkingEvent->Update(e_fElpaseTime);
			if( m_pCurrentWorkingEvent->m_eObjectMouseBehavior == eObjectMouseBehavior::eOMB_NONE )
				m_pCurrentWorkingEvent = nullptr;
		}
		else
		{
			cClickBehaviorGroup::Update(e_fElpaseTime);
		}
		m_AlwaysNeedToWorkClickEventGroup.Update(e_fElpaseTime);
	}

    cClickBehavior*    cClickBehaviorDispatcher::MouseDown(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return nullptr;
		m_pCurrentWorkingEvent = cClickBehaviorGroup::MouseDown(e_iPosX,e_iPosY);
		//m_AlwaysNeedToWorkClickEventGroup.MouseDown(e_iPosX,e_iPosY);
		return m_pCurrentWorkingEvent ;
	}

    cClickBehavior*    cClickBehaviorDispatcher::MouseMove(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return nullptr;
		if( m_pCurrentWorkingEvent )
			m_pCurrentWorkingEvent->MouseMove(e_iPosX,e_iPosY);
		//m_AlwaysNeedToWorkClickEventGroup.MouseMove(e_iPosX,e_iPosY);
		return m_pCurrentWorkingEvent;
	}

    cClickBehavior*    cClickBehaviorDispatcher::MouseUp(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return nullptr;
		if( m_pCurrentWorkingEvent )
			m_pCurrentWorkingEvent->MouseUp(e_iPosX,e_iPosY);
		//m_AlwaysNeedToWorkClickEventGroup.MouseUp(e_iPosX,e_iPosY);
		return m_pCurrentWorkingEvent;
	}

	void	cClickBehaviorDispatcher::SetTopClickEvent(cClickBehavior*e_pEvent)
	{
		m_pCurrentWorkingEvent = e_pEvent;
	}
}