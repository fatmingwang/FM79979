#include "stdafx.h"
#include "BluffingGirlApp.h"
#include "AllPhaseDefine.h"
#include "MainMenuPhase.h"
#include "IABSetup.h"
#include "StorePhase.h"
#include "FBPhase.h"
#include "ChoiceGirlPhase.h"
#include "BluffingGirlPhaseManager.h"
#include "SetupPhase.h"
#include "ChoiceGirlPhase.h"
#include "ChoiceGirlStagePhase.h"
#include "BluffingDiceGame.h"
#include "WinPrizePhase.h"
#include "PlayerData.h"
#include "HeartRecoverPhase.h"
#include "StageClearFile.h"
#include "HeartRecoverSelectGirlPhase.h"
#include "SetupPhase.h"
#include "GirlIntroducingPhase.h"
#include "GameRulePhase.h"
#include "ShowLogoPhase.h"
#include "NetWorkConnectPhase.h"
#include "GameNetwork.h"
#include "BluffingDiceMultiPlayerGame.h"
#include "TitlePhase.h"
#include "FBFunction.h"
namespace FATMING_CORE
{
	extern LoadingProgress	g_pLoadingProgressCallBack;
}

void	BGLoading()
{
	cGameApp::m_sTimeAndFPS.Update();
	if(cGameApp::m_spSoundParser)
		cGameApp::m_spSoundParser->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
}

int	g_iSpeed = 1;
cShowLogoPhase*g_pShowLogoPhase = 0;
cFreetypeGlyphRender*g_pFreetypeGlyphRender = nullptr;
void	WTLoading()
{
	cGameApp::m_sTimeAndFPS.Update();
	static float	l_sfElpaseTime = 0.f;
	l_sfElpaseTime += cGameApp::m_sTimeAndFPS.fElpaseTime;
	if( l_sfElpaseTime > 0.1f )
	{
		l_sfElpaseTime = 0.f;
	}
	else
		return;
	//if(cGameApp::m_spSoundParser)
	//	cGameApp::m_spSoundParser->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	UseShaderProgram(DEFAULT_SHADER);
	Vector4 l_vViewPort = cGameApp::m_svViewPortSize;
	glViewport((int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y,(int)cGameApp::m_svViewPortSize.Width(),(int)cGameApp::m_svViewPortSize.Height());
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	UseShaderProgram();
	if( g_pShowLogoPhase )
	{
		g_pShowLogoPhase->Update(0.032f);
		g_pShowLogoPhase->Render();
	}
#ifdef ANDROID
	cGameApp::m_pNvEGLUtil->swap();
#endif
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}
//#include "../../Core/AVIPlayer/FFMpegPlayer.h"
cBluffingGirlApp*g_pBluffingGirlApp = 0;
cPlayerData*	cBluffingGirlApp::m_spPlayerData = 0;
POINT	g_RealMousePos = {0,0};
cUIImage*g_pCanvasImage = 0;
cStageClearFile*g_pStageClearFile = 0;
cFBFunction*g_pFBFunction = 0;
cGameNetwork*		cBluffingGirlApp::m_spGameNetwork = 0;

#if defined(ANDROID)
cBluffingGirlApp::cBluffingGirlApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize,e_pNvEGLUtil)
#elif defined(WIN32)
cBluffingGirlApp::cBluffingGirlApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cBluffingGirlApp::cBluffingGirlApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	cGameApp::m_spSoundParser->Parse("BluffingGirl/Sound/Sound.xml");
	m_sfVersion = 1.f;
	*m_psstrGameAppName = "BluffingGirlEN";
	g_pLoadingProgressCallBack = WTLoading;
	g_pBluffingGirlApp = this;
	m_bLeave = false;
	m_pPhaseManager = 0;
#ifdef DEBUG
	this->m_sbSpeedControl = true;
#endif
	g_pStageClearFile = new cStageClearFile();
#ifndef WASM
	m_spGameNetwork = new cGameNetwork();
#endif
}

cBluffingGirlApp::~cBluffingGirlApp()
{
	SAFE_DELETE(m_pPhaseManager);
	SAFE_DELETE(m_spPlayerData);
	SAFE_DELETE(g_pCanvasImage);
	SAFE_DELETE(g_pStageClearFile);
	SAFE_DELETE(m_spGameNetwork);
	SAFE_DELETE(g_pFBFunction);
	SAFE_DELETE(g_pFreetypeGlyphRender);
	Destroy();
	g_pBluffingGirlApp = 0;
}

