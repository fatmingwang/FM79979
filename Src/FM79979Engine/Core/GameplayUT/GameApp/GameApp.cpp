#include "GameApp.h"
#include "../../Common/Log/FMLog.h"
#include "../MultiTouch/MultiTouchPoints.h"
#include "../Render/Parser/AnimationParser.h"
#include "../Render/CommonRender/SimplePrimitive.h"
#include "../Render/UI/ImageButton.h"
#include "../Physic/2DImageCollisionData.h"
#ifdef WIN32
#include <direct.h>
#include "../OpenGL/WindowsOpenGL.h"
#endif
#include "../../Sound/SoundManager.h"
#include "../Render/Particle/AllParticleInclude.h"
#include "../Render/Texture/TextureManager.h"
#include "locale.h"
using namespace UT;
#if defined(ANDROID)//openAL,android.c
#include "../../Android/nv_egl_util.h"
#include "../../Android/JNIUtil.h"
#include "../../Android/nv_file.h"
#endif
#ifdef WASM
#include "locale.h"
#endif
namespace	FATMING_CORE
{
	extern void	DumpGraphicsInfo();

#if defined(ANDROID)
	JNIEnv*												cGameApp::m_spThreadEnv = 0;
	ANativeActivity*									cGameApp::m_spANativeActivity = 0;
	jobject*											cGameApp::m_spAppThreadThis = 0;
	NvEGLUtil*											cGameApp::m_pNvEGLUtil = 0;
	sJNIUtilData*										cGameApp::m_spJNIUtilData = 0;
#endif
	cOpenGLRender*										cGameApp::m_spOpenGLRender = nullptr;
	double												cGameApp::m_dbGamePlayTime = 0;
	float												cGameApp::m_sfGameSpeedValue = 1.f;
	bool												cGameApp::m_sbSpeedControl = false;
	bool												cGameApp::m_sbFullScreen = false;
	//
	Vector3												cGameApp::m_svAccelerometer = Vector3::Zero;
	float												cGameApp::m_sfForce = 0.f;
	//
	sMultiTouchPoints*									cGameApp::m_spMultiTouchPoints = nullptr;
	bool												cGameApp::m_sbDoMultiTouch = false;
	bool												cGameApp::m_sbDoLockFPS = false;
	//
	cPaticleManager*									cGameApp::m_spPaticleManager = nullptr;
	cBehaviorPaticleManager*							cGameApp::m_spBehaviorPaticleManager = nullptr;
	cGlyphFontRender*									cGameApp::m_spGlyphFontRender = nullptr;
	cNamedTypedObjectVector<cGlyphFontRender>*			cGameApp::m_spGlyphFontRenderVector = nullptr;
	cNamedTypedObjectVector<cExternalFunction>*			cGameApp::m_spExternalFunctionVector = nullptr;
	cNamedTypedObjectVector<c2DImageCollisionData>*		cGameApp::m_sp2DImageCollisionDataVector = nullptr;
	cNamedTypedObjectVector<NamedTypedObject>*			cGameApp::m_spColladaParserVector = nullptr;
	cAnimationParser*									cGameApp::m_spAnimationParser = nullptr;
	cImageParser*										cGameApp::m_spImageParser = nullptr;
	cSoundParser*										cGameApp::m_spSoundParser = nullptr;
	cNamedTypedObjectVector<cCurveManager>*				cGameApp::m_spPathFileList = nullptr;

#if defined(ANDROID)
	extern sJNIUtilData*g_pMainThreadJNIUtilData;
	extern bool		GetExternalSDPath(std::string*e_pTarget);
	extern void	SetupAPKFilePath(ANativeActivity* e_pActivity, JNIEnv*e_pThreadEnv);
	cGameApp::cGameApp(ANativeActivity* e_pActivity, JNIEnv*e_pThreadEnv, jobject* e_pAppThreadThis, Vector2 e_vGameResolution, Vector2 e_vViewportSize, NvEGLUtil*e_pNvEGLUtil)
#elif defined( WIN32)
	cGameApp::cGameApp(HWND e_Hwnd, Vector2 e_vGameResolution, Vector2 e_vViewportSize)
#else
	cGameApp::cGameApp(Vector2 e_vGameResolution, Vector2 e_vViewportSize)
#endif
	{
		if (!cGameApp::m_psstrGameAppName)
		{
			cGameApp::m_psstrGameAppName = new std::string;
			*cGameApp::m_psstrGameAppName = "GameApp";
		}
#ifdef ANDROID
		m_spThreadEnv = e_pThreadEnv;
		m_spANativeActivity = e_pActivity;
		m_spAppThreadThis = e_pAppThreadThis;
		m_pNvEGLUtil = e_pNvEGLUtil;
		m_spJNIUtilData = new sJNIUtilData(m_spThreadEnv);
		m_spJNIUtilData->Init();
		g_pMainThreadJNIUtilData = m_spJNIUtilData;
		SetupAPKFilePath(e_pActivity, e_pThreadEnv);
#endif
		if (this->m_sbDebugFunctionWorking)
		{
			FMLog::Init();
		}
#ifdef WIN32
		DWORD  nBufferLength = 0;
		wchar_t l_strDirectory[MAX_PATH];
		//GetCurrentDirectory(nBufferLength, l_strDirectory);
		GetModuleFileName(NULL, l_strDirectory, MAX_PATH);
		std::wstring l_strDirectpryInfo = L"Working Directory:";
		l_strDirectpryInfo += l_strDirectory;
		FMLog::LogWithFlag(l_strDirectpryInfo.c_str(), CORE_LOG_FLAG);
#endif
		m_bDoScreenShot = false;
		m_sbMouseClickStatus[0] = m_sbMouseClickStatus[1] = m_sbMouseClickStatus[2] = false;
		cGameApp::m_sbGamePause = false;
		OutputDebugInfoString(L"GameApp Start");
		m_dbGamePlayTime = 0;
		SystemErrorCheck();
		if (!m_spOpenGLRender)
			m_spOpenGLRender = new cOpenGLRender(e_vGameResolution, e_vViewportSize);
		else
		{
			m_spOpenGLRender->m_vGameResolution;
			m_spOpenGLRender->m_vViewPortSize.x = 0;
			m_spOpenGLRender->m_vViewPortSize.y = 0;
			m_spOpenGLRender->m_vViewPortSize.z = e_vViewportSize.x;
			m_spOpenGLRender->m_vViewPortSize.w = e_vViewportSize.y;
			m_spOpenGLRender->m_vDeviceViewPortSize = m_spOpenGLRender->m_vViewPortSize;
		}
#if defined(WIN32) && !defined(UWP)
		m_spOpenGLRender->Init(e_Hwnd,true);
		PrintMemoryInfo();
#else
		m_spOpenGLRender->Init();
#endif
		memset(m_sucKeyData, 0, sizeof(bool)*MAX_PATH);
		m_bMouseHover = false;
		m_spMultiTouchPoints = new sMultiTouchPoints();
		if (!m_spAnimationParser)
		{
			//in android some how it will close force sunndenly by I donno who.
			//m_spThreadPool = new cFUThreadPool;
			m_spAnimationParser = new cAnimationParser();
			m_spImageParser = m_spAnimationParser->GetAllBaseImageList();
			m_spSoundParser = new cSoundParser();
			m_spBehaviorPaticleManager = new cBehaviorPaticleManager();
			m_spPaticleManager = m_spBehaviorPaticleManager->m_pPaticleManager;
			m_spPathFileList = new cNamedTypedObjectVector<cCurveManager>;
			m_spGlyphFontRenderVector = new cNamedTypedObjectVector<cGlyphFontRender>();
			m_spExternalFunctionVector = new cNamedTypedObjectVector<cExternalFunction>();
			m_sp2DImageCollisionDataVector = new cNamedTypedObjectVector<c2DImageCollisionData>;
			//please new in ur app
			//cNamedTypedObjectVector<cColladaParser>*l_pColladaParserVector = new cNamedTypedObjectVector<cColladaParser>();
			//m_spColladaParserVector = (cNamedTypedObjectVector<NamedTypedObject>*)l_pColladaParserVector;
		}
#if defined WIN32 || defined LINUX || defined WASM
		char *locale = setlocale(LC_ALL, "");
#endif
		//std::wstring l_str111 = GetDIDOrInstallationRandomID();
		//std::string l_str222 = UT::WcharToChar(l_str111);
		//const char* l_str333 = l_str222.c_str();
		//SystemErrorCheck();
#ifdef WASM
		cGameApp::Init();
#endif
	}

