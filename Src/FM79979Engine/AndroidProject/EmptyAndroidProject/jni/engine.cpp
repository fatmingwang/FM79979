//----------------------------------------------------------------------------------
// File:        android/native_subclass/jni/engine.cpp
// SDK Version: v10.10 
// Email:       tegradev@nvidia.com
// Site:        http://developer.nvidia.com/
//
// Copyright (c) 2007-2012, NVIDIA CORPORATION.  All rights reserved.
//
// TO  THE MAXIMUM  EXTENT PERMITTED  BY APPLICABLE  LAW, THIS SOFTWARE  IS PROVIDED
// *AS IS*  AND NVIDIA AND  ITS SUPPLIERS DISCLAIM  ALL WARRANTIES,  EITHER  EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED  TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL  NVIDIA OR ITS SUPPLIERS
// BE  LIABLE  FOR  ANY  SPECIAL,  INCIDENTAL,  INDIRECT,  OR  CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION,  DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE  USE OF OR INABILITY  TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
//
//----------------------------------------------------------------------------------
#include "stdio.h"
#include "engine.h"

#include <EGL/egl.h>
#include <EGL/eglplatform.h>
#include <GLES2/gl2.h>

#include <android/log.h>
#include "../../../FishingFun/Win32Test/stdafx.h"
#include "../../../EmptyProject/EngineTest/GameApp.h"
#include "unistd.h"

//#include "gpg/gpg.h"

bool	g_bLostFocus = false;;
cGameApp*g_pAndroidTestApp = 0;
Engine*g_pEngine = 0;
extern float	g_fSensorValue[3];
extern float	g_fForce;
Engine::Engine(NvEGLUtil& egl, struct android_app* app) :
	mEgl(egl)
{
	//gpg::AndroidPlatformConfiguration platform_configuration;
	//platform_configuration.SetActivity(app->activity->clazz);
	//add gpg,z to lin input
	m_bAppRunning = true;
	g_pEngine = this;
    mApp = app;

	mGameplayMode = true;
	m_uiInitialized = false;

    app->userData = this;
	app->onAppCmd = &Engine::handleCmdThunk;
    app->onInputEvent = &Engine::handleInputThunk;

	mOrientation = nv_app_get_display_rotation(mApp);

}

Engine::~Engine()
{
	SAFE_DELETE(g_pAndroidTestApp);
}

bool Engine::initUI()
{
	if (m_uiInitialized)
		return true;
	setGameplayMode(true);
	cGameApp::m_spThreadEnv = this->mApp->appThreadEnv;
	cGameApp::m_spANativeActivity = this->mApp->activity;
	//cGameApp::m_spAppThreadThis = &mApp->appThreadThis;
	if( !g_pAndroidTestApp )
	{
		Vector2	l_vViewport(mEgl.getSurfaceWidth(),mEgl.getSurfaceHeight());
		int	l_iX = l_vViewport.x;
		int	l_iY = l_vViewport.y;
		g_pAndroidTestApp = new cEngineTestApp(mApp->activity,mApp->appThreadEnv,&mApp->appThreadThis,Vector2(720,1280),l_vViewport,&mEgl);
		g_pAndroidTestApp->Init();
		g_pAndroidTestApp->m_sbDoMultiTouch = true;
		cGameApp::m_seDeviceDirection = eDD_PORTRAIT;
	}
	m_uiInitialized = true;
	return true;
}

bool Engine::checkWindowResized()
{
	if (mEgl.checkWindowResized())
	{
		LOGI("Window size change %dx%d", mEgl.getWidth(), mEgl.getHeight()); 
		return true;
	}

	return false;
}

bool Engine::resizeIfNeeded()
{
	return false;
	if (!mResizePending)
		return false;

	int w = mEgl.getWidth();
	int h = mEgl.getHeight();
	int height = (w > h) ? (h / 16) : (w / 16);


	mResizePending = false;

	return true;
}



bool Engine::renderFrame(bool allocateIfNeeded)
{
    if (!mEgl.isReadyToRender(allocateIfNeeded))
        return false;
	if (!initUI())
	{
		LOGW("Could not initialize UI - assets may be missing!");
		ANativeActivity_finish(mApp->activity);
		return false;
	}
	//resizeIfNeeded();
	mOrientation = nv_app_get_display_rotation(mApp);
	if( g_pAndroidTestApp )
	{
		int	l_iOrientation = nv_app_get_display_rotation(mApp);
		// (0=ROTATION_0, 1=ROTATION_90, 2=ROTATION_180, 3=ROTATION_180
		switch( l_iOrientation )
		{
			case 0:
				cGameApp::m_seDeviceDirection = eDD_PORTRAIT;
				break;
			case 1:
				cGameApp::m_seDeviceDirection = eDD_LANDSCAPE_LEFT;
				break;
			case 2:
				cGameApp::m_seDeviceDirection = eDD_UPSIDE_DOWN;
				break;
			case 3:
				cGameApp::m_seDeviceDirection = eDD_LANDSCAPE_LEFT;
				break;
		}
		
		if( g_bLostFocus )
		{
			Sleep(10);
			return true;
		}
		g_pAndroidTestApp->m_svAccelerometer.x = g_fSensorValue[0];
		g_pAndroidTestApp->m_svAccelerometer.y = g_fSensorValue[1];
		g_pAndroidTestApp->m_svAccelerometer.z = g_fSensorValue[2];
		g_pAndroidTestApp->m_sfForce = g_fForce;
		g_pAndroidTestApp->Run();
	}

    mEgl.swap();

    return true;
}



