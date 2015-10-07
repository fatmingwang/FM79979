#include "stdafx.h"
#include "WalkingInMapPhase.h"
#include "SceneControl.h"
#include "BattlePhase.h"
#include "UIInfo.h"
#include "GameApp.h"
cWalkingInMapPhase::cWalkingInMapPhase(cMainRoleData*e_pMainRoleData,cObjectListByName<cStoreyData>*e_pAllStoreyData
									   ,int e_iViewableRow,int e_iViewableColumn,POINT e_GridSize,POINT e_iOffsetMapPosIndex,WCHAR*e_strCurrentStoreyName)
{
	m_pMonsterGridBehavior = 0;
	m_bWalkingModeIsFix = false;
	m_GridSize = e_GridSize;
	m_pMainRoleData = e_pMainRoleData;
	m_pMainRoleData->SetMapPosIndex(e_iOffsetMapPosIndex);
	m_pAllStoreyData = e_pAllStoreyData;
	assert(m_pAllStoreyData->Count());
	m_pCurrentStoreyData = (*m_pAllStoreyData)[e_strCurrentStoreyName];
	if( !m_pCurrentStoreyData )
	{
		UT::ErrorMsg(e_strCurrentStoreyName,L"no this storey!");
	}
	m_pMapSceneRule = 0;
	if( m_pCurrentStoreyData )
		m_pMapSceneRule = new cMapSceneViewRule(m_pCurrentStoreyData,e_iViewableRow,e_iViewableColumn,m_pMainRoleData,m_GridSize,e_iOffsetMapPosIndex);
	m_pVirtualGamePad = 0;
	this->SetName(WALKING_IN_MAP_PHASE);
	//m_pVirtualGamePad = new cVirtualGamePad();
}

cWalkingInMapPhase::~cWalkingInMapPhase()
{
	SAFE_DELETE(m_pMapSceneRule);
}

void	cWalkingInMapPhase::Init()
{
	m_pMonsterGridBehavior = 0;
}

void	cWalkingInMapPhase::Update(float e_fElpaseTime)
{
	if( m_pMapSceneRule )
	{
		if( !m_bWalkingModeIsFix )
			m_pMapSceneRule->UpdateForScrollableScreen(e_fElpaseTime);
		else
			m_pMapSceneRule->UpdateForFixScreen(e_fElpaseTime);
		m_pMainRoleData->Update(e_fElpaseTime);
	}
}
//it would be called if the stage is changed.
void	cWalkingInMapPhase::Render()
{
	if( m_pMapSceneRule )
	{
		cUIInfo*l_pUIInfo = g_pMagicTowerApp->GetUIINfo();
		cMPDI*l_pMPDI = l_pUIInfo->GetUIInfoMPDI();

		float	l_fStartPosX = l_pMPDI->GetPos().x;
		Vector2	l_vDrawSize = l_pMPDI->GetDrawSize();
		l_fStartPosX += l_vDrawSize.x;

		Vector4	l_vViewRect(l_fStartPosX,0,cGameApp::m_svGameResolution.x-l_fStartPosX,cGameApp::m_svGameResolution.y);
		l_vViewRect = ViewRectToOpenGLScissor(l_vViewRect);
		glViewport((int)l_vViewRect.x,(int)l_vViewRect.y,(int)l_vViewRect.z,(int)l_vViewRect.w);
		glScissor((int)l_vViewRect.x,(int)l_vViewRect.y,(int)l_vViewRect.z,(int)l_vViewRect.w);
		//m_pMapSceneRule->Render();
		if( !m_bWalkingModeIsFix )
			m_pMapSceneRule->RenderForScrollableScreen();
		else
			m_pMapSceneRule->RenderForFixScreen();
		m_pMainRoleData->Render();
	}
}

void	cWalkingInMapPhase::DebugRender()
{

}

void	cWalkingInMapPhase::MouseDown(int e_iX,int e_iY)
{

}

void	cWalkingInMapPhase::MouseUp(int e_iX,int e_iY)
{

}

void	cWalkingInMapPhase::MouseHover(int e_iX,int e_iY)
{

}

const   WCHAR*	cWalkingInMapPhase::GetNextPhaseName()
{
	return 0;
}

void*	cWalkingInMapPhase::GetData()
{
	return m_pMonsterGridBehavior;
}

