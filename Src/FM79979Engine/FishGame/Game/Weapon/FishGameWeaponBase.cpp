#include "stdafx.h"
#include "FishGameWeaponBase.h"
cFishGameWeaponBase::cFishGameWeaponBase()
{
	for (int i = 0; i < eWS_MAX; ++i)
		m_pWeaponAnimation[i] = nullptr;;
	m_fWeaponFireSpeed = 1.f;
	m_eMovingDirection = eD_MAX;
	m_bFire = false;
}
cFishGameWeaponBase::~cFishGameWeaponBase()
{

}