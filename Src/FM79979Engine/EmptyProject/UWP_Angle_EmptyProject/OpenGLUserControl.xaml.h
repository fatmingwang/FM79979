//
// OpenGLUserControl.xaml.h
// Declaration of the OpenGLUserControl class
//

#pragma once

#include "OpenGLUserControl.g.h"
#include "OpenGLES.h"
#include <angle_windowsstore.h>

using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Platform;
using Windows::ApplicationModel::SuspendingEventArgs;
using Windows::ApplicationModel::Activation::LaunchActivatedEventArgs;
using Windows::Foundation::Collections::PropertySet;
using Windows::Foundation::EventHandler;

namespace UWP_Angle_EmptyProject
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenGLUserControl sealed
	{
	public:
		OpenGLUserControl();
		virtual ~OpenGLUserControl();
		//void	ScheduleRendering();
		//Platforms::
		property EventHandler<Platform::Object^>^ f_MouseEvent;
	private:

		void OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void CreateRenderSurface();
		void DestroyRenderSurface();
		void RecoverFromLostDevice();
		void StartRenderLoop();
		void StopRenderLoop();

		OpenGLES*			m_pOpenGLES;
		EGLSurface			m_RenderSurface;     // This surface is associated with a swapChainPanel on the page

		Concurrency::critical_section m_RenderSurfaceCriticalSection;
		Windows::Foundation::IAsyncAction^ m_pRenderLoopWorker;
		void swapChainPanel_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void swapChainPanel_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		//void SwapChainPanel_KeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
		//void SwapChainPanel_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);

		void MyKeyDown(CoreWindow^,KeyEventArgs^);
		void MyKeyUp(CoreWindow^, KeyEventArgs^);

	};
}
