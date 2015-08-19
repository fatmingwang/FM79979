#include "../stdafx.h"
//for iphone
//#include "../stdafx.h"
#include "EventObject.h"
#include "EventImage.h"
#include "EventMPDI.h"
#include "EventParticle.h"
#include "EventSound.h"
#include "EventText.h"
#include "EventVariableDataRender.h"
namespace   FATMING_CORE
{
	const wchar_t*         cEventObject::TypeID( L"cEventObject" );
	//cObjectAndName<cEventObject>*	CloneEventObject(cObjectAndName<cEventObject>*e_pEventObject)
	//{
	//	cObjectAndName<cEventObject>*l_pReturnData = 0;
	//	switch(e_pEventObject->GetObject()->GetEventObjectType())
	//	{
	//		case eEOT_TEXT:
	//		{
	//			cObjectAndName<cEventText>*l_pData = new cObjectAndName<cEventText>((cObjectAndName<cEventText>*)e_pEventObject);
	//			l_pReturnData = (cObjectAndName<cEventObject>*)l_pData;
	//		}
	//			break;
	//		case eEOT_IMAGE:
	//		{
	//			cObjectAndName<cEventImage>*l_pData = new cObjectAndName<cEventImage>((cObjectAndName<cEventImage>*)e_pEventObject);
	//			l_pReturnData = (cObjectAndName<cEventObject>*)l_pData;
	//		}
	//			break;
	//		case eEOT_MPDI:
	//		{
	//			cObjectAndName<cEventMPDI>*l_pData = new cObjectAndName<cEventMPDI>((cObjectAndName<cEventMPDI>*)e_pEventObject);
	//			l_pReturnData = (cObjectAndName<cEventObject>*)l_pData;
	//		}
	//			break;
	//		case eEOT_SOUND:
	//		{
	//			cObjectAndName<cEventSound>*l_pData = new cObjectAndName<cEventSound>((cObjectAndName<cEventSound>*)e_pEventObject);
	//			l_pReturnData = (cObjectAndName<cEventObject>*)l_pData;
	//		}
	//			break;
	//		case eEOT_PARTICLE:
	//		{
	//			cObjectAndName<cEventParticle>*l_pData = new cObjectAndName<cEventParticle>((cObjectAndName<cEventParticle>*)e_pEventObject);
	//			l_pReturnData = (cObjectAndName<cEventObject>*)l_pData;
	//		}
	//			break;
	//		case eEOT_PARTICLE_GROUP:
	//		{
	//			cObjectAndName<cEventParticleGroup>*l_pData = new cObjectAndName<cEventParticleGroup>((cObjectAndName<cEventParticleGroup>*)e_pEventObject);
	//			l_pReturnData = (cObjectAndName<cEventObject>*)l_pData;
	//		}
	//			break;
	//	}
	//	return l_pReturnData;
	//}

	eEventObjectType	GetEventObjectType(const WCHAR*e_strName)
	{
		if( !wcscmp(e_strName,L"Text") )
			return eEOT_TEXT;
		if( !wcscmp(e_strName,L"Image") )
			return eEOT_IMAGE;
		if( !wcscmp(e_strName,L"MPDI") )
			return eEOT_MPDI;
		if( !wcscmp(e_strName,L"Sound") )
			return eEOT_SOUND;
		if( !wcscmp(e_strName,L"Particle") )
			return eEOT_PARTICLE;
		if( !wcscmp(e_strName,L"PrtG") )
			return eEOT_PARTICLE_GROUP;
		if( !wcscmp(e_strName,L"VariableRender") )
			return eEOT_VARIABLE_RENDER;
		return eEOT_MAX;
	}

	const WCHAR*		GetEventObjectTypeName(cEventObject*e_pEventObject)
	{
		if(e_pEventObject->Type() == cEventVariableDataRender::TypeID)
		{
			return L"VariableRender";
		}
		else
		if(e_pEventObject->Type() == cEventImage::TypeID)
		{
			return L"Image";
		}
		else
		if(e_pEventObject->Type() == cEventMPDI::TypeID)
		{
			return L"MPDI";
		}
		else
		if(e_pEventObject->Type() == cEventParticle::TypeID)
		{
			return L"Particle";
		}
		else
		if(e_pEventObject->Type() == cEventParticleGroup::TypeID)
		{
			return L"PrtG";
		}
		else
		if(e_pEventObject->Type() == cEventSound::TypeID)
		{
			return L"Sound";
		}
		else
		if(e_pEventObject->Type() == cEventText::TypeID)
		{
			return L"Text";
		}
		else
		if(e_pEventObject->Type() == cEventXmlText::TypeID)
		{
			return L"Text";
		}
		return 0;	
	}

    cEventObject::cEventObject(eEventObjectType e_eEventObjectType,Vector3 e_vPos,Vector2 e_vSize,Vector4 e_vColor)
    {
		m_vColor = e_vColor;
		//m_pvViewPortSissor = 0;
		this->SetLocalPosition(e_vPos);
        m_vSize = e_vSize;
        m_eEventObjectType = e_eEventObjectType;
    }
    cEventObject::cEventObject(cEventObject*e_pEvent_PageData)
    {
		//if( e_pEvent_PageData->m_pvViewPortSissor )
		//{
		//	m_pvViewPortSissor = new Vector4();
		//	*m_pvViewPortSissor = *e_pEvent_PageData->m_pvViewPortSissor;
		//}
		m_vColor = e_pEvent_PageData->m_vColor;
		this->SetLocalTransform(e_pEvent_PageData->GetLocalTransform());
        m_vSize = e_pEvent_PageData->m_vSize;
        m_eEventObjectType = e_pEvent_PageData->m_eEventObjectType;    
        this->SetName(e_pEvent_PageData->GetName());
    }

    cEventObject::~cEventObject()
	{
		//SAFE_DELETE(m_pvViewPortSissor);
	}

    char*   cEventObject::GetEventObjectTypeString()
    {
        switch(m_eEventObjectType)
        {
            case eEOT_TEXT:
                return "Text";
            case eEOT_IMAGE:
                return "Image";
            case eEOT_MPDI:
                return "MPDI";
            case eEOT_SOUND:
                return "Sound";
            break;
            case eEOT_PARTICLE:
                return "Particle";
            break;
            case eEOT_PARTICLE_GROUP:
                return "ParticleGroup";
            break;          
        }
        return "Error";
    }    

    Vector3 cEventObject::GetPosWithPagePos()
    {
		return this->GetLocalPosition();
  //      cEventPackage*l_pEventPageDataList = dynamic_cast<cEventPackage*>(this->GetOwner());
		//if( l_pEventPageDataList )
		//	return l_pEventPageDataList->GetRelativePosition()+this->m_vPos;
		return Vector3::Zero;
    }
}