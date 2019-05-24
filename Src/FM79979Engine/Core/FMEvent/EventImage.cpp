#include "../stdafx.h"
#include "EventObject.h"
#include "EventImage.h"
#include "EventManager.h"
namespace   FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cEventImage);
    cEventImage::cEventImage(Vector3 e_vPos,Vector2 e_vSize,cBaseImage*e_pImage):cEventObject(eEOT_IMAGE,e_vPos,e_vSize)
    {
        m_pImage = e_pImage;
    }
    
    cEventImage::cEventImage(cEventImage*e_pEvent_PIUnit):cEventObject(e_pEvent_PIUnit)
    {
        m_pImage = e_pEvent_PIUnit->m_pImage;
    }

	cEventImage::cEventImage(TiXmlElement*e_pElement):cEventObject(eEOT_IMAGE,Vector3::Zero)
	{
		cPuzzleImage*l_pPI = 0;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME_WITH_DEFINE(cBaseImage::TypeID)
		    {
				m_pImage = dynamic_cast<cBaseImage*>(cGameApp::m_spImageParser->GetObject(l_strValue));
		        if(!m_pImage)
		        {
					std::string l_str = *g_pCurrentEventManager->m_pstrWorkingDirectory;
			        l_str += ".png";
					this->m_pImage = new cBaseImage((char*)l_str.c_str());
					cGameApp::m_spImageParser->AddObjectNeglectExist(m_pImage);
		        }
		    }
		    else
			COMPARE_NAME_WITH_DEFINE(cPuzzleImage::TypeID)
			{
			    l_pPI = dynamic_cast<cPuzzleImage*>(cGameApp::m_spImageParser->GetObject(l_strValue));
			    if( !l_pPI )
			    {
					std::string	l_strFileName = g_pCurrentEventManager->GetFullpathFileName(l_strValue);
					const char*l_strPIName = cPuzzleImage::GetFileName(l_strFileName.c_str()).c_str();
			        //char*l_strFullfileName = FileNameConbineCurrentPageDirectory(l_strPIName);
			        //bool    l_b = cGameApp::m_spImageParser->Parse(l_strFullfileName);
			        std::string l_str = *g_pCurrentEventManager->m_pstrWorkingDirectory;
			        l_str += l_strPIName;
					l_pPI = cGameApp::m_spImageParser->GetPuzzleImageByFileName((char*)l_str.c_str());
			        assert(l_pPI&&"get PI failed!");
			    }
			}
			else
			COMPARE_NAME_WITH_DEFINE(cPuzzleImageUnit::TypeID)
			{
			    m_pImage = l_pPI->GetObject(l_strValue);
			}
			else
			COMPARE_NAME("Pos")
			{
				this->SetLocalPosition(VALUE_TO_VECTOR3);
			}
			else
			COMPARE_NAME("Size")
			{
			    m_vSize = VALUE_TO_VECTOR2;
			}
		PARSE_NAME_VALUE_END	
	}

    void    cEventImage::InternalRender()
    {
		m_pImage->SetWidth((int)m_vSize.x);
		m_pImage->SetHeight((int)m_vSize.y);
		Vector3 l_vPos = GetPosWithPagePos();
        m_pImage->Render((int)l_vPos.x,(int)l_vPos.y);
    }

    NamedTypedObject*   cEventImage::GetResourcePointer()
    {
        if( m_pImage->Type() == cBaseImage::TypeID )
            return (NamedTypedObject*)m_pImage;
        else
        if( m_pImage->Type() == cPuzzleImageUnit::TypeID )
            return m_pImage->GetOwner();
        return 0;
    }
	//	<EventObject Type="Image">
		//<Data Pos="200.00,200.00" Size="300,300" PI="SelectMode" PIUnit="2" />
	//	</EventObject>
	//	<EventObject Type="Image">
		//<Data Pos="200.00,200.00" Size="300,300" BaseImage="SelectMode" />
	//	</EventObject>
	TiXmlElement*	cEventImage::ToTiXmlElement()
	{
		TiXmlElement*l_pDataTiXmlElement = new TiXmlElement(L"Data");
		l_pDataTiXmlElement->SetAttribute(L"Pos",ValueToStringW(this->GetLocalPosition()));
		l_pDataTiXmlElement->SetAttribute(L"Size",ValueToStringW(*this->GetSize()));
		if(cBaseImage::TypeID == this->m_pImage->Type() )
			l_pDataTiXmlElement->SetAttribute(L"BaseImage",this->m_pImage->GetName());
		else
		{
			l_pDataTiXmlElement->SetAttribute(L"PI",m_pImage->GetOwner()->GetName());
			l_pDataTiXmlElement->SetAttribute(L"PIUnit",this->m_pImage->GetName());
		}
		return l_pDataTiXmlElement;
		TiXmlElement*l_pEventObjectTiXmlElement = new TiXmlElement(L"EventObject");
		l_pEventObjectTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,L"Image");
		l_pEventObjectTiXmlElement->LinkEndChild(l_pDataTiXmlElement);
		return l_pEventObjectTiXmlElement;
	}
//end namespace
}