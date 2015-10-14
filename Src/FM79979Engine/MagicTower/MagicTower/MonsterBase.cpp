#include "stdafx.h"
#include "MonsterBase.h"
#include "LevelData.h"
extern WCHAR*g_strMonsterStatusName[] = {L"Idle",L"Attack"L"Hurt"L"Died"};

cMonsterBase::cMonsterBase(cGridBehavior*e_pGridBehavior):cCharacterBase(e_pGridBehavior->GetLevelLayerGridData()->GetTemplateDataNode()->GetDataNode())
{
	m_pGridBehavior = e_pGridBehavior;
	SetStatus(eMTMS_IDLE);
}

cMonsterBase::~cMonsterBase()
{

}

void	cMonsterBase::UpdateAttackBehavior(float e_ElpaseTime)
{

}

cMonsterBase*	cMonsterBase::GetMonster(cGridBehavior*e_pGridBehavior)
{
	assert( e_pGridBehavior->GetMagicTowerGridType() == MTGT_MONSTER );
	cMonsterBase*l_pMonsterBase = new cMonsterBase(e_pGridBehavior);
	return l_pMonsterBase;
}

void	cMonsterBase::Update(float e_fElpaseTime)
{
	cFAIAnimationCharacter::Update(e_fElpaseTime);
	int	l_iCurrentStatusIndex=  this->GetCurrentWorkingObjectIndex();
	if( eMTMS_IDLE == l_iCurrentStatusIndex )
	{
		UpdateAttackBehavior(e_fElpaseTime);
	}
	UpdateAttackBehavior(e_fElpaseTime);
	if( l_iCurrentStatusIndex != eMTMS_IDLE )
	{
		if( this->GetCurrentWorkingObject()->IsAnimationDone() )
		{
			this->SetCurrentWorkingObject(eMTMS_IDLE);
		}
	}
}

void	cMonsterBase::Render()
{
	cFAIAnimationCharacter::Render();
	RenderStatusInfo();
}

void	cMonsterBase::SetStatus(eMagicTowerMonsterStatus e_eMagicTowerMonsterStatus)
{
	m_eCurrentStatus = e_eMagicTowerMonsterStatus;
	this->SetCurrentWorkingObject(g_strMonsterStatusName[m_eCurrentStatus]);
}