#pragma once

#include "PlayerMessageBehavior.h"
#include "PlayerKeyBehavior.h"

enum eFishPlayerKey
{
	eFPK_UP = 0,
	eFPK_DOWN,
	eFPK_LEFT,
	eFPK_RIGHT,
	eFPK_FIRE,
	eFPK_BET,
	eFPK_MAX
};

class	cPlayerBehaviorBase : public Frame,public cPlayerMessageBehavior
{
	friend class							cPlayerManager;
	int										m_iPlayerID;
protected:
	void									ProcessPlayer(TiXmlElement* e_pElement);
	void									ProcessPlayerBody(TiXmlElement* e_pElement);
	unsigned char							m_Key[eFPK_MAX];
	void									KeyUpdate(float e_fElpaseTime, bool e_bAllowToFire);
	void									SetupKey();
	void									LevelUpKeyUpdate(float e_fElpaseTime);
	void									RotatingKeyUpdate(float e_fElpaseTime);
	void									EnemyLockKeyUpdate(float e_fElpaseTime);
	void									FireKeyUpdate(float e_fElpaseTime, bool e_bAllowToFire);
	void									DisableAutoFire();
	bool									DeviceNotExistAssignPlayerIndexByKey0123456789KeyUpdate(float e_fElpaseTime);
	bool									DeviceNotExistAssignPlayerIndexByKey0123456789KeyUpdateAfter();
	sNSecondsNoShootThenFire*				m_pControlPanelNSecondsNoShootThenFire;
	sLongPressedToAutoDoSomethingChecker*	m_pLongPressedToAutoFireChecker;
	sLongPressedToAutoDoSomethingChecker*	m_pLongPressedToAutoLockFishChecker;
	sBetRateKeyDownTC						m_BetRateKeyDownTC;
	cMPDI*									m_pTimeOutCountDownMPDI;//for motro and control limit time up to fire
																	//if motor and control setting both exists,only need one.
	//
	bool									IsAllowToFire();
	virtual	void							InternalRender() = 0;
	virtual	void							InternalUpdate(float e_fElpaseTime) = 0;
	virtual	void							InternalInit() = 0;
	//to check how many bullet remain on the scene.
	GET_SET_DEC(int,m_iShootNumBuleet,GetShootNumBuleet,SetShootNumBuleet);
public:
	cPlayerBehaviorBase(TiXmlElement* e_pElement, cMatrix44 e_Matrix44, int e_iPlayerID);
	~cPlayerBehaviorBase();
	int					GetPlayerID(){return m_iPlayerID;}
	virtual	void		Init(void);
	virtual	void		Update( float e_fElpaseTime );
	virtual	void		Render(void);
	virtual	void		AddScore(cMonster* Fish, int64 Money, bool e_bIsDied = true, bool e_bDieByMiniGame = false) = 0;
	int64				GetScore();
	virtual bool		AddScoreByMoneyChangeType(int64 e_i64AddScore, eMoneyChangeType e_eMoneyChangeType);
	void				SetupTimeUpFireAndLongPressedAutoFire(float e_fTimeUpFireTime, float e_fLongPressedAutoFireTime);
};