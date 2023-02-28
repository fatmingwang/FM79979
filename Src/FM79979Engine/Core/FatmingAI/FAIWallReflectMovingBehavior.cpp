#include "stdafx.h"
#include "FAIWallReflectMovingBehavior.h"
#include "FAIMachine.h"
namespace FATMING_AI
{
	//<cFAIWallReflectMovingBehavior Name="1" Probability="10" NextAIName="Waddle" SpeedRestrict="1,2" WallHitRetrict="5,10" />
	const wchar_t*cFAIWallReflectMovingBehavior::TypeID( L"cFAIWallReflectMovingBehavior" );
	cFAIWallReflectMovingBehavior::cFAIWallReflectMovingBehavior(TiXmlElement*e_pTiXmlElement)
	{
		m_pHittWallCallback = 0;
		m_fSpeedScale = 1.f;
		m_fSpeedRestrict.Min = m_fSpeedRestrict.Max = 1.f;
		m_vWall = Vector4(0,0,cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		m_CurrentWallHitTC.SetTargetTime(10.f);
		AI_PARSE_NAME_VALUE_START
			COMPARE_NAME("Wall")
			{
				m_vWall = VALUE_TO_VECTOR4;
				m_vWall.z *= cGameApp::m_spOpenGLRender->m_vGameScale.x;
				m_vWall.w *= cGameApp::m_spOpenGLRender->m_vGameScale.y;
			}
			else
			COMPARE_NAME("SpeedRestrict")
			{
				m_fSpeedRestrict = VALUE_TO_VECTOR2;
			}
			else
			COMPARE_NAME("WallHitRetrict")
			{
				m_WallHitRetrict = VALUE_TO_VECTOR2;
			}
		PARSE_NAME_VALUE_END
	}

    cFAIWallReflectMovingBehavior::cFAIWallReflectMovingBehavior(cFAIWallReflectMovingBehavior*e_pFAIWallReflectMovingBehavior):cFAIBaseBehave(e_pFAIWallReflectMovingBehavior)
	{
		m_CurrentWallHitTC.SetTargetTime(e_pFAIWallReflectMovingBehavior->m_CurrentWallHitTC.fTargetTime);
		m_WallHitRetrict = e_pFAIWallReflectMovingBehavior->m_WallHitRetrict;
		m_pHittWallCallback = e_pFAIWallReflectMovingBehavior->m_pHittWallCallback;
		m_fSpeedScale = 1.f;
		m_vWall = e_pFAIWallReflectMovingBehavior->m_vWall;
		m_fSpeedRestrict = e_pFAIWallReflectMovingBehavior->m_fSpeedRestrict;
	}

    cFAIWallReflectMovingBehavior::~cFAIWallReflectMovingBehavior()
	{
	
	}
	void    cFAIWallReflectMovingBehavior::InternalInit()
	{
		float	l_fRadius = 10.f;
		const cBound*l_pBound = this->GetSrcCharacter()->GetWorldBound();
		if( l_pBound )
		{
			if(l_pBound->GetType() == cBound::Sphere_Bound)
			{
				l_fRadius = l_pBound->GetSphere().fRadius;
			}
		}
		Vector4	l_vRadiusWall = this->m_vWall.Extend(l_fRadius);
		this->m_vDirection = l_vRadiusWall.GetRelativeDirectionFromPos(this->GetSrcCharacter()->GetWorldPosition());
		m_fSpeedScale = this->m_fSpeedRestrict.Rand();
		m_iNumWallHit = (int)m_WallHitRetrict.Rand();
		m_CurrentWallHitTC.SetTargetTime((float)m_iNumWallHit);
	}

	void    cFAIWallReflectMovingBehavior::InternalUpdate(float e_fElpaseTime)
	{
		m_CurrentWallHitTC.Update(e_fElpaseTime);
		if( m_CurrentWallHitTC.bTragetTimrReached )
			this->m_bSatisfiedCondition = true;
		float	l_fRadius = 10.f;
		const cBound*l_pBound = this->GetSrcCharacter()->GetWorldBound();
		if( l_pBound )
		{
			if(l_pBound->GetType() == cBound::Sphere_Bound)
			{
				l_fRadius = l_pBound->GetSphere().fRadius;
			}
		}
		Vector3	l_vNewPos = this->GetSrcCharacter()->GetWorldPosition();
		l_vNewPos += this->GetSrcCharacter()->GetSpeed()*m_fSpeedScale*m_vDirection*e_fElpaseTime;
		Vector3	l_vNewPos2,l_vDirection2;
		if( m_vWall.CollideReflect(l_vNewPos,m_vDirection,&l_vDirection2,&l_vNewPos2,l_fRadius/2.f) )
		{//refelect
			l_vNewPos = l_vNewPos2;
			m_vDirection = l_vDirection2.Normalize();
			if( m_pHittWallCallback )
			{
				m_pHittWallCallback(this);
			}
		}
		this->GetSrcCharacter()->SetWorldPosition(l_vNewPos);
	}
}