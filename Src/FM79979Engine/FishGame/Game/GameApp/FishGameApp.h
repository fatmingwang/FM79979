#pragma once

class	cFishGameApp:public cGameApp
{
protected:
	void	Update(float e_fElpaseTime);
	void	Render();
public:
#if defined(ANDROID)
	cFishGameApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil);
#elif defined(WIN32)
	cFishGameApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920,1080),Vector2 e_vViewportSize = Vector2(1920,1080));
#else//IOS emscripten
	cFishGameApp(Vector2 e_vGameResolution = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT),Vector2 e_vViewportSize = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT));
#endif
	virtual ~cFishGameApp();

	void	Init();
	virtual	void	MouseDown(int e_iPosX,int e_iPosY) override;
	virtual	void	MouseMove(int e_iPosX,int e_iPosY) override;
	virtual	void	MouseUp(int e_iPosX,int e_iPosY) override;
	virtual	void	KeyDown(char e_char) override;
	virtual	void	KeyUp(char e_char) override;
	//if true game exit
	bool	m_bLeave;
};

bool	InitOnceObject();
bool	DeleteOnceObject();