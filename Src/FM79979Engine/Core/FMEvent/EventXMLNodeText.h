#ifndef _EVENT_XML_TEXT_H_
#define _EVENT_XML_TEXT_H_

#include "EventObject.h"

namespace   FATMING_CORE
{
	//
	//parse a xml and svae it into EventManger,
	//get text from m_strFullPath.
	//
	//
	class	cEVentXMLNodeText:public cEventObject
	{
		//
		cNodeISAX*		m_pNodeISAX;
		//find final path from root to target node
		std::vector<std::wstring>	m_strFullPath;
		std::vector<std::wstring>	m_strText;
		//
		virtual void    InternalInit()override;
		virtual void    InternalUpdate(float e_fElpaseTime)override;
		virtual void    InternalRender()override;
	public:
		DEFINE_TYPE_INFO();
		cEVentXMLNodeText(Vector3 e_vPos,Vector4 e_vColor);
		cEVentXMLNodeText(cEVentXMLNodeText*e_pEVentXMLNodeText);
		virtual ~cEVentXMLNodeText();
        EVENT_CLONE_DEFINE(cEVentXMLNodeText);
		virtual	TiXmlElement*	ToTiXmlElement()override;
		virtual NamedTypedObject*   GetResourcePointer()override {return 0;}
	};
}

#endif