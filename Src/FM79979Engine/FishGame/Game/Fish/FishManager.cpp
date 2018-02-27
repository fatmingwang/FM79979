#include "stdafx.h"
#include "FishManager.h"
#include "../Probability/FishShowPaobability.h"
cFishManager::cFishManager()
{
	m_ppWorkingMonster = nullptr;;
	m_iWorkingMonsterSize = 0;
	m_pFishShowProbability = nullptr;
	//	m_pFAIBehaviorParser = nullptr;
}

cFishManager::~cFishManager()
{
	SAFE_DELETE(m_pFishShowProbability);
}

bool	cFishManager::MyParse(TiXmlElement*e_pRoot)
{
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pRoot)
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pRoot, L"FishBodyTypeAmount")
		{
			m_pFishShowProbability = new cFishShowProbability(e_pRoot);
		}
		else
		{
			auto l_pObject = cFishGameCommonRegisterManager::GetObject(e_pRoot);
			if (l_pObject)
			{
				assert(GetMonsterByID(l_pObject->GetID()) == nullptr &&"same fish ID" );
				this->AddObjectNeglectExist(l_pObject);
			}
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
	return false;
}

void	cFishManager::MonsterLeaveScene()
{

}

void	cFishManager::Init()
{
	if(m_pFishShowProbability)
		m_pFishShowProbability->Init();
}

void	cFishManager::Update(float e_fElpaseTime)
{
	int l_iWaitForFetchFishCount[eFBT_Total] = {0,0,0,0};
	if (m_ppWorkingMonster)
	{
		for (int i = 0; i < m_iWorkingMonsterSize; ++i)
		{
			if (m_ppWorkingMonster[i])
			{
				m_ppWorkingMonster[i]->Update(e_fElpaseTime);
				if (m_ppWorkingMonster[i]->GetFishStatus() == eFishStatus::eFS_WAITING_FOR_CLEAN)
				{
					--m_iWorkingMonsterSize;
					++l_iWaitForFetchFishCount[m_ppWorkingMonster[i]->GetFishBodyType()];
					m_ppWorkingMonster[i] = m_ppWorkingMonster[m_iWorkingMonsterSize];
				}
			}
		}
	}
	if (m_pFishShowProbability)
	{
		m_pFishShowProbability->Update(e_fElpaseTime);
		//FishDired(eFishBodyType e_eFishBodyType);
	}
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

cFishBase*	cFishManager::ForceFishRequire(int e_iFishIndex, cFAIMachine* e_pAI)
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