#include "../stdafx.h"
#include "EventObject.h"
#include "EventSound.h"
#include "EventManager.h"
namespace   FATMING_CORE
{
    const wchar_t*         cEventSound::TypeID( L"cEventSound" );
	cEventSound::cEventSound(TiXmlElement*e_pElement):cEventObject(eEOT_SOUND)
	{
		m_bPlayed = false;
		m_pBasicSound = 0;
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cEventSound::TypeID);
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("SoundFileName")
			{
			    m_pBasicSound = cGameApp::m_spSoundParser->GetObject(UT::GetFileNameWithoutFullPath((WCHAR*)l_strValue));
                if(!m_pBasicSound)
                {
                    std::string l_strFileName = *g_pCurrentEventManager->m_pstrWorkingDirectory;
                    l_strFileName += UT::WcharToChar(l_strValue);
					//std::string l_strFileName = UT::WcharToChar(l_strValue);
					cGameApp::OutputDebugInfoString(l_strValue);
					cGameApp::OutputDebugInfoString(L"   AddStaticSound\n");
                    if(cGameApp::m_spSoundParser->AddStaticSound(this,(char*)l_strFileName.c_str()))
                    {
                        m_pBasicSound = cGameApp::m_spSoundParser->GetObject(cGameApp::m_spSoundParser->Count()-1);
						
                    }
                    else
                    {
                        UT::ErrorMsg(L"parse sound file error",(WCHAR*)l_strValue);
                    }
                }
				else
				{
					m_pBasicSound->AddRef(this);
					cGameApp::OutputDebugInfoString(l_strValue);
					cGameApp::OutputDebugInfoString(L"   AddRef\n");
				}
			}
			else
			COMPARE_NAME("loop")
			{
				if( m_pBasicSound )
					m_pBasicSound->SetLoop(VALUE_TO_BOOLEAN);
			}
			else
			COMPARE_NAME("FileResourceKeep")
			{
				//if( m_pBasicSound )
				//	m_pBasicSound->SetLoop(VALUE_TO_BOOLEAN);
			}
			else
			COMPARE_NAME("StopWhileLeave")
			{
				if( m_pBasicSound )
					m_pBasicSound->SetStopWhileLeave(VALUE_TO_BOOLEAN);
			}
		PARSE_NAME_VALUE_END
	}

    cEventSound::cEventSound(cBasicSound*e_pBasicSound):cEventObject(eEOT_SOUND)
    {
        m_pBasicSound = e_pBasicSound;
		m_pBasicSound->AddRef(this);
    }
    
    cEventSound::cEventSound(cEventSound*e_pEventSound):cEventObject(e_pEventSound)
    {
        m_pBasicSound = e_pEventSound->m_pBasicSound;
		m_pBasicSound->AddRef(this);
    }
    
	cEventSound::~cEventSound()
	{
		//fo it at collectresource
		int	l_iCount = m_pBasicSound->GetReferenceCount();
		if( l_iCount == 1 )
		{
			cGameApp::m_spSoundParser->RemoveObjectWithoutDelete(m_pBasicSound);
		}
		m_pBasicSound->Play(false);
		m_pBasicSound->Release(this);
		cGameApp::OutputDebugInfoString(m_pBasicSound->GetName());
		cGameApp::OutputDebugInfoString(L"   Release\n");
	}

    void    cEventSound::InternalMouseUp(int e_iPosX,int e_iPosY)
    {
        //this->m_pBasicSound->Play(true);
    }
    
    bool    cEventSound::InternalCollide(int e_iPosX,int e_iPosY)
    {
        return true;
    }
    
    void    cEventSound::InternalInit()
    {
        if( m_pBasicSound )
        {
			m_pBasicSound->Play(false);
            if(m_pBasicSound->IsLoop())
            {
                //m_pBasicSound->Play(true);
            }
        }
		m_bPlayed = false;
    }
    
    void    cEventSound::Destroy()
    {
        //if( m_pBasicSound )
        //{
        //    if(m_pBasicSound->IsLoop())
        //    {
        //        m_pBasicSound->Play(false);
        //    }
        //}
    }

	void    cEventSound::InternalUpdate(float e_fElpaseTime)
	{
        if( m_pBasicSound )
        {
			//if( !m_pBasicSound->IsPlay() && !this->m_bPlayed )
			if( !this->m_bPlayed )
			{
				m_pBasicSound->Play(true);
				m_bPlayed = true;
			}
        }
	}
//================================
//
//================================
    NamedTypedObject*   cEventSound::GetResourcePointer()
    {
		return 0;
        return (NamedTypedObject*)m_pBasicSound;
    }

	//<EventObject Type="Sound ">
	//	<Data SoundFileName="xxx.wav" loop="0"/>
	//</EventObject>
	TiXmlElement*	cEventSound::ToTiXmlElement()
	{
		TiXmlElement*l_pDataTiXmlElement = new TiXmlElement(L"Data");
		std::wstring	l_strSoundFileName = this->m_pBasicSound->GetName();
		l_strSoundFileName += L".wav";
		l_pDataTiXmlElement->SetAttribute(L"SoundFileName",l_strSoundFileName.c_str());
		l_pDataTiXmlElement->SetAttribute(L"loop",m_pBasicSound->IsLoop()?1:0);
		return l_pDataTiXmlElement;
		TiXmlElement*l_pEventObjectTiXmlElement = new TiXmlElement(L"EventObject");
		l_pEventObjectTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,L"Sound");
		l_pEventObjectTiXmlElement->LinkEndChild(l_pDataTiXmlElement);
		return l_pEventObjectTiXmlElement;
	}
//end namespace
}