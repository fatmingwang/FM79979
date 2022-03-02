//
// UpAndDownNavpage.xaml.h
// Declaration of the UpAndDownNavpage class
//

#pragma once

#include "UpAndDownNavpage.g.h"
#include "OpenGLUserControl.xaml.h"
//https://docs.microsoft.com/en-us/windows/apps/design/controls/navigationview
namespace UIEditor
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class UpAndDownNavpage sealed
	{
		OpenGLUserControl^ m_OpenGLUserControl;
	public:
		UpAndDownNavpage();
	private:
		void m_NavigationView_SelectionChanged(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs^ args);
		void NavigationViewItem_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Grid_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
