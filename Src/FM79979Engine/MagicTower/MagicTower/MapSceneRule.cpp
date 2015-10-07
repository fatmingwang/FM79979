#include "stdafx.h"
#include "MapSceneRule.h"

cMapSceneViewRule::cMapSceneViewRule(cStoreyData*e_pStoreyData,int e_iViewableRow,int e_iViewableColumn,
							 cMainRoleData*e_pMainRoleData,POINT e_GridSize,POINT e_iOffsetMapPosIndex)
{
	m_iOffsetMapPosIndex = e_iOffsetMapPosIndex;
	m_pMainRoleData = e_pMainRoleData;
	m_GridSize = e_GridSize;
	m_iCurrentRow = 0;
	m_iCurrentColumn = 0;
	this->m_pStoreyData = e_pStoreyData;
	m_iViewableRow = e_iViewableRow;
	m_iViewableColumn = e_iViewableColumn;

	m_iTotalRowBlocks = this->m_pStoreyData->GetGridSize().x/m_iViewableRow;
	m_iTotalColumnBlocks = this->m_pStoreyData->GetGridSize().y/m_iViewableColumn;
	//assert(m_iViewableRow*m_iTotalRowBlocks == m_pStoreyData->GetGridSize().x&&"map size is not match");
	//assert(m_iViewableColumn*m_iTotalColumnBlocks == m_pStoreyData->GetGridSize().y&&"map size is not match");
	m_bDirty = true;
	m_iCurrentMapWall[0] = 0;
	m_iCurrentMapWall[1] = 0;
	m_iCurrentMapWall[2] = m_iViewableRow-1;
	m_iCurrentMapWall[3] = m_iViewableColumn-1;
}

cMapSceneViewRule::~cMapSceneViewRule()
{

}

void	cMapSceneViewRule::SetStoreyData(cStoreyData*e_pStoreyData)
{
	if( !e_pStoreyData )
	{
		UT::ErrorMsg(L"storey is empty!",L"Error!");
		return;
	}
	this->m_pStoreyData = e_pStoreyData;
	m_iTotalRowBlocks = this->m_pStoreyData->GetGridSize().x/m_iViewableRow;
	m_iTotalColumnBlocks = this->m_pStoreyData->GetGridSize().y/m_iViewableColumn;
	//assert(m_iViewableRow*m_iTotalRowBlocks == m_pStoreyData->GetGridSize().x&&"map size is not match");
	//assert(m_iViewableColumn*m_iTotalColumnBlocks == m_pStoreyData->GetGridSize().y&&"map size is not match");
}

void	cMapSceneViewRule::UpdateForFixScreen(float e_fElpaseTime)
{
	POINT	l_MapPosIndex = m_pMainRoleData->GetMapPosIndex();
	if( l_MapPosIndex.x<m_iCurrentMapWall[0]||
		l_MapPosIndex.y<m_iCurrentMapWall[1]||
		l_MapPosIndex.x>m_iCurrentMapWall[2]||
		l_MapPosIndex.y>m_iCurrentMapWall[3])
		m_bDirty = true;
	if(this->m_bDirty)
	{
		Vector3	l_vMainCharacterPos = this->m_pMainRoleData->GetWorldPosition();
		m_bDirty = false;
		this->m_iCurrentColumn = l_MapPosIndex.x/m_iViewableColumn;
		this->m_iCurrentRow = l_MapPosIndex.y/m_iViewableRow;
		assert(m_iCurrentRow<=m_iTotalRowBlocks);
		assert(m_iCurrentColumn<=m_iTotalColumnBlocks);
		int	l_iIndex = m_iCurrentRow*m_iTotalRowBlocks+m_iCurrentColumn;
		m_iCurrentMapWall[0] = m_iViewableColumn*m_iCurrentColumn;
		m_iCurrentMapWall[1] = m_iViewableRow*m_iCurrentRow;
		m_iCurrentMapWall[2] = m_iCurrentMapWall[0]+m_iViewableColumn-1;
		m_iCurrentMapWall[3] = m_iCurrentMapWall[1]+m_iViewableRow-1;
	}
	l_MapPosIndex.x%=m_iViewableColumn;
	l_MapPosIndex.y%=m_iViewableRow;
	Vector2	l_vRenderPos = Vector2((float)l_MapPosIndex.x*m_GridSize.x,(float)l_MapPosIndex.y*m_GridSize.y);
	m_pMainRoleData->SetRenderPos(l_vRenderPos);
}

