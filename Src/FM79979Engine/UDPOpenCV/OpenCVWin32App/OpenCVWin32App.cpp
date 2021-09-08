// OpenCVWin32App.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include <corecrt_io.h>
#include "OpenCVWin32App.h"
#include "../../Core/AllCoreInclude.h"
#include "../../AllLibInclude.h"
#include "../Win32UDPOpenCV/UDPOpenCVApp.h"
#include "windowsx.h"

#ifdef DEBUG
#pragma comment(lib, "../../../lib/opencv_world440d.lib")
#else
#pragma comment(lib, "../../../lib/opencv_world440.lib")
#endif

cGameApp* g_pGameApp = 0;
HWND g_hWnd = nullptr;
bool	g_bLeave = false;
POINT g_WindowSize;
POINT g_MousePosition;


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OPENCVWIN32APP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    if (g_hWnd && cGameApp::m_spOpenGLRender)
    {
        g_pGameApp = new cUDPOpenCVApp(g_hWnd, cGameApp::m_spOpenGLRender->m_vGameResolution, Vector2(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height()));
        g_pGameApp->Init();
        cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution((int)g_WindowSize.x, (int)g_WindowSize.y, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENCVWIN32APP));

    MSG msg;
    SetTimer(g_hWnd, 0, 0, NULL);
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OPENCVWIN32APP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OPENCVWIN32APP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
struct MonitorRects
{
    std::vector<RECT>   rcMonitors;

    static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
    {
        MonitorRects* pThis = reinterpret_cast<MonitorRects*>(pData);
        pThis->rcMonitors.push_back(*lprcMonitor);
        return TRUE;
    }

    MonitorRects()
    {
        EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)this);
    }
};
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
    bool	l_bFullScreen = false;
    bool l_bCreate = true;
    if (l_bCreate)
    {
        cNodeISAX	l_NodeISAX;
        cGameApp::CreateDefaultOpenGLRender();
        cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 0;
        cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 0;
        cGameApp::m_spOpenGLRender->m_vViewPortSize.z = 1024.;
        cGameApp::m_spOpenGLRender->m_vViewPortSize.w = 768.f;
        cGameApp::ResoluctionParse("CarGoApp/CarGoAppSetup.xml");
    }

    DWORD	l_dwFlag = WS_OVERLAPPEDWINDOW;
    if (cGameApp::m_sbFullScreen)
        l_dwFlag = WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    MonitorRects l_MonitorRects;
    int l_iStartPosX = 0;
    int l_iStartPosY = 0;
    int l_iWidth = 800;
    int l_iHeight = 600;
    if (l_bCreate)
    {
        int l_iWidth = (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width();
        int l_iHeight = (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height();
        if (cGameApp::m_sbFullScreen)
        {
            l_iWidth = l_MonitorRects.rcMonitors[0].right;
            l_iHeight = l_MonitorRects.rcMonitors[0].bottom;
        }
        if (l_MonitorRects.rcMonitors.size() > 1)
        {
            l_iStartPosX = l_MonitorRects.rcMonitors[1].left;
            l_iStartPosY = l_MonitorRects.rcMonitors[1].top;
            if (cGameApp::m_sbFullScreen)
            {
                l_iWidth = l_MonitorRects.rcMonitors[1].right - l_iStartPosX;
                l_iHeight = l_MonitorRects.rcMonitors[1].bottom - l_iStartPosY;
            }
        }
        cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)l_iWidth;
        cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)l_iHeight;
    }
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   g_hWnd = hWnd;
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case  WM_SIZE:
        g_WindowSize.x = (int)LOWORD(lParam);
        g_WindowSize.y = (int)HIWORD(lParam);
        if (cGameApp::m_spOpenGLRender)
            cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution((int)LOWORD(lParam), (int)HIWORD(lParam), (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
        break;
    case WM_TIMER:
        if (!g_bLeave && g_pGameApp)
        {
            g_pGameApp->Run();
#ifdef WIN32
            SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
            Sleep(1);
        }
        else
        {
            SAFE_DELETE(g_pGameApp);
        }
        break;
    case WM_KEYDOWN:
        if (g_pGameApp)
        {
            g_pGameApp->KeyDown((char)LOWORD(wParam));
        }
        switch (LOWORD(wParam))
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
        if (g_pGameApp)
        {
            g_pGameApp->KeyUp((char)LOWORD(wParam));
        }
        break;
    case WM_MOUSEWHEEL:
        cGameApp::m_sMouseWhellDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        break;
    case WM_LBUTTONDOWN:
        if (g_pGameApp)
        {
            g_pGameApp->MouseDown(g_MousePosition.x, g_MousePosition.y);
            g_pGameApp->MouseButtonClickEvent(true, 0);
        }
        return 0;
    case WM_LBUTTONUP:
        if (g_pGameApp)
        {
            g_pGameApp->MouseUp(g_MousePosition.x, g_MousePosition.y);
            g_pGameApp->MouseButtonClickEvent(false, 0);
        }
        return 0;
    case WM_MOUSEMOVE:
        g_MousePosition.x = (int)(GET_X_LPARAM(lParam));
        g_MousePosition.y = (int)(GET_Y_LPARAM(lParam));
        g_pGameApp->m_sMousePosition = g_MousePosition;
        if (g_pGameApp)
            g_pGameApp->MouseMove(g_MousePosition.x, g_MousePosition.y);

    case WM_MBUTTONDOWN:
        if (g_pGameApp)
            g_pGameApp->MouseButtonClickEvent(true, 2);
        break;
    case WM_MBUTTONUP:
        if (g_pGameApp)
            g_pGameApp->MouseButtonClickEvent(false, 2);
        break;
    case WM_RBUTTONDOWN:
        if (g_pGameApp)
            g_pGameApp->MouseButtonClickEvent(true, 1);
        break;
    case WM_RBUTTONUP:
        if (g_pGameApp)
            g_pGameApp->MouseButtonClickEvent(false, 1);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
