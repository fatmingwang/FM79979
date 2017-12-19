#include "stdafx.h"
#include "BluffingDiceMultiPlayerGame.h"
#include "DiceTable.h"
#include "BluffingDiceAI.h"
#include "SpecialDiceRule.h"
#include "AllPhaseDefine.h"
#include "BluffingGirlApp.h"
#include "PlayerData.h"
#include "ChoiceGirlStagePhase.h"
#include "StageClearFile.h"
#include "GameNetwork.h"
#include "NetWorkConnectPhase.h"

cBluffingDiceMultiPlayerGame::cBluffingDiceMultiPlayerGame()
{
	this->SetName(BLUFFING_MULTI_PLAY_DICEGAME_PHASE_NAME);
	m_strNextPhaseName = TITLE_PHASE_NAME;
	m_NetworkErrorTC.SetTargetTime(3.f);
	m_pWhoAmIMPDI = 0;
	m_pPleaseCallMPDI = 0;
	m_iWhichPlayerIsBluffing = -1;
	m_iWhichPlayerCatch = -1;
	m_pOrtherPlayerCallIndex = 0;
	m_pOrtherPlayerCallDicePoint = 0;
	m_pOrtherPlayerCallDiceCount = 0;
	m_pOrtherPlayerCallDiceMPDI = 0;
	m_bReceivedCatchMessage = false;
	m_pWaitingPlayerRespondMPDI = 0;
	m_iMyIndexOfPlayers = 0;
	m_pShowMPDI = 0;
	m_pResultMPDI = 0;
	m_pBGMPDI = 0;
	m_strGameSetupFileName = "BluffingGirl/PhaseData/MultiPlayerSetup.xml";
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
}
cBluffingDiceMultiPlayerGame::~cBluffingDiceMultiPlayerGame()
{
	Destroy();
}

void	cBluffingDiceMultiPlayerGame::Destroy()
{
	m_pBGMPDI = 0;
	SAFE_DELETE(m_pOrtherPlayerCallDiceMPDI);
	SAFE_DELETE(m_pPlayerWin);
	SAFE_DELETE(m_pAIWin);
	SAFE_DELETE(m_pResultMPDI);
	SAFE_DELETE(m_pShowMPDI);
	SAFE_DELETE(m_pCallImageButton);
	SAFE_DELETE(m_pCatchImageButton);
	SAFE_DELETE(m_pWhoAmIMPDI);
	m_pPlayerDiceTable = 0;
	if( m_pBluffingDiceAI )
	{
		std::vector<cDiceTable*>*l_pTable = m_pBluffingDiceAI->GetDiceTableVector();
		while(l_pTable->size())
		{
			cDiceTable*l_pDiceTable = (*l_pTable)[0];
			SAFE_DELETE(l_pDiceTable);
			l_pTable->erase(l_pTable->begin());
		}
	}
	SAFE_DELETE(m_pBluffingDiceAI);
	SAFE_DELETE(m_pDiceRule);
	
	SAFE_DELETE(m_pSpecialDiceRule);
	m_ResourceStamp.ReleaseNewResource();
	cGameApp::m_spSoundParser->RemoveAllBGM();
}
//<Root>
//	<DiceRule Min="1" Max="7" OneDiceRule="1" PercentForSpecialNumeral1="0.1" NumDice="5"/>
//	<DiceTableData FileName="">
//	<AIData WinCallPercentage="0.6" SkipStep="1,2" VisibleDice="5,6" />
//</Root>
void	cBluffingDiceMultiPlayerGame::HandleElementData(TiXmlElement*e_pTiXmlElement)
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
	}
	else
	if(!wcscmp(l_strValue,L"AIThinkingMPDI"))
	{
		m_pWaitingPlayerRespondMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		if( m_pWaitingPlayerRespondMPDI )
		{
			m_pWaitingPlayerRespondMPDI->Init();
			m_pWaitingPlayerRespondMPDI->SetAnimationLoop(true);
		}
	}
	else
	COMPARE_VALUE("WhoIAm")
	{
		SAFE_DELETE(m_pWhoAmIMPDI);
		m_pWhoAmIMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement());
		cSubMPDI*l_pSubMPDI = m_pWhoAmIMPDI->GetObject(1);
		if( l_pSubMPDI )
		{
			cPuzzleImage*l_pPI = l_pSubMPDI->GetPuzzleImage();
			std::wstring	l_strPlayerIndex = L"player_0";
			l_strPlayerIndex += ValueToStringW(this->m_iMyIndexOfPlayers+1);
			int	l_iIndex = l_pPI->GetObjectIndexByName(l_strPlayerIndex.c_str());
			l_pSubMPDI->ChangeAllImageByImageIndex(l_iIndex);
			m_pWhoAmIMPDI->Init();
			m_pWhoAmIMPDI->Update(0.016f);
		}
	}
	else
	COMPARE_VALUE("OrtherPlayerCallMPDI")
	{
		SAFE_DELETE(m_pOrtherPlayerCallDiceMPDI);
		m_pOrtherPlayerCallDiceMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement());
		m_pOrtherPlayerCallDiceMPDI->SetAnimationDone(true);
		m_pOrtherPlayerCallIndex = m_pOrtherPlayerCallDiceMPDI->GetObject(L"player_01");
		m_pOrtherPlayerCallDiceCount = m_pOrtherPlayerCallDiceMPDI->GetObject(L"Color_Number_06");;
		m_pOrtherPlayerCallDicePoint = m_pOrtherPlayerCallDiceMPDI->GetObject(L"Dice_Color_003");;
	}
}

