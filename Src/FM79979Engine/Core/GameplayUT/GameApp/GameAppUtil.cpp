#include "GameApp.h"

#ifdef WIN32
#include <direct.h>
#endif
#include "../../Sound/SoundManager.h"
#include "../Render/Particle/AllParticleInclude.h"
//#include "ImageButton.h"
#include "../Physic/2DImageCollisionData.h"
#if defined(ANDROID)//openAL,android.c
#include <android/log.h>
#include "../../Android/nv_egl_util.h"
#include "../../Android/JNIUtil.h"
#endif

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
#if defined(WIN32) && !defined(UWP)
void ToggleFullScreen(bool e_bFullScreen, HWND e_hwnd)
{
	static DWORD savedExStyle;
	static DWORD savedStyle;
	static RECT rcSaved;

	if (e_bFullScreen)
	{
		// Moving to full screen mode.
		savedExStyle = GetWindowLong(e_hwnd, GWL_EXSTYLE);
		savedStyle = GetWindowLong(e_hwnd, GWL_STYLE);
		GetWindowRect(e_hwnd, &rcSaved);
		int l_iSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
		int l_iSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
		SetWindowLong(e_hwnd, GWL_EXSTYLE, 0);
		SetWindowLong(e_hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		SetWindowPos(e_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		SetWindowPos(e_hwnd, HWND_TOPMOST, 0, 0, l_iSM_CXSCREEN, l_iSM_CYSCREEN, SWP_SHOWWINDOW);
		cGameApp::m_spOpenGLRender->m_vViewPortSize.x = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = 0;
		cGameApp::m_spOpenGLRender->m_vViewPortSize.y = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = 0;
		cGameApp::m_spOpenGLRender->m_vViewPortSize.z = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = (float)l_iSM_CXSCREEN;
		cGameApp::m_spOpenGLRender->m_vViewPortSize.w = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = (float)l_iSM_CYSCREEN;
	}
	else
	{
		// Moving back to windowed mode.
		SetWindowLong(e_hwnd, GWL_EXSTYLE, savedExStyle);
		SetWindowLong(e_hwnd, GWL_STYLE, savedStyle);
		SetWindowPos(e_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		SetWindowPos(e_hwnd, HWND_NOTOPMOST, rcSaved.left, rcSaved.top, rcSaved.right - rcSaved.left, rcSaved.bottom - rcSaved.top, SWP_SHOWWINDOW);
		cGameApp::m_spOpenGLRender->m_vViewPortSize.x = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = 0;
		cGameApp::m_spOpenGLRender->m_vViewPortSize.y = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = 0;
		cGameApp::m_spOpenGLRender->m_vViewPortSize.z = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = (float)(rcSaved.right - rcSaved.left);
		cGameApp::m_spOpenGLRender->m_vViewPortSize.w = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = (float)(rcSaved.bottom - rcSaved.top);
	}
	cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution((int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height(), (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
}
#endif
namespace	FATMING_CORE
{
	//e_iKeyBoardType = 0 as default,e_iKeyBoardType = 1 as Numeric pad,
	//in android if e_iKeyBoardType is one u have to setup
	//FinishInput(String e_strValue);,Chancel();take a look at cpp
	//take a see at cpp
	//RegisterNativeFunction("util/NumericEditTextDialog","FinishInput","(Ljava/lang/String;)V",(void*)NetWorkConnectPhaseFinishInput);
	//RegisterNativeFunction("util/NumericEditTextDialog","Chancel","()V",(void*)NetWorkConnectPhaseKeyBoardChancel);
	//void	NetWorkConnectPhaseKeyBoardChancel(){	g_bKeyBoardChancel = true;}
	//void NetWorkConnectPhaseFinishInput(JNIEnv* env,jobject thiz,  jstring e_InputValue)
	//{
	//	const char *l_str = env->GetStringUTFChars(e_InputValue, 0);
	//    env->ReleaseStringUTFChars(e_InputValue,l_str);
	//}

	void cGameApp::CreateDefaultOpenGLRender()
	{
		if (!m_spOpenGLRender)
		{
			m_spOpenGLRender = new cOpenGLRender(Vector2(1920, 1080), Vector2(1920, 1080));
		}
	}

	//<root FullScreen="0" Resolution="960,640" ViewPort="960,640" DeviceOrietation="0" />
	void	cGameApp::ResoluctionParse(const char*e_strFileName)
	{
		CreateDefaultOpenGLRender();
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
			const wchar_t*l_strDebugFunctionWorking = l_pTiXmlElement->Attribute(L"DebugFunction");
			const wchar_t*l_strLockFPS = l_pTiXmlElement->Attribute(L"LockFPS");
			if (l_strLockFPS)
			{
				cGameApp::m_sbDoLockFPS = GetBoolean(l_strLockFPS);
			}
			if( l_strDebugFunctionWorking )
			{
				m_sbDebugFunctionWorking = _wtoi(l_strDebugFunctionWorking)?true:false;
			}
			if( l_strResolution )
			{
				Vector2	l_vResolution = GetVector2(l_strResolution);
				if( !l_strViewPort )
				{
					cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 0;
					cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 0;
					cGameApp::m_spOpenGLRender->m_vViewPortSize.z = l_vResolution.x;
					cGameApp::m_spOpenGLRender->m_vViewPortSize.w = l_vResolution.y;
				}
				cGameApp::m_spOpenGLRender->m_vGameResolution.x = l_vResolution.x;
				cGameApp::m_spOpenGLRender->m_vGameResolution.y = l_vResolution.y;
				//if(!m_spClickMouseBehavior)
				//	m_spClickMouseBehavior = new cClickMouseBehavior();
				//m_spClickMouseBehavior->SetCollisionRange(Vector4(0,0,m_spOpenGLRender->m_vGameResolution.x,m_spOpenGLRender->m_vGameResolution.y));
			}
			if( l_strViewPort )
			{
				Vector2	l_vResolution = GetVector2(l_strViewPort);
				cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 0;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 0;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.z = l_vResolution.x;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.w = l_vResolution.y;
			}
			if( l_strDeviceOrietation )
			{
				int	l_iIndex = _wtoi(l_strDeviceOrietation);
				if (l_iIndex <= eDD_LANDSCAPE_RIGHT)
				{
					cGameApp::m_spOpenGLRender->m_eDeviceDirection = (eDeviceDirection)l_iIndex;
				}
			}
			if( l_strHideMouseCursor )
			{
#if defined(WIN32) && !defined(UWP)
				ShowCursor(_wtoi(l_strHideMouseCursor)==0?false:true);
#endif
			}
			if( l_strTexturePowerOfTwo )
			{
				//MyGLGetIntegerv(GL_ARB_texture_non_power_of_two, &l_iValue);
				//g_bSupportNonPowerOfTwoTexture = l_iValue ? true : false;
				//auto detect while platfore is PC,but you still can test it
				g_bSupportNonPowerOfTwoTexture = _wtoi(l_strTexturePowerOfTwo)?true:false;
			}
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
#ifdef ANDROID
	bool		GetExternalSDPath(std::string*e_pTarget)
	{
		jclass thisClass = cGameApp::m_spThreadEnv->GetObjectClass(*cGameApp::m_spAppThreadThis);
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		//DoPurchaseProducts(int e_iProductsType,String e_strProductsID)
		jmethodID l_GetExternalSDCardPath = cGameApp::m_spThreadEnv->GetMethodID(thisClass,"GetExternalSDCardPath", "()Ljava/lang/String;");
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		jstring l_Path = (jstring)cGameApp::m_spThreadEnv->CallObjectMethod(*cGameApp::m_spAppThreadThis,l_GetExternalSDCardPath);
		if( l_Path )
		{
			const char *js = cGameApp::m_spThreadEnv->GetStringUTFChars(l_Path, nullptr);
			if( js )
			{
				*e_pTarget = js;
				*e_pTarget += "/";
				cGameApp::m_spThreadEnv->ReleaseStringUTFChars(l_Path, js);
				return true;
			}
		}
		return false;
	}

	void	SetupAPKFilePath(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv)
	{
		if( !cGameApp::m_spAPKPath )
		{
			jclass clazz = e_pThreadEnv->GetObjectClass(e_pActivity->clazz);
			jmethodID methodID = e_pThreadEnv->GetMethodID(clazz, "getPackageCodePath", "()Ljava/lang/String;");
			jobject result = e_pThreadEnv->CallObjectMethod( e_pActivity->clazz, methodID);
			const char* str;
			jboolean isCopy;
			str = e_pThreadEnv->GetStringUTFChars((jstring)result, &isCopy);
			cGameApp::m_spAPKPath = new std::string;
			*cGameApp::m_spAPKPath = str;
			FMLog::LogWithFlag(str, CORE_LOG_FLAG);
			FMLog::LogWithFlag(e_pActivity->internalDataPath, CORE_LOG_FLAG);
			cGameApp::m_spInternalDirectory = new std::string;
			*cGameApp::m_spInternalDirectory = e_pActivity->internalDataPath;
			if( e_pActivity->externalDataPath )
			{
				FMLog::LogWithFlag(e_pActivity->externalDataPath, CORE_LOG_FLAG);
				cGameApp::m_spExternalDirectory = new std::string;
				*cGameApp::m_spExternalDirectory = e_pActivity->externalDataPath;
			}
			NvFInit(e_pActivity->assetManager);
			std::string l_Target;
			if(GetExternalSDPath(&l_Target))
			{
				cGameApp::m_spExternalSDDirectory = new std::string();
				*cGameApp::m_spExternalSDDirectory = l_Target;
				FMLog::LogWithFlag(*cGameApp::m_spExternalSDDirectory, CORE_LOG_FLAG);
			}
		}
	}
#endif
//
}