int Engine::handleInput(AInputEvent* event)
{
	if( !event )
		return 0;
    //We only handle motion events (touchscreen) and key (button/key) events
	int32_t eventType = AInputEvent_getType(event);
	if (eventType == AINPUT_EVENT_TYPE_MOTION)
	{
		int32_t actionUnmasked = AMotionEvent_getAction(event);
		int32_t action = (actionUnmasked & AMOTION_EVENT_ACTION_MASK);	
		static int	l_siIgnoreTime = 0;
		bool	l_bTouch = false;
		if(AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN || AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_MULTIPLE )
		{
			l_bTouch = true;
		}
		if( cGameApp::m_sbGamePause )
			l_siIgnoreTime = 1;
		if( g_pAndroidTestApp && !cGameApp::m_sbGamePause && l_siIgnoreTime == 0 )
		{
			if(g_pAndroidTestApp->m_sbDoMultiTouch)
			{
				int numEvents = AMotionEvent_getPointerCount(event);
				for (int index=0; index<numEvents; index++)
				{
					int		id = AMotionEvent_getPointerId(event, index);
					float	l_fPosX = AMotionEvent_getX(event, index);
					float	l_fPosY = AMotionEvent_getY(event, index);
					float	l_fpressure = AMotionEvent_getPressure(event, index);
					float	l_fSize =AMotionEvent_getSize(event, index);
					g_pAndroidTestApp->m_spMultiTouchPoints->Touch(l_bTouch,(int)l_fPosX,(int)l_fPosY,id);
					g_pAndroidTestApp->TouchSignalProcess(l_fPosX,l_fPosY,l_bTouch);
				}
				g_pAndroidTestApp->m_spMultiTouchPoints->Update(0);
			}
			else
			{
				int32_t iX = AMotionEvent_getX(event, 0);
				int32_t iY = AMotionEvent_getY(event, 0);
				g_pAndroidTestApp->TouchSignalProcess(iX,iY,l_bTouch);
			}
		}
		if( l_siIgnoreTime > 0 )
		{
			if( l_bTouch == false )
				l_siIgnoreTime = 0;
		}

		//int32_t action = AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction((const AInputEvent*)event);
		// A tap on the screen takes us out of autopause into gameplay mode if
		// we were paused.  No other touch processing is done.
		if (action == AMOTION_EVENT_ACTION_DOWN)
		{
			setGameplayMode(true);
			g_pAndroidTestApp->m_spMultiTouchPoints->Init();
		}
		//,
		return 1;
	} 
	else
	if (eventType == AINPUT_EVENT_TYPE_KEY) 
	{
		int32_t code = AKeyEvent_getKeyCode((const AInputEvent*)event);
		int32_t action = AKeyEvent_getAction((const AInputEvent*)event);

		// if we are in gameplay mode, we eat the back button and move into
		// pause mode.  If we are already in pause mode, we allow the back
		// button to be handled by the OS, which means we'll be shut down
		if ((code == AKEYCODE_BACK) && mGameplayMode)
		{
			setGameplayMode(false);
			return 1;
		}

		std::string	l_strDebugInfo = "Code:";
		l_strDebugInfo += ValueToString(code);
		l_strDebugInfo += "___Action";
		l_strDebugInfo += ValueToString(action);
		cGameApp::OutputDebugInfoString(l_strDebugInfo);

		bool	l_bVaildKey = false;
		bool	l_bKeyDown = action==0?true:false;
		if((code >= AKEYCODE_A && code <= AKEYCODE_Z))
		{//windows 65-90
			code -= AKEYCODE_A;
			code += 65;
			l_bVaildKey = true;
		}
		else
		if((code >= AKEYCODE_0 && code <= AKEYCODE_9))
		{//windows 45-97
			code -= AKEYCODE_0;
			code += 45;
			l_bVaildKey = true;
		}
		if( l_bVaildKey )
		{
			if( l_bKeyDown )
			{
				g_pAndroidTestApp->KeyDown((char)code);
			}
			else
			{
				g_pAndroidTestApp->KeyUp((char)code);
			}
		}
	}

    return 0;
}
//extern int	g_iHeartTest;
void Engine::handleCommand(int cmd)
{
    switch (cmd)
    {
		//save player data.
		 case APP_CMD_SAVE_STATE:
 // The system has asked us to save our current state.  Do so.
			 {
				//this->mApp->savedState = malloc(sizeof(int));
				//int*l_pNumHeart = (int*)this->mApp->savedState;
				//*l_pNumHeart = g_iHeartTest;
				//mApp->stateSaved = sizeof(int);
			 }
			 break;
		// The window is being shown, get it ready.
		// Note that on ICS, the EGL size will often be correct for the new size here
		// But on HC it will not be.  We need to defer checking the new res until the
		// first render with the new surface!
        case APP_CMD_INIT_WINDOW:
        case APP_CMD_WINDOW_RESIZED:
			mEgl.setWindow(mApp->window);
        	break;

        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
			mEgl.setWindow(NULL);
			break;

        case APP_CMD_GAINED_FOCUS:
			g_bLostFocus = false;
			mEgl.setWindow(mApp->window);
			if( cGameApp::m_spSoundParser )
				cGameApp::m_spSoundParser->Pause(false);
			break;
		case APP_CMD_STOP:
        case APP_CMD_LOST_FOCUS:
			if(cGameApp::m_spSoundParser)
			{
				//cGameApp::m_spSoundParser->Stop();
				cGameApp::m_spSoundParser->Pause(true);
			}
			g_bLostFocus = true;
			break;
		case APP_CMD_PAUSE:
        	// Move out of gameplay mode if we are in it.  But if we are
			// in another dialog mode, leave it as-is
            if (mGameplayMode)
				setGameplayMode(false);
            break;
		case APP_CMD_START:
			if (mGameplayMode)
				setGameplayMode(true);
			break;
		// ICS does not appear to require this, but on GB phones,
		// not having this causes rotation changes to be delayed or
		// ignored when we're in a non-rendering mode like autopause.
		// The forced renders appear to allow GB to process the rotation
		case APP_CMD_CONFIG_CHANGED:
			renderFrame(true);
			break;
    }
}

