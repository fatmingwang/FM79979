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
//https://stackoom.com/question/3WFfH/UWP-WinUI-TreeView%E4%BB%A5%E7%BC%96%E7%A8%8B%E6%96%B9%E5%BC%8F%E6%BB%9A%E5%8A%A8%E5%88%B0%E9%A1%B9%E7%9B%AE


//https://stackoverflow.com/questions/52015723/uwp-winui-treeview-programatically-scroll-to-item
//scroll treeview to target node
DependencyObject^ FindChildTypeName(DependencyObject^ parant, String ^ e_strTypeName)
{
    int count = VisualTreeHelper::GetChildrenCount(parant);

    for (int i = 0; i < count; i++)
    {
        auto MyChild = VisualTreeHelper::GetChild(parant, i);
        auto l_TypeID1 = MyChild->GetType();
        auto l_strType1 = l_TypeID1->ToString();
        FMLog::Log(l_strType1->Data(),false);
        auto l_strCheckBoxID = CheckBox::typeid;
        if (l_strCheckBoxID->ToString() == l_strType1)
        {
            //auto l_strTreeViewItemID = Microsoft::UI::Xaml::Controls::TreeViewItem::typeid->ToString();
            auto l_strTreeViewItemID = e_strTypeName;
            CheckBox^ l_pCheckBox = (CheckBox^)MyChild;
            auto l_pParent = parant;
            while (l_pParent)
            {
                Platform::String^ l_strParentType = l_pParent->GetType()->ToString();
                if (l_strTreeViewItemID == l_strParentType)
                {
                    Microsoft::UI::Xaml::Controls::TreeViewItem^ l_pTreeViewITem = (Microsoft::UI::Xaml::Controls::TreeViewItem^)l_pParent;
                    //SolidColorBrush^ l_pSolidColorBrush;
                    //bool l_bChecked = (bool)l_pCheckBox->IsChecked->Value;
                    //if (l_bChecked)
                    //{
                    //    l_pSolidColorBrush = ref new SolidColorBrush(Windows::UI::Colors::Red);
                    //}
                    //else
                    //{
                    //    l_pSolidColorBrush = ref new SolidColorBrush(Windows::UI::Colors::Green);
                    //}
                    //l_pTreeViewITem->Background = l_pSolidColorBrush;
                    return l_pTreeViewITem;
                }
                l_pParent = VisualTreeHelper::GetParent(l_pParent);
            }
        }
        //if (l_TypeID1 == l_TypeID2 && ((FrameworkElement^)MyChild)->Name == ControlName)
          //  return MyChild;

        auto FindResult = FindChildTypeName(MyChild, e_strTypeName);
        if (FindResult != nullptr)
            return FindResult;
    }
    return nullptr;
}

template<class T>T^ FindParentWithTypeByVisualTreeHelper(DependencyObject^ dependencyObject)
{
    auto parent = VisualTreeHelper::GetParent(dependencyObject);

    if (parent == nullptr) 
        return nullptr;
    auto l_strTypeID = T::typeid->ToString();
    if (l_strTypeID == parent->GetType()->ToString())
        return  (T^)parent;
    return nullptr;
}

MUX::TreeViewNode^ CloneNode(MUX::TreeViewNode^e_pNode)
{
    MUX::TreeViewNode^ l_pCloneNode = ref new MUX::TreeViewNode();
    l_pCloneNode->Content = e_pNode->Content;
    for(auto l_Child : e_pNode->Children)
    { 
        auto l_pCloneChildNode = CloneNode(l_Child);
        l_pCloneNode->Children->Append(l_pCloneChildNode);
    }
    return l_pCloneNode;
}


void UWP_Angle_EmptyProject::TreeViewTest::CreateNode(Platform::String^ e_strNodeContent)
{
    auto l_Child2 = ref new MUX::TreeViewNode();
    l_Child2->Content = e_strNodeContent;
    
}

