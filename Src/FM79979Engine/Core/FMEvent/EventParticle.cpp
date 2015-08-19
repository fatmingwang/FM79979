#include "../stdafx.h"
#include "EventObject.h"
#include "EventParticle.h"
#include "EventManager.h"
namespace   FATMING_CORE
{
    const wchar_t*         cEventParticle::TypeID( L"cEventParticle" );
    const wchar_t*         cEventParticleGroup::TypeID( L"cEventParticleGroup" );
    cEventParticle::cEventParticle(Vector3 e_vPos,cPrtEmitter*e_pPrtEmitter):cEventObject(eEOT_PARTICLE,e_vPos)
    {
        m_pPrtEmitter = new cPrtEmitter(e_pPrtEmitter);
        m_vParticlePos = Vector3::Zero;
    }
    
    cEventParticle::cEventParticle(cEventParticle*e_pEventSound):cEventObject(e_pEventSound)
    {
        m_pPrtEmitter = new cPrtEmitter(e_pEventSound->m_pPrtEmitter);
        m_vParticlePos = Vector3::Zero;
    }

	cEventParticle::cEventParticle(TiXmlElement*e_pElement):cEventObject(eEOT_PARTICLE)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cEventParticle::TypeID);
		this->m_pPrtEmitter = 0;
		m_vParticlePos = Vector3::Zero;
		cBehaviorPaticleManager*l_pBehaviorPaticleManager = 0;
		Vector3	l_vPos;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("Emitter")
			{
				assert(l_pBehaviorPaticleManager);
				cPrtEmitter*l_pPrtEmitter = l_pBehaviorPaticleManager->m_pPaticleManager->GetObject((WCHAR*)l_strValue);
				m_pPrtEmitter = new cPrtEmitter(l_pPrtEmitter);
			}
			else
			COMPARE_NAME("PrtGFile")
			{
				l_pBehaviorPaticleManager = g_pCurrentEventManager->m_pPrtGList->GetObject((WCHAR*)l_strValue);
                if(!l_pBehaviorPaticleManager)
                {
					std::string l_strFileName = *g_pCurrentEventManager->m_pstrWorkingDirectory;
                    l_strFileName += UT::WcharToChar(l_strValue);
					l_strFileName += ".prtg";
					l_pBehaviorPaticleManager = new cBehaviorPaticleManager;
					l_pBehaviorPaticleManager->Parse((char*)l_strFileName.c_str());
					l_pBehaviorPaticleManager->SetName(l_strValue);
					g_pCurrentEventManager->m_pPrtGList->AddObjectNeglectExist(l_pBehaviorPaticleManager);
                }
			}
			else
			COMPARE_NAME("Pos")
			{
				this->SetLocalPosition(VALUE_TO_VECTOR3);
			}
		PARSE_NAME_VALUE_END	
	}
    
    cEventParticle::~cEventParticle()
    {
        SAFE_DELETE(m_pPrtEmitter);
    }

    void    cEventParticle::InternalMouseUp(int e_iPosX,int e_iPosY)
    {
        m_vParticlePos = Vector3((float)e_iPosX,(float)e_iPosY,0.f);
        if( !m_pPrtEmitter->IsActived() )
			m_pPrtEmitter->Emit(m_vParticlePos);
        else
        {
            m_pPrtEmitter->SetPos(m_vParticlePos);
            m_pPrtEmitter->Emit();
        }
        
    }
    
    bool    cEventParticle::InternalCollide(int e_iPosX,int e_iPosY)
    {
        return true;
    }
    
    void    cEventParticle::InternalInit()
    {
        if( m_pPrtEmitter )
        {
            //m_pPrtEmitter->Stop();
			m_pPrtEmitter->Emit(this->GetLocalPosition());
            m_vParticlePos = Vector3::Zero;
        }
    }
    
    void    cEventParticle::Destroy()
    {
        m_pPrtEmitter->Stop();
    }
    
    void    cEventParticle::InternalUpdate(float e_fElpaseTime)
    {
        if( m_pPrtEmitter )
        {
            m_pPrtEmitter->SetPos(this->GetPosWithPagePos()+m_vParticlePos);
            m_pPrtEmitter->Update(e_fElpaseTime);
        }
    }

    void    cEventParticle::InternalRender()
    {
        if( m_pPrtEmitter )
        {
            m_pPrtEmitter->Render();
        }
    }

    NamedTypedObject*   cEventParticle::GetResourcePointer()
    {
		return 0;
		//NamedTypedObject*l_pOwner = m_pPrtEmitter->GetOwner();
		//if( l_pOwner->GetOwner() )
			//return l_pOwner->GetOwner()->GetOwner();
		//return l_pOwner->GetOwner();
		return m_pPrtEmitter->GetOwner()->GetOwner();
    }
	//	<EventObject Type="Particle">
		//<Data PrtGFile="aabb.prtg" Emitter="IAMEmitter" Pos="100,100,100" />
	//	</EventObject>
	TiXmlElement*	cEventParticle::ToTiXmlElement()
	{
		TiXmlElement*l_pEventObjectTiXmlElement = new TiXmlElement(L"EventObject");
		l_pEventObjectTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,L"Particle");
		{
			TiXmlElement*l_pDataTiXmlElement = new TiXmlElement(L"Data");
			l_pDataTiXmlElement->SetAttribute(L"Pos",ValueToStringW(this->GetLocalPosition()));
			l_pDataTiXmlElement->SetAttribute(L"PrtGFile",this->m_pPrtEmitter->GetOwner()->GetName());
			l_pDataTiXmlElement->SetAttribute(L"Emitter",this->m_pPrtEmitter->GetName());
			l_pEventObjectTiXmlElement->LinkEndChild(l_pDataTiXmlElement);
		}
		return l_pEventObjectTiXmlElement;
	}