	cGameApp::~cGameApp()
	{
		SystemErrorCheck();
		Destroy();
#ifdef WIN32
		//PrintMemoryInfo();
#elif defined(ANDROID)
		SAFE_DELETE(cGameApp::m_spAPKPath);
		SAFE_DELETE(m_spExternalDirectory);
		SAFE_DELETE(m_spExternalSDDirectory);
		SAFE_DELETE(m_spInternalDirectory);
		SAFE_DELETE(m_spJNIUtilData);
#endif
		SystemErrorCheck();
		//SAFE_DELETE(m_spThreadPool);
		SAFE_DELETE(m_spOpenGLRender);
		SAFE_DELETE(cGameApp::m_psstrGameAppName);
		NamedTypedObject::DumpUnReleaseInfo();
		FMLog::Destroy();
	}

	void	cGameApp::Init()
	{
		if (m_spGlyphFontRender)
			return;
#ifdef DEBUG
		DumpGraphicsInfo();
#endif
		m_sbSpeedControl = this->m_sbDebugFunctionWorking;
		m_sbLeave = false;
		SystemErrorCheck();
		//This hint can improve the speed of texturing when perspective- correct texture coordinate interpolation isn't needed, such as when using a glOrtho() projection.
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
		m_sTimeAndFPS.Update();
		FMLog::LogWithFlag("parse font data", CORE_LOG_FLAG, true);
		if (!m_spGlyphFontRender)
		{
#ifdef WASM
			m_spGlyphFontRender = new cGlyphFontRender("assets/Font", 3000);
#else
			m_spGlyphFontRender = new cGlyphFontRender("Font", 3000);
#endif
			FMLog::LogWithFlag("check font image", CORE_LOG_FLAG, true);
			if (!m_spGlyphFontRender->GetFontImage())
			{
				SAFE_DELETE(m_spGlyphFontRender);
			}
			else
			{
				m_spGlyphFontRenderVector->AddObject(m_spGlyphFontRender);
			}
		}
		if (!m_spGlyphFontRender)
			FMLog::LogWithFlag("font create failed", CORE_LOG_FLAG, true);
		else
			FMLog::LogWithFlag("font create ok", CORE_LOG_FLAG, true);
		m_sTimeAndFPS.Update();
		srand(m_sTimeAndFPS.uiCurrentTime);
		SystemErrorCheck();
	}

