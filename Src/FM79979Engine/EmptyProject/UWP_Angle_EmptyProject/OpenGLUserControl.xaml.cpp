﻿//
// OpenGLUserControl.xaml.cpp
// Implementation of the OpenGLUserControl class
//

#include "pch.h"
#include "OpenGLUserControl.xaml.h"
#include "NavPage.xaml.h"
#include "strsafe.h"
using namespace UWP_Angle_EmptyProject;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::System::Threading;
using namespace Windows::Storage;
using namespace Concurrency;
// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

cMPDI*g_pMPDI = nullptr;
cGameApp*g_pGameApp = nullptr;
cOrthogonalCamera* g_pOrthogonalCamera = nullptr;
namespace UWP_Angle_EmptyProject
{
	OpenGLUserControl::OpenGLUserControl()
	{
		InitializeComponent();
		m_pOpenGLES = nullptr;
		m_pOpenGLES = new OpenGLES();
		g_pOrthogonalCamera = new cOrthogonalCamera();
		this->Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &OpenGLUserControl::OnPageLoaded);
		auto l_pCoreWindow = Window::Current->CoreWindow;
		l_pCoreWindow->GetForCurrentThread()->KeyDown += ref new Windows::Foundation::TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &OpenGLUserControl::MyKeyDown);
		l_pCoreWindow->GetForCurrentThread()->KeyUp += ref new Windows::Foundation::TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &OpenGLUserControl::MyKeyUp);
		//Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->CharacterReceived += ref new
