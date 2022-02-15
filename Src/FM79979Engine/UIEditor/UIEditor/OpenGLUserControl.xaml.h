//
// OpenGLUserCOntrol.xaml.h
// Declaration of the OpenGLUserCOntrol class
//

#pragma once

#include "OpenGLUserCOntrol.g.h"
#include "../../Core/CoreUWP/CoreUWP/OpenGLES.h"
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Platform;

using Windows::ApplicationModel::SuspendingEventArgs;
using Windows::ApplicationModel::Activation::LaunchActivatedEventArgs;
using Windows::Foundation::Collections::PropertySet;
using Windows::Foundation::EventHandler;
namespace UIEditor
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenGLUserCOntrol sealed
	{
		bool								m_bMouseDown;
		void								OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void								OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void								CreateRenderSurface();
		void								DestroyRenderSurface();
		void								RecoverFromLostDevice();
		void								StartRenderLoop();
		void								StopRenderLoop();
		//
		OpenGLES*							m_pOpenGLES;
		EGLSurface							m_RenderSurface;     // This surface is associated with a swapChainPanel on the page
		Concurrency::critical_section		m_RenderSurfaceCriticalSection;
		Windows::Foundation::IAsyncAction^	m_pRenderLoopWorker;
	public:
		OpenGLUserCOntrol();
	private:
		void MyKeyDown(CoreWindow^, KeyEventArgs^);
		void MyKeyUp(CoreWindow^, KeyEventArgs^);
		//
		void swapChainPanel_PointerWheelChanged(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void swapChainPanel_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void swapChainPanel_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void swapChainPanel_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void swapChainPanel_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
	};
}
