#include "stdafx.h"
#include "GameObjectSelector.h"
#include "SelectFishRodFishThreadPhase.h"
#include "GameDefine.h"
#include "StageSelectPhase.h"
#include "GameApp.h"

cSelectFishRodFishThreadPhase::cSelectFishRodFishThreadPhase()
{
	SetName(SELECT_FISH_ROD_FISH_THREAD_PHASE);
	m_pUIBGMPDI = 0;
	m_pFishRodGameObjectSelector = 0;
	m_pFishThreadGameObjectSelector = 0;
	m_pYesNoButton = 0;
	m_SelectableFishThread.SetFromResource(true);
}

cSelectFishRodFishThreadPhase::~cSelectFishRodFishThreadPhase()
{
	Destroy();
}

void	cSelectFishRodFishThreadPhase::Destroy()
{
	SAFE_DELETE(m_pYesNoButton);
	SAFE_DELETE(m_pFishRodGameObjectSelector);
	SAFE_DELETE(m_pFishThreadGameObjectSelector);
	m_pUIBGMPDI = 0;
}

//<StageSelectPhase>
//	<Camera ViewRect="" StartPos="" >
//	<cButtonDialog Description="" YesButton="IAmYes" NoButton="IAmNo" BG="BGG" StripButtonForBG="1">
//		<cMPDI MPDIList="" MPDI=""/>
//		<cImageButton ....>
//	</cButtonDialog>
	//<Stage Name="1" FishThread="1" />
	//<Stage Name="2" FishThread="2" />
//</StageSelectPhase>

//<StageSelectPhase>
//</StageSelectPhase>

void	cSelectFishRodFishThreadPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("UIBG")
	{
		m_pUIBGMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if( m_pUIBGMPDI )
			m_pUIBGMPDI->Init();
	}
	else
	COMPARE_VALUE_WITH_DEFINE(cGameObjectSelector::TypeID)
	{
		cGameObjectSelector*l_pGameObjectSelector = new cGameObjectSelector(e_pTiXmlElement);
		if(!wcscmp(l_pGameObjectSelector->GetName(),L"FishRod"))
		{
			m_pFishRodGameObjectSelector = l_pGameObjectSelector;
			//SAFE_DELETE(l_pGameObjectSelector);
			l_pGameObjectSelector->SetGameItemInfoReferenceData((cNamedTypedObjectVector<NamedTypedObject>*)(&cFishingFunApp::m_spPlayerData->m_FishRodDataVector),0);
		}
		else
		if(!wcscmp(l_pGameObjectSelector->GetName(),L"FishThread"))
		{
			m_pFishThreadGameObjectSelector = l_pGameObjectSelector;
			l_pGameObjectSelector->SetGameItemInfoReferenceData(&m_SelectableFishThread,1);
		}
		else
		{
			UT::ErrorMsg(cGameObjectSelector::TypeID,L"Name not match");
		}
	}
	else
	COMPARE_VALUE("YesNo")
	{
		m_pYesNoButton = cButtonDialog::GetMe(e_pTiXmlElement->FirstChildElement());
		if( m_pYesNoButton )
			m_pYesNoButton->Init();
	}
}

