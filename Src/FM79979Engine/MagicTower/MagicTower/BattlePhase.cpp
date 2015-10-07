#include "stdafx.h"
#include "BattlePhase.h"
#include "MagicTowerDefineDataNode.h"
#include "MainCharacterBehavior.h"
#include "BattlAttackBase.h"
#include "MonsterBase.h"
cBattlePhase::cBattlePhase(cMainRoleData*e_pMainRoleData,cGridBehavior*e_pGridBehavior)
{
	this->SetName(BATTLE_PHASE);
	m_pBattleAttack = 0;
	m_pBattleDefence = 0;
	m_pGridBehavior = e_pGridBehavior;
	m_pMonsterBase	= cMonsterBase::GetMonster(e_pGridBehavior);
	m_pMainRoleData	= e_pMainRoleData;
	m_pMainRoleData->SetBattleMode(true);
	m_pMonsterBase->SetLocalPosition(Vector3(740,200,0));
}

cBattlePhase::~cBattlePhase()
{
	m_pMainRoleData->SetBattleMode(false);
	SAFE_DELETE(m_pMonsterBase);
	SAFE_DELETE(m_pBattleAttack);
	SAFE_DELETE(m_pBattleDefence);
}

void	cBattlePhase::Init()
{
	m_bSatisfiedCondition = false;
	SAFE_DELETE(m_pBattleAttack);
	SAFE_DELETE(m_pBattleDefence);
	m_pBattleAttack = cBattleBase::GetBattleBase(m_pMainRoleData->GetBattleAttackType());
	m_pBattleDefence = cBattleBase::GetBattleBase(m_pMainRoleData->GetBattleDefenceType());

	if( m_pBattleAttack )
		m_pBattleAttack->Init();
	if( m_pBattleDefence )
		m_pBattleDefence->Init();
}

void	cBattlePhase::ProcessAttack()
{
	m_pMonsterBase->GetStatus()->m_iHP -= 1;
}

void	cBattlePhase::Update(float e_fElpaseTime)
{
	if( m_pBattleAttack )
	{
		m_pBattleAttack->Update(e_fElpaseTime);
		float	l_fPower = m_pBattleAttack->GetPower();
		if( l_fPower != -1.f )
		{
			if( l_fPower <= 0.01f )
			{
				m_pMainRoleData->SetCurrentWorkingObject(eMTMRS_ATTACK_FAILED);
				cGameApp::SoundPlay(L"MagicTower/Sound/AttackSound1.wav",true);
			}
			else
			{
				m_pMainRoleData->SetCurrentWorkingObject(eMTMRS_ATTACK);
				cGameApp::SoundPlay(L"MagicTower/Sound/AttackSound2.wav",true);
			}
			m_pBattleAttack->Init();
			m_pMonsterBase->SetCurrentWorkingObject(eMTMS_HURT);
		}
		else
		{
			if(m_pMainRoleData->GetCurrentWorkingObjectIndex() != eMTMRS_IDLE)
			{
				if( m_pMainRoleData->GetCurrentWorkingObject()->IsDone() )
				{
					switch(m_pMainRoleData->GetCurrentWorkingObjectIndex())
					{
						case eMTMRS_ATTACK:
							ProcessAttack();
							break;
					}
					m_pMainRoleData->SetCurrentWorkingObject(eMTMRS_IDLE);
				}
			}
		}
	}
	if( m_pBattleDefence )
		m_pBattleDefence->Update(e_fElpaseTime);
	m_pMainRoleData->Update(e_fElpaseTime);
	if( m_pMonsterBase )
		m_pMonsterBase->Update(e_fElpaseTime);
	if( m_pMonsterBase->GetStatus()->m_iHP <= 0 )
	{
		m_pGridBehavior->SetWorking(false);
		this->m_bSatisfiedCondition = true;
	}
}

void	cBattlePhase::Render()
{
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	if( m_pBattleAttack )
		m_pBattleAttack->Render();
	if( m_pBattleDefence )
		m_pBattleDefence->Render();
	m_pMainRoleData->Render();
	if( m_pMonsterBase )
		m_pMonsterBase->Render();
}

void	cBattlePhase::DebugRender()
{
	if( m_pBattleAttack )
		m_pBattleAttack->DebugRender();
}

void	cBattlePhase::MouseDown(int e_iX,int e_iY)
{
	if( m_pMainRoleData->GetCurrentWorkingObjectIndex() != eMTMRS_IDLE )
		return;
	if( m_pMonsterBase->GetStatus()->m_iHP < 1 )
		return;
	if( m_pBattleAttack )
		m_pBattleAttack->MouseDown(e_iX,e_iY);
	if( m_pBattleDefence )
		m_pBattleDefence->MouseDown(e_iX,e_iY);
}

void	cBattlePhase::MouseMove(int e_iX,int e_iY)
{
	if( m_pMainRoleData->GetCurrentWorkingObjectIndex() != eMTMRS_IDLE )
		return;
	if( m_pMonsterBase->GetStatus()->m_iHP < 1 )
		return;
	if( m_pBattleAttack )
		m_pBattleAttack->MouseMove(e_iX,e_iY);
	if( m_pBattleDefence )
		m_pBattleDefence->MouseMove(e_iX,e_iY);
}

void	cBattlePhase::MouseUp(int e_iX,int e_iY)
{
	if( m_pMainRoleData->GetCurrentWorkingObjectIndex() != eMTMRS_IDLE )
		return;
	if( m_pMonsterBase->GetStatus()->m_iHP < 1 )
		return;
	if( m_pBattleAttack )
	{
		m_pBattleAttack->MouseUp(e_iX,e_iY);
	}
	if( m_pBattleDefence )
		m_pBattleDefence->MouseUp(e_iX,e_iY);
}

void	cBattlePhase::KeyUP(char e_cKey)
{

}

const WCHAR*	cBattlePhase::GetNextPhaseName()
{
	return WALKING_IN_MAP_PHASE;
}