#include "stdafx.h"
#include "FAICharacterInterface.h"
#include "FAIStatusChangeByProbability.h"
#include "FAIMachine.h"
#include "FAIAnimationCharacter.h"
namespace FATMING_AI
{
	const wchar_t*cFAIStatusChangeByProbability::TypeID( L"cFAIStatusChangeByProbability" );
	//<cFAIStatusChangeByProbability Name="" Probability="" ForceToChangeBehavior="0" StartBehaviorStatus="Idle" >
	//	<Data Type="" Probability="" HoldTime="" />
	//	<Data Type="" Probability="" HoldTime="" />
	//	<Data Type="" Probability="" HoldTime="" />
	//	<Data Type="" Probability="" HoldTime="" />
	//</cFAIStatusChangeByProbability>
	cFAIStatusChangeByProbability::cFAIStatusChangeByProbability(TiXmlElement*e_pTiXmlElement)
	{
		m_eStartBehaviodStatus = eFAI_CBS_MAX;
		m_pCurrentTypeHoldTimeUpdate = 0;
		m_bForceToChangeBehavior = false;
		m_pCharacterStatus = 0;
		//m_ePriorBehaviodStatus = eFAI_CBS_MAX;
		ASSERT_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pTiXmlElement,cFAIStatusChangeByProbability::TypeID);
		AI_PARSE_NAME_VALUE_START
			COMPARE_NAME("ForceToChangeBehavior")
			{
				m_bForceToChangeBehavior = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("StartBehaviorStatus")
			{
				m_eStartBehaviodStatus = GetCharacterBehaviodStatus(l_strValue);
			}
		PARSE_NAME_VALUE_END
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		std::vector<float>					l_ProbabilityVector;
		std::vector<sHoldTimeAndType>		l_HoldTimeAndTypeVector;
		while( e_pTiXmlElement )
		{
			sHoldTimeAndType	l_HoldTimeAndType;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("Type")
				{
					l_HoldTimeAndType.FAICharacterBehaviorStatus = GetCharacterBehaviodStatus(l_strValue);
					
				}
				else
				COMPARE_NAME("Probability")
				{
					l_ProbabilityVector.push_back(VALUE_TO_FLOAT);
				}
				else
				COMPARE_NAME("HoldTime")
				{
					l_HoldTimeAndType.RandHoldTime = (VALUE_TO_VECTOR2);
					l_HoldTimeAndType.TC.SetTargetTime(l_HoldTimeAndType.RandHoldTime.Rand());
				}
			PARSE_NAME_VALUE_END
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
		m_ProbabilityValue.SetupData(l_ProbabilityVector,l_HoldTimeAndTypeVector);
	}

    cFAIStatusChangeByProbability::cFAIStatusChangeByProbability(cFAIStatusChangeByProbability*e_pFAIPathBehavior):cFAIBaseBehave(e_pFAIPathBehavior)
	{
		m_pCurrentTypeHoldTimeUpdate = 0;
		m_bForceToChangeBehavior = e_pFAIPathBehavior->m_bForceToChangeBehavior;
		m_pCharacterStatus = 0;
		m_eStartBehaviodStatus = e_pFAIPathBehavior->m_eStartBehaviodStatus;
		//m_ePriorBehaviodStatus = eFAI_CBS_MAX;
		m_ProbabilityValue = e_pFAIPathBehavior->m_ProbabilityValue;
	}

    cFAIStatusChangeByProbability::~cFAIStatusChangeByProbability()
	{
	
	}

	void    cFAIStatusChangeByProbability::InternalInit()
	{
		m_pCharacterStatus = this->m_pSrcCharacter->GetStatus();
		//m_ePriorBehaviodStatus = m_pCharacterStatus->GetFAICharacterBehaviorStatus();
		//m_PriorCharacterStatus = *m_pCharacterStatus;
		sHoldTimeAndType*	l_pHoldTimeAndType = 0;
		for(int i=0;i<(int)m_ProbabilityValue.m_ValueVector.size();++i)
		{
			if( m_eStartBehaviodStatus == m_ProbabilityValue.m_ValueVector[i].FAICharacterBehaviorStatus )
			{
				l_pHoldTimeAndType = &m_ProbabilityValue.m_ValueVector[i];
				break;
			}
		}
		if( l_pHoldTimeAndType == 0 )
			l_pHoldTimeAndType = m_ProbabilityValue.GetValueByProbability();
		if( m_eStartBehaviodStatus != eFAI_CBS_MAX )
		{
			m_pCharacterStatus->SetChracterBehaviorStatus(m_eStartBehaviodStatus,true);
		}
		else
			m_pCharacterStatus->SetChracterBehaviorStatus(l_pHoldTimeAndType->FAICharacterBehaviorStatus,m_bForceToChangeBehavior);
		m_pCurrentTypeHoldTimeUpdate = l_pHoldTimeAndType;
		//assert(cFAIAnimationCharacter::TypeID == this->m_pSrcCharacter->Type())
	}

	void    cFAIStatusChangeByProbability::InternalUpdate(float e_fElpaseTime)
	{
		if( m_pCurrentTypeHoldTimeUpdate )
		{
			m_pCurrentTypeHoldTimeUpdate->TC.Update(e_fElpaseTime);
			if(m_pCurrentTypeHoldTimeUpdate->TC.bTragetTimrReached)
			{
				if( !m_pSrcCharacter->IsCharacterBehaviorStatusLock() )
				{
					m_pCurrentTypeHoldTimeUpdate = m_ProbabilityValue.GetValueByProbability();
					m_pCurrentTypeHoldTimeUpdate->TC.Start();
					m_pCharacterStatus->SetChracterBehaviorStatus(m_pCurrentTypeHoldTimeUpdate->FAICharacterBehaviorStatus,false);
				}
				else
				if( this->m_bForceToChangeBehavior )
				{
					m_pCharacterStatus->SetChracterBehaviorStatus(m_pCurrentTypeHoldTimeUpdate->FAICharacterBehaviorStatus,m_bForceToChangeBehavior);
				}
			}
		}
		else
		{
		
		}
		//if( m_ePriorBehaviodStatus != m_pCharacterStatus->GetFAICharacterBehaviorStatus() )
		//{
		//	m_ePriorBehaviodStatus = m_pCharacterStatus->GetFAICharacterBehaviorStatus();
		//	cFAIAnimationCharacter*l_pFAIAnimationCharacter = (cFAIAnimationCharacter*)this->m_pSrcCharacter;
		//	int	l_iIndex = l_pFAIAnimationCharacter->GetObjectIndexByName(g_strCharacterStatus[m_ePriorBehaviodStatus]);
		//	if( l_iIndex != -1 )
		//		l_pFAIAnimationCharacter->SetCurrentWorkingObject(l_iIndex);
		//	//this->m_bSatisfiedCondition = true;
		//}
		//m_ePriorBehaviodStatus = m_pCharacterStatus->GetFAICharacterBehaviorStatus();
	}

}