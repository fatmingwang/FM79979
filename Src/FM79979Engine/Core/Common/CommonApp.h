#pragma once

#include "CommonDefine.h"
#include "Utility.h"
#include "../Math/Vector4.h"

namespace	FATMING_CORE
{
	class cCommonApp
	{
	public:
		static bool m_sbGamePause;
		static	UT::sTimeAndFPS										m_sTimeAndFPS;	//
		//
		static	class cMessageSenderManager*						m_spMessageSenderManager;
		static	bool												m_sbDebugFunctionWorking;
		static	bool												m_sbAllowParseBinaryFile;
		static	std::string*										m_psstrGameAppName;
		//mouse
			//
		static	bool												m_sbEnableMouseSingnal;
		static	bool												m_sbEnableKeyboardSingnal;
		static  POINT												m_sMousePosition;
		static  POINT												m_sScreenMousePosition;
		static	bool    											m_bMouseHover;
		static	bool												m_sbMouseClickStatus[3];//0 left,1 right,2 mid button
		static	short												m_sMouseWhellDelta;
		static	bool												m_sbTouched;//if true mouse down
		//all key data is pressed or not
		static	bool												m_sucKeyData[MAX_PATH];
		//
		//this one could be write into file,so we could check user version
		static  float												m_sfVersion;
		//if true game exit
		static	bool												m_sbLeave;

		virtual	void												Update(float e_fElpaseTime);
		//
			//GAME_PAUSE_EVENT_ID	-123456789 is game pause
		static	bool												EventMessageShot(unsigned int e_usID, void*e_pData);
		static	bool												EventMessageShot(unsigned int e_usID, void*e_pData, int e_iDataSize);
	};
}

//bool    											cGameApp::m_bMouseHover = false;;
//bool												cGameApp::m_sbTouched = false;
//bool												cGameApp::m_sbGamePause = false;
//float												cGameApp::m_sfOpenGLVersion = 2.f;
//POINT												cGameApp::m_sMousePosition = { 0,0 };
//POINT												cGameApp::m_sScreenMousePosition = { 0,0 };
//bool												cGameApp::m_sbMouseClickStatus[3] = { false,false,false };
//short												cGameApp::m_sMouseWhellDelta = 0;
//Vector4												cGameApp::m_spOpenGLRender->m_vViewPortSize(0.f, 0.f, 1920.f, 1080.f);
//Vector4												cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize(0.f, 0.f, 1920.f, 1080.f);
//float												cGameApp::m_sfVersion = 1.f;
//bool												cGameApp::m_sbLeave = false;
//UT::sTimeAndFPS										cGameApp::m_sTimeAndFPS;
//std::vector<int>*									cGameApp::m_piSupportCompressedFormatVector = nullptr;;
//cMessageSenderManager*								cGameApp::m_spMessageSenderManager = nullptr;
//bool												cGameApp::m_sucKeyData[MAX_PATH];
//bool												cGameApp::m_sbEnableMouseSingnal = true;
//bool												cGameApp::m_sbEnableKeyboardSingnal = true;
//bool												cGameApp::m_sbDebugFunctionWorking = false;
//bool												cGameApp::m_sbAllowParseBinaryFile = true;
//std::string*										cGameApp::m_psstrGameAppName = 0;

//bool	cGameApp::EventMessageShot(unsigned int e_usID, void*e_pData)
//{
//	if (m_spMessageSenderManager)
//		return m_spMessageSenderManager->EventMessageShot(e_usID, e_pData);
//	return false;
//}
//
//bool	cGameApp::EventMessageShot(unsigned int e_usID, void*e_pData, int e_iDataSize)
//{
//	if (m_spMessageSenderManager)
//		return m_spMessageSenderManager->EventMessageShot(e_usID, (char*)e_pData, e_iDataSize);
//	return false;
//}