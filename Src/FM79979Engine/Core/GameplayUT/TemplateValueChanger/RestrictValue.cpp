#include "RestrictValue.h"

namespace FATMING_CORE
{
	TiXmlElement*				cRestrictValueBase::ToTiXmlElement()
	{
		if( !wcscmp(m_strType,L"cRestrictValue") )
		{
			TiXmlElement*l_pTiXmlElement = new TiXmlElement(m_strType);
			l_pTiXmlElement->SetAttribute(DATA_TYPE,ValueToStringW(m_eDataType));
			l_pTiXmlElement->SetAttribute(L"Loop",m_bValueLoop?1:0);
			SetAttribute(l_pTiXmlElement);
			return l_pTiXmlElement;
		}
		return 0;
	}
	cRestrictValueBase*	cRestrictValueBase::GetRestrictValueBaseByElement(TiXmlElement*e_pElement)
	{
		cRestrictValueBase*l_pRestrictValueBase = 0;
		eDataType	l_eDataType = UT::GetDataType(ATTRIBUT_VALUE_WITH_DEFINE(e_pElement,DATA_TYPE));
		switch( l_eDataType )
		{
				case eDT_INT:
					l_pRestrictValueBase = new cRestrictValue<int>(e_pElement);
					break;				
				case eDT_BYTE:
					l_pRestrictValueBase = new cRestrictValue<char>(e_pElement);
					break;
				case eDT_DOUBLE:
					l_pRestrictValueBase = new cRestrictValue<double>(e_pElement);
					break;
				case eDT_FLOAT:
					l_pRestrictValueBase = new cRestrictValue<float>(e_pElement);
					break;
				default:
					assert(0 &&"cRestrictValueBase::GetRestrictValueBaseByElement not support format");
					break;
		}
		return l_pRestrictValueBase;
	}
}