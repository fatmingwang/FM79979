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
		//cMPDINode*
		property Windows::Foundation::Collections::IMap<Platform::Object^, size_t>^	m_TreeviewNodeAndcMPDINodeMap;
		bool GetTreeNode(size_t *e_puiOutAddress);
	private:
		Platform::Object^ m_pLastIvokedTreeviewNode;
		void m_UITreeView_DragItemsCompleted(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewDragItemsCompletedEventArgs^ args);
		void m_UITreeView_ItemInvoked(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewItemInvokedEventArgs^ args);
		void m_UITreeView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuFlyoutItem_Add_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuFlyoutItem_Copy_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuFlyoutItem_Cut_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuFlyoutItem_Paste_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
