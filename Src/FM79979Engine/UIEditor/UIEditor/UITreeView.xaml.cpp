//
// UITreeView.xaml.cpp
// Implementation of the UITreeView class
//

#include "pch.h"
#include "UITreeView.xaml.h"

using namespace UIEditor;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

UITreeView::UITreeView()
{
	InitializeComponent();
}


void UIEditor::UITreeView::m_UITreeView_DragItemsCompleted(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewDragItemsCompletedEventArgs^ args)
{

}


void UIEditor::UITreeView::m_UITreeView_ItemInvoked(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewItemInvokedEventArgs^ args)
{

}


void UIEditor::UITreeView::m_UITreeView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	for (int i = 0; i < 15; ++i)
	{
		Microsoft::UI::Xaml::Controls::TreeViewNode^ l_pNode = ref new Microsoft::UI::Xaml::Controls::TreeViewNode();
		l_pNode->Content = i.ToString();
		m_UITreeView->RootNodes->Append(l_pNode);
	}

}
