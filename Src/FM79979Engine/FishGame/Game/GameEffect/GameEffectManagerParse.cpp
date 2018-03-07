#include "stdafx.h"
#include "GameEffectBase.h"


bool	cGameEffectManager::MyParse(TiXmlElement*e_pRoot)
{
	return true;
}

bool	cGameEffectManager::ProcessGameEffect(TiXmlElement*e_pRoot)
{
	return true;
}


void cFishGameCommonRegisterManager<cGameEffectBase>::AddCloneRegisterFunction()
{
	int a = 0;
}