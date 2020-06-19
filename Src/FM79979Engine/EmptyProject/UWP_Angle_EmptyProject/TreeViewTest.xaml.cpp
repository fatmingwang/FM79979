//
// TreeViewTest.xaml.cpp
// Implementation of the TreeViewTest class
//

#include "pch.h"
#include "TreeViewTest.xaml.h"
#include <experimental/coroutine>
using namespace UWP_Angle_EmptyProject;

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

TreeViewTest::TreeViewTest()
{
	InitializeComponent();
    
    MUX::TreeViewNode^ workFolder = ref new MUX::TreeViewNode();// { Content = "Work Documents" };
    workFolder->Content = "Root";
    workFolder->IsExpanded = true;

    auto l_Child1 = ref new MUX::TreeViewNode();
    auto l_Child2 = ref new MUX::TreeViewNode();
    l_Child1->Content = "Child1";
    l_Child2->Content = "Child2";
    workFolder->Children->Append(l_Child1);
    workFolder->Children->Append(l_Child2);
    //workFolder->ChildrenAdd(new mux.TreeViewNode() { Content = "Feature Schedule" });
    //workFolder.Children.Add(new mux.TreeViewNode() { Content = "Overall Project Plan" });
    //workFolder.Children.Add(new mux.TreeViewNode() { Content = "Feature Resources Allocation" });

    //mux.TreeViewNode remodelFolder = new mux.TreeViewNode() { Content = "Home Remodel" };
    //remodelFolder.IsExpanded = true;

    //remodelFolder.Children.Add(new mux.TreeViewNode() { Content = "Contractor Contact Info" });
    //remodelFolder.Children.Add(new mux.TreeViewNode() { Content = "Paint Color Scheme" });
    //remodelFolder.Children.Add(new mux.TreeViewNode() { Content = "Flooring woodgrain type" });
    //remodelFolder.Children.Add(new mux.TreeViewNode() { Content = "Kitchen cabinet style" });

    //personalFolder = new mux.TreeViewNode() { Content = "Personal Documents" };
    //personalFolder.IsExpanded = true;
    //personalFolder.Children.Add(remodelFolder);

    //sampleTreeView.RootNodes.Add(workFolder);
    //sampleTreeView.RootNodes.Add(personalFolder);


    m_TreeView->RootNodes->Append(workFolder);
    m_TreeView->Focus(Windows::UI::Xaml::FocusState::Keyboard);
    //m_TreeView->

}


void UWP_Angle_EmptyProject::TreeViewTest::m_TreeView_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
    auto l_SelectedNode = m_TreeView->SelectedNode;
    if (l_SelectedNode)
    {
        m_pSelectedNode = l_SelectedNode;
        m_TextBox->Text = l_SelectedNode->Content->ToString();
        m_ContentDialog->ShowAsync();
    }
}


void UWP_Angle_EmptyProject::TreeViewTest::m_TreeView_PointerEntered(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
    //m_TreeView->Focus(Windows::UI::Xaml::FocusState::Keyboard);
}


void UWP_Angle_EmptyProject::TreeViewTest::m_ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
    if (m_pSelectedNode)
    {
        m_pSelectedNode->Content = m_TextBox->Text;
    }
}

void UWP_Angle_EmptyProject::TreeViewTest::m_TreeView_DragItemsCompleted(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewDragItemsCompletedEventArgs^ args)
{
    MUX::TreeViewNode^ l_1 = (MUX::TreeViewNode^)args->NewParentItem;
    auto l_2 = args->Items;
    auto l_3 = args->DropResult;
    int a = 0;
    auto l_iSize = l_2->Size;
    MUX::TreeViewNode^l_SelectedItem = (MUX::TreeViewNode^)l_2->GetAt(0);
    MUX::TreeViewNode^ l_pParent = (MUX::TreeViewNode^)l_SelectedItem->Parent;
    auto l_ParentContent = l_pParent->Content;
    auto l_SelectedItemContent = l_SelectedItem->Content;
    if (l_3 != Windows::ApplicationModel::DataTransfer::DataPackageOperation::None)
    {
        if (m_pSelectedNode)
        {
            auto l_SelectedName = m_pSelectedNode->Content;
            int a = 0;
        }
        if (l_1)
        {
            auto l_ParentSelectedName = l_1->Content;
            unsigned int l_uiIndex = -1;
            if (l_1->Children->IndexOf(l_SelectedItem, &l_uiIndex))
            {

            }
            int a = 0;
        }
    }
}


void UWP_Angle_EmptyProject::TreeViewTest::m_TreeView_DragItemsStarting(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewDragItemsStartingEventArgs^ args)
{
    Windows::ApplicationModel::DataTransfer::DataPackage^l_pData = args->Data;
    auto l_2 = args->Items;
    auto l_iSize = l_2->Size;
    MUX::TreeViewNode^ l_SelectedItem = (MUX::TreeViewNode^)l_2->GetAt(0);
    auto l_SelectedItemContent = l_SelectedItem->Content;
    m_pSelectedNodeOriginalParent = l_SelectedItem->Parent;
    m_pSelectedNode = l_SelectedItem;
    if (m_pSelectedNodeOriginalParent)
    {
        unsigned int l_uiIndex = -1;
        if (m_pSelectedNodeOriginalParent->Children->IndexOf(m_pSelectedNode, &l_uiIndex))
        {
            int a = 0;
        }
    }
    int a = 0;
}


void UWP_Angle_EmptyProject::TreeViewTest::m_TreeView_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
    //FlyoutShowOptions^ myOption = ref new FlyoutShowOptions();
    //myOption->ShowMode = FlyoutShowMode::Transient;// : FlyoutShowMode.Standard;
    //CommandBarFlyout1->ShowAt(m_TreeView, myOption);

}


void UWP_Angle_EmptyProject::TreeViewTest::AppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}
