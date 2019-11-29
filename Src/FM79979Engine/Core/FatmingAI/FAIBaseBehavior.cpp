#include "Stdafx.h"
#include "FAIBaseBehavior.h"
namespace FATMING_AI
{
    const wchar_t*cFAIBaseBehave::TypeID( L"cFAIBaseBehave" );

	cFAIBaseBehave::cFAIBaseBehave()
	{
		m_pData = 0;
	    m_bSatisfiedCondition = false;
	    m_pSrcCharacter = 0;
	    m_bEnable = true;
	    m_bKillAfterDone = false; 
	    m_iProbabilityToDo = 1;	
	}

	cFAIBaseBehave::cFAIBaseBehave(cFAIBaseBehave*e_pBaseAIBehave)
	{
	    m_iProbabilityToDo = e_pBaseAIBehave->m_iProbabilityToDo;
	    m_pData = e_pBaseAIBehave->m_pData;
	    m_bSatisfiedCondition = false;
	    m_strNextAIBehaviorName = e_pBaseAIBehave->m_strNextAIBehaviorName;
	    m_pSrcCharacter = 0;
	    m_bKillAfterDone = e_pBaseAIBehave->m_bKillAfterDone;
	    m_bEnable = e_pBaseAIBehave->m_bEnable;
	    this->SetName(e_pBaseAIBehave->GetName());
	}
	cFAIBaseBehave::~cFAIBaseBehave()
	{

	}
	void	cFAIBaseBehave::SetSrcCharacter(cFAICharacterInterface*e_pAICharacterInterface)
	{
		m_pSrcCharacter = e_pAICharacterInterface;
	}
    void    cFAIBaseBehave::SetNextAIBehaviorName(const WCHAR*e_strName)
    {
		m_strNextAIBehaviorName.clear();
		if( e_strName )
			m_strNextAIBehaviorName = e_strName;
    }
//end namespace FATMING_AI
}