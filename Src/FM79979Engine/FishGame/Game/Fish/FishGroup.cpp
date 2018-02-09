#include "stdafx.h"
#include "FishGroup.h"
#include "FishManager.h"
#include "../GameDefine/GameParameterDefine.h"
#include "../GameDefine/FishGameEventMessageID.h"

cFishGroup::cFishGroup(cMPDI*e_pMPDI,cFishManager*e_pFishManager)
{
	m_fCurrentTime = 0.f;
	if (!e_pFishManager)
		return;
	std::wstring l_strName = e_pMPDI->GetName();
	l_strName += L":cFishGroup";
	assert(e_pMPDI&&"cFishGroup::cFishGroup MPDI not exists");
	this->SetName(l_strName);
	int l_iCount = e_pMPDI->Count();
	for (int i = 0; i < l_iCount; ++i)
	{
		cSubMPDI*l_pObject = (*e_pMPDI)[i];
		sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = l_pObject->GetPointData(0);
		if (l_pTexBehaviorDataWithImageIndexData)
		{
			auto l_pPIUnit = l_pTexBehaviorDataWithImageIndexData->GetPuzzleImageUnit();
			if (l_pPIUnit)
			{
				int l_iID = e_pFishManager->GetFishIDByName(l_pPIUnit->GetName());
				float l_fStartTime = l_pObject->GetStartTime();
				assert(l_iID != -1 && "FishName wrong!cFishGroup::cFishGroup");
				m_OriginalTimeAndFishIDPairVector.push_back(std::pair<float, int>(l_fStartTime, l_iID));
			}
		}
	}
	std::sort(m_OriginalTimeAndFishIDPairVector.begin(), m_OriginalTimeAndFishIDPairVector.end());
}
cFishGroup::~cFishGroup()
{

}
void	cFishGroup::Init()
{
	m_CurrentTimeAndFishIDPairVector = m_OriginalTimeAndFishIDPairVector;
	m_fCurrentTime = 0.f;
}

bool	cFishGroup::Update(float e_fElpaseTime, cFishManager*e_pFishManager)
{
	m_fCurrentTime += e_fElpaseTime;
	while(m_OriginalTimeAndFishIDPairVector.size())
	{
		auto l_Pair = m_OriginalTimeAndFishIDPairVector[0];
		float l_fTime = l_Pair.first;
		if (m_fCurrentTime >= l_fTime)
		{
			e_pFishManager->ForceFishRequire(l_Pair.second);
			m_OriginalTimeAndFishIDPairVector.erase(m_OriginalTimeAndFishIDPairVector.begin());
		}
	}
	return m_OriginalTimeAndFishIDPairVector.size() ? true : false;
}