void	cBluffingDiceMultiPlayerGame::ProcessDiceRule(TiXmlElement*e_pTiXmlElement)
{
	assert(m_pDiceRule == 0);
	m_pDiceRule = new sDiceRule(e_pTiXmlElement);
	m_pDiceRule->iAllDiceCount = (m_iIAmReadyMessageVector.size()*5);
	//m_pDiceRule->iAllDiceCount = 20;
	
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
void	cBluffingDiceMultiPlayerGame::ProcessDiceTableData(TiXmlElement*e_pTiXmlElement)
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
				while( l_pTiXmlElement )
				{
					if( !wcscmp(l_pTiXmlElement->Value(),L"DiceTableData") )
					{
						const WCHAR*l_strType = l_pTiXmlElement->Attribute(L"Type");
						if( l_strType &&!wcscmp(l_strType,L"Player") )
						{
							std::vector<cDiceTable*>	l_AllDiceTable;
							assert( m_pPlayerDiceTable == 0 &&"player data exists");
							//79979
							size_t	l_Size = m_iIAmReadyMessageVector.size();
							for( size_t i=0;i<l_Size;++i )
							{
								cDiceTable*l_pPlayerDiceTable = new cDiceTable(l_pTiXmlElement->FirstChildElement(),m_pDiceRule,eBT_PLAYER);
								l_AllDiceTable.push_back(l_pPlayerDiceTable);
								l_pPlayerDiceTable->SetReveal(true);
								if( m_iMyIndexOfPlayers == i )
								{
									m_pPlayerDiceTable = l_pPlayerDiceTable;
								}
							}
							m_pBluffingDiceAI->AssignTableData(&l_AllDiceTable);
						}
					}
					l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
				}
			}
		}
	}
	else
	{
		UT::ErrorMsg(L"ProcessDiceTableData error",L"no FileName attribute");
	}
}

void	cBluffingDiceMultiPlayerGame::ProcessDiceAIData(TiXmlElement*e_pTiXmlElement)
{
	assert(m_pBluffingDiceAI==0);
	assert(m_pDiceRule);
	m_pBluffingDiceAI = new cBluffingDiceAI(e_pTiXmlElement,m_pDiceRule);
}

//CallLegalSound="CallClick.wav" CallInlegalSound="CallClickInlegal.wav"
void	cBluffingDiceMultiPlayerGame::ProcessSoundData(TiXmlElement*e_pTiXmlElement)
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
void	cBluffingDiceMultiPlayerGame::ProcessImageData(TiXmlElement*e_pTiXmlElement)
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
					m_pPlayerWin = new cMPDI(l_pMPDILIst->GetObject(l_strValue));
				}
				else
				COMPARE_NAME("LoseMPDI")
				{
					m_pAIWin = new cMPDI(l_pMPDILIst->GetObject(l_strValue));
				}
			PARSE_NAME_VALUE_END
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
				{
					UT::ErrorMsg(L"this button is not using",l_pImageButton->GetName());
					SAFE_DELETE(l_pImageButton);
				}
			}

		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"ShowMPDI"))
		{
			cMPDI*l_pShowNotUseClairvoyantMPDI = cMPDI::GetMe(GetXmlElementByNameFromElement(L"NoUseClairvoyant",e_pTiXmlElement)->FirstChildElement(),false);
			SAFE_DELETE(m_pShowMPDI);
			m_pShowMPDI = new cMPDI(l_pShowNotUseClairvoyantMPDI);
			m_pShowMPDI->Init();
			m_pShowMPDI->SetAnimationLoop(false);
			m_pShowMPDI->SetStayAtLastFrame(true);
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"ResultMPDI"))
		{
			cMPDI*l_pResultNotUseClairvoyantMPDI = cMPDI::GetMe(GetXmlElementByNameFromElement(L"NoUseClairvoyant",e_pTiXmlElement)->FirstChildElement(),false);
			SAFE_DELETE(m_pResultMPDI);
			m_pResultMPDI = new cMPDI(l_pResultNotUseClairvoyantMPDI);
			m_pResultMPDI->Init();
			m_pResultMPDI->SetAnimationLoop(false);
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"BG"))
		{
			m_pBGMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
			if( m_pBGMPDI )
			{
				m_pBGMPDI->Init();
				m_pBGMPDI->Update(0.016f);
			}
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"PleaseCallMPDI"))
		{
			m_pPleaseCallMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
			if( m_pPleaseCallMPDI )
			{
				m_pPleaseCallMPDI->Init();
			}
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
}

