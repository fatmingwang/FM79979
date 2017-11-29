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
#if defined(ANDROID)//openAL,android.c
#include <android/log.h>
#include "../Android/nv_egl_util.h"
#include "../Android/JNIUtil.h"
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
		cGameApp::m_svViewPortSize.x = cGameApp::m_svDeviceViewPortSize.x = 0;
		cGameApp::m_svViewPortSize.y = cGameApp::m_svDeviceViewPortSize.x = 0;
		cGameApp::m_svViewPortSize.z = cGameApp::m_svDeviceViewPortSize.x = (float)l_iSM_CXSCREEN;
		cGameApp::m_svViewPortSize.w = cGameApp::m_svDeviceViewPortSize.x = (float)l_iSM_CYSCREEN;
	}
	else
	{
		// Moving back to windowed mode.
		SetWindowLong(e_hwnd, GWL_EXSTYLE, savedExStyle);
		SetWindowLong(e_hwnd, GWL_STYLE, savedStyle);
		SetWindowPos(e_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		SetWindowPos(e_hwnd, HWND_NOTOPMOST, rcSaved.left, rcSaved.top, rcSaved.right - rcSaved.left, rcSaved.bottom - rcSaved.top, SWP_SHOWWINDOW);
		cGameApp::m_svViewPortSize.x = cGameApp::m_svDeviceViewPortSize.x = 0;
		cGameApp::m_svViewPortSize.y = cGameApp::m_svDeviceViewPortSize.x = 0;
		cGameApp::m_svViewPortSize.z = cGameApp::m_svDeviceViewPortSize.x = (float)(rcSaved.right - rcSaved.left);
		cGameApp::m_svViewPortSize.w = cGameApp::m_svDeviceViewPortSize.x = (float)(rcSaved.bottom - rcSaved.top);
	}
	cGameApp::SetAcceptRationWithGameresolution((int)cGameApp::m_svViewPortSize.Width(), (int)cGameApp::m_svViewPortSize.Height(), (int)cGameApp::m_svGameResolution.x, (int)cGameApp::m_svGameResolution.y);
}
namespace	FATMING_CORE
{
	void	DumpGraphicsInfo()
	{
		std::wstring	l_str;
		const GLubyte*l_strGL_VERSION = glGetString(GL_VERSION);
		const GLubyte*l_strGL_SHADING_LANGUAGE_VERSION = glGetString(GL_SHADING_LANGUAGE_VERSION);
		//https://stackoverflow.com/questions/28242148/glews-glewissupportedgl-version-3-1-returning-false-on-mbp-osx-10-10-1-with
		//OSX
		//const GLubyte*l_strGL_GL_MINOR_VERSION = glGetString(GL_MINOR_VERSION);
		//const GLubyte*l_strGL_GL_MAJOR_VERSION = glGetString(GL_MAJOR_VERSION);
		//too much things...I am lazy to parse this
		//const GLubyte*l_strGL_GL_EXTENSIONS = glGetString(GL_EXTENSIONS);
		float l_fVersion = GetFloat((char*)l_strGL_VERSION);
		float l_f2 = GetFloat((char*)l_strGL_SHADING_LANGUAGE_VERSION);
		cGameApp::m_sfOpenGLVersion = l_fVersion;
		l_str = L"GL_VERSION:";					l_str += ValueToStringW((char*)l_strGL_VERSION);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
		l_str = L"GL_SHADING_LANGUAGE_VERSION:";					l_str += ValueToStringW((char*)l_strGL_SHADING_LANGUAGE_VERSION);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());

		GLint	l_iValue = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&l_iValue);					l_str = L"GL_MAX_TEXTURE_SIZE:";					l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&l_iValue);	l_str = L"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:";	l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&l_iValue);			l_str = L"GL_MAX_TEXTURE_IMAGE_UNITS:";			l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
//#ifndef WIN32
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS,&l_iValue);		l_str = L"GL_MAX_FRAGMENT_UNIFORM_VECTORS:";		l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
		glGetIntegerv(GL_MAX_VARYING_VECTORS,&l_iValue);				l_str = L"GL_MAX_VARYING_VECTORS:";				l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS,&l_iValue);			l_str = L"GL_MAX_VERTEX_UNIFORM_VECTORS:";		l_str += ValueToStringW(l_iValue);	l_str += L"\n";	cGameApp::OutputDebugInfoString(l_str.c_str());
//#endif
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
	void	cGameApp::SetAcceptRationWithGameresolution(int e_iDeviceViewportWidth,int e_iDeviceViewportHeight,int e_iTargetResolutionWidth,int e_iTargetResolutionHeight)
	{
		double l_DeviceScreenRation = (double)e_iDeviceViewportWidth/e_iDeviceViewportHeight;
		double l_GameResolutionRation = (double)e_iTargetResolutionWidth/e_iTargetResolutionHeight;
		if( l_DeviceScreenRation == l_GameResolutionRation )
			return;
        float l_fScaleX = e_iDeviceViewportWidth / (float)e_iTargetResolutionWidth;
        float l_fScaleY = e_iDeviceViewportHeight / (float)e_iTargetResolutionHeight;
		l_fScaleX = l_fScaleY = min(l_fScaleX, l_fScaleY);
        // calculate the rect of viewport
        float l_fViewPortW = (float)e_iTargetResolutionWidth * l_fScaleX;
        float l_fViewPortH = (float)e_iTargetResolutionHeight * l_fScaleY;

		m_svViewPortSize.x = (e_iDeviceViewportWidth - l_fViewPortW) / 2;
		m_svViewPortSize.y = (e_iDeviceViewportHeight - l_fViewPortH) / 2;
		m_svViewPortSize.z = m_svViewPortSize.x+l_fViewPortW;
		m_svViewPortSize.w = m_svViewPortSize.y+l_fViewPortH;
		glViewport((int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y,(int)cGameApp::m_svViewPortSize.Width(),(int)cGameApp::m_svViewPortSize.Height());
#ifdef WIN32
		m_svDeviceViewPortSize.x = 0;
		m_svDeviceViewPortSize.y = 0;
		m_svDeviceViewPortSize.z = (float)e_iDeviceViewportWidth;
		m_svDeviceViewPortSize.w = (float)e_iDeviceViewportHeight;
#endif
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
#endif
//
}