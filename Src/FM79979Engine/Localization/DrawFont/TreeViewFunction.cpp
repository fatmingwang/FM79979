#include "stdafx.h"
#include "TreeViewFunction.h"


GCFORM::TreeNode^	GetSelectTreeNodeParnet(GCFORM::TreeView^e_pTreeView)
{
	return e_pTreeView->SelectedNode->Parent;
}
bool	IsTreeNodeCotainName(GCFORM::TreeNode^e_pTreeNode,System::String^e_pString)
{
	if( !e_pTreeNode )
	{
		WARING_MSG("no such node");
		return false;
	}
	GCFORM::TreeNode^ l_pNode = e_pTreeNode->FirstNode;
	while(l_pNode)
	{
		if( l_pNode->Name->Equals(e_pString) )
			return true;
		l_pNode = l_pNode->NextNode;
	}
	return false;
}

bool	IsSelectTreeNodeParentContainInputString(GCFORM::TreeView^e_pTreeView,System::String^e_pString)
{
	return IsTreeNodeCotainName(GetSelectTreeNodeParnet(e_pTreeView),e_pString);
}
