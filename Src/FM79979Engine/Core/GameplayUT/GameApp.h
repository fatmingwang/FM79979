#ifndef _GAME_APP_H_
#define _GAME_APP_H_

#if defined(ANDROID)
#include <jni.h>
#include <android/native_activity.h>
#include "../Android/nv_native_app_glue.h"
#include "../Android/nv_egl_util.h"
#endif

//#include "PhaseControl.h"
#include "MultiTouchPoints.h"
#include "GameUTDefine.h"
#include "../GLSL/Shader.h"
#include "../AllImageInclude.h"
#include "../AllMathInclude.h"



class	cFUThreadPool;
//for android
class	NvEGLUtil;
namespace	FATMING_CORE
{
	class	cExternalFunction;
	class	cSoundParser;
	class	cPaticleManager;
	class	cBehaviorPaticleManager;
	class	cParticleEmitterGroup;
	class	cPrtEmitter;
	class	cClickMouseBehavior;
	class	cBinaryFile;
	class	cBasicSound;
	class	cNumeralImage;
	class	c2DImageCollisionData;
	struct	sJNIUtilData;
	struct	sMultiTouchPoints;
	//in windows make sure touch points convert as well!.
	//view port x and y might swap if direction is lanscape.
	POINT	ConvertCoordinate(int e_iPosX,int e_iPosY,POINT e_ViewPort);


	class	cGameApp
	{
	protected:
		virtual	void				Update(float e_fElpaseTime);
		virtual	void				Render();
	public:
		double						m_dbGamePlayTime;
#if defined(ANDROID)
		cGameApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject* e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil);
#elif defined(WIN32)
		cGameApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920.f,1080.f),Vector2 e_vViewportSize = Vector2(1920.f,1080.f));
		virtual	void			OpenglInit(HWND e_Hwnd);
#else
		cGameApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize );
#endif
		virtual~cGameApp();
		virtual	void				Destroy();
		//initial shader and phase manager
		virtual	void				Init();
		//update and render
		virtual	void				Run();
		//basicly we don't use below function,instead call by TouchSignalProcess
		virtual	void				MouseDown(int e_iPosX,int e_iPosY);
		virtual	void				MouseMove(int e_iPosX,int e_iPosY);
		virtual	void				MouseUp(int e_iPosX,int e_iPosY);
		//e_iButtonIndex:0 for left,1 for right,2 for mid.
		virtual	void				MouseButtonClickEvent(bool e_bDown,int e_iButtonIndex);
		//it will automaticly to call mouse relateive function for their behavior
		//this one for single touch
		//while 2 touche point is clicked,keep doing 2 touch event but if first touch point is mouse up,keep doing mouse2 event until mouseup2 is called,
		//make m_b2Touched is set to false.
		virtual void				TouchSignalProcess(int e_iPosX,int e_iPosY,bool e_bTouch);
		//virtual void    TouchTwoProcess(int e_iPosX,int e_iPosY,int e_iPos2X,int e_iPos2Y,bool e_bTouch1,bool e_bTouch2);
		virtual	void				KeyPress(char e_char);
		virtual	void				KeyDown(char e_char);
		virtual	void				KeyUp(char e_char);
		void						SetAcceptRation(int e_iWidthRation = 9,int e_iHeightRation = 16,int e_iGap = 80);
		void						RenderPause();
		//static objects
#if defined(ANDROID)
		static	std::string*		m_spAPKPath;
		static	std::string*		m_spInternalDirectory;//system
		static	std::string*		m_spExternalDirectory;//internal sd
		static	std::string*		m_spExternalSDDirectory;//external sd
		static  JNIEnv*				m_spThreadEnv;
		static  ANativeActivity*	m_spANativeActivity;
		static	jobject*			m_spAppThreadThis;
		static	NvEGLUtil*			m_pNvEGLUtil;
		static	sJNIUtilData*		m_spJNIUtilData;
#elif defined(WIN32)
		static	HDC													m_sHdc;
		static	HANDLE												m_sHandle;
		static	HGLRC												m_sHGLRC;
