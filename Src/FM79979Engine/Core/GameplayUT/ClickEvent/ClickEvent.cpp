#include "../../Stdafx.h"
#include "../Mouse_Touch.h"
#include "../../Sound/BasicSound.h"
#include "ClickEvent.h"
namespace FATMING_CORE
{

	TYPDE_DEFINE_MARCO(cObjectClickRespond);
	TYPDE_DEFINE_MARCO(cClickEvent);
	TYPDE_DEFINE_MARCO(cClickEventGroup);
	TYPDE_DEFINE_MARCO(cClickEventDispatcher);



	cObjectClickRespond::cObjectClickRespond()
	{
		m_bIsOnClick = false;
		m_pRenderObject = nullptr;
		m_pClickFunction = nullptr;
		m_pClickSound = nullptr;
		m_pClickEventParent = nullptr;
	}
	cObjectClickRespond::~cObjectClickRespond()
	{
		SAFE_RELEASE(m_pClickSound,this);
	}

	cBasicSound*	cObjectClickRespond::GetClickSound()
	{
		return this->m_pClickSound;	
	}

	void	cObjectClickRespond::SetClickSound(cBasicSound*e_pBasicSound)
	{
		SAFE_RELEASE(m_pClickSound,this);
		m_pClickSound = e_pBasicSound;
		m_pClickSound->AddRef(this);
	}

	void	cObjectClickRespond::OnClick()
	{
		if( m_pRenderObject == nullptr )
			return;
		m_bIsOnClick = true;
		m_OnClickMatrix = m_pRenderObject->GetLocalTransform();
		m_OnHorverMatrix = m_OnClickMatrix*cMatrix44::ZAxisRotationMatrix(0.1f);
		if( m_pRenderObject )
			m_pRenderObject->SetLocalTransform(m_OnHorverMatrix);
	}

	void	cObjectClickRespond::OnDoubleClick()
	{
		OnLeave();
		if(m_pClickFunction)
			m_pClickFunction(this);
		
	}

	void	cObjectClickRespond::OnMouseUp()
	{
		OnLeave();
		if(m_pClickFunction)
			m_pClickFunction(this);
		if( m_pClickSound )
			m_pClickSound->Play(true);
	}

	void	cObjectClickRespond::OnLeave()
	{
		if( m_pRenderObject == nullptr )
			return;
		if( m_bIsOnClick )
		{
			m_bIsOnClick = false;
			if( m_pRenderObject )
				m_pRenderObject->SetLocalTransform(m_OnClickMatrix);
		}
	}

	void	cObjectClickRespond::OnHorver()
	{
		if( !m_bIsOnClick )
		{
			m_bIsOnClick = true;
			if( m_pRenderObject  )
				m_pRenderObject->SetLocalTransform(m_OnHorverMatrix);
		}
	}

	void	cObjectClickRespond::Update(float e_fElpaseTime,eObjectMouseBehavior e_eObjectMouseBehavior)
	{

	}
	bool	cObjectClickRespond::Collide(int e_iPosX,int e_iPosY)
	{
		if(this->m_pRenderObject)
		{
			auto l_pBond = m_pRenderObject->GetWorldBound();
			if( l_pBond )
				return l_pBond->Collide(e_iPosX,e_iPosY);
		}
		return false;
	}

	cClickEvent::cClickEvent(bool e_bUseDefaultClickEffect)
	{
		m_bSwallowedTouch = true;
		m_TCForMouseUp.SetTargetTime(0.3f);
		m_pMouseMoveData = new sMouseMoveData();
		m_pObjectClickRespond = nullptr;
		m_bEnable = true;
		m_eObjectMouseBehavior = eOMB_NONE;
		if( e_bUseDefaultClickEffect )
		{
			m_pObjectClickRespond = new cObjectClickRespond();
		}
	}

	cClickEvent::cClickEvent(cObjectClickRespond*e_pObjectClickRespond)
	{
		m_bSwallowedTouch = true;
		m_TCForMouseUp.SetTargetTime(0.3f);
		m_bEnable = true;
		m_eObjectMouseBehavior = eOMB_NONE;
		m_pObjectClickRespond = e_pObjectClickRespond;
		m_pMouseMoveData = new sMouseMoveData();
	}

	cClickEvent::~cClickEvent()
	{
		SAFE_DELETE(m_pObjectClickRespond);
		SAFE_DELETE(m_pMouseMoveData);
	}

