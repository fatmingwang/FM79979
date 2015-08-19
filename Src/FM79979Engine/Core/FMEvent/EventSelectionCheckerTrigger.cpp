#include "../stdafx.h"
#include "EventButtonTrigger.h"
#include "EventSelectionCheckerTrigger.h"
#include "EventPackageRunner.h"
#include "EventManager.h"
namespace FATMING_CORE
{
	const  wchar_t*        cEventSelectionCheckerTrigger::TypeID = L"cEventSelectionCheckerTrigger";
	cEventSelectionCheckerTrigger::cEventSelectionCheckerTrigger(TiXmlElement*e_pTiXmlElement)
	{
		m_bKeepDoingUntilSameResult = false;
		m_pOutputResultVariable = 0;
		m_pCompareResultVariable = 0;
		m_bShowHint = false;
		m_vHintPos = Vector3::Zero;
		m_iClickCount = 0;
		//not using here
		m_pIndexVariable = 0;
		m_pPrefixButton = 0;
		m_bStayAtLastSelect = false;
		std::wstring	l_strPrefixButtonName;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventSelectionCheckerTrigger::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				SetName(l_strValue);
			}
			else
			COMPARE_NAME("StayAtLastSelect")
			{
				m_bStayAtLastSelect = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME_WITH_DEFINE(BUTTON_SELECTION_TYPE)
			{
				m_eButtonSelectionType = StringToButtonSelectionType(l_strValue);	
				if( m_eButtonSelectionType == eBST_ALL_CLICK )
				{
					UT::ErrorMsg(L"FUCK",L"I am lazy to support cEventMultiButton AllClick now lah...FUCK");
				}
			}
			else
			COMPARE_NAME("PrefixButtonName")
			{
				l_strPrefixButtonName = l_strValue;
			}
			//else
			//COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
			//{
			//	m_pIndexVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
			//	if( !m_pIndexVariable )
			//	{
			//		UT::ErrorMsg(l_strValue,L"there is not this variable");
			//	}
			//}
			else
			COMPARE_NAME("ClickCount")
			{
				m_iClickCount = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("CompareResultVariable")
			{
				m_pCompareResultVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
			}
			else
			COMPARE_NAME("OutputResultVariable")
			{
				m_pOutputResultVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
			}
			else
			COMPARE_NAME("HintPos")
			{
				m_bShowHint = true;
				m_vHintPos = GetVector2(l_strValue);
			}
			else
			COMPARE_NAME("KeepDoingUntilSameResult")
			{
				m_bKeepDoingUntilSameResult = VALUE_TO_BOOLEAN;
			}
		PARSE_NAME_VALUE_END
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while( e_pTiXmlElement )
		{
			cEventBase*l_pEventBase = g_pCurrentEventManager->GetTriggerEvent(e_pTiXmlElement);
			bool	l_bIsPrefix = false;
			if( l_pEventBase )
			{
				if( wcslen(l_strPrefixButtonName.c_str()) )
				{
					if( !wcscmp(l_strPrefixButtonName.c_str(),l_pEventBase->GetName()) )
						l_bIsPrefix = true;
				}
				if( l_bIsPrefix )
				{
					assert( m_pPrefixButton == 0 && "m_pPrefixButton is exists!" );
					m_pPrefixButton = l_pEventBase;
				}
				else
					this->AddObjectNeglectExist(l_pEventBase);
			}
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
		m_bCheckOutOfWorking = false;
	}

	cEventSelectionCheckerTrigger::cEventSelectionCheckerTrigger(cEventSelectionCheckerTrigger*e_pEventSelectionCheckerTrigger):cEventMultiButton(e_pEventSelectionCheckerTrigger)
	{
		m_bShowHint = e_pEventSelectionCheckerTrigger->m_bShowHint;
		m_vHintPos = e_pEventSelectionCheckerTrigger->m_vHintPos;
		m_pCompareResultVariable = e_pEventSelectionCheckerTrigger->m_pCompareResultVariable;
		m_pOutputResultVariable = e_pEventSelectionCheckerTrigger->m_pOutputResultVariable;
		m_bKeepDoingUntilSameResult = e_pEventSelectionCheckerTrigger->m_bKeepDoingUntilSameResult;
	}

	cEventSelectionCheckerTrigger::~cEventSelectionCheckerTrigger()
	{
	
	}

	void		cEventSelectionCheckerTrigger::InternalRender()
	{
		cEventMultiButton::InternalRender();
		if( m_bShowHint )
		{
			int	l_iCount = (int)this->m_PressedEventButtonList.size();
			std::wstring	l_str;
			if( l_iCount  )
			{
				for(int i=0;i<l_iCount;++i  )
					l_str += L"*";
				g_pCurrentEventManager->m_pEventFont->RenderFont(m_vHintPos.x,m_vHintPos.y,l_str.c_str());
			}
			else
			{
				//Vector2	l_vSize = g_pCurrentEventManager->m_pEventFont->GetRenderSize(L"please input password");
				//g_pCurrentEventManager->m_pEventFont->RenderFont(m_vHintPos.x-l_vSize.x/4,m_vHintPos.y,L"please input password");
			}
		}
	}

	void		cEventSelectionCheckerTrigger::InternalUpdate(float e_fElpaseTime)
	{
		cEventMultiButton::InternalUpdate(e_fElpaseTime);
		if( m_pCompareResultVariable )
		{
			if(this->m_bSatisfiedCondition)
			{
				std::wstring	l_str = m_pCompareResultVariable->ConvertToString();
				std::wstring	l_strPressedResult;
				int	l_iCount = (int)this->m_PressedEventButtonList.size();
				for(int i=0;i<l_iCount;++i  )
					l_strPressedResult += m_PressedEventButtonList[i]->GetName();
				if( m_pOutputResultVariable )
					m_pOutputResultVariable->SetDataByString(l_strPressedResult.c_str());
				if(wcscmp(l_strPressedResult.c_str(),l_str.c_str()))
				{
					if( m_bKeepDoingUntilSameResult )
					{
						m_bSatisfiedCondition = false;
						this->Init();
					}
				}

			}
		}
	}
	//<cEventSelectionCheckerTrigger Name="79979" ButtonSelectionType="Repeat" StayAtLastSelect="0" ClickCount="5" HintPos="230,560" CompareResultVariable="VendorPassword" OutputResultVariable="InputPassword" KeepDoingUntilSameResult="0" PrefixButtonName="Prefix"   >
	//	<cEventButton Name="Prefix" Text="請輸入密碼" BGColor="0.3,0.3,0.0,1" Pos="230,0,0" />
	//	<cEventButton Name="1" Text="停一" 		BGColor="1,0,0.5,1" Pos="260,100,0" KeyData="C" />
	//	<cEventButton Name="2" Text="停二" 		BGColor="1,0,0.5,1" Pos="260,200,0" KeyData="V"/>
	//	<cEventButton Name="3" Text="停三" 		BGColor="1,0,0.5,1" Pos="260,300,0" KeyData="B"/>
	//	<cEventButton Name="4" Text="停四" 		BGColor="1,0,0.5,1" Pos="260,400,0" KeyData="N"/>
	//	<cEventButton Name="5" Text="停五" 		BGColor="1,0,0.5,1" Pos="260,500,0" KeyData="M"/>
	//</cEventSelectionCheckerTrigger>
	TiXmlElement*		cEventSelectionCheckerTrigger::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		l_pEvent->SetAttribute(NAME,this->GetName());
		l_pEvent->SetAttribute(BUTTON_SELECTION_TYPE,ValueToString(this->m_eButtonSelectionType));
		l_pEvent->SetAttribute(L"StayAtLastSelect",this->m_bStayAtLastSelect?L"1":L"0");
		if(this->m_iClickCount != 0 )
		{
			l_pEvent->SetAttribute(L"ClickCount",this->m_iClickCount);
		}
		if( m_bShowHint )
		{
			l_pEvent->SetAttribute(L"HintPos",ValueToStringW(m_vHintPos));
		}
		if( m_pCompareResultVariable )
		{
			l_pEvent->SetAttribute(L"CompareResultVariable",m_pCompareResultVariable->GetName());
		}
		if( m_pOutputResultVariable )
		{
			l_pEvent->SetAttribute(L"OutputResultVariable",m_pOutputResultVariable->GetName());
		}
		if( m_bKeepDoingUntilSameResult )
		{
			l_pEvent->SetAttribute(L"KeepDoingUntilSameResult",1);
		}
		if(m_pPrefixButton)
		{
			l_pEvent->SetAttribute(L"PrefixButtonName",m_pPrefixButton->GetName());
			cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(m_pPrefixButton);
			l_pEvent->LinkEndChild(l_pEventButton->ToTiXmlElement());
		}
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(this->m_ObjectList[i]);
			l_pEvent->LinkEndChild(l_pEventButton->ToTiXmlElement());
		}
		return l_pEvent;
	}
//end namespace FATMING_CORE
}