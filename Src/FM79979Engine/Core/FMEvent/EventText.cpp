#include "../stdafx.h"
#include "EventText.h"
#include "EventManager.h"
namespace   FATMING_CORE
{
    const wchar_t*         cEventText::TypeID( L"cEventText" );
	const wchar_t*         cEventXmlText::TypeID( L"cEventXmlText" );
	cEventText::cEventText(Vector3 e_vPos,const WCHAR*e_strText,float e_fFontSize,Vector4 e_vColor):cEventObject(eEOT_TEXT,e_vPos,Vector2(1,1),e_vColor)
    {
		this->m_vSize.x = e_fFontSize;
        m_strText = e_strText;
    }

	cEventText::cEventText(TiXmlElement*e_pElement):cEventObject(eEOT_TEXT,Vector3::Zero)
	{
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("Text")
			{
				m_strText = l_strValue;
			}
			else
			COMPARE_NAME("FontSize")
			{
			    this->m_vSize.x = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("Pos")
			{
				this->SetLocalPosition(VALUE_TO_VECTOR3);
			}
			else
			COMPARE_NAME("Color")
			{
				m_vColor = VALUE_TO_VECTOR4;
			}
		PARSE_NAME_VALUE_END	
	}

    cEventText::cEventText(cEventText*e_pEvent_Text):cEventObject(e_pEvent_Text)
    {
		//this->m_vSize.x = e_fFontSize;
        m_strText = e_pEvent_Text->m_strText;
    }

    void    cEventText::InternalUpdate(float e_fElpaseTime)
    {

    }
    void    cEventText::InternalRender()
    {
        Vector3 l_vPos = GetPosWithPagePos();
		g_pCurrentEventManager->m_pEventFont->SetFontColor(m_vColor);
		g_pCurrentEventManager->m_pEventFont->SetScale(m_vSize.x);
        g_pCurrentEventManager->m_pEventFont->RenderFont(l_vPos.x,l_vPos.y,(WCHAR*)m_strText.c_str());
		g_pCurrentEventManager->m_pEventFont->SetScale(1.f);
		g_pCurrentEventManager->m_pEventFont->SetFontColor(Vector4::One);
    }
	//	<EventObject Type="Text">
	//		<Data Pos="200.00,200.00,0.00" Size="300,300" Type="Text" Text="79979"/>
	//	</EventObject>
	TiXmlElement*	cEventText::ToTiXmlElement()
	{
		TiXmlElement*l_pDataTiXmlElement = new TiXmlElement(L"Data");
		l_pDataTiXmlElement->SetAttribute(L"Pos",ValueToStringW(this->GetLocalPosition()));
		l_pDataTiXmlElement->SetAttribute(L"Size",ValueToStringW(this->m_vSize));
		l_pDataTiXmlElement->SetAttribute(L"Type",L"Text");
		l_pDataTiXmlElement->SetAttribute(L"Text",this->m_strText.c_str());
		return l_pDataTiXmlElement;
		TiXmlElement*l_pEventObjectTiXmlElement = new TiXmlElement(L"EventObject");
		l_pEventObjectTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,L"Text");
		l_pEventObjectTiXmlElement->SetAttribute(NAME,this->GetName());
		l_pEventObjectTiXmlElement->LinkEndChild(l_pDataTiXmlElement);
		return l_pEventObjectTiXmlElement;
	}

