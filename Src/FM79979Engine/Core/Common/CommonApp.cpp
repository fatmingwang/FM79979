#include "CommonApp.h"
#include "EventSender/MessageSender.h"
namespace	FATMING_CORE
{
	UT::sTimeAndFPS										cCommonApp::m_sTimeAndFPS;	
	class cMessageSenderManager*						cCommonApp::m_spMessageSenderManager = nullptr;
#ifdef DEBUG
	bool												cCommonApp::m_sbDebugFunctionWorking = true;
#else
	bool												cCommonApp::m_sbDebugFunctionWorking = false;
#endif
	bool												cCommonApp::m_sbAllowParseBinaryFile = true;
	std::string*										cCommonApp::m_psstrGameAppName = nullptr;	
	bool												cCommonApp::m_sbEnableMouseSingnal = true;
	bool												cCommonApp::m_sbEnableKeyboardSingnal = true;
	POINT												cCommonApp::m_sMousePosition = {0,0};
	POINT												cCommonApp::m_sScreenMousePosition = {0,0};
	bool    											cCommonApp::m_bMouseHover = false;;
	bool												cCommonApp::m_sbMouseClickStatus[3] = {false,false, false};//0 left,1 right,2 mid button
	short												cCommonApp::m_sMouseWhellDelta = 0;
	bool												cCommonApp::m_sbTouched = false;//if true mouse down
	bool												cCommonApp::m_sucKeyData[MAX_PATH];
	float												cCommonApp::m_sfVersion = 1.f;
	bool												cCommonApp::m_sbLeave = false;
	bool												cCommonApp::m_sbGamePause = false;
//Android
#if defined(ANDROID)
	std::string*										cCommonApp::m_spAPKPath = nullptr;
	std::string*										cCommonApp::m_spInternalDirectory = nullptr;
	std::string*										cCommonApp::m_spExternalDirectory = nullptr;
	std::string*										cCommonApp::m_spExternalSDDirectory = nullptr;
#endif

	cCommonApp::cCommonApp()
	{
		m_spMessageSenderManager = new cMessageSenderManager();
	}

	cCommonApp::~cCommonApp()
	{
		SAFE_DELETE(m_spMessageSenderManager);
	}

	void cCommonApp::Update(float e_fElpaseTime)
	{
		if (m_spMessageSenderManager)
			m_spMessageSenderManager->Update(e_fElpaseTime);
	}
	bool	cCommonApp::EventMessageShot(unsigned int e_usID, void*e_pData)
	{
		if (m_spMessageSenderManager)
			return m_spMessageSenderManager->EventMessageShot(e_usID, e_pData);
		return false;
	}
	
	bool	cCommonApp::EventMessageShot(unsigned int e_usID, void*e_pData, int e_iDataSize)
	{
		if (m_spMessageSenderManager)
			return m_spMessageSenderManager->EventMessageShot(e_usID, (char*)e_pData, e_iDataSize);
		return false;
	}
	bool cCommonApp::EventMessageShotImmediately(unsigned int e_usID, void * e_pData)
	{
		if (m_spMessageSenderManager)
			return m_spMessageSenderManager->EventMessageShotImmediately(e_usID, (char*)e_pData);
		return false;
	}
}