//			Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::CharacterReceivedEventArgs^>(this, &UWP_Angle_EmptyProject::OpenGLUserControl::OnCharacterReceived);
	}

	OpenGLUserControl::~OpenGLUserControl()
	{
		StopRenderLoop();
		SAFE_DELETE(g_pGameApp);
		DestroyRenderSurface();
		if (m_pOpenGLES)
			delete m_pOpenGLES;
	}

	void OpenGLUserControl::OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
	{
		// The SwapChainPanel has been created and arranged in the page layout, so EGL can be initialized.
		CreateRenderSurface();
		StartRenderLoop();
		auto l_pCoreWindow = Window::Current->CoreWindow;
		l_pCoreWindow->VisibilityChanged += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::VisibilityChangedEventArgs^>(this, &OpenGLUserControl::OnVisibilityChanged);
	}

	void OpenGLUserControl::OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
	{
		if (!g_pMPDI)
			return;
		if (args->Visible && m_RenderSurface != EGL_NO_SURFACE)
		{
			StartRenderLoop();
		}
		else
		{
			StopRenderLoop();
		}
	}

	void OpenGLUserControl::CreateRenderSurface()
	{

		if (m_RenderSurface == EGL_NO_SURFACE)
		{
			// The app can configure the the SwapChainPanel which may boost performance.
			// By default, this template uses the default configuration.
			if (m_pOpenGLES)
			{
				m_pOpenGLES->Initialize();
				m_RenderSurface = m_pOpenGLES->CreateSurface(swapChainPanel, nullptr, nullptr);
			}

			// You can configure the SwapChainPanel to render at a lower resolution and be scaled up to
			// the swapchain panel size. This scaling is often free on mobile hardware.
			//
			// One way to configure the SwapChainPanel is to specify precisely which resolution it should render at.
			// Size customRenderSurfaceSize = Size(800, 600);
			// mRenderSurface = mOpenGLES->CreateSurface(swapChainPanel, &customRenderSurfaceSize, nullptr);
			//
			// Another way is to tell the SwapChainPanel to render at a certain scale factor compared to its size.
			// e.g. if the SwapChainPanel is 1920x1280 then setting a factor of 0.5f will make the app render at 960x640
			// float customResolutionScale = 0.5f;
			// mRenderSurface = mOpenGLES->CreateSurface(swapChainPanel, nullptr, &customResolutionScale);
			// 
		}
	}

	void OpenGLUserControl::DestroyRenderSurface()
	{
		if (m_pOpenGLES)
			m_pOpenGLES->DestroySurface(m_RenderSurface);
		m_RenderSurface = EGL_NO_SURFACE;
	}

	void OpenGLUserControl::RecoverFromLostDevice()
	{
		// Stop the render loop, reset OpenGLES, recreate the render surface
		// and start the render loop again to recover from a lost device.

		StopRenderLoop();

		{
			critical_section::scoped_lock lock(m_RenderSurfaceCriticalSection);

			DestroyRenderSurface();
			if (m_pOpenGLES)
				m_pOpenGLES->Reset();
			CreateRenderSurface();
		}

		StartRenderLoop();
	}

	void OpenGLUserControl::StartRenderLoop()
	{
		// If the render loop is already running then do not start another thread.
		if (m_pRenderLoopWorker != nullptr && m_pRenderLoopWorker->Status == Windows::Foundation::AsyncStatus::Started)
		{
			return;
		}

		// Create a task for rendering that will be run on a background thread.
		auto workItemHandler = ref new Windows::System::Threading::WorkItemHandler([this](Windows::Foundation::IAsyncAction ^ action)
		{
			critical_section::scoped_lock lock(m_RenderSurfaceCriticalSection);

			while (action->Status == Windows::Foundation::AsyncStatus::Started)
			{
				if (0)
				{
					auto l_Error = glGetError();
					if (m_pOpenGLES)
						m_pOpenGLES->MakeCurrent(m_RenderSurface);
					l_Error = glGetError();
					EGLint panelWidth = 0;
					EGLint panelHeight = 0;
					if (m_pOpenGLES)
					{
						m_pOpenGLES->GetSurfaceDimensions(m_RenderSurface, &panelWidth, &panelHeight);
					}
					l_Error = glGetError();
					//some network things need to do after all UI created or show no internet conneciton found
					if (!g_pGameApp)
					{
						if (m_pOpenGLES)
							m_pOpenGLES->MakeCurrent(m_RenderSurface);
						g_pGameApp = new cGameApp(nullptr);
						g_pGameApp->Init();
						auto l_pMPDILIst = cGameApp::GetMPDIListByFileName(L"MPDI/bgrounda01.mpdi");
						if (l_pMPDILIst)
						{
							//g_pMPDI = l_pMPDILIst->GetObject(L"SelectCustomerPointPage");
							g_pMPDI = l_pMPDILIst->GetObject(0);
							if (g_pMPDI)
								g_pMPDI->Init();
						}
					}
					else
					{
						//glViewport(0, 0, panelWidth, panelHeight);
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						glClearColor(0.1f, 0.1f, 0.1f, 1.f);
						g_pGameApp->Run();
						//cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution(panelWidth, panelHeight, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
						cGameApp::m_spOpenGLRender->m_vViewPortSize = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize = Vector4(0.f, 0.f, (float)panelWidth, (float)panelHeight);
					}

					//glEnable2D(panelWidth, panelHeight);
					if (g_pOrthogonalCamera)
					{
						g_pOrthogonalCamera->SetResolution(Vector2(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height()));
						g_pOrthogonalCamera->Render();
						g_pOrthogonalCamera->DrawGrid();
					}
					if (g_pMPDI)
					{
						g_pMPDI->Update(g_pGameApp->m_sTimeAndFPS.fElpaseTime);
						g_pMPDI->Render();
						g_pGameApp->ShowInfo();
					}
					// The call to eglSwapBuffers might not be successful (i.e. due to Device Lost)
					// If the call fails, then we must reinitialize EGL and the GL resources.
					if (m_pOpenGLES && m_pOpenGLES->SwapBuffers(m_RenderSurface) != GL_TRUE)
					{
						// XAML objects like the SwapChainPanel must only be manipulated on the UI thread.
						swapChainPanel->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([=]()
							{
								RecoverFromLostDevice();
							}, CallbackContext::Any));

						return;
					}
				}
			}
		});

		// Run task on a dedicated high priority background thread.
		m_pRenderLoopWorker = Windows::System::Threading::ThreadPool::RunAsync(workItemHandler, Windows::System::Threading::WorkItemPriority::High, Windows::System::Threading::WorkItemOptions::TimeSliced);
	}

	void OpenGLUserControl::StopRenderLoop()
	{
		if (m_pRenderLoopWorker)
		{
			m_pRenderLoopWorker->Cancel();
			m_pRenderLoopWorker = nullptr;
		}
	}

	EGLSurface CreateSurface(EGLDisplay e_pDisplay, EGLConfig e_pConfig, SwapChainPanel^ panel, const Size* renderSurfaceSize, const float* resolutionScale)
	{
		if (!panel)
		{
			throw Exception::CreateException(E_INVALIDARG, L"SwapChainPanel parameter is invalid");
		}

		if (renderSurfaceSize != nullptr && resolutionScale != nullptr)
		{
			throw Exception::CreateException(E_INVALIDARG, L"A size and a scale can't both be specified");
		}

		EGLSurface surface = EGL_NO_SURFACE;

		//const EGLint surfaceAttributes[] =
		//{
		//	// EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER is part of the same optimization as EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER (see above).
		//	// If you have compilation issues with it then please update your Visual Studio templates.
		//	EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER, EGL_TRUE,
		//	EGL_NONE
		//};

		// Create a PropertySet and initialize with the EGLNativeWindowType.
		PropertySet^ surfaceCreationProperties = ref new PropertySet();
		surfaceCreationProperties->Insert(ref new String(EGLNativeWindowTypeProperty), panel);

		// If a render surface size is specified, add it to the surface creation properties
		if (renderSurfaceSize != nullptr)
		{
			surfaceCreationProperties->Insert(ref new String(EGLRenderSurfaceSizeProperty), PropertyValue::CreateSize(*renderSurfaceSize));
		}

		// If a resolution scale is specified, add it to the surface creation properties
		if (resolutionScale != nullptr)
		{
			surfaceCreationProperties->Insert(ref new String(EGLRenderResolutionScaleProperty), PropertyValue::CreateSingle(*resolutionScale));
		}

		surface = eglCreateWindowSurface(e_pDisplay, e_pConfig, reinterpret_cast<IInspectable*>(surfaceCreationProperties), nullptr);
		if (surface == EGL_NO_SURFACE)
		{
			throw Exception::CreateException(E_FAIL, L"Failed to create EGL surface");
		}

		return surface;
	}
}
//https://docs.microsoft.com/en-us/uwp/api/windows.ui.input.pointerpoint?view=winrt-19041
void UWP_Angle_EmptyProject::OpenGLUserControl::swapChainPanel_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (!g_pGameApp)
		return;
	Windows::UI::Xaml::Input::Pointer^ ptr = e->Pointer;
	// Multiple, simultaneous mouse button inputs are processed here.
	// Mouse input is associated with a single pointer assigned when 
	// mouse input is first detected. 
	// Clicking additional mouse buttons (left, wheel, or right) during 
	// the interaction creates secondary associations between those buttons 
	// and the pointer through the pointer pressed event. 
	// The pointer released event is fired only when the last mouse button 
	// associated with the interaction (not necessarily the initial button) 
	// is released. 
	// Because of this exclusive association, other mouse button clicks are 
	// routed through the pointer move event.          
	//if (ptr->PointerDeviceType == Windows::Devices::Input::PointerDeviceType::Mouse)
	{
		// To get mouse state, we need extended pointer details.
		// We get the pointer info through the getCurrentPoint method
		// of the event argument. 
		if (f_MouseEvent != nullptr)
		{
			f_MouseEvent(this, e);
		}
		//String^ l_str = "";
		Windows::UI::Input::PointerPoint^ ptrPt = e->GetCurrentPoint(swapChainPanel);
		//Windows::Devices::Input::PointerDeviceType::Mouse;
		//e->Pointer->PointerDeviceType;
		bool l_bLeft = false;
		bool l_bRight = false;
		int	 l_iWheel = ptrPt->Properties->MouseWheelDelta;
		if (ptrPt->Properties->IsLeftButtonPressed)
		{
			//eventLog.Text += "\nLeft button: " + ptrPt.PointerId;
			//l_str = "\nLeft button: " + ptrPt->PointerId;
			g_pGameApp->MouseDown((int)ptrPt->Position.X, (int)ptrPt->Position.Y);
			l_bLeft = true;
		}
		else
		if (ptrPt->Properties->IsMiddleButtonPressed)
		{
			//eventLog.Text += "\nWheel button: " + ptrPt.PointerId;
			//l_str = "\nWheel button: " + ptrPt->PointerId;
		}
		else
		if (ptrPt->Properties->IsRightButtonPressed)
		{
			//eventLog.Text += "\nRight button: " + ptrPt.PointerId;
			//l_str = "\nRight button: " + ptrPt->PointerId;
			l_bRight = true;
		}
		else
		{
			g_pGameApp->MouseMove((int)ptrPt->Position.X, (int)ptrPt->Position.Y);
		}
		if(Window::Current->CoreWindow->GetKeyState(Windows::System::VirtualKey::Control) == Windows::UI::Core::CoreVirtualKeyStates::Down)
		{
			l_iWheel *= 10;
		}

		g_pOrthogonalCamera->CameraUpdateByMouse(l_bLeft, l_bRight, l_iWheel,(int)ptrPt->Position.X, (int)ptrPt->Position.Y,
			Vector2(cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.Height()),1.f);
		auto l_vMousePos = g_pOrthogonalCamera->GetMouseWorldPos();
		cGameApp::m_sMousePosition.x = (long)l_vMousePos.x;
		cGameApp::m_sMousePosition.y = (long)l_vMousePos.y;
	}

	// Prevent most handlers along the event route from handling the same event again.
	e->Handled = true;
	//swapChainPanel->Focus(FocusState::Programmatic);
	//swapChainPanel->FocusVisualMargin()
	// Display pointer details.
	//updateInfoPop(e);
}


