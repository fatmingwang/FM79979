#include "../stdafx.h"
#include "EventObject.h"
#include "EventImage.h"
#include "EventMPDI.h"
#include "EventParticle.h"
#include "EventSound.h"
#include "EventText.h"
#include "EventVariableDataRender.h"
#include "EventManager.h"
#include "EventHeaderDefine.h"

namespace   FATMING_CORE
{
	//	<EventObject Type="MPDI">
	//		<Data Name="" AttributeDistance="0,0" ViewRect="0,0,1000,000" ElementDistance="0,100" cEventVariable="CurrentShowIndex" FileName="xx.xml" RootElementAttributePos="500,0"/>
	//	</EventObject>
	cEventXmlText*	ProcessEventObjectData_EventXmlText(TiXmlElement*e_pTixmlElement)
	{
		return new cEventXmlText(e_pTixmlElement);
	}

	//	<EventObject Type="Text">
	//		<Data Pos="200.00,200.00,0.00" Size="300,300" Type="Text" Text="79979"/>
	//	</EventObject>
    cEventObject*    ProcessEventObjectData_Text(TiXmlElement*e_pElement)
	{
		return new cEventText(e_pElement);
	}
	//	<EventObject Type="Image">
		//<Data Pos="200.00,200.00" Size="300,300" PI="SelectMode" PIUnit="2" />
	//	</EventObject>
	//	<EventObject Type="Image">
		//<Data Pos="200.00,200.00" Size="300,300" BaseImage="SelectMode" />
	//	</EventObject>
    cEventObject*    ProcessEventObjectData_Image(TiXmlElement*e_pElement)
	{
		return new cEventImage(e_pElement);
	}
	//	<EventObject Type="MPDI ">
		//<Data Pos="0.00,0.00" RelativePath="MPDI/" MPDIList="SelectMode_mpdi" MPDI="Selected" Scale="1" ClickToPlay="0" StopAtLastFrame="1" Loop="1" />
	//	</EventObject>
    cEventObject*    ProcessEventObjectData_MPDI(TiXmlElement*e_pElement)
	{
		return new cEventMPDI(e_pElement);
	}
	//	<EventObject Type="Sound ">
		//<Data Pos="0.00,0.00" SoundFileName="xxx.wav" loop="0"/>
	//	</EventObject>
    cEventObject*    ProcessEventObjectData_Sound(TiXmlElement*e_pElement)
	{
		return new cEventSound(e_pElement);
	}
	//	<EventObject Type="Particle">
		//<Data PrtGFile="aabb.prtg" Emitter="IAMEmitter" Pos="100,100,100" />
	//	</EventObject>
	cEventObject*    ProcessEventObjectData_Particle(TiXmlElement*e_pElement)
	{
		return new cEventParticle(e_pElement);
	}
	//	<EventObject Type="PrtG">
		//<Data PrtGFile="aabb.prtg" PrtG="IAMEmitter" Pos="100,100,100" />
	//	</EventObject>
	cEventObject*    ProcessEventObjectData_PrtG(TiXmlElement*e_pElement)
	{
		return new cEventParticleGroup(e_pElement);
	}
	//	<EventObject Type="MPDI">
	//		<Data Name="" Pos="0,0" cEventVariable="Variable" PrefixText="Test" />
	//	</EventObject>
	cEventObject*    ProcessEventObjectData_EventVariableDataRender(TiXmlElement*e_pElement)
	{
		return new cEventVariableDataRender(e_pElement);
	}
	//	<EventObject Type="MPDI">
	//		<Data MPDIList="" MPDI="" StopAtLastFrame=""/>
	//	</EventObject>
	cEventObject*	cEventManager::GetEventObject(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cEventObject::TypeID);
		eEventObjectType	l_eEventObjectType = GetEventObjectType(ATTRIBUT_VALUE_WITH_DEFINE(e_pElement,EVENT_OBJECT_TYPE));
		TiXmlElement l_Element(*e_pElement->FirstChildElement());
		//ELEMENT_VALUE_ASSERT_CHECK(e_pElement->FirstChildElement(),EVENT_OBJECT_DATA);
		switch(l_eEventObjectType)
		{
			case eEOT_TEXT:
				{
					if(!wcscmp(l_Element.Value(),cEventText::TypeID))
						return ProcessEventObjectData_Text(&l_Element);
					if(!wcscmp(l_Element.Value(),cEventXmlText::TypeID))
						return ProcessEventObjectData_EventXmlText(&l_Element);					
				}
				break;
			case eEOT_IMAGE:
				return ProcessEventObjectData_Image(&l_Element);
				break;
			case eEOT_MPDI:
				return ProcessEventObjectData_MPDI(&l_Element);
				break;
			case eEOT_SOUND:
				return ProcessEventObjectData_Sound(&l_Element);
				break;
			case eEOT_PARTICLE:
				return ProcessEventObjectData_Particle(&l_Element);
				break;
			case eEOT_PARTICLE_GROUP:
				return ProcessEventObjectData_PrtG(&l_Element);
				break;
			case eEOT_VARIABLE_RENDER:
				return ProcessEventObjectData_EventVariableDataRender(&l_Element);
				break;
			default:
				return 0;
				break;
		}
		return 0;
	}
//end namespace   FATMING_CORE
}