	void	cGameApp::Destroy()
	{
		FMLog::WriteLog("Destroy start");
		SystemErrorCheck();
		SAFE_DELETE(m_sp2DImageCollisionDataVector);
		SAFE_DELETE(m_spColladaParserVector);
		SAFE_DELETE(m_spMultiTouchPoints);
		SAFE_DELETE(m_spSoundParser);
		//must delete before m_spAnimationParser,because m_spBehaviorPaticleManager has a ImageParser
		SAFE_DELETE(m_spBehaviorPaticleManager);
		SAFE_DELETE(m_spAnimationParser);
		SAFE_DELETE(m_spPathFileList);
		SAFE_DELETE(m_spGlyphFontRenderVector);
		m_spGlyphFontRender = 0;
		SAFE_DELETE(m_spExternalFunctionVector);
		SystemErrorCheck();
		FMLog::WriteLog("all Destroy done");
	}

	void	cGameApp::Update(float e_fElpaseTime)
	{
		cCommonApp::Update(e_fElpaseTime);
		if (this->m_spOpenGLRender)
			m_spOpenGLRender->Update(e_fElpaseTime);
		SystemErrorCheck();
	}

	void	cGameApp::Render()
	{
		if (this->m_spOpenGLRender)
			m_spOpenGLRender->Render();
	}

