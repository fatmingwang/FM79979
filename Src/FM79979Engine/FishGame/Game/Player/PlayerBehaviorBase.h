#pragma once

#include "PlayerMessageBehavior.h"

class	cPlayerBehaviorBase : public Frame,public cPlayerMessageBehavior
{
	friend class				cPlayerManager;
protected:
	virtual	void				InternalRender() = 0;
	virtual	void				InternalInit() = 0;
	int							m_iPlayerID;
	//to check how many bullet remain on the scene.
	GET_SET_DEC(int,m_iShootNumBuleet,GetShootNumBuleet,SetShootNumBuleet);
public:
	cPlayerBehaviorBase();
	~cPlayerBehaviorBase();
	//cPlayerWeapon*		m_pWeapon;
	int					GetPlayerID(){return m_iPlayerID;}
	virtual	void		Init(void);
	virtual	void		Update( float e_fElpaseTime );
	virtual	void		Render(void);
	virtual	void		AddScore(cMonster* Fish, int64 Money, bool e_bIsDied = true, bool e_bDieByMiniGame = false) = 0;
	int64				GetScore();
	virtual bool		AddScoreByMoneyChangeType(int64 e_i64AddScore, eMoneyChangeType e_eMoneyChangeType);
};