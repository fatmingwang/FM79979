//
// TopNavPage.xaml.h
// Declaration of the TopNavPage class
//

#pragma once

#include "TopNavPage.g.h"
#include "HorizontalNavPage.xaml.h"

namespace UIEditor
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class TopNavPage sealed
	{
		HorizontalNavPage^ m_pHorizontalNavPage;
	public:
		TopNavPage();
	private:
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void m_NavigationView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
