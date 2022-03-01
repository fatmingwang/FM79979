//
// NavPage.xaml.h
// Declaration of the NavPage class
//

#pragma once

#include "NavPage.g.h"
//https://docs.microsoft.com/en-us/windows/apps/design/controls/navigationview
namespace UIEditor
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class NavPage sealed
	{
	public:
		NavPage();
	private:
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
