#include "stdafx.h"
#include "BluffingDiceGame.h"
#include "DiceTable.h"
#include "BluffingDiceAI.h"
#include "SpecialDiceRule.h"
#include "AllPhaseDefine.h"
#include "BluffingGirlApp.h"
#include "PlayerData.h"
#include "ChoiceGirlStagePhase.h"
#include "StageClearFile.h"
int	g_iSpecialDiceRulePoint = 0;
bool*	g_pbUseClairvoyant = 0;


cBluffingDiceGame::cBattleRoundData::cBattleRoundData()
{
	m_pHintMPDI = 0;
	m_iNumRound = 0;
	m_iPlayerWinCount = 0;
	m_iAIWinCount = 0;
	m_iTotalRound = 3;
	m_iWinCount = 2;
	m_iCurrentWinCount = 0;
	m_iCurrentLoseCount = 0;
}

cBluffingDiceGame::cBattleRoundData::~cBattleRoundData()
{
	Destroy();
}

void	cBluffingDiceGame::cBattleRoundData::Destroy()
{
	SAFE_DELETE(m_pHintMPDI);
}

void	cBluffingDiceGame::cBattleRoundData::Init()
{
	m_iCurrentWinCount = 0;
	m_iCurrentLoseCount = 0;
	if( !m_pHintMPDI )
	{
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(L"BluffingGirl/Image/GamePlay.mpdi");
		cMPDI*l_pMPDI = l_pMPDIList->GetObject(L"Pass_Necessary");
		if( l_pMPDI )
		{
			m_pHintMPDI = new cMPDI(l_pMPDI);
			m_pHintMPDI->Init();
			m_pHintMPDI->Update(0.016f);
			RefreshResult();
		}
	}
}

void	cBluffingDiceGame::cBattleRoundData::Render()
{
	if( m_pHintMPDI )
		m_pHintMPDI->Render();
}

void	cBluffingDiceGame::cBattleRoundData::RefreshResult()
{
	if( !m_pHintMPDI )
		return;
	for( int i=1;i<5;++i )
	{
		const WCHAR*l_strNumeralPrefix = L"CanDoShake_00";
		std::wstring	l_strObjectName = l_strNumeralPrefix;
		if( i == 1 )
			l_strObjectName += ValueToStringW(this->m_iTotalRound);
		else
		if( i == 2 )
			l_strObjectName += ValueToStringW(this->m_iWinCount);
		else
		if( i == 3 )
			l_strObjectName += ValueToStringW(this->m_iCurrentWinCount);
		else
		if( i == 4 )
			l_strObjectName += ValueToStringW(this->m_iCurrentLoseCount);
		int	l_iIndex = m_pHintMPDI->GetObject(i)->GetPointData(0)->pPI->GetObjectIndexByName(l_strObjectName.c_str());
		m_pHintMPDI->GetObject(i)->ChangeAllImageByImageIndex(l_iIndex);
	}
}

void	cBluffingDiceGame::cBattleRoundData::RoundAdd()
{
	++m_iNumRound;
	RefreshResult();
	std::wstring	l_str = L"Round:";
	l_str += ValueToStringW(m_iNumRound);
	cGameApp::WriteLog(l_str.c_str());
}
void	cBluffingDiceGame::cBattleRoundData::ResultLog(int e_iWinIndex)
{
	if( e_iWinIndex == 0 )
	{
		cGameApp::SoundPlay(L"BluffingGirl/Sound/win.wav",true);
	}
	else
	{
		cGameApp::SoundPlay(L"BluffingGirl/Sound/lose.wav",true);
	}
	std::wstring	l_str = e_iWinIndex == 0?L"PlayerWin	":L"AIWin	";
	l_str += L"AIWinCount = ,	";
	l_str += ValueToStringW(m_iAIWinCount);
	l_str += L"PlayerWinCount = ";
	l_str += ValueToStringW(m_iPlayerWinCount);
	cGameApp::WriteLog(L"=========================");
	cGameApp::WriteLog(l_str.c_str());
	cGameApp::WriteLog(L"=========================");		
}


cBluffingDiceGame::cBluffingDiceGame()
{
	m_pDiceCallSound = 0;
	m_pShowRuleButton = 0;
	m_pResultNotUseClairvoyantMPDI = 0;
	m_pResultUseClairvoyantMPDI = 0;
	m_pShowUseClairvoyantMPDI = 0;
	m_pShowNotUseClairvoyantMPDI = 0;
	m_pBattleRoundData = new cBattleRoundData();
	m_pStageData = 0;
	m_pUseClairvoyant = 0;
	//g_pbUseClairvoyant  = new bool;
	if( g_pbUseClairvoyant  )
		*g_pbUseClairvoyant = true;
	m_pVideo = 0;
	this->SetName(BLUFFING_DICEGAME_PHASE_NAME);
	m_pBGMPDI = 0;
	m_pShowMPDI = 0;
	m_bLastStage = false;
	m_strBGFileName = L"Image/Girl/Girl1/BG.png";
	m_strGameSetupFileName = "BluffingGirl/PhaseData/GirlStage/GameSetup/EasyMode.xml";
	m_pBGImage = 0;
	m_pResultMPDI = 0;
	m_eCatchType = eCT_NO_CATCH;
	m_iWinIndex = -1;
	m_iCallDicePoint = -1;
	m_iCallDiceCountIndex = 0;
	m_pDiceRule = 0;
	m_pBluffingDiceAI = 0;
	m_iTurnToCallIndex = 0;
	m_pCallImageButton = 0;
	m_pCatchImageButton = 0;
	m_pPlayerDiceTable = 0;
	//m_pCallClickLegalSound = 0;
	m_pPlayerCallSubMPDI = 0;
	m_pAICallSubMPDI = 0;
	m_pAICatchSubMPDI = 0;
	m_pPlayerCatchSubMPDI = 0;
	m_pPlayerWin = 0;
	m_pAIWin = 0;
	m_pSpecialDiceRule = 0;
	m_pFailedMenuButtonList = 0;
}
cBluffingDiceGame::~cBluffingDiceGame()
{
	Destroy();
	SAFE_DELETE(m_pBattleRoundData);
}