	void	cGameApp::Run()
	{
		cTexture::m_suiLastUsingImageIndex = -1;
		m_sTimeAndFPS.Update();
		float	l_fElpaseTime = m_sTimeAndFPS.fElpaseTime;
		m_dbGamePlayTime += l_fElpaseTime;
		if (cGameApp::m_sbDoLockFPS)
			l_fElpaseTime = 0.016f;
		if (m_sbGamePause)
			l_fElpaseTime = 0.f;
		Update(m_sbSpeedControl ? l_fElpaseTime*this->m_sfGameSpeedValue : l_fElpaseTime);
		Render();
		if (m_bDoScreenShot)
		{
			m_bDoScreenShot = false;
			std::wstring l_strFileName;
			if (m_psstrGameAppName)
			{
				l_strFileName += ValueToStringW(this->m_psstrGameAppName->c_str());
				l_strFileName += L"_";
			}
			l_strFileName += L"ScreenShot_";
			l_strFileName += GetSystemTimeForFile(false);
			l_strFileName += L".jpeg";
			SaveCurrentBufferToImage(ValueToString(l_strFileName).c_str(), (int)this->m_spOpenGLRender->m_vDeviceViewPortSize.Width(), (int)this->m_spOpenGLRender->m_vDeviceViewPortSize.Height());
		}
		cGameApp::m_sMouseWhellDelta = 0;
	}

	void	cGameApp::MouseDown(int e_iPosX, int e_iPosY)
	{
		m_sScreenMousePosition.x = e_iPosX;
		m_sScreenMousePosition.y = e_iPosY;
		//if( !m_sbEnableMouseSingnal )
		//	return;
		POINT	l_Viewport = { (int)m_spOpenGLRender->m_vViewPortSize.Width(),(int)m_spOpenGLRender->m_vViewPortSize.Height() };
		m_sMousePosition = this->m_spOpenGLRender->ConvertCoordinate(e_iPosX, e_iPosY, l_Viewport);
		m_sbTouched = true;
	}

	void	cGameApp::MouseMove(int e_iPosX, int e_iPosY)
	{
		m_sScreenMousePosition.x = e_iPosX;
		m_sScreenMousePosition.y = e_iPosY;
		//if( !m_sbEnableMouseSingnal )
		//	return;
		POINT	l_Viewport = { (int)m_spOpenGLRender->m_vViewPortSize.Width(),(int)m_spOpenGLRender->m_vViewPortSize.Height() };
		m_sMousePosition = this->m_spOpenGLRender->ConvertCoordinate(e_iPosX, e_iPosY, l_Viewport);
	}

	void	cGameApp::MouseUp(int e_iPosX, int e_iPosY)
	{
		m_sScreenMousePosition.x = e_iPosX;
		m_sScreenMousePosition.y = e_iPosY;
		//if( !m_sbEnableMouseSingnal )
		//	return;
		POINT	l_Viewport = { (int)m_spOpenGLRender->m_vViewPortSize.Width(),(int)m_spOpenGLRender->m_vViewPortSize.Height() };
		m_sMousePosition = this->m_spOpenGLRender->ConvertCoordinate(e_iPosX, e_iPosY, l_Viewport);
		m_sbTouched = false;
	}

	void	cGameApp::MouseButtonClickEvent(bool e_bDown, int e_iButtonIndex)
	{
		assert(e_iButtonIndex < 3 && "only 012");
		m_sbMouseClickStatus[e_iButtonIndex] = e_bDown;
	}

	void    cGameApp::TouchSignalProcess(int e_iPosX, int e_iPosY, bool e_bTouch)
	{
		//if( !m_sbEnableMouseSingnal )
		//	return;
		if (e_bTouch)
		{
			if (m_bMouseHover)
				MouseMove(e_iPosX, e_iPosY);
			else
				MouseDown(e_iPosX, e_iPosY);
			m_bMouseHover = true;
			return;
		}
		else
		{
			POINT	l_Viewport = { (int)m_spOpenGLRender->m_vViewPortSize.Width(),(int)m_spOpenGLRender->m_vViewPortSize.Height() };
			m_sMousePosition = this->m_spOpenGLRender->ConvertCoordinate(e_iPosX, e_iPosY, l_Viewport);
		}
		if (m_bMouseHover)
			MouseUp(e_iPosX, e_iPosY);
		m_bMouseHover = false;
	}

	void	cGameApp::KeyDown(char e_char)
	{
		if (!m_sbEnableKeyboardSingnal)
			return;
		m_sucKeyData[(unsigned char)e_char] = true;
	}

	//void	cGameApp::KeyPress(char e_char)
	//{
	//	if (!m_sbEnableKeyboardSingnal)
	//		return;
	//	//m_sucKeyData[(unsigned char)e_char] = true;
	//}