void	cBluffingDiceMultiPlayerGame::Init()
{
	this->Destroy();
	m_NetworkErrorTC.Start();
	this->m_iTurnToCallIndex = 0;
	m_iWhichPlayerCatch = -1;
	m_iWhichPlayerIsBluffing = -1;
	m_pOrtherPlayerCallDicePoint = 0;
	m_pOrtherPlayerCallDiceCount = 0;
	m_pOrtherPlayerCallDiceMPDI = 0;
	m_pPleaseCallMPDI = 0;
	m_eGameStatus = e_WAITING_START;
	m_bReceivedCatchMessage = false;
	m_ResourceStamp.StampResource1();
	//
	m_iCallDicePoint = -1;
	m_iCallDiceCountIndex = 0;
	m_eCatchType = eCT_NO_CATCH;
	if(!this->Parse(m_strGameSetupFileName.c_str()))
		UT::ErrorMsg(m_strGameSetupFileName.c_str(),"parse failed!");
	if( m_pPlayerDiceTable )
		m_pPlayerDiceTable->Init();
	if( m_pBluffingDiceAI )
	{
		std::vector<cDiceTable*>*l_pDiceTableVector = this->m_pBluffingDiceAI->GetDiceTableVector();
		for( size_t i=0;i<l_pDiceTableVector->size();++i )
		{
			cDiceTable*l_pPlayerDiceTable = (*l_pDiceTableVector)[i];
			std::vector<int>	l_iCurrentDiceTable;
			std::wstring	l_strOutputString = L"Player:";
			l_strOutputString += ValueToStringW(i);
			l_strOutputString += L"DiceTable:";
			for(int j=0;j<m_pDiceRule->iNumDice;++j)
				l_iCurrentDiceTable.push_back(cBluffingGirlApp::m_spGameNetwork->m_DiceTable[i*this->m_pDiceRule->iNumDice+j]);
			l_strOutputString += ValueToStringW<int>(l_iCurrentDiceTable);
			l_strOutputString += L"\n";
			cGameApp::OutputDebugInfoString(l_strOutputString);
			l_pPlayerDiceTable->SetDiceTable(l_iCurrentDiceTable);
		}
	}
	ChangeDiceShowImage();
	ChangeDiceResultImage();
	//
	//std::vector<int> l_test;
	//for( int i=0;i<5;++i )
	//	l_test.push_back(0);
	//m_pPlayerDiceTable->SetDiceTable(l_test);
	//
	if( m_pCallImageButton )
		m_pCallImageButton->Init();
	if( m_pCatchImageButton )
		m_pCatchImageButton->Init();
	if( m_pPlayerWin )
		m_pPlayerWin->Init();
	if( m_pAIWin )
		m_pAIWin->Init();
	//
	//
	m_ResourceStamp.StampResource2();
	cGameApp::m_sTimeAndFPS.Update();
	if( this->m_iMyIndexOfPlayers != 0 )
		SendMessage(eGMI_TO_SERVER_I_AM_READY);
}

void	cBluffingDiceMultiPlayerGame::DoCatch(bool e_bMyCall)
{
	m_eCatchType = e_bMyCall?eCT_PLAYER_CATCH:eCT_AI_CATCH;
	cGameApp::SoundPlay(BUTTON_CLICK_SOUND,true);
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
	cMPDI*l_pMPDI = 0;
	if( IsCatchWin() )
	{
		m_iWinIndex = 0;
		l_pMPDI = m_pPlayerWin;
	}
	else
	{
		m_iWinIndex = 1;
		l_pMPDI = m_pAIWin;
	}
	if(l_pMPDI)
	{
		cSubMPDI*l_pSubMPDI1 = l_pMPDI->GetObject(L"player_01");
		cSubMPDI*l_pSubMPDI2 = l_pMPDI->GetObject(L"player_02");
		if( l_pSubMPDI1 )
		{
			cPuzzleImage*l_pPI = l_pSubMPDI1->GetPuzzleImage();
			std::wstring	l_strPlayerIndex = L"player_0";
			l_strPlayerIndex += ValueToStringW(this->m_iWhichPlayerCatch+1);
			int	l_iIndex = l_pPI->GetObjectIndexByName(l_strPlayerIndex.c_str());
			l_pSubMPDI1->ChangeAllImageByImageIndex(l_iIndex);
		}
		if( l_pSubMPDI2 )
		{
			cPuzzleImage*l_pPI = l_pSubMPDI2->GetPuzzleImage();
			std::wstring	l_strPlayerIndex = L"player_0";
			l_strPlayerIndex += ValueToStringW(this->m_iWhichPlayerIsBluffing+1);
			int	l_iIndex = l_pPI->GetObjectIndexByName(l_strPlayerIndex.c_str());
			l_pSubMPDI2->ChangeAllImageByImageIndex(l_iIndex);
		}
	}
}

void	cBluffingDiceMultiPlayerGame::TurnIndexChange(int e_iPlayerIndex)
{
	m_iTurnToCallIndex = e_iPlayerIndex;
	if( m_iTurnToCallIndex >= (int)m_iIAmReadyMessageVector.size() )
		m_iTurnToCallIndex = 0;
	cSubMPDI*l_pSubMPDI = m_pWaitingPlayerRespondMPDI->GetObject(m_pWaitingPlayerRespondMPDI->Count()-1);
	std::wstring	l_strPlayerIndex = L"player_0";
	l_strPlayerIndex += ValueToStringW(this->m_iTurnToCallIndex+1);
	int	l_iIndex = l_pSubMPDI->GetPuzzleImage()->GetObjectIndexByName(l_strPlayerIndex.c_str());
	l_pSubMPDI->ChangeAllImageByImageIndex(l_iIndex);
}

