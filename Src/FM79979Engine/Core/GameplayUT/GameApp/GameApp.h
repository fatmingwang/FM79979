#pragma once

#if defined(ANDROID)
#include <jni.h>
#include <android/native_activity.h>
#include "../../Android/nv_native_app_glue.h"
#include "../../Android/nv_egl_util.h"
#include "../../Android/JNIUtil.h"
#endif
#include "../../Common/CommonApp.h"
#include "../OpenGL/OpenGLRender.h"
#include "../../Common/Template/TemplateClass.h"
#include "../GameUTDefine.h"
//for android
class	NvEGLUtil;
namespace	FATMING_CORE
{
	class cOpenGLRender;
	class	cGameApp:public cCommonApp
	{
		bool						m_bDoScreenShot;
	protected:
		virtual	void				Update(float e_fElpaseTime)override;
		virtual	void				Render();
	public:
		static double				m_dbGamePlayTime;
#if defined(ANDROID)
		cGameApp(ANativeActivity* e_pActivity, JNIEnv*e_pThreadEnv, jobject* e_pAppThreadThis, Vector2 e_vGameResolution, Vector2 e_vViewportSize, NvEGLUtil*e_pNvEGLUtil);
#elif defined(WIN32)
		cGameApp(HWND e_Hwnd, Vector2 e_vGameResolution = Vector2(1920.f, 1080.f), Vector2 e_vViewportSize = Vector2(1920.f, 1080.f));
#else
		cGameApp(Vector2 e_vGameResolution, Vector2 e_vViewportSize);
#endif
		virtual~cGameApp();
		virtual	void				Destroy();
		//initial shader and phase manager
		virtual	void				Init();
		//update and render
		virtual	void				Run();
		//basicly we don't use below function,instead call by TouchSignalProcess
		virtual	void				MouseDown(int e_iPosX, int e_iPosY);
		virtual	void				MouseMove(int e_iPosX, int e_iPosY);
		virtual	void				MouseUp(int e_iPosX, int e_iPosY);
		//e_iButtonIndex:0 for left,1 for right,2 for mid.
		virtual	void				MouseButtonClickEvent(bool e_bDown, int e_iButtonIndex);
		//it will automaticly to call mouse relateive function for their behavior
		//this one for single touch
		//while 2 touche point is clicked,keep doing 2 touch event but if first touch point is mouse up,keep doing mouse2 event until mouseup2 is called,
		//make m_b2Touched is set to false.
		virtual void				TouchSignalProcess(int e_iPosX, int e_iPosY, bool e_bTouch);
		//virtual void    TouchTwoProcess(int e_iPosX,int e_iPosY,int e_iPos2X,int e_iPos2Y,bool e_bTouch1,bool e_bTouch2);
		//virtual	void				KeyPress(char e_char);
		virtual	void				KeyDown(char e_char);
		virtual	void				KeyUp(char e_char);
		void						RenderPause();
		//static objects
#if defined(ANDROID)
		static  JNIEnv*				m_spThreadEnv;
		static  ANativeActivity*	m_spANativeActivity;
		static	jobject*			m_spAppThreadThis;
		static	NvEGLUtil*			m_pNvEGLUtil;
		static	sJNIUtilData*		m_spJNIUtilData;
#endif
		static	cOpenGLRender*										m_spOpenGLRender;
		static	bool												m_sbDoMultiTouch;
		static	bool												m_sbDoLockFPS;
		//
		static  class cAnimationParser*								m_spAnimationParser;
		static  class cSoundParser*									m_spSoundParser;
		static  class cImageParser*									m_spImageParser;
		static	class cPaticleManager*								m_spPaticleManager;
		static	class cBehaviorPaticleManager*						m_spBehaviorPaticleManager;
		static	class cGlyphFontRender*								m_spGlyphFontRender;
		static	cNamedTypedObjectVector<class cGlyphFontRender>*	m_spGlyphFontRenderVector;
		static	cNamedTypedObjectVector<class cExternalFunction>*	m_spExternalFunctionVector;
		static	cNamedTypedObjectVector<class cCurveManager>*		m_spPathFileList;
		static	cNamedTypedObjectVector<class c2DImageCollisionData>*m_sp2DImageCollisionDataVector;
		static	cNamedTypedObjectVector<NamedTypedObject>*			m_spColladaParserVector;
		static	struct sMultiTouchPoints*							m_spMultiTouchPoints;
		//betwen -1,1,device direction
		static  Vector3												m_svAccelerometer;
		//0-40(or bigger depend on device)????what is this?
		static  float												m_sfForce;
		static	bool												m_sbFullScreen;
		//will be effect by key + and -,so keep - and + reserved
		static	float												m_sfGameSpeedValue;
		//if true it will control by key + and -
		static	bool												m_sbSpeedControl;
		//=================================================
		static	void												CreateDefaultOpenGLRender();
		static	void												ResoluctionParse(const char*e_strFileName);
		static	void												ApplyViewPort();
		static	class cPuzzleImage*									GetPuzzleImage(const wchar_t*e_strName);
		static	class cBaseImage*									GetBaseImage(const wchar_t*e_strName);
		static	class cPuzzleImage*									GetPuzzleImageByFileName(const wchar_t*e_strFileName);
		static	class cPuzzleImage*									GetPuzzleImageByFileName(std::wstring e_strFileName);
		static	class cMPDIList*									GetMPDIList(const wchar_t*e_strName);
		static	class cMPDIList*									GetMPDIListByFileName(const wchar_t*e_strFileName, bool e_bShowErrorMsg = false);
		static	class cMPDIList*									GetMPDIList(const char*e_strFileName, const wchar_t*e_strName);
		static	class cMPDIList*									GetMPDIList(int e_iIndex);
		static	class cCurveManager*								GetCurveManagerByFileName(const wchar_t*e_strFileName);
		static	class cParticleEmitterGroup*						GetPRTG(const char*e_strFileName, const wchar_t*e_strName);
		static	class cParticleEmitterGroup*						GetPRTG(const wchar_t*e_strFileName, const wchar_t*e_strName);
		static	class cParticleEmitterGroup*						GetPRTG(const wchar_t*e_strName);
		static	class cPrtEmitter*									GetPrtEmitter(const char*e_strFileName, const wchar_t*e_strName);
		static	class cPrtEmitter*									GetPrtEmitter(const wchar_t*e_strFileName, const wchar_t*e_strName);
		static	class cPrtEmitter*									GetPrtEmitter(const wchar_t*e_strName);
		static	bool												SoundPlay(const wchar_t*e_strSoundName, bool e_bPlay);
		static	class cBasicSound*									GetSoundByFileName(const wchar_t*e_strSoundFileName);
		static	class cBasicSound*									GetSound(const wchar_t*e_strSoundName);
		static	void												RenderFont(long e_iPosX, long e_iPosY, const wchar_t*e_strInfo);
		static	void												RenderFont(int e_iPosX, int e_iPosY, const wchar_t*e_strInfo);
		static	void												RenderFont(int e_iPosX, int e_iPosY, std::wstring e_strInfo);
		static	void												RenderFont(float e_fPosX, float e_fPosY, const wchar_t*e_strInfo);
		static	void												RenderFont(float e_fPosX, float e_fPosY, std::wstring e_strInfo);
		static	void												RenderFont(Vector2 e_vPos, const wchar_t*e_pString);
		static	void												OutputDebugInfoString(const wchar_t*e_str, bool e_bWithNextLineSymbol = true, bool e_bWriteLog = false);
		static	void												OutputDebugInfoString(std::wstring e_str, bool e_bWithNextLineSymbol = true, bool e_bWriteLog = false);
		static	void												OutputDebugInfoString(std::string e_str, bool e_bWithNextLineSymbol = true, bool e_bWriteLog = false);
		static	void												OutputDebugInfoString(const char*e_str, bool e_bWithNextLineSymbol = true, bool e_bWriteLog = false);
		static	void												WriteLog(const wchar_t*e_strMessage);
		static	void												WriteLog(std::wstring e_strMessage);
		static	void												WriteLog(const char*e_strMessage);
		static	void												WriteLog(std::string e_strMessage);
		static	void												CallExternalFunction(std::wstring e_strName, void*e_pData);
		static	bool												AddExternalFunction(DoButtonGoal_Callback e_pCallBack, const wchar_t*e_strExternalFunctionName);
		static	cGlyphFontRender*									GetGlyphFontRenderByFileName(const wchar_t*e_strFileName);
		static	cGlyphFontRender*									GetGlyphFontRender(const wchar_t*e_strFileName);
		static	void												ShowInfo();
		//e_iKeyBoardType = 0 as default,e_iKeyBoardType = 1 as Numeric pad,
		//in android if e_iKeyBoardType is one u have to setup
		//FinishInput(String e_strValue);,Chancel();take a look at cpp
		//take a see at cpp
		static  void												OpenKeyBoard(int e_iKeyBoardType, const wchar_t*e_strTitle = L"GameApp", const wchar_t*e_strMessage = L"please input data");
		static	std::wstring										GetDIDOrInstallationRandomID();
	};
	//end namespace
	POINT					GetScreenResolution();
}

