#include "stdafx.h"
#include "FishManager.h"
#include "../Probability/FishShowPaobability.h"
#include "../GameDefine/GameDefine.h"
#include "../GameDefine/GameParameterDefine.h"
const int g_ciWorkingMonsterCount = 500;
cFishManager::cFishManager()
{
	m_WorkingMonster.SetCount(g_ciWorkingMonsterCount);
	m_pFishShowProbability = nullptr;
}

cFishManager::~cFishManager()
{
	Destroy();
}

bool	cFishManager::MyParse(TiXmlElement*e_pRoot)
{
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pRoot)
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pRoot, L"cFishShowProbability")
		{
			m_pFishShowProbability = new cFishShowProbability(e_pRoot);
		}
		else
		{
			auto l_pObject = cFishGameCommonRegisterManager::GetObjectByParseXmlElement(e_pRoot);
			if (l_pObject)
			{
				assert(GetFishByID(l_pObject->GetID()) == nullptr &&"same fish ID" );
				m_FishIDAndIndexOfVector.insert(std::make_pair(l_pObject->GetID(),this->Count()));
				this->AddObjectNeglectExist(l_pObject);
			}
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
	GenerateReservedFishAndRenderOrder();
	return false;
}

void	cFishManager::GenerateReservedFishAndRenderOrder()
{
	//generate
	std::vector<int>	l_OriginalRenderVector;
	std::vector<int>	l_RenderVector;
	if (m_pFishShowProbability)
	{
		const int*l_piFishBodyTypeAllowToShowCountWithCurrentPlayerCountSetup = m_pFishShowProbability->GetFishBodyTypeAllowToShowCountWithCurrentPlayerCountSetup();
		if (l_piFishBodyTypeAllowToShowCountWithCurrentPlayerCountSetup)
		{
			int l_iCount = this->Count();
			for (int i = 0; i < l_iCount; ++i)
			{
				auto l_pFish = (*this)[i];
				int l_iFishBodyReservedCount = l_piFishBodyTypeAllowToShowCountWithCurrentPlayerCountSetup[l_pFish->GetFishBodyType()];
				assert(l_iFishBodyReservedCount> 0 && "fish body type count is small than 0!?");
				//ex:small need 65 is too much it may need 65/2
				//l_iFishBodyReservedCount /= 2;
				if (l_iFishBodyReservedCount < 5)
					l_iFishBodyReservedCount = 5;
				std::vector<cFishBase*>*l_pFishVector = new std::vector<cFishBase*>;
				for (int j=0; j < l_iFishBodyReservedCount; ++j)
				{
					cFishBase*l_pFishBase = dynamic_cast<cFishBase*>(l_pFish->Clone());
					l_pFishVector->push_back(l_pFishBase);
				}
				m_AllFish.push_back(l_pFishVector);
				int l_iValue = (int)l_pFish->GetLocalPosition().z;
				l_RenderVector.push_back(l_iValue);
			}
		}
	}
	assert((int)l_RenderVector.size() == this->Count()&&"fish count is not match render order!?");
	l_OriginalRenderVector = l_RenderVector;
	std::sort(l_RenderVector.begin(), l_RenderVector.begin());
	int l_iOriginalRenderVectorSize = (int)l_OriginalRenderVector.size();
	int l_iRenderVectorSize = (int)l_RenderVector.size();
	for (int i = 0; i < l_iRenderVectorSize; ++i)
	{
		for (int j = 0; j < l_iOriginalRenderVectorSize; ++j)
		{
			if (l_OriginalRenderVector[i] == l_RenderVector[j])
			{
				auto l_pFish = (*this)[i];
				m_FishIDAndRenderOrderIndex[l_pFish->GetID()] = i;
				cTempContainer<cFishBase>*l_pTempContainer = new cTempContainer<cFishBase>();
				l_pTempContainer->SetCount(g_ciWorkingMonsterCount);
				m_RenderWorkingMonsterVector.push_back(l_pTempContainer);
				break;
			}
			
		}
	}
}

void	cFishManager::MonsterLeaveScene()
{
	for (int i = 0; i < m_WorkingMonster.m_iNumWorking; ++i)
	{
		auto l_pFish = m_WorkingMonster.m_ppObjects[i];
		if(l_pFish->IsStatusAllowToDied())
			l_pFish->MonsterLeave();
	}
	
}

