#pragma once

#include "CommonDefine.h"
#include "Utility.h"
#include "../Math/Vector4.h"
#define	MAX_PERFORMANCE_TC_COUNT	100

namespace	FATMING_CORE
{
	class cCommonApp
	{
		std::map<int,UT::sTimeAndFPS>								m_LazyTimeElpaseMap;
	public:
		static bool m_sbGamePause;
		static	UT::sTimeAndFPS										m_sTimeAndFPS;	//
		static	UT::sTimeAndFPS										m_sPerformanceFPSChecker[MAX_PERFORMANCE_TC_COUNT];	//
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
		static	cTimeoutCallBackFunction*							m_spTimeoutCallBackFunction;
		//android
#if defined(ANDROID)
		static	std::string*		m_spAPKPath;
		static	std::string*		m_spInternalDirectory;//system
		static	std::string*		m_spExternalDirectory;//internal sd
		static	std::string*		m_spExternalSDDirectory;//external sd
		static	std::string*		m_spobbPath;//external sd
		
#elif defined(UWP)
		static	std::string*		m_spUWPAssetsDirectory;//external sd
		static	std::string*		m_spUWPAppDataLocalDirectory;//external sd
#endif
		cCommonApp();
		virtual ~cCommonApp();
		virtual	void												Update(float e_fElpaseTime);
		//
		//GAME_PAUSE_EVENT_ID	-123456789 is game pause
		static	bool												EventMessageShot(unsigned int e_usID, void*e_pData);
		static	bool												EventMessageShot(unsigned int e_usID, void*e_pData, int e_iDataSize);
		//lazy to keep point data so shot immediately,
		//ensure not call recursively,event call evnet infinty
		static	bool												EventMessageShotImmediately(unsigned int e_usID, void*e_pData);
		static  float												DumpLogWhileElpaseTimeIsTooMuch(const char*e_strName, float e_fAllownedElpaseTime = 0.1f, bool e_bWriteToFile = true,int e_iTCIndex = 0);

		float														GetElpaseTimeByID(int e_iID);
		//e_iID == -1 clear all data
		void														ClearElpaseTimeByID(int e_iID);
	};

	//SetTimeoutByCommonApp([]() 
	//	{
	//		FMLOG("qoo1");
	//	}, 1);
	//SetTimeoutByCommonApp([]()
	//	{
	//		FMLOG("qoo2");
	//	}, 2);
	uint64		SetTimeoutByCommonApp(f_TimeoutCallbackFunction e_f_TimeoutCallbackFunction, float e_fTime);
	bool		RemoveTimeoutFunctionByCommonApp(uint64 e_i64ID);
}