// test.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "test.h"
#include "BluffingGirlApp.h"
#include "Windowsx.h"
//#define	FFMPEG
#include "../../AllLibInclude.h"
#include <iostream>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
//#include "vld.h"
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Urlmon.lib")
extern int MyTest();
//#include "curl.h"
//int CurlTest(void)
//{
//  CURL *curl;
//  CURLcode res;
// 
//  curl = curl_easy_init();
//  if(curl) {
//    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
//    /* example.com is redirected, so we tell libcurl to follow redirection */ 
//    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
// 
//    /* Perform the request, res will get the return code */ 
//    res = curl_easy_perform(curl);
//    /* Check for errors */ 
//    if(res != CURLE_OK)
//      fprintf(stderr, "curl_easy_perform() failed: %s\n",
//              curl_easy_strerror(res));
// 
//    /* always cleanup */ 
//    curl_easy_cleanup(curl);
//  }
//  return 0;
//}
#define MAX_LOADSTRING 100

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


cGameApp*g_pGameApp = 0;
POINT g_WindowSize;
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MyTest();
	OutputDebugString(L"GameStart\n");
	_CrtMemState s1,s2,s3;
	_CrtMemCheckpoint(&s1);// Memory snapshot will be taken at this point

   _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
   _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
   _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
   _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );

	const WCHAR*l_str = NamedTypedObject::TypeID;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	//HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	cGameApp::m_sbDebugFunctionWorking = true;
	g_pGameApp = new cBluffingGirlApp(g_hWnd,cGameApp::m_spOpenGLRender->m_vGameResolution,Vector2(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(),cGameApp::m_spOpenGLRender->m_vViewPortSize.Height()));
	if( g_pGameApp )
		g_pGameApp->Init();
	cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution((int)g_WindowSize.x,(int)g_WindowSize.y,(int)cGameApp::m_spOpenGLRender->m_vGameResolution.x,(int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	SetTimer (g_hWnd, 0, 0, NULL) ;

    //MouseHook = SetWindowsHookEx(WH_MOUSE_LL,MouseHookProc,hInstance,0);
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)&&!g_bLeave)
	//while(1)
	{
		//g_pGameApp->Run();
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	//int *pp = new int[500];
	//UnhookWindowsHookEx(MouseHook);
	SAFE_DELETE(g_pGameApp);
	_CrtMemCheckpoint(&s2);// Another Memory snapshot will be taken at this point
	if (_CrtMemDifference(&s3,&s1,&s2))
    // _CrtMemDifference will calculate the memory allocation,
    // which has not been deallocated done between s1 and s2
    // memory check points.
    {
        _CrtDumpMemoryLeaks();
        // This line will dump the memory leak found between s1 and s2
        // memory check points.
    }
	NamedTypedObject::DumpUnReleaseInfo();
	return (int) msg.wParam;
}



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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_TEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
	cGameApp::CreateDefaultOpenGLRender();
	cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 1024.;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 768.f;
	cGameApp::ResoluctionParse("BluffingGirl/EngineTestSetup.xml");

	DWORD	l_dwFlag = WS_OVERLAPPEDWINDOW;
	if(cGameApp::m_sbFullScreen)
		l_dwFlag = WS_VISIBLE | WS_POPUP |	WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	int l_iWidth = (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width();
	int l_iHeight = (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height();
	g_hWnd = CreateWindow(szWindowClass, szTitle, l_dwFlag, 0, 0, l_iWidth,l_iHeight, NULL, NULL, hInstance, NULL);

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
bool	g_bLeftMouseDown = false;
bool	g_bRightMouseDown = false;
short	g_cMouseWhellDelta = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_cMouseWhellDelta = 0;
	float   l_fScaleX = cGameApp::m_spOpenGLRender->m_vGameResolution.x/cGameApp::m_spOpenGLRender->m_vViewPortSize.x;
	float   l_fScaleY = cGameApp::m_spOpenGLRender->m_vGameResolution.y/cGameApp::m_spOpenGLRender->m_vViewPortSize.y;	
	switch (message)
	{
	case  WM_SIZE:
		//cGameApp::SetAcceptRationWithGameresolution((int)LOWORD(lParam),(int)HIWORD(lParam));
		g_WindowSize.x = (int)LOWORD(lParam);
		g_WindowSize.y = (int)HIWORD(lParam);
		cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution((int)LOWORD(lParam),(int)HIWORD(lParam),(int)cGameApp::m_spOpenGLRender->m_vGameResolution.x,(int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		//cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)LOWORD(lParam);
		//cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)HIWORD(lParam);
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
	//case WM_MOUSEHWHEEL:
	case WM_MOUSEWHEEL:
		g_cMouseWhellDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	case WM_LBUTTONDOWN:
		g_bLeftMouseDown = true;
		g_bRightMouseDown = false;
		if( g_pGameApp )
			g_pGameApp->MouseDown(g_MousePosition.x,g_MousePosition.y);
			return 0;
	case WM_LBUTTONUP:
		g_bLeftMouseDown = false;
		if( g_pGameApp )
			g_pGameApp->MouseUp(g_MousePosition.x,g_MousePosition.y);
			return 0;
	case WM_RBUTTONDOWN:
		g_bRightMouseDown = true;
		g_bLeftMouseDown = false;
			return 0;
	case WM_RBUTTONUP:
		g_bRightMouseDown = false;
			return 0;
	case WM_MOUSEMOVE:
		g_MousePosition.x = (int)(GET_X_LPARAM(lParam));
		g_MousePosition.y = (int)(GET_Y_LPARAM(lParam));
		g_pGameApp->m_sMousePosition = g_MousePosition;
		if( g_pGameApp )
			g_pGameApp->MouseMove(g_MousePosition.x,g_MousePosition.y);

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}