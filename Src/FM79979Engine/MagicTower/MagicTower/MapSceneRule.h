#ifndef	_MAGIC_TOWER_MAP_SCENE_RULE_H_
#define	_MAGIC_TOWER_MAP_SCENE_RULE_H_

#include "LevelData.h"
#include "MainCharacterBehavior.h"
//a map contian a lot units,here is the rule to divide it into a small partition,
//so the user can control character to move in a part of map,and only could see a part it map,
//because somethimes the map is too big,so we have to divide it into a suitable size

	class	cMapSceneViewRule
	{
		cStoreyData*m_pStoreyData;
		cMainRoleData*m_pMainRoleData;

		int	m_iViewableRow;
		int	m_iViewableColumn;
		//map pos index,left up right down
		int	m_iCurrentMapWall[4];
		bool	m_bDirty;
		//the row and column here are represent blocks,
		//ex viewable row is 5 viewable column us 7
		//and total row is 2 total column is 2
		//it's mean 4 blocks(2*2)
		//and we have 2*5 = total row,
		//and we have 2*7 = total column,
		int		m_iCurrentRow;
		int		m_iCurrentColumn;
		int		m_iTotalRowBlocks;
		int		m_iTotalColumnBlocks;
		POINT	m_GridSize;
		//set main character always in the map's render position
		POINT	m_iOffsetMapPosIndex;
		//
		virtual	void	RenderWithOffsetIndesForFixScreen(int e_iOffsetIndex);
		virtual	void	RenderWithOffsetIndesForScrollableScreen(POINT e_OffsetIndex);
	public:
		cMapSceneViewRule(cStoreyData*e_pStoreyData,int e_iViewableRow,int e_iViewableColmun,cMainRoleData*e_pMainRoleData,POINT e_GridSize,POINT e_iOffsetMapPosIndex);
		~cMapSceneViewRule();
		void	SetStoreyData(cStoreyData*e_pStoreyData);
		virtual	void	UpdateForScrollableScreen(float e_fElpaseTime);
		virtual	void	RenderForScrollableScreen();
		virtual	void	UpdateForFixScreen(float e_fElpaseTime);
		virtual	void	RenderForFixScreen();
	};

#endif