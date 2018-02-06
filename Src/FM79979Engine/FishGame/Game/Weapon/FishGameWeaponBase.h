#ifndef _cPlayerWeapon_H_
#define _cPlayerWeapon_H_

#include "PlayerBehaviorBase.h"

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
	cMPDI*			m_pWeaponAnimation[eWS_MAX];
	float			m_fWeaponFireSpeed;//somehow control panel ask to fire fast...change by SetOneSecondCanFireNBullet
	eDirection		m_eMovingDirection;
	bool			m_bFire;
	void			InternalStatusChange(eDirection e_eDirection) = 0;
public:
	cPlayerWeapon();
	virtual ~cPlayerWeapon();
	void			StatusChange(eDirection e_eDirection);
	virtual	void	Init() = 0;
	virtual	void	Update(float e_fElpaseTime) = 0;
	virtual	void	Render();
	virtual	bool	Fire() = 0;
	bool			IsAllowFire() = 0;
};


class cWeaponManager:public cRenderObject
{
	cNamedTypeObjectVector<cFishGameWeaponBase*>	m_CommonWeaponVector;
	cNamedTypeObjectVector<cFishGameWeaponBase*>	m_MiniGameWeaponVector;
	cFishGameWeaponBase*							m_pCurrentWeapon;
	cFishGameWeaponBase*							m_pLastSelectedCommonWeapon;//from mini game to common weapon
public:
	void	SwitchWeapon();
	void	SwitchToNextCommonWeapon();
};

#endif