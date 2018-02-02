/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "SceneChange.h"
#include "../Probability/ProbLibHandler.h"
//
#include "../FishApp.h"
//
#include "../Monster/MonsterManager.h"
#include "../Monster/FishGroup.h"
#include "../MiniGame/MiniGameManager.h"
//
#include "../Probability/ProbabilityMain.h"
#include "../Probability/Probability_Liu/Probability_Liu.h"
#include "../Probability/ProbabilityData.h"
#include "../PrizeGame/PrizeGameBase.h"
#include "../ControlPanel/FishControlPanel.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../PlayerBehavior/PlayerManager.h"
#include "../PlayerBehavior/PlayerWeapon.h"
#include "../PlayerBehavior/WinMoneyEffect.h"
//
#include "../Bullet/BulletManager.h"
#include "../Bullet/Bullet.h"

#define TAKE_WIN_SCORE_LOWER_LIMIT		10000
#define TAKE_WIN_TIME			8
#define TAKE_WIN_TIME_LIMIT		7

extern cProbLibHandler*	g_ProbLibHandler;
//
/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
sSpecialSceneData::sSpecialSceneData()
{
	fTime = 0.f;MovingMethod = eMM_MAX;pPrizeGameBase =0;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
sSpecialSceneData::~sSpecialSceneData()
{
	SAFE_DELETE(pPrizeGameBase);
}

eMovingMethod	GetMovingMethod(const WCHAR*e_strName)
{

	if( !wcscmp(L"Roulette",e_strName) )
	{
		return eMM_ROULETTE;
	}
	else if( !wcscmp(L"PrizeGame",e_strName) )
	{
		return eMM_PRIZEGAME;
	}
	return eMM_MAX;
}


sLittlePrizeData::sLittlePrizeData()
{
	pPrizeGameBase = 0;
}

sLittlePrizeData::~sLittlePrizeData()
{
	SAFE_DELETE(pPrizeGameBase);
}

cSceneChange::cSceneChange()
{
	m_pDragonBallMaxSound = nullptr;
	m_pDragonBall78Sound = nullptr;
	m_pShow4Sound = nullptr;
	m_pDBTakeWinSound = nullptr;
	duration = 2.0f;     // 振動多久
	speed = 20.0f;       // 擺動速度
	magnitude = 5.0f; // 振幅大小
	elapsed = 0.0f;
	randomStart = frand(-1000.0f, 1000.0f);

	m_iCurrentNormalBGIndex = -1;
	m_iCurrentSpecialBGIndex = -1;
	m_iCurrentSpecialSceneIndex = -1;
	m_bStopChangeScene = false;
	m_bSceneIsSpecial = false;
	m_bTimeToLeave = false;
	m_fFishRunAwaySpeed = 2.f;
	m_iTotalProbability = 0;
	m_pBGImage = 0;
	m_CurrentBGName = "";
	m_pbSceneChange = &cFishApp::m_sbSceneChange;
	m_pWaterMPDI = 0;
	m_pWaterList = 0;
	m_pLastBGImage = 0;
	m_pShowChangeSceneMPDI = 0;
	m_StopGenerateFishTime.SetTargetTime(15.f);
	m_iSissorWidth = (int)cGameApp::m_svGameResolution.x;
	m_pFishGroupByPrizeGame = 0;
	m_pSpecialSceneData =0;

	m_iLittlePrizeIndex = 0;
	m_pLittlePrizeData = 0;
	// special scene
	m_bIsSpecialScene = false;
	m_iTurnOutMPDIIndex = 0;

	m_fBounsGameBossNoPlayerHurtTC = 0;
	m_iCurrentBonusBGIndex=-1;
	m_iCurrentBonusSceneIndex = -1;
	m_bIsBonusScene = false;
	m_bSceneIsBonus = false;
	m_pShowChangeBonusSceneMPDI = 0;
	m_pChangeBonusSceneMPDI = 0;
	m_pDragonBallBaseMPDIList = 0;
	//m_pDragonBallBackMPDI = 0;
	m_pOutBonusSceneMPDI = 0;
	m_pBonusGameMassageMPDI = 0;


//	cSimplexNoise::Init();

	m_bBonusGameLeave = false;
	m_bBonusGameMassage = false;
	m_bBonusGameEnd = false;
	m_bBonusGameReady = false;
	m_pBonusGameMassageNumerial = 0;
	m_iBonusGameFundin = 0;
	m_bPalyerResume = false;
	m_pBonusGameWinnerMPDI = 0;
	m_matDragonBallTransform = cMatrix44::Identity;
	m_i64PastTotalIn = 0;

	m_fTakeWinTime = 0;
	m_pCollectionPlate = nullptr;
#ifdef COLLECTION_GAME_FEATURE
	m_pCollectionPlate = new cCollectionPlate();
#endif

	m_fBonusTime = 0;
	m_iBonusDeadTime = 0;
}

cSceneChange::~cSceneChange()
{
	SAFE_DELETE(m_pBonusGameMassageNumerial);
	SAFE_DELETE(m_pCollectionPlate);
	DELETE_VECTOR(m_SpecialSceneList,sSpecialSceneData*);
	DELETE_VECTOR(m_BonusSceneList,sSpecialSceneData*);
	DELETE_VECTOR(m_LittlePrizeList,sLittlePrizeData*);
}

void	cSceneChange::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("NormaleBG")
	{
		ProcessNormalBG(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("SpecialBG")
	{
		ProcessSpecialBG(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("BonusBG")
	{
		ProcessBonusBG(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("BG")
	{
		ProcessBG(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("SpecialScene")
	{
		ProcessSpecialScene(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("LittlePrize")
	{
		ProcessLittlePrize(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("BonusScene")
	{
		ProcessBonusScene(e_pTiXmlElement);
	}
}

void cSceneChange::ProcessLittlePrize(TiXmlElement*e_pTiXmlElement)
{

	sLittlePrizeData * l_pLittlePrizeData = new	sLittlePrizeData;
	PARSE_ELEMENT_START(e_pTiXmlElement)
	COMPARE_NAME("FileName")
	{
		l_pLittlePrizeData->pPrizeGameBase = new cPrizeGameBase();
		l_pLittlePrizeData->pPrizeGameBase->ProcessPrizeGameData(UT::WcharToChar(l_strValue).c_str());
	}

	PARSE_NAME_VALUE_END
	m_LittlePrizeList.push_back(l_pLittlePrizeData);
}



void cSceneChange::ProcessSpecialBG(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR *BG_MPDIListName = e_pTiXmlElement->Attribute( L"Name" );
	const WCHAR *BG_MPDI_Name    = e_pTiXmlElement->Attribute( L"MPDI" );
	const WCHAR *Probability     = e_pTiXmlElement->Attribute( L"Probability" );

	std::vector< std::string > MPDIListName = GetStringListByCommaDivide( BG_MPDIListName );
	std::vector< std::string > MPDIName     = GetStringListByCommaDivide( BG_MPDI_Name );

	m_SpecialBG.m_iProbability = _wtoi( Probability );

	m_iTotalProbability += m_SpecialBG.m_iProbability;

	for ( std::size_t i = 0; i < MPDIListName.size(); i++ )
	{
		 cMPDIList *pMPDIList = cGameApp::GetMPDIListByFileName( ValueToStringW( MPDIListName[i] ).c_str() );
		
		 if ( pMPDIList )
		 {
			 for ( std::size_t j = 0; j < MPDIName.size(); j++ )
			 {
				  cMPDI *pMPDI = pMPDIList->GetObject( ValueToStringW( MPDIName[j] ) );

				  if ( pMPDI )
				  {
					  cMPDI *pTempBGMPDI = new cMPDI( pMPDI );

					  if ( pTempBGMPDI )
					  {
						  m_AllBGImageList.AddObjectNeglectExist( pTempBGMPDI );
						  m_SpecialBG.m_vImageNameList.push_back( MPDIName[j] );
					  }
				  }
				  else
					  UT::ErrorMsg( ValueToStringW( MPDIName[j] ), L"Load Special BG MPDI Error!!!" );
			 }
		 }
		 else
			 UT::ErrorMsg( ValueToStringW( MPDIListName[i] ), L"Load Special BG MPDILIST Error!!!" );
	}
}

void cSceneChange::ProcessBonusBG(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR *BG_MPDIListName = e_pTiXmlElement->Attribute( L"Name" );
	const WCHAR *BG_MPDI_Name    = e_pTiXmlElement->Attribute( L"MPDI" );
	const WCHAR *Probability     = e_pTiXmlElement->Attribute( L"Probability" );

	std::vector< std::string > MPDIListName = GetStringListByCommaDivide( BG_MPDIListName );
	std::vector< std::string > MPDIName     = GetStringListByCommaDivide( BG_MPDI_Name );

	m_BonusBG.m_iProbability = _wtoi( Probability );

	m_iTotalProbability += m_BonusBG.m_iProbability;

	for ( std::size_t i = 0; i < MPDIListName.size(); i++ )
	{
		 cMPDIList *pMPDIList = cGameApp::GetMPDIListByFileName( ValueToStringW( MPDIListName[i] ).c_str() );
		
		 if ( pMPDIList )
		 {
			 for ( std::size_t j = 0; j < MPDIName.size(); j++ )
			 {
				  cMPDI *pMPDI = pMPDIList->GetObject( ValueToStringW( MPDIName[j] ).c_str());

				  if ( pMPDI )
				  {
					  cMPDI *pTempBGMPDI = new cMPDI( pMPDI );

					  if ( pTempBGMPDI )
					  {
						  m_AllBGImageList.AddObjectNeglectExist( pTempBGMPDI );
						  m_BonusBG.m_vImageNameList.push_back( MPDIName[j] );
					  }
				  }
				  else
					  UT::ErrorMsg( ValueToStringW( MPDIName[j] ).c_str(), L"Load Bonus BG MPDI Error!!!" );
			 }
		 }
		 else
			 UT::ErrorMsg( ValueToStringW( MPDIListName[i] ).c_str(), L"Load Bonus BG MPDILIST Error!!!" );
	}
}

//<BG ChangeCircle="300,500">
//	<NormaleBG Name="BG1.png,BG2.png,BG3.png" Probability="50" />
//	<SpecialBG Name="BG1.png,BG2.png,BG3.png" Probability="50" />
//</BG>

void cSceneChange::ProcessNormalBG(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR *BG_MPDIListName = e_pTiXmlElement->Attribute( L"Name" );
	const WCHAR *BG_MPDI_Name    = e_pTiXmlElement->Attribute( L"MPDI" );
	const WCHAR *Probability     = e_pTiXmlElement->Attribute( L"Probability" );

	std::vector< std::string > MPDIListName = GetStringListByCommaDivide( BG_MPDIListName );
	std::vector< std::string > MPDIName     = GetStringListByCommaDivide( BG_MPDI_Name );

	m_NormalBG.m_iProbability = _wtoi( Probability );

	m_iTotalProbability += m_NormalBG.m_iProbability;

	for ( std::size_t i = 0; i < MPDIListName.size(); i++ )
	{
		 cMPDIList *pMPDIList = cGameApp::GetMPDIListByFileName( ValueToStringW( MPDIListName[i] ).c_str());
		
		 if ( pMPDIList )
		 {
			 for ( std::size_t j = 0; j < MPDIName.size(); j++ )
			 {
				  cMPDI *pMPDI = pMPDIList->GetObject( ValueToStringW( MPDIName[j] ).c_str());

				  if ( pMPDI )
				  {
					  cMPDI *pTempBGMPDI = new cMPDI( pMPDI );

					  if ( pTempBGMPDI )
					  {
						  m_AllBGImageList.AddObjectNeglectExist( pTempBGMPDI );
						  m_NormalBG.m_vImageNameList.push_back( MPDIName[j] );
					  }
				  }
				  else
					  UT::ErrorMsg( ValueToStringW( MPDIName[j] ).c_str(), L"Load Normal BG MPDI Error!!!" );
			 }
		 }
		 else
			 UT::ErrorMsg( ValueToStringW( MPDIListName[i] ).c_str(), L"Load Normal BG MPDILIST Error!!!" );
	}
}

void	cSceneChange::ProcessSpecialScene(TiXmlElement*e_pTiXmlElement)
{
	sSpecialSceneData	*l_pSpecialSceneData = new sSpecialSceneData;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Time")
		{
			l_pSpecialSceneData->fTime = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("FileName")
		{
			if( l_pSpecialSceneData->MovingMethod == eMM_PRIZEGAME )
			{
				l_pSpecialSceneData->pPrizeGameBase = new cPrizeGameBase();
				l_pSpecialSceneData->pPrizeGameBase->ProcessPrizeGameData(UT::WcharToChar(l_strValue).c_str());
			}
		}
		else
		COMPARE_NAME("MovingMethod")
		{
			l_pSpecialSceneData->MovingMethod = GetMovingMethod(l_strValue);
			l_pSpecialSceneData->m_bIsSpecialScenceMPDIIndex =false;
		}
		else
		COMPARE_NAME("FishGroupDelayTime")
		{
			//l_pSpecialSceneData->pFishGroup->m_fFishGroupDelayTime = ( float ) _wtof(l_strValue);
			l_pSpecialSceneData->pPrizeGameBase->m_fPrizeGameFishDelayTime = ( float ) _wtof(l_strValue);
		}
		else
		COMPARE_NAME("FishGroupWaitingForCleanInScence")
		{
			//l_pSpecialSceneData->pFishGroup->m_bIsFishGroupWaitingForCleanInScence = _wtoi(l_strValue)?true:false;
			l_pSpecialSceneData->pPrizeGameBase->m_bIsPrizeGameFishWaitingForCleanInScence = _wtoi(l_strValue)?true:false;
		}
		else
		COMPARE_NAME("MPDI")
		{
			l_pSpecialSceneData->m_vSpecialScenceMPDINameList = GetStringListByCommaDivide(l_strValue);
			l_pSpecialSceneData->m_iSpecialScenceMPDIIndex = -1;
			l_pSpecialSceneData->m_bIsSpecialScenceMPDIIndex =true;

		}
		else
		COMPARE_NAME("ID")
		{
			l_pSpecialSceneData->m_iID =  VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
	m_SpecialSceneList.push_back(l_pSpecialSceneData);
}

void	cSceneChange::ProcessBonusScene(TiXmlElement*e_pTiXmlElement)
{
	sSpecialSceneData	*l_pSpecialSceneData = new sSpecialSceneData;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Time")
		{
			l_pSpecialSceneData->fTime = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("FileName")
		{
			if( l_pSpecialSceneData->MovingMethod == eMM_PRIZEGAME )
			{
				l_pSpecialSceneData->pPrizeGameBase = new cPrizeGameBase();
				l_pSpecialSceneData->pPrizeGameBase->ProcessPrizeGameData(UT::WcharToChar(l_strValue).c_str());
			}
		}
		else
		COMPARE_NAME("MovingMethod")
		{
			l_pSpecialSceneData->MovingMethod = GetMovingMethod(l_strValue);
			l_pSpecialSceneData->m_bIsSpecialScenceMPDIIndex =false;
		}
		else
		COMPARE_NAME("FishGroupDelayTime")
		{
			//l_pSpecialSceneData->pFishGroup->m_fFishGroupDelayTime = ( float ) _wtof(l_strValue);
			l_pSpecialSceneData->pPrizeGameBase->m_fPrizeGameFishDelayTime = ( float ) _wtof(l_strValue);
		}
		else
		COMPARE_NAME("FishGroupWaitingForCleanInScence")
		{
			//l_pSpecialSceneData->pFishGroup->m_bIsFishGroupWaitingForCleanInScence = _wtoi(l_strValue)?true:false;
			l_pSpecialSceneData->pPrizeGameBase->m_bIsPrizeGameFishWaitingForCleanInScence = _wtoi(l_strValue)?true:false;
		}
		else
		COMPARE_NAME("MPDI")
		{
			l_pSpecialSceneData->m_vSpecialScenceMPDINameList = GetStringListByCommaDivide(l_strValue);
			l_pSpecialSceneData->m_iSpecialScenceMPDIIndex = -1;
			l_pSpecialSceneData->m_bIsSpecialScenceMPDIIndex =true;

		}
		else
		COMPARE_NAME("ID")
		{
			l_pSpecialSceneData->m_iID =  VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
	m_BonusSceneList.push_back(l_pSpecialSceneData);
}

void	cSceneChange::ProcessBG(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strChangeCircle = e_pTiXmlElement->Attribute(L"ChangeCircle");
	m_TimeCircleRestrict = GetVector2(l_strChangeCircle);

	const WCHAR*l_strFishRunAwaySpeed = e_pTiXmlElement->Attribute(L"FishRunAwaySpeed");
	if( l_strFishRunAwaySpeed )
	{
		m_fFishRunAwaySpeed = (float)_wtof(l_strFishRunAwaySpeed);
	}

	const WCHAR*l_strLittlePrizeTime = e_pTiXmlElement->Attribute(L"LittlePrizeTime");
	if(l_strLittlePrizeTime)
		m_LittlePrizeTime = GetVector2(l_strLittlePrizeTime);

}

void	cSceneChange::Init()
{
	m_pDragonBallMaxSound = cGameApp::m_spSoundParser->GetObject(SOUND_NAME_DragonBallMax);
	m_pDragonBall78Sound = cGameApp::m_spSoundParser->GetObject(SOUND_NAME_DragonBall78);
	m_pShow4Sound = cGameApp::m_spSoundParser->GetObject(SOUND_NAME_Show4);
	m_pDBTakeWinSound = cGameApp::m_spSoundParser->GetObject(SOUND_NAME_DBTakeWin);

	m_bStopChangeScene = false;
	m_eSceneChangeStatus = eSCS_NONE;
	m_bTimeToLeave = false;
	//wcscpy( m_wstrTurnPath, BONUS_SCENE_CHANGE);
	//wcscat( m_wstrTurnPath, cFishApp::m_pGameSetup->GetLocalName());
	//wcscat( m_wstrTurnPath, L".mpdi");

	if(!this->Parse(SCENE_CHANGE_SETUP_FILE))
	{
		UT::ErrorMsg(SCENE_CHANGE_SETUP_FILE,"parse failed!");
	}

	ChangeScene();

	m_ChangeSceneCircle.SetTargetTime(m_TimeCircleRestrict.Rand());

	m_LittlePrizeTC.SetTargetTime(m_LittlePrizeTime.Rand());
	

	m_pWaterList = cGameApp::GetMPDIListByFileName(BG_WATER_MPDI);

	if ( m_pBGImage )
		m_pBGImage->Init();

	if( m_pWaterList )
	{
		m_pWaterMPDI = m_pWaterList->GetObject(0);
		m_pWaterMPDI->Init();
	}
	cMPDIList*l_pMPDIList;
	auto l_strFileName = cFishApp::m_spLanguageFile->GetFileName(SCENE_CHANGE_ALERT);
	if(l_strFileName)
		l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strFileName);
	m_pShowChangeSceneMPDI = l_pMPDIList->GetObject(L"FishTimeShow");
	if(m_pShowChangeSceneMPDI)
		m_pShowChangeSceneMPDI->Init();

	cMPDIList*l_pMPDIList2 = cGameApp::GetMPDIListByFileName(BONUS_SCENE_CHANGE);
	if (l_pMPDIList2)
		m_pChangeBonusSceneMPDI = l_pMPDIList2->GetObject(L"TurrnIn");
	if (m_pCollectionPlate)
	{
		m_pDragonBallBaseMPDIList = m_pCollectionPlate->GetMPDIList();
	}
	if(m_pDragonBallBaseMPDIList)
	{
		if(m_pCollectionPlate)
			m_pCollectionPlate->Init();
		GetNumerialImage();
		//if(m_pDragonBallBaseMPDIList)
			//m_pDragonBallBackMPDI = m_pDragonBallBaseMPDIList->GetObject(L"DB9Normal_0");
		//if(m_pDragonBallBackMPDI)
			//m_pDragonBallBackMPDI->Init();
	}
	BYTE	ucTarget;
	if (m_pCollectionPlate && !m_pCollectionPlate->IsCollectionGameArrive(ucTarget))
	{
		if( cFishApp::m_spGameSaveData->m_pSaveData != NULL  && cFishApp::m_spPlayerManager)
		{
			for( int i = 0 ; i < cFishApp::m_spPlayerManager->Count() ; i++ )
			{
				cPlayerBehaviorBase* l_pPlayer = cFishApp::m_spPlayerManager->GetObject( i );
				if( cFishApp::m_spGameSaveData->m_pProbabilitySaveData->m_i64PlayerDragonBigWin[l_pPlayer->GetPlayerID()] > 0 )
				{
					l_pPlayer->AddScoreByMoneyChangeType(cFishApp::m_spGameSaveData->m_pProbabilitySaveData->m_i64PlayerDragonBigWin[l_pPlayer->GetPlayerID()], eMCT_DRAGON_BIG_WIN_ADD_SCORE);
				}
			}
		}
	}

	//
	if( cFishApp::m_spProbabilityFish )
	{
		cFishApp::m_spProbabilityFish->SetPauseGenerateMonster(false);
	}

	for(int i=0; i<m_AllBGImageList.Count();++i)
	{
		m_AllBGImageList.GetObject(i)->Init();
	}

	//檢查玩家是否有剩餘子彈
	if( cFishApp::m_spGameSaveData->m_pSaveData != NULL && cFishApp::m_spPlayerManager)
	{
		for( int i = 0 ; i < cFishApp::m_spPlayerManager->Count() ; i++ )
		{
			cPlayerBehaviorBase* l_pPlayer = cFishApp::m_spPlayerManager->GetObject( i );
			if( cFishApp::m_spGameSaveData->m_pProbabilitySaveData->m_i64PlayerFiredBet[i] > 0 )
			{
				l_pPlayer->AddScoreByMoneyChangeType(cFishApp::m_spGameSaveData->m_pProbabilitySaveData->m_i64PlayerFiredBet[i], eMCT_FIRE_RETURN_BULLET_ADD_SCORE);
			}
			
		}
	}
	//確認音效是否有播放
	CheckBGSound();
}
bool	cSceneChange::IsNormalScene()
{
	if(m_eSceneChangeStatus == eSCS_NONE && !m_bSceneIsBonus && !m_bSceneIsSpecial && m_ChangeSceneCircle.fRestTime > 15)
		return true;
	else
		return false;
	
}

void	cSceneChange::UpdateeSCS_NONE(float e_fElpaseTime)
{
	if(!m_bSceneIsBonus && !m_bSceneIsSpecial)
		LittlePrizeUpdate(e_fElpaseTime);

	if(m_pBonusGameMassageMPDI)
		m_pBonusGameMassageMPDI->Update( e_fElpaseTime );
	CheckBonusGame();//判斷是否進神龍
	//判斷是否進入彩金或神龍
	CheckSpecialScene( e_fElpaseTime );
	
	//彩金、神龍模式流程update
	SpecialSceneUpdate( e_fElpaseTime );
}

void	cSceneChange::UpdateeeSCS_PLAY_SCENE_CHANGE_MPDI(float e_fElpaseTime)
{
	if ( m_pLastBGImage )
		m_pLastBGImage->Update( e_fElpaseTime );
	if( m_bSceneIsBonus )
	{
		if( m_pShowChangeBonusSceneMPDI )
		{
			if(!m_pShowChangeBonusSceneMPDI->IsAnimationDone())
			{
				Vector3 pos(-720.0f, -450.0f, 0.0f);
				Vector3 pos2(720.0f, 450.0f, 0.0f);
				Vector3 angle(0,0,0);
				m_matDragonBallTransform = cMatrix44::ScaleMatrix(Vector3(1.0f, 1.06f,1.f)) * cMatrix44::RotationMatrix(angle);
				m_matDragonBallTransform = cMatrix44::TranslationMatrix( pos2 ) * m_matDragonBallTransform * cMatrix44::TranslationMatrix( pos );
				m_pShowChangeBonusSceneMPDI->SetLocalTransform(m_matDragonBallTransform);
				m_pShowChangeBonusSceneMPDI->Update(e_fElpaseTime);
				
			}
			if(m_pShowChangeBonusSceneMPDI->IsAnimationDone())
			{
				//歸還子彈
				ReturnBullet();
				if(cFishApp::m_spBulletManager && cFishApp::m_spBulletManager->m_iNumWorking <= 0 )
				{
					*m_pbSceneChange = true;
					cFishApp::m_sfSceneChangeSpeed = this->m_fFishRunAwaySpeed;
					m_eSceneChangeStatus = eSCS_WAIT_FISH_LEAVE;
					cFishApp::m_spMonsterManager->MonsterLeaveScene();
				}
			}
		}
	}
	else
	{
		if( m_pShowChangeSceneMPDI )
		{
			if(m_pShowChangeSceneMPDI->IsAnimationDone())
			{
				if(!IsAllowToChangeScene())
				{
					StartMiniGame();
					return;
				}
				*m_pbSceneChange = true;
				cFishApp::m_sfSceneChangeSpeed = this->m_fFishRunAwaySpeed;
				m_eSceneChangeStatus = eSCS_WAIT_FISH_LEAVE;
				cFishApp::m_spMonsterManager->MonsterLeaveScene();
				g_ProbLibHandler->SetFreePressGameEnd();
			}
			else
				m_pShowChangeSceneMPDI->Update(e_fElpaseTime);
		}
	}
}

void	cSceneChange::UpdateeeSCS_WAIT_FISH_LEAVE(float e_fElpaseTime)
{
	if ( m_pLastBGImage )
		m_pLastBGImage->Update( e_fElpaseTime );
	LockWeaponFire();
	if(cFishApp::m_spMonsterManager->f_GetCountMonsterAlive() == 0)
	{
		if(!IsAllowToChangeScene())
		{
			StartMiniGame();
			return;
		}
		UnlockWeaponFire();
		m_bTimeToLeave = false;
		if( m_bSceneIsBonus )
		{
			if ( m_pChangeBonusSceneMPDI )
				m_pChangeBonusSceneMPDI->Init();
			m_eSceneChangeStatus = eSCS_BONUS_ALERT_GO;
			if (m_pDragonBallMaxSound)
				m_pDragonBallMaxSound->Play(false);
			if (m_pDragonBall78Sound)
				m_pDragonBall78Sound->Play(false);
			cFishApp::m_pBGSoundPlayer->PlayOtherSound(eBGSOT_ScaneChange);
		}
		else
		{
			if (m_pChangeBonusSceneMPDI)
				m_pChangeBonusSceneMPDI->Init();
			m_eSceneChangeStatus = eSCS_BUBBLE_GO;

			cFishApp::m_pBGSoundPlayer->PlayOtherSound(eBGSOT_ScaneChange);
		}
	}
}

void	cSceneChange::UpdateeeSCS_BUBBLE_GO(float e_fElpaseTime)
{
	if ( m_pLastBGImage )
		m_pLastBGImage->Update( e_fElpaseTime );
	m_pChangeBonusSceneMPDI->Update(e_fElpaseTime);
	
	if (m_pChangeBonusSceneMPDI->IsAnimationDone())
	{
		m_eSceneChangeStatus = eSCS_WAIT_BUBBLE_DISSAPEAR;
		m_iSissorWidth = 0;
		m_pLastBGImage = nullptr;
	}
}

void	cSceneChange::UpdateeeSCS_BONUS_ALERT_GO(float e_fElpaseTime)
{
	if ( m_pLastBGImage )
		m_pLastBGImage->Update( e_fElpaseTime );
	m_pChangeBonusSceneMPDI->Update(e_fElpaseTime);

	float	l_fUV[4] = {0,0,1,1};
	
	if( m_pChangeBonusSceneMPDI->IsAnimationDone())
		m_eSceneChangeStatus = eSCS_WAIT_BONUS_ALERT_DISSAPEAR;
	else
	{
		Vector2	l_vPos = cGameApp::m_svGameResolution;
		float	l_fLerp = m_pChangeBonusSceneMPDI->GetCurrentProgress()*l_fUV[2];
		l_vPos.x = (1-l_fLerp)*l_vPos.x;
		if( m_pLastBGImage )
		{
			m_iSissorWidth = (int)l_vPos.x;
			float	l_fUV2[4] = {0,0,1-l_fLerp,l_fUV[3]};
		}

	}

}

void	cSceneChange::UpdateeeSCS_WAIT_BUBBLE_DISSAPEAR(float e_fElpaseTime)
{
	m_iSissorWidth = (int)cGameApp::m_svGameResolution.x;
	*m_pbSceneChange = false;
	m_eSceneChangeStatus = eSCS_NONE;
	cFishApp::m_sfSceneChangeSpeed = 1.f;
	if( m_bSceneIsSpecial )
	{
		cFishApp::m_pBGSoundPlayer->PlayNext(eBGS_ArrayGAme);

		if( m_SpecialSceneList.size() )
		{
			
			m_StopGenerateFishTime.SetTargetTime(m_pSpecialSceneData->fTime);
			if( m_pSpecialSceneData->MovingMethod == eMM_ROULETTE )
			{
				m_pFishGroupByPrizeGame = 0;

				int	l_iPlayerIndex = -1;
				int	l_iMoney = 0;
			}
			else if(m_pSpecialSceneData->MovingMethod == eMM_PRIZEGAME)
			{
				m_pFishGroupByPrizeGame = m_pSpecialSceneData->pPrizeGameBase;
				m_pFishGroupByPrizeGame->RequirePrizeGame();
			}
			cFishApp::m_spProbabilityFish->SetPauseGenerateMonster(true);
			m_StopGenerateFishTime.Start();
			cFishApp::m_spProbabilityFish->SetSpecialScene(true);
		}
	}
	else
	{
		cFishApp::m_spProbabilityFish->SetSpecialScene (false);
		cFishApp::m_spProbabilityFish->SetBonusScene(false);
		CheckBGSound();
	}
}


void	cSceneChange::UpdateeeSCS_WAIT_BONUS_ALERT_DISSAPEAR(float e_fElpaseTime)
{
	m_iSissorWidth = (int)cGameApp::m_svGameResolution.x;
	*m_pbSceneChange = false;
	m_bBonusGameReady = false;
	m_eSceneChangeStatus = eSCS_NONE;
	cFishApp::m_sfSceneChangeSpeed = 1.f;

	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(BONUS_SCENE_CHANGE);
	if(l_pMPDIList)
		m_pOutBonusSceneMPDI = l_pMPDIList->GetObject(L"TurrnInFinish");

	cFishApp::m_pBGSoundPlayer->PlayOtherSound(eBGSOT_DragonMode);
	if( m_BonusSceneList.size() )
	{
		m_StopGenerateFishTime.SetTargetTime(m_pBonusSceneData->fTime);
		if( m_pBonusSceneData->MovingMethod == eMM_ROULETTE )
		{
			m_pFishGroupByPrizeGame = 0;

			int	l_iPlayerIndex = -1;
			int	l_iMoney = 0;
		}
		else if(m_pBonusSceneData->MovingMethod == eMM_PRIZEGAME)
		{
			m_pFishGroupByPrizeGame = m_pBonusSceneData->pPrizeGameBase;
			m_pFishGroupByPrizeGame->RequirePrizeGame();
		}
		cFishApp::m_spProbabilityFish->SetPauseGenerateMonster(true);
		m_StopGenerateFishTime.Start();
		cFishApp::m_spProbabilityFish->SetBonusScene(true);
	}

}

void	cSceneChange::ShutDownShake()
{
	m_fBounsGameBossNoPlayerHurtTC = 0;
	cFishApp::m_bShake = false;
	duration = cFishApp::m_ShakeTm = 0;
	elapsed = 0.0f;
	randomStart = frand(-1000.0f, 1000.0f);
	m_pBGImage->SetPos(Vector3::Zero);
}

void	cSceneChange::Shake(float e_fElpaseTime)
{
	if(cFishApp::m_ShakeTm <= 3.0f)
	{
		speed = 30.0f;		       // 擺動速度
		magnitude = 7.5f; // 振幅大小
	}else if(cFishApp::m_ShakeTm <= 5.0f)
	{
		speed = 50.0f;       // 擺動速度
		magnitude = 12.5f; // 振幅大小
	}

	duration = cFishApp::m_ShakeTm;
	if(elapsed < duration)
	{
		elapsed += e_fElpaseTime;
		const int l_cMagicShakeCount = 10;
		if (rand() % l_cMagicShakeCount)
		{
			float percentComplete = 1.f - (elapsed / duration);
			if (percentComplete > 1.f)
				percentComplete = 1.f;
			const float l_cfXShakeValue = 28.f*percentComplete*percentComplete;
			const float l_cfYShakeValue = 18.f* percentComplete*percentComplete;
			float x = frand(-l_cfXShakeValue, l_cfXShakeValue);
			float y = frand(-l_cfYShakeValue, l_cfYShakeValue);



			//float percentComplete = elapsed / duration;

			//float damper = 1.0f - MyMath::Clamp(2.0f * percentComplete - 1.0f, 0.0f, 1.0f);

			//float alpha = randomStart + speed * percentComplete;

			//float x = cSimplexNoise::Noise(alpha, 0.0f, 0.0f) * 2.0f - 1.0f;
			//float y = cSimplexNoise::Noise(0.0f, alpha, 0.0f) * 2.0f - 1.0f;

			//x *= magnitude * damper;
			//y *= magnitude * damper;


			Vector3 pos = m_pBGImage->GetPos();
			pos.x = x;
			pos.y = y;

			if (m_pBGImage)
				m_pBGImage->SetPos(pos);
			if (m_pCollectionPlate)
				m_pCollectionPlate->SetPos(-pos);
		}
	}
	else
	{
		cFishApp::m_bShake = false;
		duration = cFishApp::m_ShakeTm = 0;
		elapsed = 0.0f;
		randomStart = frand(-1000.0f, 1000.0f);

		if(m_pBGImage)
			m_pBGImage->SetPos(Vector3::Zero);
		if (m_pCollectionPlate)
			m_pCollectionPlate->SetPos(Vector3::Zero);
	}
}

void	cSceneChange::Update(float e_fElpaseTime)
{
	if ( m_pBGImage )
		m_pBGImage->Update( e_fElpaseTime );

	if(m_pWaterMPDI)
		m_pWaterMPDI->Update(e_fElpaseTime);
	if( m_bStopChangeScene )
		return;

	if(cFishApp::m_bShake)
		Shake(e_fElpaseTime);

	if (m_pCollectionPlate)
		m_pCollectionPlate->Update(e_fElpaseTime);

	if(m_pBonusGameWinnerMPDI)
		m_pBonusGameWinnerMPDI->Update(e_fElpaseTime);

	m_ChangeSceneCircle.Update(e_fElpaseTime);
	switch(m_eSceneChangeStatus)
	{
		case 	eSCS_NONE:
				UpdateeSCS_NONE(e_fElpaseTime);
			break;
		case 	eSCS_PLAY_SCENE_CHANGE_MPDI:
				UpdateeeSCS_PLAY_SCENE_CHANGE_MPDI(e_fElpaseTime);
			break;
		case 	eSCS_WAIT_FISH_LEAVE:
				UpdateeeSCS_WAIT_FISH_LEAVE(e_fElpaseTime);
			break;
		case 	eSCS_BUBBLE_GO:
				UpdateeeSCS_BUBBLE_GO(e_fElpaseTime);
			break;
		case 	eSCS_WAIT_BUBBLE_DISSAPEAR:
				UpdateeeSCS_WAIT_BUBBLE_DISSAPEAR(e_fElpaseTime);
			break;
		case	eSCS_BONUS_ALERT_GO:
				UpdateeeSCS_BONUS_ALERT_GO(e_fElpaseTime);
			break;
		case	eSCS_WAIT_BONUS_ALERT_DISSAPEAR:
				UpdateeeSCS_WAIT_BONUS_ALERT_DISSAPEAR(e_fElpaseTime);
			break;
	}
}

void	cSceneChange::Render()
{
	if( eSCS_NONE == m_eSceneChangeStatus )
	{
		m_pBGImage->Render();
		if (m_pCollectionPlate)
			m_pCollectionPlate->Render();
	}
	else
	{
		if( eSCS_WAIT_FISH_LEAVE == m_eSceneChangeStatus || eSCS_PLAY_SCENE_CHANGE_MPDI == m_eSceneChangeStatus )
		{
			if ( m_pLastBGImage )
				m_pLastBGImage->Render();
			if (m_pCollectionPlate)
				m_pCollectionPlate->Render();
		}
		else
		if( eSCS_BUBBLE_GO == m_eSceneChangeStatus || eSCS_WAIT_BUBBLE_DISSAPEAR == m_eSceneChangeStatus || eSCS_BONUS_ALERT_GO == m_eSceneChangeStatus || eSCS_WAIT_BONUS_ALERT_DISSAPEAR == m_eSceneChangeStatus )
		{
			if ( m_pLastBGImage )
				m_pLastBGImage->Render();
			else
				m_pBGImage->Render();
			if (m_pCollectionPlate)
				m_pCollectionPlate->Render();
		}
	}
}

void	cSceneChange::LastRender()
{
	if( eSCS_PLAY_SCENE_CHANGE_MPDI == m_eSceneChangeStatus )
	{
		if( m_bSceneIsBonus )
		{
			if ( m_pShowChangeBonusSceneMPDI )
			m_pShowChangeBonusSceneMPDI->Render();
		}else{
			if ( m_pShowChangeSceneMPDI )
				m_pShowChangeSceneMPDI->Render();
		}
	}

	if ( m_pWaterMPDI )
		m_pWaterMPDI->Render();
}

void	cSceneChange::DebugRender()
{
	if (!*m_pbSceneChange)
	{
		std::wstring	l_str = UT::ComposeMsgByFormat(L"ToChangeScene:%.2f", m_ChangeSceneCircle.fRestTime);
		cGameApp::m_spGlyphFontRender->RenderFont(0, 700, l_str.c_str());
	}

	std::wstring	l_str = UT::ComposeMsgByFormat(L"LittlePrize:%.2f", m_LittlePrizeTC.fRestTime);
	cGameApp::m_spGlyphFontRender->RenderFont(0, 730, l_str.c_str());
	if (g_ProbLibHandler)
	{
		l_str = UT::ComposeMsgByFormat(L"KillMode:%d", g_ProbLibHandler->IsKillMode()?1:0);
		cGameApp::m_spGlyphFontRender->RenderFont(0, 760, l_str.c_str());
	}
}

int		IndexOfVector(const char*e_strName,std::vector<std::string>*e_pList)
{
	size_t	l_uiSize = e_pList->size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		if(!strcmp((*e_pList)[i].c_str(),e_strName))
		{
			return (int)i;
		}
	}
	return -1;
}

void	cSceneChange::ChangeScene()
{
	assert( m_NormalBG.m_vImageNameList.size() > 0 );

	m_iCurrentNormalBGIndex++;

	if ( m_iCurrentNormalBGIndex >= (int)m_NormalBG.m_vImageNameList.size() )
		m_iCurrentNormalBGIndex = 0;

	int	l_iIndex = m_iCurrentNormalBGIndex;

	if ( IndexOfVector( m_CurrentBGName.c_str(), &m_NormalBG.m_vImageNameList ) == l_iIndex )
		l_iIndex = UT::GetLoopIndex( l_iIndex + 1, (int)m_NormalBG.m_vImageNameList.size() );

	m_pLastBGImage = m_pBGImage;
	const char *l_strImageName = m_NormalBG.m_vImageNameList[l_iIndex].c_str();
	if(cFishApp::m_spProbabilityFish)
		cFishApp::m_spProbabilityFish->GetProbabilityData()->SetBackgroundIdx (l_iIndex);
	m_pBGImage = m_AllBGImageList.GetObject( UT::CharToWchar(UT::GetFileNameWithoutFullPath(l_strImageName)) );
	m_CurrentBGName = m_NormalBG.m_vImageNameList[ l_iIndex ];
}

void	cSceneChange::ChangeSpecialScene()
{
	m_iCurrentSpecialSceneIndex = m_iCurrentSpecialSceneIndex + 1;
	if( m_iCurrentSpecialSceneIndex >= (int)m_SpecialSceneList.size() )
		m_iCurrentSpecialSceneIndex = 0;
	m_pSpecialSceneData= m_SpecialSceneList[ m_iCurrentSpecialSceneIndex ];
	
	if(m_pSpecialSceneData->m_bIsSpecialScenceMPDIIndex)
	{
		//m_pSpecialSceneData->m_iSpecialScenceMPDIIndex++;
		m_pSpecialSceneData->m_iSpecialScenceMPDIIndex =1;
		if ( m_pSpecialSceneData->m_iSpecialScenceMPDIIndex >= (int)m_pSpecialSceneData->m_vSpecialScenceMPDINameList.size() )
			m_pSpecialSceneData->m_iSpecialScenceMPDIIndex =0;

		int	l_iIndex = m_pSpecialSceneData->m_iSpecialScenceMPDIIndex;

		if ( IndexOfVector(m_CurrentBGName.c_str(), &m_pSpecialSceneData->m_vSpecialScenceMPDINameList) == l_iIndex )
			l_iIndex = UT::GetLoopIndex( l_iIndex + 1, (int)m_pSpecialSceneData->m_vSpecialScenceMPDINameList.size() );

		m_pLastBGImage  = m_pBGImage;
		const char *l_strImageName = m_pSpecialSceneData->m_vSpecialScenceMPDINameList[l_iIndex].c_str();
		m_pBGImage = m_AllBGImageList.GetObject( UT::CharToWchar(UT::GetFileNameWithoutFullPath(l_strImageName)) );
		m_CurrentBGName =m_pSpecialSceneData->m_vSpecialScenceMPDINameList[ l_iIndex ];
	}
	else
	{
		assert( m_SpecialBG.m_vImageNameList.size() > 0 );

		m_iCurrentSpecialBGIndex++;

		if ( m_iCurrentSpecialBGIndex >= (int)m_SpecialBG.m_vImageNameList.size() )
			m_iCurrentSpecialBGIndex = 0;

		int	l_iIndex = m_iCurrentSpecialBGIndex;

		if ( IndexOfVector(m_CurrentBGName.c_str(), &m_SpecialBG.m_vImageNameList) == l_iIndex )
			l_iIndex = UT::GetLoopIndex( l_iIndex + 1, (int)m_SpecialBG.m_vImageNameList.size() );

		m_pLastBGImage  = m_pBGImage;
		const char *l_strImageName = m_SpecialBG.m_vImageNameList[l_iIndex].c_str();
		m_pBGImage = m_AllBGImageList.GetObject( UT::CharToWchar(UT::GetFileNameWithoutFullPath(l_strImageName)) );
		m_CurrentBGName = m_SpecialBG.m_vImageNameList[ l_iIndex ];
	}
}

void	cSceneChange::ChangeBonusScene()
{
	m_iCurrentBonusSceneIndex = m_iCurrentBonusSceneIndex + 1;
	if( m_iCurrentBonusSceneIndex >= (int)m_BonusSceneList.size() )
		m_iCurrentBonusSceneIndex = 0;
	m_pBonusSceneData= m_BonusSceneList[ m_iCurrentBonusSceneIndex ];
	
	if(m_pBonusSceneData->m_bIsSpecialScenceMPDIIndex)
	{
		//m_pSpecialSceneData->m_iSpecialScenceMPDIIndex++;
		m_pBonusSceneData->m_iSpecialScenceMPDIIndex =1;
		if ( m_pBonusSceneData->m_iSpecialScenceMPDIIndex >= (int)m_pBonusSceneData->m_vSpecialScenceMPDINameList.size() )
			m_pBonusSceneData->m_iSpecialScenceMPDIIndex =0;

		int	l_iIndex = m_pBonusSceneData->m_iSpecialScenceMPDIIndex;

		if ( IndexOfVector(m_CurrentBGName.c_str(), &m_pBonusSceneData->m_vSpecialScenceMPDINameList) == l_iIndex )
			l_iIndex = UT::GetLoopIndex( l_iIndex + 1, (int)m_pBonusSceneData->m_vSpecialScenceMPDINameList.size() );

		m_pLastBGImage  = m_pBGImage;
		const char *l_strImageName = m_pBonusSceneData->m_vSpecialScenceMPDINameList[l_iIndex].c_str();
		m_pBGImage = m_AllBGImageList.GetObject( UT::CharToWchar(UT::GetFileNameWithoutFullPath(l_strImageName)) );
		m_CurrentBGName =m_pBonusSceneData->m_vSpecialScenceMPDINameList[ l_iIndex ];
	}
	else
	{
		assert( m_BonusBG.m_vImageNameList.size() > 0 );

		m_iCurrentBonusBGIndex++;

		if ( m_iCurrentBonusBGIndex >= (int)m_BonusBG.m_vImageNameList.size() )
			m_iCurrentBonusBGIndex = 0;

		int	l_iIndex = m_iCurrentBonusBGIndex;

		if ( IndexOfVector(m_CurrentBGName.c_str(), &m_BonusBG.m_vImageNameList) == l_iIndex )
			l_iIndex = UT::GetLoopIndex( l_iIndex + 1, (int)m_BonusBG.m_vImageNameList.size() );

		m_pLastBGImage  = m_pBGImage;
		const char *l_strImageName = m_BonusBG.m_vImageNameList[l_iIndex].c_str();
		m_pBGImage = m_AllBGImageList.GetObject( UT::CharToWchar(UT::GetFileNameWithoutFullPath(l_strImageName)) );
		m_CurrentBGName = m_BonusBG.m_vImageNameList[ l_iIndex ];
	}
}

bool	cSceneChange::IsChangeScene()
{
	return m_eSceneChangeStatus == eSCS_NONE?false:true;
}

void	cSceneChange::LittlePrizeUpdate(float e_fElpaseTime)
{
	m_LittlePrizeTC.Update(e_fElpaseTime);
	if(m_LittlePrizeTC.bTragetTimrReached)
	{
		if(m_LittlePrizeList.size())
		{
			m_iLittlePrizeIndex = rand(0,(int)m_LittlePrizeList.size());
			m_pLittlePrizeData = m_LittlePrizeList[m_iLittlePrizeIndex];

			if(m_pLittlePrizeData)
				m_pLittlePrizeData->pPrizeGameBase->RequirePrizeGame();
		}
		m_LittlePrizeTC.SetTargetTime(m_LittlePrizeTime.Rand());
	}
}

void	cSceneChange::GetNumerialImage()
{
	cPuzzleImage* pNumerial = cGameApp::GetPuzzleImageByFileName( BONUS_GAME_NUMRIAL );
	if ( pNumerial )
	{
		if(m_pBonusGameMassageNumerial)
			SAFE_DELETE(m_pBonusGameMassageNumerial);
		m_pBonusGameMassageNumerial = GetAligmentNumerialImage(pNumerial, L"DragonScoreNumerial0", L"DragonScoreNumerial9");
	}
}

void	cSceneChange::CheckBonusGame()
{
	BYTE	ucTarget;
	if( m_pCollectionPlate && m_pCollectionPlate->IsCollectionGameArrive(ucTarget))//判斷龍珠是否集滿
	{
		if(cFishApp::m_spMonsterManager)
			cFishApp::m_spMonsterManager->CollectionFishShowUp(false);//不出龍珠魚
		if ( m_pCollectionPlate->IsAppearCollectionGameFinish())//判斷集龍珠動畫是否完成
		{
			if ( !IsBonusScene() )
			{
				/*cFishApp::m_spGameSaveData->m_pSaveData->m_fBonusGamePrizePool = cFishApp::m_spGameSaveData->m_pSaveData->m_iBonusGameHitValue - 
					cFishApp::m_spGameSaveData->m_pSaveData->m_i64BonusGameSakuraPool;*/
				if(!IsAllowToChangeScene())
				{
					StartMiniGame();
					return;
				}
				LockChangeBet();
				m_bSceneIsSpecial = false;
				cFishApp::m_spProbabilityFish->SetSpecialScene(false);
				m_fBonusTime = 0;
				m_iBonusDeadTime = g_ProbLibHandler->GetRandom(5,9);
				m_bSceneIsBonus = true;
				m_bPalyerResume = true;
				m_pOutBonusSceneMPDI = 0;
				m_pBonusGameMassageMPDI = 0;
				//m_bPalyerResume = true;
				m_ChangeSceneCircle.SetTargetTime(m_TimeCircleRestrict.Rand());
			}
		}
	}
}

void	cSceneChange::CheckBGSound()
{
	if( m_bSceneIsBonus == false && m_bSceneIsSpecial == false && (cFishApp::m_spControlPanel && cFishApp::m_spControlPanel->IsControlPanelMode() == false) )
	{
		cFishApp::m_pBGSoundPlayer->PlayNext(eBGS_BaseGame);
	}
}

void	cSceneChange::ChangeTurnOutMPDI()
{
	WCHAR	wcTurnOutName[20] = {0};	
	swprintf(wcTurnOutName, L"TurrnOut%02d", (m_iTurnOutMPDIIndex + 1));
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(BONUS_SCENE_CHANGE);
	if(l_pMPDIList)
	{
		if(l_pMPDIList->GetObject(wcTurnOutName))
			m_pOutBonusSceneMPDI = l_pMPDIList->GetObject(wcTurnOutName);
		else
			return;
	}
	m_pOutBonusSceneMPDI->Init();

	m_iTurnOutMPDIIndex++;
	if(m_iTurnOutMPDIIndex > 4)
		m_iTurnOutMPDIIndex = 0;
}

void	cSceneChange::CheckSpecialScene(float e_fElpaseTime)
{
	if( m_bSceneIsBonus && m_BonusSceneList.size() > 0 )
	{
		if ( !IsBonusScene() )
		{
			if(cFishApp::m_spGameSaveData->m_pSaveData)
				cFishApp::m_spGameSaveData->m_pSaveData->m_iBonusGameWinner = -1;
			m_bBonusGameLeave = false;
			ChangeBonusScene();
			
			ShutDownShake();
			cFishApp::m_ShakeTm = 3.0f;
			cFishApp::m_bShake = true;

			if (m_pCollectionPlate)
			{
				m_pShowChangeBonusSceneMPDI = m_pCollectionPlate->GetLastCollectionShow();
				m_pShowChangeBonusSceneMPDI->Init();

				BYTE ucTarget;
				m_pCollectionPlate->IsCollectionGameArrive(ucTarget);
				cBasicSound*l_pSound = (ucTarget == 9)?m_pDragonBallMaxSound: m_pDragonBall78Sound;
				if (l_pSound)
					l_pSound->Play(true);
			}

			m_eSceneChangeStatus = eSCS_PLAY_SCENE_CHANGE_MPDI;
			SetBonusScene(true);
			m_bBonusGameReady = true;
			if (cFishApp::m_spControlPanel)
				m_i64PastTotalIn = cFishApp::m_spControlPanel->GetTotalBet();

			cPlayerData* l_pPlayer;
			int l_iNumPlayer = 0;
			if (cFishApp::m_spControlPanel && cFishApp::m_spPlayerManager)
				l_iNumPlayer = cFishApp::m_spControlPanel->GetPlayerCount();
			for (int i = 0; i<l_iNumPlayer; i++)
			{
				l_pPlayer = (cPlayerData*)cFishApp::m_spPlayerManager->GetObject(i);
				if (l_pPlayer != NULL)
					l_pPlayer->CancelLock();
			}
		}

		if(cFishApp::m_spMonsterManager && cFishApp::m_spMonsterManager->f_GetCountMonsterAlive() <= 1 && cFishApp::m_spMonsterManager->f_GetCountMonsterAlive() > 0 )
		{
			if(!m_bTimeToLeave && !m_bBonusGameLeave && !m_bBonusGameEnd && !m_bBonusGameMassage && m_bSceneIsBonus && m_bIsBonusScene)
				if(m_pFishGroupByPrizeGame)
					m_pFishGroupByPrizeGame->RequirePrizeGame();
		}
	}
	else
	{
		if( m_ChangeSceneCircle.bTragetTimrReached )
		{
			int	l_iValue = rand(0,this->m_iTotalProbability);

			if ( m_NormalBG.m_iProbability > l_iValue )
			{
				m_bSceneIsSpecial = false;
				ChangeScene();
			}
			else
			{
				if(!IsAllowToChangeScene())
				{
					StartMiniGame();
					return;
				}
				m_bSceneIsSpecial = true;
				if( m_SpecialSceneList.size() )
				{
					ChangeSpecialScene();
				}
				else
				{
					ChangeScene();
				}
			}
			m_ChangeSceneCircle.SetTargetTime(m_TimeCircleRestrict.Rand());
			if ( m_pShowChangeSceneMPDI )
				m_pShowChangeSceneMPDI->Init();
			m_eSceneChangeStatus = eSCS_PLAY_SCENE_CHANGE_MPDI;
		}
	}
}

void	cSceneChange::SpecialSceneUpdate(float e_fElpaseTime)
{
	if( cFishApp::m_spProbabilityFish )
	{
		if(cFishApp::m_spProbabilityFish->IsPauseGenerateMonster())
		{
			m_StopGenerateFishTime.Update(e_fElpaseTime);
			LeaveCheck(e_fElpaseTime);
			if(cFishApp::m_spMiniGameManager && cFishApp::m_spMonsterManager)
			if( cFishApp::m_spMonsterManager->f_GetCountMonsterAlive() == 0 )
			{
				if( m_bSceneIsBonus )
				{
					if(m_bBonusGameLeave)
					{
						if(m_pFishGroupByPrizeGame)
						{
							m_pFishGroupByPrizeGame->StopPrizeGame();
							m_pFishGroupByPrizeGame = 0;
							if (m_pShow4Sound)
								m_pShow4Sound->Play(false);
						}
						if( cFishApp::m_spGameSaveData->m_pSaveData->m_iBonusGameWinner != -1)
						{
							m_iBonusGameFundin = 0;

							int l_iNumPlayer = 6;
							if (cFishApp::m_spControlPanel)
								l_iNumPlayer = cFishApp::m_spControlPanel->GetPlayerCount();
							for (int i = 0; i<l_iNumPlayer; i++)
							{
								cPlayerBehaviorBase *pPlayer = cFishApp::m_spPlayerManager->GetObject(i);
								int64 i64Bonus = cFishApp::m_spGameSaveData->m_pProbabilitySaveData->m_i64PlayerDragonBigWin[pPlayer->GetPlayerID()];
								if (i64Bonus > m_iBonusGameFundin)
								{
									m_iBonusGameFundin = (int)i64Bonus;
									cFishApp::m_spGameSaveData->m_pSaveData->m_iBonusGameWinner = i;
								}
							}
							std::wstring l_sDragonBallName;
							wchar_t l_sTemp[3];
							swprintf(l_sTemp, 3, L"%d", cFishApp::m_spGameSaveData->m_pSaveData->m_iBonusGameWinner);
							(l_sDragonBallName = L"DB9Messabe_2_").append(l_sTemp);	
							m_pBonusGameMassageMPDI = m_pDragonBallBaseMPDIList->GetObject(l_sDragonBallName.c_str());
							if(m_pBonusGameMassageMPDI)
								m_pBonusGameMassageMPDI->Init();

							
							cMPDIList*l_pPlayerMPDIList = NULL;
							auto l_strFileName = cFishApp::m_spLanguageFile->GetFileName(PLAYERUI_MPDI);
							if(l_strFileName)
								l_pPlayerMPDIList = cGameApp::GetMPDIListByFileName(l_strFileName);
							assert(l_pPlayerMPDIList);
							m_pBonusGameWinnerMPDI = l_pPlayerMPDIList->GetObject(L"Crown");
							m_pBonusGameWinnerMPDI->Init();
							if(m_pBonusGameWinnerMPDI)
							{
								cPlayerBehaviorBase* l_pPlayer = cFishApp::m_spPlayerManager->GetObject(cFishApp::m_spGameSaveData->m_pSaveData->m_iBonusGameWinner);
								Vector3 pos = l_pPlayer->GetPlayerPosition();
								Vector3 angle = l_pPlayer->GetPlayerRotation();
								m_PlayerFrame.SetWorldTransform(cMatrix44::TranslationMatrix( pos )* cMatrix44::RotationMatrix( angle ));
								if (!m_pBonusGameWinnerMPDI->GetParent())
									m_pBonusGameWinnerMPDI->SetParent(&m_PlayerFrame, false);
							}
							

							cFishApp::m_spGameSaveData->m_pSaveData->m_iBonusGameWinner = -1;
							m_bBonusGameMassage = true;
							StarTakeDragonWin();
						}
						else
						{
							m_bPalyerResume = false;
							m_bBonusGameEnd = true;
							if(m_pOutBonusSceneMPDI)
							{
								m_pOutBonusSceneMPDI->Init();
							}
						}
						m_bBonusGameLeave = false;
					}

					if(m_pBonusGameMassageMPDI)
					{
						m_pBonusGameMassageMPDI->Update(e_fElpaseTime);
						if(TakeDragonWin (e_fElpaseTime))
						{
							if(m_bBonusGameMassage)
							{
								m_bBonusGameMassage = false;
								m_bBonusGameEnd = true;
								m_bPalyerResume = false;
								if(m_pOutBonusSceneMPDI)
								{
									m_pOutBonusSceneMPDI->Init();
								}
							}
						}
					}
					if(m_pOutBonusSceneMPDI)
					{
						m_pOutBonusSceneMPDI->Update(e_fElpaseTime);
						if( !m_bPalyerResume )
						{
							m_bPalyerResume = true;
							//子彈歸還
							ReturnBullet();
							int l_iNumPlayer = 6;
							if (cFishApp::m_spControlPanel)
								l_iNumPlayer = cFishApp::m_spControlPanel->GetPlayerCount();
							for (int i = 0; i<l_iNumPlayer; i++)
							{
								cPlayerBehaviorBase *pPlayer = cFishApp::m_spPlayerManager->GetObject(i);
								pPlayer->m_pWeapon->SetPower( pPlayer->m_iCurrentPower );
							}
						}
						if(m_bBonusGameEnd)
						{
							if(m_pOutBonusSceneMPDI->IsAnimationDone())
							{
								m_bBonusGameEnd = false;
								int l_iNumPlayer = 6;
								if (cFishApp::m_spControlPanel)
									l_iNumPlayer = cFishApp::m_spControlPanel->GetPlayerCount();
								for (int i = 0; i<l_iNumPlayer; i++)
								{
									cPlayerBehaviorBase *pPlayer = cFishApp::m_spPlayerManager->GetObject(i);
									( (cPlayerData*)pPlayer )->m_bIsBonusGamePower = false;
									pPlayer->AddScoreByMoneyChangeType(cFishApp::m_spGameSaveData->m_pProbabilitySaveData->m_i64PlayerDragonBigWin[i], eMCT_DRAGON_BIG_WIN_ADD_SCORE);
								}
								if (m_pCollectionPlate)
									m_pCollectionPlate->SetShowCollectionEnd(true);
								m_bSceneIsBonus = false;
								m_bSceneIsSpecial = false;
								cFishApp::m_spProbabilityFish->SetPauseGenerateMonster(false);
								cFishApp::m_spProbabilityFish->SetSpecialScene (false);
								cFishApp::m_spProbabilityFish->SetBonusScene(false);
								cFishApp::m_spMonsterManager->CollectionFishShowUp(true);

								cPlayerData* l_pPlayer;
								for (int i = 0; i<l_iNumPlayer; i++)
								{
									l_pPlayer = (cPlayerData*)cFishApp::m_spPlayerManager->GetObject(i);
									if (l_pPlayer != NULL)
										l_pPlayer->RestoreLock();
								}
								CheckBGSound();
							}
						}
					}
				}
				else
				{

					if(m_pFishGroupByPrizeGame)
					{
						m_pFishGroupByPrizeGame->StopPrizeGame();
						m_pFishGroupByPrizeGame = 0;
						ChangeTurnOutMPDI();
					}

					if(m_pOutBonusSceneMPDI)
					{
						if(!m_pOutBonusSceneMPDI->IsAnimationDone())
							m_pOutBonusSceneMPDI->Update(e_fElpaseTime);
						else
						{
							m_bSceneIsSpecial = false;
							cFishApp::m_spProbabilityFish->SetPauseGenerateMonster(false);
							cFishApp::m_spProbabilityFish->SetSpecialScene (false);
							cFishApp::m_spProbabilityFish->SetBonusScene(false);
							CheckBGSound();
						}
					}
					else
					{
						m_bSceneIsSpecial = false;
						cFishApp::m_spProbabilityFish->SetPauseGenerateMonster(false);
						cFishApp::m_spProbabilityFish->SetSpecialScene (false);
						cFishApp::m_spProbabilityFish->SetBonusScene(false);
						CheckBGSound();
					}
				}
			}
		}
	}
}

void	cSceneChange::LeaveBonusGame()
{
	if (!m_pCollectionPlate)
		return;
	if (!m_bTimeToLeave)
	{
		if (m_bSceneIsBonus)
		{
			m_fBounsGameBossNoPlayerHurtTC = 0;
			m_bBonusGameLeave = true;
			m_bTimeToLeave = true;
			cFishApp::m_spMonsterManager->MonsterLeaveScene();
			if (m_pCollectionPlate)
				m_pCollectionPlate->ClearCollection();
			if(m_pFishGroupByPrizeGame)
			{
				m_pFishGroupByPrizeGame->StopPrizeGame();
				m_pFishGroupByPrizeGame = 0;
			}
		}
		else
		{
			m_bTimeToLeave = true;
		}
		cFishApp::m_spMonsterManager->MonsterLeaveScene();
	}
}

void	cSceneChange::ReturnBullet()
{
	if(cFishApp::m_spBulletManager && cFishApp::m_spBulletManager->m_iNumWorking > 0 )
	{
		WCHAR* CoinEffectName = L"SilverCoin";
		eWinMoneyEffectCoinCount l_eWinMoneyEffectCoinCount;
		l_eWinMoneyEffectCoinCount = eWinMoneyEffectCoinCount_Triple;

		for(int i=0;i<cFishApp::m_spBulletManager->m_iNumWorking;++i)
		{
			cBullet*l_pBullet = cFishApp::m_spBulletManager->m_ppUpdateBulletData[i];
			eBulletStatus l_eStatus = l_pBullet->f_GetBulletStatus();
			if( l_eStatus == eBS_MOVING || l_eStatus == eBS_WAIT_FOR_FETCH || l_eStatus == eBS_TRACING_MONSTER )
			{
				cPlayerBehaviorBase *l_pPlayer = l_pBullet->GetBulletOwner();
				l_pPlayer->AddScoreByMoneyChangeType(l_pBullet->GetPayRate(),eMoneyChangeType::eMCT_FIRE_RETURN_BULLET_ADD_SCORE);
				cFishApp::m_spWinMoneyEffectManager->RequireWinMoneyEffectBase( CoinEffectName, 
					cMatrix44::TranslationMatrix( l_pBullet->f_GetBulletPos() ) * cMatrix44::RotationMatrix( l_pPlayer->GetPlayerRotation() ), 
					l_pBullet->GetPayRate(), ( (cPlayerData*)l_pPlayer ), l_eWinMoneyEffectCoinCount );
				
			}
			l_pBullet->Init();
		}
		cFishApp::m_spBulletManager->m_iNumWorking = 0;
	}
}

bool	cSceneChange::TakeDragonWin (float e_fElpaseTime)
{
	bool	bDone = true;
	int64	i64Credit = (int64)( e_fElpaseTime / 1.0 * m_fTakeWinSpeed );

	int l_iNumPlayer = 6;
	if (cFishApp::m_spControlPanel)
		l_iNumPlayer = cFishApp::m_spControlPanel->GetPlayerCount();
	for (int i = 0; i<l_iNumPlayer; i++)
	{
		cPlayerBehaviorBase*	pPlayer = cFishApp::m_spPlayerManager->GetObject(i);
		int64					i64Win = cFishApp::m_spGameSaveData->m_pProbabilitySaveData->m_i64PlayerDragonBigWin[i];

		if (0 < i64Win)
		{
			if (i64Credit < i64Win)
				i64Win = i64Credit;
			pPlayer->AddScoreByMoneyChangeType(i64Win, eMCT_DRAGON_BIG_WIN_ADD_SCORE);
		}
		if (0 < cFishApp::m_spGameSaveData->m_pProbabilitySaveData->m_i64PlayerDragonBigWin[i])
			bDone = false;
	}
	if (bDone && m_fTakeWinTime > TAKE_WIN_TIME)
	{
		m_fTakeWinTime = 0;
		if (m_pDBTakeWinSound)
			m_pDBTakeWinSound->Play(false);
		return true;
	}
	else if(bDone)
	{
		m_fTakeWinTime += e_fElpaseTime;
		if (m_pDBTakeWinSound)
			m_pDBTakeWinSound->Play(false);
		return false;
	}
	else
	{
		m_fTakeWinTime += e_fElpaseTime;
		m_fTakeWinSpeed = ((int)m_fTakeWinTime + 1) * m_fTakeWinSpeedBase;
		return false;
	}
}
//以動畫撥放時間和最高分數來算出所需的速度
void	cSceneChange::StarTakeDragonWin ()
{
	int iSegment = 0;
	iSegment = (TAKE_WIN_TIME_LIMIT + 1) * TAKE_WIN_TIME_LIMIT / 2;
	
	if(m_iBonusGameFundin > TAKE_WIN_SCORE_LOWER_LIMIT)
	{
		m_fTakeWinSpeed = (float)m_iBonusGameFundin / iSegment;
		m_fTakeWinSpeedBase = (float)m_iBonusGameFundin / iSegment;
	}
	else
	{
		m_fTakeWinSpeed = (float)TAKE_WIN_SCORE_LOWER_LIMIT / iSegment;
		m_fTakeWinSpeedBase = (float)TAKE_WIN_SCORE_LOWER_LIMIT / iSegment;

	}
	if (m_pDBTakeWinSound)
		m_pDBTakeWinSound->Play(true);
}

void	cSceneChange::AddCollectionGame(cPlayerData* e_pPlayer)
{
	if (m_pCollectionPlate)
		m_pCollectionPlate->AddCollectionGame(e_pPlayer);
}

bool	cSceneChange::IsAppearCollectionGameFinish()
{
	if(m_pCollectionPlate)
		return m_pCollectionPlate->IsAppearCollectionGameFinish();
	return true;
}

void	cSceneChange::ResetCollection()
{
	if (m_pCollectionPlate)
		m_pCollectionPlate->UpdatePlayerCollectionGame();
}

// ============== private start ==============

void	cSceneChange::LeaveCheck(float e_fElpaseTime)
{
	while (1)
	{
		int	l_iMonsterCount = cFishApp::m_spMonsterManager->f_GetCountMonsterAlive();
		if (m_bSceneIsBonus && !m_bBonusGameLeave)
		{
			if (l_iMonsterCount > 0)
			{
				//時間到後判斷是否BOSS沒人打
				m_fBounsGameBossNoPlayerHurtTC += e_fElpaseTime;
				if(m_fBounsGameBossNoPlayerHurtTC > BONUS_BOSS_NOT_GET_HURT_ALLOW_TIME)
				{
					LeaveBonusGame();
					break;
				}
			}
			if (m_StopGenerateFishTime.bTragetTimrReached)
			{
				LeaveBonusGame();
				break;
			}

			m_fBonusTime += e_fElpaseTime;
			if (m_pCollectionPlate)
			{
				if (m_fBonusTime >= m_pCollectionPlate->GetMaxBonusTime())
					LeaveBonusGame();
				else 
				if (0 != m_iBonusDeadTime && m_fBonusTime >= m_pCollectionPlate->GetMaxBonusTime() - m_iBonusDeadTime)
				{
					if(this->m_pCollectionPlate)
						g_ProbLibHandler->SetCollectionToDeadTime();
					m_iBonusDeadTime = 0;
				}
				break;
			}
		}
		else 
		if (m_StopGenerateFishTime.bTragetTimrReached || l_iMonsterCount < 1)
			LeaveBonusGame();
		break;
	}
}

void					cSceneChange::LockChangeBet()
{
	int l_iNumPlayer = 0;
	if (cFishApp::m_spControlPanel && cFishApp::m_spPlayerManager)
		l_iNumPlayer = cFishApp::m_spControlPanel->GetPlayerCount();
	for (int i = 0; i<l_iNumPlayer; i++)
	{
		cPlayerBehaviorBase *pPlayer = cFishApp::m_spPlayerManager->GetObject(i);
		pPlayer->m_iCurrentPower = pPlayer->m_pWeapon->GetPower();
		if (cFishApp::m_spControlPanel)
		{
			if (pPlayer->m_iCurrentPower < cFishApp::m_spControlPanel->GetMinBet())
				pPlayer->m_pWeapon->SetBonusGamePower(pPlayer->m_iCurrentPower);
			else
				pPlayer->m_pWeapon->SetBonusGamePower(cFishApp::m_spControlPanel->GetMinBet());
		}
		( (cPlayerData*)pPlayer )->m_bIsBonusGamePower = true;
	}
}

bool	cSceneChange::IsAllowToChangeScene()
{
	if (cFishApp::m_spPlayerManager)
	{
		int l_iNumPlayer = 6;
		if (cFishApp::m_spControlPanel)
			l_iNumPlayer = cFishApp::m_spControlPanel->GetPlayerCount();
		for (int i = 0; i<l_iNumPlayer; i++)
		{
			cPlayerBehaviorBase *pPlayer = cFishApp::m_spPlayerManager->GetObject(i);
			if (((cPlayerData*)pPlayer)->GetWaitMortar() && !((cPlayerData*)pPlayer)->GetUsedMortar())
				return false;
			if (!pPlayer->IsNormalWeapon())
				return false;
		}
	}
	if (cFishApp::m_spWinMoneyEffectManager && !cFishApp::m_spWinMoneyEffectManager->IsBigWinFinish())
		return false;

	return true;

}

void	cSceneChange::StartMiniGame()
{
	int l_iNumPlayer = 6;
	if (cFishApp::m_spControlPanel)
		l_iNumPlayer = cFishApp::m_spControlPanel->GetPlayerCount();
	for (int i = 0; i<l_iNumPlayer; i++)
	{
		cPlayerBehaviorBase *pPlayer = cFishApp::m_spPlayerManager->GetObject(i);
		((cPlayerData*)pPlayer)->MortarFire();
	}

}

void	cSceneChange::LockWeaponFire()
{
	bool bFireLockReady = true;
	if (cFishApp::m_spPlayerManager)
	{
		int l_iNumPlayer = 6;
		if (cFishApp::m_spControlPanel)
			l_iNumPlayer = cFishApp::m_spControlPanel->GetPlayerCount();
		for (int i = 0; i<l_iNumPlayer; i++)
		{
			cPlayerBehaviorBase *pPlayer = cFishApp::m_spPlayerManager->GetObject(i);
			if (!pPlayer->IsNormalWeapon())
				bFireLockReady = false;
		}
		if (bFireLockReady)
		{
			for (int i = 0; i<l_iNumPlayer; i++)
			{
				cPlayerBehaviorBase *pPlayer = cFishApp::m_spPlayerManager->GetObject(i);
				pPlayer->PlayerDenyAction(FIRE_DENY, true);
			}
		}
	}
}

void	cSceneChange::UnlockWeaponFire()
{
	if (cFishApp::m_spPlayerManager)
	{
		int l_iNumPlayer = 6;
		if (cFishApp::m_spControlPanel)
			l_iNumPlayer = cFishApp::m_spControlPanel->GetPlayerCount();
		for (int i = 0; i<l_iNumPlayer; i++)
		{
			cPlayerBehaviorBase *pPlayer = cFishApp::m_spPlayerManager->GetObject(i);
			pPlayer->PlayerDenyAction(FIRE_DENY, false);
		}
	}
}

void cSceneChange::BonusGameWinnerMessage()
{
	if (m_bSceneIsBonus)
	{
		if (m_pBonusGameMassageMPDI)
		{
			if (m_bBonusGameMassage)
			{
				m_pBonusGameMassageMPDI->Render();
				if (m_pBonusGameMassageNumerial && !m_pBonusGameMassageMPDI->IsAnimationDone())
				{
					if (m_pBonusGameWinnerMPDI)
						m_pBonusGameWinnerMPDI->Render();

					cMPDI* l_pNumD = m_pDragonBallBaseMPDIList->GetObject(L"DB9NumerialPos_D");
					cMPDI* l_pNumU = m_pDragonBallBaseMPDIList->GetObject(L"DB9NumerialPos_U");
					cMPDI* l_pNumL = m_pDragonBallBaseMPDIList->GetObject(L"DB9NumerialPos_L");
					cMPDI* l_pNumR = m_pDragonBallBaseMPDIList->GetObject(L"DB9NumerialPos_R");

					Vector4 *pTempColor = m_pBonusGameMassageNumerial->GetColor();
					std::vector<Vector3> l_vTemp;
					Vector3 NumTexPos, DrawNumPos;
					cMatrix44 TempMatrix;
					cSubMPDI* l_pSubPosMPDI = l_pNumD->GetObject(L"DB9NumerialPos_D");
					if (l_pSubPosMPDI)
					{
						l_vTemp = l_pSubPosMPDI->GetOriginalPointList();
						NumTexPos = l_vTemp[0];
						NumTexPos.x -= 11.f;
						TempMatrix = cMatrix44::TranslationMatrix(NumTexPos);
						DrawNumPos = TempMatrix.TransformCoordinate(Vector3(0, 0, 0));
						m_pBonusGameMassageNumerial->SetAngle(0);
						m_pBonusGameMassageNumerial->Draw(m_iBonusGameFundin, (int)DrawNumPos.x, (int)DrawNumPos.y, 0, true);
					}

					l_pSubPosMPDI = l_pNumU->GetObject(L"DB9NumerialPos_U");
					if (l_pSubPosMPDI)
					{
						l_vTemp = l_pSubPosMPDI->GetOriginalPointList();
						NumTexPos = l_vTemp[0];
						NumTexPos.x += 65.f;
						NumTexPos.y += 51.f;
						TempMatrix = cMatrix44::TranslationMatrix(NumTexPos);
						DrawNumPos = TempMatrix.TransformCoordinate(Vector3(0, 0, 0));
						m_pBonusGameMassageNumerial->SetAngle(180);
						m_pBonusGameMassageNumerial->Draw(m_iBonusGameFundin, (int)DrawNumPos.x, (int)DrawNumPos.y, 0, true);
					}

					l_pSubPosMPDI = l_pNumL->GetObject(L"DB9NumerialPos_L");
					if (l_pSubPosMPDI)
					{
						l_vTemp = l_pSubPosMPDI->GetOriginalPointList();
						NumTexPos = l_vTemp[0];
						NumTexPos.x += 55.f;
						TempMatrix = cMatrix44::TranslationMatrix(NumTexPos);
						DrawNumPos = TempMatrix.TransformCoordinate(Vector3(0, 0, 0));
						m_pBonusGameMassageNumerial->SetAngle(90);
						m_pBonusGameMassageNumerial->Draw(m_iBonusGameFundin, (int)DrawNumPos.x, (int)DrawNumPos.y, 0, true);
					}

					l_pSubPosMPDI = l_pNumR->GetObject(L"DB9NumerialPos_R");
					if (l_pSubPosMPDI)
					{
						l_vTemp = l_pSubPosMPDI->GetOriginalPointList();
						NumTexPos = l_vTemp[0];
						NumTexPos.x += 4.f;
						NumTexPos.y += 66.f;
						TempMatrix = cMatrix44::TranslationMatrix(NumTexPos);
						DrawNumPos = TempMatrix.TransformCoordinate(Vector3(0, 0, 0));
						m_pBonusGameMassageNumerial->SetAngle(270);
						m_pBonusGameMassageNumerial->Draw(m_iBonusGameFundin, (int)DrawNumPos.x, (int)DrawNumPos.y, 0, true);
					}

				}
			}
		}
		if (m_pOutBonusSceneMPDI)
			if (m_bBonusGameEnd)
				m_pOutBonusSceneMPDI->Render();
	}

	if (eSCS_NONE == m_eSceneChangeStatus)
	{
	}
	else
	{
		if (eSCS_PLAY_SCENE_CHANGE_MPDI == m_eSceneChangeStatus)
		{
			if (m_pShowChangeBonusSceneMPDI && m_bSceneIsBonus)
				m_pShowChangeBonusSceneMPDI->Render();
		}
		else
		if (eSCS_BONUS_ALERT_GO == m_eSceneChangeStatus || eSCS_WAIT_BONUS_ALERT_DISSAPEAR == m_eSceneChangeStatus)
		{
			if (m_bSceneIsBonus)
			{
				if (m_pChangeBonusSceneMPDI)
					m_pChangeBonusSceneMPDI->Render();
			}
		}
		else 
		if (eSCS_BUBBLE_GO == m_eSceneChangeStatus)
		{
			if(m_pChangeBonusSceneMPDI)
				m_pChangeBonusSceneMPDI->Render();
		}
	}
	if (m_pOutBonusSceneMPDI)
		if (!m_pOutBonusSceneMPDI->IsAnimationDone())
			m_pOutBonusSceneMPDI->Render();
}

// ============== private end ==============