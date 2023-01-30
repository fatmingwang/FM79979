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
	m_pLastIvokedTreeviewNode = nullptr;
}


enum eMoveType
{
	//multi-nodes change order.
	eMT_PROCESS_NODECHANGE_ORDER = 0,
	eMT_PROCESS_NODE_CHANGE_PARENT,
	eMT_MAX
};
//if parent treeviewitem is not at e_pCollections,it's parellel

//Microsoft::UI::Xaml::Controls::TreeViewNode^ GetParentInCollections(Microsoft::UI::Xaml::Controls::TreeViewNode^e_pTarget, Windows::Foundation::Collections::IVectorView<Platform::Object^>^ e_pCollections, eMoveType&e_eMoveType)
//{
//	//Microsoft::UI::Xaml::Controls::TreeViewItem^ l_pParent = nullptr;
//	unsigned int l_uiIndex = -1;
//	Microsoft::UI::Xaml::Controls::TreeViewNode^l_pTarget = e_pTarget;
//	while (e_pCollections->IndexOf(l_pTarget->Parent, &l_uiIndex))
//	{
//		l_pTarget = (Microsoft::UI::Xaml::Controls::TreeViewNode^)e_pCollections->GetAt(l_uiIndex);
//	}
//	if ( l_pTarget != e_pTarget)
//	{
//		e_eMoveType = eMoveType::eMT_PROCESS_ONE_NODE_CHANGE_PARENT;
//	}
//	else
//	{
//		e_eMoveType = eMoveType::eMT_PROCESS_PARELLEL_NODES_CHANGE_ORDER;
//	}
//	return l_pTarget;
//	
//}


//bool GetMPDINode(cMPDINode** e_pOut);
bool UIEditor::UITreeView::GetMPDINode(size_t* e_puiOutAddress)
{
	if (m_pLastIvokedTreeviewNode)
	{
		if (m_TreeviewNodeAndcMPDINodeMap->HasKey(m_pLastIvokedTreeviewNode))
		{
			*e_puiOutAddress = m_TreeviewNodeAndcMPDINodeMap->Lookup(m_pLastIvokedTreeviewNode);
			//cMPDINode*l_pMPDINode = (cMPDINode*)l_uiAddress;
			//*e_pOut = (cMPDINode*)l_uiAddress;
			return true;
		}
	}
	return false;
}

void UIEditor::UITreeView::m_UITreeView_DragItemsCompleted(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewDragItemsCompletedEventArgs^ args)
{
	auto l_Result = args->DropResult;
	if (l_Result == Windows::ApplicationModel::DataTransfer::DataPackageOperation::Move)
	{
		eMoveType l_eMoveType = eMoveType::eMT_MAX;
		if (args->Items->Size )
		{
			String^ l_strName = nullptr;
			Microsoft::UI::Xaml::Controls::TreeViewNode^ l_pParent = nullptr;
			auto l_pFirstSelectedItem = args->Items->GetAt(0);
			auto l_strType = l_pFirstSelectedItem->GetType()->ToString();
			if (args->NewParentItem)
			{
				l_pParent = (Microsoft::UI::Xaml::Controls::TreeViewNode^)args->NewParentItem;
				l_eMoveType = eMoveType::eMT_PROCESS_NODE_CHANGE_PARENT;
			}
			else
			{
//				auto l_Parent = GetParentInCollections((Microsoft::UI::Xaml::Controls::TreeViewNode^)l_pFirstSelectedItem, args->Items, l_eMoveType);
				l_eMoveType = eMoveType::eMT_PROCESS_NODECHANGE_ORDER;
				l_pParent = ((Microsoft::UI::Xaml::Controls::TreeViewNode^)l_pFirstSelectedItem)->Parent;
			}
			l_strName = l_pParent->ToString();
			l_strType = l_pParent->GetType()->ToString();
			auto ooo = args->Items;
			for (int i = 0; i < (int)args->Items->Size; ++i)
			{
				l_strName = args->Items->GetAt(i)->ToString();
				OutputDebugString(l_strName->Data());
				OutputDebugString(L"\n");
				int a = 0;
			}
			int a = 0;
		}
	}
	int a = 0;
}


void UIEditor::UITreeView::m_UITreeView_ItemInvoked(Microsoft::UI::Xaml::Controls::TreeView^ sender, Microsoft::UI::Xaml::Controls::TreeViewItemInvokedEventArgs^ args)
{
	m_pLastIvokedTreeviewNode = args->InvokedItem;
	auto l_str = args->InvokedItem->ToString();
	int a = 0;
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
