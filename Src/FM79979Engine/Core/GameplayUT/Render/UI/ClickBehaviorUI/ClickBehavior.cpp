#include "ClickBehavior.h"
#include "../../../../Sound/SoundManager.h"
#include "DefaultRenderClickBehavior.h"
#include "../../MPDI/MPDI.h"
//fuck I am lazy to fix this.
#define MOUSE_UP_IDLE_TIME	0.016f

namespace FATMING_CORE
{

	TYPDE_DEFINE_MARCO(cClickBehavior);
	TYPDE_DEFINE_MARCO(cClickBehaviorGroup);
	TYPDE_DEFINE_MARCO(cClickBehaviorDispatcher);

	#define LAZY_MOUSE_FUNCTION(Function,PosX,PosY) if(Function)Function(PosX,PosY,this)

	cClickBehavior::cClickBehavior()
	{
		m_bCollided = false;
		m_bSwallowedTouch = true;
		m_TCForMouseUp.SetTargetTime(MOUSE_UP_IDLE_TIME);
		m_bEnable = true;
		m_eObjectMouseBehavior = eOMB_NONE;
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
		m_MouseMoveData.Init();
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
		if (!this->m_bEnable)
			return false;
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
			return nullptr;
		//because after mouse up update need MOUSE_UP_IDLE_TIME,the status still struck at mouse up
		//it occur bug,so do not check status here.
		//if( m_eObjectMouseBehavior == eOMB_NONE)
		{
			if(Collide(e_iPosX,e_iPosY))
			{
				//FMLog::Log(UT::ComposeMsgByFormat(L"cClickBehavior::MouseDown---%ls---:%ls",GetName(),Type()).c_str(), false);
				m_MouseMoveData.MouseDown(e_iPosX,e_iPosY);
				m_eObjectMouseBehavior = eOMB_FIRST_TIME_INTO;
				LAZY_MOUSE_FUNCTION(m_MouseDownFunction,e_iPosX,e_iPosY);
				return this;
			}
		}
		eObjectMouseBehavior l_eObjectMouseBehavior = m_eObjectMouseBehavior;
		m_eObjectMouseBehavior = eOMB_NONE;
		return nullptr;
	}

