#include "../stdafx.h"
#include "GameApp.h"

#ifdef WIN32
//#include "../../../include/IL/il.h"
#include <direct.h>
#endif
#include "../Sound/SoundManager.h"
#include "../Particle/AllParticleInclude.h"
#include "BinaryFile.h"
#include "../Synchronization/AllFUThreadInclude.h"
#include "ImageButton.h"
#include "../Physic/2DImageCollisionData.h"

#include "EventSender/MessageSender.h"

#if defined(ANDROID)//openAL,android.c
#include <android/log.h>
#include "../Android/nv_egl_util.h"
#include "../Android/JNIUtil.h"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG,"GameApp",__VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"GameApp",__VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,"GameApp",__VA_ARGS__))
#endif
namespace	FATMING_CORE
{
	extern POINT	ConvertCoordinate(int e_iPosX,int e_iPosY,POINT e_ViewPort);
	extern void	DumpGraphicsInfo();

#if defined(ANDROID)
	std::string*										cGameApp::m_spAPKPath = 0;
	std::string*										cGameApp::m_spInternalDirectory = 0;
	std::string*										cGameApp::m_spExternalDirectory = 0;
	std::string*										cGameApp::m_spExternalSDDirectory = 0;
	JNIEnv*												cGameApp::m_spThreadEnv = 0;
	ANativeActivity*									cGameApp::m_spANativeActivity = 0;
	jobject*											cGameApp::m_spAppThreadThis = 0;
	NvEGLUtil*											cGameApp::m_pNvEGLUtil = 0;
	sJNIUtilData*										cGameApp::m_spJNIUtilData = 0;
#elif defined(WIN32)
	HDC													cGameApp::m_sHdc = 0;
	HANDLE												cGameApp::m_sHandle = 0;
	HGLRC												cGameApp::m_sHGLRC = 0;
#endif
	double												cGameApp::m_dbGamePlayTime = 0;
	float												cGameApp::m_sfDebugValue = 1.f;
	float												cGameApp::m_sfVersion = 1.f;
	bool												cGameApp::m_sbLeave = false;
	bool												cGameApp::m_sbSpeedControl = false;
	UT::sTimeAndFPS										cGameApp::m_sTimeAndFPS;
	cMessageSenderManager*								cGameApp::m_spMessageSenderManager = nullptr;
	cPaticleManager*									cGameApp::m_spPaticleManager = 0;
	cBehaviorPaticleManager*							cGameApp::m_spBehaviorPaticleManager = 0;
	cGlyphFontRender*									cGameApp::m_spGlyphFontRender = 0;
	cNamedTypedObjectVector<cGlyphFontRender>*			cGameApp::m_spGlyphFontRenderVector = 0;
	cNamedTypedObjectVector<cExternalFunction>*			cGameApp::m_spExternalFunctionVector = 0;
	cNamedTypedObjectVector<c2DImageCollisionData>*		cGameApp::m_sp2DImageCollisionDataVector = 0;
	cNamedTypedObjectVector<NamedTypedObject>*			cGameApp::m_spColladaParserVector = 0;
	Vector4												cGameApp::m_svViewPortSize(0.f,0.f,1920.f,1080.f);
	Vector4												cGameApp::m_svDeviceViewPortSize(0.f,0.f,1920.f,1080.f);	
	Vector4												cGameApp::m_svBGColor(0.1f,0.1f,0.1f,1.f);	
	Vector2												cGameApp::m_svGameResolution(1920.f,1080.f);
	Vector2												cGameApp::m_svGameScale(1.f,1.f);
	Vector3												cGameApp::m_svAccelerometer = Vector3::Zero;
	float												cGameApp::m_sfForce = 0.f;	
	POINT												cGameApp::m_sMousePosition = {0,0};
	POINT												cGameApp::m_sScreenMousePosition = {0,0};
	bool												cGameApp::m_sbMouseClickStatus[3] = {false,false,false};
	short												cGameApp::m_sMouseWhellDelta = 0;
	bool												cGameApp::m_sbFullScreen = false;
	bool    											cGameApp::m_bMouseHover = false;;
	bool												cGameApp::m_sbTouched = false;
	bool												cGameApp::m_sbGamePause = false;
	bool												cGameApp::m_sbDeviceExist = false;	
	sMultiTouchPoints*									cGameApp::m_spMultiTouchPoints = 0;
	bool												cGameApp::m_sbDoMultiTouch = false;
	cAnimationParser*									cGameApp::m_spAnimationParser = 0;
	cImageParser*										cGameApp::m_spImageParser = 0;
	cSoundParser*										cGameApp::m_spSoundParser = 0;
	cNamedTypedObjectVector<cCurveManager>*				cGameApp::m_spPathFileList;
	int													cGameApp::m_siShowErrorType = 1;
	std::wstring*										cGameApp::m_spstrErrorMsgString = 0;
	bool												cGameApp::m_sucKeyData[MAX_PATH];
	bool												cGameApp::m_sbEnableMouseSingnal = true;
	bool												cGameApp::m_sbEnableKeyboardSingnal = true;
	bool												cGameApp::m_sbDebugFunctionWorking = false;
	bool												cGameApp::m_sbAllowParseBinaryFile = true;
	std::string*										cGameApp::m_psstrGameAppName = 0;
	cBinaryFile*										cGameApp::m_spLogFile = 0;
//#if defined(ANDROID) || defined(IOS) 
	eDeviceDirection									cGameApp::m_seDeviceDirection = eDD_PORTRAIT;
	//eDeviceDirection									cGameApp::m_seDeviceDirection = eDD_LANDSCAPE_LEFT;
	//eDeviceDirection									cGameApp::m_seDeviceDirection = eDD_LANDSCAPE_RIGHT;
	//eDeviceDirection									cGameApp::m_seDeviceDirection = eDD_UPSIDE_DOWN;
//#else
	//eDeviceDirection									cGameApp::m_seDeviceDirection = eDD_LANDSCAPE_LEFT;
//#endif

#if defined(ANDROID)
#include "../Android/nv_file.h"
	extern sJNIUtilData*g_pMainThreadJNIUtilData;
	extern bool		GetExternalSDPath(std::string*e_pTarget);
	extern void	SetupAPKFilePath(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv);
	cGameApp::cGameApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject* e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil)
