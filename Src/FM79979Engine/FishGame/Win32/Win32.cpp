#include "stdafx.h"
#include "../Game/GameApp/FishGameApp.h"
#include "Windowsx.h"
#include "../../AllLibInclude.h"
#include <stdio.h>
#include <io.h>
// test.cpp : Defines the entry point for the application.


#ifndef RETAILER
#define VLD_DEBUG
#endif

#ifdef VLD_DEBUG
//https://vld.codeplex.com/wikipage?title=Using%20Visual%20Leak%20Detector&referringTitle=Documentation
#include "../../../include/vld.h"
#pragma comment(lib, "../../../lib/vld.lib")
//#pragma comment(lib, "C:/Program Files (x86)/Visual Leak Detector/lib/Win32/vld.lib")
#endif

#define MAX_LOADSTRING 100

//
////#include "multimon.h"
//
//#define MONITOR_CENTER     0x0001        // center rect to monitor 
//#define MONITOR_CLIP     0x0000        // clip rect to monitor 
//#define MONITOR_WORKAREA 0x0002        // use monitor work area 
//#define MONITOR_AREA     0x0000        // use monitor entire area 
//
//// 
////  ClipOrCenterRectToMonitor 
//// 
////  The most common problem apps have when running on a 
////  multimonitor system is that they "clip" or "pin" windows 
////  based on the SM_CXSCREEN and SM_CYSCREEN system metrics. 
////  Because of app compatibility reasons these system metrics 
////  return the size of the primary monitor. 
//// 
////  This shows how you use the multi-monitor functions 
////  to do the same thing. 
//// 
//void ClipOrCenterRectToMonitor(LPRECT prc, UINT flags)
//{
//	HMONITOR hMonitor;
//	MONITORINFO mi;
//	RECT        rc;
//	int         w = prc->right - prc->left;
//	int         h = prc->bottom - prc->top;
//
//	// 
//	// get the nearest monitor to the passed rect. 
//	// 
//	hMonitor = MonitorFromRect(prc, MONITOR_DEFAULTTONEAREST);
//
//	// 
//	// get the work area or entire monitor rect. 
//	// 
//	mi.cbSize = sizeof(mi);
//	GetMonitorInfo(hMonitor, &mi);
//
//	if (flags & MONITOR_WORKAREA)
//		rc = mi.rcWork;
//	else
//		rc = mi.rcMonitor;
//
//	// 
//	// center or clip the passed rect to the monitor rect 
//	// 
//	if (flags & MONITOR_CENTER)
//	{
//		prc->left = rc.left + (rc.right - rc.left - w) / 2;
//		prc->top = rc.top + (rc.bottom - rc.top - h) / 2;
//		prc->right = prc->left + w;
//		prc->bottom = prc->top + h;
//	}
//	else
//	{
//		prc->left = max(rc.left, min(rc.right - w, prc->left));
//		prc->top = max(rc.top, min(rc.bottom - h, prc->top));
//		prc->right = prc->left + w;
//		prc->bottom = prc->top + h;
//	}
//}
//
//void ClipOrCenterWindowToMonitor(HWND hwnd, UINT flags)
//{
//	RECT rc;
//	GetWindowRect(hwnd, &rc);
//	ClipOrCenterRectToMonitor(&rc, flags);
//	SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
//}
//ClipOrCenterWindowToMonitor(g_hWnd, MONITOR_CENTER);
void MonitorReSynch();
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND g_hWnd;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
HHOOK MouseHook;
bool	g_bLeave = false;

POINT g_WindowSize;
cGameApp*g_pGameApp = 0;
int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	//console mode
	//https://docs.microsoft.com/en-us/windows/console/setconsolemode
#ifndef  RETAILER
//	FILE*l_pConsoleStream;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	bool l_bOpenOkay = false;
	//https://stackoverflow.com/questions/9020790/using-stdin-with-an-allocconsole
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	//if (::AllocConsole())
	//{
	//	int hCrt = ::_open_osfhandle((intptr_t) ::GetStdHandle(STD_OUTPUT_HANDLE), 0);
	//	FILE *hf = ::_fdopen(hCrt, "w");
	//	*stdout = *hf;
	//	::setvbuf(stdout, NULL, _IONBF, 0);

	//	hCrt = ::_open_osfhandle((intptr_t) ::GetStdHandle(STD_ERROR_HANDLE), 0);
	//	hf = ::_fdopen(hCrt, "w");
	//	*stderr = *hf;
	//	::setvbuf(stderr, NULL, _IONBF, 0);
	//}
	//fclose(l_pConsoleStream);
	//std::ios_base::sync_with_stdio();
	printf("game start !\n");
