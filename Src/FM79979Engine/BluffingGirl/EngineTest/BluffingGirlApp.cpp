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
#ifdef WASM
#include <emscripten.h>
//https://stackify.dev/675027-proper-way-to-handle-sdl2-resizing-in-emscripten
//https://www.tutorialrepublic.com/faq/how-to-detect-screen-resolution-with-javascript.php#:~:text=Answer%3A%20Use%20the%20window.,on%20click%20of%20the%20button.
//https://stackoverflow.com/questions/1038727/how-to-get-browser-width-using-javascript-code


//https://stackoverflow.com/questions/11381673/detecting-a-mobile-browser

EM_JS
(
	bool, IsNotPC, (),
	{
		//let check = false;
		//(function(a) { if (/ (android | bb\d + | meego). + mobile | avantgo | bada\/|blackberry | blazer | compal | elaine | fennec | hiptop | iemobile | ip(hone | od) | iris | kindle | lge | maemo | midp | mmp | mobile. + firefox | netfront | opera m(ob | in)i | palm(os) ? | phone | p(ixi | re)\/|plucker | pocket | psp | series(4 | 6)0 | symbian | treo | up\.(browser | link) | vodafone | wap | windows ce | xda | xiino | android | ipad | playbook | silk / i.test(a)||/1207 | 6310 | 6590 | 3gso | 4thp | 50[1 - 6]i | 770s | 802s | a wa | abac | ac(er | oo | s\ - ) | ai(ko | rn) | al(av | ca | co) | amoi | an(ex | ny | yw) | aptu | ar(ch | go) | as(te | us) | attw | au(di | \ - m | r | s) | avan | be(ck | ll | nq) | bi(lb | rd) | bl(ac | az) | br(e | v)w | bumb | bw\ - (n | u) | c55\/|capi | ccwa | cdm\ - | cell | chtm | cldc | cmd\ - | co(mp | nd) | craw | da(it | ll | ng) | dbte | dc\ - s | devi | dica | dmob | do(c | p)o | ds(12 | \ - d) | el(49 | ai) | em(l2 | ul) | er(ic | k0) | esl8 | ez([4 - 7]0 | os | wa | ze) | fetc | fly(\ - | _) | g1 u | g560 | gene | gf\ - 5 | g\ - mo | go(\.w | od) | gr(ad | un) | haie | hcit | hd\ - (m | p | t) | hei\ - | hi(pt | ta) | hp(i | ip) | hs\ - c | ht(c(\ - | |_ | a | g | p | s | t) | tp) | hu(aw | tc) | i\ - (20 | go | ma) | i230 | iac(| \ - | \ / ) | ibro | idea | ig01 | ikom | im1k | inno | ipaq | iris | ja(t | v)a | jbro | jemu | jigs | kddi | keji | kgt(| \ / ) | klon | kpt | kwc\ - | kyo(c | k) | le(no | xi) | lg(g | \ / (k | l | u) | 50 | 54 | \ - [a - w]) | libw | lynx | m1\ - w | m3ga | m50\/|ma(te | ui | xo) | mc(01 | 21 | ca) | m\ - cr | me(rc | ri) | mi(o8 | oa | ts) | mmef | mo(01 | 02 | bi | de | do | t(\ - | |o | v) | zz) | mt(50 | p1 | v) | mwbp | mywa | n10[0 - 2] | n20[2 - 3] | n30(0 | 2) | n50(0 | 2 | 5) | n7(0(0 | 1) | 10) | ne((c | m)\ - | on | tf | wf | wg | wt) | nok(6 | i) | nzph | o2im | op(ti | wv) | oran | owg1 | p800 | pan(a | d | t) | pdxg | pg(13 | \ - ([1 - 8] | c)) | phil | pire | pl(ay | uc) | pn\ - 2 | po(ck | rt | se) | prox | psio | pt\ - g | qa\ - a | qc(07 | 12 | 21 | 32 | 60 | \ - [2 - 7] | i\ - ) | qtek | r380 | r600 | raks | rim9 | ro(ve | zo) | s55\/|sa(ge | ma | mm | ms | ny | va) | sc(01 | h\ - | oo | p\ - ) | sdk\/|se(c(\ - | 0 | 1) | 47 | mc | nd | ri) | sgh\ - | shar | sie(\ - | m) | sk\ - 0 | sl(45 | id) | sm(al | ar | b3 | it | t5) | so(ft | ny) | sp(01 | h\ - | v\ - | v) | sy(01 | mb) | t2(18 | 50) | t6(00 | 10 | 18) | ta(gt | lk) | tcl\ - | tdg\ - | tel(i | m) | tim\ - | t\ - mo | to(pl | sh) | ts(70 | m\ - | m3 | m5) | tx\ - 9 | up(\.b | g1 | si) | utst | v400 | v750 | veri | vi(rg | te) | vk(40 | 5[0 - 3] | \ - v) | vm40 | voda | vulc | vx(52 | 53 | 60 | 61 | 70 | 80 | 81 | 83 | 85 | 98) | w3c(\ - | ) | webc | whit | wi(g | nc | nw) | wmlb | wonu | x700 | yas\ - | your | zeto | zte\ - / i.test(a.substr(0, 4))) check = true; })(navigator.userAgent || navigator.vendor || window.opera);
		//return check;
		if (typeof screen.orientation !== 'undefined')
			return false;
		return true;
	}
);


