#include "../stdafx.h"
#include "GameApp.h"

#ifdef WIN32
#include "../../../include/IL/il.h"
#include <direct.h>
#endif
#include "../Sound/SoundManager.h"
#include "../Particle/AllParticleInclude.h"
#include "BinaryFile.h"
#include "../Synchronization/AllFUThreadInclude.h"
#include "ImageButton.h"
#include "../Physic/2DImageCollisionData.h"
//#include <wininet.h>
//#pragma comment(lib,"Wininet.lib")
//bool bConnect=InternetCheckConnection(L"http://www.google.com",
//              FLAG_ICC_FORCE_CONNECTION ,0);

//bool IsWindows7()
//{
//    OSVERSIONINFOEX osvi;
//    bool bOsVersionInfoEx=false;
//    unsigned long dwType=0UL;
//    bool bWin7=false;
//    ZeroMemory(&osvi, sizeof(SYSTEM_INFO));
//    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
//    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
//    if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
//        return -1;
//    if(osvi.dwMajorVersion==6)
//    {
//        if(osvi.dwMinorVersion==1)
//        {
//            if(osvi.wProductType==VER_NT_WORKSTATION)
//            {
//                bWin7=true;
//            }
//        }
//    }
//    return bWin7;
//}


//		if( m_spPhaseManager )
//			m_spPhaseManager->MouseUp(m_sMousePosition.x,m_sMousePosition.y);
//		if( m_spPhaseManager )
//			m_spPhaseManager->MouseMove(m_sMousePosition.x,m_sMousePosition.y);
//		if( m_spPhaseManager )
//			m_spPhaseManager->MouseDown(m_sMousePosition.x,m_sMousePosition.y);
//		m_spPhaseManager->Render();
//		m_spPhaseManager->Update(e_fElpaseTime);
//		SAFE_DELETE(m_spPhaseManager);
//		if( !m_spPhaseManager )
//			m_spPhaseManager = new cPhaseManager();
//		cPhaseManager*						cGameApp::m_spPhaseManager = 0;
//		m_spPhaseManager->KeyUp(e_char);
//		m_spPhaseManager->KeyPress(e_char);
//		m_spPhaseManager->KeyDown(e_char);

#if defined(ANDROID)//openAL,android.c
#include <android/log.h>
#include "../Android/nv_egl_util.h"
#include "../Android/JNIUtil.h"
JavaVM* javaVM = 0;
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG,"GameApp",__VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"GameApp",__VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,"GameApp",__VA_ARGS__))
#endif
namespace	FATMING_CORE
{
	void	DumpGraphicsInfo()
	{
		GLint	l_iValue = 0;
		std::wstring	l_str;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&l_iValue);					l_str = L"GL_MAX_TEXTURE_SIZE:";					l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&l_iValue);	l_str = L"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:";	l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&l_iValue);			l_str = L"GL_MAX_TEXTURE_IMAGE_UNITS:";			l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
#ifndef WIN32
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS,&l_iValue);		l_str = L"GL_MAX_FRAGMENT_UNIFORM_VECTORS:";		l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
		glGetIntegerv(GL_MAX_VARYING_VECTORS,&l_iValue);				l_str = L"GL_MAX_VARYING_VECTORS:";				l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS,&l_iValue);			l_str = L"GL_MAX_VERTEX_UNIFORM_VECTORS:";		l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
