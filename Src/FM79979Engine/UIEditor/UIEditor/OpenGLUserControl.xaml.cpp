//
// OpenGLUserControl.xaml.cpp
// Implementation of the OpenGLUserControl class
//

#include "pch.h"
#include "OpenGLUserControl.xaml.h"

#include "strsafe.h"


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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

//cCarDeivingServerApp* g_pUWPCarDeivingServerApp = nullptr;
extern std::vector<int>	g_AddFakeCarVector;
namespace UIEditor
{
	OpenGLUserControl::OpenGLUserControl()
	{
		InitializeComponent();
		m_bMouseDown = false;
		m_pOpenGLES = nullptr;
		m_pOpenGLES = new OpenGLES();
		this->Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &OpenGLUserControl::OnPageLoaded);
		auto l_pCoreWindow = Window::Current->CoreWindow;
		l_pCoreWindow->GetForCurrentThread()->KeyDown += ref new Windows::Foundation::TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &OpenGLUserControl::MyKeyDown);
		l_pCoreWindow->GetForCurrentThread()->KeyUp += ref new Windows::Foundation::TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &OpenGLUserControl::MyKeyUp);
		//Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->CharacterReceived += ref new
	//			Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::CharacterReceivedEventArgs^>(this, &OpenGLUserControl::OnCharacterReceived);
	}


	OpenGLUserControl::~OpenGLUserControl()
	{
		StopRenderLoop();
		DestroyRenderSurface();
		if (m_pOpenGLES)
		{
			delete m_pOpenGLES;
		}
		//SAFE_DELETE(g_pUWPCarDeivingServerApp);

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
		{
			m_pOpenGLES->DestroySurface(m_RenderSurface);
		}
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
		auto workItemHandler = ref new Windows::System::Threading::WorkItemHandler([this](Windows::Foundation::IAsyncAction^ action)
			{
				critical_section::scoped_lock lock(m_RenderSurfaceCriticalSection);

				while (action->Status == Windows::Foundation::AsyncStatus::Started)
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
					//if (!g_pUWPCarDeivingServerApp)
					//{
					//	if (m_pOpenGLES)
					//		m_pOpenGLES->MakeCurrent(m_RenderSurface);
					//	cCarDeivingServerApp::m_sstrCompileInfo = GetCompileInfo();
					//	g_pUWPCarDeivingServerApp = new cCarDeivingServerApp(nullptr, Vector2(1920, 1080), Vector2(1920, 1080));
					//	g_pUWPCarDeivingServerApp->Init();
					//	g_pUWPServerCarDrivingRouteManager = new cCarDrivingRouteManager();
					//	f_CarChangeNotifierFunction l_f_CarChangeNotifierFunction =
					//		[this](std::vector<int> e_HWIDVector)
					//	{
					//		//auto l_pCoreWindow = Window::Current->CoreWindow;
					//		m_pNavPage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal,
					//			ref new Windows::UI::Core::DispatchedHandler([this, e_HWIDVector]()
					//				{
					//					if (m_pNavPage)
					//					{
					//						Platform::Array<int>^ l_Vector = ref new Platform::Array<int>((unsigned int)e_HWIDVector.size());
					//						for (size_t i = 0; i < e_HWIDVector.size(); ++i)
					//						{
					//							l_Vector->set((unsigned int)i, e_HWIDVector[i]);
					//						}
					//						m_pNavPage->CarHWIDChangeNotifier(l_Vector);
					//					}
					//				})
					//		);
					//		//if(m_pNavPage)
					//		//	m_pNavPage->CarHWIDChangeNotifier();
					//	};
					//	g_pUWPCarDeivingServerApp->SetCarChangeNotifier(l_f_CarChangeNotifierFunction);
					//	if (g_pUWPCarDeivingServerApp && g_pUWPCarDeivingServerApp->m_pOwnCarDrivingNodeManager)
					//	{
					//		int l_iCount = g_pUWPCarDeivingServerApp->m_pOwnCarDrivingNodeManager->Count();
					//		Platform::Array<String^>^ l_Vector = ref new Platform::Array<String^>(l_iCount);
					//		for (int i = 0; i < l_iCount; i++)
					//		{
					//			auto l_pObject = g_pUWPCarDeivingServerApp->m_pOwnCarDrivingNodeManager->GetObject(i);
					//			if (l_pObject)
					//			{
					//				String^ l_strNodeName = ref new String(l_pObject->GetName());
					//				l_Vector->set(i, l_strNodeName);
					//			}
					//		}
					//		m_pNavPage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal,
					//			ref new Windows::UI::Core::DispatchedHandler([this, l_Vector]()
					//				{
					//					this->m_pNavPage->SetAllNodesName(l_Vector);
					//				})
					//		);

					//	}
					//}
					//else
					//if (g_pUWPCarDeivingServerApp)
					//{
					//	if (g_AddFakeCarVector.size())
					//	{
					//		for (size_t i = 0; i < g_AddFakeCarVector.size(); ++i)
					//		{
					//			g_pUWPCarDeivingServerApp->m_pOwnCarManager->AddAFakeCar((int)i + 1);
					//		}
					//		g_AddFakeCarVector.clear();
					//	}

					//	//glViewport(0, 0, panelWidth, panelHeight);
					//	//glEnable(GL_BLEND);
					//	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					//	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					//	//glClearColor(0.1f, 0.1f, 0.1f, 1.f);
					//	g_pUWPCarDeivingServerApp->Run();
					//	cGameApp::m_spOpenGLRender->m_vViewPortSize = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize = Vector4(0.f, 0.f, (float)panelWidth, (float)panelHeight);
					//	cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution(panelWidth, panelHeight, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
					//}
					// The call to eglSwapBuffers might not be successful (i.e. due to Device Lost)
					// If the call fails, then we must reinitialize EGL and the GL resources.
					//if (g_pUWPCarDeivingServerApp && g_pUWPCarDeivingServerApp->m_bDoRender)
					{
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

	//https://docs.microsoft.com/en-us/uwp/api/windows.ui.input.pointerpoint?view=winrt-19041
	void OpenGLUserControl::swapChainPanel_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
	{
		//if (!g_pUWPCarDeivingServerApp)
		//{
		//	return;
		//}
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
			if (!m_bMouseDown)
			{
				//g_pUWPCarDeivingServerApp->MouseDown((int)ptrPt->Position.X, (int)ptrPt->Position.Y);
			}
			else
			{
				//g_pUWPCarDeivingServerApp->MouseMove((int)ptrPt->Position.X, (int)ptrPt->Position.Y);
			}
			if (ptrPt->Properties->IsLeftButtonPressed)
			{
				//eventLog.Text += "\nLeft button: " + ptrPt.PointerId;
				//l_str = "\nLeft button: " + ptrPt->PointerId;
				l_bLeft = true;
				m_bMouseDown = true;
			}
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
			if (Window::Current->CoreWindow->GetKeyState(Windows::System::VirtualKey::Control) == Windows::UI::Core::CoreVirtualKeyStates::Down)
			{
				l_iWheel *= 10;
			}
			//g_pOrthogonalCamera->CameraUpdateByMouse(l_bLeft, l_bRight, l_iWheel, (int)ptrPt->Position.X, (int)ptrPt->Position.Y,
				//Vector2(cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.Height()), 1.f);
			//auto l_vMousePos = g_pOrthogonalCamera->GetMouseWorldPos();
			//cGameApp::m_sMousePosition.x = (long)l_vMousePos.x;
			//cGameApp::m_sMousePosition.y = (long)l_vMousePos.y;
			//if (g_pUWPCarDeivingServerApp && g_pUWPCarDeivingServerApp->m_pOrthogonalCamera)
			//{
			//	cOrthogonalCamera* l_pOrthogonalCamera = g_pUWPCarDeivingServerApp->m_pOrthogonalCamera;
			//	POINT	ptCursor = { (int)l_pOrthogonalCamera->GetMouseWorldPos().x,(int)l_pOrthogonalCamera->GetMouseWorldPos().y };
			//	l_pOrthogonalCamera->CameraUpdateByMouse(l_bLeft, l_bRight, l_iWheel, (int)ptrPt->Position.X, (int)ptrPt->Position.Y,
			//		Vector2(g_pUWPCarDeivingServerApp->m_spOpenGLRender->m_vViewPortSize.Width(), g_pUWPCarDeivingServerApp->m_spOpenGLRender->m_vViewPortSize.Height()));
			//}
		}

		// Prevent most handlers along the event route from handling the same event again.
		e->Handled = true;
		//swapChainPanel->Focus(FocusState::Programmatic);
		//swapChainPanel->FocusVisualMargin()
		// Display pointer details.
		//updateInfoPop(e);
	}


	void OpenGLUserControl::swapChainPanel_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
	{
		Windows::UI::Input::PointerPoint^ ptrPt = e->GetCurrentPoint(swapChainPanel);
		//if (g_pUWPCarDeivingServerApp)
		//{
		//	g_pUWPCarDeivingServerApp->MouseUp((int)ptrPt->Position.X, (int)ptrPt->Position.Y);
		//	m_bMouseDown = false;
		//}
	}


	//void OpenGLUserControl::SwapChainPanel_KeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
	//{
	//	if (g_pUWPCarDeivingServerApp)
	//	{
	//		g_pUWPCarDeivingServerApp->KeyDown((char)e->Key);
	//	}
	//}
	//
	//
	//void OpenGLUserControl::SwapChainPanel_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
	//{
	//	if (g_pUWPCarDeivingServerApp)
	//	{
	//		g_pUWPCarDeivingServerApp->KeyUp((char)e->Key);
	//	}
	//}

	//void OpenGLUserControl::OnCharacterReceived(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CharacterReceivedEventArgs^ args)
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
	//		g_pUWPCarDeivingServerApp->KeyUp((char)keycode);
	//	}
	//	else
	//	if(args->KeyStatus.WasKeyDown)
	//	{
	//		g_pUWPCarDeivingServerApp->KeyDown((char)keycode);
	//	}
	//}

	void OpenGLUserControl::MyKeyDown(CoreWindow^ sender, KeyEventArgs^ e)
	{
		//if (g_pUWPCarDeivingServerApp)
			//g_pUWPCarDeivingServerApp->KeyDown((char)e->VirtualKey);

	}
	void OpenGLUserControl::MyKeyUp(CoreWindow^ sender, KeyEventArgs^ e)
	{
		//if (g_pUWPCarDeivingServerApp)
		//	g_pUWPCarDeivingServerApp->KeyUp((char)e->VirtualKey);

	}
	void OpenGLUserControl::swapChainPanel_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
	{
		//if (g_pUWPCarDeivingServerApp && g_pUWPCarDeivingServerApp->m_pOrthogonalCamera)
		//{
		//	cOrthogonalCamera* l_pOrthogonalCamera = g_pUWPCarDeivingServerApp->m_pOrthogonalCamera;
		//	POINT l_ViewportSize = { (long)e->NewSize.Width,
		//		(long)e->NewSize.Height };
		//	Vector2	l_vOriginalResolution = l_pOrthogonalCamera->GetResolution();
		//	Vector2	l_vOriginalLeftUpPos = l_pOrthogonalCamera->GetCameraPos() - (l_vOriginalResolution / 2.f);
		//	l_pOrthogonalCamera->SetResolution(Vector2((float)l_ViewportSize.x, (float)l_ViewportSize.y));
		//	l_pOrthogonalCamera->SetCameraPos(l_vOriginalLeftUpPos + l_pOrthogonalCamera->GetResolution() / 2.f);
		//	l_pOrthogonalCamera->CameraUpdateByMouse(false, false, 0, 0, 0, Vector2((float)l_ViewportSize.x, (float)l_ViewportSize.y));
		//}
	}
	void OpenGLUserControl::swapChainPanel_PointerWheelChanged(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
	{

	}

	void OpenGLUserControl::swapChainPanel_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
	{

	}

	//end namespace UWPServer
}