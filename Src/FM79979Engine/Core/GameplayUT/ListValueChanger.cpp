#include "../stdafx.h"
#include "ListValueChanger.h"
namespace FATMING_CORE
{
	TiXmlElement*				cListValuChangerBase::ToTiXmlElement()
	{
		if( !wcscmp(m_strType,L"cListValueChanger") )
		{
			TiXmlElement*l_pTiXmlElement = new TiXmlElement(m_strType);
			l_pTiXmlElement->SetAttribute(DATA_TYPE,ValueToStringW(m_eDataType));
			l_pTiXmlElement->SetAttribute(L"Loop",m_bLoop?1:0);
			SetAttribute(l_pTiXmlElement);
			return l_pTiXmlElement;
		}
		return 0;
	}

	cListValuChangerBase*	cListValuChangerBase::GetListValuChangerBaseByElement(TiXmlElement*e_pElement)
	{
		cListValuChangerBase*l_pListValuChangerBase = 0;
		eDataType	l_eDataType = UT::GetDataType(ATTRIBUT_VALUE_WITH_DEFINE(e_pElement,DATA_TYPE));
		switch( l_eDataType )
		{
				case eDT_INT:
					l_pListValuChangerBase = new cListValueChanger<int>(e_pElement);
					break;
				case eDT_STRING:
					l_pListValuChangerBase = new cListValueChanger<std::string>(e_pElement);
					break;
				case eDT_WSTRING:
					l_pListValuChangerBase = new cListValueChanger<std::wstring>(e_pElement);
					break;
				case eDT_BYTE:
					l_pListValuChangerBase = new cListValueChanger<char>(e_pElement);
					break;
				case eDT_DOUBLE:
					l_pListValuChangerBase = new cListValueChanger<double>(e_pElement);
					break;
				case eDT_FLOAT:
					l_pListValuChangerBase = new cListValueChanger<float>(e_pElement);
					break;
				default:
					assert(0&&"not support cListValuChangerBase::GetListValuChangerBaseByElement");
					break;
		}
		return l_pListValuChangerBase;
	}
}