void	cBluffingDiceMultiPlayerGame::UpdateDiceCall(float e_fElpaseTime)
{
	if( m_iTurnToCallIndex == m_iMyIndexOfPlayers )
	{
		if( m_pPlayerDiceTable )
			m_pPlayerDiceTable->Update(e_fElpaseTime);
		if( m_pPleaseCallMPDI )
			m_pPleaseCallMPDI->Update(e_fElpaseTime);
		if( m_pCallImageButton)
		{
			m_pCallImageButton->Update(e_fElpaseTime);
			if( m_pCallImageButton->IsSatisfiedCondition() )
			{
				cGameApp::SoundPlay(BUTTON_CLICK_SOUND,true);
				m_pCallImageButton->Init();
				if( m_pPlayerDiceTable->m_NumeralImageButton.GetCurrentWorkingObject() && 
					m_pPlayerDiceTable->m_DiceImageButtonContainer.GetCurrentWorkingObject() && 
					m_pPlayerDiceTable->IsConditionOk(m_iCallDicePoint,m_iCallDiceCountIndex,m_iIAmReadyMessageVector.size()) )
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
					TurnIndexChange(this->m_iMyIndexOfPlayers+1);
					if( m_pPlayerDiceTable->GetCurrentSelectDice() == m_pSpecialDiceRule->m_iSpecialDicePoint )
						this->m_pSpecialDiceRule->m_bWorking = false;
					int	l_iCount = m_pPlayerDiceTable->GetCurrentSelectNumeral();
					int	l_iPoint = m_pPlayerDiceTable->GetCurrentSelectDice();
					CallDiceImageUpdate(l_iCount,l_iPoint,true,-1);
					this->SendMessage(eGMI_TO_SERVER_CALL,this->m_iMyIndexOfPlayers,l_iCount,l_iPoint);
				}
				else
				{
					cGameApp::SoundPlay(BUTTON_CLICK_WRONG_SOUND,true);
				}
			}
		}
		if( m_pCatchImageButton )
		{
			m_pCatchImageButton->Update(e_fElpaseTime);
			if( m_pCatchImageButton->IsSatisfiedCondition() )
			{
				m_pCatchImageButton->Init();
				if( m_iCallDiceCountIndex != 0 )
				{
					m_iWhichPlayerCatch = m_iMyIndexOfPlayers;
					m_iWhichPlayerIsBluffing = this->m_iTurnToCallIndex;
					if( m_iWhichPlayerIsBluffing == m_iMyIndexOfPlayers )
					{
						m_iWhichPlayerIsBluffing -= 1;
						if( m_iWhichPlayerIsBluffing < 0  )
						{
							m_iWhichPlayerIsBluffing = m_iIAmReadyMessageVector.size()-1;
						}
					}
					DoCatch(this->m_iTurnToCallIndex == this->m_iMyIndexOfPlayers);
					this->SendMessage(eGMI_TO_SERVER_CATCH);
				}
				else
				{
					cGameApp::SoundPlay(BUTTON_CLICK_WRONG_SOUND,true);
				}
			}
		}
	}
	else
	{
	}
}

void	cBluffingDiceMultiPlayerGame::ChangeDiceResultImage()
{
	if( m_pResultMPDI && m_pPlayerDiceTable)
	{
		size_t	l_siDiceCount = cBluffingGirlApp::m_spGameNetwork->m_DiceTable.size();
		std::vector<int>l_DiceTable = cBluffingGirlApp::m_spGameNetwork->m_DiceTable;
		cPuzzleImage*l_pP = m_pResultMPDI->GetObject(L"Dice_On_01")->GetPuzzleImage();
		const char*l_strPrefixName= "Dice_On_";
		for( size_t i=0;i<l_siDiceCount;++i )
		{
			std::wstring	l_strImageName =  m_pPlayerDiceTable->m_strMyDiceImagePrefixName.c_str();
			l_strImageName += ValueToStringW(l_DiceTable[i]+1);
			int	l_iDiceImageIndex = l_pP->GetObjectIndexByName(l_strImageName.c_str());
			if( l_iDiceImageIndex == -1 )
			{
				int a=0;
			}
			char	l_strTemp[MAX_PATH];
			sprintf(l_strTemp,"%s%.2d",l_strPrefixName,i+1);
			std::wstring	l_strTargetName = UT::CharToWchar(l_strTemp);
			const WCHAR*lll = l_strTargetName.c_str();
			cGameApp::OutputDebugInfoString(lll);
			m_pResultMPDI->GetObject(lll)->ChangeAllImageByImageIndex(l_iDiceImageIndex);
		}
		for( int i=l_siDiceCount;i<4*this->m_pDiceRule->iNumDice;++i )
		{
			char	l_strTemp[MAX_PATH];
			sprintf(l_strTemp,"%s%.2d",l_strPrefixName,i+1);
			std::wstring	l_strTargetName = UT::CharToWchar(l_strTemp);
			const WCHAR*lll = l_strTargetName.c_str();
			if(!m_pResultMPDI->RemoveObject(lll))
			{
				int a=0;	
			}
		}
		for( size_t i=m_iIAmReadyMessageVector.size();i<4;++i )
		{
			char	l_strTEMP[MAX_PATH];
			sprintf(l_strTEMP,"Final_Check_0%d",i+1);
			if(!m_pResultMPDI->RemoveObject(UT::CharToWchar(l_strTEMP).c_str()))
			{
				int a=0;
			}
		}
	}
}

