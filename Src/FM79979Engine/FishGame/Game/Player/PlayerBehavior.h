#pragma once

#include "PlayerMessageBehavior.h"

class cWeaponManager;
class	cPlayerBehavior : public Frame,public cPlayerMessageBehavior
{
	friend class							cPlayerManager;
	int										m_iPlayerID;
	cWeaponManager*							m_pWeaponManager;
protected:
	void									ProcessPlayer(TiXmlElement* e_pElement);
public:
	cPlayerBehavior(TiXmlElement* e_pElement, cMatrix44 e_Matrix44, int e_iPlayerID);
	~cPlayerBehavior();
	int					GetPlayerID(){return m_iPlayerID;}
	virtual	void		Init();
	virtual	void		Update( float e_fElpaseTime )override;
	virtual	void		Render()override;
	void				AddScore(int64 Money);
	bool				AddScoreByMoneyChangeType(int64 e_i64AddScore, eMoneyChangeType e_eMoneyChangeType);
	void				SetupTimeUpFireAndLongPressedAutoFire(float e_fTimeUpFireTime, float e_fLongPressedAutoFireTime);
};