#include "../stdafx.h"
#include "EventHeaderDefine.h"
#include "EventObject.h"
#include "EventImage.h"
#include "EventMPDI.h"
#include "EventParticle.h"
#include "EventSound.h"
#include "EventText.h"
#include "EventVariableDataRender.h"
#include "EventManager.h"
#include "EventPackageRunner.h"
	//<EventPackage Name="Test">
	//	<cEventObjectInstance Name="Test"  cEventObjectStatus="Test" />
	//	<cEventObjectInstance Name="Test"  cEventInstance="TalkQ" />
	//	<cEventObjectInstance Name="A_Talk"  cEventObjectStatus="A_Talk" />
	//</EventPackage>
TiXmlElement*	cEventPackage::ToTiXmlElement()
{
	TiXmlElement*l_pEventPackageTiXmlElement = new TiXmlElement(EVENT_PACKAGE);
	l_pEventPackageTiXmlElement->SetAttribute(NAME,this->GetName());
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cEventObjectInstance*l_pEventObjectInstance = this->m_ObjectList[i];
		TiXmlElement*l_pEventObjectInstanceTiXmlElement = new TiXmlElement(cEventObjectInstance::TypeID);
		l_pEventObjectInstanceTiXmlElement->SetAttribute(NAME,l_pEventObjectInstance->GetName());
		cEventObjectStatus*l_pEventObjectStatus = l_pEventObjectInstance->GetEventObjectStatus();
		if( l_pEventObjectStatus )
		{
			l_pEventObjectInstanceTiXmlElement->SetAttribute(cEventObjectStatus::TypeID,l_pEventObjectStatus->GetName());
		}
		cEventInstance*l_pEventInstance = l_pEventObjectInstance->GetEventInstanceList()->GetObject(0);
		if( l_pEventInstance )
		{
			l_pEventObjectInstanceTiXmlElement->SetAttribute(cEventInstance::TypeID,l_pEventInstance->GetName());
		}
		l_pEventPackageTiXmlElement->LinkEndChild(l_pEventObjectInstanceTiXmlElement);
	}
	return l_pEventPackageTiXmlElement;
}


	//<cEventObjectStatus Name="Test">
	//	<cEventObject Type="MPDI" Name="Test">
	//		<cEventMPDI RelativePath="MPDIFile" Pos="0,0,0" MPDIList="test_mpdi" MPDI="test" StopAtLastFrame="1" ClickToPlay="1"/>
	//	</cEventObject>
	//</cEventObjectStatus>

TiXmlElement*	cEventObjectStatus::ToTiXmlElement()
{
	TiXmlElement*l_pEventObjectStatusTiXmlElement = new TiXmlElement(cEventObjectStatus::TypeID);
	l_pEventObjectStatusTiXmlElement->SetAttribute(NAME,this->GetName());
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cObjectAndName<cEventObject>*l_pObjectAndName = this->m_ObjectList[i];
		cEventObject*l_pEventObject = l_pObjectAndName->GetObject();
		const WCHAR*l_strType = GetEventObjectTypeName(l_pEventObject);
		TiXmlElement*l_pEventObjectTiXmlElement = new TiXmlElement(cEventObject::TypeID);
		l_pEventObjectTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,l_strType);
		l_pEventObjectTiXmlElement->SetAttribute(NAME,l_pObjectAndName->GetName());
		l_pEventObjectStatusTiXmlElement->LinkEndChild(l_pEventObjectTiXmlElement);
		TiXmlElement*l_pEventObjectDataTiXmlElement = l_pEventObject->ToTiXmlElement();
		if( l_pEventObjectDataTiXmlElement )
		{
			l_pEventObjectTiXmlElement->LinkEndChild(l_pEventObjectDataTiXmlElement);
		}
		else
		{
			UT::ErrorMsg(l_pEventObject->GetName(),l_strType);
		}
	}
	return l_pEventObjectStatusTiXmlElement;
}

	//<cEventInstance Name="VendorMenu_GameSetup_4_LineBetableSelectLeft" Repeat="1" AllTriggetDoneAtSameTime="1" >
	//	<EventTrigger>
	//		<cEventButton Name="22" Text="Next" Pos="650,50,0" />		
	//	</EventTrigger>
	//	<EventActiver>
	//		<cEventButtonSelectedActiver Name="79979" cEventVariable="CurrentPackageName" Result="Answer1,Answer2,Answer3"/>
	//	</EventActiver>
	//</cEventInstance>
TiXmlElement*	cEventInstance::ToTiXmlElement()
{
	TiXmlElement*l_pEventInstanceTiXmlElement = this->GetMyTiXmlElement();
	l_pEventInstanceTiXmlElement->SetAttribute(NAME,this->GetName());
	l_pEventInstanceTiXmlElement->SetAttribute(EVENT_REPEAT,this->IsRepeat()?L"1":L"0");
	if(m_bTriggerAllDoneAtSameTime)
		l_pEventInstanceTiXmlElement->SetAttribute(L"AllTriggetDoneAtSameTime",m_bTriggerAllDoneAtSameTime?L"1":L"0");

	TiXmlElement*l_pEventTrigger = new TiXmlElement(L"EventTrigger");
	TiXmlElement*l_pEventActiver = new TiXmlElement(L"EventActiver");	
	l_pEventInstanceTiXmlElement->LinkEndChild(l_pEventTrigger);
	l_pEventInstanceTiXmlElement->LinkEndChild(l_pEventActiver);

	int	l_iCount = m_TriggerEventList.Count();
	for( int i=0;i<l_iCount;++i )
	{
		TiXmlElement*l_pChild = m_TriggerEventList[i]->ToTiXmlElement();
		l_pEventTrigger->LinkEndChild(l_pChild);
	}
	l_iCount = m_ActiveEventList.Count();
	for( int i=0;i<l_iCount;++i )
	{
		TiXmlElement*l_pChild = m_ActiveEventList[i]->ToTiXmlElement();
		l_pEventActiver->LinkEndChild(l_pChild);
	}
	//return l_pEventObjectStatusTiXmlElement;
	return l_pEventInstanceTiXmlElement;
}