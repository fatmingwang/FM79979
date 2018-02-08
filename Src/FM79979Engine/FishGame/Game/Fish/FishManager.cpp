#include "stdafx.h"
#include "FishManager.h"
cFishManager::cFishManager()
{
	m_ppWorkingMonster = nullptr;;
	m_iWorkingMonsterSize = 0;
	//	m_pFAIBehaviorParser = nullptr;
}

cFishManager::~cFishManager()
{

}

bool	cFishManager::MyParse(TiXmlElement*e_pRoot)
{
	return false;
}

cFishBase*	cFishManager::ProcessMonster(TiXmlElement*e_pTiXmlElement, const WCHAR*l_strTypeID)
{
	return nullptr;
}

void	cFishManager::ProcessFishBodyTypeAmount(TiXmlElement* e_pTiXmlElement)
{

}

void	cFishManager::MonsterLeaveScene()
{

}

void	cFishManager::Init()
{

}

void	cFishManager::Update(float e_fElpaseTime)
{

}

void	cFishManager::Render()
{

}

void	cFishManager::RenderMonsterDiedShow()
{

}

void	cFishManager::DebugRender()
{

}

void	cFishManager::Destroy()
{

}

cFishBase*	cFishManager::FishRequire (int e_iFishIndex,cFAIMachine* e_pAI)
{
	return nullptr;
}

cFishBase*	cFishManager::GetMonsterByID(int e_iID)
{
	return nullptr;
}

int		cFishManager::GetFishIDByName(const wchar_t* e_pString)
{
	return -1;
}

eFishBodyType	cFishManager::GetFishSizeByID(int e_iFishIdx)
{
	return eFBT_Total;
}

eFishBodyType	cFishManager::GetFishSizeByName(const wchar_t* e_pString)
{
	return eFBT_Total;
}