//<FishGroup>
//	<Scene Name="OOXX" MPDIFileName="">
//		<FishGroup MPDIName="Name"/>
//		<FishGroup MPDIName="Name" / >
//		<FishGroup MPDIName="Name" / >
//		<FishGroup MPDIName="Name" / >
//	</Scene>
//	<Scene Name = "OOXX" MPDIFileName="">
//		<FishGroup MPDIName="Name"/>
//		<FishGroup MPDIName="Name" / >
//		<FishGroup MPDIName="Name" / >
//		<FishGroup MPDIName="Name" / >
//	< / Scene>
//</FishGroup>
cFishGroupManager::cFishGroupManager(cFishManager*e_pFishManager)
{
	m_pFishManager = e_pFishManager;
	m_pCurrentFishGroup = nullptr;
	m_AllFinishNotInSceneChangeToSceneBTC.SetTargetTime(ALL_FISH_NOT_IN_SCENE_CHANGE_TO_SCENEB_TIME);
	m_AllFinishNotInSceneChangeToSceneBTC.Start();
}
cFishGroupManager::~cFishGroupManager()
{

}
bool	cFishGroupManager::MyParse(TiXmlElement*e_pRoot)
{
	ASSERT_TARGET_ELEMENT_VALUE(e_pRoot, "FishGroup");
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pRoot)
		auto l_strSceneName = e_pRoot->Attribute(L"Name");
		auto l_strMPDIFileName = e_pRoot->Attribute(L"MPDIFileName");
		assert(l_strSceneName&&"scene name attribute not exists!!");
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strMPDIFileName,true);
		if (l_strSceneName && l_pMPDIList)
		{
			std::wstring l_strSceneName = l_strSceneName;;
			std::vector<int> l_IndexOfVector;
			auto l_pChildElement = e_pRoot->FirstChildElement();
			while (l_pChildElement)
			{
				COMPARE_TARGET_ELEMENT_VALUE(l_pChildElement, "FishGroup")
				{
					int l_iCount = this->Count();
					auto l_strMPDIName = ATTRIBUT_VALUE(l_pChildElement, "MPDIName");
					AddFishGroupByMPDIFile(l_strMPDIName, l_pMPDIList);
					l_IndexOfVector.push_back(l_iCount);

				}
				l_pChildElement = l_pChildElement->NextSiblingElement();
			}
			PushSceneAndFishGroupData(l_strSceneName, l_IndexOfVector);
		//	m_SceneIndexAndFishGroupCountMap
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
	return false;
}
void	cFishGroupManager::AddFishGroupByMPDIFile(const wchar_t* e_strFileName,cMPDIList*e_pMPDIList)
{
	cMPDI*l_pMPDI = e_pMPDIList->GetObject(e_strFileName);
	if (!l_pMPDI)
	{
		UT::ErrorMsg(e_strFileName, L"MPDI not exists!");
	}
	else
	{
		cFishGroup*l_pFishGroup = new cFishGroup(l_pMPDI, m_pFishManager);
		this->AddObjectNeglectExist(l_pFishGroup);
	}
}

void	cFishGroupManager::PushSceneAndFishGroupData(std::wstring e_strName, std::vector<int> e_Data)
{
	m_FishGroupVectorWithSceneName.insert(std::make_pair(e_strName, e_Data));
	size_t l_uiSize = m_FishGroupVectorWithSceneName.size();
	m_SceneIndexAndFishGroupCountMap.insert(std::make_pair(l_uiSize -1, e_Data.size()));
}

void	cFishGroupManager::ToNextScene()
{
	//do scene loop check
	m_CurrentSceneData.bSceneAOrB = !m_CurrentSceneData.bSceneAOrB;
	++m_CurrentSceneData.iSceneIndex;
	if (m_CurrentSceneData.iSceneIndex >= m_SceneIndexAndFishGroupCountMap.size())
		m_CurrentSceneData.iSceneIndex = 0;
	++m_CurrentSceneData.iFishGroupIndex;
	size_t l_uiCount = m_SceneIndexAndFishGroupCountMap[m_CurrentSceneData.iSceneIndex];
	if (m_CurrentSceneData.iFishGroupIndex >= l_uiCount)
		m_CurrentSceneData.iFishGroupIndex = 0;
	cGameApp::EventMessageShot(eFGEMI_SCENE_CHANGE,&m_CurrentSceneData);
}

void	cFishGroupManager::SetCurrentSceneName(const wchar_t*e_strSceneName)
{
	m_pCurrentFishGroup = (*this)[e_strSceneName];
	if (m_pCurrentFishGroup)
		m_pCurrentFishGroup->Init();
}

void	cFishGroupManager::Update(float e_fElpaseTime)
{
	if (m_pCurrentFishGroup)
	{
		if (m_pCurrentFishGroup->Update(e_fElpaseTime, m_pFishManager))
		{
			//m_pCurrentFishGroup = nullptr;
			m_AllFinishNotInSceneChangeToSceneBTC.Update(e_fElpaseTime);
			if (m_AllFinishNotInSceneChangeToSceneBTC.bTragetTimrReached)
			{
				m_pCurrentFishGroup = nullptr;
				m_AllFinishNotInSceneChangeToSceneBTC.Start();
				ToNextScene();
			}
		}
	}
}