#endif
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&l_iValue);					l_str = L"GL_MAX_VERTEX_ATTRIBS:";				l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&l_iValue);		l_str = L"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:";	l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
		//glGetIntegerv(GL_MAX_VIEWPORT_DIMS,&l_iValue);					l_str = L"GL_MAX_VIEWPORT_DIMS:";				l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
	}
	POINT	ConvertCoordinate(int e_iPosX,int e_iPosY,POINT e_ViewPort)
	{
		Vector2		l_vScale = cGameApp::GetViewPortAndGameResolutionScale();
		POINT	l_Pos = {e_iPosX,e_iPosY};
		switch(cGameApp::m_seDeviceDirection)
		{
			case eDD_PORTRAIT://do nothing
				l_Pos.x = e_iPosX;
				l_Pos.y = e_iPosY;
			break;
			case eDD_UPSIDE_DOWN:
				l_Pos.x = e_ViewPort.x-e_iPosX;
				l_Pos.y = e_ViewPort.y-e_iPosY;
			break;
				//windows and iOS is not same?
			case eDD_LANDSCAPE_LEFT://ensure view port is right,x and y swap
				l_Pos.x = e_ViewPort.y-e_iPosY;
				l_Pos.y = e_iPosX;
			break;//ensure view port is right,x and y swap
			case eDD_LANDSCAPE_RIGHT:

				l_Pos.x = e_iPosY;
				l_Pos.y = e_ViewPort.x-e_iPosX;
			break;
			default:
				assert(0&&"ConvertCoordinate fuck");
				break;
		}
		l_Pos.x = (int)(l_Pos.x-cGameApp::m_svViewPortSize.x);
		l_Pos.y = (int)(l_Pos.y-cGameApp::m_svViewPortSize.y);
		//cGameApp::m_svViewPortSize.y;
		l_Pos.x = (int)(l_Pos.x*l_vScale.x);
		l_Pos.y = (int)(l_Pos.y*l_vScale.y);
		return l_Pos;
	}

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
	float												cGameApp::m_sfDebugValue = 1.f;
	float												cGameApp::m_sfVersion = 1.f;
	bool												cGameApp::m_sbLeave = false;
	bool												cGameApp::m_sbSpeedControl = false;
	UT::sTimeAndFPS										cGameApp::m_sTimeAndFPS;
	cPaticleManager*									cGameApp::m_spPaticleManager = 0;
	cBehaviorPaticleManager*							cGameApp::m_spBehaviorPaticleManager = 0;
	cGlyphFontRender*									cGameApp::m_spGlyphFontRender = 0;
	cNamedTypedObjectVector<cGlyphFontRender>*			cGameApp::m_spGlyphFontRenderVector = 0;
	cNamedTypedObjectVector<cExternalFunction>*			cGameApp::m_spExternalFunctionVector = 0;
	cNamedTypedObjectVector<c2DImageCollisionData>*		cGameApp::m_sp2DImageCollisionDataVector = 0;
	cNamedTypedObjectVector<NamedTypedObject>*			cGameApp::m_spColladaParserVector = 0;
	Vector4												cGameApp::m_svViewPortSize(0.f,0.f,1920.f,1080.f);
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
	bool												cGameApp::m_sbShowErrorMsgBox = true;
	std::wstring*										cGameApp::m_spstrErrorMsgString = 0;
	bool												cGameApp::m_sucKeyData[MAX_PATH];
	bool												cGameApp::m_sbEnableMouseSingnal = true;
	bool												cGameApp::m_sbEnableKeyboardSingnal = true;
#ifdef DEBUG
	bool												cGameApp::m_sbDebugFunctionWorking = true;
	bool												cGameApp::m_sbAllowParseBinaryFile = true;
#else
	bool												cGameApp::m_sbDebugFunctionWorking = false;
	bool												cGameApp::m_sbAllowParseBinaryFile = true;
#endif
	std::string*										cGameApp::m_psstrGameAppName = 0;
	cBinaryFile*										cGameApp::m_spLogFile = 0;
	cFUThreadPool*										cGameApp::m_spThreadPool = 0;
	//cClickMouseBehavior*								cGameApp::m_spClickMouseBehavior = 0;
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
	bool		GetExternalSDPath(std::string*e_pTarget)
	{
		jclass thisClass = cGameApp::m_spThreadEnv->GetObjectClass(*cGameApp::m_spAppThreadThis);
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		//DoPurchaseProducts(int e_iProductsType,String e_strProductsID)
		jmethodID l_GetExternalSDCardPath = cGameApp::m_spThreadEnv->GetMethodID(thisClass,"GetExternalSDCardPath", "()Ljava/lang/String;");
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		jstring l_Path = (jstring)cGameApp::m_spThreadEnv->CallObjectMethod(*cGameApp::m_spAppThreadThis,l_GetExternalSDCardPath);
		const char *js = cGameApp::m_spThreadEnv->GetStringUTFChars(l_Path, nullptr);
		if( js )
		{
			*e_pTarget = js;
			*e_pTarget += "/";
			cGameApp::m_spThreadEnv->ReleaseStringUTFChars(l_Path, js);
			return true;
		}
		return false;
	}

	void	SetupAPKFilePath(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv)
	{
		if( !cGameApp::m_spAPKPath )
		{
			javaVM = e_pActivity->vm;

			jclass clazz = e_pThreadEnv->GetObjectClass(e_pActivity->clazz);
			jmethodID methodID = e_pThreadEnv->GetMethodID(clazz, "getPackageCodePath", "()Ljava/lang/String;");
			jobject result = e_pThreadEnv->CallObjectMethod( e_pActivity->clazz, methodID);
			const char* str;
			jboolean isCopy;
			str = e_pThreadEnv->GetStringUTFChars((jstring)result, &isCopy);
			cGameApp::m_spAPKPath = new std::string;
			*cGameApp::m_spAPKPath = str;
			cGameApp::OutputDebugInfoString(str);
			cGameApp::OutputDebugInfoString(e_pActivity->internalDataPath);
			cGameApp::m_spInternalDirectory = new std::string;
			*cGameApp::m_spInternalDirectory = e_pActivity->internalDataPath;
			if( e_pActivity->externalDataPath )
			{
				cGameApp::OutputDebugInfoString(e_pActivity->externalDataPath);
				cGameApp::m_spExternalDirectory = new std::string;
				*cGameApp::m_spExternalDirectory = e_pActivity->externalDataPath;
			}
			NvFInit(e_pActivity->assetManager);
			std::string l_Target;
			if(GetExternalSDPath(&l_Target))
			{
				cGameApp::m_spExternalSDDirectory = new std::string();
				*cGameApp::m_spExternalSDDirectory = l_Target;
				cGameApp::OutputDebugInfoString(*cGameApp::m_spExternalSDDirectory);
			}
		}
	}


	cGameApp::cGameApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject* e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil)
