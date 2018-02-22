#include "stdafx.h"
#include "GameEffectBase.h"

cGameEffectBase::cGameEffectBase() 
{
	m_eGameEffectType = eGET_MAX;
	m_bGameEffectDone = true;
}

cGameEffectBase::~cGameEffectBase()
{
	m_iRenderPirority = 0;
}

cGameEffectManager::cGameEffectManager()
{

}

cGameEffectManager::~cGameEffectManager()
{
	DELETE_MAP(m_AllGameEffecBaseMap);
}


cGameEffectBase* cGameEffectManager::Require(eGameEffectType e_eGameEffectType)
{
	auto l_Iterator = m_AllGameEffecBaseMap.find(e_eGameEffectType);
	assert(l_Iterator != m_AllGameEffecBaseMap.end()&&"GameEffectManager::Require(eGameEffectType e_eGameEffectType) no object");
	cWaitForFetchFunctionObjectList<cGameEffectBase>*l_pList = l_Iterator->second;
	return l_pList->RequireAvaliableObject();
}

void	cGameEffectManager::Update(float e_fElpaseTime)
{
	for (auto l_Iterator = m_AllGameEffecBaseMap.begin(); l_Iterator != m_AllGameEffecBaseMap.end(); ++l_Iterator)
	{
		auto l_pWorkingList = &l_Iterator->second->m_WorkingList;
		for (auto l_WorkingListIterator = l_pWorkingList->begin(); l_WorkingListIterator != l_pWorkingList->end(); ++l_WorkingListIterator)
		{
			(*l_WorkingListIterator)->Update(e_fElpaseTime);
			if ((*l_WorkingListIterator)->IsGameEffectDone())
			{
				l_pWorkingList->erase(l_WorkingListIterator);
				l_Iterator->second->m_WaitFetchList.push_back((*l_WorkingListIterator));
			}
		}
	}
}

void	cGameEffectManager::BeforeRender()
{

}

void	cGameEffectManager::Render()
{

}

void	cGameEffectManager::LastRender()
{

}

cGameEffectBase * RequireGameEffect(eGameEffectType e_eGameEffectType)
{
	return nullptr;
}