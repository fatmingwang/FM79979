#ifndef GAME_APP_H
#define GAME_APP_H


#include "../Core/AllCoreInclude.h"
#include "../Core/FMEvent/AllEventInclude.h"
#include "EditorMode.h"
#include "BookMode.h"

class	cFMBookApp:public cGameApp
{
	MyClass*	m_pCurrentMode;

	int		m_iMode;
protected:
	void	Update(float e_fElpaseTime);
	void	Render();
public:
#if defined(ANDROID)
	cFMBookApp(int e_iMode,ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize );
#elif defined(WIN32)
	cFMBookApp(int e_iMode,HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920,1080),Vector2 e_vViewportSize = Vector2(1920,1080));
#else
	cFMBookApp(int e_iMode,Vector2 e_vGameResolution = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT),Vector2 e_vViewportSize = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT));	
#endif
	~cFMBookApp();
	bool	m_bInitOk;
	void	Init();
	void	Destroy();
    void	MouseDown(int e_iPosX,int e_iPosY);
    void	MouseMove(int e_iPosX,int e_iPosY);
    void	MouseUp(int e_iPosX,int e_iPosY);
	//if true game exit
	bool	m_bLeave;
	//0 edit all data mode,1 game mode,edit FMUtilitied mode
	void    SwitchMode(int e_iMode);
};
	extern cEditorMode*			g_pEditorMode;
	extern cBookMode*			g_pBookMode;
	//for editor use
	extern cFMBookApp*g_pFMBookApp;

#endif