#ifdef WIN32
#include "Psapi.h"
void PrintMemoryInfo(const wchar_t*e_strName = L"Memory");
void ToggleFullScreen(bool e_bFullScreen, HWND e_hwnd);
#endif

//#define	WRITELOG
//
//#ifdef 	WRITELOG
//#define		WRITELOGFILE(p)	if(cGameApp::m_spLogFile)cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(p);
//#else	
//#define		WRITELOGFILE(p) ((void)0)
//#endif


//lazy code
//#include "stdafx.h"
//#include "MusicGameApp.h"
//#include "PerformMusicPhase.h"
//
//cMusicGameApp::
//#if defined(ANDROID)
//cMusicGameApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil)
//:cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize,e_pNvEGLUtil)
//#elif defined(WIN32)
//cMusicGameApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
//#else
//cMusicGameApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_vGameResolution,e_vViewportSize)
//#endif
//{
//	SetAcceptRationWithGameresolution((int)e_vViewportSize.x,(int)e_vViewportSize.y,(int)e_vGameResolution.x,(int)e_vGameResolution.y);
//	m_pPhaseManager = nullptr;
//}
//
//cMusicGameApp::~cMusicGameApp()
//{
//	SAFE_DELETE(m_pPhaseManager);
//}
//
//void	cMusicGameApp::Init()
//{
//	cGameApp::Init();
//	m_pPhaseManager = new cPhaseManager();
//	if( m_pPhaseManager )
//	{
//	
//	}
//}
//
//
//void	cMusicGameApp::Update(float e_fElpaseTime)
//{
//	cGameApp::Update(e_fElpaseTime);
//	if( m_pPhaseManager )
//	{
//		m_pPhaseManager->Update(e_fElpaseTime);
//	}
//}
//
//void	cMusicGameApp::Render()
//{
//	cGameApp::Render();
//	if( m_pPhaseManager )
//	{
//		m_pPhaseManager->Render();
//	}
//}
//
//
//void	cMusicGameApp::MouseDown(int e_iPosX,int e_iPosY)
//{
//	cGameApp::MouseDown(e_iPosX,e_iPosY);
//	if( m_pPhaseManager )
//	{
//		m_pPhaseManager->MouseDown(e_iPosX,e_iPosY);
//	}
//}
//
//void	cMusicGameApp::MouseMove(int e_iPosX,int e_iPosY)
//{
//	cGameApp::MouseMove(e_iPosX,e_iPosY);
//	if( m_pPhaseManager )
//	{
//		m_pPhaseManager->MouseMove(e_iPosX,e_iPosY);
//	}
//}
//
//void	cMusicGameApp::MouseUp(int e_iPosX,int e_iPosY)
//{
//	cGameApp::MouseUp(e_iPosX,e_iPosY);
//	if( m_pPhaseManager )
//	{
//		m_pPhaseManager->MouseUp(e_iPosX,e_iPosY);
//	}
//}
//void	cMusicGameApp::KeyPress(char e_char)
//{
//	cGameApp::KeyPress(e_char);
//	if( m_pPhaseManager )
//	{
//		m_pPhaseManager->KeyPress(e_char);
//	}
//}
//
//void	cMusicGameApp::KeyDown(char e_char)
//{
//	cGameApp::KeyDown(e_char);
//	if( m_pPhaseManager )
//	{
//		m_pPhaseManager->KeyDown(e_char);
//	}
//}
//
//void	cMusicGameApp::KeyUp(char e_char)
//{
//	cGameApp::KeyUp(e_char);
//	if( m_pPhaseManager )
//	{
//		m_pPhaseManager->KeyUp(e_char);
//	}
//}