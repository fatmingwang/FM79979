#include "stdafx.h"
#include "SceneManager.h"
#include "BaseScene.h"
#include "CameraShake.h"
#include "../GameDefine/FishGameEventMessageID.h"
#include "../GameDefine/GameDefine.h"
#include "../Fish/SceneChangeFishGroup.h"
cSceneManager::cSceneManager(cFishManager*e_pFishManager):cWriteBinaryFile(false)
{
	m_pFishManagerReference = e_pFishManager;
	m_pCameraShake = nullptr;//cCameraShake
	m_iFishGroupCount = 0;
	m_pSceneChangeFishGroupManager = new cSceneChangeFishGroupManager(m_pFishManagerReference);
	REG_EVENT(eFGEMI_SCENE_CHANGE, &cSceneManager::SceneChangeEvent);
	REG_EVENT(eFGEMI_FISH_GROUP_GO, &cSceneManager::FishGroupGoEvent);
	std::string	l_strFileName = GetFishGameFileName(eFGFN_SCENE_MANAGER_FILE_NAME);
	sRegister_Header_Struct e_sRegister_Header_Struct;
	e_sRegister_Header_Struct.chID[0] = 'S';
	e_sRegister_Header_Struct.chID[1] = 'M';
	e_sRegister_Header_Struct.fVersion = 1.0f;
	OpenFileWhatEverFileIsExitOrNotExit(l_strFileName.c_str(), e_sRegister_Header_Struct,false);
}

cSceneManager::~cSceneManager()
{
	SAFE_DELETE(m_pCameraShake);
	SAFE_DELETE(m_pSceneChangeFishGroupManager);
}
//<!-- I am so lazy each  SceneChangeFishGroup must has same count data(FishGroup)-->
//<Scene>
//	<SceneChangeFishGroup MPDIFileName="SFG1" Name="">
//		<FishGroup MPDIName="Name1" />
//		<FishGroup MPDIName="Name2" / >
//		<FishGroup MPDIName="Name3" / >
//		<FishGroup MPDIName="Name4" / >
//	</SceneChangeFishGroup>
//<SceneContent Name="1A" NextSceneName="">
//	<cBaseScene SubSceneIndex="0" MPDIFileName="ooxx.mpdi" StartMPDI="Start" LoopMPDI="Loop" EndMPDI="End"  Time="300" SceneChangeFishGroupName="SFG1"/>
//	<cBaseScene SubSceneIndex="1" MPDIFileName="ooxx.mpdi" StartMPDI="Start" LoopMPDI="Loop" EndMPDI="End"  Time="300"/>
//</SceneContent>
//<Scene>

bool	cSceneManager::MyParse(TiXmlElement*e_pTiXmlElement)
{
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pTiXmlElement)
		COMPARE_TARGET_ELEMENT_VALUE(e_pTiXmlElement,"SceneChangeFishGroup")
		{
			ProcessSceneContent(e_pTiXmlElement);
		}
		else
		COMPARE_TARGET_ELEMENT_VALUE(e_pTiXmlElement, "SceneContent")
		{
			ProcessFishGroup(e_pTiXmlElement);
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pTiXmlElement)
	return true;
}

void	cSceneManager::ProcessSceneContent(TiXmlElement*e_pTiXmlElement)
{
	auto l_strName = e_pTiXmlElement->Attribute(L"Name");
	auto l_strNextSceneName = e_pTiXmlElement->Attribute(L"NextSceneName");
	cPhaseManager*l_pPhaseManager = new cPhaseManager();
	l_pPhaseManager->SetName(l_strName);
	l_pPhaseManager->SetNextPhaseName(l_strNextSceneName);
	int l_iCount = 0;
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pTiXmlElement)
		COMPARE_TARGET_ELEMENT_VALUE(e_pTiXmlElement, "cBaseScene")
		{
			cBaseScene*l_pBaseScene = new cBaseScene(e_pTiXmlElement);
			l_pBaseScene->SetName(ValueToStringW(l_iCount));
			++l_iCount;
			l_pPhaseManager->AddObject(l_pBaseScene);
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pTiXmlElement)
}
void	cSceneManager::ProcessFishGroup(TiXmlElement*e_pTiXmlElement)
{
	if (m_pSceneChangeFishGroupManager)
	{
		m_pSceneChangeFishGroupManager->ProcessSceneChangeFishGroup(e_pTiXmlElement);
	}
}

void	cSceneManager::LastRender()
{

}

void	cSceneManager::DebugRender()
{

}

void	cSceneManager::Update(float e_fElpaseTime)
{
	cPhaseManager::Update(e_fElpaseTime);
	if (m_pSceneChangeFishGroupManager)
	{
		m_pSceneChangeFishGroupManager->Update(e_fElpaseTime);
	}
}

bool	cSceneManager::SceneChangeEvent(void*e_pFishGroupName)
{
	//do scene loop check
	++m_CurrentSceneData.iSceneIndex;
	if (m_CurrentSceneData.iSceneIndex >= this->Count())
	{
		m_CurrentSceneData.iSceneIndex = 0;
		++m_CurrentSceneData.iFishGroupIndex;
		if (m_CurrentSceneData.iFishGroupIndex >= m_iFishGroupCount)
			m_CurrentSceneData.iFishGroupIndex = 0;
	}
	return true;
}

bool	cSceneManager::FishGroupGoEvent(void*e_pFishGroupName)
{
	//std::wstring*l_strFishGroupName = (std::wstring*)e_pFishGroupName;
	if (m_pSceneChangeFishGroupManager && m_pCurrentWorkingObject)
	{
		m_pSceneChangeFishGroupManager->SetCurrentFishGroup(m_pCurrentWorkingObject->GetName(), m_CurrentSceneData.iFishGroupIndex);
	}
//	this->m_p
	return true;
}

bool	cSceneManager::OpenFileGetData(int e_iDataSizeWithOutFileExtension, char*e_pData, sRegister_Header_Struct*e_pRegister_Header_Struct)
{
	if (e_pRegister_Header_Struct->fVersion <= 1.f)
	{
		sSceneData*l_pSceneData = (sSceneData*)e_pData;
		m_CurrentSceneData = *l_pSceneData;
		this->SetCurrentPhase(m_CurrentSceneData.iSceneIndex);
		return true;
	}
	return false;
}

void	cSceneManager::WriteFileUpdate(float e_fElpaseTime)
{
	NvFSeek(m_pFile, sizeof(sSceneData), SEEK_SET);
	NvFWrite(&m_CurrentSceneData, sizeof(m_CurrentSceneData), 1, m_pFile);
	this->Flush();
}