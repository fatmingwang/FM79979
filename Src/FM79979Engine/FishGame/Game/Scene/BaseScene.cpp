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
}
cBaseScene::~cBaseScene()
{
}

bool	cBaseScene::FishGroupFinishEvent(void*e_pFishGroupName)
{
	assert(eSS_LOOP == m_eCurrentSceneStatus&&"FishGroupFinishEvent status is wrong!");
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