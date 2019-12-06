#include "Stdafx.h"
#include "FAILeaveFiledBehavior.h"
#include "FAIMachine.h"

namespace FATMING_AI
{
	//<cFAILeaveFiledBehavior Name="" Wall="0,0,1280,720" Speed="250" Radius="150" />
	const wchar_t*cFAILeaveFiledBehavior::TypeID( L"cFAILeaveFiledBehavior" );
	cFAILeaveFiledBehavior::cFAILeaveFiledBehavior(TiXmlElement*e_pTiXmlElement)
	{
		m_CurveWithTime.SetCalAngle(true);
		int	l_iPathLOD = 3;
		m_fSpeed = 1.f;
		m_vWall = Vector4(0,0,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
		AI_PARSE_NAME_VALUE_START
			COMPARE_NAME("Wall")
			{
				this->m_vWall = VALUE_TO_VECTOR4;
				m_vWall.z *= cGameApp::m_spOpenGLRender->m_vGameScale.x;
				m_vWall.w *= cGameApp::m_spOpenGLRender->m_vGameScale.y;
			}
			else
			COMPARE_NAME("Radius")
			{
				m_fRadius = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("Speed")
			{
				m_fSpeed = VALUE_TO_FLOAT;
			}
		PARSE_NAME_VALUE_END
	}

    cFAILeaveFiledBehavior::cFAILeaveFiledBehavior(cFAILeaveFiledBehavior*e_pFAIPathBehavior):cFAIBaseBehave(e_pFAIPathBehavior)
	{
		m_CurveWithTime.SetCalAngle(true);
		this->m_vWall = e_pFAIPathBehavior->m_vWall;
		m_fRadius = e_pFAIPathBehavior->m_fRadius;
		m_fSpeed = e_pFAIPathBehavior->m_fSpeed;
	}

    cFAILeaveFiledBehavior::~cFAILeaveFiledBehavior()
	{
	
	}

	void    cFAILeaveFiledBehavior::InternalInit()
	{
		m_CurveWithTime.Destroy();
		m_CurveWithTime.SetLOD(1);
		const cBound*l_pBound = this->m_pSrcCharacter->GetWorldBound();
		if( l_pBound )
		{
			if(l_pBound->GetType() == cBound::Sphere_Bound)
			{
				this->m_fRadius = l_pBound->GetSphere().fRadius;
			}
		}
		Vector3	l_vCurrentPos = this->m_pSrcCharacter->GetWorldPosition();
		float	l_fSafeDistanceScale = 1.1f;
		float	l_fSafeRadius = l_fSafeDistanceScale*m_fRadius;
		Vector4	l_vNewWall(this->m_vWall.x-l_fSafeRadius,this->m_vWall.y-l_fSafeRadius,this->m_vWall.z+l_fSafeRadius,this->m_vWall.w+l_fSafeRadius);
		Vector3	l_vFinalPos = l_vNewWall.GetCloseWall(l_vCurrentPos.x,l_vCurrentPos.y);
		if(rand()%2)
		{
			l_vFinalPos.x = frand(l_vCurrentPos.x,l_vFinalPos.x);
		}
		else
		{
			l_vFinalPos.y = frand(l_vCurrentPos.y,l_vFinalPos.y);
		}
		m_CurveWithTime.AddPoint(l_vCurrentPos,0);
		Vector3	l_vDirection = l_vFinalPos-l_vCurrentPos;
		float	l_fModules = l_vDirection.Length()/m_fSpeed;
		if( l_fModules == 0.f )
		{
			int a=0;
		}
		int	l_iNumPoint = (int)l_fModules;
		float	l_fTime = l_fModules/l_iNumPoint;
		l_vDirection.NormalizeIt();
		bool	l_bHorizontal = rand()%2?true:false;
		for( int i=1;i<l_iNumPoint;++i )
		{
			if( l_bHorizontal )
				l_vDirection.x += frand(-0.1f,0.1f);
			else
				l_vDirection.y += frand(-0.1f,0.1f);
			l_vDirection.NormalizeIt();
			Vector3	l_vNewPos = m_fSpeed * l_vDirection;
			l_vCurrentPos += l_vNewPos;
			m_CurveWithTime.AddPoint(l_vCurrentPos,i*l_fTime);
		}
		if( l_vFinalPos.x >0.f && l_vFinalPos.x< 1280.f && l_vFinalPos.y >0.f && l_vFinalPos.y< 720.f )
		{
			int a=0;
		}
		m_CurveWithTime.AddPoint(l_vFinalPos,l_fModules);
		m_CurveWithTime.SetLOD(3);
		m_CurveWithTime.Init();
		
	}

	void    cFAILeaveFiledBehavior::InternalUpdate(float e_fElpaseTime)
	{
		m_CurveWithTime.Update(e_fElpaseTime);
		float	l_fAngle = m_CurveWithTime.GetCurrentPosToNextPointAngle();
		if(this->m_pSrcCharacter)
		{
			Vector3	l_v2DAngle(0,0,m_CurveWithTime.GetCurrentPosToNextPointAngle());
			m_pSrcCharacter->SetAngle(l_v2DAngle);
			m_pSrcCharacter->SetLocalPosition(m_CurveWithTime.GetCurrentPosition());
		}
		if(m_CurveWithTime.IsAnimationDone())
		{
			this->m_bSatisfiedCondition = true;
		}
	}

}