TreeViewTest::TreeViewTest()
{
	InitializeComponent();
    FMLog::Init();
    MUX::TreeViewNode^ workFolder = ref new MUX::TreeViewNode();
    workFolder->Content = "Root";
    workFolder->IsExpanded = true;
    m_TreeView->RootNodes->Append(workFolder);
    auto l_Child1 = ref new MUX::TreeViewNode();
    auto l_Child2 = ref new MUX::TreeViewNode();
    l_Child1->Content = "Child1";
    l_Child2->Content = "Child2";
    workFolder->Children->Append(l_Child1);
    workFolder->Children->Append(l_Child2);
    const int l_ciCount = 10;
    for (int i = 0; i < l_ciCount; ++i)
    {
        auto l_Child21 = ref new MUX::TreeViewNode();
        l_Child21->Content = i.ToString();
        l_Child2->Children->Append(l_Child21);
        auto l_NodeItem = m_TreeView->ItemFromContainer(l_Child2);
        if (l_NodeItem)
        {
            auto l_Str = l_NodeItem->GetType()->ToString();
            int a = 0;
        }
        //l_NodeItem->
    }
    //m_TreeView->Focus(Windows::UI::Xaml::FocusState::Keyboard);

    //auto l_Node = m_TreeView->NodeFromContainer(l_pTreeViewItem);
}


void UWP_Angle_EmptyProject::TreeViewTest::ShowRenameDialog()
{
    auto l_SelectedNode = m_TreeView->SelectedNode;
    auto l_Root = m_TreeView->RootNodes->GetAt(0);
    if (l_SelectedNode == l_Root)
    {
        RootNodeNotAllowRename_TeachingTip->IsOpen = true;
        return;
    }
    if (l_SelectedNode)
    {
        m_pSelectedNode = l_SelectedNode;
        m_TextBox->Text = l_SelectedNode->Content->ToString();
        m_ContentDialog->ShowAsync();
    }
}


void UWP_Angle_EmptyProject::TreeViewTest::m_ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
    if (m_pSelectedNode)
    {
        auto l_Root = m_TreeView->RootNodes->GetAt(0);
        if (l_Root == m_pSelectedNode)
        {

        }
        else
        {
            m_pSelectedNode->Content = m_TextBox->Text;
            m_TreeView->Collapse(m_TreeView->RootNodes->GetAt(0));
            m_TreeView->SelectedNode = m_pSelectedNode;
            m_TreeView->Expand(m_TreeView->RootNodes->GetAt(0));
            m_pSelectedNode = nullptr;
        }
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

void UWP_Angle_EmptyProject::TreeViewTest::AppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    AppBarButton^ l_pSender = (AppBarButton^)sender;
    auto l_strSenderName = l_pSender->Name;
    if (l_pSender->Name == this->m_RenameButton->Name)
    {
        ShowRenameDialog();
    }
    else
    if (l_strSenderName == this->m_CopyButton->Name)
    {
        m_pSelectedNodeForCloneOrCut = this->m_TreeView->SelectedNode;
    }
    else
    if(l_strSenderName == this->m_CutButton->Name)
    {
        m_pSelectedNodeForCloneOrCut = m_TreeView->SelectedNode;
        //m_TreeView->node
    }
    else
    if (l_strSenderName == this->m_DeleteButton->Name)
    {
        if (m_TreeView->SelectedNode)
        {
            if (m_TreeView->SelectedNode->Parent)
            {
                unsigned int l_uiIndex = 0;
                if (m_TreeView->SelectedNode->Parent->Children->IndexOf(m_TreeView->SelectedNode,&l_uiIndex))
                {
                    m_TreeView->SelectedNode->Parent->Children->RemoveAt(l_uiIndex);
                }
            }
        }
    }
    else
    if (l_strSenderName == this->m_PasteButton->Name)
    {
        if (m_pSelectedNodeForCloneOrCut)
        {
            if (m_TreeView->SelectedNode)
            {
                auto l_pNode = CloneNode(m_pSelectedNodeForCloneOrCut);
                m_TreeView->SelectedNode->Children->Append(l_pNode);
                //CloneXaml<MUX::TreeViewNode>(m_TreeView->SelectedNode);
                //auto l_pTreeNode = m_TreeView->SelectedNode->c
                //m_TreeView->SelectedNode->Children->Append();
            }
        }
    }
    else
    if (l_strSenderName == this->m_NewButton->Name)
    {
        //m_ContentDialog->PrimaryButtonClick->removeAll();
        //m_ContentDialog->
        //m_TextBox->Text = l_SelectedNode->Content->ToString();
        //m_ContentDialog->ShowAsync();
        //ContentDialogResult result = await noWifiDialog.ShowAsync();
    }
}

