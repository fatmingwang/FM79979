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
		//android
#if defined(ANDROID)
		static	std::string*		m_spAPKPath;
		static	std::string*		m_spInternalDirectory;//system
		static	std::string*		m_spExternalDirectory;//internal sd
		static	std::string*		m_spExternalSDDirectory;//external sd
#endif
		cCommonApp();
		~cCommonApp();
		virtual	void												Update(float e_fElpaseTime);
		//
			//GAME_PAUSE_EVENT_ID	-123456789 is game pause
		static	bool												EventMessageShot(unsigned int e_usID, void*e_pData);
		static	bool												EventMessageShot(unsigned int e_usID, void*e_pData, int e_iDataSize);
	};
}