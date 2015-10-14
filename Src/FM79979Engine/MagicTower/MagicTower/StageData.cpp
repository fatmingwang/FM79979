#include "stdafx.h"
#include "StageData.h"

cGameData::cGameData()
{
	m_ViewableSize.x = m_MainRoleFocusGridIndex.x = 5;
	m_ViewableSize.y = m_MainRoleFocusGridIndex.y = 5;

}

cGameData::~cGameData()
{

}

//all image we need to load
void	cGameData::ProcessLevelData()
{
	const WCHAR*l_strFileName = this->m_pCurrentTiXmlElement->Attribute(L"FileName");
	if(!m_LevelData.ParseWithMyParse( UT::WcharToChar(l_strFileName).c_str() ))
	{
		UT::ErrorMsg(l_strFileName,L"parse failed");
	}
}

void	cGameData::ProcessViewableSizeData()
{
	PARSE_CURRENT_ELEMENT_START
		COMPARE_NAME("Row")
		{
			m_ViewableSize.x = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("Column")
		{
			m_ViewableSize.y = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("MainRoleFocusGridIndex")
		{
			m_MainRoleFocusGridIndex = VALUE_TO_POINT;
		}
	PARSE_NAME_VALUE_END
}
//
//<Data>
//	<ImageSetupFile FileName=""/>
//	<MapDataFile FileName="" TMPFile="" />
//	<UserData FileName="" />
//</Data>

void	cGameData::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	if(e_pTiXmlElement->m_bDone)
		return;
	if(!wcscmp(l_strValue,L"LevelData"))
	{
		ProcessLevelData();
	}
	if(!wcscmp(l_strValue,L"ViewableUnits"))
	{
		ProcessViewableSizeData();
	}
}