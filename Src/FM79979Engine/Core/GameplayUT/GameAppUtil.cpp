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
	void	cGameApp::SetAcceptRationWithGameresolution(int e_iDeviceViewportWidth,int e_iDeviceViewportHeight)
	{
		double l_DeviceScreenRation = (double)e_iDeviceViewportWidth/e_iDeviceViewportHeight;
		double l_GameResolutionRation = (double)m_svGameResolution.x/m_svGameResolution.y;
		if( l_DeviceScreenRation == l_GameResolutionRation )
			return;
        float l_fScaleX = e_iDeviceViewportWidth / m_svGameResolution.x;
        float l_fScaleY = e_iDeviceViewportHeight / m_svGameResolution.y;
		l_fScaleX = l_fScaleY = min(l_fScaleX, l_fScaleY);
        // calculate the rect of viewport
        float l_fViewPortW = m_svGameResolution.x * l_fScaleX;
        float l_fViewPortH = m_svGameResolution.y * l_fScaleY;

		m_svViewPortSize.x = (e_iDeviceViewportWidth - l_fViewPortW) / 2;
		m_svViewPortSize.y = (e_iDeviceViewportHeight - l_fViewPortH) / 2;
		m_svViewPortSize.z = l_fViewPortW;
		m_svViewPortSize.w = l_fViewPortH;
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