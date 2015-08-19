#include "stdafx.h"
#include "FishingPhase.h"
#include "StageData.h"
#include "GameDefine.h"
#include "GameApp.h"

cFishingPhase::cFishingPhase()
{
	m_pStageData = 0;
	SetName(FISHING_PHASE);
}

cFishingPhase::~cFishingPhase()
{
	SAFE_DELETE(m_pStageData);
}

void	cFishingPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{

}

void	cFishingPhase::Init()
{
	Destroy();
	cGameApp::m_sbDoMultiTouch = true;
	if( !UT::IsFileExists(UT::WcharToChar(m_strStageName).c_str()) )
	{
		m_strStageName = L"Data/StageData/Stage1.xml";
	}
	m_pStageData = new cStageData(cFishingFunApp::m_spFishRodAndFishThreadData->m_pCurrentSelectedFishRodData,cFishingFunApp::m_spFishRodAndFishThreadData->m_pCurrentSelectedcFishThread);
	m_pStageData->Init(UT::WcharToChar(m_strStageName).c_str());
}
//it would be called if the stage is changed,set m_bSatisfiedCondition as false!?
void	cFishingPhase::Update(float e_fElpaseTime)
{
	if( m_pStageData )
	{
		m_pStageData->Update(e_fElpaseTime);
		if( m_pStageData->IsStageClear() )
		{
			this->m_bSatisfiedCondition = true;
			m_strNextPhaseName = SHOW_FISHING_RESULT_PHASE;
		}
	}
}

void	cFishingPhase::Render()
{
	if( m_pStageData )
		m_pStageData->Render();
}

void	cFishingPhase::Destroy()
{
	SAFE_DELETE(m_pStageData);
	cGameApp::m_sbDoMultiTouch = false;
}

void	cFishingPhase::DebugRender()
{
	if( m_pStageData )
		m_pStageData->DebugRender();
}

void	cFishingPhase::KeyUp(char e_cKey)
{

}

void	cFishingPhase::KeyDown(char e_cKey)
{

}

void	cFishingPhase::KeyPress(char e_cKey)
{

}

void*	cFishingPhase::GetData()
{
	return (void*)this->m_pStageData->GetBeenHookedFishes();
}

void	cFishingPhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	//cSelectFishRodFishThreadPhase*l_pSelectFishRodFishThreadPhase = (cSelectFishRodFishThreadPhase*)e_pData;
	//m_pSelectFishThread = l_pSelectFishRodFishThreadPhase->m_pSelectFishThread;
	//m_pSelectFishRodData = l_pSelectFishRodFishThreadPhase->m_pSelectFishRodData;
	const WCHAR*l_strStageName = (const WCHAR*)e_pData;
	m_strStageName = L"Data/StageData/";
	m_strStageName += l_strStageName;
	m_strStageName =+ L".xml";
}

void    cFishingPhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pStageData )
		m_pStageData->MouseDown(e_iPosX,e_iPosY);
}

void    cFishingPhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pStageData )
		m_pStageData->MouseMove(e_iPosX,e_iPosY);
}

void    cFishingPhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pStageData )
		m_pStageData->MouseUp(e_iPosX,e_iPosY);
}