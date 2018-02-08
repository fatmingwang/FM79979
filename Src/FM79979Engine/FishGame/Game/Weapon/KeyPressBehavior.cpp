#include "stdafx.h"
#include "KeyPressBehavior.h"
#include "../GameDefine/GameParameterDefine.h"
sChangeBetRateKeyDownTC::sChangeBetRateKeyDownTC()
{
	LevelKeyClickMouseBehavior.SetPressedRepeatMouseUp(true);
	LevelKeyClickMouseBehavior.SetRepeatTime(0.3f);
	fLevelKeyDownOneSecondTC = 0.f;
	fLevelKeyDownDynamicSpeedTC = 1.f;
	fLevelKeyDownReduceTimeTC = 0.1f;
	fLevelKeyDownFirstClickTC = 0.5f;
	bLongPressed = false;
}
sChangeBetRateKeyDownTC::~sChangeBetRateKeyDownTC()
{

}

void	sChangeBetRateKeyDownTC::Init(unsigned char e_ucKeyValue, int e_iMaxBetRate, int e_iMinBet, int e_iBetGap)
{
	LevelKeyClickMouseBehavior.SetKeyData(e_ucKeyValue);
	bLongPressed = false;
	iLevelGap = e_iBetGap;
	iMaxLevel = e_iMaxBetRate;
	fLevelKeyDownOneSecondTC = 0.f;
	int l_iOffset = e_iMaxBetRate - e_iMinBet;
	int l_iCount = l_iOffset / e_iBetGap;
	if (l_iCount <= 10)
	{
		fLevelKeyDownReduceTimeTC = 0.1f;
	}
	else
	{
		fLevelKeyDownReduceTimeTC = 0.03f;
	}
	LevelKeyClickMouseBehavior.Init();
}

void	sChangeBetRateKeyDownTC::Update(float e_fElpaseTime, int e_iBetRate)
{
	bool l_bLevelup = false;
	LevelKeyClickMouseBehavior.SingnalProcess();
	LevelKeyClickMouseBehavior.Update(e_fElpaseTime);
	if (LevelKeyClickMouseBehavior.IsSatisfiedCondition())
	{
		bool l_bLongPressed = false;
		if (fLevelKeyDownOneSecondTC >= 1.0f)
		{
			l_bLongPressed = true;
			//hold 1 second at max level
			if (iMaxLevel == e_iBetRate + iLevelGap)
			{
				fLevelKeyDownDynamicSpeedTC = LEVEL_CHANGE_REACH_MAX_STAY_TIME;
			}
			else
			{
				//from max Level leave
				if (fLevelKeyDownDynamicSpeedTC > fLevelKeyDownFirstClickTC)
					fLevelKeyDownDynamicSpeedTC = fLevelKeyDownFirstClickTC;
				fLevelKeyDownDynamicSpeedTC -= fLevelKeyDownReduceTimeTC;
			}
		}
		else
		{
			fLevelKeyDownDynamicSpeedTC = fLevelKeyDownFirstClickTC;
		}
		//avoid too fast
		if (fLevelKeyDownDynamicSpeedTC <= 0.1f)
		{
			fLevelKeyDownDynamicSpeedTC = 0.1f;
		}
		LevelKeyClickMouseBehavior.SetRepeatTime(fLevelKeyDownDynamicSpeedTC);
		LevelKeyClickMouseBehavior.Init();
		//aviod mouse up
		if (l_bLongPressed && !LevelKeyClickMouseBehavior.IsKeyDown())
		{
			l_bLevelup = false;
			LevelKeyClickMouseBehavior.SetRepeatTime(fLevelKeyDownFirstClickTC);
		}
		else
			l_bLevelup = true;
	}
	if (cGameApp::m_sucKeyData[LevelKeyClickMouseBehavior.GetKeyData()])
	{
		fLevelKeyDownOneSecondTC += e_fElpaseTime;
	}
	else
	{
		fLevelKeyDownOneSecondTC = 0.f;
	}
	bLongPressed = l_bLevelup;

}

sNSecondsNoShootThenFire::sNSecondsNoShootThenFire(cMPDI*e_pCountdownMPDI, float e_fTime)
{
	fMPDIShowTime = 5.f;
	bUpdated = false;
	bEnable = true;
	pCountdownMPDI = e_pCountdownMPDI;
	if (pCountdownMPDI)
	{
		fMPDIShowTime = pCountdownMPDI->GetEndTime();
	}
	TC.SetTargetTime(e_fTime);
}
sNSecondsNoShootThenFire::~sNSecondsNoShootThenFire() {}
void	sNSecondsNoShootThenFire::Update(float e_fElapseTime)
{
	if (bEnable)
	{
		bUpdated = true;
		TC.Update(e_fElapseTime);
		if (pCountdownMPDI && TC.fRestTime <= fMPDIShowTime)
		{
			pCountdownMPDI->Update(e_fElapseTime);
		}
	}
}
void	sNSecondsNoShootThenFire::Render()
{
	if (bEnable && pCountdownMPDI && bUpdated)
	{
		if (pCountdownMPDI && TC.fRestTime <= fMPDIShowTime)
			pCountdownMPDI->Render();
	}
}
void	sNSecondsNoShootThenFire::Start() { TC.Start(); bUpdated = false; }
bool	sNSecondsNoShootThenFire::IsAbleToDo() { return bEnable ? TC.bTragetTimrReached : false; }
void	sNSecondsNoShootThenFire::SetEnable(bool e_bEnable) { bEnable = e_bEnable; }


sLongPressedToAutoDoSomethingChecker::sLongPressedToAutoDoSomethingChecker(float e_fTargetTimeToAchieve, unsigned char e_ucKeyValue)
{
	bAbleToDo = false;
	fTargetTime = e_fTargetTimeToAchieve;
	ClickMouseBehavior.SetKeyData(e_ucKeyValue);
}
sLongPressedToAutoDoSomethingChecker::~sLongPressedToAutoDoSomethingChecker() {}
void				sLongPressedToAutoDoSomethingChecker::Start()
{
	ClickMouseBehavior.Init();
	bAbleToDo = false;
}
void				sLongPressedToAutoDoSomethingChecker::Update(float e_fElpaseTime)
{
	bool l_bLastKeyDown = ClickMouseBehavior.IsKeyDown();
	ClickMouseBehavior.Update(e_fElpaseTime);
	ClickMouseBehavior.SingnalProcess();
	if (bAbleToDo)
	{ //new key down
		if (!l_bLastKeyDown && ClickMouseBehavior.IsKeyDown())
		{
			cGameApp::m_sucKeyData[ClickMouseBehavior.GetKeyData()] = false;
			ClickMouseBehavior.Init();
			bAbleToDo = false;
			cGameApp::m_sucKeyData[ClickMouseBehavior.GetKeyData()] = true;
		}
	}
	else
	{
		if (!ClickMouseBehavior.IsKeyDown())
		{
			eObjectMouseBehavior l_ObjectMouseBehavior = ClickMouseBehavior.GetMouseBehavior();
			if (l_ObjectMouseBehavior != eObjectMouseBehavior::eOMB_NONE)
				ClickMouseBehavior.Init();
		}
		else
			if (ClickMouseBehavior.GetMouseMoveData().GetDownElpaseTime() >= fTargetTime)
			{
				bAbleToDo = true;
				ClickMouseBehavior.Init();
			}
	}
}
bool sLongPressedToAutoDoSomethingChecker::IsAbleToDo() { return bAbleToDo; }