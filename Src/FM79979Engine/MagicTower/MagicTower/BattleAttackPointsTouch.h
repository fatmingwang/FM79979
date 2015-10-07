#ifndef _MAGIC_TOWER_BATTLE_ATTACK_POINTS_TOUCH_H_
#define _MAGIC_TOWER_BATTLE_ATTACK_POINTS_TOUCH_H_

#include "BattlAttackBase.h"

class	cBattleAttackPointsTouch:public cBattleBase
{
public:
	cBattleAttackPointsTouch(eBattleType e_eBattleType);
	~cBattleAttackPointsTouch();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};

#endif