    cClickBehavior*    cClickBehavior::MouseMove(int e_iPosX,int e_iPosY)
	{
		if( !m_bEnable )
			return nullptr;
		if( m_eObjectMouseBehavior == eOMB_HORVER||
			m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ||
			m_eObjectMouseBehavior == eOMB_LEAVE)
		{
			m_MouseMoveData.MouseMove(e_iPosX,e_iPosY);
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
			return nullptr;
		if(this->Collide(e_iPosX,e_iPosY))
		{
			//m_bEnable = false;
			if (this->m_bAllowDrag)
			{
				m_MouseMoveData.MouseUp(e_iPosX, e_iPosY);
				if (m_MouseMoveData.IsDownUpDistanceSamePoint())
				{
					m_eObjectMouseBehavior = eOMB_UP;
					return this;
				}
				this->Init();
				m_bEnable = true;
			}
			else
			{
				bool l_bDoubleClick = m_MouseMoveData.MouseUp(e_iPosX, e_iPosY);
				if (!l_bDoubleClick)
					m_eObjectMouseBehavior = eOMB_UP;
				else
					m_eObjectMouseBehavior = eOMB_DOUBLU_CLICK_UP;
				return this;
			}
		}
		else
		{
			this->Init();
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
				m_bEnable = true;
				//make a time offset
				if(m_eObjectMouseBehavior == eOMB_DOUBLU_CLICK_UP)
				{
					LAZY_MOUSE_FUNCTION(m_MouseDoubleClickFunction,this->m_MouseMoveData.UpPos.x,this->m_MouseMoveData.UpPos.y);
				}
				else
				{
					LAZY_MOUSE_FUNCTION(m_MouseUpFunction,this->m_MouseMoveData.UpPos.x,this->m_MouseMoveData.UpPos.y);
				}
				if( m_pClickSound )
					m_pClickSound->Play(true);
				this->Init();
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

	void cClickBehavior::SetMouseUpFunction(ClickFunction e_MouseUpFunction)
	{
		m_MouseUpFunction = e_MouseUpFunction;
	}

	void	cClickBehavior::CreateFullScreenCollide()
	{
		this->m_CollideFunction = FullscreenCollide;
	}

	cClickBehavior* cClickBehaviorGroup::ChildrenMouseDown(int e_iPosX, int e_iPosY)
	{
		int	l_uiSize = Count();
		if (l_uiSize != 0)
		{
			for (int i = 0; i < l_uiSize; ++i)
			{
				//because we will try to hit from last to first
				int	l_iTargtIndexFromBackToFront = l_uiSize - 1 - i;
				cClickBehavior* l_pClickEvent = this->GetObject(l_iTargtIndexFromBackToFront);
				cClickBehavior* l_pResult = l_pClickEvent->MouseDown(e_iPosX, e_iPosY);
				if (l_pResult && l_pResult->IsSwallowedTouch())
				{
					//FMLog::Log(UT::ComposeMsgByFormat(L"cClickBehaviorGroup::MouseDown---Child:%d %ls---%ls", i, l_pResult->GetName(), l_pResult->Type()).c_str(), false);
					return l_pResult;
				}
			}
		}
		return nullptr;
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
			return nullptr;		
		auto l_pObject = ChildrenMouseDown(e_iPosX, e_iPosY);
		if (l_pObject)
			return l_pObject;
		if(cClickBehavior::MouseDown(e_iPosX,e_iPosY))
		{
			if( this->IsSwallowedTouch() )
			{
				FMLog::Log(UT::ComposeMsgByFormat(L"cClickBehaviorGroup::MouseDown---%ls---%ls",GetName(), Type()).c_str(), false);
				return this;
			}
		}
		return nullptr;
	}

    cClickBehavior*	cClickBehaviorGroup::MouseMove(int e_iPosX,int e_iPosY)
	{
		if(!this->IsEnable())
			return nullptr;
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
			return nullptr;
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
		cClickBehavior::Update(e_fElpaseTime);
		if (m_eObjectMouseBehavior == eOMB_NONE)
		{
			int	l_uiSize = Count();
			for (int i = 0; i < l_uiSize; ++i)
			{
				(*this)[i]->Update(e_fElpaseTime);
			}
		}
	}

	cClickBehavior*	cClickBehaviorGroup::AddDefaultRenderClickBehaviorButton(cRenderObject*e_pRenderObject,ClickFunction e_ClickFunction,cBasicSound*e_pBasicSound, bool e_bEnableClickScale)
	{
		if( !e_pRenderObject )
			return nullptr;
		//add this one?
		e_pRenderObject->Init();
		cDefaultRenderClickBehavior*l_pClickEvent = new cDefaultRenderClickBehavior();
		l_pClickEvent->SetRenderObject(e_pRenderObject);
		if(!e_pRenderObject->GenerateBound())
		{
			std::wstring l_strDebugInfo = e_pRenderObject->GetName();
			l_strDebugInfo += L" generate bound faild,the object dont override the GenerateBound";
			FMLog::LogWithFlag(l_strDebugInfo.c_str(), CORE_LOG_FLAG);
		}
		l_pClickEvent->SetMouseFunction(nullptr,nullptr,nullptr,e_ClickFunction,e_ClickFunction,nullptr);
		l_pClickEvent->SetName(e_pRenderObject->GetName());
		l_pClickEvent->SetClickSound(e_pBasicSound);
		l_pClickEvent->SetEnableClickScale(e_bEnableClickScale);
		this->AddObjectNeglectExist(l_pClickEvent);
		return l_pClickEvent;
	}

	std::tuple<cClickBehavior*,cRenderObject*>		cClickBehaviorGroup::AddDefaultRenderClickBehaviorButton(cCueToStartCurveWithTime*e_pSubMPDI,ClickFunction e_ClickFunction,cBasicSound*e_pBasicSound, bool e_bEnableClickScale)
	{
		//auto l_pImage = e_pSubMPDI->PointDataToBaseImage(0);
		cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(e_pSubMPDI->GetParent());
		if (l_pMPDI)
		{
			l_pMPDI->RemoveObjectWithoutDelete(e_pSubMPDI);
			e_pSubMPDI->SetLocalTransform(l_pMPDI->GetWorldTransform());
		}
		else
		{
			FMLog::Log("Error!!you have called cMPDI::Init before call cClickBehaviorGroup::AddDefaultRenderClickBehaviorButtonfunction!?",true);
		}
		e_pSubMPDI->Init();
		e_pSubMPDI->Update(0.00001f);
		auto l_pData = AddDefaultRenderClickBehaviorButton((cRenderObject*)e_pSubMPDI,e_ClickFunction,e_pBasicSound, e_bEnableClickScale);
		return std::make_tuple(l_pData,e_pSubMPDI);
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
		if(!this->IsEnable() || m_pCurrentWorkingEvent)
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
	
	cLazyClickBehaviorAndRenderObject::cLazyClickBehaviorAndRenderObject()
	{
		m_pClickBehaviorGroup = new cClickBehaviorGroup();
		m_pRenderObject = new cRenderObject();
		m_pRenderObject->SetName(L"RenderObject");
		m_pClickBehaviorGroup->SetName(L"Click");
	}

	cLazyClickBehaviorAndRenderObject::cLazyClickBehaviorAndRenderObject(cClickBehaviorGroup*e_pClickBehaviorGroup,cRenderObject*e_pRenderObject)
	{
		m_pClickBehaviorGroup = e_pClickBehaviorGroup;
		m_pRenderObject = e_pRenderObject;
	}
	cLazyClickBehaviorAndRenderObject::~cLazyClickBehaviorAndRenderObject(){}
	void	cLazyClickBehaviorAndRenderObject::AddChild(cRenderObject*e_pRenderObject)
	{
		if( e_pRenderObject->GetLocalBound() )
		{
#ifdef DEBUG
			//RECT l_rect = e_pRenderObject->GetLocalBound()->GetRect();
			//int i=0;
#endif
		}
		m_pRenderObject->AddChildToLast(e_pRenderObject);
		
	}
	void	cLazyClickBehaviorAndRenderObject::AddObject(cClickBehavior*e_pClickBehavior)
	{
		m_pClickBehaviorGroup->AddObject(e_pClickBehavior);
	}
	void	cLazyClickBehaviorAndRenderObject::SetEnable(bool e_bEnable)
	{
		m_pClickBehaviorGroup->SetEnable(e_bEnable);
		m_pRenderObject->SetVisible(e_bEnable);
	}
	bool	cLazyClickBehaviorAndRenderObject::IsEnable()
	{
		return m_pRenderObject->IsVisible();
	}

	void	cLazyClickBehaviorAndRenderObject::Init()
	{
		m_pRenderObject->InitNodes();
		this->m_pClickBehaviorGroup->Init();
	}

	void	cLazyClickBehaviorAndRenderObject::SetNameToAll(const wchar_t*e_strName)
	{
		this->SetName(e_strName);
		std::wstring l_strName = e_strName;
		l_strName += L"RenderObject";
		m_pRenderObject->SetName(l_strName);
		l_strName = e_strName;
		l_strName += L"Click";
		m_pClickBehaviorGroup->SetName(e_strName);
	}
}