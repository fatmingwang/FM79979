#pragma once

#include "../Common/CommonRegisterManager.h"

#include "KeyPressBehavior.h"

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

enum eWeaponStatus
{
	eWS_JUST_APPEAR = 0,
	eWS_IDLE,
	eWS_FIRE,
	eWS_WAIT_FOR_FINISH,//cannt fire wait to set new status or change weapon
	eWS_MAX
};

enum eWeaponType
{
	eWT_COMMON_WEAPON = 0,
	eWT_FAST_SHOOT_WEAPON,
	eWT_TRACE_MONSTER_WEAPON,
	eWT_MINI_GAME_WEAPON_NAME1,
	eWT_MINI_GAME_WEAPON_NAME2,
	eWT_MINI_GAME_WEAPON_NAME3,
	eWT_MINI_GAME_WEAPON_NAME4,
	eWT_MAX,
};

class	cFishGameWeaponBase :public cRenderObject
{
protected:
	//e_pucKey = cWeaponManager::m_ucKey
	//
	cMPDI*			m_pWeaponAnimation[eWS_MAX];
	float			m_fWeaponFireSpeed;//somehow control panel ask to fire fast...change by SetOneSecondCanFireNBullet
	eDirection		m_eMovingDirection;//only left or ight
	bool			m_bFire;
	virtual	bool	Fire() = 0;//require bullet
public:
	cFishGameWeaponBase();
	virtual ~cFishGameWeaponBase();
	virtual	void	Init() = 0;
	virtual	void	Update(float e_fElpaseTime,unsigned char*e_pucKey) = 0;
	virtual	void	Render() = 0;
	virtual bool	IsAllowFire() = 0;
	virtual bool	IsLeave() = 0;//after mini game fired.,common game never return true,cWeaponManager::SwitchWeapon
};