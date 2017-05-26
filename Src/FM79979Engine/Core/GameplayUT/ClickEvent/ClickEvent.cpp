#include "../../Stdafx.h"
#include "../Mouse_Touch.h"
#include "../../Sound/BasicSound.h"
#include "ClickEvent.h"
namespace FATMING_CORE
{
	void	cObjectClickRespond::OnClick(Frame*e_pFrame)
	{
		if( e_pFrame == nullptr )
			return;
		m_bIsOnClick = true;
		m_OnClickMatrix = e_pFrame->GetLocalTransform();
		cMatrix44 l_mat = m_OnClickMatrix*cMatrix44::ZAxisRotationMatrix(0.1f);
		e_pFrame->SetLocalTransform(l_mat);
	}
	void	cObjectClickRespond::OnLeave(Frame*e_pFrame)
	{
		if( e_pFrame == nullptr )
			return;
		m_bIsOnClick = false;
		e_pFrame->SetLocalTransform(m_OnClickMatrix);
	}
	void	cObjectClickRespond::Update(float e_fElpaseTime){}
	void	cObjectClickRespond::Render(){}

	cClickEventButtonRenderStatus::cClickEventButtonRenderStatus()
	{
		m_pRenderObject = nullptr;
	}
	cClickEventButtonRenderStatus::~cClickEventButtonRenderStatus()
	{
		SAFE_DELETE(m_pRenderObject);
	}

	void	cClickEventButtonRenderStatus::Init()
	{
		if( m_pRenderObject )
			m_pRenderObject->Init();
	}

	void	cClickEventButtonRenderStatus::Update(float e_fElpaseTime,eObjectMouseBehavior e_eObjectMouseBehavior)
	{
		if( m_pRenderObject )
			m_pRenderObject->Update(e_fElpaseTime);
	}
	void	cClickEventButtonRenderStatus::Render(eObjectMouseBehavior e_eObjectMouseBehavior)
	{
		if( m_pRenderObject )
			m_pRenderObject->Render();
	}

	cClickEvent::cClickEvent(bool e_bUseDefaultClickEffect)
	{
		m_pClickEventButtonRenderStatus = false;
		m_bSwallowedTouch = false;
		m_TCForMouseUp.SetTargetTime(0.3f);
		m_pMouseMoveData = new sMouseMoveData();
		m_pObjectClickRespond = nullptr;
		m_bEnable = true;
		m_pTargetFrame = nullptr;
		m_MouseDownFunction = nullptr;
		m_MouseMoveFunction = nullptr;
		m_MouseUpFunction = nullptr;
		m_ClickedFunction = nullptr;
		m_MouseLeaveFunction = nullptr;
		m_DoubleClickedFunction = nullptr;
		m_pClickSound = nullptr;
		m_eObjectMouseBehavior = eOMB_NONE;
		if( e_bUseDefaultClickEffect )
			m_pObjectClickRespond = new cObjectClickRespond();
	}

	cClickEvent::cClickEvent(cClickEventButtonRenderStatus*e_pClickEventButtonRenderStatus,bool e_bUseDefaultClickEffect)
	{
		m_pClickEventButtonRenderStatus = e_pClickEventButtonRenderStatus;
		m_bSwallowedTouch = false;
		m_TCForMouseUp.SetTargetTime(0.3f);
		m_pMouseMoveData = new sMouseMoveData();
		m_pObjectClickRespond = nullptr;
		m_bEnable = true;
		m_pTargetFrame = nullptr;
		m_MouseDownFunction = nullptr;
		m_MouseMoveFunction = nullptr;
		m_MouseUpFunction = nullptr;
		m_ClickedFunction = nullptr;
		m_MouseLeaveFunction = nullptr;
		m_DoubleClickedFunction = nullptr;
		m_pClickSound = nullptr;
		m_eObjectMouseBehavior = eOMB_NONE;
		if( e_bUseDefaultClickEffect )
			m_pObjectClickRespond = new cObjectClickRespond();	
	}

	cClickEvent::~cClickEvent()
	{
		SAFE_RELEASE(m_pClickSound,this);
		SAFE_DELETE(m_pObjectClickRespond);
		SAFE_DELETE(m_pMouseMoveData);
		SAFE_DELETE(m_pClickEventButtonRenderStatus);
	}

