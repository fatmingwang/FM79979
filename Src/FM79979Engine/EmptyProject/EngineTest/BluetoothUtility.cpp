//if compile error please add 
//#include <winsock2.h>
//#include <ws2bth.h>
//#include <Bthsdpdef.h>
//#include <Setupapi.h>
//before windows.h

#include "stdafx.h"
#include "Bluetooth.h"
#include "bluetoothapis.h"
#include <windows.h>
#include <Setupapi.h>
#include <stdio.h>
#include <tchar.h>
#include <cfgmgr32.h>
#include <devguid.h>
#include <string>
using namespace std;

#define WIN32_LEAN_AND_MEAN

typedef basic_string<TCHAR> tstring;

std::wstring GetGenericBluetoothAdapterInstanceID()
{
	unsigned i;
	CONFIGRET r;
	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	wchar_t deviceInstanceID[MAX_DEVICE_ID_LEN];
	std::wstring l_strResult;

	// Find all bluetooth radio modules
	hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_BLUETOOTH, NULL, NULL, DIGCF_PRESENT);

	if (hDevInfo == INVALID_HANDLE_VALUE)
		return NULL;

	// Get first Generic Bluetooth Adapter InstanceID
	for (i = 0; ; i++)
	{
		DeviceInfoData.cbSize = sizeof(DeviceInfoData);

		if (!SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData))
			break;

		r = CM_Get_Device_ID(DeviceInfoData.DevInst, deviceInstanceID, MAX_PATH, 0);

		if (r != CR_SUCCESS)
			continue;

		if (_tcsncmp(_TEXT("USB"), deviceInstanceID, 3) == 0)
		{
			l_strResult = deviceInstanceID;
			return deviceInstanceID;
		}
	}

	return l_strResult;
}

void find_and_replace(wchar_t* source, const wchar_t* strFind, const wchar_t* strReplace)
{
	tstring s = tstring(source);
	tstring f = tstring(strFind);
	tstring r = tstring(strReplace);
	size_t j;

	for (; (j = s.find(f)) != string::npos; )
	{
		s.replace(j, f.length(), r);
	}

	wcscpy(source, s.c_str());
}

int ChangeLocalBluetoothRadioName(const wchar_t*e_strBlueToothName)
{

	std::wstring l_strInstanceID = GetGenericBluetoothAdapterInstanceID();

	if (l_strInstanceID.size() == 0)
	{
		_tprintf(_TEXT("Failed to get Generic Bluetooth Adapter InstanceID\n"));
		return 1;
	}

	wchar_t l_strInstanceIDModified[TEMP_SIZE];
	wcscpy(l_strInstanceIDModified, l_strInstanceID.c_str());
	find_and_replace(l_strInstanceIDModified, _TEXT("\\"), _TEXT("#"));

	HANDLE hDevice;
	wchar_t fileName[1024] = { 0 };
	wcscpy(fileName, _TEXT("\\\\.\\"));
	wcscat(fileName, l_strInstanceIDModified);
	wcscat(fileName, _TEXT("#{a5dcbf10-6530-11d2-901f-00c04fb951ed}"));

	//SAFE_DELETE(l_strInstanceIDModified);

	hDevice = CreateFile(fileName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		_tprintf(_TEXT("Failed to open device. Error code: %d\n"), GetLastError());
		return EXIT_FAILURE;
	}

	//Change radio module local name in registry
	const wchar_t* RMLocalName = e_strBlueToothName;
	char bufRMLocalName[256];

	int nLength = WideCharToMultiByte(CP_UTF8, 0, RMLocalName,
		-1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, RMLocalName,
		-1, bufRMLocalName, nLength, NULL, NULL);

	HKEY hKey;
	TCHAR rmLocalNameKey[1024] = { 0 };
	LSTATUS ret;
	wcscpy(rmLocalNameKey, _TEXT("SYSTEM\\ControlSet001\\Enum\\"));
	wcscat(rmLocalNameKey, l_strInstanceID.c_str());
	wcscat(rmLocalNameKey, _TEXT("\\Device Parameters"));

	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, rmLocalNameKey,
		0L, KEY_SET_VALUE, &hKey);

	if (ret != ERROR_SUCCESS)
	{
		_tprintf(TEXT("Failed to open registry key. Error code: %d\n"), ret);
		return EXIT_FAILURE;
	}

	ret = RegSetValueEx(hKey, _TEXT("Local Name"), 0, REG_BINARY,
		(LPBYTE)bufRMLocalName, nLength);

	if (ret != ERROR_SUCCESS)
	{
		_tprintf(TEXT("Failed to set registry key. Error code: %d\n"), ret);
		return EXIT_FAILURE;
	}

	RegCloseKey(hKey);

	// This check decides what IO control code to use based on if we're in XP or Vista (and later).
	OSVERSIONINFO osver;
	osver.dwOSVersionInfoSize = sizeof(osver);
	GetVersionEx(&osver);

	UINT ctlCode = (UINT)(6 > osver.dwMajorVersion ? 0x220fd4 : 0x411008);
	long reload = 4;  // tells the control function to reset or reload or similar...
	DWORD bytes = 0; // merely a placeholder

					 // Send radio module driver command to update device information
	if (!DeviceIoControl(hDevice, ctlCode, &reload, 4, NULL, 0, &bytes, NULL))
	{
		_tprintf(TEXT("Failed to update radio module local name. Error code: %d\n"), GetLastError());
		return EXIT_FAILURE;
	}

	_tprintf(TEXT("ChangeLocalBluetoothRadioName:Done\n"));

	return EXIT_SUCCESS;
}

