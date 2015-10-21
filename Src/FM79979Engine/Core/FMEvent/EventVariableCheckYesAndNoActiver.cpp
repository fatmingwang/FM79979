#include "../stdafx.h"
#include "EventVariable.h"
#include "EventManager.h"
#include "EventVariableCheckYesAndNoActiver.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cEventVariableCheckYesAndNoActiver);

	cEventVariableCheckYesAndNoActiver::cEventVariableCheckYesAndNoActiver(TiXmlElement*e_pTiXmlElement)
	{
		m_YesEventList.SetOwner(this);
		m_NoEventList.SetOwner(this);
		m_pSrcEventVariable = 0;
		m_pCompareResultVariable = 0;
		m_pstrResult = 0;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventVariableCheckYesAndNoActiver::TypeID)
		PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("SrcEventVariable")
			{
				m_pSrcEventVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				SetName(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(L"Result")
			{
				m_pstrResult = new std::wstring;
				*m_pstrResult = l_strValue;
			}
			else
			COMPARE_NAME("CompareResultVariable")
			{
				m_pCompareResultVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
			}
		PARSE_NAME_VALUE_END
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while( e_pTiXmlElement )
		{
			TiXmlElement*l_pEventTiXmlElement = e_pTiXmlElement->FirstChildElement();
			bool	l_bYes = true;
			if( !wcscmp(e_pTiXmlElement->Value(),L"Yes") )
			{
				l_bYes = true;
			}
			else
			if( !wcscmp(e_pTiXmlElement->Value(),L"No") )
			{
				l_bYes = false;
			}
			else
			{
				UT::ErrorMsg(L"not yes nor no",L"cEventVariableCheckYesAndNoActiver data error");
			}
			while( l_pEventTiXmlElement )
			{
				cEventBase*l_pEventBase = 0;
				l_pEventBase = g_pCurrentEventManager->GetActiverEvent(l_pEventTiXmlElement);
				if( !l_pEventBase )
					l_pEventBase = g_pCurrentEventManager->GetTriggerEvent(l_pEventTiXmlElement);
				if( l_pEventBase )
				{
					if( l_bYes )
					{
						m_YesEventList.AddObjectNeglectExist(l_pEventBase);
					}
					else
					{
						m_NoEventList.AddObjectNeglectExist(l_pEventBase);
					}
				}
				l_pEventTiXmlElement = l_pEventTiXmlElement->NextSiblingElement();
			}
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
	}

	cEventVariableCheckYesAndNoActiver::cEventVariableCheckYesAndNoActiver(cEventVariableCheckYesAndNoActiver*e_pEventVariableCheckYesAndNoActiver)
	{
		m_YesEventList.SetOwner(this);
		m_NoEventList.SetOwner(this);
		m_pSrcEventVariable = e_pEventVariableCheckYesAndNoActiver->m_pSrcEventVariable;
		m_pCompareResultVariable = e_pEventVariableCheckYesAndNoActiver->m_pCompareResultVariable;
		m_pstrResult = 0;
		if( e_pEventVariableCheckYesAndNoActiver->m_pstrResult )
		{
			m_pstrResult = new std::wstring;
			*m_pstrResult = *e_pEventVariableCheckYesAndNoActiver->m_pstrResult;
		}
		m_YesEventList.CloneFromList(&e_pEventVariableCheckYesAndNoActiver->m_YesEventList);
		m_NoEventList.CloneFromList(&e_pEventVariableCheckYesAndNoActiver->m_NoEventList);
		//{ADD_CLONE_DATA_TO_NEW_VECTOR(cEventBase,(&e_pEventVariableCheckYesAndNoActiver->m_YesEventList),(&m_YesEventList));}
		//{ADD_CLONE_DATA_TO_NEW_VECTOR(cEventBase,(&e_pEventVariableCheckYesAndNoActiver->m_NoEventList),(&m_NoEventList));}
	}

	cEventVariableCheckYesAndNoActiver::~cEventVariableCheckYesAndNoActiver()
	{
		SAFE_DELETE(m_pstrResult);
	}

	void	cEventVariableCheckYesAndNoActiver::InternalInit()
	{
		LIST_DO_FOR_ALL_CHILDREN((&m_YesEventList),FetchData(cEventInstance::TypeID,this->m_pEventInstance));
		LIST_DO_FOR_ALL_CHILDREN((&m_NoEventList),FetchData(cEventInstance::TypeID,this->m_pEventInstance));
		m_YesEventList.Init();
		m_NoEventList.Init();
		if( !m_pSrcEventVariable )
		{
			UT::ErrorMsg(this->GetName(),L"cEventVariableCheckYesAndNoActiver m_pSrcEventVariable is not exists");
		}
		else
		{
			if( !m_pCompareResultVariable && !this->m_pstrResult )
				UT::ErrorMsg(this->GetName(),L"cEventVariableCheckYesAndNoActiver m_pCompareResultVariable and m_pstrResult both not exist");
		}
			
	}

	void	cEventVariableCheckYesAndNoActiver::InternalUpdate(float e_fElpasedTime)
	{
		this->m_bSatisfiedCondition = true;
		bool	l_bYes = true;
		if( m_pCompareResultVariable )
		{
			if( m_pSrcEventVariable )
				l_bYes = m_pCompareResultVariable->IsSame(this->m_pSrcEventVariable);
		}
		else
		{
			if( m_pSrcEventVariable && this->m_pstrResult )
			{
				l_bYes = m_pSrcEventVariable->IsSame(m_pstrResult->c_str());
			}
		}
		if( l_bYes )
			m_YesEventList.Update(e_fElpasedTime);
		else
			m_NoEventList.Update(e_fElpasedTime);
	}

	void	cEventVariableCheckYesAndNoActiver::InternalRender()
	{
	
	}
	//<cEventVariableCheckYesAndNoActiver SrcEventVariable="InputPassword" CompareResultVariable="VendorPassword" >
	//	<Yes>
	//		<cEventValueChangeActiver Name="Test" cEventVariable="CurrentPackageName" TargetValue="VendorMenu" />
	//	</Yes>
	//	<No>

	//	</No>
	//</cEventVariableCheckYesAndNoActiver>
	TiXmlElement*		cEventVariableCheckYesAndNoActiver::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		if(this->m_pSrcEventVariable)
		{
			l_pEvent->SetAttribute(L"SrcEventVariable",m_pSrcEventVariable->GetName());
		}
		if(this->m_pCompareResultVariable)
		{
			l_pEvent->SetAttribute(L"CompareResultVariable",m_pCompareResultVariable->GetName());
		}
		if(this->m_pstrResult)
		{
			l_pEvent->SetAttribute(L"Result",m_pstrResult->c_str());
		}
		//yes
		{
			TiXmlElement*l_pYes = new TiXmlElement(L"Yes");
			int	l_iCount = m_YesEventList.Count();
			for( int i=0;i<l_iCount;++i )
			{
				TiXmlElement*l_pChild = m_YesEventList[i]->ToTiXmlElement();
				l_pYes->LinkEndChild(l_pChild);
			}
			l_pEvent->LinkEndChild(l_pYes);
		}
		//no
		{
			TiXmlElement*l_pNo = new TiXmlElement(L"No");
			int	l_iCount = m_NoEventList.Count();
			for( int i=0;i<l_iCount;++i )
			{
				TiXmlElement*l_pChild = m_NoEventList[i]->ToTiXmlElement();
				l_pNo->LinkEndChild(l_pChild);
			}
			l_pEvent->LinkEndChild(l_pNo);
		}
		return l_pEvent;
	}
//end namespace FATMING_CORE
}