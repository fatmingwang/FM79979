#include "stdafx.h"
#include "SceneChangeFishGroup.h"
#include "FishManager.h"
#include "../GameDefine/GameParameterDefine.h"
#include "../GameDefine/FishGameEventMessageID.h"

TYPDE_DEFINE_MARCO(cSceneChangeFishGroup);
TYPDE_DEFINE_MARCO(cSceneChangeFishGroupManager);

cSceneChangeFishGroup::cSceneChangeFishGroup(cMPDI*e_pMPDI,cFishManager*e_pFishManager)
{
	m_fCurrentTime = 0.f;
	if (!e_pFishManager)
		return;
	std::wstring l_strName = e_pMPDI->GetName();
	l_strName += L":cSceneChangeFishGroup";
	assert(e_pMPDI&&"cSceneChangeFishGroup::cSceneChangeFishGroup MPDI not exists");
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
				assert(l_iID != -1 && "FishName wrong!cSceneChangeFishGroup::cSceneChangeFishGroup");
				m_OriginalTimeAndFishIDPairVector.push_back(std::pair<float, int>(l_fStartTime, l_iID));
			}
		}
	}
	std::sort(m_OriginalTimeAndFishIDPairVector.begin(), m_OriginalTimeAndFishIDPairVector.end());
}
cSceneChangeFishGroup::~cSceneChangeFishGroup()
{

}
void	cSceneChangeFishGroup::Init()
{
	m_CurrentTimeAndFishIDPairVector = m_OriginalTimeAndFishIDPairVector;
	m_fCurrentTime = 0.f;
}

bool	cSceneChangeFishGroup::Update(float e_fElpaseTime, cFishManager*e_pFishManager)
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

cSceneChangeFishGroupManager::cSceneChangeFishGroupManager(cFishManager*e_pFishManager)
{
	m_pFishManager = e_pFishManager;
	m_pCurrentFishGroup = nullptr;
	m_AllFinishNotInSceneChangeToSceneBTC.SetTargetTime(ALL_FISH_NOT_IN_SCENE_CHANGE_TO_SCENEB_TIME);
	m_AllFinishNotInSceneChangeToSceneBTC.Start();
}
cSceneChangeFishGroupManager::~cSceneChangeFishGroupManager()
{

}

//	<SceneChangeFishGroup MPDIFileName="SFG1" Name="">
//		<FishGroup MPDIName="Name1" />
//		<FishGroup MPDIName="Name2" / >
//		<FishGroup MPDIName="Name3" / >
//		<FishGroup MPDIName="Name4" / >
//	</SceneChangeFishGroup>

int	cSceneChangeFishGroupManager::ProcessSceneChangeFishGroup(TiXmlElement*e_pRoot)
{
	ASSERT_TARGET_ELEMENT_VALUE(e_pRoot, "SceneChangeFishGroup");
	auto l_strMPDIFileName = e_pRoot->Attribute(L"MPDIFileName");
	std::wstring l_strFishGroupName = e_pRoot->Attribute(L"Name");;
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strMPDIFileName,true);
	if (l_pMPDIList)
	{
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
		m_FishGroupNameAndIndexOfNamedTypedObjectVectorVector.insert(std::make_pair(l_strFishGroupName, l_IndexOfVector));
		return (int)l_IndexOfVector.size();
	}
	return 0;
}
void	cSceneChangeFishGroupManager::AddFishGroupByMPDIFile(const wchar_t* e_strFileName,cMPDIList*e_pMPDIList)
{
	cMPDI*l_pMPDI = e_pMPDIList->GetObject(e_strFileName);
	if (!l_pMPDI)
	{
		UT::ErrorMsg(e_strFileName, L"MPDI not exists!");
	}
	else
	{
		cSceneChangeFishGroup*l_pFishGroup = new cSceneChangeFishGroup(l_pMPDI, m_pFishManager);
		this->AddObjectNeglectExist(l_pFishGroup);
	}
}

void	cSceneChangeFishGroupManager::Update(float e_fElpaseTime)
{
	if (m_pCurrentFishGroup)
	{
		if (m_pCurrentFishGroup->Update(e_fElpaseTime, m_pFishManager))
		{
			//m_pCurrentFishGroup = nullptr;
			m_AllFinishNotInSceneChangeToSceneBTC.Update(e_fElpaseTime);
			if (m_AllFinishNotInSceneChangeToSceneBTC.bTragetTimrReached)
			{
				cGameApp::EventMessageShot(eFGEMI_FISH_GROUP_FINISH,nullptr);
				m_pCurrentFishGroup = nullptr;
				m_AllFinishNotInSceneChangeToSceneBTC.Start();
			}
		}
	}
}

//bool	cSceneChangeFishGroupManager::IsAllowToChangeSceneWhenFishGroupFinish()
//{
//	return m_pCurrentFishGroup ? false : true;
//}

void	cSceneChangeFishGroupManager::SetCurrentFishGroup(std::wstring e_strFishGroupName, int e_FishGroupIndexOfScene)
{
	auto l_Iterator = m_FishGroupNameAndIndexOfNamedTypedObjectVectorVector.find(e_strFishGroupName);
	assert(l_Iterator != m_FishGroupNameAndIndexOfNamedTypedObjectVectorVector.end()&&"SetCurrentFishGroup no this scene name");
	int l_iIndex = l_Iterator->second[e_FishGroupIndexOfScene];
	assert(l_Iterator->second.size() > e_FishGroupIndexOfScene && "e_FishGroupIndexOfScene is wrong!");
	m_pCurrentFishGroup = (*this)[l_iIndex];
	assert(m_pCurrentFishGroup);
	if(m_pCurrentFishGroup)
		m_pCurrentFishGroup->Init();
}