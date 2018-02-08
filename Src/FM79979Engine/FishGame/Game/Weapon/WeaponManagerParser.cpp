#include "stdafx.h"
#include "FishGameWeaponManager.h"
#include "FishGameTraceFishWeapon.h"
#include "FishGameMiniGameWeapon.h"
#include "FishGameCommonWeapon.h"


void	cWeaponManager::AddCloneRegisterFunction()
{
	REGISTER_CLONE_FUNCTION_TO_MANGER(cFishGameCommonWeapon);
	REGISTER_CLONE_FUNCTION_TO_MANGER(cFishGameMiniGameWeapon);
	REGISTER_CLONE_FUNCTION_TO_MANGER(cFishGameTraceFishWeapon);
}


bool	cWeaponManager::MyParse(TiXmlElement*e_pRoot)
{
	return true;
}