	cClickEvent*	cClickEvent::LazyCreate(cRenderObject*e_pRenderObject,bool e_bUseDefaultClickEffect)
	{
		cClickEventButtonRenderStatus*l_pClickEventButtonRenderStatus = new cClickEventButtonRenderStatus();
		l_pClickEventButtonRenderStatus->SetRenderObject(e_pRenderObject);
		cClickEvent*l_pClickEvent = new cClickEvent(l_pClickEventButtonRenderStatus,e_bUseDefaultClickEffect);
		return l_pClickEvent;
	}

	void	cClickEvent::Init()
	{
		m_TCForMouseUp.Start();
		m_eObjectMouseBehavior = eOMB_NONE;
		m_bEnable = true;
		m_pMouseMoveData->Init();
	}

	//bool	cClickEvent::MouseLeave(int e_iPosX,int e_iPosY)
	//{
	//	if( m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ||
	//		m_eObjectMouseBehavior == eOMB_HORVER)
	//	{
	//		m_eObjectMouseBehavior = eObjectMouseBehavior::eOMB_LEAVE;
	//		return true;
	//	}
	//}

	bool	cClickEvent::DoCollide(int e_iPosX,int e_iPosY)
	{
		if( m_CollideFunction )
			return m_CollideFunction(e_iPosX,e_iPosY);
		if( m_pTargetFrame == nullptr )
			return false;
		if(m_pTargetFrame->GetLocalBound()->Collide(e_iPosX,e_iPosY))
		{
			return true;
		}
		return false;
	}

    cClickEvent*    cClickEvent::MouseDown(int e_iPosX,int e_iPosY)
	{
		if( !m_bEnable )
			return false;
		if( m_eObjectMouseBehavior == eOMB_NONE)
		{
			if(DoCollide(e_iPosX,e_iPosY))
			{
				m_pMouseMoveData->MouseDown(e_iPosX,e_iPosY);
				m_eObjectMouseBehavior = eOMB_FIRST_TIME_INTO;
				if( m_MouseDownFunction )
					m_MouseDownFunction(m_pTargetFrame,e_iPosX,e_iPosY);
				return this;
			}
		}
		return nullptr;
	}

    cClickEvent*    cClickEvent::MouseMove(int e_iPosX,int e_iPosY)
	{
		if( !m_bEnable )
			return false;
		if( m_eObjectMouseBehavior == eObjectMouseBehavior::eOMB_HORVER||
			eObjectMouseBehavior::eOMB_FIRST_TIME_INTO)
		{
			m_pMouseMoveData->MouseMove(e_iPosX,e_iPosY);
			m_eObjectMouseBehavior = eOMB_HORVER;
			if( m_MouseMoveFunction )
				m_MouseMoveFunction(m_pTargetFrame,e_iPosX,e_iPosY);
			if( m_pObjectClickRespond )
			{
				if (DoCollide(e_iPosX,e_iPosY))
				{
					if( m_pObjectClickRespond->IsOnClick() == false )
						m_pObjectClickRespond->OnClick(m_pTargetFrame);
				}
				else
				{
					if( m_pObjectClickRespond->IsOnClick() == true )
						m_pObjectClickRespond->OnLeave(m_pTargetFrame);
				}
			}
			return this;
		}
		return nullptr;
	}

    cClickEvent*    cClickEvent::MouseUp(int e_iPosX,int e_iPosY)
	{
		if( m_pTargetFrame == nullptr || m_pTargetFrame->GetLocalBound() == nullptr || !m_bEnable)
			return false;
		if( m_eObjectMouseBehavior == eObjectMouseBehavior::eOMB_HORVER||
			eObjectMouseBehavior::eOMB_FIRST_TIME_INTO)
		{
			m_bEnable = false;
			m_pMouseMoveData->MouseUp(e_iPosX,e_iPosY);
			if(m_pTargetFrame->GetLocalBound()->Collide(e_iPosX,e_iPosY))
			{
				m_eObjectMouseBehavior = eOMB_UP;
				return this;
			}
			else
			{
				m_eObjectMouseBehavior = eOMB_LEAVE;
				if( m_pObjectClickRespond )
					m_pObjectClickRespond->OnLeave(m_pTargetFrame);
				if( m_MouseLeaveFunction != nullptr)
					m_MouseLeaveFunction(m_pTargetFrame,e_iPosX,e_iPosY);
			}
		}
		return nullptr;
	}

