#include "stdafx.h"
#include "FishManager.h"
#include "FishBase.h"
#include "FixedPathFish.h"
#include "FAIFish.h"

//void cFishGameCommonRegisterManager<class cFishBase>::cFishGameCommonRegisterManager<cFishBase>::AddCloneRegisterFunction()
void cFishGameCommonRegisterManager<cFishBase>::AddCloneRegisterFunction()
//void	cFishGameCommonRegisterManager<cFishBase>::AddCloneRegisterFunction()
{
	REGISTER_CLONE_FUNCTION_TO_MANGER(cFAIFish);
	//REGISTER_CLONE_FUNCTION_TO_MANGER(cFixedPathFish);
}