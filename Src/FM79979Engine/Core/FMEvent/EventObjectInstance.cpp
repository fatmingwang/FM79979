#include "../stdafx.h"
#include "EventObjectInstance.h"
#include "EventVariable.h"
#include "EventManager.h"
namespace   FATMING_CORE
{
	const wchar_t*     cEventObjectInstance::TypeID( L"cEventObjectInstance" );
	const wchar_t*     cEventObjectStatus::TypeID( L"cEventObjectStatus" );

	cEventObjectStatus::cEventObjectStatus()
	{
		m_pStatusEventVariable = 0;
		m_iStartStatusIndex = 0;
	}

	cEventObjectStatus::cEventObjectStatus(cEventObjectStatus*e_pEventObjectStatus)
		:cFMWorkingObjectChanger<cObjectAndName<cEventObject> >(e_pEventObjectStatus)
	{
		m_pStatusEventVariable = e_pEventObjectStatus->m_pStatusEventVariable;
		m_iStartStatusIndex = e_pEventObjectStatus->m_iStartStatusIndex;
	}

	void	cEventObjectStatus::Update(float e_fElpaseTime)
	{
		if( this->m_pStatusEventVariable )
		{
			if( m_pCurrentWorkingObject )
			{
				std::wstring	l_strVariableName = m_pStatusEventVariable->ConvertToString();
				int	l_iIndex = this->GetObjectIndexByName(l_strVariableName.c_str());
				if( l_iIndex != -1 )
				{
					if(wcscmp(m_pCurrentWorkingObject->GetName(),l_strVariableName.c_str()))
					{
						this->SetCurrentWorkingObject(l_iIndex);
					}
				}
			}
			//this->gtobje
		}
		if( m_pCurrentWorkingObject )
			m_pCurrentWorkingObject->Update(e_fElpaseTime); 
	}

	cEventObjectInstance::cEventObjectInstance()
	{
		m_pCurrentEventObjectStatusData = 0;
		m_pEventInstanceList = new cClickMouseBehaviorVector<cEventInstance>;
		m_pTargetEventObjectStatus = 0;
		m_pEventInstanceList->SetOwner(this);
		m_bAllStatusWorking = false;
		m_iStartStatusIndex = 0;
		m_pCurrentEventInstance = 0;
	}

	cEventObjectInstance::cEventObjectInstance(TiXmlElement*e_pTiXmlElement)
	{
		m_pCurrentEventObjectStatusData = 0;
		m_pCurrentEventInstance = 0;
		m_pEventInstanceList = new cClickMouseBehaviorVector<cEventInstance>;
		m_pTargetEventObjectStatus = 0;
		m_pEventInstanceList->SetOwner(this);
		m_bAllStatusWorking = false;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventObjectInstance::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				SetName(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventInstance::TypeID)
			{
				if(!ProcessEventInstance(l_strValue))
				{
					UT::ErrorMsg(L"there is no cEventInstance",l_strValue);
				}
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventObjectStatus::TypeID)
			{
				if( !ProcessEventObjectStatus(l_strValue) )
					UT::ErrorMsg(L"there is no cEventObjectStatus",l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_OBJECTINSTANCE_ALL_STATUS_WORJING)
			{
				m_bAllStatusWorking = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_OBJECT_STATUS_VARIABLE)
			{
				if( !ProcessStatusEventVariable(l_strValue) )
				{
					UT::ErrorMsg(L"GetEventObjectInstance there is no cEventVariable",l_strValue);
				}
			}
			else
			COMPARE_NAME("StartStatus")
			{
				this->m_iStartStatusIndex = m_pTargetEventObjectStatus->GetObjectIndexByName(l_strValue);
			}
		PARSE_NAME_VALUE_END
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		int	l_iTargetCurrentEventInstance = 0;
		int	l_iIndex = 0;
		while( e_pTiXmlElement )
		{
			ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventObjectInstance::TypeID);
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME_WITH_DEFINE(NAME)
				{
					if(!ProcessEventInstance(l_strValue))
					{
						UT::ErrorMsg(L"there is no cEventInstance",l_strValue);
					}
				}
				else
				COMPARE_NAME("StartEventInstance")
				{
					if( VALUE_TO_INT == 1 )
						l_iTargetCurrentEventInstance = l_iIndex;
				}
			PARSE_NAME_VALUE_END
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
			++l_iIndex;
		}
		if( m_pEventInstanceList->Count() >= 1)
		{
			m_pCurrentEventInstance = m_pEventInstanceList->GetObject(l_iTargetCurrentEventInstance);
		}
	}