	//cClickEvent*	cClickEvent::LazyCreate(cRenderObject*e_pRenderObject,bool e_bUseDefaultClickEffect)
	//{
	//	cClickEventButtonRenderStatus*l_pClickEventButtonRenderStatus = new cClickEventButtonRenderStatus();
	//	l_pClickEventButtonRenderStatus->SetRenderObject(e_pRenderObject);
	//	cClickEvent*l_pClickEvent = new cClickEvent(l_pClickEventButtonRenderStatus,e_bUseDefaultClickEffect);
	//	return l_pClickEvent;
	//}

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
		m_bCollided = false;
		if(m_pObjectClickRespond && this->m_pObjectClickRespond->Collide(e_iPosX,e_iPosY))
		{
			m_bCollided = true;;
		}
		return m_bCollided;
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
				if( m_pObjectClickRespond )
					m_pObjectClickRespond->OnClick();
				return this;
			}
		}
		m_eObjectMouseBehavior = eOMB_NONE;
		return nullptr;
	}

    cClickEvent*    cClickEvent::MouseMove(int e_iPosX,int e_iPosY)
	{
		if( !m_bEnable )
			return false;
		if( m_eObjectMouseBehavior == eOMB_HORVER||
			m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ||
			m_eObjectMouseBehavior == eOMB_LEAVE)
		{
			m_pMouseMoveData->MouseMove(e_iPosX,e_iPosY);
			if(DoCollide(e_iPosX,e_iPosY))
			{
				m_eObjectMouseBehavior = eOMB_HORVER;
				if( m_pObjectClickRespond )
					m_pObjectClickRespond->OnHorver();
			}
			else
			{
				m_eObjectMouseBehavior = eOMB_LEAVE;
				if( m_pObjectClickRespond )
					m_pObjectClickRespond->OnLeave();
			}
			return this;
		}
		return nullptr;
	}

    cClickEvent*    cClickEvent::MouseUp(int e_iPosX,int e_iPosY)
	{
		if( !m_bEnable)
			return false;
		if(this->DoCollide(e_iPosX,e_iPosY))
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
			if( m_pObjectClickRespond  )
				m_pObjectClickRespond->OnLeave();

		}
		return nullptr;
	}

	void    cClickEvent::Update(float e_fElpaseTime)
	{
		if(m_eObjectMouseBehavior == eOMB_UP || m_eObjectMouseBehavior == eOMB_DOUBLU_CLICK_UP)
		{
			m_TCForMouseUp.Update(e_fElpaseTime);
			if( m_TCForMouseUp.bTragetTimrReached )
			{
				//make a time offset
				if(m_eObjectMouseBehavior == eOMB_DOUBLU_CLICK_UP)
				{
					if( m_pObjectClickRespond )
						m_pObjectClickRespond->OnDoubleClick();
				}
				else
				{
					if( m_pObjectClickRespond )
						m_pObjectClickRespond->OnMouseUp();
				}
				this->Init();
			}
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

	bool	cClickEventGroup::AddButton(cObjectClickRespond*e_pObjectClickRespond)
	{
		if( !e_pObjectClickRespond )
			return false;
		cClickEvent*l_pClickEvent = new cClickEvent(e_pObjectClickRespond);
		l_pClickEvent->SetName(e_pObjectClickRespond->GetName());
		this->AddObject(l_pClickEvent);
		return true;
	}

	bool	cClickEventGroup::AddButton(cRenderObject*e_pRenderObject,ClickFunction e_ClickFunction,cBasicSound*e_pClickSound)
	{
		cObjectClickRespond*l_pObjectClickRespond = new cObjectClickRespond();
		l_pObjectClickRespond->SetRenderObject(e_pRenderObject);
		l_pObjectClickRespond->SetName(e_pRenderObject->GetName());
		l_pObjectClickRespond->SetClickSound(e_pClickSound);
		cClickEvent*l_pClickEvent = new cClickEvent(l_pObjectClickRespond);
		l_pClickEvent->SetName(e_pRenderObject->GetName());
		this->AddObject(l_pClickEvent);
		return true;
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
		{
			m_pCurrentWorkingEvent->Update(e_fElpaseTime);
			if( m_pCurrentWorkingEvent->m_eObjectMouseBehavior == eObjectMouseBehavior::eOMB_NONE )
				m_pCurrentWorkingEvent = nullptr;
		}
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