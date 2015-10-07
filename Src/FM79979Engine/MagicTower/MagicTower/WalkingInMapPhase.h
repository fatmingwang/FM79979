#ifndef _MAGIC_TOWER_WALKING_IN_MAP_PHASE_H_
#define _MAGIC_TOWER_WALKING_IN_MAP_PHASE_H_

#include "LevelData.h"
#include "MainCharacterBehavior.h"
#include "MapSceneRule.h"
#include "VirtualGamePad.h"

class cWalkingInMapPhase:public cSimplePhase
{
	cGridBehavior*					m_pMonsterGridBehavior;
	POINT							m_GridSize;
	cVirtualGamePad					*m_pVirtualGamePad;
	cMainRoleData					*m_pMainRoleData;
	cObjectListByName<cStoreyData>	*m_pAllStoreyData;
	cStoreyData						*m_pCurrentStoreyData;
	cMapSceneViewRule					*m_pMapSceneRule;
	//the scene we want to see,it could be scaled
	Vector2	m_vVisibleResoulution;
	GET_SET_DEC(cOrthogonalCamera,m_OrthogonalCamera,GetOrthogonalCamera,SetOrthogonalCamera);
	//while main character reached stair or hole or some teleportion it will set new storey and set character position
	void	ChangeFloor();
	GET_SET_DEC(bool,m_bWalkingModeIsFix,IsWalkingModeIsFix,SetWalkingModelToFix);
public:
	cWalkingInMapPhase(cMainRoleData*e_pMainRoleData,cObjectListByName<cStoreyData>*e_pAllStoreyData,
		int e_iViewableRow,int e_iViewableColumn,POINT e_GridSize,POINT e_iOffsetMapPosIndex,WCHAR*e_strCurrentStoreyName);
	~cWalkingInMapPhase();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Init();							//it would be called if the stage is changed.
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseHover(int e_iX,int e_iY);
	void	KeyDown(char e_char);
	virtual	const   WCHAR*	GetNextPhaseName();
	virtual	void*	GetData();
};

#endif