//
// NameToBthAddr converts a bluetooth device name to a bluetooth address,
// if required by performing inquiry with remote name requests.
// This function demonstrates device inquiry, with optional LUP flags.
//
ULONG NameToBthAddr(const LPWSTR pszRemoteName,PSOCKADDR_BTH pRemoteBtAddr)
{
	INT             iResult = true;
	BOOL            bContinueLookup = FALSE, bRemoteDeviceFound = FALSE;
	ULONG           ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
	HANDLE          hLookup = NULL;
	PWSAQUERYSET    pWSAQuerySet = NULL;

	ZeroMemory(pRemoteBtAddr, sizeof(*pRemoteBtAddr));

	pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,ulPQSSize);
	if (NULL == pWSAQuerySet) 
	{
		iResult = STATUS_NO_MEMORY;
		wprintf(L"!ERROR! | Unable to allocate memory for WSAQUERYSET\n");
	}

	//
	// Search for the device with the correct name
	//
	if (iResult) 
	{
		#define CXN_MAX_INQUIRY_RETRY             3
		#define CXN_DELAY_NEXT_INQUIRY            15
		for (INT iRetryCount = 0;!bRemoteDeviceFound && (iRetryCount < CXN_MAX_INQUIRY_RETRY);iRetryCount++) {
			//
			// WSALookupService is used for both service search and device inquiry
			// LUP_CONTAINERS is the flag which signals that we're doing a device inquiry.
			//
			ulFlags = LUP_CONTAINERS;

			//
			// Friendly device name (if available) will be returned in lpszServiceInstanceName
			//
			ulFlags |= LUP_RETURN_NAME;

			//
			// BTH_ADDR will be returned in lpcsaBuffer member of WSAQUERYSET
			//
			ulFlags |= LUP_RETURN_ADDR;

			if (0 == iRetryCount) {
				wprintf(L"*INFO* | Inquiring device from cache...\n");
			}
			else {
				//
				// Flush the device cache for all inquiries, except for the first inquiry
				//
				// By setting LUP_FLUSHCACHE flag, we're asking the lookup service to do
				// a fresh lookup instead of pulling the information from device cache.
				//
				ulFlags |= LUP_FLUSHCACHE;

				//
				// Pause for some time before all the inquiries after the first inquiry
				//
				// Remote Name requests will arrive after device inquiry has
				// completed.  Without a window to receive IN_RANGE notifications,
				// we don't have a direct mechanism to determine when remote
				// name requests have completed.
				//
				wprintf(L"*INFO* | Unable to find device.  Waiting for %d seconds before re-inquiry...\n", CXN_DELAY_NEXT_INQUIRY);
				Sleep(CXN_DELAY_NEXT_INQUIRY * 1000);

				wprintf(L"*INFO* | Inquiring device ...\n");
			}

			//
			// Start the lookup service
			//
			iResult = true;
			hLookup = 0;
			bContinueLookup = FALSE;
			ZeroMemory(pWSAQuerySet, ulPQSSize);
			pWSAQuerySet->dwNameSpace = NS_BTH;
			pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
			iResult = WSALookupServiceBegin(pWSAQuerySet, ulFlags, &hLookup);

			//
			// Even if we have an error, we want to continue until we
			// reach the CXN_MAX_INQUIRY_RETRY
			//
			if ((NO_ERROR == iResult) && (NULL != hLookup)) {
				bContinueLookup = TRUE;
			}
			else if (0 < iRetryCount) {
				wprintf(L"=CRITICAL= | WSALookupServiceBegin() failed with error code %d, WSAGetLastError = %d\n", iResult, WSAGetLastError());
				break;
			}

			while (bContinueLookup) {
				//
				// Get information about next bluetooth device
				//
				// Note you may pass the same WSAQUERYSET from LookupBegin
				// as long as you don't need to modify any of the pointer
				// members of the structure, etc.
				//
				// ZeroMemory(pWSAQuerySet, ulPQSSize);
				// pWSAQuerySet->dwNameSpace = NS_BTH;
				// pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
				if (NO_ERROR == WSALookupServiceNext(hLookup,
					ulFlags,
					&ulPQSSize,
					pWSAQuerySet)) 
				{
					//
					// Compare the name to see if this is the device we are looking for.
					//
					if ((pWSAQuerySet->lpszServiceInstanceName != NULL) &&
						(_wcsicmp(pWSAQuerySet->lpszServiceInstanceName, pszRemoteName)) == 0)
					{
						//
						// Found a remote bluetooth device with matching name.
						// Get the address of the device and exit the lookup.
						//
						CopyMemory(pRemoteBtAddr,(PSOCKADDR_BTH)pWSAQuerySet->lpcsaBuffer->RemoteAddr.lpSockaddr,sizeof(*pRemoteBtAddr));
						bRemoteDeviceFound = TRUE;
						bContinueLookup = FALSE;
					}
				}
				else 
				{
					iResult = WSAGetLastError();
					if (WSA_E_NO_MORE == iResult) 
					{ //No more data
						//
						// No more devices found.  Exit the lookup.
						//
						bContinueLookup = FALSE;
					}
					else 
					if (WSAEFAULT == iResult) 
					{
						//
						// The buffer for QUERYSET was insufficient.
						// In such case 3rd parameter "ulPQSSize" of function "WSALookupServiceNext()" receives
						// the required size.  So we can use this parameter to reallocate memory for QUERYSET.
						//
						HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
						pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
							HEAP_ZERO_MEMORY,
							ulPQSSize);
						if (NULL == pWSAQuerySet) {
							wprintf(L"!ERROR! | Unable to allocate memory for WSAQERYSET\n");
							iResult = STATUS_NO_MEMORY;
							bContinueLookup = FALSE;
						}
					}
					else 
					{
						wprintf(L"=CRITICAL= | WSALookupServiceNext() failed with error code %d\n", iResult);
						bContinueLookup = FALSE;
					}
				}
			}

			//
			// End the lookup service
			//
			WSALookupServiceEnd(hLookup);

			if (STATUS_NO_MEMORY == iResult) 
			{
				break;
			}
		}
	}

	if (NULL != pWSAQuerySet) 
	{
		HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
		pWSAQuerySet = NULL;
	}

	if (bRemoteDeviceFound) 
	{
		iResult = true;
	}
	else 
	{
		iResult = false;
	}

	return iResult;
}