void UWP_Angle_EmptyProject::OpenGLUserControl::swapChainPanel_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Windows::UI::Input::PointerPoint^ ptrPt = e->GetCurrentPoint(swapChainPanel);
	if (g_pGameApp)
	{
		g_pGameApp->MouseUp((int)ptrPt->Position.X, (int)ptrPt->Position.Y);
	}
}


//void UWP_Angle_EmptyProject::OpenGLUserControl::SwapChainPanel_KeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
//{
//	if (g_pGameApp)
//	{
//		g_pGameApp->KeyDown((char)e->Key);
//	}
//}
//
//
//void UWP_Angle_EmptyProject::OpenGLUserControl::SwapChainPanel_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
//{
//	if (g_pGameApp)
//	{
//		g_pGameApp->KeyUp((char)e->Key);
//	}
//}

//void UWP_Angle_EmptyProject::OpenGLUserControl::OnCharacterReceived(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CharacterReceivedEventArgs^ args)
//{
//	bool iskey = false;
//	int keycode = args->KeyCode;
//	auto l_ScanCode = args->KeyStatus.ScanCode;
//	TCHAR buf[1024];
//	size_t cbDest = 1024 * sizeof(TCHAR);
//	StringCbPrintf(buf, cbDest, TEXT("frequency = %d"), l_ScanCode);
//	OutputDebugString(buf);
//	if (args->KeyStatus.IsKeyReleased)
//	{
//		g_pGameApp->KeyUp((char)keycode);
//	}
//	else
//	if(args->KeyStatus.WasKeyDown)
//	{
//		g_pGameApp->KeyDown((char)keycode);
//	}
//}

void UWP_Angle_EmptyProject::OpenGLUserControl::MyKeyDown(CoreWindow^sender, KeyEventArgs^e)
{
	if(g_pGameApp)
		g_pGameApp->KeyDown((char)e->VirtualKey);
	
}
void UWP_Angle_EmptyProject::OpenGLUserControl::MyKeyUp(CoreWindow^ sender, KeyEventArgs^ e)
{
	if (g_pGameApp)
		g_pGameApp->KeyUp((char)e->VirtualKey);

}