#elif defined( WIN32)
	cGameApp::cGameApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize)
#else
	cGameApp::cGameApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize)
#endif
	{
#ifdef WIN32
		DWORD  nBufferLength = 0;
		wchar_t l_strDirectory[MAX_PATH];
		GetCurrentDirectory(nBufferLength, l_strDirectory);
		std::wstring l_strDirectpryInfo = L"Working Directory:";
		l_strDirectpryInfo += l_strDirectory;
		cGameApp::OutputDebugInfoString(l_strDirectpryInfo);
#endif
		m_bDoScreenShot = false;
		m_sbMouseClickStatus[0] = m_sbMouseClickStatus[1] = m_sbMouseClickStatus[2] = false;
		cGameApp::m_sbGamePause = false;
		cGameApp::m_psstrGameAppName = new std::string;
		*cGameApp::m_psstrGameAppName = "GameApp";
		OutputDebugInfoString(L"GameApp Start");
		m_dbGamePlayTime = 0;
#if defined(ANDROID)
		m_spThreadEnv = e_pThreadEnv;
		m_spANativeActivity = e_pActivity;
		m_spAppThreadThis = e_pAppThreadThis;
		m_pNvEGLUtil = e_pNvEGLUtil;
		m_spJNIUtilData = new sJNIUtilData(m_spThreadEnv);
		m_spJNIUtilData->Init();
		g_pMainThreadJNIUtilData = m_spJNIUtilData;
		SetupAPKFilePath(e_pActivity,e_pThreadEnv);
#endif
#ifdef DEBUG
		if( !m_spLogFile )
		{
			OutputDebugInfoString(L"log file");
			m_spLogFile = new cBinaryFile();
#ifdef WIN32
			int	l_iValue = _mkdir("Log");
			//if(-1 != l_iValue)
			{
				std::wstring	l_strSystemTime = L"Log/log_";
				l_strSystemTime += UT::GetSystemTimeForFile(true);
				l_strSystemTime += L".txt";
				std::string	l_strFileName = UT::WcharToChar(l_strSystemTime);
				m_spLogFile->Writefile(l_strFileName.c_str(),false,false);
			}
#else
			if( m_spLogFile )
			{
				m_spLogFile->Writefile("log.txt",false,false);
				if(m_spLogFile->GetFile())
					OutputDebugInfoString(L"create log.txt OK");
				else
					OutputDebugInfoString(L"create log.txt failed");
			}
#endif
		}
#endif
		SystemErrorCheck();
#if	defined(WIN32)
		PrintMemoryInfo();
//		ilInit();
		if( e_Hwnd )
		{
			OpenglInit(e_Hwnd);
			//m_sHdc = GetDC(e_Hwnd);
			m_sHandle = e_Hwnd;
		}
#endif
		memset(m_sucKeyData,0,sizeof(bool)*MAX_PATH);
        m_bMouseHover = false;

		m_svGameResolution = e_vGameResolution;
		m_svViewPortSize.x = 0.f;
		m_svViewPortSize.y = 0.f;
		m_svViewPortSize.z = (float)e_vViewportSize.x;
		m_svViewPortSize.w = (float)e_vViewportSize.y;
		this->m_svDeviceViewPortSize = m_svViewPortSize;
		m_spMultiTouchPoints = new sMultiTouchPoints();
        if( !m_spAnimationParser )
        {
			//in android some how it will close force sunndenly by I donno who.
			//m_spThreadPool = new cFUThreadPool;
			m_spMessageSenderManager = new cMessageSenderManager();
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
#if defined WIN32 || defined LINUX
	        char *locale;
            locale = setlocale(LC_ALL, "");
#endif
		//std::wstring l_str111 = GetDIDOrInstallationRandomID();
		//std::string l_str222 = UT::WcharToChar(l_str111);
		//const char* l_str333 = l_str222.c_str();
		//SystemErrorCheck();
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
		SAFE_DELETE(m_spLogFile);
		//SAFE_DELETE(m_spThreadPool);
		SAFE_DELETE(cGameApp::m_psstrGameAppName);
		NamedTypedObject::DumpUnReleaseInfo();
	}
#ifdef WIN32
	void	cGameApp::OpenglInit(HWND e_Hwnd)
	{
		m_sHdc = GetDC(e_Hwnd);
		m_sHGLRC = UT::InitOpenGL(e_Hwnd,true,m_sHdc);
	}
#endif
	void	cGameApp::Init()
	{
#ifdef DEBUG
		DumpGraphicsInfo();
#endif
		m_sbSpeedControl = this->m_sbDebugFunctionWorking;
		m_sbLeave = false;
		SystemErrorCheck();
		//This hint can improve the speed of texturing when perspective- correct texture coordinate interpolation isn't needed, such as when using a glOrtho() projection.
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
#ifdef OPENGLES_2_X
		g_bVBOSupported = true;
		glUseProgram( 0 );
#else
		glEnableClientState(GL_VERTEX_ARRAY);
#endif
		if( m_spLogFile )
			m_spLogFile->WriteToFileImmediatelyWithLine("init shader");

		//2d image shader
		CreateShader(g_bCommonVSClientState,DEFAULT_SHADER);
		//for non texture shader
		CreateShader(g_bCommonVSNoTextureClientState,NO_TEXTURE_SHADER);

		CreateShader(g_bMyMeshVSClientState,g_strMyMeshVS,g_strMyMeshFS,STATIC_MESH_SHADER);
		//if crush go to char*g_strMySkinningMeshVS = "
		//fin matBones[32] and change its size...
		CreateShader(g_bMySkinningMeshVSClientState,g_strMySkinningMeshVS,g_strMySkinningMeshFS,SKINNING_MESH_SHADER);
		if( m_spLogFile )
			m_spLogFile->WriteToFileImmediatelyWithLine("init shader ok");
		m_sTimeAndFPS.Update();
		if( m_spLogFile )
			m_spLogFile->WriteToFileImmediatelyWithLine("parse font data");
		if( !m_spGlyphFontRender )
		{
			m_spGlyphFontRender = new cGlyphFontRender("Font",3000);
			if(!m_spGlyphFontRender->GetFontImage())
			{
				SAFE_DELETE(m_spGlyphFontRender);
			}
			else
			{
				m_spGlyphFontRenderVector->AddObject(m_spGlyphFontRender);
			}
		}
		if( m_spLogFile )
		{
			if( !m_spGlyphFontRender)
				m_spLogFile->WriteToFileImmediatelyWithLine("parse font data failed");
			else
				m_spLogFile->WriteToFileImmediatelyWithLine("parse font data ok");
		}
		if( !m_spstrErrorMsgString )
			m_spstrErrorMsgString = new std::wstring;
		m_sTimeAndFPS.Update();
		srand(m_sTimeAndFPS.uiCurrentTime);
		SystemErrorCheck();
	}

	void	cGameApp::Destroy()
	{
		if( m_spLogFile )
			m_spLogFile->WriteToFileImmediatelyWithLine("Destroy start");
		SystemErrorCheck();
		SAFE_DELETE(m_spMessageSenderManager);
		SAFE_DELETE(m_sp2DImageCollisionDataVector);
		SAFE_DELETE(m_spColladaParserVector);
		SAFE_DELETE(m_spMultiTouchPoints);
		SAFE_DELETE(m_spSoundParser);
		SAFE_DELETE(m_spstrErrorMsgString);
	    SAFE_DELETE(m_spAnimationParser);
		SAFE_DELETE(m_spBehaviorPaticleManager);
		SAFE_DELETE(m_spPathFileList);
		SAFE_DELETE(m_spGlyphFontRenderVector);
		m_spGlyphFontRender = 0;
		SAFE_DELETE(m_spExternalFunctionVector);
		DeleteAllShader();
#if	defined(WIN32) || defined(LINUX)
//		ilShutDown();
#endif
		SystemErrorCheck();
		if( m_spLogFile )
			m_spLogFile->WriteToFileImmediatelyWithLine("all Destroy done");
	}

	void	cGameApp::Update(float e_fElpaseTime)
	{
		m_dbGamePlayTime += e_fElpaseTime;
		ShaderUpdate(e_fElpaseTime);
		if(m_spMessageSenderManager)
			m_spMessageSenderManager->Update(e_fElpaseTime);
		SystemErrorCheck();
	}
	extern cBaseShader*g_pCurrentShader;
	void	cGameApp::Render()
	{
		g_pCurrentShader = 0;
#ifndef OPENGLES_2_X
		glEnableClientState(GL_VERTEX_ARRAY);
#endif
		//first for original resolution
		glViewport(0,0,(GLsizei)this->m_svDeviceViewPortSize.Width(),(GLsizei)this->m_svDeviceViewPortSize.Height());
		glClearColor(m_svBGColor.x,m_svBGColor.y,m_svBGColor.z,m_svBGColor.w);
		glViewport((GLint)m_svViewPortSize.x,(GLint)m_svViewPortSize.y,(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height());
		glEnable(GL_SCISSOR_TEST);
		glScissor((GLint)m_svViewPortSize.x,(GLint)m_svViewPortSize.y,(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height());
		SystemErrorCheck();
		//the comment part is required if u are not using GameApp::Run
		//DEFAULT_SHADER = L"MyPrtShader";
		//NO_TEXTURE_SHADER =L"MyPrtShaderNoTexture";
		UseShaderProgram(DEFAULT_SHADER);
		cTexture::m_suiLastUsingImageIndex = -1;
		//FATMING_CORE::UseShaderProgram();
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearColor( m_svBGColor.x,m_svBGColor.y,m_svBGColor.z,m_svBGColor.w );
		//glClearDepth(1.0f);								// Depth Buffer Setup
		glEnable2D(m_svGameResolution.x,m_svGameResolution.y);
		SystemErrorCheck();
	}

	void	cGameApp::Run()
	{
	    cTexture::m_suiLastUsingImageIndex = -1;
		m_sTimeAndFPS.Update();
		float	l_fElpaseTime = m_sTimeAndFPS.fElpaseTime;
#ifdef DEBUG
		if( m_sbGamePause )
			l_fElpaseTime = 0.f;
#endif
		Update(m_sbSpeedControl?l_fElpaseTime*this->m_sfDebugValue:l_fElpaseTime);
		Render();
		if( m_bDoScreenShot )
		{
			m_bDoScreenShot = false;
			std::wstring l_strFileName;
			if( m_psstrGameAppName )
			{
				l_strFileName += ValueToStringW(this->m_psstrGameAppName->c_str());
				l_strFileName += L"_";
			}
			l_strFileName += L"ScreenShot_";
			l_strFileName += GetSystemTimeForFile(false);
			l_strFileName += L".jpeg";
			SaveCurrentBufferToImage(ValueToString(l_strFileName).c_str());
		}
		cGameApp::m_sMouseWhellDelta = 0;
	}

	void	cGameApp::MouseDown(int e_iPosX,int e_iPosY)
	{
		m_sScreenMousePosition.x = e_iPosX;
		m_sScreenMousePosition.y = e_iPosY;
		//if( !m_sbEnableMouseSingnal )
		//	return;
		POINT	l_Viewport = {(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height()};
		m_sMousePosition = ConvertCoordinate(e_iPosX,e_iPosY,l_Viewport);
		m_sbTouched = true;
	}

	void	cGameApp::MouseMove(int e_iPosX,int e_iPosY)
	{
		m_sScreenMousePosition.x = e_iPosX;
		m_sScreenMousePosition.y = e_iPosY;
		//if( !m_sbEnableMouseSingnal )
		//	return;
		POINT	l_Viewport = {(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height()};
        m_sMousePosition = ConvertCoordinate(e_iPosX,e_iPosY,l_Viewport);
	}

	void	cGameApp::MouseUp(int e_iPosX,int e_iPosY)
	{
		m_sScreenMousePosition.x = e_iPosX;
		m_sScreenMousePosition.y = e_iPosY;
		//if( !m_sbEnableMouseSingnal )
		//	return;
		POINT	l_Viewport = {(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height()};
        m_sMousePosition = ConvertCoordinate(e_iPosX,e_iPosY,l_Viewport);
		m_sbTouched = false;
	}

	void	cGameApp::MouseButtonClickEvent(bool e_bDown,int e_iButtonIndex)
	{
		assert( e_iButtonIndex < 3 &&"only 012" );
		m_sbMouseClickStatus[e_iButtonIndex] = e_bDown;
	}
	
    void    cGameApp::TouchSignalProcess(int e_iPosX,int e_iPosY,bool e_bTouch)
    {
		//if( !m_sbEnableMouseSingnal )
		//	return;
	    if( e_bTouch )
	    {
		    if( m_bMouseHover )
			    MouseMove(e_iPosX,e_iPosY);
		    else
			    MouseDown(e_iPosX,e_iPosY);
		    m_bMouseHover = true;
		    return;
	    }
		else
		{
			POINT	l_Viewport = {(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height()};
			m_sMousePosition = ConvertCoordinate(e_iPosX,e_iPosY,l_Viewport);
		}
	    if(m_bMouseHover)
		    MouseUp(e_iPosX,e_iPosY);
	    m_bMouseHover = false;
    }

	void	cGameApp::KeyDown(char e_char)
	{
		if(!m_sbEnableKeyboardSingnal)
			return;
		m_sucKeyData[(unsigned char)e_char] = true;
	}

	void	cGameApp::KeyPress(char e_char)
	{
		if(!m_sbEnableKeyboardSingnal)
			return;
		//m_sucKeyData[(unsigned char)e_char] = true;
	}

	void	cGameApp::KeyUp(char e_char)
	{
		if(!m_sbEnableKeyboardSingnal)
			return;
		//alter
		if (m_sucKeyData[17])
		{
			if (m_sbDebugFunctionWorking)
			{
				switch (e_char)
				{
					case 'R':
						m_sfDebugValue = 1.f;
						break;
					case 'P':
						m_sbGamePause = !m_sbGamePause;
						break;
					case 38://up
						this->m_seDeviceDirection = eDD_PORTRAIT;
						break;
					case 37://left
						this->m_seDeviceDirection = eDD_LANDSCAPE_LEFT;
						break;
					case 39://right
						this->m_seDeviceDirection = eDD_LANDSCAPE_RIGHT;
						break;
					case 40://down
						this->m_seDeviceDirection = eDD_UPSIDE_DOWN;
						break;
					case 44://PrtScr/SysRq
						m_bDoScreenShot = true;
						break;
					case 107://107'+'
						m_sfDebugValue *= 2.f;
						if (m_sfDebugValue > 1073741824)
							m_sfDebugValue = 1073741824;
						break;
					case 109://109'-'
						m_sfDebugValue /= 2.f;
						if (m_sfDebugValue <= 0.00000000001f)
							m_sfDebugValue = 0.00000000001f;
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
		if( cGameApp::m_sbGamePause )
		{
			RenderFilledRectangle(Vector2(0,0),cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,Vector4(0,0,0,0.7f),0);
			wchar_t	l_strDescription[] = {36938,25138,26283,20572,20013,32,32,20877,25353,19968,27425,36864,20986,0};
			m_spGlyphFontRender->SetScale(1.5f);
			cGameApp::RenderFont(cGameApp::m_svGameResolution.x/2-200,cGameApp::m_svGameResolution.y/2,l_strDescription);
			m_spGlyphFontRender->SetScale(1.3f);
			cGameApp::RenderFont(cGameApp::m_svGameResolution.x/2-300,cGameApp::m_svGameResolution.y/2+30,L"Pause! press back again to exit");
			m_spGlyphFontRender->SetScale(1.f);
		}
	}

	cPuzzleImage*	cGameApp::GetPuzzleImage(const wchar_t*e_strName){ return dynamic_cast<cPuzzleImage*>(m_spImageParser->GetObject(e_strName)); }
	cBaseImage*		cGameApp::GetBaseImage(const wchar_t*e_strName){  return dynamic_cast<cBaseImage*>(m_spImageParser->GetObject(e_strName));  }
	cPuzzleImage*	cGameApp::GetPuzzleImageByFileName(std::wstring e_strFileName){ return GetPuzzleImageByFileName(e_strFileName.c_str()); }
	cPuzzleImage*	cGameApp::GetPuzzleImageByFileName(const wchar_t*e_strFileName){ return m_spImageParser->GetPuzzleImageByFileName(e_strFileName); }
	cMPDIList*		cGameApp::GetMPDIList(const wchar_t*e_strName){ return m_spAnimationParser->GetMPDIList(e_strName); }
	cMPDIList*		cGameApp::GetMPDIListByFileName(const wchar_t*e_strFileName,bool e_bShowErrorMsg){ return m_spAnimationParser->GetMPDIListByFileName(e_strFileName,e_bShowErrorMsg); }
	cMPDIList*		cGameApp::GetMPDIList(const char*e_strFileName,const wchar_t*e_strName){ return m_spAnimationParser->GetMPDIList(e_strFileName,e_strName); }
	cMPDIList*		cGameApp::GetMPDIList(int e_iIndex){ return m_spAnimationParser->GetMPDIList(e_iIndex); }

	cCurveManager*	cGameApp::GetCurveManagerByFileName(const wchar_t*e_strFileName)
	{
		std::wstring	l_strName = UT::GetFileNameWithoutFullPath(e_strFileName);
		cCurveManager*l_pCurveManager = m_spPathFileList->GetObject(l_strName.c_str());
		if( l_pCurveManager )
			return l_pCurveManager;
		l_pCurveManager = new cCurveManager();
		if(l_pCurveManager->Parse(UT::WcharToChar(e_strFileName).c_str()))
		{
			l_pCurveManager->SetName(l_strName);
			m_spPathFileList->AddObjectNeglectExist(l_pCurveManager);
			l_pCurveManager->DoScale(cGameApp::m_svGameScale);
			return l_pCurveManager;
		}
		delete l_pCurveManager;
		return 0;
	}

	cParticleEmitterGroup*cGameApp::GetPRTG(const wchar_t*e_strFileName,const wchar_t*e_strName){ return GetPRTG(UT::WcharToChar(e_strFileName).c_str(),e_strName); }
	cParticleEmitterGroup*cGameApp::GetPRTG(const char*e_strFileName,const wchar_t*e_strName){ return m_spBehaviorPaticleManager->GetPRTG(e_strFileName,e_strName); }
	cParticleEmitterGroup*cGameApp::GetPRTG(const wchar_t*e_strName){ return m_spBehaviorPaticleManager->GetPRTG(e_strName); }

	cPrtEmitter*cGameApp::GetPrtEmitter(const char*e_strFileName,const wchar_t*e_strName){ return m_spBehaviorPaticleManager->GetPaticleManager()->GetEmitter(e_strFileName,e_strName); }
	cPrtEmitter*cGameApp::GetPrtEmitter(const wchar_t*e_strFileName,const wchar_t*e_strName){ return GetPrtEmitter(UT::WcharToChar(e_strFileName).c_str(),e_strName); }
	cPrtEmitter*cGameApp::GetPrtEmitter(const wchar_t*e_strName){ return m_spBehaviorPaticleManager->GetPaticleManager()->GetObject(e_strName); }

	void	cGameApp::RenderFont(float e_fPosX,float e_fPosY,const wchar_t*e_strInfo)
	{
			RenderFont((int)e_fPosX,(int)e_fPosY,e_strInfo);
	}


	void	cGameApp::RenderFont(long e_iPosX,long e_iPosY,const wchar_t*e_strInfo)
	{
		if( m_spGlyphFontRender )
		{
			m_spGlyphFontRender->RenderFont((int)e_iPosX,(int)e_iPosY,e_strInfo);
		}	
	}

	void	cGameApp::RenderFont(int e_iPosX,int e_iPosY,const wchar_t*e_strInfo)
	{
		if( m_spGlyphFontRender )
		{
			m_spGlyphFontRender->RenderFont(e_iPosX,e_iPosY,e_strInfo);
		}
	}

	void	cGameApp::RenderFont(int e_iPosX,int e_iPosY,std::wstring e_strInfo)
	{
		RenderFont(e_iPosX,e_iPosY,e_strInfo.c_str());
	}

	void	cGameApp::RenderFont(Vector2 e_vPos,const wchar_t*e_pString)
	{
		RenderFont(e_vPos.x,e_vPos.y,e_pString);
	}

	void	cGameApp::RenderFont(float e_fPosX,float e_fPosY,std::wstring e_strInfo)
	{
		RenderFont((int)e_fPosX,(int)e_fPosY,e_strInfo.c_str());
	}

	bool	cGameApp::SoundPlay(const wchar_t*e_strSoundName,bool e_bPlay)
	{
		if( m_spSoundParser )
		{
			cBasicSound*l_pSound = GetSound(e_strSoundName);
			if( !l_pSound )
			{
				l_pSound = GetSoundByFileName(e_strSoundName);
			}
			if( l_pSound )
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
		if( !l_pSound )
		{
			std::string	l_strFileName = UT::WcharToChar(e_strSoundFileName);
			if(IsFileExists(l_strFileName.c_str()))
			{
				if(m_spSoundParser && cGameApp::m_spSoundParser->AddSound(cGameApp::m_spSoundParser,l_strFileName.c_str()))
				{
					l_pSound = m_spSoundParser->GetObject(l_strSoundName.c_str());
				}
			}
		}
		return l_pSound;
	}
	cBasicSound*	cGameApp::GetSound(const wchar_t*e_strSoundName)
	{
		if( m_spSoundParser )
			return m_spSoundParser->GetObject(e_strSoundName);
		return 0;
	}

	void		cGameApp::OutputDebugInfoString(const wchar_t*e_str,bool e_bWithNextLineSymbol, bool e_bWriteLog)
	{
#ifdef DEBUG		
	#if defined(WIN32)
			::OutputDebugString(e_str);
			if( e_bWithNextLineSymbol )
				::OutputDebugString(L"\n");
	#elif	defined(ANDROID)
			std::string	l_str = "GameApp___ ";
			l_str += UT::WcharToChar(e_str);
			LOGI("%s",l_str.c_str());
	#elif	defined(IOS)

	#endif
			std::string	l_str = UT::WcharToChar(e_str);
			printf(l_str.c_str());
			if (e_bWriteLog)
			{
				cGameApp::WriteLog(ValueToString(e_str));
			}
#endif
	}

	void		cGameApp::OutputDebugInfoString(std::string e_str,bool e_bWithNextLineSymbol, bool e_bWriteLog)
	{
		cGameApp::OutputDebugInfoString(UT::CharToWchar(e_str), e_bWithNextLineSymbol, e_bWriteLog);
	}

	void		cGameApp::OutputDebugInfoString(const char*e_str,bool e_bWithNextLineSymbol, bool e_bWriteLog)
	{
		cGameApp::OutputDebugInfoString(UT::CharToWchar(e_str), e_bWithNextLineSymbol, e_bWriteLog);
	}

	void		cGameApp::OutputDebugInfoString(std::wstring e_str,bool e_bWithNextLineSymbol, bool e_bWriteLog)
	{
		cGameApp::OutputDebugInfoString(e_str.c_str(), e_bWithNextLineSymbol,e_bWriteLog);
	}

	void		cGameApp::ApplyViewPort()
	{
		glViewport((int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y,(int)cGameApp::m_svViewPortSize.Width(),(int)cGameApp::m_svViewPortSize.Height());
	}

	Vector2		cGameApp::GetViewPortAndGameResolutionScale()
	{
		switch(cGameApp::m_seDeviceDirection)
		{
				case eDD_PORTRAIT:
				case eDD_UPSIDE_DOWN:
					return Vector2(m_svGameResolution.x/m_svViewPortSize.Width(),m_svGameResolution.y/m_svViewPortSize.Height());
				case eDD_LANDSCAPE_LEFT:
				case eDD_LANDSCAPE_RIGHT:
					return Vector2(m_svGameResolution.x/m_svViewPortSize.Height(),m_svGameResolution.y/m_svViewPortSize.Width());
		}
		return Vector2(1,1);
	}

	void		cGameApp::WriteLog(std::wstring e_strMessage)
	{
		WriteLog((wchar_t*)e_strMessage.c_str());
	}

	void		cGameApp::WriteLog(wchar_t*e_strMessage)
	{
		if(cGameApp::m_spLogFile)cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(e_strMessage);	
	}

	void		cGameApp::WriteLog(char*e_strMessage)
	{
		if(cGameApp::m_spLogFile)cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(e_strMessage);	
	}
	void		cGameApp::WriteLog(std::string e_strMessage)
	{
		WriteLog((char*)e_strMessage.c_str());
	}

	void		cGameApp::CallExternalFunction(std::wstring e_strName,void*e_pData)
	{
		if( m_spExternalFunctionVector && e_strName.length() > 0 )
		{
			cExternalFunction*l_p = m_spExternalFunctionVector->GetObject(e_strName);
			if( l_p )
			{
				l_p->m_pDoButtonGoal_Callback(e_pData);
			}
		}
	}

	bool		cGameApp::AddExternalFunction(DoButtonGoal_Callback e_pCallBack,const wchar_t*e_strExternalFunctionName)
	{
		if( m_spExternalFunctionVector && e_strExternalFunctionName)
		{
			if(!m_spExternalFunctionVector->GetObject(e_strExternalFunctionName))
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
		std::wstring		l_strName =			 UT::GetFileNameWithoutFullPath(e_strFileName);
		cGlyphFontRender*	l_pGlyphFontRender = m_spGlyphFontRenderVector->GetObject(l_strName.c_str());
		if( !l_pGlyphFontRender )
		{
			std::string	l_strFileName = UT::WcharToChar(e_strFileName);
			if(IsFileExists(l_strFileName.c_str()))
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
		if( !m_sbDebugFunctionWorking )
			return;
		//glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
		glEnable2D(1280,720);
#ifndef OPENGLES_2_X
		glEnable(GL_TEXTURE_2D);
#endif
		if( cGameApp::m_spGlyphFontRender )
		{
			cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,0,1));
			int	l_iStaryPosY = 0;
			wchar_t   l_str[MAX_PATH];
			swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
			cGameApp::m_spGlyphFontRender->RenderFont(0,l_iStaryPosY,l_str);
			l_iStaryPosY += 20;
			cGameApp::m_spGlyphFontRender->RenderFont(0,l_iStaryPosY,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()).c_str());
			swprintf(l_str,MAX_PATH,L"RealMousePos:%d,%d",m_sScreenMousePosition.x,m_sScreenMousePosition.y);
			l_iStaryPosY += 20;
			cGameApp::m_spGlyphFontRender->RenderFont(0,l_iStaryPosY,l_str);
			std::wstring	l_strInfo = ComposeMsgByFormat(L"ElpaseTime:%.2f",(float)m_dbGamePlayTime);
			l_iStaryPosY += 20;
			cGameApp::m_spGlyphFontRender->RenderFont(0.f,(float)l_iStaryPosY,l_strInfo);
			cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,1,1));
			
		}
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	}

	bool	cGameApp::EventMessageShot(unsigned int e_usID,void*e_pData)
	{
		if(m_spMessageSenderManager)
			return m_spMessageSenderManager->EventMessageShot(e_usID,e_pData);
		return false;
	}

	std::wstring	cGameApp::GetDIDOrInstallationRandomID()
	{
		std::wstring	l_str;
#ifdef ANDROID
		l_str = GetDeviceID();
		if( l_str.length() )
			return l_str;
#endif
		l_str = L"79979";
		NvFile*l_pFile = UT::MyFileOpen("InstallFile","r");
		wchar_t	l_pData[MAX_PATH];
		if( l_pFile )
		{
			int	l_iSize = UT::GetFileSize(l_pFile);
			memset(l_pData,0,sizeof(wchar_t)*MAX_PATH);
			if( NvFRead(l_pData,l_iSize,1,l_pFile) == l_iSize )
			{				
				l_str = l_pData;
			}
		}
		else
		{
			l_pFile = UT::MyFileOpen("InstallFile","w");
			if( l_pFile )
			{
				int	l_iLength = rand()%64;
				for( int i=0;i<l_iLength;++i )
				{
					l_pData[i] = rand();
				}
				if(l_iLength*sizeof(wchar_t) == NvFWrite(l_pData,sizeof(wchar_t),l_iLength,l_pFile))
				{
					l_str = l_pData;
				}
				NvFClose(l_pFile);
			}
		}
		return  l_str;	
	}
//end namespace
}
#ifdef WIN32
void PrintMemoryInfo(wchar_t*e_strName)
//void PrintMemoryInfo( unsigned long processID )
{
	unsigned long processID = GetCurrentProcessId();
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;

 //   // Print the process identifier.
	wchar_t	l_str[MAX_PATH];
    swprintf(l_str,L"\nProcess ID: %u\n", processID );
	cGameApp::OutputDebugInfoString(l_str);
    // Print information about the memory usage of the process.

    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                             PROCESS_VM_READ,
                             FALSE, 
                             processID );
    if (nullptr == hProcess)
        return;

    if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
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
		cGameApp::OutputDebugInfoString(L"Memory Check Name:");
		cGameApp::OutputDebugInfoString(e_strName);
		cGameApp::OutputDebugInfoString(L"\n");
		swprintf(l_str,L"Current Memory:%d\n",pmc.WorkingSetSize );
		cGameApp::OutputDebugInfoString(l_str);
		if( !l_sbFirstTimeIntoHere )
		{
			l_sbFirstTimeIntoHere = true;
			l_sMemoryUsed = pmc.WorkingSetSize;
		}
		else
		{
			size_t	l_iMemoryChangeSize = pmc.WorkingSetSize-l_sMemoryUsed;
			float	l_fMB = (float)l_iMemoryChangeSize/1024.f/1024.f;
			float	l_fKB = (float)l_iMemoryChangeSize/1024.f;
			swprintf(l_str,L"Memory Change:In MB:%.2f,In KB:%.2f,In Byte:%d\n",l_fMB,l_fKB,l_iMemoryChangeSize );
			cGameApp::OutputDebugInfoString(l_str);
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