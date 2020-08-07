#ifndef _CORE_DOTNET_TOOLS_H_
#define _CORE_DOTNET_TOOLS_H_
#include "../Core/AllCommonInclude.h"
#include "DotNetCommonTools.h"
//ensure u have include AllFeelingLihInclude.h
//and it under common runtime environment

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
//namespace CDNT
namespace DNCT
{
	//for quickly to add data while list and node is not same count
	template<class TYPE>
	void	AssignListData(GCFORM::TreeNode^e_pNode,cNamedTypedObjectVector<TYPE>*e_pNamedTypedObjectList)
	{
		if(e_pNode->Nodes->Count != e_pNamedTypedObjectList->Count() )
		{
			e_pNode->Nodes->Clear();
			for( int i=0;i<e_pNamedTypedObjectList->Count();++i )
			{
				e_pNode->Nodes->Add(String(UT::WcharToChar((*e_pNamedTypedObjectList)[i]->GetName())).ToString());
			}
		}
	}

	template<class TYPE>
	void	AssignListData(GCFORM::ListBox^e_pLisBox,cNamedTypedObjectVector<TYPE>*e_pNamedTypedObjectList)
	{
		if(e_pLisBox->Items->Count != e_pNamedTypedObjectList->Count() )
		{
			e_pLisBox->Items->Clear();
			for( int i=0;i<e_pNamedTypedObjectList->Count();++i )
			{
				e_pLisBox->Items->Add(String(UT::WcharToChar((*e_pNamedTypedObjectList)[i]->GetName()).c_str()).ToString());
			}
		}
	}

	template<class T>inline	void DumpObjectListIntoListBox(System::Windows::Forms::ListBox^e_pListbox,T e_pList,bool e_bClearListBoxData = false,bool e_bAddIndex = true)
	{
		if( e_bClearListBoxData )
			e_pListbox->Items->Clear();
		for( int i=0;i<e_pList->Count();++i )
		{
		    if(e_bAddIndex)
			    e_pListbox->Items->Add(i.ToString()+":"+DNCT::WcharToGcstring((*e_pList)[i]->GetName()));
			else
			    e_pListbox->Items->Add(DNCT::WcharToGcstring((*e_pList)[i]->GetName()));
			
		}
	}

	System::Windows::Forms::TreeNode^	MPDIListToTreeNode(cMPDIList*e_pMPDIList);
	System::Windows::Forms::TreeNode^	PIToTreeNode(cPuzzleImage*e_pPI);
	//only export name,whatever the type they are.
	void	ImagerParserAddIntiTreeView(cNamedTypedObjectVector<NamedTypedObject>*e_pImageParser,System::Windows::Forms::TreeView^e_pTreeView,bool e_bClearTreeViewData);
	bool	RemoveTextureFromSelectedNodesTreeView(System::Windows::Forms::TreeView^e_pTreeView,cImageParser*e_pImageParser);

	GCFORM::TreeNode^TinyXMLNodeToTreeNode(TiXmlElement*e_pTiXmlElement);
	TiXmlElement*TreeNodeToTinyXMLNode(GCFORM::TreeNode^e_pNode,TiXmlElement*e_pParent);
	System::Drawing::Bitmap^OpenImageFile(System::String^e_strFileName);
	System::Drawing::Bitmap^OpenImageFile(System::String^e_strFileName,cUIImage**e_pUIImage);

	UINT	OpenglToDotNetColor(UINT32 e_RGBA);
};
using namespace DNCT;
#endif