	void	cGameApp::KeyUp(char e_char)
	{
		if (!m_sbEnableKeyboardSingnal)
			return;
		//alter
		if (m_sucKeyData[17])
		{
			if (m_sbDebugFunctionWorking)
			{
				switch (e_char)
				{
#if defined(WIN32)&& !defined(UWP)
				case 13:
				{
					cGameApp::m_sbFullScreen = !cGameApp::m_sbFullScreen;
					ToggleFullScreen(cGameApp::m_sbFullScreen, (HWND)m_spOpenGLRender->m_Handle);
				}
					break;
#endif
				case 'L'://enter
					this->m_sbDoLockFPS = !this->m_sbDoLockFPS;
					break;
				case 'R':
					m_sfGameSpeedValue = 1.f;
					break;
				case 'P':
					m_sbGamePause = !m_sbGamePause;
					break;
				case 38://up
					this->m_spOpenGLRender->m_eDeviceDirection = eDD_PORTRAIT;
					break;
				case 37://left
					this->m_spOpenGLRender->m_eDeviceDirection = eDD_LANDSCAPE_LEFT;
					break;
				case 39://right
					this->m_spOpenGLRender->m_eDeviceDirection = eDD_LANDSCAPE_RIGHT;
					break;
				case 40://down
					this->m_spOpenGLRender->m_eDeviceDirection = eDD_UPSIDE_DOWN;
					break;
				case 44://PrtScr/SysRq
					m_bDoScreenShot = true;
					break;
				case 107://107'+'
					m_sfGameSpeedValue *= 2.f;
					if (m_sfGameSpeedValue > 1073741824)
						m_sfGameSpeedValue = 1073741824;
					break;
				case 109://109'-'
					m_sfGameSpeedValue /= 2.f;
					if (m_sfGameSpeedValue <= 0.00000000001f)
						m_sfGameSpeedValue = 0.00000000001f;
					break;
				default:
					break;
				}
			}
			if (e_char == 'D')
				this->m_sbDebugFunctionWorking = !this->m_sbDebugFunctionWorking;
		}
		m_sucKeyData[(unsigned char)e_char] = false;
	}

	void	cGameApp::RenderPause()
	{
		if (cGameApp::m_sbGamePause)
		{
			RenderFilledRectangle(Vector2(0, 0), cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y, Vector4(0, 0, 0, 0.7f), 0);
			wchar_t	l_strDescription[] = { 36938,25138,26283,20572,20013,32,32,20877,25353,19968,27425,36864,20986,0 };
			m_spGlyphFontRender->SetScale(1.5f);
			cGameApp::RenderFont(cGameApp::m_spOpenGLRender->m_vGameResolution.x / 2 - 200, cGameApp::m_spOpenGLRender->m_vGameResolution.y / 2, l_strDescription);
			m_spGlyphFontRender->SetScale(1.3f);
			cGameApp::RenderFont(cGameApp::m_spOpenGLRender->m_vGameResolution.x / 2 - 300, cGameApp::m_spOpenGLRender->m_vGameResolution.y / 2 + 30, L"Pause! press back again to exit");
			m_spGlyphFontRender->SetScale(1.f);
		}
	}

	void	cGameApp::RenderFont(float e_fPosX, float e_fPosY, const wchar_t*e_strInfo)
	{
		RenderFont((int)e_fPosX, (int)e_fPosY, e_strInfo);
	}


	void	cGameApp::RenderFont(long e_iPosX, long e_iPosY, const wchar_t*e_strInfo)
	{
		if (m_spGlyphFontRender)
		{
			m_spGlyphFontRender->RenderFont((int)e_iPosX, (int)e_iPosY, e_strInfo);
		}
	}

	void	cGameApp::RenderFont(int e_iPosX, int e_iPosY, const wchar_t*e_strInfo)
	{
		if (m_spGlyphFontRender)
		{
			m_spGlyphFontRender->RenderFont(e_iPosX, e_iPosY, e_strInfo);
		}
	}

	void	cGameApp::RenderFont(int e_iPosX, int e_iPosY, std::wstring e_strInfo)
	{
		RenderFont(e_iPosX, e_iPosY, e_strInfo.c_str());
	}

	void	cGameApp::RenderFont(Vector2 e_vPos, const wchar_t*e_pString)
	{
		RenderFont(e_vPos.x, e_vPos.y, e_pString);
	}

	void	cGameApp::RenderFont(float e_fPosX, float e_fPosY, std::wstring e_strInfo)
	{
		RenderFont((int)e_fPosX, (int)e_fPosY, e_strInfo.c_str());
	}