#endif
	//MonitorReSynch();
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	swprintf(szTitle, MAX_LOADSTRING, L"FM79979");
	swprintf(szWindowClass, MAX_LOADSTRING, L"FM79979");
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	g_pGameApp = new cFishGameApp(g_hWnd,cGameApp::m_svGameResolution,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
	g_pGameApp->Init();
	cGameApp::SetAcceptRationWithGameresolution((int)g_WindowSize.x,(int)g_WindowSize.y,(int)cGameApp::m_svGameResolution.x,(int)cGameApp::m_svGameResolution.y);

	SetTimer (g_hWnd, 0, 0, NULL) ;

	while (GetMessage(&msg, NULL, 0, 0)&&!g_bLeave)
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}
	SAFE_DELETE(g_pGameApp);
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return (int) msg.wParam;
}
//https://msdn.microsoft.com/en-us/library/dd162826(v=vs.85).aspx
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon = 0;// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_TEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm = 0;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
	bool	l_bFullScreen = false;
	cNodeISAX	l_NodeISAX;
	cGameApp::m_svViewPortSize.x = 1024.;
	cGameApp::m_svViewPortSize.y = 768.f;
	cGameApp::ResoluctionParse("EngineTestSetup.xml");
	//cGameApp::m_svViewPortSize.x = 3600;
	//cGameApp::m_svViewPortSize.y = 1000;
	DWORD	l_dwFlag = WS_OVERLAPPEDWINDOW;
	if(cGameApp::m_sbFullScreen)
		l_dwFlag = WS_VISIBLE | WS_POPUP |	WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	g_hWnd = CreateWindow(szWindowClass, szTitle, l_dwFlag, 0, 0, (int)cGameApp::m_svViewPortSize.Width(), (int)cGameApp::m_svViewPortSize.Height(), NULL, NULL, hInstance, NULL);

	//dual screen
	//https://www.intel.com/content/dam/www/public/us/en/documents/white-papers/creating-full-screen-applications-across-multiple-displays.pdf
	//g_hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP, 0,0, 1920 * 2, 1080, NULL, NULL,hInstance, NULL);

	if (!g_hWnd)
	{
	  return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
POINT g_MousePosition;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	float   l_fScaleX = cGameApp::m_svGameResolution.x/cGameApp::m_svViewPortSize.x;
	float   l_fScaleY = cGameApp::m_svGameResolution.y/cGameApp::m_svViewPortSize.y;	
	switch (message)
	{
	case  WM_SIZE:
		g_WindowSize.x = (int)LOWORD(lParam);
		g_WindowSize.y = (int)HIWORD(lParam);
		cGameApp::SetAcceptRationWithGameresolution((int)LOWORD(lParam),(int)HIWORD(lParam),(int)cGameApp::m_svGameResolution.x,(int)cGameApp::m_svGameResolution.y);
		break;
	case WM_TIMER:
		if( !g_bLeave && g_pGameApp )
		{
			g_pGameApp->Run();
		}
		else
			SAFE_DELETE(g_pGameApp);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	//case WM_KEYFIRST:
	//	if( g_pGameApp )
	//	{
	//		g_pGameApp->KeyDown(LOWORD(wParam));
	//	}
	//	break;

	case WM_KEYDOWN:
		if( g_pGameApp )
		{
			g_pGameApp->KeyDown((char)LOWORD(wParam));
		}
		switch(LOWORD(wParam))
		{
			case VK_RETURN:
			{

			}
			break;
			case VK_ESCAPE:
				g_bLeave = true;
				break;
			break;
		}
		break;
	case WM_KEYUP:
		if( g_pGameApp )
		{
			g_pGameApp->KeyUp((char)LOWORD(wParam));
		}
		break;
	case WM_MOUSEWHEEL:
		cGameApp::m_sMouseWhellDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	case WM_LBUTTONDOWN:
		if( g_pGameApp )
		{
			g_pGameApp->MouseDown(g_MousePosition.x,g_MousePosition.y);
			g_pGameApp->MouseButtonClickEvent(true,0);
		}
			return 0;
	case WM_LBUTTONUP:
		if( g_pGameApp )
		{
			g_pGameApp->MouseUp(g_MousePosition.x,g_MousePosition.y);
			g_pGameApp->MouseButtonClickEvent(false,0);			
		}
			return 0;
	case WM_MOUSEMOVE:
		g_MousePosition.x = (int)(GET_X_LPARAM(lParam));
		g_MousePosition.y = (int)(GET_Y_LPARAM(lParam));
		g_pGameApp->m_sMousePosition = g_MousePosition;
		if( g_pGameApp )
			g_pGameApp->MouseMove(g_MousePosition.x,g_MousePosition.y);		

	case WM_MBUTTONDOWN:
		if( g_pGameApp )
			g_pGameApp->MouseButtonClickEvent(true,2);
		break;
	case WM_MBUTTONUP:
		if( g_pGameApp )
			g_pGameApp->MouseButtonClickEvent(false,2);
		break;
	case WM_RBUTTONDOWN:
		if( g_pGameApp )
			g_pGameApp->MouseButtonClickEvent(true,1);
		break;
	case WM_RBUTTONUP:
		if( g_pGameApp )
			g_pGameApp->MouseButtonClickEvent(false,1);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



void MonitorReSynch()
{
	// collect system and monitor information for future reference
	DISPLAY_DEVICE l_DISPLAY_DEVICE;
	vector<DISPLAY_DEVICE> detectedDisplays;
	vector<DISPLAY_DEVICE>::iterator itDetDisp;

	l_DISPLAY_DEVICE.cb = sizeof(l_DISPLAY_DEVICE);
	DWORD l_iDeviceIndex = 0; // device index
	int id = 1; // monitor number, as used by Display Properties > Settings
	//std::auto_ptr<SActiveDisplay> ad;
	detectedDisplays.clear();

	// Loop over all display devices 
	while (EnumDisplayDevices(0, l_iDeviceIndex, &l_DISPLAY_DEVICE, 0))
	{
		// ignore virtual mirror displays
		if (!(l_DISPLAY_DEVICE.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
		{

			// get information about the monitor attached to this display adapter. dualhead cards
			// and laptop video cards can have multiple monitors attached

			DISPLAY_DEVICE ddMon;
			ZeroMemory(&ddMon, sizeof(ddMon));
			ddMon.cb = sizeof(ddMon);
			DWORD devMon = 0;

			// please note that this enumeration may not return the correct monitor if multiple monitors
			// are attached. this is because not all display drivers return the ACTIVE flag for the monitor
			// that is actually active
			while (EnumDisplayDevices(l_DISPLAY_DEVICE.DeviceName, devMon, &ddMon, 0))
			{
				if (ddMon.StateFlags & DISPLAY_DEVICE_ACTIVE)
				{
					detectedDisplays.push_back(ddMon);
				}
				ZeroMemory(&ddMon, sizeof(ddMon));
				ddMon.cb = sizeof(ddMon);
				devMon++;
			}

			if (detectedDisplays.size()>0)
			{
				itDetDisp = detectedDisplays.begin();
				while (itDetDisp != detectedDisplays.end())
				{
					// get information about the display's position and the current display mode

					DEVMODE dm;
					ZeroMemory(&dm, sizeof(dm));
					dm.dmSize = sizeof(dm);

					if (EnumDisplaySettingsEx(itDetDisp->DeviceName, ENUM_CURRENT_SETTINGS, &dm, 0) == FALSE)
						EnumDisplaySettingsEx(itDetDisp->DeviceName, ENUM_REGISTRY_SETTINGS, &dm, 0);

					// get the monitor handle and workspace
					HMONITOR hm = 0;
					MONITORINFO mi;
					ZeroMemory(&mi, sizeof(mi));
					mi.cbSize = sizeof(mi);
					if (itDetDisp->StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
					{
						// display is enabled. only enabled displays have a monitor handle
						POINT pt = { dm.dmPosition.x, dm.dmPosition.y };
						hm = MonitorFromPoint(pt, MONITOR_DEFAULTTONULL);
						if (hm)
							GetMonitorInfo(hm, &mi);

						//ad.reset(new SActiveDisplay());
//						memcpy(&ad->ddMon, &(*itDetDisp), sizeof(DISPLAY_DEVICE));
	//					ad->hm = hm;
		//				memcpy(&ad->mi, &mi, sizeof(MONITORINFO));
			//			memcpy(&ad->dm, &dm, sizeof(DEVMODE));
						//m_DisplayInformation.push_back(*(ad.get()));
					}

					itDetDisp++;
				}
				detectedDisplays.clear();
			}
			id++;
		}
		l_iDeviceIndex++;
	}
}