void	cFishManager::Init()
{
	Destroy();
	m_pFAIBehaviorParser = new cFAIBehaviorParser();
	this->ParseWithMyParse(GetFishGameFileName(eFGFN_FISH_MANAGER_FILE_NAME).c_str());
	if(m_pFishShowProbability)
		m_pFishShowProbability->Init();
}

void	cFishManager::Update(float e_fElpaseTime)
{
	cFishShowProbability::sCurrentFishBodyTypeFishCount*l_pCurrentFishBodyTypeFishCount = nullptr;
	if(m_pFishShowProbability)
		l_pCurrentFishBodyTypeFishCount = m_pFishShowProbability->GetCurrentFishBodyTypeFishCount();
	int l_iWaitForFetchFishCount[eFBT_Total] = {0,0,0,0};
	size_t l_uiRenderWorkingMonsterVectorSize = m_RenderWorkingMonsterVector.size();
	for (size_t i = 0; i < l_uiRenderWorkingMonsterVectorSize; ++i)
	{
		auto l_pData = m_RenderWorkingMonsterVector[i];
		l_pData->m_iNumWorking = 0;
	}
	for (int i = 0; i < m_WorkingMonster.m_iNumWorking; ++i)
	{
		auto l_pFish = m_WorkingMonster.m_ppObjects[i];
		if (l_pFish)
		{
			l_pFish->Update(e_fElpaseTime);
			if (l_pFish->GetFishStatus() == eFishStatus::eFS_WAITING_FOR_CLEAN)
			{
				--m_WorkingMonster.m_iNumWorking;
				++l_iWaitForFetchFishCount[l_pFish->GetFishBodyType()];
				m_WorkingMonster.m_ppObjects[i]->SetFishStatusToWaitForFetch();
				m_WorkingMonster.m_ppObjects[i] = m_WorkingMonster.m_ppObjects[m_WorkingMonster.m_iNumWorking];
				if (l_pCurrentFishBodyTypeFishCount)
				{
					--l_pCurrentFishBodyTypeFishCount->iFishCountByFishBodyType[l_pFish->GetFishBodyType()];
					assert(l_pCurrentFishBodyTypeFishCount->iFishCountByFishBodyType[l_pFish->GetFishBodyType()] >= 0 && "fish update get serious wrong!");
				}
			}
			else
			{
				m_RenderWorkingMonsterVector[m_FishIDAndRenderOrderIndex[l_pFish->GetID()]]->Add(l_pFish);
			}
		}
	}
	if (m_pFishShowProbability)
	{
		m_pFishShowProbability->Update(e_fElpaseTime);
		size_t l_uiSize = l_pCurrentFishBodyTypeFishCount->iGenerateFishIDVector.size();
		for (size_t i = 0; i < l_uiSize; ++i)
		{
			int l_iFishID = l_pCurrentFishBodyTypeFishCount->iGenerateFishIDVector[i];
			auto l_pFish = this->ForceFishRequire(l_iFishID);
			if (l_pFish && l_pCurrentFishBodyTypeFishCount)
			{
				++l_pCurrentFishBodyTypeFishCount->iFishCountByFishBodyType[l_pFish->GetFishBodyType()];
			}
		}
		if(l_pCurrentFishBodyTypeFishCount)
			l_pCurrentFishBodyTypeFishCount->iGenerateFishIDVector.clear();
		//FishDired(eFishBodyType e_eFishBodyType);
	}
}

void	cFishManager::Render()
{
	size_t l_uiRenderWorkingMonsterVectorSize = m_RenderWorkingMonsterVector.size();
	for (size_t i = 0; i < l_uiRenderWorkingMonsterVectorSize; ++i)
	{
		auto l_pData = m_RenderWorkingMonsterVector[i];
		for (size_t j = 0; j < l_pData->m_iNumWorking; ++j)
		{
			if (l_pData->m_ppObjects[j]->GetFishStatus() != eFishStatus::eFS_DIED_SHOW)
				l_pData->m_ppObjects[j]->Render();
		}
	}
	RenderMonsterDiedShow();
}

void	cFishManager::RenderMonsterDiedShow()
{
	size_t l_uiRenderWorkingMonsterVectorSize = m_RenderWorkingMonsterVector.size();
	for (size_t i = 0; i < l_uiRenderWorkingMonsterVectorSize; ++i)
	{
		auto l_pData = m_RenderWorkingMonsterVector[i];
		for (size_t j = 0; j < l_pData->m_iNumWorking; ++j)
		{
			if (l_pData->m_ppObjects[j]->GetFishStatus() == eFishStatus::eFS_DIED_SHOW)
				l_pData->m_ppObjects[j]->Render();
		}
	}
}

