#include "stdafx.h"
#include "FAITranslationBehavior.h"
#include "FAIMachine.h"
namespace FATMING_AI
{
    const wchar_t*cFAIWalkToDestinationAIBehavior::TypeID( L"cFAIWalkToDestinationAIBehavior" );
    const wchar_t*cFAIIdleAIBehavior::TypeID( L"cFAIIdleAIBehavior" );

	cFAIWalkToDestinationAIBehavior::cFAIWalkToDestinationAIBehavior(TiXmlElement*e_pTiXmlElement)
	{
		m_fCharacterSpeed = 0.f;
		if( !e_pTiXmlElement )
			return;
        AI_PARSE_NAME_VALUE_START
            COMPARE_NAME("DestPos")
            {
                SetDestPos(VALUE_TO_VECTOR3);
            }
            else
            COMPARE_NAME("Speed")
            {
                m_fCharacterSpeed = VALUE_TO_FLOAT;
            }
        PARSE_NAME_VALUE_END
	}
    
    cFAIWalkToDestinationAIBehavior::cFAIWalkToDestinationAIBehavior(cFAIWalkToDestinationAIBehavior*e_pWalkToDestinationAIBehavior)
    :cFAIBaseBehave(e_pWalkToDestinationAIBehavior)
    {
        m_vDestPos = e_pWalkToDestinationAIBehavior->m_vDestPos;
        m_fCharacterSpeed = e_pWalkToDestinationAIBehavior->m_fCharacterSpeed;
        //m_pSrcCharacter->SetSpeed(m_fCharacterSpeed);
    }

    cFAIWalkToDestinationAIBehavior::~cFAIWalkToDestinationAIBehavior(){}

    void    cFAIWalkToDestinationAIBehavior::InternalInit()
    {
		m_CurveWithTime.Destroy();
		m_CurveWithTime.SetCalAngle(true);
        //if( e_pAIData )
          //  m_vDestPos = *(Vector3*)e_pAIData;
        //Vector3 l_vDiretion = m_vDestPos-m_pSrcCharacter->GetWorldPosition();
        //m_pSrcCharacter->SetDirection(l_vDiretion.Normalize());
		if( m_pSrcCharacter )
		{
			Vector3 l_vSrcPos = m_pSrcCharacter->GetLocalPosition();
			m_pSrcCharacter->SetLocalTransform(Quaternion::LookAtQuaternion(m_vDestPos,l_vSrcPos).ToMatrix());
			m_pSrcCharacter->SetSpeed(this->m_fCharacterSpeed);
			Vector3 l_vDirection = m_pSrcCharacter->GetLocalDirection();
			m_pSrcCharacter->SetLocalPosition(l_vSrcPos);
			float	l_fTime = m_vDestPos.Length()/m_fCharacterSpeed;
			m_CurveWithTime.AddPoint(l_vSrcPos,0.f);
			m_CurveWithTime.AddPoint(m_vDestPos,l_fTime);
		}
    }

    void    cFAIWalkToDestinationAIBehavior::InternalUpdate(float e_fElpaseTime)
    {
		Vector3	l_vDirection = m_vDestPos-m_pSrcCharacter->GetWorldPosition();
        float   l_fDistance = l_vDirection.Length();
		Vector3	l_vMovedDis = m_pSrcCharacter->GetSpeed()*e_fElpaseTime*l_vDirection.Normalize();
		Vector3	l_vNewPos = l_vMovedDis+m_pSrcCharacter->GetWorldPosition();
		m_pSrcCharacter->SetLocalPosition(l_vNewPos);
        float   l_fNewDistance = (m_vDestPos-m_pSrcCharacter->GetWorldPosition()).Length();
		//set angle
		l_vDirection.NormalizeIt();
		m_CurveWithTime.Update(e_fElpaseTime);
		m_pSrcCharacter->SetAngle(Vector3(0,0,m_CurveWithTime.GetCurrentPosToNextPointAngle()));
		if( m_CurveWithTime.IsAnimationDone() )
        {
            this->m_bSatisfiedCondition = true;
        }
    }

	cFAIIdleAIBehavior::cFAIIdleAIBehavior(TiXmlElement*e_pTiXmlElement)
	{
        AI_PARSE_NAME_VALUE_START
            COMPARE_NAME("Time")
            {
                m_IdleTimeCounter.SetTargetTime(VALUE_TO_FLOAT);
            }
        PARSE_NAME_VALUE_END
	}
    
    cFAIIdleAIBehavior::cFAIIdleAIBehavior(cFAIIdleAIBehavior*e_pIdleAIBehavior)
    :cFAIBaseBehave(e_pIdleAIBehavior)
    {
		m_IdleTimeCounter.SetTargetTime(e_pIdleAIBehavior->m_IdleTimeCounter.fTargetTime);
    }
}