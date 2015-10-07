#ifndef _MAGIC_TOWER_SCENE_CONTROL_H_
#define _MAGIC_TOWER_SCENE_CONTROL_H_

#include "StageData.h"
#include "WalkingInMapPhase.h"

class	cSceneControl:public cPhaseManager
{
	cImageParser			m_ImageParser;

	//virtual	void	InternalRender(){}
	//virtual	void	InternalUpdate(float e_fElpaseTime){}
	cGameData							m_GameData;
	cObjectListByName<cStoreyData>		m_AllStoreyData;;
public:
	cSceneControl();
	~cSceneControl();
	virtual	bool	Load();
	void	KeyDown(char e_char);
};

#endif