	void	cClickEvent::SetClickFunction(ClickFunction		e_MouseDownFunction,
											ClickFunction	e_MouseMoveFunction,
											ClickFunction	e_ClickedFunction,
											ClickFunction	e_MouseUpFunction,
											ClickFunction	e_MouseLeaveFunction,
											ClickFunction	e_DoubleClickedFunction)
	{
		m_MouseDownFunction = e_MouseDownFunction;
		m_MouseMoveFunction = e_MouseMoveFunction;
		m_ClickedFunction = e_ClickedFunction;
		m_MouseUpFunction = e_MouseUpFunction;
		m_MouseLeaveFunction = e_MouseLeaveFunction;
		m_DoubleClickedFunction	= e_DoubleClickedFunction;
	}

	void	cClickEvent::SetCollideFunction(CollideFunction	e_CollideFunction)
	{
		m_CollideFunction = e_CollideFunction;
	}

	cClickEventButtonRenderStatus*	cClickEvent::GetClickEventButtonRenderStatus()
	{
		return this->m_pClickEventButtonRenderStatus;
	}

	void		cClickEvent::SetClickEventButtonRenderStatus(cClickEventButtonRenderStatus*e_pClickEventButtonRenderStatus,bool e_bClone)
	{
		SAFE_DELETE(m_pClickEventButtonRenderStatus);
		if( e_bClone )
			m_pClickEventButtonRenderStatus = (cClickEventButtonRenderStatus*)e_pClickEventButtonRenderStatus->Clone();
		else
			m_pClickEventButtonRenderStatus = e_pClickEventButtonRenderStatus;
	}

	cBasicSound*	cClickEvent::GetClickSound()
	{
		return this->m_pClickSound;	
	}

	void	cClickEvent::SetClickSound(cBasicSound*e_pBasicSound)
	{
		SAFE_RELEASE(m_pClickSound,this);
		m_pClickSound = e_pBasicSound;
		m_pClickSound->AddRef(this);
	}

	void	cClickEvent::CopyClickFunctionFromTaget(cClickEvent*e_pClickEvent)
	{
		m_MouseDownFunction = e_pClickEvent->m_MouseDownFunction;
		m_MouseMoveFunction = e_pClickEvent->m_MouseMoveFunction;
		m_MouseUpFunction = e_pClickEvent->m_MouseUpFunction;
		m_MouseLeaveFunction = e_pClickEvent->m_MouseLeaveFunction;
		m_DoubleClickedFunction	= e_pClickEvent->m_DoubleClickedFunction;	
	}

	void    cClickEvent::Update(float e_fElpaseTime)
	{
		if(m_eObjectMouseBehavior == eOMB_UP)
		{
			m_TCForMouseUp.Update(e_fElpaseTime);
			if( m_TCForMouseUp.bTragetTimrReached )
			{
				//make a time offset
				if( m_MouseUpFunction != nullptr)
				{
					m_MouseUpFunction(m_pTargetFrame,m_pMouseMoveData->UpPos.x,m_pMouseMoveData->UpPos.y);
				}
				this->Init();
			}
		}
		if(this->m_pClickEventButtonRenderStatus)
		{
			this->m_pClickEventButtonRenderStatus->Update(e_fElpaseTime,m_eObjectMouseBehavior);
		}
	}

	void	cClickEvent::Render()
	{
		if(this->m_pClickEventButtonRenderStatus)
		{
			this->m_pClickEventButtonRenderStatus->Render(m_eObjectMouseBehavior);
		}
	}

	cClickEventGroup::cClickEventGroup()
	{

	}

	cClickEventGroup::~cClickEventGroup()
	{

	}


