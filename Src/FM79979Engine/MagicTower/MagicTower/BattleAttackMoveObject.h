#ifndef _MAGIC_TOWER_BATTLE_ATTACK_MOVE_OBJECT_H_
#define _MAGIC_TOWER_BATTLE_ATTACK_MOVE_OBJECT_H_
#include "BattlAttackBase.h"
#include "MoveObject.h"
//===================
//
//2 moving objects keep moving at a direction,while they are overlap,click button
//and get a attack power
//
//===================
class	cBattleAttackMoveObject:public cBattleBase
{
	sTwoMovingObjectController*m_pTwoMovingObjectController;
    virtual void			InternalMouseMove(int e_iPosX,int e_iPosY);
    virtual void			InternalMouseDown(int e_iPosX,int e_iPosY);
    virtual void    		InternalMouseUp(int e_iPosX,int e_iPosY);
    virtual void    		InternalMouseLeave(int e_iPosX,int e_iPosY);
public:
	cBattleAttackMoveObject(eBattleType	m_eBattleType);
	~cBattleAttackMoveObject();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	DebugRender();
};

#endif