	bool	cGameApp::SoundPlay(const wchar_t*e_strSoundName, bool e_bPlay)
	{
		if (m_spSoundParser)
		{
			cBasicSound*l_pSound = GetSound(e_strSoundName);
			if (!l_pSound)
			{
				l_pSound = GetSoundByFileName(e_strSoundName);
			}
			if (l_pSound)
			{
				l_pSound->Play(e_bPlay);
				return true;
			}
		}
		return false;
	}


	cBasicSound*	cGameApp::GetSoundByFileName(const wchar_t*e_strSoundFileName)
	{
		std::wstring	l_strSoundName = UT::GetFileNameWithoutFullPath(e_strSoundFileName);
		cBasicSound*l_pSound = GetSound(l_strSoundName.c_str());
		if (!l_pSound)
		{
			std::string	l_strFileName = UT::WcharToChar(e_strSoundFileName);
			if (IsFileExists(l_strFileName.c_str()))
			{
				if (m_spSoundParser && cGameApp::m_spSoundParser->AddSound(cGameApp::m_spSoundParser, l_strFileName.c_str()))
				{
					l_pSound = m_spSoundParser->GetObject(l_strSoundName.c_str());
				}
			}
		}
		return l_pSound;
	}
	cBasicSound*	cGameApp::GetSound(const wchar_t*e_strSoundName)
	{
		if (m_spSoundParser)
			return m_spSoundParser->GetObject(e_strSoundName);
		return 0;
	}

	void		cGameApp::OutputDebugInfoString(const wchar_t*e_str, bool e_bWithNextLineSymbol, bool e_bWriteLog)
	{
		FMLog::Log(e_str,e_bWriteLog);
	}

	void		cGameApp::OutputDebugInfoString(std::string e_str, bool e_bWithNextLineSymbol, bool e_bWriteLog)
	{
		FMLog::Log(e_str.c_str(),e_bWriteLog);
	}

	void		cGameApp::OutputDebugInfoString(const char*e_str, bool e_bWithNextLineSymbol, bool e_bWriteLog)
	{
		FMLog::Log(e_str,e_bWriteLog);
	}

	void		cGameApp::OutputDebugInfoString(std::wstring e_str, bool e_bWithNextLineSymbol, bool e_bWriteLog)
	{
		FMLog::Log(e_str.c_str(),e_bWriteLog);
	}

