#ifndef _MAGIC_TOWER_BATTLE_TOUCH_SEQUENCE_H_
#define _MAGIC_TOWER_BATTLE_TOUCH_SEQUENCE_H_

#include "BattlAttackBase.h"

class	cBattleAttackTouchSequence:public cBattleBase
{
public:
	cBattleAttackTouchSequence(eBattleType	e_eBattleType);
	~cBattleAttackTouchSequence();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};

#endif