void	cBluffingDiceMultiPlayerGame::ChangeDiceShowImage()
{
	if( m_pShowMPDI && m_pPlayerDiceTable )
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
				l_iDicePoint = this->m_pPlayerDiceTable->m_MyDiceTable[i-8];
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

void	cBluffingDiceMultiPlayerGame::UpdateDiceCatch(float e_fElpaseTime)
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
		{//player0
			if( m_pPlayerWin && !m_pPlayerWin->IsAnimationDone() )
				m_pPlayerWin->Update(e_fElpaseTime);
			if( m_pPlayerWin->IsAnimationDone() )
			{
				this->m_bSatisfiedCondition = true;
				m_strNextPhaseName = TITLE_PHASE_NAME;
			}
		}
		else
		{
			if( m_pAIWin && !m_pAIWin->IsAnimationDone() )
				m_pAIWin->Update(e_fElpaseTime);
			if( m_pAIWin->IsAnimationDone() )
			{
				this->m_bSatisfiedCondition = true;
				m_strNextPhaseName = TITLE_PHASE_NAME;
			}
		}
	}
}

void	cBluffingDiceMultiPlayerGame::SendMessage(int e_iMessageID,int e_iWhichPlayerSentIndex,int e_iDiceCount,int e_iDicePoint)
{
	sGamePacket	l_GamePacket;
	l_GamePacket.SetMessageID(e_iMessageID);
	switch(e_iMessageID)
	{
		case eGMI_TO_CLIENT_NETWORK_ERROR:
			cBluffingGirlApp::m_spGameNetwork->SendDataToAllClient(&l_GamePacket,sizeof(int));
		break;
		case eGMI_TO_SERVER_I_AM_READY:
			l_GamePacket.SetPlayerIndex(this->m_iMyIndexOfPlayers);
			cBluffingGirlApp::m_spGameNetwork->SendDataToServer(&l_GamePacket,sizeof(int)+sizeof(int));
			break;
		case eGMI_TO_CLIENT_PLAYER_TURN:
			l_GamePacket.SetPlayerIndex(this->m_iTurnToCallIndex);
			cBluffingGirlApp::m_spGameNetwork->SendDataToAllClient(&l_GamePacket,sizeof(int));
			break;
		case eGMI_TO_SERVER_CALL:
			{
				//haha...sux here,I am lazy to improve this.
				int	l_iCount = m_pPlayerDiceTable->GetCurrentSelectNumeral();
				int	l_iPoint = m_pPlayerDiceTable->GetCurrentSelectDice();
				if( e_iDiceCount != -1 )
					l_iCount = e_iDiceCount;
				if( e_iDicePoint != -1 )
					l_iPoint = e_iDicePoint;
				l_GamePacket.SetDiceCount(l_iCount);
				l_GamePacket.SetDicePoint(l_iPoint);
				l_GamePacket.SetPlayerIndex(e_iWhichPlayerSentIndex);
				m_eGameStatus = e_WAITING_PLAYER_DICE_DATA;
			}
			if( this->m_iMyIndexOfPlayers == 0 )
			{
				cBluffingGirlApp::m_spGameNetwork->SendDataToAllClient(&l_GamePacket,sizeof(int)*4);
			}
			else
			{
				cBluffingGirlApp::m_spGameNetwork->SendDataToServer(&l_GamePacket,sizeof(int)*4);
			}
			break;
		case eGMI_TO_SERVER_CATCH:
			l_GamePacket.SetPlayerIndex(m_iMyIndexOfPlayers);
			l_GamePacket.SetWhichPlayerIsBluffing(this->m_iWhichPlayerIsBluffing);
			if( this->m_iMyIndexOfPlayers == 0 )
			{
				cBluffingGirlApp::m_spGameNetwork->SendDataToAllClient(&l_GamePacket,sizeof(int)*3);
			}
			else
			{
				cBluffingGirlApp::m_spGameNetwork->SendDataToServer(&l_GamePacket,sizeof(int)*3);
			}
			break;
	}
}

