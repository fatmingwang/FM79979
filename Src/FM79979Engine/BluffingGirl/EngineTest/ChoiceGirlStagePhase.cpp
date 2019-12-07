#include "stdafx.h"
#include "ChoiceGirlStagePhase.h"
#include "BluffingGirlApp.h"
#include "PlayerData.h"
#include "StageClearFile.h"
#include "ChoiceGirlPhase.h"

namespace FATMING_CORE
{
	extern bool    g_bSkipImageLoad;
}

cChoiceGirlStagePhase::cChoiceGirlStagePhase()
{
	this->SetName(CHOICE_GIRL_STAGE_PHASE_NAME);
	m_pBGMPDI = 0;
	m_pGirlImageCover = 0;
	m_pSelectedStageData = 0;
	m_pIntoGamePhasePopUpMessager = 0;
	m_iLatestGameCouldBePlay = -1;
	m_eSelectStageTYpe = eSST_SELECT_STAGE;
	Vector4	l_vCollideRange(0,0,cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	m_pScroller = 0;
	m_pChoiceGirlStagePI = 0;
	m_strCurrentSelectedGirlNameAndStageName = L"";
	//m_pBackButton = new cImageButton(l_pCommonMPDIList->GetObject(L"Back"));
	m_pPlayGameButton = 0;
	m_pWatchVideoButton = 0;
	m_bCurrentVideoCouldBeWatch = false;
}

cChoiceGirlStagePhase::~cChoiceGirlStagePhase()
{
	Destroy();
	SAFE_DELETE(m_pSelectedStageData);
	SAFE_DELETE(m_pIntoGamePhasePopUpMessager);
	SAFE_DELETE(m_pPlayGameButton);
	SAFE_DELETE(m_pWatchVideoButton);
}

void	cChoiceGirlStagePhase::Destroy()
{
	m_pBGMPDI = 0;
	m_MainfunctionButton.Destroy();
	m_SelectedGirlImage.Destroy();
	m_StageDataVector.Destroy();
	SAFE_DELETE(m_pIntoGamePhasePopUpMessager);
	SAFE_DELETE(m_pPlayGameButton);
	SAFE_DELETE(m_pWatchVideoButton);
	m_GirlUnlokImageVector.Destroy();
	SAFE_DELETE(m_pScroller);
	m_ResourceStamp.ReleaseNewResource();
	m_pChoiceGirlStagePI = 0;
}

//	<StageData Name="Stage2" Image="Stage2" Video="Stage2" GameSetupFileName="Stage2"/>
void	cChoiceGirlStagePhase::ProcessStageData(TiXmlElement*e_pTiXmlElement)
{
	cStageData*l_pStageData = new cStageData();
	m_StageDataVector.AddObjectNeglectExist(l_pStageData);
	std::wstring	l_strStageName;
	cPuzzleImageUnit*l_pPIUnit = 0;
	cPuzzleImageUnit*l_pPIUnitClone = 0;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("ShowImage")
		{
			if( m_pChoiceGirlStagePI )
			{
				l_pPIUnit = m_pChoiceGirlStagePI->GetObject(l_strValue);
				if( l_pPIUnit )
					l_pPIUnitClone = new cPuzzleImageUnit(l_pPIUnit);
				else
				{
					UT::ErrorMsg(L"l_strValue",L"is not exists at pi");
				}
			}
		}
		else
		COMPARE_NAME("BGImage")
		{
			l_pStageData->m_strBGImage = l_strValue;
		}
		else
		COMPARE_NAME("Name")
		{
			l_pStageData->m_strStageMame = l_strValue;
		}
		else
		COMPARE_NAME("Video")
		{
			l_pStageData->m_strVideoName = l_strValue;
		}
		else
		COMPARE_NAME("GameSetupFileName")
		{
			l_pStageData->m_strGameSetupFileName = l_strValue;
		}
		else
		COMPARE_NAME("GirlIntroducingVideoFileName")
		{
			l_pStageData->m_strGirlIntroducingVideoFileName = l_strValue;
		}
		else
		COMPARE_NAME("WinVideo")
		{
			l_pStageData->m_strWinVideoFileName = l_strValue;
		}
		else
		COMPARE_NAME("LoseVideo")
		{
			l_pStageData->m_strLoseVideoFileName = l_strValue;
		}
		else
		COMPARE_NAME("TotalRound")
		{
			l_pStageData->m_iTotalRound = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("WinCount")
		{
			l_pStageData->m_iWinCount = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		l_strValue = e_pTiXmlElement->Value();
		if( !wcscmp(l_strValue,L"ShakeVideo") )
		{
			if( e_pTiXmlElement->FirstAttribute() )
			{
				std::wstring	l_strShakeVideo = e_pTiXmlElement->FirstAttribute()->Value();
				l_pStageData->m_strShakeDiceVideoFileNameVector.push_back(l_strShakeVideo);
			}
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
	if( l_pPIUnitClone )
	{
		l_pPIUnitClone->SetName(l_pStageData->m_strStageMame.c_str());
		m_pScroller->AddObject(dynamic_cast<cBaseImage*>(l_pPIUnitClone),true);
	}
	else
	{//test code
		//cTextButton*l_pTextButton = new cTextButton(Vector2(l_vPos.x,l_vPos.y),cGameApp::m_spGlyphFontRender,Vector4(1,0,0,1),L"Test11111");
		//l_pTextButton->SetLocalPosition(l_vPos);
		//m_pScroller->AddData(l_pTextButton);
	}
}
//
//<SelectedGirlImageSubMPDI>
//	<cSubMPDI cMPDIList="Image/Mm001_Stage_Select.mpdi" cMPDI="Stage001_Select" cSubMPDI="Mm001_Stage_Film001"/>
//	<cPuzzleImageUnit PI="Image/Mm001_StageFilm" PIUnit="Mm001_Stage_Film001">
//	<cPuzzleImageUnit PI="Image/Mm001_StageFilm" PIUnit="Mm001_Stage_Film002">
//	<cPuzzleImageUnit PI="Image/Mm001_StageFilm" PIUnit="Mm001_Stage_Film003">
//	<cPuzzleImageUnit PI="Image/Mm001_StageFilm" PIUnit="Mm001_Stage_Film004">
//	<cPuzzleImageUnit PI="Image/Mm001_StageFilm" PIUnit="Mm001_Stage_Film005">
//<SelectedGirlImageSubMPDI>
void	cChoiceGirlStagePhase::ProcessSelectedGirlImageSubMPDIData(TiXmlElement*e_pTiXmlElement)
{
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	cSubMPDI*l_pSubMPDI = 0;
	while( e_pTiXmlElement )
	{
		const WCHAR*l_strValue = e_pTiXmlElement->Value();
		COMPARE_VALUE("cSubMPDI")
		{
			g_bSkipImageLoad = true;
			l_pSubMPDI = cSubMPDI::GetMe(e_pTiXmlElement,false);
			g_bSkipImageLoad = false;
		}
		else
		COMPARE_VALUE("cPuzzleImageUnit")
		{
			cPuzzleImageUnit*l_pPIUnit = cPuzzleImageUnit::GetMe(e_pTiXmlElement,false);
			cPuzzleImage*l_pPI = dynamic_cast<cPuzzleImage*>(l_pPIUnit->GetOwner());
			int	l_iIndex = l_pPI->GetObjectIndexByPointer(l_pPIUnit);
			cSubMPDI*l_pSubMPDIClone = new cSubMPDI(l_pSubMPDI);
			l_pSubMPDIClone->Init();
			l_pSubMPDIClone->ChangeAllImageByImageIndex(l_iIndex);
			for(size_t i=0;i<l_pSubMPDIClone->GetPointDataList()->size();++i)
			{
				l_pSubMPDIClone->GetPointData(i)->pPI = l_pPI;
			}
			cImageButton*l_pImageButton = new cImageButton(l_pSubMPDIClone);
			SAFE_DELETE(l_pSubMPDIClone);
			m_SelectedGirlImage.AddObjectNeglectExist(l_pImageButton);
		}
		else
		COMPARE_VALUE("GirlImageCover")
		{
			g_bSkipImageLoad = true;
			m_pGirlImageCover = cSubMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
			m_pGirlImageCover->Init();
			g_bSkipImageLoad = false;
		}

		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
}
//<ChoiceGirlStagePhase PI="Image/Girl/ChoiceGirl.pi">
//	<StageData Name="Stage1" Image="Stage1" Video="Stage1" GameSetupFileName="Stage1"/>
//	<StageData Name="Stage2" Image="Stage2" Video="Stage2" GameSetupFileName="Stage2"/>
//	<StageData Name="Stage3" Image="Stage3" Video="Stage3" GameSetupFileName="Stage3"/>
//</ChoiceGirlStagePhase>
void	cChoiceGirlStagePhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("GirlScroller")
	{
		m_pScroller = cTabPage::GetMe(e_pTiXmlElement->FirstChildElement());
		if( m_pScroller )
		{
			m_pScroller->SetOutOfWorking(false);
			m_pScroller->SetReclickable(true);
			m_pScroller->SetReleaseCurrentWorkingObjectWhileReclickable(false);
			m_pScroller->Init();
		}
	}
	else
	COMPARE_VALUE("UnlockGirlHintImage")
	{
		//cRenderObject*l_pImage = GetRenderObjectBehavior(e_pTiXmlElement->FirstChildElement());
		//for( int i=1;i<m_pScroller->Count();++i )
		//{
		//	std::wstring	l_strGirlStageName = m_strCurrentSelectedGirlName;
		//	l_strGirlStageName += m_pScroller->GetObject(i-1)->GetName();
		//	if(  !cStageClearFile::IsStageClear(l_strGirlStageName.c_str()) )
		//	{
		//		cRenderObject*l_pClone = dynamic_cast<cRenderObject*>(l_pImage->Clone());
		//		l_pClone->SetParent(m_pScroller->GetObject(i));
		//		m_GirlUnlokImageVector.AddObjectNeglectExist(l_pClone);
		//	}
		//}
		//SAFE_DELETE(l_pImage);
	}
	else
	COMPARE_VALUE("StageData")
	{
		ProcessStageData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("SelectedGirlImageSubMPDI")
	{
		ProcessSelectedGirlImageSubMPDIData(e_pTiXmlElement);
	}
}

void	cChoiceGirlStagePhase::ParseGameVideoButtonData(TiXmlElement*e_pElement)
{
	TiXmlElement*l_pButtonElement = e_pElement->FirstChildElement();
	while( l_pButtonElement )
	{
		cImageButton*l_pImageButton = cImageButton::GetMe(l_pButtonElement);
		if( l_pImageButton )
		{
			if(!wcscmp(l_pImageButton->GetName(),L"Game"))
			{
				assert( m_pPlayGameButton == 0 );
				m_pPlayGameButton = l_pImageButton;
				m_pPlayGameButton->Init();
				m_pPlayGameButton->SetEnable(false);
			}
			else
			if(!wcscmp(l_pImageButton->GetName(),L"Video"))
			{
				assert( m_pWatchVideoButton == 0 );
				m_pWatchVideoButton	= l_pImageButton;
				m_pWatchVideoButton->Init();
				m_pWatchVideoButton->SetEnable(false);
			}
		}
		l_pButtonElement = l_pButtonElement->NextSiblingElement();
	}
}
//<cChoiceGirlStagePhase>
//	<ButtonList>
//		<cImageButton Name="Game" MPDIList="Image/Girl/GirlStageCommon.mpdi" MPDI="GameOrVideo" SubMPDI="Game" />
//		<cImageButton Name="Game" MPDIList="Image/Girl/GirlStageCommon.mpdi" MPDI="GameOrVideo" SubMPDI="Video" />
//	</ButtonList>
//	<cPhasePopUpMessager Name="UseClairvoyant" RenderLastPhase="1" UpdateLastPhase="0">
//		<cButtonDialog Description="Test">
//			<cMPDI cMPDIList="Image/Common.mpdi" cMPDI="UseClairvoyant"/>
//		</cButtonDialog>
//		<Messenger ButtonName="Yes" NewPhaseName="Yes" InitNewPhase="1" DestroyOldPhase="0"/>
//		<Messenger ButtonName="No" NewPhaseName="No" InitNewPhase="0" DestroyOldPhase="0"/>
//	</cPhasePopUpMessager>
//</cChoiceGirlStagePhase>
void	cChoiceGirlStagePhase::ParseGirlStagePhseData(const char*e_strFileName)
{
	cNodeISAX	l_cNodeISAX;
	if(l_cNodeISAX.ParseDataIntoXMLNode(e_strFileName))
	{
		TiXmlElement*l_pElement = l_cNodeISAX.GetRootElement();
		l_pElement = l_pElement->FirstChildElement();
		while( l_pElement )
		{
			const WCHAR*l_strValue = l_pElement->Value();
			COMPARE_VALUE("ButtonList")
			{
				ParseGameVideoButtonData(l_pElement);
			}
			else
			COMPARE_VALUE("cPhasePopUpMessager")
			{
				m_pIntoGamePhasePopUpMessager = new cPhasePopUpMessager(l_pElement);
				m_pIntoGamePhasePopUpMessager->Init();
			}
			else
			COMPARE_VALUE("Button")
			{
				m_MainfunctionButton.AddButton(l_pElement->FirstChildElement());
			}
			else
			COMPARE_VALUE("BG")
			{
				m_pBGMPDI = cMPDI::GetMe(l_pElement->FirstChildElement(),false);
				if( m_pBGMPDI )
					m_pBGMPDI->Init();
			}
			l_pElement = l_pElement->NextSiblingElement();
		}
	}

}

void	cChoiceGirlStagePhase::Init()
{
	this->Destroy();
	SAFE_DELETE(m_pSelectedStageData);
	m_eSelectStageTYpe = eSST_SELECT_STAGE;
	m_bSatisfiedCondition = false;
	m_bCurrentVideoCouldBeWatch = false;
	m_ResourceStamp.StampResource1();
	//
	if(!this->Parse(m_strFileName.c_str(),false))
	{
		UT::ErrorMsg(m_strFileName.c_str(),"parse failed!!");
	}
	m_MainfunctionButton.Init();
	//
	ParseGirlStagePhseData("BluffingGirl/PhaseData/GirlStage/GirlStageChoicePhase.xml");
	//
	m_strNextPhaseName = L"";
	m_iLatestGameCouldBePlay = -1;
	int	l_iUnClearIndex = 0;
	for( int i=0;i<m_pScroller->Count();++i )
	{
		std::wstring	l_strTemp = m_strCurrentSelectedGirlName;
		l_strTemp += m_pScroller->GetObject(i)->GetName();
		if(!cStageClearFile::IsStageClear(l_strTemp.c_str()))
		{
			if( m_iLatestGameCouldBePlay == -1 )
				m_iLatestGameCouldBePlay = i;
			else
			{
				cImageButton*l_pImageButton = dynamic_cast<cImageButton*>(this->m_pScroller->GetObject(i));
				l_pImageButton->SetEnable(false);

			}
			continue;
		}
	}
	//all stage clear
	if( m_iLatestGameCouldBePlay == -1 )
	{
		m_iLatestGameCouldBePlay = m_pScroller->Count()-1;
		//UT::ErrorMsg(L"game stage data error has same name!",L"Error");
	}
	m_MainfunctionButton.Init();
	m_SelectedGirlImage.Init();
	if( m_pScroller->Count() != m_StageDataVector.Count() || m_SelectedGirlImage.Count() != m_pScroller->Count() )
	{
		UT::ErrorMsg(L"girl select button and data count is not match!",L"Warning");
	}
	m_ResourceStamp.StampResource2();
}

void	cChoiceGirlStagePhase::UpdateIntoGame(float e_fElpaseTime)
{
	if( m_eSelectStageTYpe != eSST_INTO_GAME )
		return;
	if( m_pIntoGamePhasePopUpMessager )
	{
		m_pIntoGamePhasePopUpMessager->Update(e_fElpaseTime);
		if( m_pIntoGamePhasePopUpMessager->IsSatisfiedCondition() )
		{
			cBluffingGirlApp::m_spPlayerData->Init();
			if(m_pIntoGamePhasePopUpMessager->GetCurrentWorkingObjectIndex() == 0)//yes
			{
				if(cBluffingGirlApp::m_spPlayerData->HeartMinus())
				{
					cGameApp::SoundPlay(BUTTON_CLICK_SOUND,true);
					m_strNextPhaseName = GIRL_INTRODUCING_PHASE_NAME;
					this->m_bSatisfiedCondition = true;
					int	l_iSelectedIndex = m_pScroller->GetCurrentWorkingObjectIndex();
					if( l_iSelectedIndex != -1 )
					{
						m_pSelectedStageData = new cStageData();
						m_StageDataVector[m_pScroller->GetCurrentWorkingObjectIndex()]->Assign(m_pSelectedStageData);
	
						if( l_iSelectedIndex == m_pScroller->Count()-1 )
						{
							m_pSelectedStageData->m_bLastStage = true;
						}
						else
							m_pSelectedStageData->m_bLastStage = false;
					}
				}
				else
				{
					cBluffingGirlApp::WakePopupSystemMessenger(L"HeartNotEnough",this->GetName());
					m_pPlayGameButton->SetSatisfiedCondition(false);
					//m_pScroller->GetCurrentWorkingObject()->Init();
					m_pPlayGameButton->Init();
					m_eSelectStageTYpe = eSST_SELECT_VIDEO_OR_GAME;
					cGameApp::SoundPlay(BUTTON_CLICK_WRONG_SOUND,true);
					//show heart not enough.
				}
			}
			else//1 no
			{
				m_eSelectStageTYpe = eSST_SELECT_VIDEO_OR_GAME;
			}
			m_pIntoGamePhasePopUpMessager->Init();
		}
	}
	else
	{
		this->m_bSatisfiedCondition = true;
	}
}

void	cChoiceGirlStagePhase::UpdateSelectStage(float e_fElpaseTime)
{
	if( m_eSelectStageTYpe <= eSST_SELECT_VIDEO_OR_GAME )
	{
		if( m_pScroller )
		{
			m_pScroller->Update(e_fElpaseTime);
			if( m_pScroller->IsSatisfiedCondition() || m_pScroller->IsChancelButtonSatisfied() /*&& m_pScroller->GetCurrentWorkingObjectIndex() != -1*/ )
			{
				m_strCurrentSelectedGirlNameAndStageName = L"";
				if(m_pScroller->IsChancelButtonSatisfied())
				{
					cGameApp::SoundPlay(BUTTON_CLICK_SOUND,true);
					m_strNextPhaseName = CHOICE_GIRL_PHASE_NAME;
					this->m_bSatisfiedCondition = true;
				}
				else
				{
					m_strCurrentSelectedGirlNameAndStageName = m_strCurrentSelectedGirlName;
					if( m_pScroller->GetCurrentWorkingObjectIndex() == -1 )
					{
						m_pScroller->Init();
						m_strCurrentSelectedGirlNameAndStageName += L"";
					}
					else
						m_strCurrentSelectedGirlNameAndStageName += m_pScroller->GetCurrentWorkingObjectName();
					m_bCurrentVideoCouldBeWatch = cStageClearFile::IsStageClear(m_strCurrentSelectedGirlNameAndStageName.c_str());
					m_pWatchVideoButton->SetEnable(m_bCurrentVideoCouldBeWatch);
					m_eSelectStageTYpe = eSST_SELECT_VIDEO_OR_GAME;
					int	l_iSelectedIndex = m_pScroller->GetCurrentWorkingObjectIndex();
					if( (m_iLatestGameCouldBePlay != -1) &&( l_iSelectedIndex == -1 ||m_iLatestGameCouldBePlay < l_iSelectedIndex ))
						m_bCurrentGameCouldBePlay = false;
					else
						m_bCurrentGameCouldBePlay = true;
					m_pPlayGameButton->SetEnable(m_bCurrentGameCouldBePlay);
				}
				if( m_pScroller )
					m_pScroller->SetSatisfiedCondition(false);
			}
			if( m_pWatchVideoButton && m_bCurrentVideoCouldBeWatch )
				m_pWatchVideoButton->Update(e_fElpaseTime);
			if( m_pPlayGameButton && m_bCurrentGameCouldBePlay )
				m_pPlayGameButton->Update(e_fElpaseTime);
			if( m_pWatchVideoButton && m_pWatchVideoButton->IsSatisfiedCondition() && m_bCurrentVideoCouldBeWatch)
			{
				m_strNextPhaseName = WIN_PRIZE_PHASE;
				this->m_bSatisfiedCondition = true;
				m_pSelectedStageData = new cStageData();
				m_StageDataVector[m_pScroller->GetCurrentWorkingObjectIndex()]->Assign(m_pSelectedStageData);
				cGameApp::m_spSoundParser->RemoveAllBGM();
			}

			if( this->m_pScroller && m_pScroller->GetCurrentWorkingObjectIndex() != -1 && ((m_pPlayGameButton && m_pPlayGameButton->IsSatisfiedCondition()) || m_SelectedGirlImage.IsSatisfiedCondition() ) )
			{
				m_eSelectStageTYpe = eSST_INTO_GAME;
				m_pPlayGameButton->Init();
				m_SelectedGirlImage.Init();
			}
		}
	}
	m_MainfunctionButton.Update(e_fElpaseTime);
	if( m_MainfunctionButton.IsSatisfiedCondition() )
	{
		m_strNextPhaseName = m_MainfunctionButton.GetNextPhaseName();
		this->m_bSatisfiedCondition = true;
		m_MainfunctionButton.Init();
	}
}

void	cChoiceGirlStagePhase::Update(float e_fElpaseTime)
{
	if( m_pBGMPDI )
		m_pBGMPDI->Update(e_fElpaseTime);
	m_SelectedGirlImage.Update(e_fElpaseTime);
	UpdateSelectStage(e_fElpaseTime);
	UpdateIntoGame(e_fElpaseTime);
	if(this->m_bSatisfiedCondition && 
		wcscmp(STORE_PHASE_NAME,m_strNextPhaseName.c_str()) &&
		wcscmp(SETUP_PHASE_NAME,m_strNextPhaseName.c_str()))
	{
		Destroy();
	}

}

void	cChoiceGirlStagePhase::Render()
{
	if( m_pBGMPDI )
		m_pBGMPDI->Render();
	if( m_pScroller )
		m_pScroller->Render();
	if( m_pWatchVideoButton )
		m_pWatchVideoButton->Render();
	if( m_pPlayGameButton )
		m_pPlayGameButton->Render();
	if( m_pScroller&&m_pScroller->GetCurrentWorkingObjectIndex() != -1 )
	{
		m_SelectedGirlImage[m_pScroller->GetCurrentWorkingObjectIndex()]->Render();
		m_pGirlImageCover->RenderLastFrame();
	}
	m_MainfunctionButton.Render();
	for(int i=0;i<m_GirlUnlokImageVector.Count();++i)
	{
		m_GirlUnlokImageVector[i]->Render();
	}
	if( m_eSelectStageTYpe == eSST_INTO_GAME )
		m_pIntoGamePhasePopUpMessager->Render();
}

void	cChoiceGirlStagePhase::DebugRender()
{
	if( m_pScroller )
		m_pScroller->DebugRender();
	m_MainfunctionButton.DebugRender();
	if( m_pPlayGameButton )
		m_pPlayGameButton->DebugRender();
	if( m_pWatchVideoButton )
		m_pWatchVideoButton->DebugRender();
}

void	cChoiceGirlStagePhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_eSelectStageTYpe <= eSST_SELECT_VIDEO_OR_GAME )
	{
		if( m_pScroller )
			m_pScroller->MouseDown(e_iPosX,e_iPosY);
		if( m_eSelectStageTYpe == eSST_SELECT_VIDEO_OR_GAME )
		{
			if( m_pWatchVideoButton && m_bCurrentVideoCouldBeWatch )
				m_pWatchVideoButton->MouseDown(e_iPosX,e_iPosY);
			if( m_pPlayGameButton && m_bCurrentGameCouldBePlay )
				m_pPlayGameButton->MouseDown(e_iPosX,e_iPosY);
		}
		m_MainfunctionButton.MouseDown(e_iPosX,e_iPosY);
		m_SelectedGirlImage.Init();
		if( m_pScroller&&m_pScroller->GetCurrentWorkingObjectIndex() != -1 )
		{
			m_SelectedGirlImage[m_pScroller->GetCurrentWorkingObjectIndex()]->MouseDown(e_iPosX,e_iPosY);
		}
	}
	else
	{
		m_pIntoGamePhasePopUpMessager->MouseDown(e_iPosX,e_iPosY);
	}
}

void	cChoiceGirlStagePhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_eSelectStageTYpe <= eSST_SELECT_VIDEO_OR_GAME )
	{
		if( m_pScroller )
			m_pScroller->MouseUp(e_iPosX,e_iPosY);
		if( m_eSelectStageTYpe == eSST_SELECT_VIDEO_OR_GAME )
		{
			if( m_pWatchVideoButton && m_bCurrentVideoCouldBeWatch )
				m_pWatchVideoButton->MouseUp(e_iPosX,e_iPosY);
			if( m_pPlayGameButton && m_bCurrentGameCouldBePlay )
				m_pPlayGameButton->MouseUp(e_iPosX,e_iPosY);
		}
		m_MainfunctionButton.MouseUp(e_iPosX,e_iPosY);
		if( m_pScroller&&m_pScroller->GetCurrentWorkingObjectIndex() != -1 )
		{
			m_SelectedGirlImage[m_pScroller->GetCurrentWorkingObjectIndex()]->MouseUp(e_iPosX,e_iPosY);
		}
	}
	else
	{
		m_pIntoGamePhasePopUpMessager->MouseUp(e_iPosX,e_iPosY);
	}
}

void	cChoiceGirlStagePhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_eSelectStageTYpe <= eSST_SELECT_VIDEO_OR_GAME )
	{
		if( m_pScroller )
			m_pScroller->MouseMove(e_iPosX,e_iPosY);
		if( m_eSelectStageTYpe == eSST_SELECT_VIDEO_OR_GAME )
		{
			if( m_pWatchVideoButton && m_bCurrentVideoCouldBeWatch )
				m_pWatchVideoButton->MouseMove(e_iPosX,e_iPosY);
			if( m_pPlayGameButton && m_bCurrentGameCouldBePlay )
				m_pPlayGameButton->MouseMove(e_iPosX,e_iPosY);
		}
		m_MainfunctionButton.MouseMove(e_iPosX,e_iPosY);
		if( m_pScroller&&m_pScroller->GetCurrentWorkingObjectIndex() != -1 )
		{
			m_SelectedGirlImage[m_pScroller->GetCurrentWorkingObjectIndex()]->MouseMove(e_iPosX,e_iPosY);
		}
	}
	else
	{
		m_pIntoGamePhasePopUpMessager->MouseMove(e_iPosX,e_iPosY);
	}
}

