#include "../stdafx.h"
#include "EventVariable.h"
#include "EventManager.h"
#include "EventButtonSelectedActiver.h"
#include "EventButtonTrigger.h"
namespace FATMING_CORE
{
	//<cEventButtonSelectedActiver Name="79979" cEventVariable="CurrentPackageName" Result="SecondPage,ThirdPage,FourthPage"/>
	TYPDE_DEFINE_MARCO(cEventButtonSelectedActiver);
	cEventButtonSelectedActiver::cEventButtonSelectedActiver(TiXmlElement*e_pTiXmlElement)
	{
		m_pEventVariable = 0;
		if( !e_pTiXmlElement )
			return;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventButtonSelectedActiver::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
			{
				m_pEventVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME("Result")
			{
				std::wstring	l_str = l_strValue;
				m_strResult = GetWStringListByCommaDivide(l_str.c_str());
			}
		PARSE_NAME_VALUE_END
		if( !m_pEventVariable )
		{
			UT::ErrorMsg(L"Error",L"cEventButtonSelectedActiver no EventVariable");
		}
	}

	cEventButtonSelectedActiver::cEventButtonSelectedActiver(cEventButtonSelectedActiver*e_pEventMultiButtonSelectedActiver)
	{
		m_pEventVariable = e_pEventMultiButtonSelectedActiver->m_pEventVariable;
		m_strResult = e_pEventMultiButtonSelectedActiver->m_strResult;
	}

	cEventButtonSelectedActiver::~cEventButtonSelectedActiver()
	{
	
	}

	void		cEventButtonSelectedActiver::InternalInit()
	{
		if( !m_pEventInstance )
		{
			UT::ErrorMsg(L"no eventinstance",this->GetName());
			return;
		}
		cEventInstance*l_pEventInstance = m_pEventInstance;
		if( !l_pEventInstance )
		{//parent is not EventInstance
			return;
		}
		int	l_iCount = l_pEventInstance->m_TriggerEventList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			if( l_pEventInstance->m_TriggerEventList[i]->Type() == cEventMultiButton::TypeID )
			{
				cEventMultiButton*l_pEventMultiButton = (cEventMultiButton*)l_pEventInstance->m_TriggerEventList[i];
				if(l_pEventMultiButton->GetButtonSelectionType() != eBST_ONE)
				{
					if( m_pEventVariable )
					{
						if(m_pEventVariable->GetDataType() != eDT_WSTRING)
						{
							UT::ErrorMsg(m_pEventVariable->GetName(),L"selection mode is not click one but event variable data 's type is not wstring!?");
						}
					}
				}
				int	l_iEventMultiButtonCount = l_pEventMultiButton->Count();
				if( (int)m_strResult.size() < l_iEventMultiButtonCount || l_iEventMultiButtonCount == 0 )
				{
					UT::ErrorMsg(cEventButtonSelectedActiver::TypeID,L"Selected result data error");
				}
				if( l_pEventMultiButton->IsStayAtLastSelect() )
				{
					std::wstring	l_strVariableDataString = this->m_pEventVariable->ConvertToString();
					int	l_iCount = (int)this->m_strResult.size();
					for( int i=0;i<l_iCount;++i )
					{
						if(!wcscmp(m_strResult[i].c_str(),l_strVariableDataString.c_str()))
						{
							cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(l_pEventMultiButton->GetObject(i));
							l_pEventButton->SetBGColor(l_pEventMultiButton->GetSelectedColor());
							break;
						}
					}
				}
				return;
			}
		}	
	}