//https://stackoverflow.com/questions/17130395/real-mouse-position-in-canvas
EM_JS
(
	int, get_CanvasPosX, (),
	{
		//emscripten_border.scrollTop
		//var rect = canvas.getBoundingClientRect();
		//return rect.left;
		return 0;
	}
);

EM_JS
(
	int, get_CanvasPosY, (),
	{


		//var rect = canvas.getBoundingClientRect();
		//return rect.top;
		//https://stackoverflow.com/questions/11381673/detecting-a-mobile-browser
		if (IsNotPC())
		{
			var scrollPos = document.getElementsByTagName("html")[0].scrollTop;
			return scrollPos;
		}
		return 0;
	}
);


EM_JS
(
	int, get_canvas_width, (),
	{
			//if (window.matchMedia("(orientation: portrait)").matches) 
			//{
			//	// you're in PORTRAIT mode
			//}

			//if (window.matchMedia("(orientation: landscape)").matches) 
			//{
			//	// you're in LANDSCAPE mode
			//}
			//var l_iScreenWidth = screen.width>screen.height? screen.height: screen.width;
			//var l_iCanvansWidth = canvas.width > canvas.height ? canvas.height : canvas.width;
			//return Browser.isFullscreen? l_iScreenWidth : l_iCanvansWidth;
			//return Browser.isFullscreen ? screen.width : canvas.width;
			//return Browser.isFullscreen ? window.screen.width * window.devicePixelRatio : canvas.width;
			//return window.screen.width;
			//return canvas.height;
			//return Math.max(
			//	document.body.scrollWidth,
			//	document.documentElement.scrollWidth,
			//	document.body.offsetWidth,
			//	document.documentElement.offsetWidth,
			//	document.documentElement.clientWidth
			//);
			//return Math.max(
			//	document.body.scrollHeight,
			//	document.documentElement.scrollHeight,
			//	document.body.offsetHeight,
			//	document.documentElement.offsetHeight,
			//	document.documentElement.clientHeight
			//);
		return 720;
	}
);
EM_JS
(
	int, get_canvas_height, (),
	{
		//var l_iScreenHeight = screen.width > screen.height ? screen.width: screen.height;
		//var l_iCanvansHeight = canvas.width > canvas.height ? canvas.width: canvas.height;
		//return Browser.isFullscreen?l_iScreenHeight:l_iCanvansHeight;
		//return Browser.isFullscreen ? screen.height: canvas.height;
		//return Browser.isFullscreen ? window.screen.height * window.devicePixelRatio : canvas.height;
		//return window.screen.height;
		//return canvas.weight;
		//return Math.max(
		//	document.body.scrollHeight,
		//	document.documentElement.scrollHeight,
		//	document.body.offsetHeight,
		//	document.documentElement.offsetHeight,
		//	document.documentElement.clientHeight
		//);
			//return Math.max(
			//	document.body.scrollWidth,
			//	document.documentElement.scrollWidth,
			//	document.body.offsetWidth,
			//	document.documentElement.offsetWidth,
			//	document.documentElement.clientWidth
			//);
		return 1280;
	}
);
#endif

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
	Vector4 l_vViewPort = cGameApp::m_spOpenGLRender->m_vViewPortSize;
	glViewport((int)cGameApp::m_spOpenGLRender->m_vViewPortSize.x,(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.y,(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(),(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
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
	SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
}
//#include "../../Core/AVIPlayer/FFMpegPlayer.h"
cBluffingGirlApp*g_pBluffingGirlApp = 0;
cPlayerData*	cBluffingGirlApp::m_spPlayerData = 0;
POINT	g_RealMousePos = {0,0};
cUIImage*g_pCanvasImage = 0;
cStageClearFile*g_pStageClearFile = 0;
cFBFunction*g_pFBFunction = 0;
cMultiPlayerNetwork*		cBluffingGirlApp::m_spGameNetwork = 0;

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
	m_spGameNetwork = new cMultiPlayerNetwork();
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
	//SetAcceptRationWithGameresolution(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(),cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
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
#ifdef WASM
	auto l_iWidth = get_canvas_width();
	auto l_iHeight = get_canvas_height();
	cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution(l_iWidth, l_iHeight,
		(int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
#else
	//glViewport((int)cGameApp::m_spOpenGLRender->m_vViewPortSize.x,(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.y,(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(),(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
	glViewport((int)cGameApp::m_spOpenGLRender->m_vViewPortSize.x, (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.y, (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
	//glViewport(0,0,(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height());
	//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
#endif
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0.0,0.0,1 );
	glClearDepth(1.0f);	
	float	l_fVP[16];

	cGameApp::Render();
	glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y,l_fVP);
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
	m_sbDebugFunctionWorking = true;
	cGameApp::ShowInfo();
#ifdef DEBUG
	glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y,l_fVP);

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
	SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
	glFlush();
}

void	cBluffingGirlApp::MouseDown(int e_iPosX,int e_iPosY)
{
	g_RealMousePos.x = e_iPosX;
	g_RealMousePos.y = e_iPosY;
    cGameApp::MouseDown(e_iPosX,e_iPosY);
#ifdef WASM
	m_sMousePosition.y += get_CanvasPosY();
#endif
	if (m_pPhaseManager)
	{
		this->m_pPhaseManager->MouseDown(cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y);
	}
}

void	cBluffingGirlApp::MouseMove(int e_iPosX,int e_iPosY)
{
	g_RealMousePos.x = e_iPosX;
	g_RealMousePos.y = e_iPosY;
    cGameApp::MouseMove(e_iPosX,e_iPosY);
#ifdef WASM
	m_sMousePosition.y += get_CanvasPosY();
#endif
	if (m_pPhaseManager)
	{
		this->m_pPhaseManager->MouseMove(cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y);
	}
}

void	cBluffingGirlApp::MouseUp(int e_iPosX,int e_iPosY)
{
	g_RealMousePos.x = e_iPosX;
	g_RealMousePos.y = e_iPosY;
    cGameApp::MouseUp(e_iPosX,e_iPosY);
#ifdef WASM
	m_sMousePosition.y += get_CanvasPosY();
#endif
	if (m_pPhaseManager)
	{
		this->m_pPhaseManager->MouseUp(cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y);
	}
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