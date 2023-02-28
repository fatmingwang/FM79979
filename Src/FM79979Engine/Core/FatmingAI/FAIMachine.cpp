#include "stdafx.h"
#include "FAIMachine.h"
#include "FAIParser.h"
namespace FATMING_AI
{
    const wchar_t*cFAIMachine::TypeID( L"cFAIMachine" );
	//	<cFAIMachine Name="NormalZombie" StartBehaviorName="Walk" cFAICharacterInterface="" >
	//	    <!-- DestPos coudl set at the game -->
	//	    <cFAIMultiAITaskBehavior Name="Walk" Probability="10" TargetAIBehavior="1" NextAIName="Idle">
	//	    	<cFAIWalkToDestinationAIBehavior Name="1" DestPos="0,200,0" Speed="100" />
	//	    	<cFAIMPDIAnimationChangeAIBehavior AnimationName="Walking" />
	//	    </cFAIMultiAITaskBehavior>
	//	    <cFAIMultiAITaskBehavior Name="Idle" Probability="10" TargetAIBehavior="Idle" >
	//		<cFAIIdleAIBehavior Name="Idle" Time="2" />
	//	    	<cFAIMPDIAnimationChangeAIBehavior Name="AnimationIdle" AnimationName="Idle" />
	//	    </cFAIMultiAITaskBehavior>
	//	    <cFAIMultiAITaskBehavior Name="Waddle" Probability="50" TargetAIBehavior="Waddle" NextAIName="Idle" >
	//	    	<cFAIWalkToDestinationAIBehavior Name="Waddle" DestPos="0,200,0" Speed="100" />
	//	    	<cFAIMPDIAnimationChangeAIBehavior Name="AnimationIdle" AnimationName="Idle" />
	//	    </cFAIMultiAITaskBehavior>
	//	</cFAIMachine>
	cFAIMachine::cFAIMachine(TiXmlElement*e_pTiXmlElement)
	{
		m_bStopWhileNoAI = false;
        //m_pTopRequestBehavior = 0;
        m_pLastConditionDoneBehavior = 0;
		m_iTotalProbability = 0;
		ASSERT_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pTiXmlElement,cFAIMachine::TypeID);
		cFAICharacterInterface*l_pFAICharacterInterface = 0;
		std::wstring    l_strAIMachineName;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Name")
			{
				l_strAIMachineName = l_strValue;
			}
			else
			//COMPARE_NAME("Count")
			//{
			//	l_iCount = VALUE_TO_INT;
			//}
			//else
			COMPARE_NAME("StartBehaviorName")
			{
				m_strStartBehaviorName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cFAICharacterInterface::TypeID)
			{
				cFAICharacterInterface*l_pTargetFAICharacterInterface = g_pFAIBehaviorParser->m_FAICharacterInterfaceList.GetObject(l_strValue);
				if( l_pTargetFAICharacterInterface )
				{
					l_pFAICharacterInterface = l_pTargetFAICharacterInterface;
				}
				else
				{
					UT::ErrorMsg(L"there is no such cFAICharacterInterface",l_strValue);
				}
			}
			else
			COMPARE_NAME("StopWhileNoAI")
			{
				m_bStopWhileNoAI = VALUE_TO_BOOLEAN;
			}
		PARSE_NAME_VALUE_END
		SetName(l_strAIMachineName.c_str());
		TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while( l_pChildTiXmlElement )
		{
			cFAIBaseBehave*l_pBaseAIBehave = cFAIBehaviorParser::ProcessAIBehavior(l_pChildTiXmlElement);
			if( l_pBaseAIBehave )
			{
				bool    l_b2 = AddObject(l_pBaseAIBehave);
				if( !l_b2 )
				{
					UT::ErrorMsg(L"same name",l_pBaseAIBehave->GetName());
					SAFE_DELETE(l_pBaseAIBehave);
				}
			}
			else
				UT::ErrorMsg(L"AI cant' find!",(WCHAR*)l_pChildTiXmlElement->Value());
			l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
		}
		if( l_pFAICharacterInterface )
		{
			SetSrcCharacter(l_pFAICharacterInterface);
		}
		InitProbability();
#ifdef WIN32
		int	l_iCount = 0;
		l_iCount = Count();
		for( int i=0;i<l_iCount;++i  )
		{
			const WCHAR*l_strNextAIBehaviorName = GetObject(i)->GetNextAIBehaviorName();
			if( l_strNextAIBehaviorName && wcslen(l_strNextAIBehaviorName) )
			{
				if( !GetObject(l_strNextAIBehaviorName) )
					UT::ErrorMsg(L"AIBehavior can't find",(WCHAR*)l_strNextAIBehaviorName);                
			}
		}
#endif
		//SetCurrentWorkingObject(m_strStartBehaviorName.c_str());
		//if(!GetCurrentWorkingObject())
		//	UT::ErrorMsg(L"start AI cant' find!",(WCHAR*)l_pChildTiXmlElement->Value());
	}

	cFAIMachine::cFAIMachine(cFAIMachine*e_pAIMachine)
	{
		m_strStartBehaviorName = e_pAIMachine->m_strStartBehaviorName;
		this->m_pSrcCharacter = dynamic_cast<cFAICharacterInterface*>(e_pAIMachine->m_pSrcCharacter->Clone());
		m_bStopWhileNoAI = e_pAIMachine->m_bStopWhileNoAI;
		m_iTotalProbability = e_pAIMachine->m_iTotalProbability;
        //m_pTopRequestBehavior = 0;
        m_pLastConditionDoneBehavior = 0;
   //     if( e_pAIMachine->m_pTopRequestBehavior )
   //     {
   //         m_pTopRequestBehavior = dynamic_cast<cFAIBaseBehave*>(e_pAIMachine->m_pTopRequestBehavior->Clone());
			//m_pTopRequestBehavior->SetSrcCharacter(this->m_pSrcCharacter);
   //     }
        int l_iCount = e_pAIMachine->Count();
        for( int i=0;i<l_iCount;++i  )
        {
            cFAIBaseBehave*l_pBaseAIBehave = e_pAIMachine->GetObject(i);
            cFAIBaseBehave*l_pNewBaseAIBehave = dynamic_cast<cFAIBaseBehave*>(l_pBaseAIBehave->Clone());
			l_pNewBaseAIBehave->SetSrcCharacter(this->m_pSrcCharacter);
            this->AddObjectNeglectExist(l_pNewBaseAIBehave);
        }
        cFAIBaseBehave*l_pBaseAIBehave = e_pAIMachine->GetCurrentWorkingObject();
        if( l_pBaseAIBehave )
        {
            SetCurrentWorkingObject(l_pBaseAIBehave->GetName());
            assert(GetCurrentWorkingObject());
        }	
	}

    cFAIMachine::~cFAIMachine()
    {
		SAFE_DELETE(m_pSrcCharacter);
		//SAFE_DELETE(m_pAICharacterInterface);
        //SAFE_DELETE(m_pTopRequestBehavior);
    }

    void    cFAIMachine::InternalInit()
    {
        //int l_iCount = this->Count();
        //for( int i=0;i<l_iCount;++i )
        //{
        //    this->GetObject(i)->Init();
        //}
		if( wcslen(m_strStartBehaviorName.c_str())>0 )
		{
			SetCurrentWorkingObject(m_strStartBehaviorName.c_str());
		}
		else
		{
			cFAIBaseBehave*l_p = GetRandomAIBehavior();
			if( l_p )
			{
				SetCurrentWorkingObject(l_p->GetName());
				l_p->Update(0.f);
			}
		}
    }

    cFAIBaseBehave* cFAIMachine::GetRandomAIBehavior()
    {
        cFAIBaseBehave*l_pCurrentWorkingObject = 0;
		int	l_iIndex = 0;
		if( m_iTotalProbability != 0 )
		{
			int	l_iValue  = rand()%this->m_iTotalProbability;
			int	l_iTotalValue = 0;
			for( int i=0;i<this->Count();++i )
			{
				l_iTotalValue += (*this)[i]->GetProbability();
				if( l_iTotalValue > l_iValue )
				{
					l_iIndex = i;
#ifdef DEBUG
					if( (*this)[i]->GetProbability() == 0 )
					{//should not be here!
						int a=0;
					}
#endif
					break;
				}
			}		
		}
		l_pCurrentWorkingObject = (*this)[l_iIndex];
        assert(l_pCurrentWorkingObject);
        return l_pCurrentWorkingObject;
    }

    void    cFAIMachine::InternalUpdate(float e_fElpaseTime)
    {
        if( !m_pCurrentWorkingObject )
		{
			this->m_bSatisfiedCondition = true;
            return;
		}
        m_pCurrentWorkingObject->Update(e_fElpaseTime);
        //AI behavior done
        if( m_pCurrentWorkingObject->IsSatisfiedCondition() )
        {//directly to get next name
            m_pLastConditionDoneBehavior = m_pCurrentWorkingObject;
            cFAIBaseBehave*l_pCurrentWorkingObject = m_pCurrentWorkingObject;
            if( m_pCurrentWorkingObject->GetNextAIBehaviorName() )
            {
                m_pCurrentWorkingObject = this->GetObject(m_pCurrentWorkingObject->GetNextAIBehaviorName());
                assert(m_pCurrentWorkingObject);
            }
            else
            {//directly from probability
				if( this->Count() >1 && !m_bStopWhileNoAI )
				{
					//while( m_pCurrentWorkingObject->IsKillAfterDone()&&m_pCurrentWorkingObject == l_pCurrentWorkingObject )
					{
						m_pCurrentWorkingObject = GetRandomAIBehavior();
					}
				}
				else
				{
					m_pCurrentWorkingObject = 0;
				}
            }
			if( m_pCurrentWorkingObject )
			{
				//m_pCurrentWorkingObject->Start(this->m_pData);
				m_pCurrentWorkingObject->Init();
				if(l_pCurrentWorkingObject->IsKillAfterDone())
				{
					if( l_pCurrentWorkingObject == m_pCurrentWorkingObject )
						m_pCurrentWorkingObject = 0;
					this->RemoveObject(l_pCurrentWorkingObject->GetName());
					InitProbability();
				}
			}
            //assert(m_pCurrentWorkingObject);
        }
    }
    //not necessary,it might helpful for debug mode
    void    cFAIMachine::Render()
    {
        if( m_pCurrentWorkingObject )
            m_pCurrentWorkingObject->Render();
    }

	void    cFAIMachine::DebugRender()
	{
		if( m_pCurrentWorkingObject )
		{
			Vector3	l_vPos = GetSrcCharacter()->GetWorldPosition();
			//cGameApp::m_spGlyphFontRender->SetScale(0.5f);
			cGameApp::m_spGlyphFontRender->SetColor(Vector4::Red);
			cGameApp::m_spGlyphFontRender->RenderFont(l_vPos.x,l_vPos.y-30,m_pCurrentWorkingObject->GetName());
			//cGameApp::m_spGlyphFontRender->SetScale(1.f);
			cGameApp::m_spGlyphFontRender->SetColor(Vector4::One);
		}
	}

	void    cFAIMachine::Init()
	{
		cFAIBaseBehave::Init();
		//cFMWorkingObjectChanger<cFAIBaseBehave>::Init();
	}

    void    cFAIMachine::InitProbability()
    {
        int l_iCount = this->Count();
        for( int i=0;i<l_iCount;++i )
        {
            m_iTotalProbability += (*this)[i]->GetProbability();
        }
    }

	void	cFAIMachine::SetSrcCharacter(cFAICharacterInterface*e_pAICharacterInterface)
	{
		SAFE_DELETE(this->m_pSrcCharacter);
		if( e_pAICharacterInterface )
		{
			m_pSrcCharacter = dynamic_cast<cFAICharacterInterface*>(e_pAICharacterInterface->Clone());
		}
		int l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			this->GetObject(i)->SetSrcCharacter(m_pSrcCharacter);
		}
	}

	bool	cFAIMachine::IsDone()
	{
		return this->m_pCurrentWorkingObject?false:true;
	}

	bool	cFAIMachine::IsBehaviorChanged()
	{
		if( this->m_pCurrentWorkingObject )
		{
			if( m_pLastConditionDoneBehavior != m_pCurrentWorkingObject )
				return true;
		}
		return false;
	}
}