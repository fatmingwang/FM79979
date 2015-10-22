#include "stdafx.h"
#include "RegionMap.h"
#include "RegionMapChange.h"
#include "RegionMapInfo.h"
#include "TradeRoutes.h"
extern char*g_strSelectDirectory;

cRegionMap::cRegionMap(TiXmlElement*e_pElement)
{
	m_pRegionMapPointInfoManager = new cRegionMapPointInfoManager();
	m_pTradeRoutesManager = new cTradeRoutesManager();
	if( e_pElement )
	{
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("Name")
			{
				this->SetName(l_strValue);
				m_iID = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("MapInfoFile")
			{
				std::string	l_strFileName = g_strSelectDirectory;
				l_strFileName += "/";
				l_strFileName += UT::WcharToChar(l_strValue).c_str();
				PARSE_FILE_CHECK(m_pRegionMapPointInfoManager,l_strFileName.c_str());
			}
			else
			COMPARE_NAME("MapTradeRouteFile")
			{
				std::string	l_strFileName = g_strSelectDirectory;
				l_strFileName += "/";
				l_strFileName += UT::WcharToChar(l_strValue).c_str();
				PARSE_FILE_CHECK(m_pTradeRoutesManager,l_strFileName.c_str());
			}
			else
			COMPARE_NAME("FontFileName")
			{
				//l_strFontFileName = l_pAttribute->Value();
			}
		PARSE_NAME_VALUE_END			
		//e_pElement = e_pElement->FirstChildElement();
		//while( e_pElement )
		//{
		//	const char*l_strValue = e_pElement->Value();
		//	ELEMENT_VALUE_COMPARE("RegionsButton")
		//	{
		//		GenerateRegionsButton(e_pElement,l_pRegionsButtonLayer);
		//	}
		//	else
		//	ELEMENT_VALUE_COMPARE("ZoomData")
		//	{
		//		GenerateZoomButton(e_pElement);
		//	}
		//	else
		//	ELEMENT_VALUE_COMPARE("MapImage")
		//	{
		//		GenerateCameraClickEvent(e_pElement);
		//	}
		//	e_pElement = e_pElement->NextSiblingElement();
		//}
	}
}


cRegionMap::~cRegionMap()
{
	SAFE_DELETE(m_pRegionMapPointInfoManager);
	SAFE_DELETE(m_pTradeRoutesManager);
}

void	cRegionMap::GenerateRegionsButton(TiXmlElement*e_pElement)
{
	const WCHAR*l_strTargetMapName = e_pElement->Attribute(L"TargetMapName");
	const WCHAR*l_strText = e_pElement->Attribute(L"Text");
}