void	cFishManager::DebugRender()
{
	for (int i = 0; i < m_WorkingMonster.m_iNumWorking; ++i)
	{
		auto l_pFish = m_WorkingMonster.m_ppObjects[i];
		if (l_pFish)
		{
			l_pFish->DebugRender();
		}
	}
	cFishShowProbability::sCurrentFishBodyTypeFishCount*l_pCurrentFishBodyTypeFishCount = m_pFishShowProbability->GetCurrentFishBodyTypeFishCount();
	if (l_pCurrentFishBodyTypeFishCount)
	{
		Vector2 l_vPos = FISH_BODY_AMOUNT_DEBUG_RENDER_POS;
		for (int i = 0; i < eFBT_Total; ++i)
		{
			int l_iCount = l_pCurrentFishBodyTypeFishCount->iFishCountByFishBodyType[i];
			std::wstring l_str = FishBodyTypeToString((eFishBodyType)i);
			l_str += L":";
			l_str += ValueToStringW(l_iCount);
			cGameApp::RenderFont(l_vPos.x, l_vPos.y, l_str.c_str());
			l_vPos.y += 30;
		}
	}
}

void	cFishManager::Destroy()
{
	SAFE_DELETE(m_pFishShowProbability);
	size_t l_uiFishVectorSize = m_AllFish.size();
	for (size_t i = 0; i<l_uiFishVectorSize; ++i)
	{
		DELETE_VECTOR((*m_AllFish[i]));
	}
	DELETE_VECTOR(m_AllFish);
	DELETE_VECTOR(m_RenderWorkingMonsterVector);
	SAFE_DELETE(m_pFAIBehaviorParser);
}

cFishBase*	cFishManager::RequireWaitForFetchFish(int e_iFishID)
{
	assert(m_FishIDAndIndexOfVector.find(e_iFishID) != m_FishIDAndIndexOfVector.end()&&"no this fish ID!?");
	int l_iIndexOfVector = m_FishIDAndIndexOfVector[e_iFishID];
	cFishBase*l_pFish = GetWaitForFetchFishFromVector(m_AllFish[l_iIndexOfVector]);
	if (l_pFish)
	{
		l_pFish->Init();
		m_WorkingMonster.Add(l_pFish);
	}
	assert(m_WorkingMonster.m_iNumWorking <= g_ciWorkingMonsterCount && "fish over 500!?");
	return l_pFish;
}

cFishBase*	cFishManager::GetWaitForFetchFishFromVector(std::vector<cFishBase*>*e_pFishVector)
{
	size_t l_uiSize = e_pFishVector->size();
	for (size_t i = 0; i < l_uiSize; ++i)
	{
		if ((*e_pFishVector)[i]->GetFishStatus() == eFS_WAIT_FOR_FETCH)
		{
			return (*e_pFishVector)[i];
		}
	}
	return nullptr;
}

cFishBase*	cFishManager::ForceFishRequire(int e_iFishID)
{
	cFishBase*l_pFish = RequireWaitForFetchFish(e_iFishID);
	if (!l_pFish)
	{
		int l_iIndexOfVector = m_FishIDAndIndexOfVector[e_iFishID];
		l_pFish = dynamic_cast<cFishBase*>((*m_AllFish[l_iIndexOfVector])[0]->Clone());
		m_AllFish[l_iIndexOfVector]->push_back(l_pFish);
		l_pFish->Init();
		m_WorkingMonster.Add(l_pFish);
#ifndef RETAILER
		std::wstring l_str = L"Fish not enough!?ID is";
		l_str += ValueToStringW(e_iFishID).c_str();
		cGameApp::OutputDebugInfoString(l_str);
#endif
	}
	assert(m_WorkingMonster.m_iNumWorking <= g_ciWorkingMonsterCount && "fish over 500!?");
	return l_pFish;
}

const cFishBase*	cFishManager::GetFishByID(int e_iID)
{
	int l_iCount = this->Count();
	for (int i = 0; i < l_iCount; ++i)
	{
		auto l_pFish = (*this)[i];
		if (l_pFish->GetID() == e_iID)
			return l_pFish;
	}
	return nullptr;
}