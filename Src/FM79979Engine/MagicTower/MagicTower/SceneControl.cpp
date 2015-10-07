#include "stdafx.h"
#include "SceneControl.h"

cSceneControl::cSceneControl()
{

}

cSceneControl::~cSceneControl()
{

}

bool	cSceneControl::Load()
{
	WCHAR	l_str[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,l_str);
	bool l_b = m_GameData.Parse("MagicTower/Data.xml");
	assert(l_b);
	int	l_iTotalStorey = m_GameData.m_LevelData.Count();
	assert(l_b);
	POINT	l_vGridSize;
	for( int i=0;i<l_iTotalStorey;++i )
	{
		cLevelLayerList*l_pLevelLayerData = m_GameData.m_LevelData[i];
		l_vGridSize = l_pLevelLayerData->GetGridSize();
		Vector2	l_vSize = l_pLevelLayerData->GetSize();
		int	l_iWidth = (int)(l_vSize.x/l_vGridSize.x);
		int	l_iHeight = (int)(l_vSize.y/l_vGridSize.y);
		int	l_iSize = l_iWidth*l_iHeight;
		cStoreyData*l_pStoreyData = new cStoreyData(l_pLevelLayerData);
		l_pStoreyData->SetName(l_pLevelLayerData->GetName());
		this->m_AllStoreyData.AddObject(l_pStoreyData);
	}

	cWalkingInMapPhase*l_pWalkingInMapPhase = new cWalkingInMapPhase(cMagicTowerApp::m_spMainRoleData,&m_AllStoreyData,m_GameData.m_ViewableSize.x,m_GameData.m_ViewableSize.y,l_vGridSize,m_GameData.m_MainRoleFocusGridIndex,L"1");
	l_b = this->AddObject(l_pWalkingInMapPhase);
	assert(l_b);
	this->SetCurrentCurrentPhase(l_pWalkingInMapPhase->GetName());
	return true;
}

void	cSceneControl::KeyDown(char e_char)
{
	cWalkingInMapPhase*l_pWalkingInMapPhase = (cWalkingInMapPhase*)(*this)[0];
	l_pWalkingInMapPhase->KeyDown(e_char);
}