void	cBluffingDiceGame::Destroy()
{
	m_pBGMPDI = 0;
	if( m_pBattleRoundData )
		m_pBattleRoundData->Destroy();
	if( m_pDiceCallSound )
		m_pDiceCallSound->Release(this);
	m_pDiceCallSound = 0;
	SAFE_DELETE(m_pShowRuleButton);
	SAFE_DELETE(m_pUseClairvoyant);
	SAFE_DELETE(m_pVideo);
	SAFE_DELETE(m_pBGImage);
	SAFE_DELETE(m_pResultMPDI);
	SAFE_DELETE(m_pShowMPDI);
	SAFE_DELETE(m_pFailedMenuButtonList);
	SAFE_DELETE(m_pCallImageButton);
	SAFE_DELETE(m_pCatchImageButton);
	SAFE_DELETE(m_pBluffingDiceAI);
	SAFE_DELETE(m_pDiceRule);
	SAFE_DELETE(m_pPlayerDiceTable);
	SAFE_DELETE(m_pSpecialDiceRule);
	DELETE_VECTOR(m_AIDiceTableCallResultVector);
	m_ResourceStamp.ReleaseNewResource();
	cGameApp::m_spSoundParser->RemoveAllBGM();
}
//<Root>
//	<DiceRule Min="1" Max="7" OneDiceRule="1" PercentForSpecialNumeral1="0.1" NumDice="5"/>
//	<DiceTableData FileName="">
//	<AIData WinCallPercentage="0.6" SkipStep="1,2" VisibleDice="5,6" />
//</Root>
void	cBluffingDiceGame::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("DiceTableData")
	{
		ProcessDiceTableData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("AIData")
	{
		ProcessDiceAIData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("DiceRule")
	{
		ProcessDiceRule(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("ImageData")
	{
		ProcessImageData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("SoundData")
	{
		ProcessSoundData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("SpecialDiceRule")
	{
		assert(m_pSpecialDiceRule == 0);
		m_pSpecialDiceRule = new cSpecialDiceRule(e_pTiXmlElement);
		g_iSpecialDiceRulePoint = m_pSpecialDiceRule->m_iSpecialDicePoint;
	}
	else
	COMPARE_VALUE("cPhasePopUpMessager")
	{
		assert( m_pUseClairvoyant == 0 );
		m_pUseClairvoyant = new cPhasePopUpMessager(e_pTiXmlElement);
		m_pUseClairvoyant->Init();
	}
}

void	cBluffingDiceGame::ProcessDiceRule(TiXmlElement*e_pTiXmlElement)
{
	assert(m_pDiceRule == 0);
	m_pDiceRule = new sDiceRule(e_pTiXmlElement);
}
//<Root>
//	<DiceTableData Type="Player">
//		<DiceTable PI="Image/DiceImage.pi" Dice1="last_result 1" Dice2="last_result 2" Dice3="last_result 3" Dice4="last_result 4" Dice5="last_result 5" Dice6="last_result 5" BlockDice="Black" NumeralImage="Probability_0,Probability_9">
//			<Data DicePos="100,800" DiceImageGap="39" NumeralPos="100,840" NumeralImageGap="39" />
//		</DiceTable>
//	</DiceTableData>
//	<DiceTableData Type="AI">
//		<DiceTable PI="Image/DiceImage.pi" Dice1="last_result 1" Dice2="last_result 2" Dice3="last_result 3" Dice4="last_result 4" Dice5="last_result 5" Dice6="last_result 5" BlockDice="Black" >
//			<Data 	DicePos="100,400" DiceImageGap="39" NumeralPos="100,840" NumeralImageGap="39" />
//		</DiceTable>
//	</DiceTableData>
//</Root>
void	cBluffingDiceGame::ProcessDiceTableData(TiXmlElement*e_pTiXmlElement)
{
	if( !m_pBluffingDiceAI )
		return;
	//<DiceTableData FileName="">
	const WCHAR*l_strFileName = e_pTiXmlElement->Attribute(L"FileName");
	if( l_strFileName )
	{
		cNodeISAX	l_NodeISAX;
		if(l_NodeISAX.ParseDataIntoXMLNode(l_strFileName))
		{
			assert(m_pDiceRule && "dice rule please put at first child element");
			TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
			if(l_pTiXmlElement)
			{
				l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
				int	l_iDiceTableIndex = 0;
				while( l_pTiXmlElement )
				{
					if( !wcscmp(l_pTiXmlElement->Value(),L"DiceTableData") )
					{
						const WCHAR*l_strType = l_pTiXmlElement->Attribute(L"Type");
						if( l_strType &&!wcscmp(l_strType,L"Player") )
						{
							assert( m_pPlayerDiceTable == 0 &&"player data exists");
							m_pPlayerDiceTable = new cDiceTable(l_pTiXmlElement->FirstChildElement(),m_pDiceRule,eBT_PLAYER);
							m_pPlayerDiceTable->SetReveal(true);
						}
						else
						{
							cAIDiceTableCallResult*l_pAIDiceTableCallResult = new cAIDiceTableCallResult(this->m_pBluffingDiceAI,l_pTiXmlElement->FirstChildElement(),l_iDiceTableIndex);
							//l_pAIDiceTableCallResult->GetDiceTable()->SetReveal(true);
							m_AIDiceTableCallResultVector.push_back(l_pAIDiceTableCallResult);
						}
						++l_iDiceTableIndex;
					}
					l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
				}
				std::vector<cDiceTable*>	l_AllDiceTable;
				l_AllDiceTable.push_back(m_pPlayerDiceTable);
				for(size_t i=0;i<m_AIDiceTableCallResultVector.size();++i)
					l_AllDiceTable.push_back(m_AIDiceTableCallResultVector[i]->GetDiceTable());
				m_pBluffingDiceAI->AssignTableData(&l_AllDiceTable);
			}
		}
	}
	else
	{
		UT::ErrorMsg(L"ProcessDiceTableData error",L"no FileName attribute");
	}
}

void	cBluffingDiceGame::ProcessDiceAIData(TiXmlElement*e_pTiXmlElement)
{
	assert(m_pBluffingDiceAI==0);
	assert(m_pDiceRule);
	m_pBluffingDiceAI = new cBluffingDiceAI(e_pTiXmlElement,m_pDiceRule);
}

//CallLegalSound="CallClick.wav" CallInlegalSound="CallClickInlegal.wav"
void	cBluffingDiceGame::ProcessSoundData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("CallLegalSound")
		{
			//m_pCallClickLegalSound = cGameApp::GetSoundByFileName(l_strValue);
			
		}
		else
		COMPARE_NAME("CallInlegalSound")
		{
			//m_pCallClickInlegalSound =  cGameApp::GetSoundByFileName(l_strValue);;
		}
	PARSE_NAME_VALUE_END
}
//<ImageData PI="Image/Dice/Dice.pi" CallDicePointImagePrefixName="Probability board_point" CallDiceCountImagePrefixName="congratulation_"/>
void	cBluffingDiceGame::ProcessImageData(TiXmlElement*e_pTiXmlElement)
{
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		if(!wcscmp(e_pTiXmlElement->Value(),L"CallHint"))
		{
			cMPDIList*l_pMPDILIst = 0;
			cMPDI*l_pMPDI = 0;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("MPDIList")
				{
					l_pMPDILIst  =cGameApp::GetMPDIListByFileName(l_strValue);
				}
				else
				COMPARE_NAME("MPDI")
				{
					if( l_pMPDILIst )
						l_pMPDI = l_pMPDILIst->GetObject(l_strValue);
				}
				else
				COMPARE_NAME("PlayerCall")
				{
					m_pPlayerCallSubMPDI = l_pMPDI->GetObject(l_strValue);
					if( m_pPlayerCallSubMPDI )
						m_pPlayerCallSubMPDI->SetAnimationDone(true);
				}
				else
				COMPARE_NAME("AICall")
				{
					m_pAICallSubMPDI = l_pMPDI->GetObject(l_strValue);
					if( m_pAICallSubMPDI )
						m_pAICallSubMPDI->SetAnimationDone(true);
				}
			PARSE_NAME_VALUE_END
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"CatchHint"))
		{
			cMPDIList*l_pMPDILIst = 0;
			cMPDI*l_pMPDI = 0;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("MPDIList")
				{
					l_pMPDILIst  =cGameApp::GetMPDIListByFileName(l_strValue);
				}
				else
				COMPARE_NAME("MPDI")
				{
					l_pMPDI = l_pMPDILIst->GetObject(l_strValue);
				}
				else
				COMPARE_NAME("PlayerCatch")
				{
					m_pPlayerCatchSubMPDI = l_pMPDI->GetObject(l_strValue);
					if( m_pPlayerCatchSubMPDI )
						m_pPlayerCatchSubMPDI->SetAnimationDone(true);
				}
				else
				COMPARE_NAME("AICatch")
				{
					m_pAICatchSubMPDI = l_pMPDI->GetObject(l_strValue);
					if( m_pAICatchSubMPDI )
						m_pAICatchSubMPDI->SetAnimationDone(true);
				}
			PARSE_NAME_VALUE_END
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"CatchResult"))
		{
			cMPDIList*l_pMPDILIst = 0;
			cMPDI*l_pMPDI = 0;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("MPDIList")
				{
					l_pMPDILIst  =cGameApp::GetMPDIListByFileName(l_strValue);
				}
				else
				COMPARE_NAME("WinMPDI")
				{
					m_pPlayerWin = l_pMPDILIst->GetObject(l_strValue);
				}
				else
				COMPARE_NAME("LoseMPDI")
				{
					m_pAIWin = l_pMPDILIst->GetObject(l_strValue);
				}
			PARSE_NAME_VALUE_END
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"FailedMenu"))
		{
			m_pFailedMenuButtonList = GetClickMouseBehaviorList(e_pTiXmlElement->FirstChildElement());
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"cImageButton"))
		{//<ImageButton PI="Image/Dice/Dice.pi" CallButton="Call" CallPos="500,500" CatchButton="Open" CatchPos="500,550" />
			cImageButton*l_pImageButton = cImageButton::GetMe(e_pTiXmlElement);
			//cImageButton*l_pImageButton = 0;
			if( l_pImageButton )
			{
				if( l_pImageButton->IsSameName(L"Catch") )
					m_pCatchImageButton = l_pImageButton;
				else
				if( l_pImageButton->IsSameName(L"Call") )
					m_pCallImageButton = l_pImageButton;
				else
				if( l_pImageButton->IsSameName(L"ShowRule") )
					m_pShowRuleButton = l_pImageButton;
				else
				{
					UT::ErrorMsg(L"this button is not using",l_pImageButton->GetName());
					SAFE_DELETE(l_pImageButton);
				}
			}

		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"ShowMPDI"))
		{
			m_pShowUseClairvoyantMPDI = cMPDI::GetMe(GetXmlElementByNameFromElement(L"UseClairvoyant",e_pTiXmlElement)->FirstChildElement(),false);
			m_pShowNotUseClairvoyantMPDI = cMPDI::GetMe(GetXmlElementByNameFromElement(L"NoUseClairvoyant",e_pTiXmlElement)->FirstChildElement(),false);
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"ResultMPDI"))
		{
			m_pResultUseClairvoyantMPDI = cMPDI::GetMe(GetXmlElementByNameFromElement(L"UseClairvoyant",e_pTiXmlElement)->FirstChildElement(),false);
			m_pResultNotUseClairvoyantMPDI = cMPDI::GetMe(GetXmlElementByNameFromElement(L"NoUseClairvoyant",e_pTiXmlElement)->FirstChildElement(),false);
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"BG"))
		{
			m_pBGMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
			if( m_pBGMPDI )
				m_pBGMPDI->Init();
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
}


