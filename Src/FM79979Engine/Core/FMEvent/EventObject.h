#ifndef _EVENT_PATGE_DATA_H_
#define _EVENT_PATGE_DATA_H_

#include "../AllCoreInclude.h"
#include "../Particle/AllParticleInclude.h"
#include "EventBase.h"

namespace   FATMING_CORE
{
    enum    eEventObjectType
    {
        eEOT_TEXT = 0,
		eEOT_XML_TEXT,
        eEOT_IMAGE,
        eEOT_MPDI,
        eEOT_SOUND,
        eEOT_PARTICLE,
        eEOT_PARTICLE_GROUP,
		eEOT_VARIABLE_RENDER,
        eEOT_MAX
    };
	class				cEventObject;
	eEventObjectType	GetEventObjectType(const WCHAR*e_strName);
	const WCHAR*		GetEventObjectTypeName(cEventObject*e_pEventObject);
    //base Event page data
    //all Event data must inhirent this!.
    class   cEventObject:public cEventBase
    {
        //GETP_SET_DEC(Vector3,m_vPos,GetPos,SetPos);
        GETP_SET_DEC(Vector2,m_vSize,GetSize,SetSize);
		GETP_SET_DEC(Vector4,m_vColor,GetColor,SetColor);
        eEventObjectType   m_eEventObjectType;
		//it m_pvViewPortSissor is not null,object render only could be visible in m_pvViewPortSissor
		//glDisable(sisscor);
		//Vector4*	m_pvViewPortSissor;
		//
		virtual	void	InternalInit()override {}
		virtual	void	InternalRender()override {}
		virtual void    InternalUpdate(float e_fElpaseTime)override {}
    public:
		DEFINE_TYPE_INFO();
		cEventObject(eEventObjectType e_eEventObjectType,Vector3 e_vPos = Vector3::Zero,Vector2 e_vSize = Vector2(1.f,1.f),Vector4 e_vColor = Vector4::One);
        cEventObject(cEventObject*e_pEvent_PageData);
        virtual ~cEventObject();
        eEventObjectType   GetEventObjectType(){return m_eEventObjectType;}
        char*           GetEventObjectTypeString();
        Vector3 GetPosWithPagePos();
        //if resource is same return true or false
        bool    IsResourceSame(cEventObject*e_pEventPageData);
		//some object is allow click to to play,and the event might want this data to detect mouse up event
		//and event is get data but event object,ex:MPDI.cEventObjectMouseUpDectet
		bool	IsMouseUp(){ return false; }
		virtual	bool	IsPlayDone(){ return true; }
		//force to release resource ex:MPDI.Particle....else
		//virtual	void	ForceReleaseResource() = 0;
		virtual	TiXmlElement*	ToTiXmlElement()override { return nullptr; }
		//make sure momory leak is not exist
		virtual NamedTypedObject*   GetResourcePointer()override = 0;
    };
    
    enum    eFMBSlidePageDirection
    {
        eFMBSPD_CENTER = 0,
        eFMBSPD_LEFT,
        eFMBSPD_UP,
        eFMBSPD_RIGHT,
        eFMBSPD_DOWN,
        eFMBSPD_MAX,
    };

	//cObjectAndName<cEventObject>*	CloneEventObject(cObjectAndName<cEventObject>*e_pEventObject);
}

#endif