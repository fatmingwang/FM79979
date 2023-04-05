#include "Stdafx.h"
#include "FAIAnimationBehavior.h"
#include "FAIMachine.h"
#include "FAIAnimationCharacter.h"
namespace FATMING_AI
{
    const wchar_t*cFAIMPDIAnimationChangeAIBehavior::TypeID( L"cFAIMPDIAnimationChangeAIBehavior" );
    const wchar_t*cFAIMPDIAnimationMovingAIBehavior::TypeID( L"cFAIMPDIAnimationMovingAIBehavior" );
    

	cFAIMPDIAnimationChangeAIBehavior::cFAIMPDIAnimationChangeAIBehavior(TiXmlElement*e_pTiXmlElement)
	{
		m_pFMAnimationChanger = 0;
		if( !e_pTiXmlElement )
			return;
        AI_PARSE_NAME_VALUE_START
            COMPARE_NAME("AnimationName")
            {
				m_strTargetAnimation = l_strValue;
            }
        PARSE_NAME_VALUE_END
	}

    cFAIMPDIAnimationChangeAIBehavior::cFAIMPDIAnimationChangeAIBehavior(cFAIMPDIAnimationChangeAIBehavior*e_pMPDIAnimationChangeAIBehavior)
    :cFAIBaseBehave(e_pMPDIAnimationChangeAIBehavior)
    {
        m_pFMAnimationChanger = 0;
		m_strTargetAnimation = e_pMPDIAnimationChangeAIBehavior->m_strTargetAnimation;
		this->SetSrcCharacter(e_pMPDIAnimationChangeAIBehavior->m_pSrcCharacter);
    }

    cFAIMPDIAnimationChangeAIBehavior::~cFAIMPDIAnimationChangeAIBehavior()
    {
    }

	void	cFAIMPDIAnimationChangeAIBehavior::SetSrcCharacter(cFAICharacterInterface*e_pAICharacterInterface)
	{
		cFAIBaseBehave::SetSrcCharacter(e_pAICharacterInterface);
		m_pFMAnimationChanger = (cFAIAnimationCharacter*)e_pAICharacterInterface->GetAnimationData();
	}

	void    cFAIMPDIAnimationChangeAIBehavior::InternalInit()
	{
		if( !m_pFMAnimationChanger && m_pSrcCharacter )
		{
			m_pFMAnimationChanger = (cFAIAnimationCharacter*)this->m_pSrcCharacter->GetAnimationData();
		}
		if( m_pFMAnimationChanger )
		{
			int l_iTargetIndex = m_pFMAnimationChanger->GetObjectIndexByName(m_strTargetAnimation.c_str());
			m_pFMAnimationChanger->SetCurrentWorkingObject(l_iTargetIndex);
			cRenderObject*l_pBaseBehavior = m_pFMAnimationChanger->GetCurrentWorkingObject();
			if(l_pBaseBehavior->Type() == cMPDI::TypeID)
			{
				cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(l_pBaseBehavior);
				//l_pMPDI-SetPosByImageCenter(m_pSrcCharacter->GetWorldPosition());
				l_pMPDI->SetParent(m_pSrcCharacter);
			}
		}
		else
		{
			UT::ErrorMsg(m_pSrcCharacter->GetName(),L" this object is not animation cjaracter!!");
		}
		//m_pFMAnimationChanger->GetCurrentWorkingObject()->Init();
		//m_pFMAnimationChanger->GetCurrentWorkingObject()->SetLoop(false);
	}

	void    cFAIMPDIAnimationChangeAIBehavior::InternalUpdate(float e_fElpaseTime)
	{
		cRenderObject*l_pBaseBehavior = m_pFMAnimationChanger->GetCurrentWorkingObject();
		if( l_pBaseBehavior )
		{
			l_pBaseBehavior->Update(e_fElpaseTime);
			if( l_pBaseBehavior->Type() == cMPDI::TypeID )
			{
				cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(l_pBaseBehavior);
				//l_pMPDI->SetPosByImageCenter(m_pSrcCharacter->GetLocalPosition());
				l_pMPDI->SetAnimationLoop(true);
				if( l_pMPDI->IsAnimationDone() )
					this->m_bSatisfiedCondition = true;
			}
		}
	}

    void    cFAIMPDIAnimationChangeAIBehavior::Render()
    {
		cRenderObject*l_pBaseBehavior = m_pFMAnimationChanger->GetCurrentWorkingObject();
		if( l_pBaseBehavior )
		{
			if( l_pBaseBehavior->Type() == cMPDI::TypeID )
			{
				cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(l_pBaseBehavior);
				l_pMPDI->Render(); 
			}
		}
    }
	
    cFAIMPDIAnimationMovingAIBehavior::cFAIMPDIAnimationMovingAIBehavior(TiXmlElement*e_pTiXmlElement)
    :cFAIMPDIAnimationChangeAIBehavior(e_pTiXmlElement)
    {
        m_fSafeDistance = 1000.f;
        m_vDestPos = Vector3(0,0,0);
		if( !e_pTiXmlElement )
			return;
        AI_PARSE_NAME_VALUE_START
            COMPARE_NAME("DestPos")
            {
                m_vDestPos = VALUE_TO_VECTOR3;
            }
            else
            COMPARE_NAME("SafeDistance")
            {
                m_fSafeDistance = VALUE_TO_FLOAT;
            }
        PARSE_NAME_VALUE_END
    }
    
    cFAIMPDIAnimationMovingAIBehavior::cFAIMPDIAnimationMovingAIBehavior(cFAIMPDIAnimationMovingAIBehavior*e_pMPDIAnimationMovingAIBehavior)
    :cFAIMPDIAnimationChangeAIBehavior(e_pMPDIAnimationMovingAIBehavior)
    {
        m_fSafeDistance = e_pMPDIAnimationMovingAIBehavior->m_fSafeDistance;
        m_vDestPos = e_pMPDIAnimationMovingAIBehavior->m_vDestPos;
    }

    void    cFAIMPDIAnimationMovingAIBehavior::InternalInit()
    {
        cFAIMPDIAnimationChangeAIBehavior::InternalInit();
        ////m_pFMAnimationChanger->GetCurrentWorkingObject()->SetLoop(false);
        float   l_fDisToDest = (m_vDestPos-this->m_pSrcCharacter->GetLocalPosition()).Length();
        if(l_fDisToDest<=m_fSafeDistance)
        {
            m_bSatisfiedCondition = true;
        }
    }
    
    void    cFAIMPDIAnimationMovingAIBehavior::InternalUpdate(float e_fElpaseTime)
    {
		cRenderObject*l_pBaseBehavior = m_pFMAnimationChanger->GetCurrentWorkingObject();
		if( l_pBaseBehavior )
		{
			if( l_pBaseBehavior->Type() == cMPDI::TypeID )
			{
				cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(l_pBaseBehavior);
				if( l_pMPDI->IsAnimationDone() )
				{
					this->m_pSrcCharacter->SetLocalPosition(this->m_pSrcCharacter->GetLocalPosition()+l_pMPDI->GetPathCurrentPos());
				}
			}
		}
    }
}