#elif defined( WIN32)
	cGameApp::cGameApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize)
#else
	cGameApp::cGameApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize)
#endif
	{
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
		ilInit();
		if( e_Hwnd )
		{
			OpenglInit(e_Hwnd);
			//m_sHdc = GetDC(e_Hwnd);
			m_sHandle = e_Hwnd;
			DumpGraphicsInfo();
		}
#endif
		memset(m_sucKeyData,0,sizeof(bool)*MAX_PATH);
        m_bMouseHover = false;

		m_svGameResolution = e_vGameResolution;
		m_svViewPortSize.x = 0.f;
		m_svViewPortSize.y = 0.f;
		m_svViewPortSize.z = (float)e_vViewportSize.x;
		m_svViewPortSize.w = (float)e_vViewportSize.y;
		m_spMultiTouchPoints = new sMultiTouchPoints();
		//if(!m_spClickMouseBehavior)
		//	m_spClickMouseBehavior = new cClickMouseBehavior();
        if( !m_spAnimationParser )
        {
			//in android some how it will close force sunndenly by I donno who.
			m_spThreadPool = new cFUThreadPool;
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
		SAFE_DELETE(m_spThreadPool);
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
		CreateShader(g_bMySkinningMeshVSClientState,g_strMySkinningMeshVS,g_strMySkinningMeshFS,SKINNING_MESH_SHADER);
		if( m_spLogFile )
			m_spLogFile->WriteToFileImmediatelyWithLine("init shader ok");
		m_sTimeAndFPS.Update();
		if( m_spLogFile )
			m_spLogFile->WriteToFileImmediatelyWithLine("parse font data");
		if( !m_spGlyphFontRender )
		{
			m_spGlyphFontRender = new cGlyphFontRender("Font");
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
		//m_spClickMouseBehavior->SetCollisionRange(Vector4(0,0,this->m_svGameResolution.x,this->m_svGameResolution.y));
		m_sTimeAndFPS.Update();
		srand(m_sTimeAndFPS.uiCurrentTime);
		SystemErrorCheck();
	}

	void	cGameApp::Destroy()
	{
		if( m_spLogFile )
			m_spLogFile->WriteToFileImmediatelyWithLine("Destroy start");
		SystemErrorCheck();
		//SAFE_DELETE(m_spClickMouseBehavior);
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
		ilShutDown();
#endif
		SystemErrorCheck();
		if( m_spLogFile )
			m_spLogFile->WriteToFileImmediatelyWithLine("all Destroy done");
	}

	void	cGameApp::Update(float e_fElpaseTime)
	{
		m_dbGamePlayTime += e_fElpaseTime;
		SystemErrorCheck();
	}
	extern cBaseShader*g_pCurrentShader;
	void	cGameApp::Render()
	{
		g_pCurrentShader = 0;
#ifndef OPENGLES_2_X
		glEnableClientState(GL_VERTEX_ARRAY);
#endif
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
		//m_spClickMouseBehavior->MouseDown(e_iPosX,e_iPosY);
	}

	void	cGameApp::MouseMove(int e_iPosX,int e_iPosY)
	{
		m_sScreenMousePosition.x = e_iPosX;
		m_sScreenMousePosition.y = e_iPosY;
		//if( !m_sbEnableMouseSingnal )
		//	return;
		POINT	l_Viewport = {(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height()};
        m_sMousePosition = ConvertCoordinate(e_iPosX,e_iPosY,l_Viewport);
		//m_spClickMouseBehavior->MouseMove(e_iPosX,e_iPosY);
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
		//m_spClickMouseBehavior->MouseMove(e_iPosX,e_iPosY);
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
		//if(!m_sbEnableKeyboardSingnal)
		//	return;
		m_sucKeyData[(unsigned char)e_char] = true;
	}

	void	cGameApp::KeyPress(char e_char)
	{
		//if(!m_sbEnableKeyboardSingnal)
		//	return;
		m_sucKeyData[(unsigned char)e_char] = true;
	}

	void	cGameApp::KeyUp(char e_char)
	{
		//SHORT	l_ = GetAsyncKeyState('A');
		//cGameApp::KeyUp(e_char);
		//std::string	l_sss = UT::ComposeMsgByFormat("KeyValue:%s",&e_char);
		//std::wstring l_ss2 = UT::CharToWchar(l_sss.c_str());
		//cGameApp::OutputDebugInfoString(l_ss2.c_str());
		//cGameApp::OutputDebugInfoString(L"\n");

		//if(!m_sbEnableKeyboardSingnal)
		//	return;
#ifdef WIN32
	    if( e_char == 107 )//107'+'
		{
			m_sfDebugValue *= 2.f;
			if( m_sfDebugValue >1073741824 )
				m_sfDebugValue  = 1073741824;
		}
		else
		if( e_char == 109 )//109'-'
		{
			m_sfDebugValue /= 2.f;	
			if( m_sfDebugValue <= 0.00000000001f )
				m_sfDebugValue = 0.00000000001f;
		}
		if(m_sucKeyData['P'])
			m_sbGamePause = !m_sbGamePause;
		if( cGameApp::m_sucKeyData[17] )//alt
		{
			switch(e_char)
			{
				case 38://up
					//cGameApp::m_svGameResolution.x = ;
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
			}
		}
#endif
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

	void	cGameApp::SetAcceptRation(int e_iWidthRation,int e_iHeightRation,int e_iGap)
	{
		float	l_fWidthAceptRation = cGameApp::m_svViewPortSize.Width()/cGameApp::m_svViewPortSize.Height();
		float	l_fTargetAceptRation = e_iWidthRation/(float)e_iHeightRation;
		
		if(fabs(l_fWidthAceptRation-l_fTargetAceptRation)>0.02)
		{
			Vector4	l_vOriginalSize = cGameApp::m_svViewPortSize;
			int		l_iWidth = (int)cGameApp::m_svViewPortSize.Width();
			int		l_iHeight = (int)cGameApp::m_svViewPortSize.Height();
			float	l_fNewW =  cGameApp::m_svViewPortSize.Width()/e_iGap;
			float	l_fNewH = cGameApp::m_svViewPortSize.Height()/e_iGap;
			if( l_fNewW > l_fNewH )
			{
				int	l_iGap = l_iHeight/9;
				l_iGap = (l_iWidth-(l_iGap*16))/2;
				cGameApp::m_svViewPortSize.y = (float)l_iGap;
				cGameApp::m_svViewPortSize.w = l_vOriginalSize.w-l_iGap;
			}
			else
			{
				int	l_iGap = l_iHeight/16;
				l_iGap = (l_iWidth-(l_iGap*9))/2;
				cGameApp::m_svViewPortSize.x = (float)l_iGap;
				cGameApp::m_svViewPortSize.z = l_vOriginalSize.z-l_iGap;
			}
		}		
	}

	//<root FullScreen="0" Resolution="960,640" ViewPort="960,640" DeviceOrietation="0" />
	void	cGameApp::ResoluctionParse(char*e_strFileName)
	{
		cNodeISAX	l_NodeISAX;
		bool	l_bFullScreen = false;
		bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(e_strFileName);
		if(l_b)
		{
			TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
			const wchar_t*l_strFullscreen = l_pTiXmlElement->Attribute(L"FullScreen");
			if( l_strFullscreen )
				m_sbFullScreen = _wtoi(l_strFullscreen)?true:false;
			const wchar_t*l_strResolution = l_pTiXmlElement->Attribute(L"Resolution");
			const wchar_t*l_strViewPort = l_pTiXmlElement->Attribute(L"ViewPort");
			const wchar_t*l_strDeviceOrietation = l_pTiXmlElement->Attribute(L"DeviceOrietation");
			const wchar_t*l_strHideMouseCursor = l_pTiXmlElement->Attribute(L"HideMouseCursor");
			const wchar_t*l_strTexturePowerOfTwo = l_pTiXmlElement->Attribute(L"TexturePowerOfTwo");
			const wchar_t*l_strDevice = l_pTiXmlElement->Attribute(L"Device");
			const wchar_t*l_strDebugFunctionWorking = l_pTiXmlElement->Attribute(L"DebugFunction");
			if( l_strDebugFunctionWorking )
			{
				m_sbDebugFunctionWorking = _wtoi(l_strDebugFunctionWorking)?true:false;
			}
			if( l_strDevice )
				m_sbDeviceExist = _wtoi(l_strDevice)?true:false;
			if( l_strResolution )
			{
				Vector2	l_vResolution = GetVector2(l_strResolution);
				if( !l_strViewPort )
				{
					cGameApp::m_svViewPortSize.x = 0;
					cGameApp::m_svViewPortSize.y = 0;
					cGameApp::m_svViewPortSize.z = l_vResolution.x;
					cGameApp::m_svViewPortSize.w = l_vResolution.y;
				}
				cGameApp::m_svGameResolution.x = l_vResolution.x;
				cGameApp::m_svGameResolution.y = l_vResolution.y;
				//if(!m_spClickMouseBehavior)
				//	m_spClickMouseBehavior = new cClickMouseBehavior();
				//m_spClickMouseBehavior->SetCollisionRange(Vector4(0,0,m_svGameResolution.x,m_svGameResolution.y));
			}
			if( l_strViewPort )
			{
				Vector2	l_vResolution = GetVector2(l_strViewPort);
				cGameApp::m_svViewPortSize.x = 0;
				cGameApp::m_svViewPortSize.y = 0;
				cGameApp::m_svViewPortSize.z = l_vResolution.x;
				cGameApp::m_svViewPortSize.w = l_vResolution.y;
			}
			if( l_strDeviceOrietation )
			{
				int	l_iIndex = _wtoi(l_strDeviceOrietation);
				if( l_iIndex <= eDD_LANDSCAPE_RIGHT )
					cGameApp::m_seDeviceDirection = (eDeviceDirection)l_iIndex;
			}
			if( l_strHideMouseCursor )
			{
#ifdef WIN32
				ShowCursor(_wtoi(l_strHideMouseCursor)==0?false:true);
#endif
			}
			if( l_strTexturePowerOfTwo )
			{
				//g_bSupportNonPowerOfTwoTexture = _wtoi(l_strTexturePowerOfTwo)?true:false;
			}
		}
	}
	cPuzzleImage*	cGameApp::GetPuzzleImage(const wchar_t*e_strName){ return dynamic_cast<cPuzzleImage*>(m_spImageParser->GetObject(e_strName)); }
	cBaseImage*		cGameApp::GetBaseImage(const wchar_t*e_strName){  return dynamic_cast<cBaseImage*>(m_spImageParser->GetObject(e_strName));  }
	cPuzzleImage*	cGameApp::GetPuzzleImageByFileName(std::wstring e_strFileName){ return GetPuzzleImageByFileName(e_strFileName.c_str()); }
	cPuzzleImage*	cGameApp::GetPuzzleImageByFileName(const wchar_t*e_strFileName){ return m_spImageParser->GetPuzzleImageByFileName(e_strFileName); }
	//<cNumeralImage PIFileName="" NumeralImageName=""/>
	//<cNumeralImage PIFileName="" StartPIUnitName="" EndPIUnitName=""/>
	cNumeralImage*	cGameApp::GetNumeralImageByXmlElement(TiXmlElement*e_pTiXmlElement)
	{
		if( e_pTiXmlElement )
		{
			const wchar_t*l_strTempString = 0;
			cPuzzleImage*l_pPI = 0;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("PIFileName")
				{
					l_pPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
				}
				else
				COMPARE_NAME("NumeralImageName")
				{
					return l_pPI->GetNumeralImageByName(l_strValue);
				}
				else
				COMPARE_NAME("StartPIUnitName")
				{
					l_strTempString = l_strValue;
				}
				else
				COMPARE_NAME("EndPIUnitName")
				{
					if( l_pPI )
						return l_pPI->GetNumeralImageByName(l_strTempString,l_strValue);
				}
			PARSE_NAME_VALUE_END
			//cNumeralImage*l_pNumeralImage;
		}
		return 0;
	}

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

	void		cGameApp::OutputDebugInfoString(const wchar_t*e_str)
	{
#ifdef DEBUG		
	#if defined(WIN32)
			::OutputDebugString(e_str);
	#elif	defined(ANDROID)
			std::string	l_str = "GameApp___ ";
			l_str += UT::WcharToChar(e_str);
			LOGD("%s",l_str.c_str());
	#elif	defined(IOS)
			std::string	l_str = UT::WcharToChar(e_str);
			printf(l_str.c_str());
	#endif
#endif
	}

	void		cGameApp::OutputDebugInfoString(std::string e_str)
	{
		cGameApp::OutputDebugInfoString(UT::CharToWchar(e_str));
	}

	void		cGameApp::OutputDebugInfoString(const char*e_str)
	{
		cGameApp::OutputDebugInfoString(UT::CharToWchar(e_str));
	}

	void		cGameApp::OutputDebugInfoString(std::wstring e_str)
	{
		cGameApp::OutputDebugInfoString(e_str.c_str());
	}

	void		cGameApp::ApplyViewPort()
	{
		glViewport((int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y,(int)cGameApp::m_svViewPortSize.z,(int)cGameApp::m_svViewPortSize.w);
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
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
#ifndef OPENGLES_2_X
		glEnable(GL_TEXTURE_2D);
#endif
		if( cGameApp::m_spGlyphFontRender )
		{
			int	l_iStaryPosY = 0;
			wchar_t   l_str[MAX_PATH];
			swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
			cGameApp::m_spGlyphFontRender->RenderFont(0,l_iStaryPosY,l_str);
			l_iStaryPosY += 20;
			cGameApp::m_spGlyphFontRender->RenderFont(0,l_iStaryPosY,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()).c_str());
			swprintf(l_str,MAX_PATH,L"RealMousePos:%d,%d",m_sScreenMousePosition.x,m_sScreenMousePosition.y);
			l_iStaryPosY += 20;
			cGameApp::m_spGlyphFontRender->RenderFont(0,l_iStaryPosY,l_str);
		}	
	}
	void	cGameApp::OpenKeyBoard(int e_iKeyBoardType,const wchar_t*e_strTitle,const wchar_t*e_strMessage)
	{
#ifdef ANDROID
		if( e_iKeyBoardType == 0 )
		{
			jclass thisClass = m_spThreadEnv->GetObjectClass(*m_spAppThreadThis);
			jmethodID launchURL = m_spThreadEnv->GetMethodID(thisClass, "OpenKeyBoard", "()V");
			m_spThreadEnv->CallStaticVoidMethod(thisClass,launchURL);
		}
		else
		if( e_iKeyBoardType == 1 )
		{
			//bool	RegisterNativeFunction("util/NumericEditTextDialog","FinishInput","(Ljava/lang/String;)V",void*FunctionPointer);
			//bool	RegisterNativeFunction("util/NumericEditTextDialog","Chancel","()V",void*FunctionPointer);
			jclass l_Class = GetCustomJavaClass("util/NumericEditTextDialog");
			jstring	l_strTitle = WcharToJava(cGameApp::m_spThreadEnv,e_strTitle);
			jstring	l_strMessage = WcharToJava(cGameApp::m_spThreadEnv,e_strMessage);
			jmethodID l_ShowDialog = cGameApp::m_spJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class, "ShowDialog", "(Ljava/lang/String;Ljava/lang/String;)V");
			cGameApp::m_spJNIUtilData->pJNIEnv->CallStaticVoidMethod(l_Class,l_ShowDialog,l_strTitle,l_strMessage);
			cGameApp::m_spJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);
		}
#endif
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
		static	int	l_sMemoryUsed = 0;
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
			int	l_iMemoryChangeSize = pmc.WorkingSetSize-l_sMemoryUsed;
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