	cEventObjectInstance::cEventObjectInstance(cEventObjectInstance*e_pEventObjectInstance):cEventObjectStatus(e_pEventObjectInstance)
	{
		m_pCurrentEventInstance = 0;
		m_pTargetEventObjectStatus = e_pEventObjectInstance->m_pTargetEventObjectStatus;
		int	l_iCount = e_pEventObjectInstance->Count();
		m_pEventInstanceList = dynamic_cast<cClickMouseBehaviorVector<cEventInstance>*>(e_pEventObjectInstance->Clone());
		m_pEventInstanceList->SetOwner(this);
		assert(e_pEventObjectInstance->m_pCurrentEventInstance&&"no current event instance!");
		int	l_iIndex = e_pEventObjectInstance->m_pEventInstanceList->GetObjectIndexByPointer(e_pEventObjectInstance->m_pCurrentEventInstance);
		m_pCurrentEventInstance = m_pEventInstanceList->GetObject(l_iIndex);
		assert(m_pCurrentEventInstance);
		m_bAllStatusWorking = e_pEventObjectInstance->m_bAllStatusWorking;
		//l_iCount = this->Count();
		//for( int i=0;i<l_iCount;++i )
		//{
		//	cObjectAndName<cEventObject>*l_pData = CloneEventObject(e_pEventObjectStatus->GetObject(i));
		//	this->AddObjectNeglectExist(l_pData);
		//}
	}

	bool	cEventObjectInstance::ProcessEventInstance(const WCHAR*e_strEventInstanceName)
	{
		cEventInstance*l_pEventInstance = g_pCurrentEventManager->GetEventInstance(e_strEventInstanceName);
		if( l_pEventInstance )
		{
			AddEventInstance(l_pEventInstance);
			return true;
		}
		return false;
	}

	bool	cEventObjectInstance::ProcessEventObjectStatus(const WCHAR*e_strEventObjectStatusName)
	{
			cEventObjectStatus* l_pEventObjectStatus = g_pCurrentEventManager->GetEventObjectStatus(e_strEventObjectStatusName);
			if( l_pEventObjectStatus )
			{
				SetTargetEventObjectStatus(l_pEventObjectStatus);
				return true;
			}
			return false;
	}

	bool	cEventObjectInstance::ProcessStatusEventVariable(const WCHAR*e_strStatusEventVariableName)
	{
		m_pStatusEventVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(e_strStatusEventVariableName);
		if( !m_pStatusEventVariable )
		{
			return false;
		}
		return true;
	}

	cEventObjectInstance::~cEventObjectInstance()
	{
		SAFE_DELETE(m_pEventInstanceList);
	}
	
	bool cEventObjectInstance::InternalCollide(int e_iPosX,int e_iPosY)
	{
		if( m_bAllStatusWorking )
			return true;
		if( !m_pCurrentEventObjectStatusData )
			return true;
		return m_pCurrentEventObjectStatusData->IsCollided(e_iPosX,e_iPosY);
	}

	bool      cEventObjectInstance::SetCurrentWorkingObject(int e_iIndex,bool e_bRestart)
	{
		if( cEventObjectStatus::SetCurrentWorkingObject(e_iIndex,e_bRestart) )
		{
			m_pCurrentEventObjectStatusData = this->m_pCurrentWorkingObject->GetObject();
		}
		return 0;
	}

