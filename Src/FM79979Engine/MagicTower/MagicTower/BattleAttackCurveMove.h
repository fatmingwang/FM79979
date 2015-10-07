#ifndef _MAGIC_TOWER_BATTLE_ATTACK_CURVE_MOVE_H_
#define _MAGIC_TOWER_BATTLE_ATTACK_CURVE_MOVE_H_

#include "BattlAttackBase.h"

class	cBattleAttackCurveMove:public cBattleBase
{
public:
	cBattleAttackCurveMove(eBattleType	m_eBattleType);
	~cBattleAttackCurveMove();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};

#endif