    cClickEvent*    cClickEventGroup::MouseDown(int e_iPosX,int e_iPosY)
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
				cClickEvent*l_pClickEvent = this->GetObject(l_iTargtIndexFromBackToFront);
				cClickEvent*l_pResult = l_pClickEvent->MouseDown(e_iPosX,e_iPosY);
				if( l_pResult && l_pResult->IsSwallowedTouch())
				{
					return l_pResult;
				}
			}
		}
		if(cClickEvent::MouseMove(e_iPosX,e_iPosY))
		{
			if( this->IsSwallowedTouch() )
			{
				return this;
			}
		}
		return nullptr;
	}

    cClickEvent*	cClickEventGroup::MouseMove(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return false;
		int	l_uiSize = Count();
		for( int i=0;i<l_uiSize;++i )
		{
			int	l_iTargtIndexFromBackToFront = l_uiSize-1-i;
			cClickEvent*l_pClickEvent = this->GetObject(l_iTargtIndexFromBackToFront);
			cClickEvent*l_pResult = l_pClickEvent->MouseMove(e_iPosX,e_iPosY);
			if( l_pResult && l_pResult->IsSwallowedTouch())
			{
				return l_pResult;
			}
		}
		if(cClickEvent::MouseMove(e_iPosX,e_iPosY))
		{
			if( this->IsSwallowedTouch() )
				return this;
		}
		return nullptr;
	}

    cClickEvent*	cClickEventGroup::MouseUp(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return false;
		int	l_uiSize = Count();
		for( int i=0;i<l_uiSize;++i )
		{
			int	l_iTargtIndexFromBackToFront = l_uiSize-1-i;
			cClickEvent*l_pClickEvent = this->GetObject(l_iTargtIndexFromBackToFront);
			cClickEvent*l_pResult = l_pClickEvent->MouseUp(e_iPosX,e_iPosY);
			if( l_pResult && l_pResult->IsSwallowedTouch())
			{
				return l_pResult;
			}
		}
		if(cClickEvent::MouseUp(e_iPosX,e_iPosY))
		{
			if( this->IsSwallowedTouch() )
				return this;
		}
		return nullptr;
	}


	void	cClickEventGroup::Init()
	{
		cClickEvent::Init();
		for(auto l_pData :this->m_ObjectList)
		{
			l_pData->Init();
		}
	}

    void    cClickEventGroup::Update(float e_fElpaseTime)
	{
		int	l_uiSize = Count();
		for( int i=0;i<l_uiSize;++i )
		{
			(*this)[i]->Update(e_fElpaseTime);
		}
	}

	void	cClickEventGroup::Render()
	{
		cClickEvent::Init();
		for(auto l_pData :this->m_ObjectList)
		{
			l_pData->Render();
		}	
	}

	cClickEventDispatcher::cClickEventDispatcher()
	{
		m_pCurrentWorkingEvent = 0;
	}

	cClickEventDispatcher::~cClickEventDispatcher()
	{
		
	}

	void    cClickEventDispatcher::Update(float e_fElpaseTime)
	{
		if(!this->IsEnable())
			return;
		if( m_pCurrentWorkingEvent )
			m_pCurrentWorkingEvent->Update(e_fElpaseTime);
		else
		{
			cClickEventGroup::Update(e_fElpaseTime);
		}
		m_AlwaysNeedToWorkClickEventGroup.Update(e_fElpaseTime);
	}

    cClickEvent*    cClickEventDispatcher::MouseDown(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return nullptr;
		m_pCurrentWorkingEvent = cClickEventGroup::MouseDown(e_iPosX,e_iPosY);
		m_AlwaysNeedToWorkClickEventGroup.MouseDown(e_iPosX,e_iPosY);
		return m_pCurrentWorkingEvent ;
	}

    cClickEvent*    cClickEventDispatcher::MouseMove(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return nullptr;
		if( m_pCurrentWorkingEvent )
			m_pCurrentWorkingEvent->MouseMove(e_iPosX,e_iPosY);
		m_AlwaysNeedToWorkClickEventGroup.MouseMove(e_iPosX,e_iPosY);
		return m_pCurrentWorkingEvent;
	}

    cClickEvent*    cClickEventDispatcher::MouseUp(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return nullptr;
		if( m_pCurrentWorkingEvent )
			m_pCurrentWorkingEvent->MouseUp(e_iPosX,e_iPosY);
		m_AlwaysNeedToWorkClickEventGroup.MouseUp(e_iPosX,e_iPosY);
		return m_pCurrentWorkingEvent;
	}

	void	cClickEventDispatcher::SetTopClickEvent(cClickEvent*e_pEvent)
	{
		m_pCurrentWorkingEvent = e_pEvent;
	}
}