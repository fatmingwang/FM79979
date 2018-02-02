#pragma once

#include "PlayerBehaviorBase.h"

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

struct sLevelKeyDownTC
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
	sLevelKeyDownTC();
	~sLevelKeyDownTC();
	void							Init(unsigned char e_ucKeyValue,int e_iMaxLevel,int e_iMinBet, int e_iLevelGap);
	void							Update(float e_fElpaseTime,int e_iCurrentLevel);
	bool							IsLongPress() {return bLongPressed;}
};

class	cPlayerData : public cPlayerBehaviorBase
{
	unsigned char							m_cInput_Right;
	unsigned char							m_cInput_Left;
	unsigned char							m_cInput_Fire;
	unsigned char							m_cInput_Down;
	unsigned char							m_cInput_Level;
	unsigned char							m_cInput_Up;//???? useless why need this one??
	//
	void									LevelUpKeyUpdate(float e_fElpaseTime);
	void									RotatingKeyUpdate(float e_fElpaseTime);
	void									EnemyLockKeyUpdate(float e_fElpaseTime);
	void									FireKeyUpdate(float e_fElpaseTime, bool e_bAllowToFire);
	void									DisableAutoFire();
	bool									DeviceNotExistAssignPlayerIndexByKey0123456789KeyUpdate(float e_fElpaseTime);
	bool									DeviceNotExistAssignPlayerIndexByKey0123456789KeyUpdateAfter();
	sNSecondsNoShootThenFire*				m_pMotorNSecondsNoShootThenFire;
	sNSecondsNoShootThenFire*				m_pControlPanelNSecondsNoShootThenFire;
	sLongPressedToAutoDoSomethingChecker*	m_pLongPressedToAutoFireChecker;
	sLongPressedToAutoDoSomethingChecker*	m_pLongPressedToAutoLockFishChecker;
	sLevelKeyDownTC							m_LevelKeyDownTC;
	cMPDI*									m_pTimeOutCountDownMPDI;//for motro and control limit time up to fire
	//if motor and control setting both exists,only need one.
	sNSecondsNoShootThenFire*				GetCurrentWorkingNSecondsNoShootThenFire();
	bool									IsAllowToFire();
private:
	void									ProcessPlayer( TiXmlElement* e_pElement );
	void									ProcessPlayerBody( TiXmlElement* e_pElement );	//
	void									KeyUpdate(float e_fElpaseTime,bool e_bAllowToFire);
public:
	//
	cPlayerData( TiXmlElement* e_pElement,cMatrix44 e_Matrix44,int e_iPlayerID);
	~cPlayerData(void);
	void							SetupKey();

	virtual	void					InternalRender();
	virtual	void					InternalInit();
	virtual	void					InternalUpdate(float e_fElpaseTime);
	virtual	void					Destroy(void);
	//
	virtual	void					AddScore( cMonster* Fish, int64 Money, bool e_bIsDied = true, bool e_bDieByMiniGame = false );
	void							SetupTimeUpFireAndLongPressedAutoFire(float e_fTimeUpFireTime,float e_fLongPressedAutoFireTime);
};