//================================
//
//================================
    cEventParticleGroup::cEventParticleGroup(const WCHAR*e_strFileName,Vector3 e_vPos,cParticleEmitterGroup*e_pParticleEmitterGroup):cEventObject(eEOT_PARTICLE_GROUP,e_vPos)
    {
		m_strFileName = e_strFileName;
        m_pParticleEmitterGroup = new cParticleEmitterGroup(e_pParticleEmitterGroup);
		m_pParticleEmitterGroup->SetAnimationLoop(true);
    }
    
    cEventParticleGroup::cEventParticleGroup(cEventParticleGroup*e_pEventParticleGroup):cEventObject(e_pEventParticleGroup)
    {
        m_pParticleEmitterGroup = new cParticleEmitterGroup(e_pEventParticleGroup->m_pParticleEmitterGroup);
    }
    
	cEventParticleGroup::cEventParticleGroup(TiXmlElement*e_pElement):cEventObject(eEOT_PARTICLE_GROUP)
	{
		m_pParticleEmitterGroup = 0;
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cEventParticleGroup::TypeID);
		cEventParticleGroup*l_pEventParticleGroup = 0;
        cParticleEmitterGroup*l_pParticleEmitterGroup = 0;
		cBehaviorPaticleManager*l_pBehaviorPaticleManager = 0;
		Vector3	l_vPos;
		std::wstring	l_strFileName;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("PrtG")
			{
				assert(l_pBehaviorPaticleManager);
				l_pParticleEmitterGroup = dynamic_cast<cParticleEmitterGroup*>(l_pBehaviorPaticleManager->GetObject((WCHAR*)l_strValue));
			}
			else
			COMPARE_NAME("PrtGFile")
			{
				m_strFileName = l_strValue;
				l_pBehaviorPaticleManager = g_pCurrentEventManager->m_pPrtGList->GetObject((WCHAR*)l_strValue);
                if(!l_pBehaviorPaticleManager)
                {
                    std::string l_strFileName = *g_pCurrentEventManager->m_pstrWorkingDirectory;
                    l_strFileName += UT::WcharToChar(l_strValue);
					l_strFileName += ".prtg";
					l_pBehaviorPaticleManager = new cBehaviorPaticleManager;
					l_pBehaviorPaticleManager->Parse((char*)l_strFileName.c_str());
					l_pBehaviorPaticleManager->SetName(l_strValue);
					g_pCurrentEventManager->m_pPrtGList->AddObjectNeglectExist(l_pBehaviorPaticleManager);
                }
			}
			else
			COMPARE_NAME("Pos")
			{
				this->SetLocalPosition(VALUE_TO_VECTOR3);
			}
		PARSE_NAME_VALUE_END
		if( l_pParticleEmitterGroup )
		{
			m_pParticleEmitterGroup = new cParticleEmitterGroup(l_pParticleEmitterGroup);
			m_pParticleEmitterGroup->SetAnimationLoop(true);
		}
	}

    cEventParticleGroup::~cEventParticleGroup()
    {
        SAFE_DELETE(m_pParticleEmitterGroup);
    }
    
    
    void    cEventParticleGroup::InternalMouseUp(int e_iPosX,int e_iPosY)
    {
        if( !m_pParticleEmitterGroup->IsStart() )
            m_pParticleEmitterGroup->Init();
        else
        {
            //m_pParticleEmitterGroup->SetPos(this->GetPosWithPagePos());
        }
        
    }
    
    bool    cEventParticleGroup::InternalCollide(int e_iPosX,int e_iPosY)
    {
        return true;
    }
    
    void    cEventParticleGroup::Init()
    {
        if( m_pParticleEmitterGroup )
        {
			m_pParticleEmitterGroup->SetPos(this->GetLocalPosition());
            m_pParticleEmitterGroup->Init();
            //m_pParticleEmitterGroup->Stop();
        }
    }
    
    void    cEventParticleGroup::PageLeave()
    {
        m_pParticleEmitterGroup->Stop();
    }
    
    void    cEventParticleGroup::Update(float e_fElpaseTime)
    {
        if( m_pParticleEmitterGroup )
        {
            m_pParticleEmitterGroup->Update(e_fElpaseTime);
            m_pParticleEmitterGroup->SetPos(this->GetPosWithPagePos());
        }
    }

    void    cEventParticleGroup::InternalRender()
    {
        if( m_pParticleEmitterGroup )
        {
            m_pParticleEmitterGroup->Render();
        }
    }

    NamedTypedObject*   cEventParticleGroup::GetResourcePointer()
    {
		return 0;
		return (NamedTypedObject*)m_pParticleEmitterGroup->GetOwner();
    }
	//<EventObject Type="PrtG">
	//	<Data PrtGFile="aabb.prtg" PrtG="IAMEmitter" Pos="100,100,100" />
	//</EventObject>
	TiXmlElement*	cEventParticleGroup::ToTiXmlElement()
	{
		TiXmlElement*l_pDataTiXmlElement = new TiXmlElement(L"Data");
		l_pDataTiXmlElement->SetAttribute(L"PrtGFile",this->m_strFileName.c_str());
		l_pDataTiXmlElement->SetAttribute(L"PrtG",this->m_pParticleEmitterGroup->GetName());
		return 	l_pDataTiXmlElement;
		TiXmlElement*l_pEventObjectTiXmlElement = new TiXmlElement(L"EventObject");
		l_pEventObjectTiXmlElement->LinkEndChild(l_pDataTiXmlElement);
		l_pEventObjectTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,L"Particle");
		return l_pEventObjectTiXmlElement;
	}
//end namespace
}