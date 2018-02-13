#include "stdafx.h"
#include "PlayerBehavior.h"

cPlayerBehavior::cPlayerBehavior(TiXmlElement* e_pElement, cMatrix44 e_Matrix44, int e_iPlayerID)
{
	m_iPlayerID = -1;
	m_pWeaponManager = nullptr;//cWeaponManager*
}
cPlayerBehavior::~cPlayerBehavior()
{

}

void		cPlayerBehavior::ProcessPlayer(TiXmlElement* e_pElement)
{

}

void		cPlayerBehavior::Init()
{

}

void		cPlayerBehavior::Update( float e_fElpaseTime )
{

}

void		cPlayerBehavior::Render()
{

}

void		cPlayerBehavior::AddScore(int64 Money)
{

}

bool		cPlayerBehavior::AddScoreByMoneyChangeType(int64 e_i64AddScore, eMoneyChangeType e_eMoneyChangeType)
{
	return true;
}

void		cPlayerBehavior::SetupTimeUpFireAndLongPressedAutoFire(float e_fTimeUpFireTime, float e_fLongPressedAutoFireTime)
{

}