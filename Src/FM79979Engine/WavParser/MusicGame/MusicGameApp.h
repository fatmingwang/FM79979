#pragma once
namespace	FATMING_CORE
{
	class cSoundCapture;
}
class cSoundFFTCapture;
class cMessageSenderManager;
class cMusicGameApp:public cGameApp
{
	cPhaseManager*			m_pPhaseManager;
public:
#if defined(ANDROID)
		cMusicGameApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil);
#elif defined(WIN32)
		cMusicGameApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920,1080),Vector2 e_vViewportSize = Vector2(1920,1080));
#else
		cMusicGameApp(Vector2 e_vGameResolution = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT),Vector2 e_vViewportSize = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT));
#endif
		virtual ~cMusicGameApp();
		//
		virtual	void				Init();
		virtual	void				Update(float e_fElpaseTime);
		virtual	void				Render();

		virtual	void				EditorInit();
		virtual	void				EditorUpdate(float e_fElpaseTime);
		virtual	void				EditorRender();

		virtual	void				EditorRun();

		virtual	void				MouseDown(int e_iPosX,int e_iPosY);
		virtual	void				MouseMove(int e_iPosX,int e_iPosY);
		virtual	void				MouseUp(int e_iPosX,int e_iPosY);
		virtual	void				KeyPress(char e_char);
		virtual	void				KeyDown(char e_char);
		virtual	void				KeyUp(char e_char);
		//I just lazy so put here,
		static cSoundCapture*		m_pSoundCapture;
		static cSoundFFTCapture*	m_pSoundFFTCapture;
};