void	cBluffingGirlApp::Init()
{
	FMLog::LogWithFlag("cBluffingGirlApp::Init start \n", CORE_LOG_FLAG);
#ifndef WASM
	cGameApp::Init();
#endif
	//SetAcceptRationWithGameresolution(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height());
	////for sound volume
	m_pPhaseManager = new cBluffingGirlPhaseManager();

	cSetupPhase*l_pSetupPhase = new cSetupPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pSetupPhase);
	cGameApp::OutputDebugInfoString(L"new g_pShowLogoPhase");
	g_pShowLogoPhase = new cShowLogoPhase();
	m_pPhaseManager->AddObjectNeglectExist(g_pShowLogoPhase);
	m_pPhaseManager->SetCurrentCurrentPhase(g_pShowLogoPhase->GetName());
	cGameApp::GetPuzzleImageByFileName(L"BluffingGirl/Image/Mm_Photo_001.pi");
	//common file parse
	cMPDIList*l_pCommonMPDIList = cGameApp::GetMPDIListByFileName(L"BluffingGirl/Image/Main_Massage.mpdi");
	cGameApp::OutputDebugInfoString(L"new cTitlePhase");
	cTitlePhase*l_pTitlePhase = new cTitlePhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pTitlePhase);
	//
	cGameApp::OutputDebugInfoString(L"new cMainMenuPhase");
	cMainMenuPhase*l_pMainMenuPhase = new cMainMenuPhase("BluffingGirl/PhaseData/MainMenuPhase.xml");
	this->m_pPhaseManager->AddObjectNeglectExist(l_pMainMenuPhase);
	//
	cGameApp::OutputDebugInfoString(L"new cStorePhase");
	cStorePhase*l_pIABMenu = new cStorePhase();	l_pIABMenu->Parse("BluffingGirl/IAB/IABMenu.xml");
	this->m_pPhaseManager->AddObjectNeglectExist(l_pIABMenu);
	//
	cGameApp::OutputDebugInfoString(L"new cFBPhase");
	cFBPhase*l_pFBPhase = new cFBPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pFBPhase);
	//
	cGameApp::OutputDebugInfoString(L"new cChoiceGirlStagePhase");
	cChoiceGirlStagePhase*l_pChoiceGirlStagePhase = new cChoiceGirlStagePhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pChoiceGirlStagePhase);
	//
	cGameApp::OutputDebugInfoString(L"new cChoiceGirlPhase");
	cChoiceGirlPhase*l_pChoiceGirlPhase = new cChoiceGirlPhase("BluffingGirl/PhaseData/ChoiceGirlPhase.xml");
	cGameApp::OutputDebugInfoString(L"after new cChoiceGirlPhase");
	this->m_pPhaseManager->AddObjectNeglectExist(l_pChoiceGirlPhase);
	///
	cGameApp::OutputDebugInfoString(L"new cBluffingDiceGame");
	cBluffingDiceGame*l_pBluffingDiceGame = new cBluffingDiceGame();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pBluffingDiceGame);
	//
	cGameApp::OutputDebugInfoString(L"new cWinPrizePhase");
	cWinPrizePhase*l_pWinPrizePhase = new cWinPrizePhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pWinPrizePhase);
	//
	cGameApp::OutputDebugInfoString(L"new cHeartRecoverSelectGirlPhase");
	cHeartRecoverSelectGirlPhase*l_pHeartRecoverSelectGirlPhase = new cHeartRecoverSelectGirlPhase("BluffingGirl/PhaseData/HeartRecoverChoiceGirlPhase.xml");
	this->m_pPhaseManager->AddObjectNeglectExist(l_pHeartRecoverSelectGirlPhase);
	//
	cGameApp::OutputDebugInfoString(L"new cHeartRecoverPhase");
	cHeartRecoverPhase*l_pHeartRecoverPhase = new cHeartRecoverPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pHeartRecoverPhase);
	//
	cGameApp::OutputDebugInfoString(L"new cGirlIntroducingPhase");
	cGirlIntroducingPhase*l_pGirlIntroducingPhase = new cGirlIntroducingPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pGirlIntroducingPhase);
	//
	cGameApp::OutputDebugInfoString(L"new cGameRulePhase");
	cGameRulePhase*l_pGameRulePhase = new cGameRulePhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pGameRulePhase);
	//
	cGameApp::OutputDebugInfoString(L"new cNetWorkConnectPhase");
	cNetWorkConnectPhase*l_pNetWorkConnectPhase = new cNetWorkConnectPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pNetWorkConnectPhase);
	//
	cGameApp::OutputDebugInfoString(L"new cBluffingDiceMultiPlayerGame");
	cBluffingDiceMultiPlayerGame*l_pBluffingDiceMultiPlayerGame = new cBluffingDiceMultiPlayerGame();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pBluffingDiceMultiPlayerGame);
	//
	m_spPlayerData = new cPlayerData("BluffingGirl/PlayerData/PlayerItemData.xml","PlayerData.xml");
	m_spPlayerData->Init();
	//
	//m_pPhaseManager->SetCurrentCurrentPhase(l_pHeartRecoverPhase->GetName());
	//m_pPhaseManager->SetCurrentCurrentPhase(l_pWinPrizePhase->GetName());
	//m_pPhaseManager->SetCurrentCurrentPhase(l_pMainMenuPhase->GetName());
	//m_pPhaseManager->SetCurrentCurrentPhase(l_pBluffingDiceGame->GetName());
	//m_pPhaseManager->SetCurrentCurrentPhase(l_pChoiceGirlPhase->GetName());
	//m_pPhaseManager->SetCurrentCurrentPhase(NET_WORK_CONNECT_PHASE);
	//m_pPhaseManager->SetCurrentCurrentPhase(BLUFFING_MULTI_PLAY_DICEGAME_PHASE_NAME);
	//m_pPhaseManager->SetCurrentCurrentPhase(l_pTitlePhase->GetName());

	m_pPhaseManager->RegisterPopupMessenger("BluffingGirl/PhaseData/ClairvoyantNotEnoughPopupMessenger.xml");
	m_pPhaseManager->RegisterPopupMessenger("BluffingGirl/PhaseData/NotEnoughPopupMessenger.xml");
	m_pPhaseManager->RegisterPopupMessenger("BluffingGirl/PhaseData/GameFailedPopupMessenger.xml");
	//m_pPhaseManager->RegisterPopupMessenger("BluffingGirl/PhaseData/WarningMessengerPopupMessenger.xml");
	m_pPhaseManager->RegisterPopupMessenger("BluffingGirl/PhaseData/BuyGirlPopupMessenger.xml");
	g_pFBFunction = new cFBFunction();
	g_pLoadingProgressCallBack = BGLoading;
	g_pShowLogoPhase->m_bGotoMainPhase = true;
	this->m_sTimeAndFPS.Update();
	FMLog::LogWithFlag("cBluffingGirlApp::Init end \n", CORE_LOG_FLAG);

	//g_pFreetypeGlyphRender = new cFreetypeGlyphRender("fireflysung.ttf",64);
}
bool	g_bTest = false;
void	cBluffingGirlApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
	if( g_pFBFunction )
		g_pFBFunction->Update(e_fElpaseTime);
	if( m_pPhaseManager )
		this->m_pPhaseManager->Update(e_fElpaseTime);
	if( m_spPlayerData )
		m_spPlayerData->Update(e_fElpaseTime);
	if( cGameApp::m_spSoundParser )
		cGameApp::m_spSoundParser->Update(e_fElpaseTime);
}
#ifdef WASM
//void	JSCallRender();
cMPDI*	GetMPDI(const char*e_strFileName,int e_iIndex)
{
	auto l_pMPDIList = cGameApp::GetMPDIListByFileName(ValueToStringW(e_strFileName).c_str());
	if (l_pMPDIList)
	{
		return l_pMPDIList->GetObject(e_iIndex);
	}
	return nullptr;
}
#endif
void	cBluffingGirlApp::Render()
{
	//glViewport((int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y,(int)cGameApp::m_svViewPortSize.Width(),(int)cGameApp::m_svViewPortSize.Height());
	glViewport((int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y,(int)cGameApp::m_svViewPortSize.Width(),(int)cGameApp::m_svViewPortSize.Height());
	//glViewport(0,0,(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height());
	//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0.0,0.0,1 );
	glClearDepth(1.0f);	
	float	l_fVP[16];

	cGameApp::Render();
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
	cMatrix44	l_matProjection;
	//glhOrthof2((float*)l_matProjection.m,-360, 720+360, 1280+640, -640, -10000, 10000);
	//FATMING_CORE::SetupShaderViewProjectionMatrix(l_matProjection,true);
	//GL_ALPHA_TEST only for opengl es1
	//glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glAlphaFunc(GL_GREATER,0.001f);
	if( m_pPhaseManager )
		this->m_pPhaseManager->Render();
	if( m_spPlayerData )
	{
		if( m_pPhaseManager->GetCurrentPhaseName() )
		{
			if( wcscmp(WIN_PRIZE_PHASE,this->m_pPhaseManager->GetCurrentPhaseName()) && wcscmp(HEART_RECOVER_PHASE_NAME,this->m_pPhaseManager->GetCurrentPhaseName()) && wcscmp(GIRL_INTRODUCING_PHASE_NAME,this->m_pPhaseManager->GetCurrentPhaseName())
				&&wcscmp(RULE_PHASE_NAME,this->m_pPhaseManager->GetCurrentPhaseName())
				&&wcscmp(SHOWLOGO_PHASE_NAME,this->m_pPhaseManager->GetCurrentPhaseName())
				&&wcscmp(NET_WORK_CONNECT_PHASE,this->m_pPhaseManager->GetCurrentPhaseName())
				&&wcscmp(BLUFFING_MULTI_PLAY_DICEGAME_PHASE_NAME,this->m_pPhaseManager->GetCurrentPhaseName())
				&&wcscmp(TITLE_PHASE_NAME,this->m_pPhaseManager->GetCurrentPhaseName())
				)
				m_spPlayerData->Render();
		}
	}
#ifdef DEBUG
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);

	if( cGameApp::m_spGlyphFontRender )
	{
	    WCHAR   l_str[MAX_PATH];
	    swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,0,l_str);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,40,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
	    swprintf(l_str,MAX_PATH,L"RealMousePos:%d,%d",g_RealMousePos.x,g_RealMousePos.y);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,20,l_str);
		//std::strin l_stttt = UT::ComposeMsgByFormat("Force%f",g_fForce);
	    //swprintf(l_str,MAX_PATH,L"Force:%f",g_fForce);
	}
	//if( m_pPhaseManager )
	//	this->m_pPhaseManager->DebugRender();
	if (g_pFreetypeGlyphRender)
	{
		wchar_t l_str[4] = { 24185,20320,23064,0 };
		g_pFreetypeGlyphRender->RenderFont(Vector2(100, 100), l_str);
	}