/**
 * Process the next input event.
 */
int32_t Engine::handleInputThunk(struct android_app* app, AInputEvent* event)
{
    Engine* engine = (Engine*)app->userData;
	if (!engine)
		return 0;

	return engine->handleInput(event);
}

/**
 * Process the next main command.
 */
void Engine::handleCmdThunk(struct android_app* app, int32_t cmd)
{
    Engine* engine = (Engine*)app->userData;
	if (!engine)
		return;

	engine->handleCommand(cmd);
}

// If we cause an exception in JNI, we print the exception info to
// the log, we clear the exception to avoid a pending-exception
// crash, and we force the function to return.

bool Engine::showToast(const char* text)
{
	// JNI is running the equivalent of the following Java code
	// activity.showToastAlert(text);

	jstring jniText = mApp->appThreadEnv->NewStringUTF(text);
	EXCEPTION_RETURN(mApp->appThreadEnv);

    jclass thisClass = mApp->appThreadEnv->GetObjectClass(mApp->appThreadThis);
	EXCEPTION_RETURN(mApp->appThreadEnv);

	jmethodID showToastAlert = mApp->appThreadEnv->GetMethodID(thisClass, 
		"showToastAlert", "(Ljava/lang/String;)V");
	EXCEPTION_RETURN(mApp->appThreadEnv);

	mApp->appThreadEnv->CallVoidMethod(mApp->appThreadThis, showToastAlert, jniText);
	EXCEPTION_RETURN(mApp->appThreadEnv);

	return true;
}

bool Engine::launchURL(const char* url)
{
	// JNI is running the equivalent of the following Java code
	// activity.launchURL(url);

	jstring jniText = mApp->appThreadEnv->NewStringUTF(url);
	EXCEPTION_RETURN(mApp->appThreadEnv);

    jclass thisClass = mApp->appThreadEnv->GetObjectClass(mApp->appThreadThis);
	EXCEPTION_RETURN(mApp->appThreadEnv);

	jmethodID launchURL = mApp->appThreadEnv->GetMethodID(thisClass, 
		"launchURL", "(Ljava/lang/String;)V");
	EXCEPTION_RETURN(mApp->appThreadEnv);

	mApp->appThreadEnv->CallVoidMethod(mApp->appThreadThis, launchURL, jniText);
	EXCEPTION_RETURN(mApp->appThreadEnv);
	return true;
}

void Engine::setGameplayMode(bool running)
{
	mGameplayMode = running;
	cGameApp::m_sbGamePause = !running;	
}
