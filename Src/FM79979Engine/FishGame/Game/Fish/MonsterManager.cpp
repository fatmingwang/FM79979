#include "stdafx.h"
#include "MonsterManager.h"
cMonsterManager::cMonsterManager()
{
	m_ppWorkingMonster = nullptr;;
	m_iWorkingMonsterSize = 0;
	//	m_pFAIBehaviorParser = nullptr;
}

cMonsterManager::~cMonsterManager()
{

}

bool	cMonsterManager::MyParse(TiXmlElement*e_pRoot)
{
	return false;
}

cMonsterBase*	cMonsterManager::ProcessMonster(TiXmlElement*e_pTiXmlElement, const WCHAR*l_strTypeID)
{
	return nullptr;
}

void	cMonsterManager::ProcessFishBodyTypeAmount(TiXmlElement* e_pTiXmlElement)
{

}

void	cMonsterManager::MonsterLeaveScene()
{

}

void	cMonsterManager::Init()
{

}

void	cMonsterManager::Update(float e_fElpaseTime)
{

}

void	cMonsterManager::Render()
{

}

void	cMonsterManager::RenderMonsterDiedShow()
{

}

void	cMonsterManager::DebugRender()
{

}

void	cMonsterManager::Destroy()
{

}

cMonsterBase*	cMonsterManager::FishRequire (int e_iFishIndex,cFAIMachine* e_pAI)
{
	return nullptr;
}

cMonsterBase*	cMonsterManager::GetMonsterByID(int e_iID)
{
	return nullptr;
}

int		cMonsterManager::GetFishIDByName(const wchar_t* e_pString)
{
	return -1;
}

eBodyType	cMonsterManager::GetFishSizeByID(int e_iFishIdx)
{
	return eBT_Total;
}

eBodyType	cMonsterManager::GetFishSizeByName(const wchar_t* e_pString)
{
	return eBT_Total;
}