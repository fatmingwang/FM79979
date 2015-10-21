#include "../stdafx.h"
#include "EventXMLNodeText.h"
#include "EventManager.h"

namespace   FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cEVentXMLNodeText);

	cEVentXMLNodeText::cEVentXMLNodeText(Vector3 e_vPos,Vector4 e_vColor):cEventObject(eEOT_XML_TEXT,e_vPos,Vector2(1,1),e_vColor)
	{
	
	}

	cEVentXMLNodeText::cEVentXMLNodeText(cEVentXMLNodeText*e_pEVentXMLNodeText):cEventObject(e_pEVentXMLNodeText)
	{
	
	}

	cEVentXMLNodeText::~cEVentXMLNodeText()
	{
	
	}

	void    cEVentXMLNodeText::InternalInit()
	{
	
	}

	void    cEVentXMLNodeText::InternalUpdate(float e_fElpaseTime)
	{
	
	}

	void    cEVentXMLNodeText::InternalRender()
	{
	
	}

	TiXmlElement*	cEVentXMLNodeText::ToTiXmlElement()
	{
		return 0;
	}

}