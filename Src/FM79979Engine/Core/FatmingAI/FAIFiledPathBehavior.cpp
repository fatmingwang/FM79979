#include "stdafx.h"
#include "FAIFiledPathBehavior.h"
#include "FAIMachine.h"

namespace FATMING_AI
{
	//<cFAIFiledPathBehavior Name="" Wall="0,0,1280,720" XDistanceRestrict="100,300" YDistanceRestrict="100,300" TimeRestrict="7,10" PointRestrict="3,9" LOD="3" LeaveAwayProbability="0.05" />
	const wchar_t*cFAIFiledPathBehavior::TypeID( L"cFAIFiledPathBehavior" );
	cFAIFiledPathBehavior::cFAIFiledPathBehavior(TiXmlElement*e_pTiXmlElement)
	{
		int	l_iPathLOD = 3;
		m_fDirectionOffsetX = Vector2(-0.1f,0.1f);
		m_fDirectionOffsetY = Vector2(-0.1f,0.1f);
		m_fLeaveAwayProbability = 0.05f;
		m_iDistanceChangePoint = Vector2(3,5);
		m_vWall = Vector4(0,0,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
		m_CurveWithTime.SetCalAngle(true);
		AI_PARSE_NAME_VALUE_START
			COMPARE_NAME("Wall")
			{
				this->m_vWall = VALUE_TO_VECTOR4;
				m_vWall.z *= cGameApp::m_svGameScale.x;
				m_vWall.w *= cGameApp::m_svGameScale.y;
			}
			else
			COMPARE_NAME("XDistanceRestrict")
			{
				m_fXDistance = VALUE_TO_VECTOR2;
			}
			else
			COMPARE_NAME("YDistanceRestrict")
			{
				m_fYDistance = VALUE_TO_VECTOR2;
			}
			else
			COMPARE_NAME("TimeRestrict")
			{
				m_fTime = VALUE_TO_VECTOR2;
			}
			else
			COMPARE_NAME("PointRestrict")
			{
				m_iPoint = VALUE_TO_VECTOR2;
			}
			else
			COMPARE_NAME("LOD")
			{
				this->m_CurveWithTime.SetLOD(VALUE_TO_INT);
			}
			else
			COMPARE_NAME("LeaveAwayProbability")
			{
				m_fLeaveAwayProbability = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("DistanceChangePoint")
			{
				m_iDistanceChangePoint = VALUE_TO_VECTOR2;
			}
			else
			COMPARE_NAME("DirectionOffsetX")
			{
				m_fDirectionOffsetX = VALUE_TO_VECTOR2;
			}
			else
			COMPARE_NAME("DirectionOffsetY")
			{
				m_fDirectionOffsetY = VALUE_TO_VECTOR2;
			}
		PARSE_NAME_VALUE_END
	}

    cFAIFiledPathBehavior::cFAIFiledPathBehavior(cFAIFiledPathBehavior*e_pFAIPathBehavior):cFAIBaseBehave(e_pFAIPathBehavior)
	{
		m_fDirectionOffsetX = e_pFAIPathBehavior->m_fDirectionOffsetX;
		m_fDirectionOffsetY = e_pFAIPathBehavior->m_fDirectionOffsetY;
		m_iDistanceChangePoint = e_pFAIPathBehavior->m_iDistanceChangePoint;
		m_fLeaveAwayProbability = e_pFAIPathBehavior->m_fLeaveAwayProbability;
		this->m_vWall = e_pFAIPathBehavior->m_vWall;
		m_fXDistance = e_pFAIPathBehavior->m_fXDistance;
		m_fYDistance = e_pFAIPathBehavior->m_fYDistance;
		m_fTime = e_pFAIPathBehavior->m_fTime;
		m_iPoint = e_pFAIPathBehavior->m_iPoint;
		m_CurveWithTime.SetCalAngle(true);
		this->m_CurveWithTime.SetLOD(e_pFAIPathBehavior->m_CurveWithTime.GetLOD());
	}

    cFAIFiledPathBehavior::~cFAIFiledPathBehavior()
	{
	
	}

	Vector3	GetRelativeDirectionFromPosAndWall(Vector4 e_vWall,Vector3 e_vPos)
	{
		return Vector3(0.5f-(e_vPos.x/e_vWall.Width()),0.5f-(e_vPos.y/e_vWall.Height()),0).Normalize();
	}

	void    cFAIFiledPathBehavior::InternalInit()
	{
		//test code.
		//m_CurveWithTime.Destroy();
		//m_CurveWithTime.AddPoint(Vector3(0,320,0),0);
		//m_CurveWithTime.AddPoint(Vector3(1280,320,0),30);
		//m_CurveWithTime.Init();
		//return;
		int	l_iPathLOD = m_CurveWithTime.GetLOD();
		m_CurveWithTime.Destroy();
		m_CurveWithTime.SetLOD(1);
		assert(m_fXDistance.Max<=this->m_vWall.Width());
		assert(m_fYDistance.Max<=this->m_vWall.Height());
		Vector3	l_vStartPos = this->m_pSrcCharacter->GetWorldPosition();
		Vector3	l_vCharacterPos = l_vStartPos;
		//0.5 as center 
		float	l_fCenterPosValue = 0.5f+frand(-0.2f,0.2f);
		Vector3	l_vDirection(l_fCenterPosValue-(l_vCharacterPos.x/this->m_vWall.Width()),l_fCenterPosValue-(l_vCharacterPos.y/m_vWall.Height()),0);
		l_vDirection.NormalizeIt();
		int	l_iNumPoints = this->m_iPoint.Rand();
		float	l_fTime = m_fTime.Rand()/l_iNumPoints;
		m_CurveWithTime.AddPoint(l_vCharacterPos,0.f);
		Vector3	l_vLastPos;
		float	l_fTargetTime = 0.f;
		float	l_fSpeed = this->m_pSrcCharacter->GetSpeed();
		int		l_iNextChangePoint = m_iDistanceChangePoint.Rand();
		float	l_fScaleX = m_fXDistance.Rand();
		float	l_fScaleY = m_fYDistance.Rand();
		float	l_fBound = 10.f;
		const cBound*l_pBound = this->GetSrcCharacter()->GetWorldBound();
		if( l_pBound )
		{
			if(l_pBound->GetType() == cBound::Sphere_Bound)
			{
				l_fBound = l_pBound->GetSphere().fRadius;
			}
		}
		Vector4	l_vNewWall = m_vWall.Extend(l_fBound);
		for( int i=0;i<l_iNumPoints;++i )
		{
			--l_iNextChangePoint;
			l_fTargetTime = l_fTime*(i+1);
			if( l_iNextChangePoint == 0 )
			{
				l_fScaleX = m_fXDistance.Rand();
				l_fScaleY = m_fYDistance.Rand();
				l_vDirection.x += m_fDirectionOffsetX.Rand();
				l_vDirection.y += m_fDirectionOffsetY.Rand();
				l_vDirection.NormalizeIt();
			}
			Vector3	l_vNewPos(l_fScaleX*l_vDirection.x*l_fSpeed,l_fScaleY*l_vDirection.y*l_fSpeed,0.f);
			float	l_fLength = l_vNewPos.Length();
			l_vLastPos = l_vCharacterPos+l_vNewPos;
			l_vCharacterPos = l_vLastPos;
			m_CurveWithTime.AddPoint(l_vLastPos,l_fTargetTime);
			if( l_iNextChangePoint == 0 )
			{
				l_iNextChangePoint = m_iDistanceChangePoint.Rand();
			}
			if(!l_vNewWall.CollidePoint(l_vLastPos.x,l_vLastPos.y))
			{
				break;
			}
		}
		Vector3	l_vPos1 = m_CurveWithTime.GetOriginalPointList()[0];
		Vector3	l_vPos2 = m_CurveWithTime.GetOriginalPointList()[m_CurveWithTime.GetOriginalTimeList().size()-1];
		float	l_fLength2 = (l_vPos2-l_vPos1).Length();
		m_CurveWithTime.SetLOD(l_iPathLOD);
		m_CurveWithTime.Init();
	}

	void    cFAIFiledPathBehavior::InternalUpdate(float e_fElpaseTime)
	{
		float	l_fAngle = m_CurveWithTime.GetCurrentPosToNextPointAngle();
		Vector3	l_vPos = m_CurveWithTime.GetCurrentPosition();
		bool	l_bKeepMoving = true;
		if(m_CurveWithTime.IsAnimationDone())
		{
			if( m_fLeaveAwayProbability >= frand(0,1) )
				this->m_bSatisfiedCondition = true;
			float	l_fBound = 10.f;
			Vector3	l_vDirection = m_CurveWithTime.GetCurrentDirection();
			const cBound*l_pBound = this->GetSrcCharacter()->GetWorldBound();
			if( l_pBound )
			{
				if(l_pBound->GetType() == cBound::Sphere_Bound)
				{
					l_fBound = l_pBound->GetSphere().fRadius;
				}
			}
			Vector4	l_vScrCharacterVisibleWall = this->m_vWall;
			l_vScrCharacterVisibleWall.x -= l_fBound;
			l_vScrCharacterVisibleWall.y -= l_fBound;
			l_vScrCharacterVisibleWall.z += l_fBound;
			l_vScrCharacterVisibleWall.w += l_fBound;
			float	l_fSpeed = this->m_pSrcCharacter->GetRunSpeed();
			if( l_vDirection.Length() == 0 )
				l_vDirection = Vector3(1,0,0);
			l_vPos = (l_vDirection*l_fSpeed*e_fElpaseTime)+l_vPos;
			m_CurveWithTime.SetCurrentPosition(l_vPos);
			if(l_vScrCharacterVisibleWall.CollidePoint(l_vPos.x,l_vPos.y))
			{
				l_bKeepMoving = true;
			}
			else
			{
				l_bKeepMoving = false;
			}
		}
		else
			m_CurveWithTime.Update(e_fElpaseTime);
		m_pSrcCharacter->SetLocalPosition(l_vPos);
		Vector3	l_v2DAngle(0,0,m_CurveWithTime.GetCurrentPosToNextPointAngle());
		m_pSrcCharacter->SetAngle(l_v2DAngle);
		float	l_fSpeed = m_CurveWithTime.GetSpeedBySecond(e_fElpaseTime);
		if( l_fSpeed >= 0.f )
		{
			m_pSrcCharacter->SetAnimationSpeed(l_fSpeed);
		}
		if( !l_bKeepMoving )
		{
			this->m_bSatisfiedCondition = true;
		}
	}

	void cFAIFiledPathBehavior::Render()
	{
		m_CurveWithTime.Render(Vector4(1, 1, 1, 0.2f));
	}
}