void	cBluffingDiceMultiPlayerGame::ProcessNetworkMessage(float e_fElpaseTime)
{
	if( m_bReceivedCatchMessage )
		return;
	if( this->m_iMyIndexOfPlayers == 0 )
	{
		size_t	l_iConnectClientCount = cBluffingGirlApp::m_spGameNetwork->ClientCount();
		if(l_iConnectClientCount != m_iIAmReadyMessageVector.size()-1 )
		{
			m_eGameStatus = e_NETWORK_ERROR;
			SendMessage(eGMI_TO_CLIENT_NETWORK_ERROR);
			m_bReceivedCatchMessage = true;
		}
	}
	else
	if(cBluffingGirlApp::m_spGameNetwork->IsConnectionFailed())
	{
		m_eGameStatus = e_NETWORK_ERROR;
		m_bReceivedCatchMessage = true;
		return;
	}
	cBluffingGirlApp::m_spGameNetwork->Update(e_fElpaseTime);
	cFUSynchronizedHold hold(&cBluffingGirlApp::m_spGameNetwork->m_ReceivedData);
	while(cBluffingGirlApp::m_spGameNetwork->m_ReceivedData.size()>0)
	{
		sReceivedPacket*l_pReceivedPacket = cBluffingGirlApp::m_spGameNetwork->m_ReceivedData[0];
		sGamePacket*l_pGamePacket = (sGamePacket*)l_pReceivedPacket->pData;
		if(l_pGamePacket->GetMessageID() == eGMI_TO_SERVER_I_AM_READY )
		{
			int	l_iPlayerIndex = l_pGamePacket->GetPlayerIndex();
			if( l_iPlayerIndex >-1 && l_iPlayerIndex < (int)m_iIAmReadyMessageVector.size() )
			m_iIAmReadyMessageVector[l_iPlayerIndex] = 1;
		}
		else
		if(l_pGamePacket->GetMessageID() == eGMI_TO_CLIENT_PLAYER_TURN )
		{
			int l_iPlayerIndex = l_pGamePacket->GetPlayerIndex();
			this->m_iTurnToCallIndex = l_iPlayerIndex;
			if( m_iTurnToCallIndex == this->m_iMyIndexOfPlayers )
			{
				m_eGameStatus = e_GAME_PLAY;
			}
			else
				m_eGameStatus = e_WAITING_PLAYER_DICE_DATA;
		}
		else
		if(l_pGamePacket->GetMessageID() == eGMI_TO_SERVER_CALL )
		{
			if( m_pAICallSubMPDI )
			{
				m_pAICallSubMPDI->Init();
				m_pAICallSubMPDI->Update(0.001f);
			}
			if( m_pPlayerCallSubMPDI )
				m_pPlayerCallSubMPDI->SetAnimationDone(true);
			int	l_iDiceCount = l_pGamePacket->GetDiceCount();
			int	l_iDicePoint = l_pGamePacket->GetDicePoint();
			int	l_iPlayerIndex = l_pGamePacket->GetPlayerIndex();
			if( l_iPlayerIndex != this->m_iMyIndexOfPlayers )
			{
				if( l_iDicePoint == m_pSpecialDiceRule->m_iSpecialDicePoint )
					this->m_pSpecialDiceRule->m_bWorking = false;
				CallDiceImageUpdate(l_iDiceCount,l_iDicePoint,false,l_iPlayerIndex);
				if( this->m_iMyIndexOfPlayers == 0 )
				{
					this->SendMessage(eGMI_TO_SERVER_CALL,l_iPlayerIndex,l_iDiceCount,l_iDicePoint);
				}
				TurnIndexChange(l_iPlayerIndex+1);
				if( m_iTurnToCallIndex == m_iMyIndexOfPlayers )
					m_eGameStatus = e_GAME_PLAY;
			}
		}
		else			
		if(l_pGamePacket->GetMessageID() == eGMI_TO_SERVER_CATCH )
		{
			m_bReceivedCatchMessage = true;
			int	l_iCatchPlayerIndex = l_pGamePacket->GetPlayerIndex();
			int	l_iWhichPlayerIsBluffing = l_pGamePacket->GetWhichPlayerIsBluffing();
			m_iWhichPlayerCatch = l_iCatchPlayerIndex;
			m_iWhichPlayerIsBluffing = l_iWhichPlayerIsBluffing;
			if( this->m_iMyIndexOfPlayers != l_iCatchPlayerIndex )
			{
				if( m_iMyIndexOfPlayers == 0 )
				{
					sGamePacket	l_GamePacket;
					l_GamePacket.SetMessageID(eGMI_TO_SERVER_CATCH);
					l_GamePacket.SetPlayerIndex(l_iCatchPlayerIndex);
					l_GamePacket.SetWhichPlayerIsBluffing(l_iWhichPlayerIsBluffing);
					cBluffingGirlApp::m_spGameNetwork->SendDataToAllClient(&l_GamePacket,sizeof(int)*3);
				}
				this->DoCatch(m_iMyIndexOfPlayers == l_iCatchPlayerIndex?true:false);
			}
		}
		else
		if(l_pGamePacket->GetMessageID() == eGMI_TO_CLIENT_NETWORK_ERROR )
		{
			m_eGameStatus = e_NETWORK_ERROR;
			m_bReceivedCatchMessage = true;		
		}
		cBluffingGirlApp::m_spGameNetwork->m_ReceivedData.erase(cBluffingGirlApp::m_spGameNetwork->m_ReceivedData.begin());
		delete l_pReceivedPacket;
	}
}

