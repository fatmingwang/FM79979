#ifndef GAME_APP_H
#define GAME_APP_H

	class	cMyApp:public cGameApp
	{
		class cGUIForFileTransfer* m_pGUIForFileTransfer;
	protected:
		void	Update(float e_fElpaseTime);
		void	Render();
#ifdef WINDOWS
		virtual	void	OpenglInit(HWND e_Hwnd){}
#endif
	public:
#if defined(ANDROID)
		cMyApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil);
#elif defined(WIN32)
		cMyApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920,1080),Vector2 e_vViewportSize = Vector2(1920,1080));
#else
		cMyApp(Vector2 e_vGameResolution = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT),Vector2 e_vViewportSize = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT));
#endif
		virtual ~cMyApp();

		void	Init();
		virtual	void	MouseDown(int e_iPosX,int e_iPosY);
		virtual	void	MouseMove(int e_iPosX,int e_iPosY);
		virtual	void	MouseUp(int e_iPosX,int e_iPosY);		
		//void	Destory();

		//void	Run();

		void	KeyDown(char e_char);
		void	KeyUp(char e_char);
		//if true game exit
		bool	m_bLeave;
	};
#endif