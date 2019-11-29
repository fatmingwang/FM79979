#include "Stdafx.h"
#include "FAIMultiTaskBehavior.h"
#include "FAIMachine.h"
#include "FAIParser.h"
namespace FATMING_AI
{
    const wchar_t*cFAIMultiAITaskBehavior::TypeID( L"cFAIMultiAITaskBehavior" );

	cFAIMultiAITaskBehavior::cFAIMultiAITaskBehavior(TiXmlElement*e_pTiXmlElement)
	{
		m_iTargetAIBehavior = -1;
		if(!e_pTiXmlElement)
			return;
        const   WCHAR*l_strTargetBehaviorName = 0;
        AI_PARSE_NAME_VALUE_START
            COMPARE_NAME("TargetAIBehavior")
            {
                l_strTargetBehaviorName = l_strValue;
            }
        PARSE_NAME_VALUE_END
        TiXmlElement*l_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
        while( l_pTiXmlElement )
        {
            cFAIBaseBehave*l_pChileAIBehave = cFAIBehaviorParser::ProcessAIBehavior(l_pTiXmlElement);
			if( l_pChileAIBehave )
				this->AddObjectNeglectExist(l_pChileAIBehave);
            l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
        }
        if( l_strTargetBehaviorName )
        {
            int l_iIndex = this->GetObjectIndexByName(l_strTargetBehaviorName);
            assert(l_iIndex!=-1);
            this->SetTargetAIBehavior(l_iIndex);
        }
	}
    cFAIMultiAITaskBehavior::cFAIMultiAITaskBehavior(cFAIMultiAITaskBehavior*e_pMultiAITaskBehavior)
    :cFAIBaseBehave(e_pMultiAITaskBehavior)
    {
        m_iTargetAIBehavior = e_pMultiAITaskBehavior->m_iTargetAIBehavior;
        int l_iCount = e_pMultiAITaskBehavior->Count();
        for( int i=0;i<l_iCount;++i  )
        {
            this->AddObjectNeglectExist(dynamic_cast<cFAIBaseBehave*>(e_pMultiAITaskBehavior->GetObject(i)->Clone()));
        }
    }

	cFAIMultiAITaskBehavior::~cFAIMultiAITaskBehavior()
	{

	}

    void    cFAIMultiAITaskBehavior::InternalInit()
    {
        int l_iCount = this->Count();
        for( int i=0;i<l_iCount;++i )
        {
            this->m_ObjectList[i]->Init();
        }
    }
    void    cFAIMultiAITaskBehavior::InternalUpdate(float e_fElpaseTime)
    {
        bool l_bAllDone = true;
        int l_iCount = this->Count();
        for( int i=0;i<l_iCount;++i )
        {
            this->m_ObjectList[i]->Update(e_fElpaseTime);
            if( l_bAllDone )
                l_bAllDone = this->m_ObjectList[i]->IsSatisfiedCondition();
        }
        if( l_bAllDone )
            this->m_bSatisfiedCondition = true;
        else
        if( m_iTargetAIBehavior != -1 )
        {
            m_bSatisfiedCondition = this->m_ObjectList[m_iTargetAIBehavior]->IsSatisfiedCondition();
            #ifdef DEBUG
            if( m_bSatisfiedCondition )
            {
                int a=0;
            }
            #endif
        }
    }     

    void    cFAIMultiAITaskBehavior::SetSrcCharacter(cFAICharacterInterface*e_pAICharacterInterface)
    {
        m_pSrcCharacter = e_pAICharacterInterface;
        int l_iCount = this->Count();
        for( int i=0;i<l_iCount;++i )
        {
            m_ObjectList[i]->SetSrcCharacter(m_pSrcCharacter);
        }
    }
}