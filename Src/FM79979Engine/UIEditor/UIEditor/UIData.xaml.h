//
// UIData.xaml.h
// Declaration of the UIData class
//

#pragma once

#include "UIData.g.h"

namespace UIEditor
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class UIData sealed
	{
	public:
		UIData();
	private:
		void ComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	};
}
