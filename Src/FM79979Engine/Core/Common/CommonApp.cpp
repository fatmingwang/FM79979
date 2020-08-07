#include "CommonApp.h"
#include "EventSender/MessageSender.h"
#ifdef UWP
extern "C" IMAGE_DOS_HEADER __ImageBase;

std::wstring GetExecutablePath()
{
	std::wstring buffer;
	size_t nextBufferLength = MAX_PATH;

	for (;;)
	{
		buffer.resize(nextBufferLength);
		nextBufferLength *= 2;

		SetLastError(ERROR_SUCCESS);

		auto pathLength = GetModuleFileName(reinterpret_cast<HMODULE>(&__ImageBase), &buffer[0], static_cast<DWORD>(buffer.length()));

		if (pathLength == 0)
			throw std::exception("GetModuleFileName failed"); // You can call GetLastError() to get more info here

		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			buffer.resize(pathLength);
			return buffer;
		}
	}
}

void RemoveLastPathComponent(std::wstring& path)
{
	auto directoryLength = path.length() - 1;

	while (directoryLength > 0 && path[directoryLength] != '\\' && path[directoryLength] != '/')
		directoryLength--;

	if (directoryLength > 0)
		path.resize(directoryLength);
}

std::wstring GetExecutableDirectory()
{
	auto executablePath = GetExecutablePath();
	RemoveLastPathComponent(executablePath);
	return executablePath;
}
#endif

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
#elif defined(UWP)
	std::string*										cCommonApp::m_spUWPAssetsDirectory = nullptr;
	std::string*										cCommonApp::m_spUWPAppDataLocalDirectory = nullptr;
	using namespace Windows::Storage;
#endif

	cCommonApp::cCommonApp()
	{
		m_spMessageSenderManager = new cMessageSenderManager();
#ifdef UWP

		m_spUWPAssetsDirectory = new std::string;
		m_spUWPAppDataLocalDirectory = new std::string;
		//https://stackoverflow.com/questions/54759816/uwp-how-to-access-local-data-directory
		StorageFolder^ localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;
		const wchar_t* wPath = localFolder->Path->Data();
		*m_spUWPAppDataLocalDirectory = WcharToChar(wPath);
		*m_spUWPAppDataLocalDirectory += "/";
		*m_spUWPAssetsDirectory = WcharToChar(GetExecutableDirectory());
		*m_spUWPAssetsDirectory += "/Assets/";
//#ifdef DEBUG
//		*m_spUWPAppDataLocalDirectory = "C:/Users/desig.DESIGN24-NB/Desktop/Work/CarDrivingNodes/Media/";
//		*m_spUWPAssetsDirectory = "C:/Users/desig.DESIGN24-NB/Desktop/Work/CarDrivingNodes/Media/";
//		//*m_spUWPAppDataLocalDirectory += "../../";
//		//*m_spUWPAssetsDirectory += "../../";
//#endif
		
#endif
	}

	cCommonApp::~cCommonApp()
	{
		SAFE_DELETE(m_spMessageSenderManager);
#ifdef UWP
		SAFE_DELETE(m_spUWPAssetsDirectory);
		SAFE_DELETE(m_spUWPAppDataLocalDirectory);
#endif
#if defined(ANDROID)
		SAFE_DELETE(cCommonApp::m_spAPKPath);
		SAFE_DELETE(cCommonApp::m_spInternalDirectory);
		SAFE_DELETE(cCommonApp::m_spExternalDirectory);
		SAFE_DELETE(cCommonApp::m_spExternalSDDirectory);
#endif
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