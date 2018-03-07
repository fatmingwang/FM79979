#include "stdafx.h"
#include "BaseScene.h"
#include "../GameDefine/GameParameterDefine.h"
#include "../GameDefine/FishGameEventMessageID.h"
TYPDE_DEFINE_MARCO(cBaseScene);

//<cBaseScene SubSceneIndex = "0" SceneChangeFishGroupName = "SceneChangeFishGroup1">
//	<StartMPDI>        <cMPDI cMPDIList = "FishGame/Image/Turn/Turn.mpdi" cMPDI = "Start" />      </StartMPDI>
//	<LoopMPDI>        <cMPDI cMPDIList = "FishGame/Image/Turn/Turn.mpdi" cMPDI = "Loop" />      </LoopMPDI>
//	<EndMPDI>        <cMPDI cMPDIList = "FishGame/Image/Turn/Turn.mpdi" cMPDI = "End" />      </EndMPDI>
//</cBaseScene>
//<cBaseScene SubSceneIndex = "1" Time = "300" GenerateFishAfterStartMPDI = "">
//	<StartMPDI>        <cMPDI cMPDIList = "FishGame/Image/Turn/Turn.mpdi" cMPDI = "Start" />      </StartMPDI>
//	<LoopMPDI>        <cMPDI cMPDIList = "FishGame/Image/Turn/Turn.mpdi" cMPDI = "Loop" />      </LoopMPDI>
//	<EndMPDI>        <cMPDI cMPDIList = "FishGame/Image/Turn/Turn.mpdi" cMPDI = "End" />      </EndMPDI>
//</cBaseScene>

cBaseScene::cBaseScene(TiXmlElement*e_pTiXmlElement)
{
	for (int i = 0; i < eSS_MAX; ++i)
		m_pBGMPDI[i] = nullptr;
	m_pstrSceneChangeFishGroupName = nullptr;
	m_pToNextSceneTC = nullptr;

	if(m_pstrSceneChangeFishGroupName)
		REG_EVENT(eFGEMI_FISH_GROUP_FINISH, &cBaseScene::FishGroupFinishEvent);
	cMPDIList*l_pMPDIList = nullptr;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("MPDIFileName")
		{
			l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue,true);
		}
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
		else//do nothing just for human vision
		COMPARE_NAME("SubSceneIndex")
		{
		int a = 0;
		}
	
	PARSE_NAME_VALUE_END
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pTiXmlElement)
		auto l_strStatusMPDI = e_pTiXmlElement->Value();
		eSceneStatus l_eSceneStatus = eSS_MAX;
		if(!wcscmp(l_strStatusMPDI,L"StartMPDI")){l_eSceneStatus = eSS_START;}else
		if(!wcscmp(l_strStatusMPDI, L"LoopMPDI")){l_eSceneStatus = eSS_LOOP;}else
		if(!wcscmp(l_strStatusMPDI, L"EndMPDI")) {l_eSceneStatus = eSS_END;}
		if(l_eSceneStatus != eSS_MAX)
		{
			m_pBGMPDI[l_eSceneStatus] = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement());
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pTiXmlElement)
}

cBaseScene::~cBaseScene()
{
	for (int i = 0; i < eSS_MAX; ++i)
	{
		SAFE_DELETE(m_pBGMPDI[i]);
	}
	SAFE_DELETE(m_pToNextSceneTC);
	SAFE_DELETE(m_pstrSceneChangeFishGroupName);
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
	if (m_pBGMPDI[m_eCurrentSceneStatus])
	{
		m_pBGMPDI[m_eCurrentSceneStatus]->Render();
	}
}

void	cBaseScene::DebugRender()
{
	std::wstring l_str;
	if(m_eCurrentSceneStatus == eSS_START)
	{ 
		l_str = L"eSS_START\n";
	}
	else
	if(m_eCurrentSceneStatus == eSS_LOOP)
	{
		l_str = L"eSS_LOOP\n";
	}
	else
	if(m_eCurrentSceneStatus == eSS_END)
	{
		l_str = L"eSS_STAReSS_END\n";
	}
	if (m_pstrSceneChangeFishGroupName)
	{
		l_str += *m_pstrSceneChangeFishGroupName;
		l_str += L"\n";
	}
	if (m_pToNextSceneTC)
	{
		l_str += ValueToStringW(m_pToNextSceneTC->fRestTime);
	}
	cGameApp::RenderFont(SCENE_MANAGER_DEBUG_RENDER_POS, l_str.c_str());
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