#endif
		static	UT::sTimeAndFPS										m_sTimeAndFPS;
		static	Vector4												m_svViewPortSize;
		//while game resolution is change we want to re scale all data check this one
		static	Vector2												m_svGameScale;
		//viewport resolution or game resolution,both them could be same,but depend on the game type.
		static	Vector2												m_svGameResolution;
		static  cAnimationParser*									m_spAnimationParser;
		static  cSoundParser*										m_spSoundParser;
		static  cImageParser*										m_spImageParser;
		static	cPaticleManager*									m_spPaticleManager;
		static	cBehaviorPaticleManager*							m_spBehaviorPaticleManager;
		static	cGlyphFontRender*									m_spGlyphFontRender;
		static	cNamedTypedObjectVector<cGlyphFontRender>*			m_spGlyphFontRenderVector;
		static	cNamedTypedObjectVector<cExternalFunction>*			m_spExternalFunctionVector;
		static	sMultiTouchPoints*									m_spMultiTouchPoints;
		static	bool												m_sbDoMultiTouch;
		static	cNamedTypedObjectVector<cCurveManager>*				m_spPathFileList;
		static	cNamedTypedObjectVector<c2DImageCollisionData>*		m_sp2DImageCollisionDataVector;
		static	cNamedTypedObjectVector<NamedTypedObject>*			m_spColladaParserVector;
		static	bool    											m_bMouseHover;
		//betwen -1,1,device direction
		static  Vector3												m_svAccelerometer;
		//0-40(or bigger depend on device)
		static  float												m_sfForce;
		static  POINT												m_sMousePosition;
		static  POINT												m_sScreenMousePosition;
		static	bool												m_sbMouseClickStatus[3];//0 left,1 right,2 mid button
		static	short												m_sMouseWhellDelta;
		static	bool												m_sbFullScreen;
		static	bool												m_sbTouched;//if true mouse down
		static	bool												m_sbGamePause;
		static	eDeviceDirection									m_seDeviceDirection;
		static	cFUThreadPool*										m_spThreadPool;
		//if windows messagebox will pop up or just a outputdebugstring
		static	bool												m_sbShowErrorMsgBox;
		static	std::wstring*										m_spstrErrorMsgString;
		//will be effect by key + and -,so keep - and + reserved
		static	float												m_sfDebugValue;
		//this one could be write into file,so we could check user version
		static  float												m_sfVersion;
		//if true game exit
		static	bool												m_sbLeave;
		//if true it will control by key + and -
		static	bool												m_sbSpeedControl;
		static	bool												m_sbDebugFunctionWorking;
		//all key data is pressed or not
		static	bool												m_sucKeyData[MAX_PATH];
		//for io
		static	bool												m_sbDeviceExist;
		//
		static	bool												m_sbEnableMouseSingnal;
		static	bool												m_sbEnableKeyboardSingnal;
		static	bool												m_sbAllowParseBinaryFile;
		static	cBinaryFile*										m_spLogFile;
		static	std::string*										m_psstrGameAppName;

		//mouse
		//static cClickMouseBehavior*		m_spClickMouseBehavior;

		//=================================================
		static	void					ResoluctionParse(char*e_strFileName);

		static	Vector2					GetViewPortAndGameResolutionScale();
		static	void					ApplyViewPort();

		static	cPuzzleImage*			GetPuzzleImage(const wchar_t*e_strName);
		static	cBaseImage*				GetBaseImage(const wchar_t*e_strName);
		static	cPuzzleImage*			GetPuzzleImageByFileName(const wchar_t*e_strFileName);
		static	cPuzzleImage*			GetPuzzleImageByFileName(std::wstring e_strFileName);

		static	cNumeralImage*			GetNumeralImageByXmlElement(TiXmlElement*e_pTiXmlElement);

		static	cMPDIList*				GetMPDIList(const wchar_t*e_strName);
		static	cMPDIList*				GetMPDIListByFileName(const wchar_t*e_strFileName,bool e_bShowErrorMsg = false);
		static	cMPDIList*				GetMPDIList(const char*e_strFileName,const wchar_t*e_strName);
		static	cMPDIList*				GetMPDIList(int e_iIndex);
		static	cCurveManager*			GetCurveManagerByFileName(const wchar_t*e_strFileName);

		static	cParticleEmitterGroup*	GetPRTG(const char*e_strFileName,const wchar_t*e_strName);
		static	cParticleEmitterGroup*	GetPRTG(const wchar_t*e_strFileName,const wchar_t*e_strName);
		static	cParticleEmitterGroup*	GetPRTG(const wchar_t*e_strName);
		static	cPrtEmitter*			GetPrtEmitter(const char*e_strFileName,const wchar_t*e_strName);
		static	cPrtEmitter*			GetPrtEmitter(const wchar_t*e_strFileName,const wchar_t*e_strName);
		static	cPrtEmitter*			GetPrtEmitter(const wchar_t*e_strName);
		static	bool					SoundPlay(const wchar_t*e_strSoundName,bool e_bPlay);
		static	cBasicSound*			GetSoundByFileName(const wchar_t*e_strSoundFileName);
		static	cBasicSound*			GetSound(const wchar_t*e_strSoundName);
		static	void					RenderFont(int e_iPosX,int e_iPosY,const wchar_t*e_strInfo);
		static	void					RenderFont(int e_iPosX,int e_iPosY,std::wstring e_strInfo);
		static	void					RenderFont(float e_fPosX,float e_fPosY,const wchar_t*e_strInfo);
		static	void					RenderFont(float e_fPosX,float e_fPosY,std::wstring e_strInfo);
		static	void					OutputDebugInfoString(const wchar_t*e_str);
		static	void					OutputDebugInfoString(std::wstring e_str);
		static	void					OutputDebugInfoString(std::string e_str);
		static	void					OutputDebugInfoString(const char*e_str);
		static	void					WriteLog(wchar_t*e_strMessage);
		static	void					WriteLog(std::wstring e_strMessage);
		static	void					WriteLog(char*e_strMessage);
		static	void					WriteLog(std::string e_strMessage);
		static	void					CallExternalFunction(std::wstring e_strName,void*e_pData);
		static	bool					AddExternalFunction(DoButtonGoal_Callback e_pCallBack,const wchar_t*e_strExternalFunctionName);
		static	cGlyphFontRender*		GetGlyphFontRenderByFileName(const wchar_t*e_strFileName);
		static	cGlyphFontRender*		GetGlyphFontRender(const wchar_t*e_strFileName);
		static	void					ShowInfo();
		//e_iKeyBoardType = 0 as default,e_iKeyBoardType = 1 as Numeric pad,
		//in android if e_iKeyBoardType is one u have to setup
		//FinishInput(String e_strValue);,Chancel();take a look at cpp
		static  void					OpenKeyBoard(int e_iKeyBoardType,const wchar_t*e_strTitle=L"GameApp",const wchar_t*e_strMessage=L"please input data");
		static	std::wstring			GetDIDOrInstallationRandomID();
	};
	//end namespace
}

#ifdef WIN32
#include "Psapi.h"
void PrintMemoryInfo( wchar_t*e_strName = L"Memory" );
#endif

//#define	WRITELOG
//
//#ifdef 	WRITELOG
//#define		WRITELOGFILE(p)	if(cGameApp::m_spLogFile)cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(p);
//#else	
//#define		WRITELOGFILE(p) ((void)0)
//#endif

#endif