void	cBluffingDiceMultiPlayerGame::Update(float e_fElpaseTime)
{
	if(m_eGameStatus == e_NETWORK_ERROR )
	{
		m_NetworkErrorTC.Update(e_fElpaseTime);
		if( m_NetworkErrorTC.bTragetTimrReached )
		{
			this->m_bSatisfiedCondition = true;
		}
		return;
	}
	ProcessNetworkMessage(e_fElpaseTime);
	if( m_eGameStatus == e_GAME_PLAY || m_eGameStatus == e_WAITING_PLAYER_DICE_DATA )
	{
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
		if( m_eGameStatus == e_WAITING_PLAYER_DICE_DATA && m_pWaitingPlayerRespondMPDI && !m_bReceivedCatchMessage && m_pShowMPDI->IsAnimationDone() )
		{
			m_pWaitingPlayerRespondMPDI->Update(e_fElpaseTime);
		}
	}
	else
	if( m_eGameStatus == e_WAITING_START && this->m_iMyIndexOfPlayers == 0)
	{
		bool	l_bAllReady = true;
		for( size_t i=0;i<m_iIAmReadyMessageVector.size();++i )
		{
			if( m_iIAmReadyMessageVector[i] == 0)
				l_bAllReady = false;
		}
		if( l_bAllReady )
		{
			if( this->m_iMyIndexOfPlayers == 0 )
			{
				m_eGameStatus = e_GAME_PLAY;
				SendMessage(eGMI_TO_CLIENT_PLAYER_TURN);
			}
			else
			{
				m_eGameStatus = e_WAITING_PLAYER_DICE_DATA;
			}
		}
	}

	if(this->m_bSatisfiedCondition&& ( wcscmp(STORE_PHASE_NAME,m_strNextPhaseName.c_str()) && wcscmp(RULE_PHASE_NAME,m_strNextPhaseName.c_str()) ))
	{
		Destroy();
		if(!wcscmp(TITLE_PHASE_NAME,m_strNextPhaseName.c_str()))
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

void	cBluffingDiceMultiPlayerGame::CallDiceImageUpdate(int e_iCount,int e_iPoint,bool e_bPlayer,int e_iTurnIndex)
{
	std::wstring	l_str = e_bPlayer?L"PlayerCall:":L"AICall:";
	l_str += L"DiceCount:"; l_str += ValueToStringW(e_iCount);
	l_str += L"DicePoint:"; l_str += ValueToStringW(e_iPoint);
	cGameApp::WriteLog(l_str.c_str());
	m_iCallDiceCountIndex = e_iCount;
	m_iCallDicePoint = e_iPoint;
	if( !e_bPlayer )
	{
		if( m_pOrtherPlayerCallIndex )
		{
			std::wstring	l_str = L"player_0";
			l_str += ValueToStringW(e_iTurnIndex+1);
			int	l_iImageIndex = m_pOrtherPlayerCallIndex->GetPuzzleImage()->GetObjectIndexByName(l_str.c_str());
			m_pOrtherPlayerCallIndex->ChangeAllImageByImageIndex(l_iImageIndex);
		}
		if( m_pOrtherPlayerCallDiceCount )
		{
			char	l_strTemp[MAX_PATH];
			sprintf(l_strTemp,"Color_Number_%.2d",e_iCount+1);
			//std::wstring	l_str = L"Color_Number_";
			//l_str += ValueToStringW(e_iCount+1);
			int	l_iImageIndex = m_pOrtherPlayerCallDiceCount->GetPuzzleImage()->GetObjectIndexByName(UT::CharToWchar(l_strTemp).c_str());
			m_pOrtherPlayerCallDiceCount->ChangeAllImageByImageIndex(l_iImageIndex);
		}
		if( m_pOrtherPlayerCallDicePoint )
		{
			std::wstring	l_str = L"Dice_Color_00";
			l_str += ValueToStringW(e_iPoint+1);
			int	l_iImageIndex = m_pOrtherPlayerCallDicePoint->GetPuzzleImage()->GetObjectIndexByName(l_str.c_str());
			m_pOrtherPlayerCallDicePoint->ChangeAllImageByImageIndex(l_iImageIndex);
		}

		m_pOrtherPlayerCallDiceMPDI->Init();
		m_pOrtherPlayerCallDiceMPDI->Update(0.8f);
	}
	if( e_bPlayer )
	{
		m_pPlayerDiceTable->ChangeCallData(e_iCount,e_iPoint);
	}
}

void	cBluffingDiceMultiPlayerGame::Render()
{
	if(m_eGameStatus == e_NETWORK_ERROR )
	{
		//cGameApp::m_spGlyphFontRender->SetScale(.f);
		cGameApp::RenderFont(250,600,L"Net Work Error!!");
		//cGameApp::m_spGlyphFontRender->SetScale(1.f);
	}
	else
	if( m_eGameStatus == eGS_DICE_SHAKE )
	{

	}
	else
	if( m_eGameStatus == e_GAME_PLAY || m_eGameStatus == e_WAITING_PLAYER_DICE_DATA || m_eGameStatus == e_WAITING_START )
	{
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
		if( m_pSpecialDiceRule )
			m_pSpecialDiceRule->Render();
		if( m_pWhoAmIMPDI )
			m_pWhoAmIMPDI->Render();
		if(m_eCatchType != eCT_NO_CATCH  )
		{
			//this->m_pPlayerDiceTable->ClickButtonRender();
			if( m_pResultMPDI)
			{
				m_pResultMPDI->Render();
			}
			if(!m_pResultMPDI || m_pResultMPDI->IsAnimationDone() )
			{
				if( m_iWinIndex == 0 )
				{//player
					if( m_pPlayerWin )
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
			if( m_pOrtherPlayerCallDiceMPDI )
				m_pOrtherPlayerCallDiceMPDI->Render();
			if( m_pPleaseCallMPDI && m_iTurnToCallIndex == m_iMyIndexOfPlayers )
				m_pPleaseCallMPDI->Render();
		}
		if(m_eGameStatus == e_WAITING_PLAYER_DICE_DATA && !m_bReceivedCatchMessage )
		{
			if( m_pWaitingPlayerRespondMPDI )
			{
				m_pWaitingPlayerRespondMPDI->Render();
			}
		}
		else
		if( m_eGameStatus == e_WAITING_START && !m_bReceivedCatchMessage )
		{

		}
	}
}

void	cBluffingDiceMultiPlayerGame::DebugRender()
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
	}
}

bool	cBluffingDiceMultiPlayerGame::IsCatchWin()
{
	return this->m_pBluffingDiceAI->IsCatchWin(m_iCallDiceCountIndex,m_iCallDicePoint,this->m_pSpecialDiceRule->m_bWorking);
}

void	cBluffingDiceMultiPlayerGame::MouseDown(int e_iX,int e_iY)
{
	if( m_pShowMPDI && !m_pShowMPDI->IsAnimationDone())
		return;
	if( m_eGameStatus == e_GAME_PLAY )
	{
		if( m_eCatchType == eCT_NO_CATCH )
		{
			if( m_iTurnToCallIndex == m_iMyIndexOfPlayers )
			{
				m_pPlayerDiceTable->MouseDown(e_iX,e_iY);
				if( m_pCallImageButton && !m_bReceivedCatchMessage )
					m_pCallImageButton->MouseDown(e_iX,e_iY);
			}
			if(m_pCatchImageButton && m_iCallDicePoint != -1 && !m_bReceivedCatchMessage )
				m_pCatchImageButton->MouseDown(e_iX,e_iY);
		}
	}
}

void	cBluffingDiceMultiPlayerGame::MouseMove(int e_iX,int e_iY)
{
	if( m_pShowMPDI && !m_pShowMPDI->IsAnimationDone()  )
		return;
	if( m_eGameStatus == e_GAME_PLAY )
	{
		if( m_eCatchType == eCT_NO_CATCH )
		{
			if( m_iTurnToCallIndex == m_iMyIndexOfPlayers )
			{
				if( m_pPlayerDiceTable )
					m_pPlayerDiceTable->MouseMove(e_iX,e_iY);
				if( m_pCallImageButton && !m_bReceivedCatchMessage )
					m_pCallImageButton->MouseMove(e_iX,e_iY);
			}
			if(m_pCatchImageButton && m_iCallDicePoint != -1 && !m_bReceivedCatchMessage )
				m_pCatchImageButton->MouseMove(e_iX,e_iY);
		}		
	}	
}

void	cBluffingDiceMultiPlayerGame::MouseUp(int e_iX,int e_iY)
{
	if( m_pShowMPDI && !m_pShowMPDI->IsAnimationDone()  )
		return;
	if( m_eGameStatus == e_GAME_PLAY )
	{
		if( m_eCatchType == eCT_NO_CATCH )
		{
			if( m_iTurnToCallIndex == m_iMyIndexOfPlayers )
			{
				m_pPlayerDiceTable->MouseUp(e_iX,e_iY);
				if( m_pCallImageButton && !m_bReceivedCatchMessage )
					m_pCallImageButton->MouseUp(e_iX,e_iY);
			}
			if(m_pCatchImageButton && m_iCallDicePoint != -1 && !m_bReceivedCatchMessage )
				m_pCatchImageButton->MouseUp(e_iX,e_iY);
		}
	}	
}



const   WCHAR*	cBluffingDiceMultiPlayerGame::GetNextPhaseName()
{
	cBluffingGirlApp::m_spGameNetwork->Destroy();
	cBluffingGirlApp::m_spGameNetwork->SetNewClientConnectedCallBack(0);
	cBluffingGirlApp::m_spGameNetwork->SetDisconnectCallbackl(0);
	return m_strNextPhaseName.c_str();
}

void	cBluffingDiceMultiPlayerGame::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	cNetWorkConnectPhase*l_pNetWorkConnectPhase = (cNetWorkConnectPhase*)e_pData;
	m_iMyIndexOfPlayers = l_pNetWorkConnectPhase->m_iPlayerIDIndex;
	m_iIAmReadyMessageVector.clear();
	for( int i=0;i<l_pNetWorkConnectPhase->m_iTotalPlayer;++i )
	{
		m_iIAmReadyMessageVector.push_back(0);
	}
	m_iIAmReadyMessageVector[0] = 1;
}