void	cWalkingInMapPhase::KeyDown(char e_char)
{
	POINT	l_MapPosIndex = m_pMainRoleData->GetMapPosIndex();
	Vector2	l_vPos = m_pMainRoleData->GetRenderPos();
	Vector2	l_vStepPos = Vector2::Zero;
	//m_pCurrentStoreyData->
	switch(e_char)
	{
		case 37://left
			m_pMainRoleData->SetDirection(eVGPD_LEFT);
			l_MapPosIndex.x-=1;
			l_vStepPos.x = (float)-this->m_GridSize.x;
			break;
		case 38://up
			m_pMainRoleData->SetDirection(eVGPD_UP);
			l_MapPosIndex.y-=1;
			l_vStepPos.y = (float)-this->m_GridSize.y;
			break;
		case 39://right
			m_pMainRoleData->SetDirection(eVGPD_RIGHT);
			l_MapPosIndex.x+=1;
			l_vStepPos.x = (float)this->m_GridSize.x;
			break;
		case 40://down
			m_pMainRoleData->SetDirection(eVGPD_DOWN);
			l_MapPosIndex.y+=1;
			l_vStepPos.y = (float)this->m_GridSize.y;
			break;
	}
	m_pMainRoleData->SetRenderPos(l_vPos);
	POINT	l_RowAndColumn = m_pCurrentStoreyData->GetRowAndColumn();
	if( l_MapPosIndex.x<0||l_MapPosIndex.x>= l_RowAndColumn.x ||
		l_MapPosIndex.y<0||l_MapPosIndex.y>= l_RowAndColumn.y )
	{
		return;
	}
	bool	l_bNewPosition = false;
	cGridBehavior*l_pNewPositionGridBehavior = 0;
	int	l_iLayerCount = m_pCurrentStoreyData->Count();
	for( int i=0;i<l_iLayerCount;++i )
	{
		eMagicTowerGridType l_eMagicTowerGridType = MTGT_ROAD;
		//int	l_iIndex = l_MapPosIndex.x+l_MapPosIndex.y*m_pCurrentStoreyData->m_iColumn;
		cGridBehavior*l_pGridBehavior = m_pCurrentStoreyData->GetGridBehavior(l_MapPosIndex,i);
		if( l_pGridBehavior )
		{
			l_eMagicTowerGridType = l_pGridBehavior->GetMagicTowerGridType();
			if( !l_pGridBehavior->IsWorking() )
				continue;
		}
		if( l_eMagicTowerGridType == MTGT_BLOCK || l_eMagicTowerGridType == MTGT_NPC )
		{
			l_bNewPosition = false;
		}
		else
		{
			l_bNewPosition = true;
			l_pNewPositionGridBehavior = l_pGridBehavior;
			if( l_eMagicTowerGridType == MTGT_UP || l_eMagicTowerGridType == MTGT_DOWN  )
			{
				int	l_iIndex = GetInt(m_pCurrentStoreyData->GetName());
				if( l_eMagicTowerGridType == MTGT_UP )
				{
					cGameApp::SoundPlay(L"MagicTower/Sound/StoreyUp.wav",true);
					l_iIndex++;
				}
				else
				{
					cGameApp::SoundPlay(L"MagicTower/Sound/StoreyDown.wav",true);
					l_iIndex--;
				}
				if( m_pAllStoreyData->GetObject(ValueToStringW(l_iIndex)) )
				{
					this->m_pCurrentStoreyData = m_pAllStoreyData->GetObject(ValueToStringW(l_iIndex));
					this->m_pMapSceneRule->SetStoreyData(this->m_pCurrentStoreyData);
				}
				else
				{
					UT::ErrorMsg(L"storey is empty!",ValueToStringW(l_iIndex));
				}
			}
			if( MTGT_MONSTER ==  l_eMagicTowerGridType )
			{
				m_pMonsterGridBehavior = l_pGridBehavior;
				cBattlePhase*l_pBattlePhase = new cBattlePhase(this->m_pMainRoleData,l_pNewPositionGridBehavior);
				l_pBattlePhase->SetKillAfterSatisfiedCondition(true);
				cMagicTowerApp::m_spSceneControl->AddObject(l_pBattlePhase);
				cMagicTowerApp::m_spSceneControl->SetCurrentCurrentPhase(BATTLE_PHASE);
			}
		}
		assert(this->m_pCurrentStoreyData);
	}
	if( l_bNewPosition )
	{
		if( m_bWalkingModeIsFix )
			m_pMainRoleData->SetRenderPos(l_vPos+l_vStepPos);
		m_pMainRoleData->SetMapPosIndex(l_MapPosIndex);	
	}
}