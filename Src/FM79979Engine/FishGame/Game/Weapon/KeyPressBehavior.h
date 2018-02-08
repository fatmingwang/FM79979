#pragma once

struct sNSecondsNoShootThenFire
{
	bool				bUpdated;
	bool				bEnable;
	float				fMPDIShowTime;
	cMPDI*				pCountdownMPDI;
	sNSecondsNoShootThenFire(cMPDI*e_pCountdownMPDI, float e_fTime);
	~sNSecondsNoShootThenFire();
	UT::sTimeCounter	TC;
	void				Update(float e_fElapseTime);
	void				Render();
	void				Start();
	bool				IsAbleToDo();
	void				SetEnable(bool e_bEnable);
};

struct sLongPressedToAutoDoSomethingChecker
{
	cClickMouseBehavior ClickMouseBehavior;
	bool				bAbleToDo;
	float				fTargetTime;
	sLongPressedToAutoDoSomethingChecker(float e_fTargetTimeToAchieve, unsigned char e_ucKeyValue);
	~sLongPressedToAutoDoSomethingChecker();
	void				Start();
	void				Update(float e_fElpaseTime);
	bool				IsAbleToDo();
};

struct sChangeBetRateKeyDownTC
{
	int								iLevelGap;
	int								iMaxLevel;
	bool							bLongPressed;
	cClickMouseBehavior				LevelKeyClickMouseBehavior;
	float							fLevelKeyDownOneSecondTC;
	float							fLevelKeyDownDynamicSpeedTC;
	float							fLevelKeyDownReduceTimeTC;
	//original speed 0.3 or 0.5 second(should from setup file.) to avoid player accidently touch
	float							fLevelKeyDownFirstClickTC;
	sChangeBetRateKeyDownTC();
	~sChangeBetRateKeyDownTC();
	void							Init(unsigned char e_ucKeyValue,int e_iMaxBetRate,int e_iMinBet, int e_iBetGap);
	void							Update(float e_fElpaseTime,int e_iBetRate);
	bool							IsLongPress() {return bLongPressed;}
};