#include "../stdafx.h"
#include "EventObject.h"
#include "EventVariable.h"
#include "EventVariableDataRender.h"
#include "EventManager.h"

namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cEventVariableDataRender);

	cEventVariableDataRender::cEventVariableDataRender(TiXmlElement*e_pTiXmlElement):cEventObject(eEOT_VARIABLE_RENDER,Vector3::Zero)
	{
		m_eImageAnchorType = eIAT_LEFT_CENTER;
		Vector3	l_vPos;
		cEventVariable*l_pEventVariable = 0;
		std::wstring	l_strPrefix;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
			{
				m_pEventVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
			}
			else
				COMPARE_NAME("Pos")
			{
				this->SetLocalPosition(VALUE_TO_VECTOR3);
			}
			else
				COMPARE_NAME("PrefixText")
			{
				m_strPrefixText = l_strValue;
			}
			else		
				COMPARE_NAME("AnchorType")
			{
				m_eImageAnchorType = eIAT_RIGHT_CENTER;
			}
		PARSE_NAME_VALUE_END
	}

	cEventVariableDataRender::cEventVariableDataRender(cEventVariable*e_pNameAndData,Vector3 e_vPos,const WCHAR*e_strPrefixText):cEventObject(eEOT_VARIABLE_RENDER,e_vPos)
	{
		if( e_strPrefixText&&wcslen(e_strPrefixText)>0 )
			m_strPrefixText = e_strPrefixText;
		m_pEventVariable = e_pNameAndData;
	}

	cEventVariableDataRender::cEventVariableDataRender(cEventVariableDataRender*e_pEventVariableDataRender):cEventObject(e_pEventVariableDataRender)
	{
		m_eImageAnchorType = e_pEventVariableDataRender->m_eImageAnchorType;
		m_pEventVariable = e_pEventVariableDataRender->m_pEventVariable;
		m_strPrefixText = e_pEventVariableDataRender->m_strPrefixText;
	}

	cEventVariableDataRender::~cEventVariableDataRender()
	{

	}

	NamedTypedObject*           cEventVariableDataRender::GetResourcePointer(){ return 0; }

	void    cEventVariableDataRender::InternalRender()
	{
		std::wstring	l_str = m_pEventVariable->ConvertToString();
		Vector2			l_vRenderSizeOffset = g_pCurrentEventManager->m_pEventFont->GetRenderSize(l_str.c_str());
		if(m_eImageAnchorType)
		{
			//if( m_eImageAnchorType == eIAT_RIGHT_CENTER)
			//	l_vRenderSize.x = 0.f;
			if( m_eImageAnchorType == eIAT_LEFT_CENTER)
				l_vRenderSizeOffset.x = 0.f;
		}
		Vector3	l_vPos = this->GetWorldPosition();
		if(this->m_pEventVariable)
		{
			if( m_strPrefixText.length() )
			{
				Vector4	l_vDrawRect;
				g_pCurrentEventManager->m_pEventFont->RenderFont(l_vPos.x,l_vPos.y,m_strPrefixText.c_str(),&l_vDrawRect);
				std::wstring	l_str = m_pEventVariable->ConvertToString();
				g_pCurrentEventManager->m_pEventFont->RenderFont(l_vDrawRect.z-l_vRenderSizeOffset.x,l_vDrawRect.y,l_str.c_str());
			}
			else
			{
				g_pCurrentEventManager->m_pEventFont->RenderFont(l_vPos.x-l_vRenderSizeOffset.x,l_vPos.y,l_str.c_str());
			}
		}
		else
		{
			g_pCurrentEventManager->m_pEventFont->RenderFont(l_vPos.x-l_vRenderSizeOffset.x,l_vPos.y,L"m_pEventVariable not exist");
		}
	}
	TiXmlElement*	cEventVariableDataRender::ToTiXmlElement()
	{
		TiXmlElement*l_pDataTiXmlElement = new TiXmlElement(cEventVariableDataRender::TypeID);
		if( m_pEventVariable )
			l_pDataTiXmlElement->SetAttribute(cEventVariable::TypeID,this->m_pEventVariable->GetName());
		l_pDataTiXmlElement->SetAttribute(L"Pos",ValueToStringW(this->GetLocalPosition()));
		l_pDataTiXmlElement->SetAttribute(L"PrefixText",this->m_strPrefixText.c_str());
		return l_pDataTiXmlElement;

		TiXmlElement*l_pEventObjectTiXmlElement = new TiXmlElement(L"EventObject");
		l_pEventObjectTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,L"VariableRender");
		l_pEventObjectTiXmlElement->SetAttribute(NAME,this->GetName());
		l_pEventObjectTiXmlElement->LinkEndChild(l_pDataTiXmlElement);
		return l_pEventObjectTiXmlElement;
	}
	//end namespace
}