	void			cEventObjectInstance::MouseMove(int e_iPosX,int e_iPosY)
	{
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->MouseMove(e_iPosX,e_iPosY)) 
		}
		else
		if(this->m_pCurrentEventObjectStatusData) 
			m_pCurrentEventObjectStatusData->MouseMove(e_iPosX,e_iPosY);
		if( m_pCurrentEventInstance )
			m_pCurrentEventInstance->MouseMove(e_iPosX,e_iPosY);
	}

    void			cEventObjectInstance::MouseDown(int e_iPosX,int e_iPosY)
	{
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->MouseDown(e_iPosX,e_iPosY)) 
		}
		else
		if(this->m_pCurrentEventObjectStatusData) 
			m_pCurrentEventObjectStatusData->MouseDown(e_iPosX,e_iPosY);
		if( m_pCurrentEventInstance )
			m_pCurrentEventInstance->MouseDown(e_iPosX,e_iPosY);
	}

    void    		cEventObjectInstance::MouseUp(int e_iPosX,int e_iPosY)
	{
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->MouseUp(e_iPosX,e_iPosY)) 
		}
		else
		if(this->m_pCurrentEventObjectStatusData)
			m_pCurrentEventObjectStatusData->MouseUp(e_iPosX,e_iPosY);
		if( m_pCurrentEventInstance )
			m_pCurrentEventInstance->MouseUp(e_iPosX,e_iPosY);
	}

	void	cEventObjectInstance::Init()
	{
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->Init()) 
		}
		else
		{
			this->SetCurrentWorkingObject(m_iStartStatusIndex,false);
			cEventObjectStatus::Init(); 
		}
		m_pEventInstanceList->Init();
	}

	void	cEventObjectInstance::Update(float e_fElpaseTime)
	{
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->Update(e_fElpaseTime)) 
		}
		else
			cEventObjectStatus::Update(e_fElpaseTime);
		if( m_pCurrentEventInstance )
			m_pCurrentEventInstance->Update(e_fElpaseTime);
	}

	void	cEventObjectInstance::Render()
	{
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->Render()) 
		}
		else
			cEventObjectStatus::Render();
		if( m_pCurrentEventInstance )
			m_pCurrentEventInstance->Render();
	}

	void	cEventObjectInstance::Destroy()
	{
		m_pEventInstanceList->Destroy();
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			this->m_ObjectList[i]->Destroy();
		}
	}

	void	cEventObjectInstance::SetAllStatusWorking(bool e_bAllStatusWorking)
	{
		m_bAllStatusWorking = e_bAllStatusWorking;
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->Init());
			//LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->Start());
		}
	}

	void	cEventObjectInstance::AddEventInstance(cEventInstance*e_pEventInstance,bool e_bClone)
	{
		if( e_bClone )
		{
			cEventInstance*l_pEventInstance = e_pEventInstance->Clone();
			m_pEventInstanceList->AddObjectNeglectExist(l_pEventInstance);
		}
		else
		{
			m_pEventInstanceList->AddObjectNeglectExist(e_pEventInstance);
		}
	}

	void	cEventObjectInstance::SetTargetEventObjectStatus(cEventObjectStatus*e_pEventObjectStatus)
	{
		cEventObjectStatus::Destroy();
		CloneFromList(e_pEventObjectStatus);
		//ADD_CLONE_DATA_TO_NEW_VECTOR(cObjectAndName<cEventObject>,e_pEventObjectStatus,this);
		m_pTargetEventObjectStatus = e_pEventObjectStatus;
		this->m_pStatusEventVariable = e_pEventObjectStatus->GetStatusEventVariable();
	}

	void	cEventObjectInstance::CollectResourceList(cNamedTypedObjectVector<NamedTypedObject>*e_pObjectListByName)
	{
		int	l_iCount = Count();
		for( int i=0;i<l_iCount;++i )
		{
			NamedTypedObject*l_pResource = GetObject(i)->GetObject()->GetResourcePointer();
			if( l_pResource )
			{//if if crash here,it's possible resource parent(MPDIList)was deleted
				e_pObjectListByName->AddObject(l_pResource);
			}
		}
		l_iCount = m_pEventInstanceList->Count();
		for( int i=0;i<l_iCount;++i )
		{
			m_pEventInstanceList->GetObject(i)->CollectResourceList(e_pObjectListByName);
		}
	}
	//<cEventObjectInstance Name="VendorMenu" cEventInstance="VendorMenu" cEventObjectStatus="VendorMenu" EventObjectStatusVariable="VendorMenuSelectedIndex" />
	TiXmlElement*	cEventObjectInstance::ToTiXmlElement()
	{
		TiXmlElement*l_pTiXmlElement = new TiXmlElement(cEventObjectInstance::TypeID);
		l_pTiXmlElement->SetAttribute(NAME,this->GetName());
		if(m_pEventInstanceList&&m_pEventInstanceList->Count())
		{
			int	l_iEventInstanceCount = m_pEventInstanceList->Count();
			if( l_iEventInstanceCount == 1 )
				l_pTiXmlElement->SetAttribute(cEventInstance::TypeID,m_pEventInstanceList->GetObject(0)->GetName());
			else
			{
				for( int i=0;i<l_iEventInstanceCount;++i )
				{
					TiXmlElement*l_pChildTiXmlElement = new TiXmlElement(cEventObjectInstance::TypeID);	
					l_pTiXmlElement->LinkEndChild(l_pChildTiXmlElement);
					l_pChildTiXmlElement->SetAttribute(L"Name",m_pEventInstanceList->GetObject(i)->GetName());
				}
			}
		}

		if(this->Count())
		{
			if( m_pTargetEventObjectStatus )
				l_pTiXmlElement->SetAttribute(cEventObjectStatus::TypeID,m_pTargetEventObjectStatus->GetName());
		}
		if( m_pTargetEventObjectStatus )
		{
			l_pTiXmlElement->SetAttribute(L"EventObjectStatusVariable",m_pStatusEventVariable->GetName());
		}
		return l_pTiXmlElement;
	}
//end Event
}