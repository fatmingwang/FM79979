#pragma once

class cMusicGameApp:public cGameApp
{
	protected:
#if defined(ANDROID)
		cMusicGameApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil);
#elif defined(WIN32)
		cMusicGameApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920,1080),Vector2 e_vViewportSize = Vector2(1920,1080));
#else
		cMusicGameApp(Vector2 e_vGameResolution = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT),Vector2 e_vViewportSize = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT));
#endif

		virtual	void				Init();
		virtual	void				Update(float e_fElpaseTime);
		virtual	void				Render();
};