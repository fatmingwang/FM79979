//
// TreeViewTest.xaml.h
// Declaration of the TreeViewTest class
//

#pragma once

#include "TreeViewTest.g.h"
#define MUX Microsoft::UI::Xaml::Controls
//https://xamlbrewer.wordpress.com/2018/06/08/using-a-treeview-control-for-navigation-in-uwp/
namespace UWP_Angle_EmptyProject
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class TreeViewTest sealed
	{
		std::map<TreeViewNode^, NamedTypedObject*>m_MapTest;
		MUX::TreeViewNode^ m_pSelectedNode;
		MUX::TreeViewNode^ m_pSelectedNodeOriginalParent;
		MUX::TreeViewNode^ m_pSelectedNodeForCloneOrCut;
		void	CreateNode(Platform::String^e_strNodeContent);
		void	RemoveNode(MUX::TreeViewNode^ e_pNode);
	public:
		TreeViewTest();
	private:
		void ShowRenameDialog();
		void m_ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
		void m_TreeView_DragItemsCompleted(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewDragItemsCompletedEventArgs^ args);
		void m_TreeView_DragItemsStarting(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewDragItemsStartingEventArgs^ args);
		void AppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CheckBox_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		//void m_TreeView_ItemInvoked(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewItemInvokedEventArgs^ args);
		void m_TreeView_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
	};
}
