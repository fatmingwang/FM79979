
//
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
//#define DBG_NEW new
#endif
#include "stdafx.h"
#include "test.h"
#include "GameApp.h"

#include "../../AllLibInclude.h"
#include "Windowsx.h"
// test.cpp : Defines the entry point for the application.
#include <corecrt_io.h>
#include <fcntl.h>
#include "../../Core/FMgltf/glTF.h"
//#define VLD_DEBUG
//
//#ifdef VLD_DEBUG
////https://vld.codeplex.com/wikipage?title=Using%20Visual%20Leak%20Detector&referringTitle=Documentation
//#include "../../../include/vld.h"
//#pragma comment(lib, "../../../lib/vld.lib")
////#pragma comment(lib, "C:/Program Files (x86)/Visual Leak Detector/lib/Win32/vld.lib")
//#endif

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
cGameApp*g_pGameApp = nullptr;

//#define offsetof(s,m) ((::size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	//auto l_Value1 = offsetof(ImDrawVert, pos);
	//auto l_Value2 = offsetof(ImDrawVert, uv);
	//auto l_Value3 = offsetof(ImDrawVert, col);
	//glTF::Asset l_Asset;
	//l_Asset.Parse(L"C:/Users/Fatming/Desktop/DirectX-Graphics-Samples-master/DirectX-Graphics-Samples-master/Samples/Desktop/D3D12Raytracing/src/D3D12RaytracingMiniEngineSample/Sponza/pbr/sponza2.gltf");
	for (int i = 0; i < 100; ++i)
	{
		int gl_VertexID = i;
		int particleID = gl_VertexID >> 2; // 4 vertices per particle
		//vec4 particlePos = pos[particleID];

		//Out.color = vec4(0.5, 0.2, 0.1, 1.0);

		//map vertex ID to quad vertex
		POINT quadPos = { (((gl_VertexID - 1) & 2), (gl_VertexID & 2)) };
		quadPos.x = quadPos.x >> 1;
		quadPos.y = quadPos.y >> 1;
		int a = 0;

	}
	_CrtMemState s1;
	_CrtMemCheckpoint(&s1);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: Place code here.
	MSG msg;
	//HACCEL hAccelTable;
	cGameApp::CreateDefaultOpenGLRender();
	// Initialize global strings
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	swprintf(szTitle, L"FM79979");
	swprintf(szWindowClass, L"FM79979");
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	cGameApp::m_sbDebugFunctionWorking = true;
	if (1)
	{
		g_pGameApp = new cEngineTestApp(g_hWnd, cGameApp::m_spOpenGLRender->m_vGameResolution, Vector2(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height()));
		g_pGameApp->Init();
		cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution((int)g_WindowSize.x, (int)g_WindowSize.y, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	}
	SetTimer (g_hWnd, 0, 0, NULL) ;


    //MouseHook = SetWindowsHookEx(WH_MOUSE_LL,MouseHookProc,hInstance,0);
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)&&!g_bLeave)
	{
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (::IsIconic(g_hWnd))
			{
				::Sleep(10);
				continue;
			}
		}
	}

	ImGui_ImplWin32_Shutdown();

	//UnhookWindowsHookEx(MouseHook);
	SAFE_DELETE(g_pGameApp);
	//SAFE_DELETE(g_pGameApp->m_spOpenGLRender);
	NamedTypedObject::DumpUnReleaseInfo();

	_CrtMemState s2;
	_CrtMemCheckpoint(&s2);
	_CrtMemState s3;
	if (_CrtMemDifference(&s3, &s1, &s2))
	{
		_CrtMemDumpStatistics(&s3);
		_CrtDumpMemoryLeaks();
	}
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
	DWORD	l_dwFlag = WS_OVERLAPPEDWINDOW;
	if (cGameApp::m_spOpenGLRender)
	{
		cNodeISAX	l_NodeISAX;
		cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 1024.;
		cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 768.f;
		cGameApp::ResoluctionParse("EngineTestSetup.xml");
		if (cGameApp::m_sbFullScreen)
			l_dwFlag = WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		g_hWnd = CreateWindow(szWindowClass, szTitle, l_dwFlag, 0, 0, (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height(), NULL, NULL, hInstance, NULL);
	}
	else
	{
		g_hWnd = CreateWindow(szWindowClass, szTitle, l_dwFlag, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);
	}

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
	ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
	float   l_fScaleX = cGameApp::m_spOpenGLRender->m_vGameResolution.x/ cGameApp::m_spOpenGLRender->m_vViewPortSize.x;
	float   l_fScaleY = cGameApp::m_spOpenGLRender->m_vGameResolution.y/ cGameApp::m_spOpenGLRender->m_vViewPortSize.y;
	switch (message)
	{
	case  WM_SIZE:
		g_WindowSize.x = (int)LOWORD(lParam);
		g_WindowSize.y = (int)HIWORD(lParam);
		cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution((int)LOWORD(lParam),(int)HIWORD(lParam),(int)cGameApp::m_spOpenGLRender->m_vGameResolution.x,(int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		break;
	case WM_TIMER:
		if( !g_bLeave && g_pGameApp )
		{
			g_pGameApp->Run();
		}
		else
		{
			SAFE_DELETE(g_pGameApp);
		}
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