#endif
	RenderPause();
	cGameApp::ShowInfo();
#ifdef WASM
	//JSCallRender();
#endif
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
	glFlush();
}

void	cBluffingGirlApp::MouseDown(int e_iPosX,int e_iPosY)
{
	g_RealMousePos.x = e_iPosX;
	g_RealMousePos.y = e_iPosY;
    cGameApp::MouseDown(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
		this->m_pPhaseManager->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}

void	cBluffingGirlApp::MouseMove(int e_iPosX,int e_iPosY)
{
	g_RealMousePos.x = e_iPosX;
	g_RealMousePos.y = e_iPosY;
    cGameApp::MouseMove(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
		this->m_pPhaseManager->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}

void	cBluffingGirlApp::MouseUp(int e_iPosX,int e_iPosY)
{
	g_RealMousePos.x = e_iPosX;
	g_RealMousePos.y = e_iPosY;
    cGameApp::MouseUp(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
		this->m_pPhaseManager->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}

void	cBluffingGirlApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
	if( m_pPhaseManager )
		this->m_pPhaseManager->KeyDown(e_char);
}

void	cBluffingGirlApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
	if( m_pPhaseManager )
		this->m_pPhaseManager->KeyUp(e_char);
	//if( g_pFBFunction )
	//	g_pFBFunction->DoFBSharePrize();
}

void	cBluffingGirlApp::RegisterPopupSystemMessenger(char*e_strFileName)
{
	if( g_pBluffingGirlApp && g_pBluffingGirlApp->m_pPhaseManager )
	{
		g_pBluffingGirlApp->m_pPhaseManager->RegisterPopupMessenger(e_strFileName);
	}
}

void	cBluffingGirlApp::WakePopupSystemMessenger(WCHAR*e_strName,const WCHAR*e_strPhaseNameName)
{
	if( g_pBluffingGirlApp && g_pBluffingGirlApp->m_pPhaseManager )
	{
		g_pBluffingGirlApp->m_pPhaseManager->WakePopupMessenger(e_strName,e_strPhaseNameName);
	}
}

cPhasePopUpMessager*cBluffingGirlApp::GetPopupSystemMessenger(WCHAR*e_strName)
{
	if( g_pBluffingGirlApp && g_pBluffingGirlApp->m_pPhaseManager )
	{
		return g_pBluffingGirlApp->m_pPhaseManager->GetPopupMessenger(e_strName);
	}
	return 0;
}

cSimplePhase*	cBluffingGirlApp::GetPhase(const WCHAR*e_strPhaseName)
{
	if( g_pBluffingGirlApp &&g_pBluffingGirlApp->m_pPhaseManager )
		return g_pBluffingGirlApp->m_pPhaseManager->GetObject(e_strPhaseName);
	return 0;
}

cSimplePhase*	cBluffingGirlApp::GetCurrentPhase()
{
	if( g_pBluffingGirlApp &&g_pBluffingGirlApp->m_pPhaseManager )
		return g_pBluffingGirlApp->m_pPhaseManager->GetCurrentWorkingObject();
	return 0;
}