void	cBluffingDiceGame::GameRestartInit()
{
	m_eGameStatus = eGS_DICE_SHAKE;
	m_pBattleRoundData->RoundAdd();
	//
	m_iCallDicePoint = -1;
	m_iCallDiceCountIndex = 0;
	m_eCatchType = eCT_NO_CATCH;
	if( m_pUseClairvoyant )
		m_pUseClairvoyant->Init();
	if( m_pPlayerDiceTable )
		m_pPlayerDiceTable->Init();
	for(size_t i=0;i<m_AIDiceTableCallResultVector.size();++i)
	{
		m_AIDiceTableCallResultVector[i]->Init();
		m_AIDiceTableCallResultVector[i]->GetDiceTable()->SetReveal(cBluffingGirlApp::m_spPlayerData->IsUsingClairvoyant());
		if( g_pbUseClairvoyant )
			m_AIDiceTableCallResultVector[i]->GetDiceTable()->SetReveal(*g_pbUseClairvoyant);
	}
	if( m_pSpecialDiceRule )
		this->m_pSpecialDiceRule->m_bWorking = true;
	if( m_pCallImageButton )
		m_pCallImageButton->Init();
	if( m_pShowRuleButton )
		m_pShowRuleButton->Init();
	if( m_pCatchImageButton )
		m_pCatchImageButton->Init();
	if( m_pPlayerWin )
		m_pPlayerWin->Init();
	if( m_pAIWin )
		m_pAIWin->Init();
	if( m_pFailedMenuButtonList )
		m_pFailedMenuButtonList->Init();
	if( this->m_pStageData )
	{
		if( m_pStageData->m_strShakeDiceVideoFileNameVector.size() > 0 )
		{
			if( !m_pVideo )
				m_pVideo = new cFMVVideo();
			int	l_iIndex = rand()%m_pStageData->m_strShakeDiceVideoFileNameVector.size();
			if(!m_pVideo->OpenFile(UT::WcharToChar(m_pStageData->m_strShakeDiceVideoFileNameVector[l_iIndex]).c_str()))
			{
				SAFE_DELETE(m_pVideo);
			}
		}
	}
	if( m_pBGImage )
	{
		m_pBGImage->SetWidth((int)cGameApp::m_spOpenGLRender->m_vGameResolution.x);
		m_pBGImage->SetHeight((int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	}
	//
	cGameApp::m_sTimeAndFPS.Update();
}

void	cBluffingDiceGame::Init()
{
	m_eGameStatus = eGS_DICE_SHAKE;
	this->Destroy();
	m_ResourceStamp.StampResource1();
	m_pBattleRoundData->RoundAdd();
	m_pBattleRoundData->Init();
	//
	m_iCallDicePoint = -1;
	m_iCallDiceCountIndex = 0;
	m_eCatchType = eCT_NO_CATCH;
	if(!this->Parse(m_strGameSetupFileName.c_str()))
		UT::ErrorMsg(m_strGameSetupFileName.c_str(),"parse failed!");
	if( m_pPlayerDiceTable )
		m_pPlayerDiceTable->Init();
	//
	//std::vector<int> l_test;
	//for( int i=0;i<5;++i )
	//	l_test.push_back(0);
	//m_pPlayerDiceTable->SetDiceTable(l_test);
	//
	for(size_t i=0;i<m_AIDiceTableCallResultVector.size();++i)
	{
		m_AIDiceTableCallResultVector[i]->Init();
		m_AIDiceTableCallResultVector[i]->GetDiceTable()->SetReveal(cBluffingGirlApp::m_spPlayerData->IsUsingClairvoyant());
		if( g_pbUseClairvoyant )
			m_AIDiceTableCallResultVector[i]->GetDiceTable()->SetReveal(*g_pbUseClairvoyant);
	}
	if( m_pCallImageButton )
		m_pCallImageButton->Init();
	if( m_pCatchImageButton )
		m_pCatchImageButton->Init();
	if( m_pShowRuleButton )
		m_pShowRuleButton->Init();
	if( m_pPlayerWin )
		m_pPlayerWin->Init();
	if( m_pAIWin )
		m_pAIWin->Init();
	if( m_pFailedMenuButtonList )
		m_pFailedMenuButtonList->Init();
	if( this->m_pStageData )
	{
		if( m_pStageData->m_strShakeDiceVideoFileNameVector.size() > 0 )
		{
			m_pVideo = new cFMVVideo();
			int	l_iIndex = rand()%m_pStageData->m_strShakeDiceVideoFileNameVector.size();
			if(!m_pVideo->OpenFile(UT::WcharToChar(m_pStageData->m_strShakeDiceVideoFileNameVector[l_iIndex]).c_str()))
			{
				SAFE_DELETE(m_pVideo);
			}
		}
	}
	m_pBGImage = new cBaseImage(UT::WcharToChar(m_strBGFileName).c_str());
	if( m_pBGImage )
	{
		m_pBGImage->SetWidth((int)cGameApp::m_spOpenGLRender->m_vGameResolution.x);
		m_pBGImage->SetHeight((int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	}
	//
	//
	m_ResourceStamp.StampResource2();
	cGameApp::m_sTimeAndFPS.Update();
}

void	cBluffingDiceGame::WriteWinMessage(int e_iWinIndex)
{
	ChangeDiceResultImage();
	if( e_iWinIndex == 0 )
	{
		++m_pBattleRoundData->m_iPlayerWinCount;
		++m_pBattleRoundData->m_iCurrentWinCount;
	}
	else
	{
		++m_pBattleRoundData->m_iAIWinCount;
		++m_pBattleRoundData->m_iCurrentLoseCount;
	}
	m_pBattleRoundData->ResultLog(e_iWinIndex);
	m_pBattleRoundData->RefreshResult();
}

void	cBluffingDiceGame::UpdateDiceCall(float e_fElpaseTime)
{
	if( m_iTurnToCallIndex == 0 )
	{
		if(cBluffingGirlApp::m_spPlayerData->IsUsingClairvoyant() || (g_pbUseClairvoyant&&*g_pbUseClairvoyant))
		{
			for(size_t i=0;i<m_AIDiceTableCallResultVector.size();++i)
				m_AIDiceTableCallResultVector[i]->GetDiceTable()->Update(e_fElpaseTime);
		}
		if( m_pPlayerDiceTable )
			m_pPlayerDiceTable->Update(e_fElpaseTime);
		if( m_pCallImageButton)
		{
			m_pCallImageButton->Update(e_fElpaseTime);
			if( m_pCallImageButton->IsSatisfiedCondition() )
			{
				cGameApp::SoundPlay(BUTTON_CLICK_SOUND,true);
				m_pCallImageButton->Init();
				if( m_pPlayerDiceTable->m_NumeralImageButton.GetCurrentWorkingObject() && m_pPlayerDiceTable->m_DiceImageButtonContainer.GetCurrentWorkingObject() && m_pPlayerDiceTable->IsConditionOk(m_iCallDicePoint,m_iCallDiceCountIndex,1+this->m_AIDiceTableCallResultVector.size()) )
				{
					if( m_pPlayerCallSubMPDI )
					{
						m_pPlayerCallSubMPDI->Init();
						m_pPlayerCallSubMPDI->Update(0.001f);
					}
					if( m_pAICallSubMPDI )
						m_pAICallSubMPDI->SetAnimationDone(true);
					//if( m_pCallClickLegalSound )
					//	m_pCallClickLegalSound->Play(true);
					cGameApp::SoundPlay(BUTTON_CLICK_SOUND,true);
					++m_iTurnToCallIndex;
					cAIDiceTableCallResult*l_pAIDiceTableCallResult = m_AIDiceTableCallResultVector[m_iTurnToCallIndex-1];
					l_pAIDiceTableCallResult->SetWaitAI(true);
					if( m_pPlayerDiceTable->GetCurrentSelectDice() == m_pSpecialDiceRule->m_iSpecialDicePoint )
						this->m_pSpecialDiceRule->m_bWorking = false;
					CallDiceImageUpdate(m_pPlayerDiceTable->GetCurrentSelectNumeral(),m_pPlayerDiceTable->GetCurrentSelectDice(),true);
				}
				else
				{
					cGameApp::SoundPlay(BUTTON_CLICK_WRONG_SOUND,true);
					//if( m_pCallClickInlegalSound )
					//	m_pCallClickInlegalSound->Play(true);
				}
			}
		}
		if( m_pCatchImageButton )
		{
			m_pCatchImageButton->Update(e_fElpaseTime);
			if( m_pCatchImageButton->IsSatisfiedCondition() )
			{
				cGameApp::SoundPlay(BUTTON_CLICK_SOUND,true);
				//m_pCatchImageButton->Init();
				if( m_iCallDiceCountIndex != 0 )
				{
					//if( m_pCallClickLegalSound )
					//	m_pCallClickLegalSound->Play(true);
					if( m_pAICallSubMPDI )
						m_pAICallSubMPDI->SetAnimationDone(true);
					if( m_pPlayerCallSubMPDI )
						m_pPlayerCallSubMPDI->SetAnimationDone(true);
					m_eCatchType = eCT_PLAYER_CATCH;
					if( m_pPlayerCatchSubMPDI )
					{
						m_pPlayerCatchSubMPDI->Init();
						m_pPlayerCatchSubMPDI->Update(0.001f);
					}
					if( IsCatchWin() )
					{
						m_iWinIndex = 0;
					}
					else
					{
						m_iWinIndex = 1;
					}
					WriteWinMessage(m_iWinIndex);
				}
				else
				{
					cGameApp::SoundPlay(BUTTON_CLICK_WRONG_SOUND,true);
				}
			}
		}
		if(m_pShowRuleButton)
		{
			m_pShowRuleButton->Update(e_fElpaseTime);
			if( m_pShowRuleButton->IsSatisfiedCondition() )
			{
				m_pShowRuleButton->Init();
				this->m_bSatisfiedCondition = true;
				m_strNextPhaseName = RULE_PHASE_NAME;
			}
		}
	}
	else
	{
		cAIDiceTableCallResult*l_pAIDiceTableCallResult = m_AIDiceTableCallResultVector[m_iTurnToCallIndex-1];
		l_pAIDiceTableCallResult->AIUpdate(m_iCallDiceCountIndex,m_iCallDicePoint,m_pSpecialDiceRule->m_bWorking,e_fElpaseTime);
		if(!l_pAIDiceTableCallResult->IsWaitForAI())
		{
			if(l_pAIDiceTableCallResult->IsCatch())
			{
				if( m_pAICatchSubMPDI )
				{
					m_pAICatchSubMPDI->Init();
					m_pAICatchSubMPDI->Update(0.001f);
				}
				if( m_pAICallSubMPDI )
					m_pAICallSubMPDI->SetAnimationDone(true);
				if( m_pPlayerCallSubMPDI )
					m_pPlayerCallSubMPDI->SetAnimationDone(true);
				m_eCatchType = eCT_AI_CATCH;
				if( IsCatchWin() )
					m_iWinIndex = m_iTurnToCallIndex;
				else
					m_iWinIndex = 0;
				WriteWinMessage(m_iWinIndex);
			}
			else
			{
				if( m_pAICallSubMPDI )
				{
					m_pAICallSubMPDI->Init();
					m_pAICallSubMPDI->Update(0.001f);
				}
				if( m_pPlayerCallSubMPDI )
					m_pPlayerCallSubMPDI->SetAnimationDone(true);
				CallDiceImageUpdate(m_pBluffingDiceAI->m_iDiceCountCallResult,m_pBluffingDiceAI->m_iDicePointCallResult,false);
				if( m_pBluffingDiceAI->m_iDicePointCallResult == m_pSpecialDiceRule->m_iSpecialDicePoint )
						this->m_pSpecialDiceRule->m_bWorking = false;
			}
			++m_iTurnToCallIndex;
			//now only 1 AI to avoid AI catch AI.
			//if( m_iTurnToCallIndex > (int)m_AIDiceTableCallResultVector.size() )
			{
				m_iTurnToCallIndex = 0;
			}
		}
	}
}

void	cBluffingDiceGame::ChangeDiceResultImage()
{
	if( m_pResultMPDI )
	{
		//dice_item_001~
		cSubMPDI*l_pSubMPDI = 0;
		for( int i=2;i<12;++i )
		{
			l_pSubMPDI = m_pResultMPDI->GetObject(i);
			if( !l_pSubMPDI )
				continue;
			std::wstring	l_strImageName =  m_pPlayerDiceTable->m_strMyDiceImagePrefixName.c_str();
			int	l_iImageIndex = -1;
			int	l_iDicePoint = -1;
			if( i<7 )
			{
				l_iDicePoint = this->m_pPlayerDiceTable->m_MyDiceTable[i-2];
				l_strImageName += ValueToStringW(l_iDicePoint+1);
			}
			else
			{
				//0 is player
				cAIDiceTableCallResult*l_pAIDiceTableCallResult = m_AIDiceTableCallResultVector[0];
				l_iDicePoint = l_pAIDiceTableCallResult->GetDiceTable()->GetMyDiceContent()[i-7];
				l_strImageName += ValueToStringW(l_iDicePoint+1);
			}
			l_iImageIndex = l_pSubMPDI->GetPointData(0)->pPI->GetObjectIndexByName(l_strImageName.c_str());
			if( l_iImageIndex == -1 )
			{
				int a=0;
			}
			l_pSubMPDI->ChangeAllImageByImageIndex(l_iImageIndex);
		}
	}
}

void	cBluffingDiceGame::ChangeDiceShowImage()
{
	if( m_pShowMPDI )
	{
		//dice_item_001~
		cSubMPDI*l_pSubMPDI = 0;
		for( int i=3;i<13;++i )
		{
			l_pSubMPDI = m_pShowMPDI->GetObject(i);
			if( !l_pSubMPDI )
				continue;
			std::wstring	l_strImageName =  m_pPlayerDiceTable->m_strMyDiceImagePrefixName.c_str();
			int	l_iImageIndex = -1;
			int	l_iDicePoint = -1;
			if( i<8 )
			{
				l_iDicePoint = this->m_pPlayerDiceTable->m_MyDiceTable[i-3];
				l_strImageName += ValueToStringW(l_iDicePoint+1);
			}
			else
			{
				//0 is player
				cAIDiceTableCallResult*l_pAIDiceTableCallResult = m_AIDiceTableCallResultVector[0];
				l_iDicePoint = l_pAIDiceTableCallResult->GetDiceTable()->GetMyDiceContent()[i-8];
				l_strImageName += ValueToStringW(l_iDicePoint+1);
			}
			l_iImageIndex = l_pSubMPDI->GetPointData(0)->pPI->GetObjectIndexByName(l_strImageName.c_str());
			if( l_iImageIndex == -1 )
			{
				int a=0;
			}
			l_pSubMPDI->ChangeAllImageByImageIndex(l_iImageIndex);
		}
	}
}

void	cBluffingDiceGame::UpdateDiceCatch(float e_fElpaseTime)
{
	bool	l_bWaitCatchHintDone = false;
	if( m_eCatchType == eCT_AI_CATCH )
	{//play video phase
		if( m_pAICatchSubMPDI )
		{
			if( !this->m_pAICatchSubMPDI->IsAnimationDone() )
				this->m_pAICatchSubMPDI->Update(e_fElpaseTime);
			else
			{
				l_bWaitCatchHintDone = true;
			}
		}
		else
		{
			l_bWaitCatchHintDone  =true;
		}
	}
	else
	if( m_eCatchType == eCT_PLAYER_CATCH )
	{//retry again menu or leave
		if( m_pPlayerCatchSubMPDI )
		{
			if( m_pPlayerCatchSubMPDI->IsAnimationDone() )
				m_pPlayerCatchSubMPDI->Update(e_fElpaseTime);
			else
			{
				l_bWaitCatchHintDone  =true;
			}
		}
		else
		{
			l_bWaitCatchHintDone  =true;
		}
	}
	if( l_bWaitCatchHintDone )
	{
		if( m_pResultMPDI )
		{
			m_pResultMPDI->Update(e_fElpaseTime);
			if( !m_pResultMPDI->IsAnimationDone() )
				return;
		}
		if( m_iWinIndex == 0 )
		{//player
			if( m_pPlayerWin && !m_pPlayerWin->IsAnimationDone() )
				m_pPlayerWin->Update(e_fElpaseTime);
			else
			{
				m_eGameStatus = eGS_RESULT_VIDEO;
			}
		}
		else
		{
			if( m_pAIWin && !m_pAIWin->IsAnimationDone() )
				m_pAIWin->Update(e_fElpaseTime);
			else
			{
				m_eGameStatus = eGS_RESULT_VIDEO;
			}
		}
	}
	if( m_eGameStatus == eGS_RESULT_VIDEO )
	{
		cGameApp::m_spSoundParser->RemoveAllBGM();
		if(!this->m_pVideo)
			m_pVideo = new cFMVVideo();
		const WCHAR*l_strVideoName = 0;
		if( m_iWinIndex == 0 )
			l_strVideoName = this->m_pStageData->m_strWinVideoFileName.c_str();
		else
			l_strVideoName = this->m_pStageData->m_strLoseVideoFileName.c_str();
		if(!m_pVideo->OpenFile(UT::WcharToChar(l_strVideoName).c_str()))
		{
			SAFE_DELETE(m_pVideo);
		}
		cBluffingGirlApp::m_spPlayerData->m_bEnableRender = false;
	}
}

void	cBluffingDiceGame::UpdateDiceShakeVideo(float e_fElpaseTime)
{
	cBluffingGirlApp::m_spPlayerData->m_bEnableRender = false;
	if( m_pVideo )
	{
		if( !m_pVideo->m_bUsingMultiThread )
			m_pVideo->Update(e_fElpaseTime);
		if(m_pVideo->IsPlayDone())
		{
			m_pVideo->Destroy();
			m_eGameStatus = eGS_CLAIRVOYANT_USE_CHOICE;
			cBasicSound*l_pSound = cGameApp::GetSoundByFileName(L"BluffingGirl/Sound/BG.ogg");
			if(l_pSound)
			{
				l_pSound->Play(true);
				l_pSound->SetLoop(true);
			}
		}
	}
	else
		m_eGameStatus = eGS_CLAIRVOYANT_USE_CHOICE;
	if(m_eGameStatus == eGS_CLAIRVOYANT_USE_CHOICE)
	{
		Update(e_fElpaseTime);
		cBluffingGirlApp::m_spPlayerData->m_bEnableRender = true;
	}
}

void	cBluffingDiceGame::UpdateResultVideo(float e_fElpaseTime)
{
	if( m_pVideo )
	{
		if( !m_pVideo->m_bUsingMultiThread )
			m_pVideo->Update(e_fElpaseTime);
		if( !m_pVideo->IsPlayDone() )
			return;
	}
	if( m_pBattleRoundData->m_iCurrentWinCount < m_pBattleRoundData->m_iWinCount )
	{
		int l_iRestRound = m_pBattleRoundData->m_iTotalRound-(m_pBattleRoundData->m_iCurrentLoseCount+m_pBattleRoundData->m_iCurrentWinCount);
		int	l_iRestTimeForWin = m_pBattleRoundData->m_iWinCount-m_pBattleRoundData->m_iCurrentWinCount;
		if( l_iRestRound > 0 && l_iRestRound >= l_iRestTimeForWin )
		//if( m_pBattleRoundData->m_iCurrentLoseCount < m_pBattleRoundData->m_iWinCount && m_pBattleRoundData->m_iCurrentWinCount < m_pBattleRoundData->m_iWinCount )
		{
			this->GameRestartInit();
			return;
		}
	}

	cBluffingGirlApp::m_spPlayerData->m_bEnableRender = true;
	//no clairvoyant
	cBluffingGirlApp::m_spPlayerData->Init();
	if( m_pBattleRoundData->m_iCurrentWinCount >= m_pBattleRoundData->m_iWinCount )
	{//player win.
		this->m_bSatisfiedCondition = true;
		m_strNextPhaseName = WIN_PRIZE_PHASE;
		cStageClearFile::StageClear(this->m_strCurrentSelectedGirlNameAndStageName.c_str());
		if( wcslen(m_strUnlockGirlName.c_str()) )
		{
			cStageClearFile::StageClear(m_strUnlockGirlName.c_str());
		}
	}
	else
	{
		if( m_pBGMPDI )
			m_pBGMPDI->Update(e_fElpaseTime);
		if( m_pFailedMenuButtonList )
		{
			m_pFailedMenuButtonList->Update(e_fElpaseTime);
			if( m_pFailedMenuButtonList->IsSatisfiedCondition() )
			{
				switch( m_pFailedMenuButtonList->GetCurrentWorkingObjectIndex() )
				{
					case 0:
						m_strNextPhaseName = STORE_PHASE_NAME;
						this->m_bSatisfiedCondition = true;
						break;
					case 1:
						{
							this->m_bSatisfiedCondition = true;
							m_strNextPhaseName = CHOICE_GIRL_PHASE_NAME;
							cBasicSound*l_pSound = cGameApp::GetSoundByFileName(L"BluffingGirl/Sound/MainBG.ogg");
							if(l_pSound)
							{
								l_pSound->Play(true);
								l_pSound->SetLoop(true);
							}
						}
						break;
					case 2://restart
						if(cBluffingGirlApp::m_spPlayerData->HeartMinus())
						{
							m_pBattleRoundData->Init();
							this->GameRestartInit();
						}
						else
						{//show heart not enough
							cBluffingGirlApp::WakePopupSystemMessenger(L"GoToStore",this->GetName());
						}
						break;
				}
				m_pFailedMenuButtonList->Init();
			}
		}
		else
		{
			m_pBattleRoundData->Init();
			this->GameRestartInit();
		}
	}
}


void	cBluffingDiceGame::Update(float e_fElpaseTime)
{
	if( m_eGameStatus == eGS_DICE_SHAKE )
	{
		UpdateDiceShakeVideo(e_fElpaseTime);
	}
	else
	if( m_eGameStatus == e_GAME_PLAY )
	{
		if( m_pBGMPDI )
			m_pBGMPDI->Update(e_fElpaseTime);
		if( m_pShowMPDI && !m_pShowMPDI->IsAnimationDone() )
			m_pShowMPDI->Update(e_fElpaseTime);
		else
		{
			if( m_eCatchType == eCT_NO_CATCH )
				UpdateDiceCall(e_fElpaseTime);
			else
			{
				UpdateDiceCatch(e_fElpaseTime);
			}
		}
	}
	else
	if( m_eGameStatus == eGS_RESULT_VIDEO )
	{
		UpdateResultVideo(e_fElpaseTime);
	}
	else
	if( m_eGameStatus == eGS_CLAIRVOYANT_USE_CHOICE )
	{
		if( m_pBGMPDI )
			m_pBGMPDI->Update(e_fElpaseTime);
		UpdateUseChairvoyant(e_fElpaseTime);
	}

	if(this->m_bSatisfiedCondition&& ( wcscmp(STORE_PHASE_NAME,m_strNextPhaseName.c_str()) && wcscmp(RULE_PHASE_NAME,m_strNextPhaseName.c_str()) ))
	{
		Destroy();
		if(!wcscmp(CHOICE_GIRL_PHASE_NAME,m_strNextPhaseName.c_str()))
		{
			cBasicSound*l_pSound = cGameApp::GetSoundByFileName(L"BluffingGirl/Sound/MainBG.ogg");
			if(l_pSound)
			{
				l_pSound->Play(true);
				l_pSound->SetLoop(true);
			}			
		}
	}
}

void	cBluffingDiceGame::CallDiceImageUpdate(int e_iCount,int e_iPoint,bool e_bPlayer)
{
	std::wstring	l_str = e_bPlayer?L"PlayerCall:":L"AICall:";
	l_str += L"DiceCount:"; l_str += ValueToStringW(e_iCount);
	l_str += L"DicePoint:"; l_str += ValueToStringW(e_iPoint);
	cGameApp::WriteLog(l_str.c_str());
	m_iCallDiceCountIndex = e_iCount;
	m_iCallDicePoint = e_iPoint;
	if( !e_bPlayer )
	{
		//play call sound
		if( m_pDiceCallSound  )	
			m_pDiceCallSound->Release(this);
		char	l_strSoundFileName[MAX_PATH];
		sprintf(l_strSoundFileName,"BluffingGirl/Sound/%d%d.wav",m_iCallDiceCountIndex+1,m_iCallDicePoint+1);
		m_pDiceCallSound = new cOpanalWAV(this,l_strSoundFileName,false);
		m_pDiceCallSound->Play(true);
	}
	if( e_bPlayer )
	{
		m_pPlayerDiceTable->ChangeCallData(e_iCount,e_iPoint);
	}
	else
	{
		cAIDiceTableCallResult*l_pAIDiceTableCallResult = m_AIDiceTableCallResultVector[m_iTurnToCallIndex-1];
		l_pAIDiceTableCallResult->GetDiceTable()->ChangeCallData(e_iCount,e_iPoint);
	}
}

void	cBluffingDiceGame::Render()
{
	if( m_eGameStatus == eGS_DICE_SHAKE )
	{
		if( m_pVideo )
		{
			m_pVideo->Render();
		}
	}
	else
	if( m_eGameStatus == e_GAME_PLAY )
	{
		if( m_pBGImage )
			m_pBGImage->Render();
		if( m_pBGMPDI )
			m_pBGMPDI->Render();
		if( m_pCallImageButton )
			m_pCallImageButton->Render();
		if( m_pCatchImageButton )
			m_pCatchImageButton->Render();
		if( m_pAICallSubMPDI )
			m_pAICallSubMPDI->Render();
		if( m_pPlayerCallSubMPDI )
			m_pPlayerCallSubMPDI->Render();
		if( m_pAICatchSubMPDI )
			m_pAICatchSubMPDI->Render();
		if( m_pPlayerCatchSubMPDI )
			m_pPlayerCatchSubMPDI->Render();
		if(m_eCatchType == eCT_NO_CATCH  )
		{
			for(size_t i=0;i<m_AIDiceTableCallResultVector.size();++i)
			{
				m_AIDiceTableCallResultVector[i]->Render();
			}
		}
		if( m_pShowRuleButton )
			m_pShowRuleButton->Render();
		if( m_pSpecialDiceRule )
			m_pSpecialDiceRule->Render();
		if(m_eCatchType != eCT_NO_CATCH  )
		{
			this->m_pPlayerDiceTable->ClickButtonRender();
			if( m_pResultMPDI)
			{
				m_pResultMPDI->Render();
			}
			if(!m_pResultMPDI || m_pResultMPDI->IsAnimationDone() )
			{
				if( m_iWinIndex == 0 )
				{//player
					m_pPlayerWin->Render();
				}
				else
				if( m_iWinIndex == 1 )
				{
					if( m_pAIWin && !m_pAIWin->IsAnimationDone() )
						m_pAIWin->Render();
				}
			}
		}
		else
		{
			if( m_pShowMPDI )
				m_pShowMPDI->Render();
			if( m_pPlayerDiceTable )
				m_pPlayerDiceTable->Render();	
		}
		if( m_pBattleRoundData )
			this->m_pBattleRoundData->Render();
	}
	else
	if( m_eGameStatus == eGS_RESULT_VIDEO )
	{
		int l_iRestRound = m_pBattleRoundData->m_iTotalRound-(m_pBattleRoundData->m_iCurrentLoseCount+m_pBattleRoundData->m_iCurrentWinCount);
		if( m_pVideo && !m_pVideo->IsPlayDone() )
			m_pVideo->Render();
		else
		if( m_pBattleRoundData->m_iWinCount-m_pBattleRoundData->m_iCurrentWinCount > l_iRestRound )
		{
			if( m_pBGImage )
				m_pBGImage->Render();
			if( m_pBGMPDI )
				m_pBGMPDI->Render();
			if( m_iWinIndex == 1 && m_pFailedMenuButtonList )
			{
				m_pFailedMenuButtonList->Render();
			}
		}
	}
	if( m_eGameStatus == eGS_CLAIRVOYANT_USE_CHOICE )
	{
		if( m_pBGImage )
			m_pBGImage->Render();
		if( m_pBGMPDI )
			m_pBGMPDI->Render();
		if( m_pUseClairvoyant )
			m_pUseClairvoyant->Render();
		if( m_pBattleRoundData )
			this->m_pBattleRoundData->Render();
	}
}

void	cBluffingDiceGame::DebugRender()
{
	m_pPlayerDiceTable->DebugRender();
	if( m_pCallImageButton )
		m_pCallImageButton->DebugRender();
	if( m_pCatchImageButton )
		m_pCatchImageButton->DebugRender();
	if( m_pAICallSubMPDI )
		m_pAICallSubMPDI->DebugRender();
	if( m_pPlayerCallSubMPDI )
		m_pPlayerCallSubMPDI->DebugRender();
	if( m_pAICatchSubMPDI )
		m_pAICatchSubMPDI->DebugRender();
	if( m_pPlayerCatchSubMPDI )
		m_pPlayerCatchSubMPDI->DebugRender();
	for(size_t i=0;i<m_AIDiceTableCallResultVector.size();++i)
	{
		//m_AIDiceTableCallResultVector[i]->DebugRender();
	}
	//if( m_pSpecialDiceRule )
	//	m_pSpecialDiceRule->DebugRender();
	if( m_iWinIndex == 0 )
	{//player
		m_pPlayerWin->DebugRender();
	}
	else
	if( m_iWinIndex == 1 )
	{
		if( m_pAIWin && !m_pAIWin->IsAnimationDone() )
			m_pAIWin->DebugRender();
		else
		{
			if( m_pFailedMenuButtonList )
			{
				m_pFailedMenuButtonList->DebugRender();
			}
		}
	}
}

bool	cBluffingDiceGame::IsCatchWin()
{
	for(size_t i=0;i<m_AIDiceTableCallResultVector.size();++i)
	{
		m_AIDiceTableCallResultVector[i]->GetDiceTable()->SetReveal(true);
	}
	return this->m_pBluffingDiceAI->IsCatchWin(m_iCallDiceCountIndex,m_iCallDicePoint,this->m_pSpecialDiceRule->m_bWorking);
}

void*	cBluffingDiceGame::GetData()
{
	return (void*)m_strVideoFileName.c_str();
}

void	cBluffingDiceGame::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	m_bLastStage = false;
	m_strVideoFileName = L"";
	m_strCurrentSelectedGirlNameAndStageName = L"";
	m_strUnlockGirlName = L"";
	cChoiceGirlStagePhase*l_pPhase = (cChoiceGirlStagePhase*)cBluffingGirlApp::GetPhase(CHOICE_GIRL_STAGE_PHASE_NAME);
	if( l_pPhase )
	{
		m_strCurrentSelectedGirlNameAndStageName = l_pPhase->m_strCurrentSelectedGirlNameAndStageName;
		const WCHAR*l_strCurrentSelectedGirlName = l_pPhase->m_strCurrentSelectedGirlName.c_str();
		cChoiceGirlStagePhase::cStageData*l_pStageData = 	l_pPhase->GetCurrentStageData();
		m_pBattleRoundData->m_iTotalRound = l_pStageData->m_iTotalRound;
		m_pBattleRoundData->m_iWinCount = l_pStageData->m_iWinCount;
		m_pStageData = l_pStageData;
		m_strBGFileName	= l_pStageData->m_strBGImage;
		if( l_pStageData->m_bLastStage )
		{
			//check all gir's final stage is unlocked and get newest unlock girl name
			cNodeISAX	l_AllStageFile;
			if(l_AllStageFile.ParseDataIntoXMLNode("BluffingGirl/PhaseData/AllStage.xml"))
			{
				//first finr current girl index
				TiXmlElement*l_pElemnt = l_AllStageFile.GetRootElement();
				int	l_iUnlockGirlIndex = 1;
				int	l_iLastStageClearCount = 0;
				std::wstring	l_strUnlockGirlName;
				if(l_pElemnt)
				{
					l_pElemnt = l_pElemnt->FirstChildElement();
					l_pElemnt = l_pElemnt->NextSiblingElement();
					while( l_pElemnt )
					{
						const WCHAR*l_strStageName = l_pElemnt->FirstAttribute()->Value();
						const WCHAR*l_strGirlName = l_pElemnt->Value();
						if( cStageClearFile::IsStageClear(l_strStageName) || !wcscmp(l_strGirlName,l_strCurrentSelectedGirlName) )
						{
							++l_iLastStageClearCount;
						}
						if(!cStageClearFile::IsGirlUnlock(l_strGirlName))
						{
							if( l_strUnlockGirlName.length() == 0 )
							{
								l_strUnlockGirlName = l_pElemnt->Value();
								break;
							}
						}
						if( l_strUnlockGirlName.length() == 0 )
							++l_iUnlockGirlIndex;
						l_pElemnt = l_pElemnt->NextSiblingElement();
					}
				}
				if( l_iUnlockGirlIndex-1 == l_iLastStageClearCount )
				{
					m_strUnlockGirlName = l_strUnlockGirlName;
				}
			}
		}
		m_strGameSetupFileName = UT::WcharToChar(l_pStageData->m_strGameSetupFileName);
		m_strVideoFileName = l_pStageData->m_strVideoName;
		m_bLastStage = l_pStageData->m_bLastStage;
	}
}



void	cBluffingDiceGame::UpdateUseChairvoyant(float e_fElpaseTime)
{
	if( m_eGameStatus != eGS_CLAIRVOYANT_USE_CHOICE )
		return;
	if( m_pUseClairvoyant )
	{
		m_pUseClairvoyant->Update(e_fElpaseTime);
		if( m_pUseClairvoyant->IsSatisfiedCondition() )
		{
			cBluffingGirlApp::m_spPlayerData->Init();
			if(m_pUseClairvoyant->GetCurrentWorkingObjectIndex() == 0)//yes
			{
				if( cBluffingGirlApp::m_spPlayerData->ClairvoyantCount() >0 )
				{
					cBluffingGirlApp::m_spPlayerData->UsingClairvoyant();
					m_eGameStatus = e_GAME_PLAY;
				}
				else
				{//play sound
					cBluffingGirlApp::m_spPlayerData->Init();
					m_pUseClairvoyant->Init();
					cMPDI*l_pBG = dynamic_cast<cMPDI*>(m_pUseClairvoyant->GetBG());
					l_pBG->Update(1.f);
					cBluffingGirlApp::WakePopupSystemMessenger(L"ClairvoyantNotEnough",this->GetName());
				}
			}
			else//1 no
			{
				m_eGameStatus = e_GAME_PLAY;
			}
		}
	}
	else
	{
		m_eGameStatus = e_GAME_PLAY;
	}

	if(m_eGameStatus == e_GAME_PLAY )
	{
		SAFE_DELETE(m_pShowMPDI);
		SAFE_DELETE(m_pResultMPDI);
		if((g_pbUseClairvoyant && *g_pbUseClairvoyant) || cBluffingGirlApp::m_spPlayerData->IsUsingClairvoyant())
		{
			m_pShowMPDI = new cMPDI(m_pShowUseClairvoyantMPDI);
			m_pResultMPDI = new cMPDI(m_pResultUseClairvoyantMPDI);
		}
		else
		{
			m_pShowMPDI = new cMPDI(m_pShowNotUseClairvoyantMPDI);
			m_pResultMPDI = new cMPDI(m_pResultNotUseClairvoyantMPDI);
		}
		m_pShowMPDI->Init();
		m_pShowMPDI->SetAnimationLoop(false);
		m_pShowMPDI->SetStayAtLastFrame(true);

		
		m_pResultMPDI->Init();
		m_pResultMPDI->Update(0.016f);
		m_pResultMPDI->SetAnimationLoop(false);
		m_pResultMPDI->SetStayAtLastFrame(true);
		//
		if( m_pBluffingDiceAI )
		{
			m_pBluffingDiceAI->SetCheatEnable(!cBluffingGirlApp::m_spPlayerData->IsUsingClairvoyant());
			if( g_pbUseClairvoyant )
				m_pBluffingDiceAI->SetCheatEnable(!*g_pbUseClairvoyant);
		}
		ChangeDiceShowImage();
		cGameApp::SoundPlay(L"BluffingGirl/Sound/DiceShaka.wav",true);
	}
}



void	cBluffingDiceGame::MouseDown(int e_iX,int e_iY)
{
	if( m_eGameStatus == eGS_CLAIRVOYANT_USE_CHOICE )
	{
		if( m_pUseClairvoyant )
			m_pUseClairvoyant->MouseDown(e_iX,e_iY);
	}
	if( (m_pShowMPDI && !m_pShowMPDI->IsAnimationDone()) || (m_AIDiceTableCallResultVector.size()>0&&m_AIDiceTableCallResultVector[0]->IsWaitForAI()) )
		return;
	if( m_eGameStatus == e_GAME_PLAY )
	{
		if( m_eCatchType == eCT_NO_CATCH )
		{
			if( m_iTurnToCallIndex == 0 )
			{
				m_pPlayerDiceTable->MouseDown(e_iX,e_iY);
			}
			if(m_pCatchImageButton && m_iCallDicePoint != -1 )
				m_pCatchImageButton->MouseDown(e_iX,e_iY);
			if( m_pCallImageButton )
				m_pCallImageButton->MouseDown(e_iX,e_iY);
			if(m_pShowRuleButton)
				m_pShowRuleButton->MouseDown(e_iX,e_iY);
		}
	}
	else
	if( m_eGameStatus == eGS_RESULT_VIDEO )
	{
		if( m_iWinIndex == 1 || !this->m_pVideo || m_pVideo->IsPlayDone() )
		{
			if( m_pAIWin && m_pAIWin->IsAnimationDone() )
			{
				if( m_pFailedMenuButtonList )
					m_pFailedMenuButtonList->MouseDown(e_iX,e_iY);
			}
		}
	}
}

void	cBluffingDiceGame::MouseMove(int e_iX,int e_iY)
{
	if( m_eGameStatus == eGS_CLAIRVOYANT_USE_CHOICE )
	{
		if( m_pUseClairvoyant )
			m_pUseClairvoyant->MouseMove(e_iX,e_iY);
	}
	if( (m_pShowMPDI && !m_pShowMPDI->IsAnimationDone()) || (m_AIDiceTableCallResultVector.size()>0&&m_AIDiceTableCallResultVector[0]->IsWaitForAI()) )
		return;
	if( m_eGameStatus == e_GAME_PLAY )
	{
		if( m_eCatchType == eCT_NO_CATCH )
		{
			if( m_iTurnToCallIndex == 0 )
			{
				if( m_pPlayerDiceTable )
					m_pPlayerDiceTable->MouseMove(e_iX,e_iY);
			}
			if(m_pCatchImageButton && m_iCallDicePoint != -1 )
				m_pCatchImageButton->MouseMove(e_iX,e_iY);
			if( m_pCallImageButton )
				m_pCallImageButton->MouseMove(e_iX,e_iY);
			if(m_pShowRuleButton)
				m_pShowRuleButton->MouseMove(e_iX,e_iY);
		}		
	}
	else
	if( m_eGameStatus == eGS_RESULT_VIDEO )
	{
		if( m_iWinIndex == 1 || !this->m_pVideo || m_pVideo->IsPlayDone() )
		{
			if( m_pAIWin && m_pAIWin->IsAnimationDone() )
			{
				if( m_pFailedMenuButtonList )
					m_pFailedMenuButtonList->MouseMove(e_iX,e_iY);
			}
		}
	}
}

void	cBluffingDiceGame::MouseUp(int e_iX,int e_iY)
{
	if( m_eGameStatus == eGS_CLAIRVOYANT_USE_CHOICE )
	{
		if( m_pUseClairvoyant )
			m_pUseClairvoyant->MouseUp(e_iX,e_iY);
	}
	if( (m_pShowMPDI && !m_pShowMPDI->IsAnimationDone()) || (m_AIDiceTableCallResultVector.size()>0&&m_AIDiceTableCallResultVector[0]->IsWaitForAI()) )
		return;
	if( m_eGameStatus == e_GAME_PLAY )
	{
		if( m_eCatchType == eCT_NO_CATCH )
		{
			if( m_iTurnToCallIndex == 0 )
			{
				m_pPlayerDiceTable->MouseUp(e_iX,e_iY);
			}
			if(m_pCatchImageButton && m_iCallDicePoint != -1 )
				m_pCatchImageButton->MouseUp(e_iX,e_iY);
			if( m_pCallImageButton )
				m_pCallImageButton->MouseUp(e_iX,e_iY);
			if(m_pShowRuleButton)
				m_pShowRuleButton->MouseUp(e_iX,e_iY);
		}
	}
	else
	if( m_eGameStatus == eGS_RESULT_VIDEO )
	{
		if( m_iWinIndex == 1 || !this->m_pVideo || m_pVideo->IsPlayDone() )
		{
			if( m_pAIWin && m_pAIWin->IsAnimationDone() )
			{
				if( m_pFailedMenuButtonList )
					m_pFailedMenuButtonList->MouseUp(e_iX,e_iY);
			}
		}
	}
}



const   WCHAR*	cBluffingDiceGame::GetNextPhaseName()
{
	return m_strNextPhaseName.c_str();
}


void	cBluffingDiceGame::KeyUp(char e_cKey)
{
#ifdef DEBUG
	if( e_cKey == 17 )
	{
		this->m_bSatisfiedCondition = true;
		m_strNextPhaseName = WIN_PRIZE_PHASE;
		cStageClearFile::StageClear(this->m_strCurrentSelectedGirlNameAndStageName.c_str());
		if( wcslen(m_strUnlockGirlName.c_str()) )
		{
			cStageClearFile::StageClear(m_strUnlockGirlName.c_str());
		}
	}
#endif
}
