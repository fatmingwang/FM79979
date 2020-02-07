//
// OpenGLUserControl.xaml.cpp
// Implementation of the OpenGLUserControl class
//

#include "pch.h"
#include "OpenGLUserControl.xaml.h"

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
namespace UWP_Angle_EmptyProject
{
	OpenGLUserControl::OpenGLUserControl()
	{
		InitializeComponent();
		m_pOpenGLES = nullptr;
		m_pOpenGLES = new OpenGLES();
		//this->InitGL();
		this->Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &OpenGLUserControl::OnPageLoaded);
	}

	OpenGLUserControl::~OpenGLUserControl()
	{
		StopRenderLoop();
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
				auto l_Error = glGetError();
				if (m_pOpenGLES)
					m_pOpenGLES->MakeCurrent(m_RenderSurface);
				l_Error = glGetError();
				EGLint panelWidth = 0;
				EGLint panelHeight = 0;
				if (m_pOpenGLES)
					m_pOpenGLES->GetSurfaceDimensions(m_RenderSurface, &panelWidth, &panelHeight);
				l_Error = glGetError();
				if (g_pGameApp)
				{
					g_pGameApp->Run();
					glViewport(0, 0, panelWidth, panelHeight);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glClearColor(1, 0, 0, 1);
				}
				glEnable2D(1920, 1080);
				if (g_pMPDI)
				{
					g_pMPDI->Update(0.016f);
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