	void		cEventButtonSelectedActiver::InternalUpdate(float e_fElpasedTime)
	{
		cEventInstance*l_pEventInstance = this->m_pEventInstance;
		if(!l_pEventInstance )
		{//what whever parent it is just setup result as well.
			const WCHAR*l_strResult = 0;
			if( m_strResult.size() >0 )
			{
				l_strResult = m_strResult[0].c_str();
			}
			if( l_strResult )
			{
				if( m_pEventVariable )
					m_pEventVariable->SetDataByString(l_strResult);
#ifdef DEBUG
				else
				{
					UT::ErrorMsg(L"Error",L"cEventButtonSelectedActiver no EventVariable");
				}
#endif
			}
			this->m_bSatisfiedCondition = true;
			return;
		}
		int	l_iCount = l_pEventInstance->m_TriggerEventList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			if( l_pEventInstance->m_TriggerEventList[i]->Type() == cEventMultiButton::TypeID )
			{
				cEventMultiButton*l_pEventMultiButton = (cEventMultiButton*)l_pEventInstance->m_TriggerEventList[i];
				if(l_pEventMultiButton->GetButtonSelectionType() == eBST_ONE)
				{
					if( m_strResult.size() != l_pEventMultiButton->Count() )
						continue;
					int	l_iIndex = l_pEventMultiButton->GetCurrentWorkingObjectIndex();
					const WCHAR*l_strResult = 0;
					if((int)m_strResult.size() >= l_iIndex)
					{
						if( l_iIndex != -1 )
						{
							l_strResult = m_strResult[l_iIndex].c_str();
						}
					}
					else
					{
						cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(l_pEventMultiButton->GetObject(l_iIndex));
						if( wcslen(l_pEventButton->GetResult()) )
						{
							l_strResult = l_pEventButton->GetResult();
						}
					}
					if( l_strResult )
						m_pEventVariable->SetDataByString(l_strResult);
					if( !l_strResult || l_iIndex == -1 )
					{
						UT::ErrorMsg(cEventButtonSelectedActiver::TypeID,L"Selected result data error");
					}
				}
				else
				if(l_pEventMultiButton->GetButtonSelectionType() == eBST_ALL_PRESSED)
				{
					int	l_iCount = (int)l_pEventMultiButton->Count();
					int	l_iResultCount = (int)m_strResult.size();
					std::wstring	l_strResult;
					if( l_iCount )
					{
						int	l_iIndex = 0;
						l_strResult += this->m_strResult[l_iIndex].c_str();
					}
					m_pEventVariable->SetDataByString(l_strResult.c_str());				
				}
				else
				{
					int	l_iCount = (int)l_pEventMultiButton->m_PressedEventButtonList.size();
					int	l_iResultCount = (int)m_strResult.size();
					std::wstring	l_strResult;
					for( int j=0;j<l_iCount;++j )
					{
						int	l_iIndex = l_pEventMultiButton->GetObjectIndexByPointer(l_pEventMultiButton->m_PressedEventButtonList[j]);
						if( l_iIndex != -1 && l_iIndex<l_iResultCount )
						{
							l_strResult += this->m_strResult[l_iIndex].c_str();
							//if( j != l_iCount-1 )
							//	l_strResult += L",";
						}
					}
					m_pEventVariable->SetDataByString(l_strResult.c_str());
				}
				this->m_bSatisfiedCondition = true;
				return;
			}
			else
			if( l_pEventInstance->m_TriggerEventList[i]->Type() == cEventButton::TypeID )
			{
				cEventButton*l_pEventButton = (cEventButton*)l_pEventInstance->m_TriggerEventList[i];
				const WCHAR*l_strResult = 0;
				if( m_strResult.size() >0 )
				{
					l_strResult = m_strResult[0].c_str();
				}
				else
				if( wcslen(l_pEventButton->GetResult()) )
				{
					l_strResult = l_pEventButton->GetResult();
					
				}
				if( l_strResult )
				{
					m_pEventVariable->SetDataByString(l_strResult);
					this->m_bSatisfiedCondition = true;
				}
				else
				{
					UT::ErrorMsg(cEventButton::TypeID,L"Selected result data error");
				}
			}
			else			
			{//force to do
				m_pEventVariable->SetDataByString(m_strResult[0].c_str());
				this->m_bSatisfiedCondition = true;
			}
		}
		this->m_bSatisfiedCondition = true;
	}

	void		cEventButtonSelectedActiver::InternalRender()
	{
	
	}
	//<cEventButtonSelectedActiver Name="79979" cEventVariable="CurrentPackageName" Result="SecondPage,ThirdPage,FourthPage"/>
	TiXmlElement*		cEventButtonSelectedActiver::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		if( m_pEventVariable )
		{
			l_pEvent->SetAttribute(cEventVariable::TypeID,m_pEventVariable->GetName());
		}
		std::wstring	l_strResult;
		size_t	l_uiSize = m_strResult.size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			l_strResult = m_strResult[i];
			if( i != l_uiSize-1 )
			{
				l_strResult += L",";
			}
		}
		l_pEvent->SetAttribute(L"Result",l_strResult.c_str());
		return l_pEvent;
	}
//end namespace FATMING_CORE
}