#include "../stdafx.h"
#include "EventVariable.h"
#include "EventManager.h"
#include "EventObjecInstanceStatusChangeActiver.h"
#include "EventPackageRunner.h"

namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cEventObjectInstanceStatusChangeActiver);
	cEventObjectInstanceStatusChangeActiver::cEventObjectInstanceStatusChangeActiver(TiXmlElement*e_pTiXmlElement)
	{
		m_pstrStatusName = 0;
		m_pStatusNameByVariable = 0;
		std::wstring	l_strStatusName;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventObjectInstanceStatusChangeActiver::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_STATUS_NAME)
			{
				m_pstrStatusName = new std::wstring;
				*m_pstrStatusName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventObjectInstance::TypeID)
			{
				m_strEventObjectInstanceName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
			{
				m_pStatusNameByVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
				if( !m_pStatusNameByVariable )
				{
					UT::ErrorMsg(L"variable is not exist!",l_strValue);
				}
			}
		PARSE_NAME_VALUE_END
		
	}
	//cEventObjectInstanceStatusChangeActiver::cEventObjectInstanceStatusChangeActiver(const WCHAR*e_strTargetStatusName,const WCHAR*e_strEventObjectInstanceName)
	//{
	//	m_strStatusName = e_strTargetStatusName;
	//	m_strEventObjectInstanceName = e_strEventObjectInstanceName;
	//	m_pStatusNameByVariable = ;
	//}

	cEventObjectInstanceStatusChangeActiver::cEventObjectInstanceStatusChangeActiver(cEventObjectInstanceStatusChangeActiver*e_pEventObjectInstanceStatusChangeActiver)
	{
		m_pstrStatusName = 0;
		if( e_pEventObjectInstanceStatusChangeActiver->m_pstrStatusName )
		{
			m_pstrStatusName = new std::wstring;
			*m_pstrStatusName = *e_pEventObjectInstanceStatusChangeActiver->m_pstrStatusName;
		}
		m_strEventObjectInstanceName = e_pEventObjectInstanceStatusChangeActiver->m_strEventObjectInstanceName;
		m_pStatusNameByVariable = e_pEventObjectInstanceStatusChangeActiver->m_pStatusNameByVariable;
	}

	cEventObjectInstanceStatusChangeActiver::~cEventObjectInstanceStatusChangeActiver()
	{
		SAFE_DELETE(m_pstrStatusName);
	}

	void			cEventObjectInstanceStatusChangeActiver::InternalInit()
	{
		cEventObjectInstance*l_pEventObjectInstance = g_pCurrentEventManager->GetEventObjectInstanceFromCurrentEventPackage(m_strEventObjectInstanceName.c_str());
		if( !l_pEventObjectInstance )
		{
			UT::ErrorMsg(m_strEventObjectInstanceName.c_str(),L"EventObjectInstance is not exit!!");
		}
		else
		{
			int	l_iIndex = -1;
			if( m_pStatusNameByVariable )
			{
				l_iIndex = l_pEventObjectInstance->GetObjectIndexByName(m_pStatusNameByVariable->ConvertToString().c_str());
				if( l_iIndex == -1 )
				{
					UT::ErrorMsg(m_pStatusNameByVariable->GetName(),L"EventObjectInstance doesn't contain such status name!!");
				}
			}
			else
			{
				l_iIndex = l_pEventObjectInstance->GetObjectIndexByName(m_pstrStatusName->c_str());
				if( l_iIndex == -1 )
				{
					UT::ErrorMsg(m_pstrStatusName->c_str(),L"EventObjectInstance doesn't contain such status name!!");
				}
			}
		}
	}

	void			cEventObjectInstanceStatusChangeActiver::InternalUpdate(float e_fElpasedTime)
	{
		cEventObjectInstance*l_pEventObjectInstance = g_pCurrentEventManager->m_pEventPackageRunner->GetCurrentEventPackage()->GetObject(m_strEventObjectInstanceName.c_str());
		if( l_pEventObjectInstance )
		{
			int	l_iIndex = -1;
			if( m_pStatusNameByVariable )
			{
				l_iIndex = l_pEventObjectInstance->GetObjectIndexByName(m_pStatusNameByVariable->ConvertToString().c_str());			
			}
			else
			{
				l_iIndex = l_pEventObjectInstance->GetObjectIndexByName(m_pstrStatusName->c_str());
			}
			if( l_iIndex != -1 )
				l_pEventObjectInstance->SetCurrentWorkingObject(l_iIndex);
		}
		this->m_bSatisfiedCondition = true;
	}

	void			cEventObjectInstanceStatusChangeActiver::InternalRender()
	{
		int a=0;
	}
	//<cEventObjectInstanceStatusChangeActiver cEventObjectInstance="AuditMenu" cEventVariable="AuditMenuSelectedIndex" />
	//<cEventObjectInstanceStatusChangeActiver cEventObjectInstance="GeneratorTest" StatusName="Hello" />
	TiXmlElement*		cEventObjectInstanceStatusChangeActiver::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		l_pEvent->SetAttribute(cEventObjectInstance::TypeID,m_strEventObjectInstanceName.c_str());
		if(m_pStatusNameByVariable)
		{
			l_pEvent->SetAttribute(cEventVariable::TypeID,m_pStatusNameByVariable->GetName());
		}
		if( m_pstrStatusName )
		{
			l_pEvent->SetAttribute(EVENT_STATUS_NAME,m_pstrStatusName->c_str());
		}
		return l_pEvent;
	}
//end namespace FATMING_CORE
}