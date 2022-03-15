//
// UITreeView.xaml.h
// Declaration of the UITreeView class
//

#pragma once

#include "UITreeView.g.h"

namespace UIEditor
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class UITreeView sealed
	{
	public:
		UITreeView();
	private:
		void m_UITreeView_DragItemsCompleted(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewDragItemsCompletedEventArgs^ args);
		void m_UITreeView_ItemInvoked(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewItemInvokedEventArgs^ args);
		void m_UITreeView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
