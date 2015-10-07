#ifndef _MAGIC_TOWER_MONSTER_BASE_H
#define _MAGIC_TOWER_MONSTER_BASE_H

#include "CharacterBase.h"

enum	eMagicTowerMonsterStatus
{
	eMTMS_IDLE = 0,
	eMTMS_ATTACK,
	eMTMS_HURT,
	eMTMS_DIED,
	eMTMS_MAX
};

extern WCHAR*g_strMonsterStatusName[];

	class	cGridBehavior;
	class	cMonsterBase:public cCharacterBase
	{
		cGridBehavior*				m_pGridBehavior;
		eMagicTowerMonsterStatus	m_eCurrentStatus;
		virtual	void	UpdateAttackBehavior(float e_ElpaseTime);
	public:
		cMonsterBase(cGridBehavior*e_pGridBehavior);
		~cMonsterBase();
		static cMonsterBase*	GetMonster(cGridBehavior*e_pGridBehavior);
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
		virtual	void	RenderAttack(){}
		virtual	void	SetStatus(eMagicTowerMonsterStatus e_eMagicTowerMonsterStatus);
	};

#endif