void UWP_Angle_EmptyProject::TreeViewTest::CheckBox_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    CheckBox^l_pCheckBox = (CheckBox^)sender;
    auto l_pParent = VisualTreeHelper::GetParent(l_pCheckBox);
    while (l_pParent)
    {
        auto l_strParent = l_pParent->GetType()->ToString();
        if (l_strParent == L"Microsoft.UI.Xaml.Controls.TreeViewItem")
        {
            Microsoft::UI::Xaml::Controls::TreeViewItem^ l_pTreeViewItem = (Microsoft::UI::Xaml::Controls::TreeViewItem^)l_pParent;
            auto l_Node = m_TreeView->NodeFromContainer(l_pTreeViewItem);
            if (l_Node)
            {
                SolidColorBrush^ l_pSolidColorBrush;
                bool l_bChecked = (bool)l_pCheckBox->IsChecked->Value;
                if (l_bChecked)
                {
                    l_pSolidColorBrush = ref new SolidColorBrush(Windows::UI::Colors::Red);
                }
                else
                {
                    l_pSolidColorBrush = ref new SolidColorBrush(Windows::UI::Colors::Green);
                }
                //l_pTreeViewItem->Background = l_pSolidColorBrush;
                break;
            }
        }
        l_pParent = VisualTreeHelper::GetParent(l_pParent);
    }
}

//void UWP_Angle_EmptyProject::TreeViewTest::m_TreeView_ItemInvoked(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewItemInvokedEventArgs^ args)
//{
//    auto l_strType =  args->InvokedItem->GetType()->ToString();
//    this->m_pSelectedNode = (MUX::TreeViewNode^)args->InvokedItem;
//    auto l_pNode = this->m_TreeView->SelectedNode;
//    auto l_str = sender->GetType()->ToString();
//    int a = 0;
//}

//bool DoesPointContainElement(Point testPoint,UIElement referenceFrame)
//{
//    
//    foreach(UIElement item in elementStack)
//    {
//        FrameworkElement feItem = item as FrameworkElement;
//        //cast to FrameworkElement, need the Name property
//        if (feItem != null)
//        {
//            if (feItem.Name.Equals(elementName))
//            {
//                return true;
//            }
//        }
//    }
//    // elementName was not in this stack 
//    return false;
//}
//https://stackoverflow.com/questions/15437679/how-to-know-when-a-tree-view-item-is-visible-on-screen
void UWP_Angle_EmptyProject::TreeViewTest::m_TreeView_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
    auto l_Transform = m_TreeView->TransformToVisual(nullptr);
    Windows::Foundation::Rect l_Rect(0.f,0.f, (float)m_TreeView->ActualWidth, (float)m_TreeView->ActualHeight);
    auto l_TransformRect = l_Transform->TransformBounds(l_Rect);
    auto l_VisibleElements = VisualTreeHelper::FindElementsInHostCoordinates(l_TransformRect, m_TreeView);
    Windows::UI::Input::PointerPoint^ ptrPt = e->GetCurrentPoint((Windows::UI::Xaml::UIElement^)sender);
    auto l_Pos = e->GetCurrentPoint(this);
    auto l_X = ptrPt->Position.X+ l_TransformRect.X;
    auto l_Y = ptrPt->Position.Y+ l_TransformRect.Y;
    auto l_X2 = l_Pos->Position.X + l_TransformRect.X;
    auto l_Y2 = l_Pos->Position.Y + l_TransformRect.Y;
    this->m_TreeView->RootNodes->GetAt(0)->Content = l_X.ToString()+","+ l_Y.ToString();
    //auto  elementStack = VisualTreeHelper::FindElementsInHostCoordinates(l_Pos->Position, m_TreeView);
    for each(UIElement^item in l_VisibleElements)
    {
        auto l_StrType = item->GetType()->ToString();
        if (l_StrType == L"Microsoft.UI.Xaml.Controls.TreeViewItem")
        {
            auto l_Node = m_TreeView->NodeFromContainer(item);
            auto l_TreeViewItemTransform = item->TransformToVisual(nullptr);
            Windows::Foundation::Rect l_ItemRect(0.f,0.f, (float)item->ActualSize.x, (float)item->ActualSize.y);
            auto l_TreeViewItemTransformRect = l_TreeViewItemTransform->TransformBounds(l_ItemRect);
            if (l_Y >= l_TreeViewItemTransformRect.Y&&
                l_Y <= l_TreeViewItemTransformRect.Y+ item->ActualSize.y)
            {
                this->m_pSelectedNode = l_Node;
                //l_Node->Content = "Hitted";
                return;
            }
            else
            {
                l_Node->Content = "not Hitted";
            }
            auto l_strContent = l_Node->Content;
            int a = 0;
        }
        //FrameworkElement feItem = item as FrameworkElement;
        ////cast to FrameworkElement, need the Name property
        //if (feItem != null)
        //{
        //    if (feItem.Name.Equals(elementName))
        //    {

        //    }
        //}
    }
    // elementName was not in this stack 

}