	void		cGameApp::ApplyViewPort()
	{
		glViewport((int)cGameApp::m_spOpenGLRender->m_vViewPortSize.x, (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.y, (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
	}

	void		cGameApp::WriteLog(std::wstring e_strMessage)
	{
		WriteLog(e_strMessage.c_str());
	}

	void		cGameApp::WriteLog(const wchar_t*e_strMessage)
	{
		FMLog::WriteLog(e_strMessage);
	}

	void		cGameApp::WriteLog(const char*e_strMessage)
	{
		FMLog::WriteLog(e_strMessage);
	}
	void		cGameApp::WriteLog(std::string e_strMessage)
	{
		FMLog::WriteLog(e_strMessage);
	}

	void		cGameApp::CallExternalFunction(std::wstring e_strName, void*e_pData)
	{
		if (m_spExternalFunctionVector && e_strName.length() > 0)
		{
			cExternalFunction*l_p = m_spExternalFunctionVector->GetObject(e_strName);
			if (l_p)
			{
				l_p->m_pDoButtonGoal_Callback(e_pData);
			}
		}
	}

	bool		cGameApp::AddExternalFunction(DoButtonGoal_Callback e_pCallBack, const wchar_t*e_strExternalFunctionName)
	{
		if (m_spExternalFunctionVector && e_strExternalFunctionName)
		{
			if (!m_spExternalFunctionVector->GetObject(e_strExternalFunctionName))
			{
				cExternalFunction*l_pExternalFunction = new cExternalFunction();
				l_pExternalFunction->SetName(e_strExternalFunctionName);
				l_pExternalFunction->m_pDoButtonGoal_Callback = e_pCallBack;
				m_spExternalFunctionVector->AddObjectNeglectExist(l_pExternalFunction);
			}
		}
		return false;
	}

	cGlyphFontRender*	cGameApp::GetGlyphFontRenderByFileName(const wchar_t*e_strFileName)
	{
		std::wstring		l_strName = UT::GetFileNameWithoutFullPath(e_strFileName);
		cGlyphFontRender*	l_pGlyphFontRender = m_spGlyphFontRenderVector->GetObject(l_strName.c_str());
		if (!l_pGlyphFontRender)
		{
			std::string	l_strFileName = UT::WcharToChar(e_strFileName);
			if (IsFileExists(l_strFileName.c_str()))
			{
				l_pGlyphFontRender = new cGlyphFontRender(l_strFileName.c_str());
				m_spGlyphFontRenderVector->AddObjectNeglectExist(l_pGlyphFontRender);
			}
		}
		return l_pGlyphFontRender;
	}

	cGlyphFontRender*	cGameApp::GetGlyphFontRender(const wchar_t*e_strFileName)
	{
		return m_spGlyphFontRenderVector->GetObject(e_strFileName);
	}

	void	cGameApp::ShowInfo()
	{
		if (!m_sbDebugFunctionWorking)
			return;
		//glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		glEnable2D(1280, 720);
//#ifndef OPENGLES_2_X
//		MyGLEnable(GL_TEXTURE_2D);
//#endif
		if (cGameApp::m_spGlyphFontRender)
		{
			cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1, 1, 0, 1));
			int	l_iStaryPosY = 0;
			wchar_t   l_str[MAX_PATH];
			swprintf(l_str, MAX_PATH, L"MousePos:%d,%d", cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y);
			cGameApp::m_spGlyphFontRender->RenderFont(0, l_iStaryPosY, l_str);
			l_iStaryPosY += 20;
			cGameApp::m_spGlyphFontRender->RenderFont(0, l_iStaryPosY, UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()).c_str());
			swprintf(l_str, MAX_PATH, L"RealMousePos:%d,%d", m_sScreenMousePosition.x, m_sScreenMousePosition.y);
			l_iStaryPosY += 20;
			cGameApp::m_spGlyphFontRender->RenderFont(0, l_iStaryPosY, l_str);
			std::wstring	l_strInfo = ComposeMsgByFormat(L"ElpaseTime:%.2f", (float)m_dbGamePlayTime);
			l_iStaryPosY += 20;
			cGameApp::m_spGlyphFontRender->RenderFont(0.f, (float)l_iStaryPosY, l_strInfo);
			l_iStaryPosY += 20;
			cGameApp::m_spGlyphFontRender->RenderFont(0.f, (float)l_iStaryPosY, cGameApp::m_sbDoLockFPS ? L"LockFPS" : L"No LockFPS");
			l_iStaryPosY += 20;
			l_strInfo = L"Speed:";
			l_strInfo += ValueToStringW(m_sfGameSpeedValue);
			cGameApp::m_spGlyphFontRender->RenderFont(0.f, (float)l_iStaryPosY, l_strInfo);
			l_iStaryPosY += 20;
			l_strInfo = L"Pause:";
			l_strInfo += ValueToStringW(m_sbGamePause);
			cGameApp::m_spGlyphFontRender->RenderFont(0.f, (float)l_iStaryPosY, l_strInfo);
			cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1, 1, 1, 1));

		}
		
		glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	}

	std::wstring	cGameApp::GetDIDOrInstallationRandomID()
	{
		std::wstring	l_str;
#ifdef ANDROID
		l_str = GetDeviceID();
		if (l_str.length())
			return l_str;
#endif
		l_str = L"79979";
		NvFile*l_pFile = UT::MyFileOpen("InstallFile", "r");
		wchar_t	l_pData[MAX_PATH];
		if (l_pFile)
		{
			int	l_iSize = UT::GetFileSize(l_pFile);
			memset(l_pData, 0, sizeof(wchar_t)*MAX_PATH);
			if (NvFRead(l_pData, l_iSize, 1, l_pFile) == l_iSize)
			{
				l_str = l_pData;
			}
		}
		else
		{
			l_pFile = UT::MyFileOpen("InstallFile", "w");
			if (l_pFile)
			{
				int	l_iLength = rand() % 64;
				for (int i = 0; i<l_iLength; ++i)
				{
					l_pData[i] = rand();
				}
				if (l_iLength * sizeof(wchar_t) == NvFWrite(l_pData, sizeof(wchar_t), l_iLength, l_pFile))
				{
					l_str = l_pData;
				}
				NvFClose(l_pFile);
			}
		}
		return  l_str;
	}
	POINT GetScreenResolution()
	{
#if defined(WIN32) && !defined(UWP)
		POINT	l_ScreenResolution = { GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN) };
		return l_ScreenResolution;
