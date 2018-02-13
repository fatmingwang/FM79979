#include "stdafx.h"
#include "BaseScene.h"
#include "../GameDefine/FishGameEventMessageID.h"

cBaseScene::cBaseScene(TiXmlElement*e_pTiXmlElement)
{
	for (int i = 0; i < eSS_MAX; ++i)
		m_pBGMPDI[i] = nullptr;
	m_pstrSceneChangeFishGroupName = nullptr;
	m_pToNextSceneTC = nullptr;

	if(m_pstrSceneChangeFishGroupName)
		REG_EVENT(eFGEMI_FISH_GROUP_FINISH, &cBaseScene::FishGroupFinishEvent);
	e_pTiXmlElement;
	//<SceneContent Name="1A" NextSceneName="">
	//	<cBaseScene MPDIFileName="ooxx.mpdi" StartMPDI="Start" LoopMPDI="Loop" EndMPDI="End"  Time="300" SceneChangeFishGroupName="SFG1"/>
	//	<cBaseScene MPDIFileName="ooxx.mpdi" StartMPDI="Start" LoopMPDI="Loop" EndMPDI="End"  Time="300"/>
	//</SceneContent>
	cMPDIList*l_pMPDIList = nullptr;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("MPDIFileName")
		{
			l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue,true);
		}
		else
		COMPARE_NAME("StartMPDI")
		{
			if (l_pMPDIList)
				m_pBGMPDI[eSS_START] = l_pMPDIList->GetObject(l_strValue);
		}
		else
		COMPARE_NAME("LoopMPDI")
		{
		if (l_pMPDIList)
			m_pBGMPDI[eSS_LOOP] = l_pMPDIList->GetObject(l_strValue);
		}
		else
		COMPARE_NAME("EndMPDI")
		{
		if (l_pMPDIList)
			m_pBGMPDI[eSS_END] = l_pMPDIList->GetObject(l_strValue);
		}
		else
		COMPARE_NAME("Time")
		{
			m_pToNextSceneTC = new sTimeCounter();
			m_pToNextSceneTC->SetTargetTime(VALUE_TO_FLOAT);
		}
		else
		COMPARE_NAME("SceneChangeFishGroupName")
		{
			m_pstrSceneChangeFishGroupName = new std::wstring;
			*m_pstrSceneChangeFishGroupName = l_strValue;
		}
	PARSE_NAME_VALUE_END
}
cBaseScene::~cBaseScene()
{
}

bool	cBaseScene::FishGroupFinishEvent(void*e_pFishGroupName)
{
	//assert(eSS_LOOP == m_eCurrentSceneStatus&&"FishGroupFinishEvent status is wrong!");
	if(m_eCurrentSceneStatus == eSS_LOOP)
		m_eCurrentSceneStatus = eSS_END;
	return true;
}

void	cBaseScene::Init()
{
	bool l_bFirstHit = false;
	for (int i = eSS_START; i < eSS_MAX; ++i)
	{
		if (m_pBGMPDI[i])
		{
			m_pBGMPDI[i]->Init();
		}
	}
	for (int i = eSS_START; i < eSS_MAX; ++i)
	{
		 if (m_pBGMPDI[i])
		 {
			 m_eCurrentSceneStatus = (eSceneStatus)i;
			 break;
		 }
	}
	//assert(!m_pBGMPDI[eSS_START]->IsAnimationLoop());
	//assert(m_pBGMPDI[eSS_LOOP]->IsAnimationLoop());
	//assert(!m_pBGMPDI[eSS_END]->IsAnimationLoop());
	if (m_pToNextSceneTC)
		m_pToNextSceneTC->Start();
	m_bSatisfiedCondition = false;
}

void	cBaseScene::ToNextScene()
{
	m_bSatisfiedCondition = true;
	cGameApp::EventMessageShot(eFGEMI_SCENE_CHANGE, nullptr);
#ifdef DEBUG
	//auto l_strNextPhaseName = this->GetNextPhaseName();
	//cPhaseManager*l_pPhaseManager = dynamic_cast<cPhaseManager*>(this->GetOwner());
	//assert(l_pPhaseManager->GetObject(l_strNextPhaseName));
#endif
}

void	cBaseScene::UpdateStartStatus(float e_fElpaseTime)
{
	if (m_pBGMPDI[m_eCurrentSceneStatus])
	{
		if (m_pBGMPDI[m_eCurrentSceneStatus]->IsAnimationDone())
		{
			m_eCurrentSceneStatus = eSS_LOOP;
		}
	}
	else
		m_eCurrentSceneStatus = eSS_LOOP;
	if (m_pstrSceneChangeFishGroupName && m_eCurrentSceneStatus == eSS_LOOP)
	{
		cGameApp::EventMessageShot(eFGEMI_SCENE_CHANGE, m_pstrSceneChangeFishGroupName);
	}
}		
		
void	cBaseScene::UpdateLoopStatus(float e_fElpaseTime)
{
	if (m_pToNextSceneTC)
	{
		m_pToNextSceneTC->Update(e_fElpaseTime);
		if (m_pToNextSceneTC->bTragetTimrReached)
		{
			m_eCurrentSceneStatus = eSS_END;
		}
	}
	else
	if(!m_pstrSceneChangeFishGroupName)
	{
		//it should't never happen
		//it should wait FishGroupFinishEvent to set this.
		m_eCurrentSceneStatus = eSS_END;
	}
}
		
void	cBaseScene::UpdateEndStatus(float e_fElpaseTime)
{
	if(m_pBGMPDI[m_eCurrentSceneStatus])
	{
		if (m_pBGMPDI[m_eCurrentSceneStatus]->IsAnimationDone())
		{
			ToNextScene();
		}
	}
	else
	{
		ToNextScene();
	}
}

void	cBaseScene::Update(float e_fElpaseTime)
{
	if (m_eCurrentSceneStatus == eSS_START)
		UpdateStartStatus(e_fElpaseTime);
	else
	if (m_eCurrentSceneStatus == eSS_LOOP)
		UpdateLoopStatus(e_fElpaseTime);
	else
	if (m_eCurrentSceneStatus == eSS_END)
		UpdateEndStatus(e_fElpaseTime);

	if (m_pBGMPDI[m_eCurrentSceneStatus])
	{
		m_pBGMPDI[m_eCurrentSceneStatus]->Update(e_fElpaseTime);
	}

}

void	cBaseScene::Render()
{

}
//
//cBossScene::cBossScene(TiXmlElement*e_pTiXmlElement):cBaseScene(e_pTiXmlElement)
//{
//	
//}
//
//cBossScene::~cBossScene()
//{
//
//}
//
//void	cBossScene::Init()
//{
//
//}
//
//void	cBossScene::Update(float e_fElpaseTime)
//{
//
//}
//
//void	cBossScene::Render()
//{
//
//}