	cEventXmlText::cEventXmlText(TiXmlElement*e_pTiXmlElement):cEventObject(eEOT_TEXT,Vector3::Zero)
	{
		m_pNodeISAX = 0;
		m_bXmlFromPointer = false;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventXmlText::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME("AttributeDistance")
			{
				m_vAttributeDistance = VALUE_TO_VECTOR2;
			}
			else
			COMPARE_NAME("ViewRect")
			{
				m_vRenderRect = VALUE_TO_VECTOR4;
				this->SetLocalPosition(Vector3(m_vRenderRect.x,m_vRenderRect.y,0.f));
			}
			else
			COMPARE_NAME("ElementDistance")
			{
				m_vElementDistance = VALUE_TO_VECTOR2;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
			{
				m_pRenderIndexEventVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
			}
			else
			COMPARE_NAME("FileName")
			{
				m_strFileName = UT::WcharToChar(l_strValue);
				const char*l_strFileName = g_pCurrentEventManager->GetFullpathFileName(l_strValue).c_str();
				m_pNodeISAX = new cNodeISAX;
				bool	l_b = m_pNodeISAX->ParseDataIntoXMLNode(l_strFileName);
				if( !l_b )
				{
					//UT::ErrorMsg(e_strFileName,L"file open failed");
				}
			}
			else
			COMPARE_NAME("RootElementAttributePos")
			{
				m_vRootElementAttributePos = VALUE_TO_VECTOR2;
			}
			else
			COMPARE_NAME("ListPerPage")
			{
				m_iListPerPage = VALUE_TO_INT;
			}
		PARSE_NAME_VALUE_END
	}

	cEventXmlText::cEventXmlText(const WCHAR*e_strFileName,Vector4 e_vRenderRect,Vector2 e_vAttributeDistance,Vector2 e_vElementDistance,const WCHAR*e_strVariableName,Vector2 e_vRootElementAttributePos,int e_iListPerPage):cEventObject(eEOT_TEXT,Vector2(e_vRenderRect.x,e_vRenderRect.y))
	{
		m_bXmlFromPointer = false;
		m_vRootElementAttributePos = e_vRootElementAttributePos;
		m_pRenderIndexEventVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(e_strVariableName);
		m_vElementDistance = e_vElementDistance;
		m_vAttributeDistance = e_vAttributeDistance;
		m_vRenderRect = e_vRenderRect;
		m_vRootElementAttributePos = e_vRootElementAttributePos;
		m_iListPerPage = e_iListPerPage;
		const char*l_strFileName = g_pCurrentEventManager->GetFullpathFileName(e_strFileName).c_str();
		m_strFileName = l_strFileName;
		m_pNodeISAX = new cNodeISAX;
		bool	l_b = m_pNodeISAX->ParseDataIntoXMLNode(l_strFileName);
		if( !l_b )
		{
			//UT::ErrorMsg(e_strFileName,L"file open failed");
		}
	}
    cEventXmlText::cEventXmlText(cEventXmlText*e_pEventXmlText):cEventObject(e_pEventXmlText)
	{
		m_strFileName = e_pEventXmlText->m_strFileName;
		m_bXmlFromPointer = true;
		m_pRenderIndexEventVariable = e_pEventXmlText->m_pRenderIndexEventVariable;
		m_vElementDistance = e_pEventXmlText->m_vElementDistance;
		m_vAttributeDistance = e_pEventXmlText->m_vAttributeDistance;
		m_vRenderRect = e_pEventXmlText->m_vRenderRect;
		m_vRootElementAttributePos = e_pEventXmlText->m_vRootElementAttributePos;
		m_iListPerPage = e_pEventXmlText->m_iListPerPage;
		m_pNodeISAX = e_pEventXmlText->m_pNodeISAX;
	}

    cEventXmlText::~cEventXmlText()
	{
		if(!m_bXmlFromPointer)
			SAFE_DELETE(m_pNodeISAX);
	}

	void    cEventXmlText::InternalInit()
	{
		m_iCurrentRootChildElementIndex = 0;
		if( !m_pRenderIndexEventVariable )
		{
			UT::ErrorMsg(L"where is the event variable!?",L"please add correct event variable");
		}
		else
		{
			if(m_pRenderIndexEventVariable->GetDataType() != eDT_INT )
			{
				UT::ErrorMsg(L"event variable data type is wrong!?",L"please set correct data type");
			}
			else
			{
				m_pRenderIndexEventVariable->SetDataByString(L"0");
			}
		}
		m_iRootChildrenCount = 0;
		TiXmlElement*l_pElement = m_pNodeISAX->GetRootElement();
		if( l_pElement )
		{
			m_iRootChildrenCount = m_pNodeISAX->RootChildrenCount();
			l_pElement = m_pNodeISAX->GetRootElement()->FirstChildElement();
			if( m_iListPerPage == 0 && l_pElement )
			{
				Vector2	l_vSize = g_pCurrentEventManager->m_pEventFont->GetRenderSize(L"0");
				float	l_fHeight = l_vSize.y+this->m_vElementDistance.y;
				this->m_iListPerPage = (int)(this->m_vRenderRect.Height()/l_fHeight);
			}
		}
		//if( m_iRootChildrenCount>0 )
		//	m_pRenderIndexEventVariable->SetInt(m_iRootChildrenCount-1);
		ToLastPage();
	}

	void	cEventXmlText::ToLastPage()
	{
		int	l_iIndex = 0;
		if( m_iRootChildrenCount%m_iListPerPage == 0 )
		{
			l_iIndex = (m_iRootChildrenCount/m_iListPerPage)-1;
		}
		else
		{
			l_iIndex = (m_iRootChildrenCount/m_iListPerPage);
		}
		m_pRenderIndexEventVariable->SetDataByString(ValueToStringW(l_iIndex));
	}

	void    cEventXmlText::InternalUpdate(float e_fElpaseTime)
	{
		if( m_iListPerPage == 0 )
			return;
		TiXmlElement*l_pTiXmlElement = this->m_pNodeISAX->GetRootElement();
		if( l_pTiXmlElement )
		{
			int	l_iCurrentIndex = *m_pRenderIndexEventVariable->GetInt();
			if( l_iCurrentIndex < 0 )
			{//set to max
				if( m_iRootChildrenCount>m_iListPerPage )
				{
					ToLastPage();
				}
				else
					m_pRenderIndexEventVariable->SetDataByString(L"0");
			}
			else
			if( m_iListPerPage*l_iCurrentIndex >= m_iRootChildrenCount )
			{
				m_pRenderIndexEventVariable->SetDataByString(L"0");
			}
		}
	}

	void    cEventXmlText::InternalRender()
	{
		if( m_iListPerPage == 0 )
			return;
		TiXmlElement*l_pTiXmlElement = this->m_pNodeISAX->GetRootElement();
		if( l_pTiXmlElement )
		{
			int	l_iIndex = *m_pRenderIndexEventVariable->GetInt();
			TiXmlElement*l_pTiXmlElement = m_pNodeISAX->GetRootChildElementByIndex(l_iIndex*m_iListPerPage);
			if( l_pTiXmlElement )
			{
				Vector2	l_vStartPos(m_vRootElementAttributePos.x,m_vRootElementAttributePos.y);
				for( int i=0;i<m_iListPerPage;++i )
				{
					TiXmlAttribute*	l_pAttribute = l_pTiXmlElement->FirstAttribute();
					Vector2	l_vElementPos(l_vStartPos);
					const WCHAR*	l_strValue = 0;
					const WCHAR*	l_strName = 0;
					while(l_pAttribute)
					{
						l_strValue = l_pAttribute->Value();
						//l_strName = l_pAttribute->Name();
						Vector4		l_vDrawSize;
						g_pCurrentEventManager->m_pEventFont->RenderFont(l_vElementPos.x,l_vElementPos.y,l_strValue,&l_vDrawSize);
						l_pAttribute = l_pAttribute->Next();
						l_vElementPos += m_vAttributeDistance;
						//l_vElementPos.x += l_vDrawSize.Width();
					}
					l_vStartPos += m_vElementDistance;
					l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
					if( !l_pTiXmlElement )
						break;
				}
			}
		}
		else
		{
			g_pCurrentEventManager->m_pEventFont->RenderFont(m_vRootElementAttributePos.x,m_vRootElementAttributePos.y,L"file not exists");
		}
	}

	TiXmlElement*	cEventXmlText::ToTiXmlElement()
	{
		TiXmlElement*l_pDataTiXmlElement = new TiXmlElement(L"Data");
		l_pDataTiXmlElement->SetAttribute(L"AttributeDistance",ValueToStringW(this->m_vAttributeDistance));
		l_pDataTiXmlElement->SetAttribute(L"ViewRect",ValueToStringW(this->m_vRenderRect));
		l_pDataTiXmlElement->SetAttribute(L"ElementDistance",ValueToStringW(this->m_vElementDistance));
		if( m_pRenderIndexEventVariable )
			l_pDataTiXmlElement->SetAttribute(cEventVariable::TypeID,m_pRenderIndexEventVariable->GetName());
		l_pDataTiXmlElement->SetAttribute(L"FileName",UT::CharToWchar(m_strFileName.c_str()));
		l_pDataTiXmlElement->SetAttribute(L"RootElementAttributePos",ValueToStringW(m_vRootElementAttributePos));
		l_pDataTiXmlElement->SetAttribute(L"ListPerPage",ValueToStringW(m_iListPerPage));
		return l_pDataTiXmlElement;
		TiXmlElement*l_pEventObjectTiXmlElement = new TiXmlElement(L"EventObject");
		l_pEventObjectTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,L"Text");
		l_pEventObjectTiXmlElement->SetAttribute(NAME,this->GetName());
		l_pEventObjectTiXmlElement->LinkEndChild(l_pDataTiXmlElement);
		return l_pEventObjectTiXmlElement;
	}
//end namespace   FATMING_CORE
}