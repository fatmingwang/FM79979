#pragma once

#include "FishGameWeaponBase.h"
#include "KeyPressBehavior.h"

class	cFishGameCommonWeapon :public cFishGameWeaponBase
{
protected:
	void									RotatingKeyUpdate(float e_fElpaseTime);
	void									LevelUpKeyUpdate(float e_fElpaseTime);
	void									EnemyLockKeyUpdate(float e_fElpaseTime);
	void									DisableAutoFire();
	sNSecondsNoShootThenFire*				m_pControlPanelNSecondsNoShootThenFire;
	sLongPressedToAutoDoSomethingChecker*	m_pLongPressedToAutoFireChecker;
	sLongPressedToAutoDoSomethingChecker*	m_pLongPressedToAutoLockFishChecker;
	sChangeBetRateKeyDownTC					m_ChangeBetRateKeyDownTC;
	cMPDI*									m_pTimeOutAutoShotOneBulletCountDownMPDI;//for motro and control limit time up to fire
	virtual	bool							Fire()override;
public:
	cFishGameCommonWeapon();
	virtual ~cFishGameCommonWeapon();
	static cFishGameCommonWeapon*GetMe(TiXmlElement*e_pTiXmlElement);
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime,unsigned char*e_pucKey)override;
	virtual	void	Render()override;
	virtual bool	IsAllowFire()override;
	bool			IsLeave() { return false; }
};