void	cMapSceneViewRule::RenderForFixScreen()
{
	int	l_iOffsetIndex = (this->m_iViewableColumn*this->m_iCurrentColumn)
		+(this->m_pStoreyData->GetRowAndColumn().y*this->m_iViewableRow*this->m_iCurrentRow);
	Vector2	l_vPos = Vector2::Zero;
	Vector2	l_vCurrentCameraPos = Vector2((float)this->m_iCurrentColumn*m_iViewableColumn*this->m_GridSize.x,(float)this->m_iCurrentRow*m_iViewableRow*this->m_GridSize.y);
	cMagicTowerApp::m_sp2DCamera->Render(l_vCurrentCameraPos);
	RenderWithOffsetIndesForFixScreen(l_iOffsetIndex);
	cMagicTowerApp::m_sp2DCamera->Render(Vector2::Zero);
}

void	cMapSceneViewRule::UpdateForScrollableScreen(float e_fElpaseTime)
{
	this->m_pMainRoleData->SetRenderPos(Vector2((float)m_iOffsetMapPosIndex.x*this->m_GridSize.x,(float)m_iOffsetMapPosIndex.y*this->m_GridSize.y));
}
int		GetIndexInTwoArray(POINT e_CurrentIndex,int e_iTotalColumn)
{
	return (e_CurrentIndex.y*e_iTotalColumn)+e_CurrentIndex.x;
}

void	cMapSceneViewRule::RenderForScrollableScreen()
{
	//offset position
	POINT	l_RoleMapPosIndex = this->m_pMainRoleData->GetMapPosIndex();
	l_RoleMapPosIndex.x-=m_iOffsetMapPosIndex.x;
	l_RoleMapPosIndex.y-=m_iOffsetMapPosIndex.y;
	//camera
	Vector2	l_vCurrentCameraPos = Vector2((float)l_RoleMapPosIndex.x*this->m_GridSize.x,(float)l_RoleMapPosIndex.y*this->m_GridSize.y);
	cMagicTowerApp::m_sp2DCamera->Render(l_vCurrentCameraPos);
	RenderWithOffsetIndesForScrollableScreen(l_RoleMapPosIndex);
	cMagicTowerApp::m_sp2DCamera->Render(Vector2::Zero);
}

void	cMapSceneViewRule::RenderWithOffsetIndesForFixScreen(int e_iOffsetIndex)
{
	POINT	l_RowAndColumn = m_pStoreyData->GetRowAndColumn();
	for(int i=0;i<m_iViewableRow;++i)
	{
		for(int j=0;j<m_iViewableColumn;++j)
		{
			int	l_iIndex = e_iOffsetIndex + j + (i*l_RowAndColumn.x );
			if( l_iIndex<0 )
				continue;
			if( l_iIndex>=(l_RowAndColumn.x*l_RowAndColumn.y) )
				continue;
			m_pStoreyData->RenderAllLevelLayerGridObject(l_iIndex);
		}
	}
}

void	cMapSceneViewRule::RenderWithOffsetIndesForScrollableScreen(POINT e_OffsetIndex)
{
	POINT	l_RowAndColumn = m_pStoreyData->GetRowAndColumn();
	int	l_iViewColumn = m_iViewableColumn;
	int	l_iViewRow = m_iViewableRow;
	if( e_OffsetIndex.x < 0 )
	{
		l_iViewColumn += e_OffsetIndex.x;
		e_OffsetIndex.x = 0;
	}
	if( e_OffsetIndex.y < 0 )
	{
		l_iViewRow += e_OffsetIndex.y;
		e_OffsetIndex.y = 0;
	}
	for(int i=0;i<l_iViewRow;++i)
	{
		for(int j=0;j<l_iViewColumn;++j)
		{
			int	l_iIndex = (e_OffsetIndex.x + j) + ((i+e_OffsetIndex.y)*l_RowAndColumn.x );
			if( l_iIndex<0 )
				continue;
			if( l_iIndex>=(l_RowAndColumn.x*l_RowAndColumn.y) )
				continue;
			m_pStoreyData->RenderAllLevelLayerGridObject(l_iIndex);
		}
	}
}