#else
		POINT	l_ScreenResolution = { (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.x,(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.y };
		return l_ScreenResolution;
#endif
	}
	//end namespace
}
#if defined(WIN32) && !defined(UWP)
void PrintMemoryInfo(const wchar_t*e_strName)
//void PrintMemoryInfo( unsigned long processID )
{
	unsigned long processID = GetCurrentProcessId();
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	//   // Print the process identifier.
	wchar_t	l_str[MAX_PATH];
	swprintf(l_str, L"\nProcess ID: %u\n", processID);
	FMLog::LogWithFlag(l_str, CORE_LOG_FLAG);
	// Print information about the memory usage of the process.

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE,
		processID);
	if (nullptr == hProcess)
		return;

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		static bool	l_sbFirstTimeIntoHere = false;
		static	size_t	l_sMemoryUsed = 0;
		//swprintf(l_str,L"\tPageFaultCount: 0x%08X\n", pmc.PageFaultCount );

		//swprintf(l_str,L"\tPageFaultCount: %d\n", pmc.PageFaultCount );
		//cGameApp::OutputDebugInfoString(l_str);
		//      swprintf(l_str,L"\tYour app's PEAK MEMORY CONSUMPTION: %d\n", 
		//                pmc.PeakWorkingSetSize );
		//cGameApp::OutputDebugInfoString(l_str);
		//      swprintf(l_str,L"\tYour app's CURRENT MEMORY CONSUMPTION: %d\n", pmc.WorkingSetSize );
		FMLog::LogWithFlag(L"Memory Check Name:", CORE_LOG_FLAG);
		FMLog::LogWithFlag(e_strName, CORE_LOG_FLAG);
		swprintf(l_str, L"Current Memory:%zu\n", pmc.WorkingSetSize);
		FMLog::LogWithFlag(l_str, CORE_LOG_FLAG);
		if (!l_sbFirstTimeIntoHere)
		{
			l_sbFirstTimeIntoHere = true;
			l_sMemoryUsed = pmc.WorkingSetSize;
		}
		else
		{
			size_t	l_uiMemoryChangeSize = pmc.WorkingSetSize - l_sMemoryUsed;
			float	l_fMB = (float)l_uiMemoryChangeSize / 1024.f / 1024.f;
			float	l_fKB = (float)l_uiMemoryChangeSize / 1024.f;
			swprintf(l_str, L"Memory Change:In MB:%.2f,In KB:%.2f,In Byte:%zu\n", l_fMB, l_fKB, l_uiMemoryChangeSize);
			FMLog::LogWithFlag(l_str, CORE_LOG_FLAG);
			l_sMemoryUsed = pmc.WorkingSetSize;
		}
		//cGameApp::OutputDebugInfoString(l_str);
		//      swprintf(l_str,L"\tQuotaPeakPagedPoolUsage: %d\n", 
		//                pmc.QuotaPeakPagedPoolUsage );
		//cGameApp::OutputDebugInfoString(l_str);
		//      swprintf(l_str,L"\tQuotaPagedPoolUsage: %d\n", 
		//                pmc.QuotaPagedPoolUsage );
		//cGameApp::OutputDebugInfoString(l_str);
		//      swprintf(l_str,L"\tQuotaPeakNonPagedPoolUsage: %d\n", 
		//                pmc.QuotaPeakNonPagedPoolUsage );
		//cGameApp::OutputDebugInfoString(l_str);
		//      swprintf(l_str,L"\tQuotaNonPagedPoolUsage: %d\n", 
		//                pmc.QuotaNonPagedPoolUsage );
		//cGameApp::OutputDebugInfoString(l_str);
		//      swprintf(l_str,L"\tPagefileUsage: %d\n", pmc.PagefileUsage ); 
		//cGameApp::OutputDebugInfoString(l_str);
		//      swprintf(l_str,L"\tPeakPagefileUsage: %d\n", 
		//                pmc.PeakPagefileUsage );
		//cGameApp::OutputDebugInfoString(l_str);
	}

	//CloseHandle( hProcess );
}
#endif