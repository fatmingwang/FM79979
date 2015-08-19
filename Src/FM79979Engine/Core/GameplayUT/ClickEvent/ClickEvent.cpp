#include "../../Stdafx.h"
#include "../Mouse_Touch.h"
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

	cClickEvent::cClickEvent(bool e_bUseDefaultClickEffect)
	{
		m_bAllowParallelClick = false;
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
		m_eObjectMouseBehavior = eOMB_NONE;
		if( e_bUseDefaultClickEffect )
			m_pObjectClickRespond = new cObjectClickRespond();
	}

	cClickEvent::~cClickEvent()
	{
		SAFE_DELETE(m_pObjectClickRespond);
		SAFE_DELETE(m_pMouseMoveData);
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

    bool    cClickEvent::MouseDown(int e_iPosX,int e_iPosY)
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
				return true;
			}
		}
		return false;
	}

    bool    cClickEvent::MouseMove(int e_iPosX,int e_iPosY)
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
			return true;
		}
		return false;
	}

    bool    cClickEvent::MouseUp(int e_iPosX,int e_iPosY)
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
				return true;
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
		return false;
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
				m_eObjectMouseBehavior = eOMB_NONE;
				m_bEnable = true;
			}
		}
	}

	cClickEventGroup::cClickEventGroup()
	{
		m_pCurrentWorkingEvent = 0;
	}

	cClickEventGroup::~cClickEventGroup()
	{

	}

    bool    cClickEventGroup::MouseDown(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return false;
		int	l_uiSize = Count();
		if( l_uiSize == 0 )
			return false;
		for( int i=0;i<l_uiSize;++i )
		{
			//because we will try to hit from last to first
			int	l_iTargtIndexFromBackToFront = l_uiSize-1-i;
			cClickEvent*l_pClickEvent = (*this)[l_iTargtIndexFromBackToFront];
			if( !m_pCurrentWorkingEvent || l_pClickEvent->IsAllowParallelClick() )
			{
				if(l_pClickEvent->MouseDown(e_iPosX,e_iPosY))
				{
					if( !l_pClickEvent->IsAllowParallelClick() )
						m_pCurrentWorkingEvent = l_pClickEvent;
				}
			}
		}
		if( m_pCurrentWorkingEvent )
			return true;
		return false;
	}

    bool    cClickEventGroup::MouseMove(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return false;
		int	l_uiSize = Count();
		if( l_uiSize == 0 )
			return false;
		bool	l_bEventFired = false;
		if( m_pCurrentWorkingEvent )
		{
			m_pCurrentWorkingEvent->MouseMove(e_iPosX,e_iPosY);
			l_bEventFired = true;
		}
		for( int i=0;i<l_uiSize;++i )
		{
			int	l_iTargtIndexFromBackToFront = l_uiSize-1-i;
			cClickEvent*l_pClickEvent = (*this)[l_iTargtIndexFromBackToFront];
			if( l_pClickEvent->IsAllowParallelClick() && m_pCurrentWorkingEvent != l_pClickEvent )
			{
				l_pClickEvent->MouseMove(e_iPosX,e_iPosY);
				l_bEventFired = true;
			}
		}
		return false;
	}

    bool    cClickEventGroup::MouseUp(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return false;
		int	l_uiSize = Count();
		if( l_uiSize == 0 )
			return false;
		bool	l_bEventFired = false;
		if( m_pCurrentWorkingEvent )
		{
			m_pCurrentWorkingEvent->MouseUp(e_iPosX,e_iPosY);
			l_bEventFired = true;
		}
		for( int i=0;i<l_uiSize;++i )
		{
			int	l_iTargtIndexFromBackToFront = l_uiSize-1-i;
			cClickEvent*l_pClickEvent = (*this)[l_iTargtIndexFromBackToFront];
			if( l_pClickEvent->IsAllowParallelClick() && m_pCurrentWorkingEvent != l_pClickEvent )
			{
				l_pClickEvent->MouseUp(e_iPosX,e_iPosY);
				l_bEventFired = true;
			}
		}
		return l_bEventFired;
	}


	void	cClickEventGroup::Init()
	{
		m_pCurrentWorkingEvent	= 0;
	}

    void    cClickEventGroup::Update(float e_fElpaseTime)
	{
		int	l_uiSize = Count();
		for( int i=0;i<l_uiSize;++i )
		{
			(*this)[i]->Update(e_fElpaseTime);
		}
	}

	cClickEventDispatcher::cClickEventDispatcher()
	{
		m_pTopMenu = 0;
	}

	cClickEventDispatcher::~cClickEventDispatcher()
	{
		
	}

    bool    cClickEventDispatcher::MouseDown(int e_iPosX,int e_iPosY)
	{
		if( m_pTopMenu )
		{
			return m_pTopMenu->MouseDown(e_iPosX,e_iPosY);
		}
		else
		{
			return cClickEventGroup::MouseDown(e_iPosX,e_iPosY);
		}
		return false;
	}

    bool    cClickEventDispatcher::MouseMove(int e_iPosX,int e_iPosY)
	{
		if( m_pTopMenu )
		{
			return m_pTopMenu->MouseMove(e_iPosX,e_iPosY);
		}
		else
		{
			return cClickEventGroup::MouseMove(e_iPosX,e_iPosY);
		}
		return false;
	}

    bool    cClickEventDispatcher::MouseUp(int e_iPosX,int e_iPosY)
	{
		if( m_pTopMenu )
		{
			return m_pTopMenu->MouseUp(e_iPosX,e_iPosY);
		}
		else
		{
			return cClickEventGroup::MouseUp(e_iPosX,e_iPosY);
		}
		return false;
	}

	void	cClickEventDispatcher::SetTopClickEvent(cClickEvent*e_pEvent)
	{
		m_pTopMenu = e_pEvent;
	}
}