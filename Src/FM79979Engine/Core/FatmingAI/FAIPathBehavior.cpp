#include "stdafx.h"
#include "FAIPathBehavior.h"
#include "FAIMachine.h"
#include "FAIParser.h"
namespace FATMING_AI
{
	extern cFAIBehaviorParser*g_pFAIBehaviorParser;
	//<cFAIPathBehavior Name="FixedPath" PathFile="ABC.path" />
	const wchar_t*cFAIPathBehavior::TypeID( L"cFAIPathBehavior" );
	cFAIPathBehavior::cFAIPathBehavior(TiXmlElement*e_pTiXmlElement)
	{
		m_CurveWithTime.SetCalAngle(true);
		m_pCurveManager = 0;
		AI_PARSE_NAME_VALUE_START
			COMPARE_NAME("PathFile")
			{
				m_pCurveManager = cGameApp::GetCurveManagerByFileName(l_strValue);
				if( !m_pCurveManager )
				{
					UT::ErrorMsg(l_strValue,L"path parse failed");
				}
				int	l_iCount = m_pCurveManager->Count();
				for( int i=0;i<l_iCount;++i )
				{
					if( m_pCurveManager->GetObject(i)->GetEndTime() == 0.f )
					{
						UT::ErrorMsg(m_pCurveManager->GetObject(i)->GetName(),L"time is zero!!");
					}
				}
			}
		PARSE_NAME_VALUE_END
	}

    cFAIPathBehavior::cFAIPathBehavior(cFAIPathBehavior*e_pFAIPathBehavior):cFAIBaseBehave(e_pFAIPathBehavior)
	{
		m_CurveWithTime.SetCalAngle(true);
		m_pCurveManager = e_pFAIPathBehavior->m_pCurveManager;
	}

    cFAIPathBehavior::~cFAIPathBehavior()
	{
	
	}

	void    cFAIPathBehavior::InternalInit()
	{
		if( m_pCurveManager )
		{
			if( m_pCurveManager->Count() )
				m_CurveWithTime = *m_pCurveManager->GetObject(rand()%m_pCurveManager->Count());
			m_CurveWithTime.Init();
			m_CurveWithTime.SetCalAngle(true);
		}
	}

	void    cFAIPathBehavior::InternalUpdate(float e_fElpaseTime)
	{
		m_CurveWithTime.Update(e_fElpaseTime);
		float	l_fAngle = m_CurveWithTime.GetCurrentPosToNextPointAngle();
		Vector3	l_v2DAngle(0,0,m_CurveWithTime.GetCurrentPosToNextPointAngle());
		m_pSrcCharacter->SetAngle(l_v2DAngle);
		Vector3	l_vPos = m_CurveWithTime.GetCurrentPosition();
		if(this->m_pSrcCharacter)
		{
			float	l_fSpeed = m_CurveWithTime.GetSpeedBySecond(e_fElpaseTime);
			if( l_fSpeed >= 0.f )
			{
				m_pSrcCharacter->SetAnimationSpeed(l_fSpeed);
			}
			m_pSrcCharacter->SetLocalPosition(l_vPos);
		}
		if(m_CurveWithTime.IsCurveMoveDone())
		{
			this->m_bSatisfiedCondition = true;
		}
	}

}