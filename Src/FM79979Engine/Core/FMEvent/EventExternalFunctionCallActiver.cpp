#include "../stdafx.h"
#include "EventBase.h"
#include "EventVariable.h"
#include "EventExternalFunctionCallActiver.h"
#include "EventManager.h"

namespace FATMING_CORE
{
	//<cEventExternalFunctionCallActiver	Function="" />
	TYPDE_DEFINE_MARCO(cEventExternalFunctionCallActiver);
	cEventExternalFunctionCallActiver::cEventExternalFunctionCallActiver(TiXmlElement*e_pTiXmlElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventExternalFunctionCallActiver::TypeID)	
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Function")
			{
				m_strExternamFunctionName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				SetName(l_strValue);
			}
		PARSE_NAME_VALUE_END
	}

	cEventExternalFunctionCallActiver::cEventExternalFunctionCallActiver(cEventExternalFunctionCallActiver*e_pEventExternalFunctionCallActiver)
	{
		m_strExternamFunctionName = e_pEventExternalFunctionCallActiver->m_strExternamFunctionName;
	}

	cEventExternalFunctionCallActiver::~cEventExternalFunctionCallActiver()
	{
	
	}

	void	cEventExternalFunctionCallActiver::InternalInit()
	{
		cExternalFunction*l_pExternalFunction = g_pCurrentEventManager->m_pExternalFunctionList->GetObject(m_strExternamFunctionName.c_str());
		if( !l_pExternalFunction )
		{
			UT::ErrorMsg(m_strExternamFunctionName.c_str(),L"cann't find external cEventExternalFunctionCallActiver function!");
		}
	}

	void	cEventExternalFunctionCallActiver::InternalUpdate(float e_fElpasedTime)
	{
		cExternalFunction*l_pExternalFunction = g_pCurrentEventManager->m_pExternalFunctionList->GetObject(m_strExternamFunctionName.c_str());
		if( l_pExternalFunction )
		{
			NamedTypedObject*l_pParent = this->GetOwner();
			const WCHAR*l_str = l_pParent ->Type();
			if( l_pParent && l_pParent->GetOwner() )
			{
				l_pParent = l_pParent->GetOwner();
				l_str = l_pParent ->Type();
			}
			if( l_pParent )
			{
				l_pExternalFunction->m_pDoButtonGoal_Callback(l_pParent);
			}
			else
				l_pExternalFunction->m_pDoButtonGoal_Callback(0);
		}
		this->m_bSatisfiedCondition = true;	
	}

	TiXmlElement*		cEventExternalFunctionCallActiver::ToTiXmlElement()
	{
		TiXmlElement*l_pElement = this->GetMyTiXmlElement();
		l_pElement->SetAttribute(L"Function",m_strExternamFunctionName.c_str());
		return l_pElement;
	}
}