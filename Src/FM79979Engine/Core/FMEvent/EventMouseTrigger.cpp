#include "../stdafx.h"
#include "EventMouseTrigger.h"
#include "EventPackageRunner.h"
#include "EventManager.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cEventMouseTrigger);
	TYPDE_DEFINE_MARCO(cEventObjectMouseUpDectet);

	cEventMouseTrigger::cEventMouseTrigger(TiXmlElement*e_pTiXmlElement)
	{
		m_fMoveDistance = 0.f;
		m_eGestureEnum = eGestureEnum::eGE_MAX;
		m_eCurrentGestureEnum = eGestureEnum::eGE_MAX;
		m_vCollisionRange = Vector4(0,0,cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		if( !e_pTiXmlElement )
			return;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventMouseTrigger::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME("Distance")
			{
				m_fMoveDistance = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("Gesture")
			{
				m_eGestureEnum = FATMING_CORE::GetGesture(l_strValue);
			}
			else
			COMPARE_NAME("CollideRange")
			{
				this->m_vCollisionRange = VALUE_TO_VECTOR4;
			}
		PARSE_NAME_VALUE_END
	}

	cEventMouseTrigger::cEventMouseTrigger(cEventMouseTrigger*e_pEventTest)
	{
		m_vCollisionRange = e_pEventTest->m_vCollisionRange;
		m_eGestureEnum = e_pEventTest->m_eGestureEnum;
		m_eCurrentGestureEnum = eGestureEnum::eGE_MAX;
		m_fMoveDistance = e_pEventTest->m_fMoveDistance;
	}

	cEventMouseTrigger::~cEventMouseTrigger()
	{
	
	}

	void	cEventMouseTrigger::InternalInit()
	{
		m_eCurrentGestureEnum = eGestureEnum::eGE_MAX;
	}

    void	cEventMouseTrigger::InternalMouseMove(int e_iPosX,int e_iPosY)
	{
		if(this->m_eObjectMouseBehavior == eOMB_HORVER)
			m_eCurrentGestureEnum = eGestureEnum::eGE_HORVER;
	}

    void	cEventMouseTrigger::InternalMouseDown(int e_iPosX,int e_iPosY)
	{
		m_eCurrentGestureEnum = eGestureEnum::eGE_CLICK_DOWN;
	}

    void    cEventMouseTrigger::InternalMouseUp(int e_iPosX,int e_iPosY)
	{
		if(this->m_eObjectMouseBehavior == eOMB_UP)
			m_eCurrentGestureEnum = eGestureEnum::eGE_CLICK_UP;
		if( m_fMoveDistance != 0.f )
		{
			Vector2	l_vDirection;
			float	l_fDis = this->m_MouseMoveData.DownUpDistance(&l_vDirection);
			if(l_fDis>this->m_fMoveDistance)
			{
				if( fabs(l_vDirection.x)>fabs(l_vDirection.y) )
				{
					if(l_vDirection.x>0.f)
						m_eCurrentGestureEnum = eGestureEnum::eGE_SLIDE_RIGHT;
					else
						m_eCurrentGestureEnum = eGestureEnum::eGE_SLIDE_LEFT;
				}
				else
				{
					if(l_vDirection.y>0.f)
						m_eCurrentGestureEnum = eGestureEnum::eGE_SLIDE_UP;
					else
						m_eCurrentGestureEnum = eGestureEnum::eGE_SLIDE_DOWN;
				}
			}
		}
		cClickMouseBehavior::Init();
	}

    void    cEventMouseTrigger::InternalMouseDoubleClickUp(int e_iPosX,int e_iPosY)
	{
		m_eCurrentGestureEnum = eGestureEnum::eGE_DOUBLE_CLICK;
	}

	void	cEventMouseTrigger::InternalUpdate(float e_fElpasedTime)
	{
		if(m_eCurrentGestureEnum == m_eGestureEnum)
		{
			this->m_bSatisfiedCondition = true;
		}
	}

	void	cEventMouseTrigger::InternalRender()
	{
		//if( this->GetPos() )
		{
			//Vector3	l_vPos = *this->GetPos();
			//this->m_vCollisionRange;
		}
		//else
		{
			//GLRender::RenderRectangle(Vector2(m_vCollisionRange.x,m_vCollisionRange.y),m_vCollisionRange.Width(),m_vCollisionRange.Height(),Vector4::One);
		}
	}

	TiXmlElement*		cEventMouseTrigger::ToTiXmlElement()
	{
		TiXmlElement*l_pTiXmlElement = cEventBase::ToTiXmlElement();
		l_pTiXmlElement->SetAttribute(L"Gesture",ValueToStringW(this->m_eCurrentGestureEnum));
		l_pTiXmlElement->SetAttribute(L"Distance",ValueToStringW(this->m_fMoveDistance));
		return l_pTiXmlElement;
	}

	cEventObjectMouseUpDectet::cEventObjectMouseUpDectet()
	{
		m_pParentEventObjectInstance = 0;
		m_bWaitForPlayDone = false;
		m_bEventObjectInstanceMouseUp = false;
	}

	cEventObjectMouseUpDectet::~cEventObjectMouseUpDectet()
	{
	
	}

	void		cEventObjectMouseUpDectet::InternalInit()
	{
		m_bEventObjectInstanceMouseUp = false;
	}

	void		cEventObjectMouseUpDectet::InternalUpdate(float e_fElpasedTime)
	{
		if(!m_pParentEventObjectInstance)
		{			
			m_pParentEventObjectInstance = g_pCurrentEventManager->m_pEventPackageRunner->GetCurrentEventPackage()->GetObject(m_strEventObjectInstanceName.c_str());
		}
		if( !m_pParentEventObjectInstance )
			return;
		if(m_pParentEventObjectInstance->GetCurrentWorkingObject())
		{
			if(!m_bEventObjectInstanceMouseUp)
			{
				if(m_pParentEventObjectInstance->GetCurrentWorkingObject()->GetObject()->IsMouseUp())
				{
					m_bEventObjectInstanceMouseUp = true;
					if( !m_bWaitForPlayDone )
						this->m_bSatisfiedCondition = true;
				}
			}
			else
			{
				if(m_pParentEventObjectInstance->GetCurrentWorkingObject()->GetObject()->IsPlayDone())
				{
					this->m_bSatisfiedCondition = true;
				}
			}
		}
	}

	TiXmlElement*		cEventObjectMouseUpDectet::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		//l_pEvent->SetAttribute(NAME,this->GetName());
		//l_pEvent->SetAttribute(BUTTON_SELECTION_TYPE,ValueToString(this->m_eButtonSelectionType));
		//l_pEvent->SetAttribute(L"StayAtLastSelect",this->m_bStayAtLastSelect?L"1":L"0");
		//if(this->m_iClickCount != 0 )
		//{
		//	l_pEvent->SetAttribute(L"ClickCount",this->m_iClickCount);
		//}
		//if(m_pPrefixButton)
		//{
		//	l_pEvent->SetAttribute(L"PrefixButtonName",m_pPrefixButton->GetName());
		//	cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(m_pPrefixButton);
		//	l_pEvent->LinkEndChild(l_pEventButton->ToTiXmlElement());
		//}
		//int	l_iCount = this->Count();
		//for( int i=0;i<l_iCount;++i )
		//{
		//	cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(this->m_ObjectList[i]);
		//	l_pEvent->LinkEndChild(l_pEventButton->ToTiXmlElement());
		//}
		//return l_pEvent;
		return 0;
	}
//end namespace FATMING_CORE
}