#include "../../stdafx.h"
#include "../../Sound/BasicSound.h"
#include "DefaultRenderClickBehavior.h"
#define MOUSE_DOWN_SCALE_VALUE	1.2f
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cDefaultRenderClickBehavior);
	cDefaultRenderClickBehavior::cDefaultRenderClickBehavior()
	{
		m_bIsOnClick = false;
		m_pRenderObject = nullptr;
	}
	cDefaultRenderClickBehavior::~cDefaultRenderClickBehavior()
	{
		
	}

	void	cDefaultRenderClickBehavior::SetRenderObject(cRenderObject*e_pRenderObject)
	{
		m_pRenderObject = e_pRenderObject;
	}
    cClickBehavior*    cDefaultRenderClickBehavior::MouseDown(int e_iPosX,int e_iPosY)
	{
		auto l_Result = cClickBehavior::MouseDown(e_iPosX,e_iPosY);
		if( l_Result )
		{
			if( m_pRenderObject == nullptr )
				return l_Result;
			m_bIsOnClick = true;
			m_OnClickMatrix = m_pRenderObject->GetLocalTransform();
			m_OnHorverMatrix = m_OnClickMatrix*cMatrix44::ScaleMatrix(Vector3(MOUSE_DOWN_SCALE_VALUE,MOUSE_DOWN_SCALE_VALUE,MOUSE_DOWN_SCALE_VALUE));
			if( m_pRenderObject )
			{
				m_pRenderObject->SetLocalTransform(m_OnHorverMatrix);
				m_pRenderObject->SetAutoUpdateBound(false);
			}		
		}
		return l_Result;
	}

    cClickBehavior*    cDefaultRenderClickBehavior::MouseMove(int e_iPosX,int e_iPosY)
	{
		auto l_Result = cClickBehavior::MouseMove(e_iPosX,e_iPosY);
		if( l_Result )
		{
			eObjectMouseBehavior l_ObjectMouseBehavior = m_eObjectMouseBehavior;
			if( l_ObjectMouseBehavior == eOMB_HORVER )
			{
				if( !m_bIsOnClick )
				{
					m_bIsOnClick = true;
					if( m_pRenderObject  )
					{
						m_pRenderObject->SetLocalTransform(m_OnHorverMatrix);
					}
				}			
			}
			else
			if( l_ObjectMouseBehavior == eOMB_LEAVE)
			{
				MouseLeave();
			}
		}
		return l_Result;
	}


	void	cDefaultRenderClickBehavior::MouseLeave()
	{
		if( m_pRenderObject == nullptr )
			return;
		if( m_bIsOnClick )
		{
			m_bIsOnClick = false;
			if( m_pRenderObject )
			{
				m_pRenderObject->SetLocalTransform(m_OnClickMatrix);
				m_pRenderObject->SetAutoUpdateBound(true);
			}
		}
	}


	bool	cDefaultRenderClickBehavior::Collide(int e_iPosX,int e_iPosY)
	{
		if( m_bAllowDrag && m_bCollided )
			return true;
		m_bCollided = false;
		if(this->m_pRenderObject)
		{
			auto l_pBond = m_pRenderObject->GetWorldBound();
			if( l_pBond )
			{
				m_bCollided = l_pBond->Collide(e_iPosX,e_iPosY);
				return m_bCollided;
			}
		}
		return cClickBehavior::Collide(e_iPosX,e_iPosY);
	}

	void	cDefaultRenderClickBehavior::Update(float e_fElpaseTime)
	{
		auto l_eObjectMouseBehavior = m_eObjectMouseBehavior;
		cClickBehavior::Update(e_fElpaseTime);
		if(l_eObjectMouseBehavior == eOMB_UP || l_eObjectMouseBehavior == eOMB_DOUBLU_CLICK_UP)
		{
			if(m_eObjectMouseBehavior == eOMB_NONE)
			{
				MouseLeave();
			}
		}
	}
//end namespace FATMING_CORE
}