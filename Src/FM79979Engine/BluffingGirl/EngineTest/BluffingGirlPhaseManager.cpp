#include "stdafx.h"
#include "BluffingGirlPhaseManager.h"
#include "StorePhase.h"
#include "SetupPhase.h"
#include "AllPhaseDefine.h"
#include "BluffingGirlApp.h"
#include "PlayerData.h"

cBluffingGirlPhaseManager::cBluffingGirlPhaseManager()
{
}

cBluffingGirlPhaseManager::~cBluffingGirlPhaseManager()
{

}

void	cBluffingGirlPhaseManager::Update(float	e_fElpaseTime)
{
	if(PopUpMessagerUpdate(e_fElpaseTime))
	{
		if( this->m_iCurrentPhase != -1 )
		{
			cSimplePhase *l_pSimplePhase = m_ObjectList[m_iCurrentPhase];
			l_pSimplePhase->Update(e_fElpaseTime);
			if( l_pSimplePhase->IsSatisfiedCondition() )
			{
				if(!wcscmp(l_pSimplePhase->GetName(),SETUP_PHASE_NAME) || !wcscmp(l_pSimplePhase->GetName(),STORE_PHASE_NAME) ||
					!wcscmp(l_pSimplePhase->GetName(),RULE_PHASE_NAME))
				{
					const WCHAR*	l_strLastPhaseName = l_pSimplePhase->GetNextPhaseName();
					m_iCurrentPhase = GetObjectIndexByName(l_strLastPhaseName);
					l_pSimplePhase = m_ObjectList[m_iCurrentPhase];
					l_pSimplePhase->SetSatisfiedCondition(false);
					return;				
				}
				const WCHAR*l_strName = l_pSimplePhase->GetNextPhaseName();
				if( !wcscmp(l_strName,WIN_PRIZE_PHASE) )
				{
					cBluffingGirlApp::m_spPlayerData->SetItemClickEnable(false);
				}
				else
					cBluffingGirlApp::m_spPlayerData->SetItemClickEnable(true);
					
				int	l_iIndex = GetObjectIndexByName(l_strName);
				if( l_iIndex == -1  )
				{
	#ifdef WIN32
					cGameApp::OutputDebugInfoString(L"error Phase or end phase?? ");
	#endif
				}
				m_iCurrentPhase = l_iIndex;
				cSimplePhase*l_pTargetPhase = 0;
				if( l_iIndex != -1 )
				{
					l_pTargetPhase = m_ObjectList[l_iIndex];
					l_pTargetPhase->SetSatisfiedCondition(false);//if u do not allow m_bSatisfiedCondition to be false again,set it as true in the Init function if u wanted.
					l_pTargetPhase->FetchData(l_pSimplePhase->GetName(),l_pSimplePhase->GetData());
					l_pTargetPhase->Init();
					l_pTargetPhase->Update(0.001f);
				}
				if(l_pSimplePhase->IsKillAfterSatisfiedCondition())
					this->RemoveObject(l_pSimplePhase);
				this->m_pCurrentWorkingObject = l_pTargetPhase;
				if( l_pTargetPhase )
				{
					m_iCurrentPhase = this->GetObjectIndexByPointer(l_pTargetPhase);
					//try to avoid empty phase render...!?
					//l_pTargetPhase->Update(0.00f);
				}
				cGameApp::m_sTimeAndFPS.Update();
			}
		}
	}
}