//----------------------------------------------------------------------------------
// File:        android/native_subclass/jni/engine.h
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
#ifndef __ENGINE_H
#define __ENGINE_H

#include "Android/nv_native_app_glue.h"
#include "Android/nv_egl_util.h"

#define APP_NAME "BluffingGirl"

#include <android/sensor.h>

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG,  \
											 APP_NAME, \
											 __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,  \
											 APP_NAME, \
											 __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  \
											 APP_NAME, \
											 __VA_ARGS__))

class Engine
{
	bool		m_bAppRunning;
public:
	Engine(NvEGLUtil& egl, struct android_app* app);
	~Engine();

	bool isGameplayMode() { return mGameplayMode; }
	bool renderFrame(bool allocateIfNeeded);

	struct android_app* mApp;
protected:

	void setGameplayMode(bool paused);
	bool checkWindowResized();

	static void handleCmdThunk(struct android_app* app, int32_t cmd);
	static int32_t handleInputThunk(struct android_app* app, AInputEvent* event);

	bool initUI();

	bool resizeIfNeeded();

	int handleInput(AInputEvent* event);
	void handleCommand(int cmd);

	bool showToast(const char* text);
	bool launchURL(const char* url);

	NvEGLUtil& mEgl;

	bool mResizePending;

	bool mGameplayMode;

	bool m_uiInitialized;
	int mOrientation;
};
extern Engine*g_pEngine;

#endif // __ENGINE_H
