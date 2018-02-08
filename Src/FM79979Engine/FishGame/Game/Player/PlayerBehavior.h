#pragma once

#include "PlayerMessageBehavior.h"
#include "../WriteFile/WriteFile.h"

class	cPlayerBehavior : public Frame,public cPlayerMessageBehavior
{
	friend class							cPlayerManager;
	int										m_iPlayerID;
protected:
	void									ProcessPlayer(TiXmlElement* e_pElement);
	virtual	void							InternalRender() = 0;
	virtual	void							InternalUpdate(float e_fElpaseTime) = 0;
	virtual	void							InternalInit() = 0;
public:
	cPlayerBehavior(TiXmlElement* e_pElement, cMatrix44 e_Matrix44, int e_iPlayerID);
	~cPlayerBehavior();
	int					GetPlayerID(){return m_iPlayerID;}
	virtual	void		Init(void);
	virtual	void		Update( float e_fElpaseTime );
	virtual	void		Render(void);
	virtual	void		AddScore(cMonster* Fish, int64 Money, bool e_bIsDied = true, bool e_bDieByMiniGame = false) = 0;
	int64				GetScore();
	virtual bool		AddScoreByMoneyChangeType(int64 e_i64AddScore, eMoneyChangeType e_eMoneyChangeType);
	void				SetupTimeUpFireAndLongPressedAutoFire(float e_fTimeUpFireTime, float e_fLongPressedAutoFireTime);
};