const   WCHAR*	cChoiceGirlStagePhase::GetNextPhaseName()
{
	return m_strNextPhaseName.c_str();
}

void*	cChoiceGirlStagePhase::GetData()
{
	cChoiceGirlStagePhase::cStageData*l_pStageData = GetCurrentStageData();
	if( !l_pStageData )
		return 0;
	if( wcscmp(m_strNextPhaseName.c_str(),WIN_PRIZE_PHASE) == 0 )
	{
		return (void*)l_pStageData->m_strVideoName.c_str();
	}
	return (void*)l_pStageData->m_strBGImage.c_str();
	return (void*)L"Image/Girl/Girl1/BG.png";
	return (void*)m_pScroller->GetCurrentWorkingObjectName();
}

void	cChoiceGirlStagePhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	if( e_pData )
	{
		const WCHAR*l_strSelectedGirlName = (const WCHAR*)e_pData;
		m_strCurrentSelectedGirlName = l_strSelectedGirlName;
		m_strFileName = "BluffingGirl/PhaseData/GirlStage/";
		m_strFileName += UT::WcharToChar(m_strCurrentSelectedGirlName.c_str());
		m_strFileName += ".xml";
	}
}

cChoiceGirlStagePhase::cStageData*		cChoiceGirlStagePhase::GetCurrentStageData()
{
	if( m_pSelectedStageData)
	{
		return m_pSelectedStageData;
	}
	return 0;
}