void	cSelectFishRodFishThreadPhase::Init()
{
	Destroy();
	PARSE_FAILED_MESSAGE_BOX(this,"Data/Phase/SelectFishRodFishThreadPhase.xml");
}
//it would be called if the stage is changed,set m_bSatisfiedCondition as false!?
void	cSelectFishRodFishThreadPhase::Update(float e_fElpaseTime)
{
	if( m_pUIBGMPDI )
		m_pUIBGMPDI->Update(e_fElpaseTime);
	if( m_pFishRodGameObjectSelector )
	{
		m_pFishRodGameObjectSelector->Update(e_fElpaseTime);
	}
	if( m_pFishThreadGameObjectSelector )
	{
		m_pFishThreadGameObjectSelector->Update(e_fElpaseTime);
	}
	if( m_pYesNoButton )
	{
		m_pYesNoButton->Update(e_fElpaseTime);
		if( m_pYesNoButton->IsSatisfiedCondition() )
		{
			if( m_pYesNoButton->GetYesImageButton()->IsSatisfiedCondition() )
			{
				const WCHAR*l_strFishThreadName = m_pFishThreadGameObjectSelector->GetSelectItemName();
				const WCHAR*l_strFishRodName = m_pFishRodGameObjectSelector->GetSelectItemName();
				if( l_strFishRodName )
					cFishingFunApp::m_spFishRodAndFishThreadData->m_pCurrentSelectedFishRodData = cFishingFunApp::m_spFishRodAndFishThreadData->m_FishRodDataVector.GetObject(l_strFishRodName);
				if( l_strFishThreadName )
					cFishingFunApp::m_spFishRodAndFishThreadData->m_pCurrentSelectedcFishThread = cFishingFunApp::m_spFishRodAndFishThreadData->m_FishThreadVector.GetObject(l_strFishThreadName);
				m_strNextPhaseName = FISHING_PHASE;
			}
			else
			{
				m_strNextPhaseName = STAGE_SELECT_PHASE;
			}
			this->m_bSatisfiedCondition = true;
		}
	}
}

void	cSelectFishRodFishThreadPhase::Render()
{
	if( m_pUIBGMPDI )
		m_pUIBGMPDI->Render();
	if( m_pFishRodGameObjectSelector )
	{
		m_pFishRodGameObjectSelector->Render();
	}
	if( m_pFishThreadGameObjectSelector )
	{
		m_pFishThreadGameObjectSelector->Render();
	}
	if( m_pYesNoButton )
	{
		m_pYesNoButton->Render();
	}
}

void	cSelectFishRodFishThreadPhase::DebugRender()
{

}

void	cSelectFishRodFishThreadPhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if(m_pFishRodGameObjectSelector)
		m_pFishRodGameObjectSelector->MouseDown(e_iPosX,e_iPosY);
	if(m_pFishThreadGameObjectSelector)
		m_pFishThreadGameObjectSelector->MouseDown(e_iPosX,e_iPosY);
	if( m_pYesNoButton )
		m_pYesNoButton->MouseDown(e_iPosX,e_iPosY);
}

void	cSelectFishRodFishThreadPhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if(m_pFishRodGameObjectSelector)
		m_pFishRodGameObjectSelector->MouseMove(e_iPosX,e_iPosY);
	if(m_pFishThreadGameObjectSelector)
		m_pFishThreadGameObjectSelector->MouseMove(e_iPosX,e_iPosY);
	if( m_pYesNoButton )
		m_pYesNoButton->MouseMove(e_iPosX,e_iPosY);
}

void	cSelectFishRodFishThreadPhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if(m_pFishRodGameObjectSelector)
		m_pFishRodGameObjectSelector->MouseUp(e_iPosX,e_iPosY);
	if(m_pFishThreadGameObjectSelector)
		m_pFishThreadGameObjectSelector->MouseUp(e_iPosX,e_iPosY);
	if( m_pYesNoButton )
		m_pYesNoButton->MouseUp(e_iPosX,e_iPosY);
}

void*	cSelectFishRodFishThreadPhase::GetData()
{
	return (void*)m_strStageName.c_str();
	//return this;
}

void	cSelectFishRodFishThreadPhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	cStageSelectPhase*l_pStageSelectPhase = (cStageSelectPhase*)e_pData;
	m_strStageName = l_pStageSelectPhase->m_strStageName;
	size_t	l_siSize = l_pStageSelectPhase->m_FishThreadsNameVector.size();
	for( size_t i=0;i<l_siSize;++i )
	{
		cFishThread*l_pFishThread = cFishingFunApp::m_spFishRodAndFishThreadData->m_FishThreadVector.GetObject(l_pStageSelectPhase->m_FishThreadsNameVector[i]);
		if( l_pFishThread )
			m_SelectableFishThread.AddObjectNeglectExist(l_pFishThread);
		else
		{
			UT::ErrorMsg(l_pStageSelectPhase->m_FishThreadsNameVector[i].c_str(),L"FishRod is not exist!");
		}
	}
}