#pragma once

#include "FishGameWeaponBase.h"


class	cFishGameTraceFishWeapon :public cFishGameWeaponBase
{
protected:
	virtual	bool							Fire()override;
public:
	cFishGameTraceFishWeapon();
	virtual ~cFishGameTraceFishWeapon();
	static cFishGameTraceFishWeapon*GetMe(TiXmlElement*e_pTiXmlElement);
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime,unsigned char*e_pucKey)override;
	virtual	void	Render()override;
	virtual bool	IsAllowFire()override;
	bool			IsLeave() { return false; }
};