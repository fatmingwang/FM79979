#include "../stdafx.h"
#include "EventObject.h"
#include "EventMPDI.h"
#include "EventManager.h"
namespace   FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cEventMPDI);
    cEventMPDI::cEventMPDI(Vector3 e_vPos,cMultiPathDynamicImage*e_pMPDI,bool e_bClickToPlay,float e_fScale,bool e_bLoop):cEventObject(eEOT_MPDI,e_vPos)
    {
        m_pMPDI = new cMultiPathDynamicImage(e_pMPDI);
		m_pMPDI->SetAnimationLoop(e_bLoop);
		m_pMPDI->SetOwner(e_pMPDI->GetOwner());
        m_pMPDI->Init();
		m_pMPDI->Update(EPSIONAL);
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(e_vPos)*cMatrix44::ScaleMatrix(Vector3(e_fScale,e_fScale,e_fScale));
		m_pMPDI->SetLocalTransform(l_mat);
		//m_pMPDI->SetPos(e_vPos);
		//m_pMPDI->SetScale(e_fScale);
        m_bClickToPlay = e_bClickToPlay;
        if( m_bClickToPlay )
        {
            m_bMouseClicked = true;
            m_pMPDI->SetAnimationLoop(false);
        }
        else
        {
            m_bMouseClicked = false;
			if( !m_pMPDI->IsStayAtLastFrame() )
				m_pMPDI->SetAnimationLoop(true);
			else
				m_pMPDI->SetAnimationLoop(false);
        }
    }
    
    cEventMPDI::cEventMPDI(cEventMPDI*e_pEvent_MPDI):cEventObject(e_pEvent_MPDI)
    {
		m_pMPDI = 0;
		if( e_pEvent_MPDI->m_pMPDI )
		{
			m_pMPDI = new cMultiPathDynamicImage(e_pEvent_MPDI->m_pMPDI);
			m_pMPDI->SetOwner(e_pEvent_MPDI->m_pMPDI->GetOwner());
			m_pMPDI->Init();
			m_pMPDI->Update(EPSIONAL);
		}
        //m_pMPDI->SetLoop(true);
        m_bClickToPlay = e_pEvent_MPDI->m_bClickToPlay;
        m_bMouseClicked = e_pEvent_MPDI->m_bMouseClicked;
    }
	//<cEventMPDI MPDIList="" MPDI="" ClickToPlay="" Pos="" RelativePath="" StopAtLastFrame="" Scale="" Loop="" />
	cEventMPDI::cEventMPDI(TiXmlElement*e_pTiXmlElement):cEventObject(eEOT_MPDI,Vector3::Zero)
	{
		//ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventMPDI::TypeID);
		m_bClickToPlay = false;
		m_bMouseClicked = false;
		m_pMPDI = 0;
		cMPDIList*l_pMPDIList = 0;
		bool	l_bLoop = false;
		float	l_fScale = 1.f;
		Vector3	l_vPos(0,0,0);
		std::string	l_strRelativePath;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("MPDIList")
			{
		        std::string l_strMPDIName = cMPDIList::GetFileName((WCHAR*)l_strValue);
		        //char*    l_strMPDIListFullName = FileNameConbineCurrentPageDirectory(e_strMPDIName);
		        //bool    l_b = cGameApp::m_spAnimationParser->Parse(l_strMPDIListFullName);
		        std::string l_str = *g_pCurrentEventManager->m_pstrWorkingDirectory;
				if( l_strRelativePath.length() )
				{
					l_str += l_strRelativePath;
					l_str += "/";
				}
		        l_str += l_strMPDIName;
				l_pMPDIList = dynamic_cast<cMPDIList*>(cGameApp::m_spAnimationParser->GetMPDIListByFileName(l_str.c_str()));
			    assert(l_pMPDIList);
			}
			else
			COMPARE_NAME("MPDI")
			{
				if( l_pMPDIList )
				{
					cMPDI*l_pMPDI = l_pMPDIList->GetObject(l_strValue);
					if( !l_pMPDI )
					{
						UT::ErrorMsg(L"mpdi name error",l_strValue);
					}
					else
					{
						this->m_pMPDI = new cMPDI(l_pMPDI);
						m_pMPDI->SetOwner(l_pMPDI->GetOwner());
						l_bLoop = m_pMPDI->IsAnimationLoop();
					}
				}
			}
			else
			COMPARE_NAME("ClickToPlay")
			{
			    m_bClickToPlay = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("Pos")
			{
				l_vPos = VALUE_TO_VECTOR3;
			    //m_vPos = VALUE_TO_VECTOR3;
			}
			else
			COMPARE_NAME("RelativePath")
			{
				l_strRelativePath = UT::WcharToChar(l_strValue);
			}
			else
			if( !wcscmp(L"StopAtLastFrame",l_strName) || !wcscmp(L"StayAtLastFrame",l_strName) )
			//COMPARE_NAME("StopAtLastFrame")
			{
				if( m_pMPDI )
					m_pMPDI->SetStayAtLastFrame(VALUE_TO_BOOLEAN);
			}
			else
			COMPARE_NAME("Scale")
			{
				l_fScale = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("Loop")
			{
				l_bLoop = VALUE_TO_BOOLEAN;
			}
	    PARSE_NAME_VALUE_END
		if( m_pMPDI )
		{
			cMatrix44	l_mat = cMatrix44::TranslationMatrix(l_vPos)*cMatrix44::ScaleMatrix(Vector3(l_fScale,l_fScale,l_fScale));
			m_pMPDI->SetLocalTransform(l_mat);
			m_pMPDI->SetAnimationLoop(l_bLoop);
			m_pMPDI->Init();
			m_pMPDI->Update(EPSIONAL);
		}
        if( m_bClickToPlay )
        {
            m_bMouseClicked = true;
			if( m_pMPDI )
				m_pMPDI->SetAnimationLoop(false);
        }
        else
        {
            m_bMouseClicked = false;
			if( m_pMPDI )
			{
				if( !m_pMPDI->IsStayAtLastFrame() )
					m_pMPDI->SetAnimationLoop(true);
				else
					m_pMPDI->SetAnimationLoop(false);
			}
        }
	}

    cEventMPDI::~cEventMPDI()
    {
        SAFE_DELETE(m_pMPDI);
    }
    
    void	cEventMPDI::InternalInit()
	{
        m_bMouseClicked = true;
		if( m_pMPDI )
		{
			if( !m_bClickToPlay )
			{
				m_pMPDI->Init();
				m_pMPDI->Update(EPSIONAL);
			}
			else
			{
				m_pMPDI->Init();
				m_pMPDI->Update(EPSIONAL);
				m_bMouseClicked = false;
			}
		}
	}

    void    cEventMPDI::InternalMouseUp(int e_iPosX,int e_iPosY)
    {
        m_bMouseClicked = true;
		if( m_bClickToPlay && m_pMPDI )
		{
			m_pMPDI->Init();
			m_pMPDI->Update(EPSIONAL);
		}
    }
    
    bool    cEventMPDI::InternalCollide(int e_iPosX,int e_iPosY)
    {
        if( m_bClickToPlay && m_pMPDI )
        {
            if(!this->m_pMPDI->Collide(e_iPosX,e_iPosY))
                return false;
			return true;
        }
        return false;
    }

    void    cEventMPDI::InternalUpdate(float e_fElpaseTime)
    {
		if( m_pMPDI )
		{
			if( m_bClickToPlay )
			{
				if( m_bMouseClicked )
				{
					m_pMPDI->Update(e_fElpaseTime);
					if( m_pMPDI->IsAnimationDone() )
					{
						//m_bMouseClicked = true;
					}
				}
			}
			else
			{
				m_pMPDI->Update(e_fElpaseTime);
			}
		}
    }

	void    cEventMPDI::DebugRender()
	{
		if(this->m_pMPDI)
			this->m_pMPDI->RenderCollide();
	}

    void    cEventMPDI::InternalRender()
    {
		if( m_pMPDI )
		{
			Vector3 l_vPos = GetPosWithPagePos();
			m_pMPDI->SetPos(l_vPos);
			if( m_bClickToPlay && !m_bMouseClicked )
				m_pMPDI->RenderFirsttFrameImage(true,Vector4(1.f,1.f,1.f,0.3f));
			else 
				m_pMPDI->Render();
		}
    }

	bool	cEventMPDI::IsPlayDone()
	{
		if( m_pMPDI )
			return m_pMPDI->IsAnimationDone();
		return true;
	}

    NamedTypedObject*   cEventMPDI::GetResourcePointer()
    {
		if( !m_pMPDI )
			return 0;
		NamedTypedObject*l_pNamedTypedObject = m_pMPDI->GetOwner();
        return l_pNamedTypedObject;
    }
	//	<EventObject Type="MPDI ">
		//<Data Pos="0.00,0.00" MPDIList="SelectMode_mpdi" MPDI="Selected" Scale="1" ClickToPlay="0" StopAtLastFrame="1" Loop="1" />
	//	</EventObject>
	TiXmlElement*	cEventMPDI::ToTiXmlElement()
	{
		if( !m_pMPDI )
			return 0;
		TiXmlElement*l_pDataTiXmlElement = new TiXmlElement(L"Data");
		l_pDataTiXmlElement->SetAttribute(L"Pos",ValueToStringW(this->GetLocalPosition()));
		l_pDataTiXmlElement->SetAttribute(L"MPDIList",this->m_pMPDI->GetOwner()->GetName());
		l_pDataTiXmlElement->SetAttribute(L"MPDI",this->m_pMPDI->GetName());
		Vector3 scale;
		Vector3 rotation;
		Vector3 translation;
		float inverted;
		m_pMPDI->GetLocalTransform().Decompose(scale, rotation, translation, inverted);
		l_pDataTiXmlElement->SetAttribute(L"Scale",ValueToStringW(scale.x).c_str());
		l_pDataTiXmlElement->SetAttribute(L"ClickToPlay",this->m_bClickToPlay?L"1":L"0");
		l_pDataTiXmlElement->SetAttribute(L"StopAtLastFrame",this->m_pMPDI->IsStayAtLastFrame()?L"1":L"0");
		l_pDataTiXmlElement->SetAttribute(L"Loop",this->m_pMPDI->IsAnimationLoop()?L"1":L"0");
		return l_pDataTiXmlElement;
		TiXmlElement*l_pEventObjectTiXmlElement = new TiXmlElement(L"EventObject");
		l_pEventObjectTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,L"MPDI");
		l_pEventObjectTiXmlElement->LinkEndChild(l_pDataTiXmlElement);
		return l_pEventObjectTiXmlElement;
	}
//end namespace
}