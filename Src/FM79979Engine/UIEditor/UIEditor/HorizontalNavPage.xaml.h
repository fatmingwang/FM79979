//
// HorizontalNavPage.xaml.h
// Declaration of the HorizontalNavPage class
//

#pragma once

#include "HorizontalNavPage.g.h"
#include "OpenGLUserControl.xaml.h"

namespace UIEditor
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HorizontalNavPage sealed
	{
		OpenGLUserControl^m_OpenGLUserControl;
	public:
		HorizontalNavPage();
	private:
		void NavView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void NavView_KeyUp(CoreWindow^ sender, KeyEventArgs^ e);
	};
}
