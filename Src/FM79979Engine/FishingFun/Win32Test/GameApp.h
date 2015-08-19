#ifndef GAME_APP_H
#define GAME_APP_H

#include "FishRodAndFishThreadData.h"
#include "PlayerData.h"

class	cFishingFunApp:public cGameApp
{
	cPhaseManager*	m_pPhaseManager;
protected:
	void	Update(float e_fElpaseTime);
	void	Render();
#ifdef WIN32
	virtual	void	OpenglInit(HWND e_Hwnd){}
#endif
public:
#if defined(ANDROID)
	cFishingFunApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil);
#elif defined(WIN32)
	cFishingFunApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920,1080),Vector2 e_vViewportSize = Vector2(1920,1080));
#else
	cFishingFunApp(Vector2 e_vGameResolution = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT),Vector2 e_vViewportSize = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT));
#endif
	~cFishingFunApp();

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
	//
	static	cFishRodAndFishThreadData*							m_spFishRodAndFishThreadData;
	//static	cNamedTypedObjectVector<cColladaParser>*			m_spAllColladaData;
	static  cColladaParser*										ProcessDaeFile(TiXmlElement*e_pElement);
	static  cPlayerData*										m_spPlayerData;
};

#endif