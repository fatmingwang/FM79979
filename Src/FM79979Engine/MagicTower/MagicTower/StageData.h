#ifndef _MAGIC_TOWER_STAGE_DATA_H_
#define _MAGIC_TOWER_STAGE_DATA_H_

#include "LevelData.h"
#include "MainCharacterBehavior.h"
#include "../../Core/FMLevelData/AllLevelDataInclude.h"


//<Data>
//	<ImageSetupFile FileName=""/>
//	<MapDataFile FileName="" TMPFile="" />
//	<UserData FileName="" />
//</Data>
class	cGameData:public ISAXCallback
{
	//each stage data,all position and name data,xml node from gamelevel,xml node data
	void	ProcessLevelData();
	void	ProcessViewableSizeData();
	//
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
public:
	cGameData();
	~cGameData();
	cLevelData					m_LevelData;
	//get data from cTMPData,not done yet
	//cCellData*					GetTargetCellData(WCHAR*e_strName){ return 0; }
	POINT						m_ViewableSize;
	POINT						m_MainRoleFocusGridIndex;
};

#endif