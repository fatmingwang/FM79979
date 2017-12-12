//
#include "stdafx.h"
#include "test.h"
#include "GameApp.h"

#include "../../AllLibInclude.h"
#ifdef WASM
cGameApp*g_pGameApp = 0;
int main()
{
	g_pGameApp = new cEngineTestApp(cGameApp::m_svGameResolution, Vector2(cGameApp::m_svViewPortSize.Width(), cGameApp::m_svViewPortSize.Height()));
	g_pGameApp->Init();
	cGameApp::SetAcceptRationWithGameresolution(800,600, (int)cGameApp::m_svGameResolution.x, (int)cGameApp::m_svGameResolution.y);
	while (1)
	{
		g_pGameApp->Run();
	}

	SAFE_DELETE(g_pGameApp);
	return 0;
}


#else
#include "Windowsx.h"
// test.cpp : Defines the entry point for the application.



#define VLD_DEBUG

#ifdef VLD_DEBUG
//https://vld.codeplex.com/wikipage?title=Using%20Visual%20Leak%20Detector&referringTitle=Documentation
#include "../../../include/vld.h"
#pragma comment(lib, "../../../lib/vld.lib")
//#pragma comment(lib, "C:/Program Files (x86)/Visual Leak Detector/lib/Win32/vld.lib")
#endif

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

POINT g_WindowSize;
cGameApp*g_pGameApp = 0;
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
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
	g_pGameApp = new cEngineTestApp(g_hWnd,cGameApp::m_svGameResolution,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
	g_pGameApp->Init();
	cGameApp::SetAcceptRationWithGameresolution((int)g_WindowSize.x,(int)g_WindowSize.y,(int)cGameApp::m_svGameResolution.x,(int)cGameApp::m_svGameResolution.y);
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

	//UnhookWindowsHookEx(MouseHook);
	SAFE_DELETE(g_pGameApp);
	_CrtDumpMemoryLeaks();
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
	cGameApp::m_svViewPortSize.x = 1024.;
	cGameApp::m_svViewPortSize.y = 768.f;
	cGameApp::ResoluctionParse("EngineTestSetup.xml");

	DWORD	l_dwFlag = WS_OVERLAPPEDWINDOW;
	if(cGameApp::m_sbFullScreen)
		l_dwFlag = WS_VISIBLE | WS_POPUP |	WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	g_hWnd = CreateWindow(szWindowClass, szTitle, l_dwFlag, 0, 0, (int)cGameApp::m_svViewPortSize.Width(), (int)cGameApp::m_svViewPortSize.Height(), NULL, NULL, hInstance, NULL);

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
				//g_bLeave = true;
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
#endif