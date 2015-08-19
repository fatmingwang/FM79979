using namespace System;
using namespace System::Windows::Forms;
using namespace System::Xml;
using namespace System::IO;
#include "DotNetDefine.h"

namespace DNCT
{
	//===============================================
	//here for combine xml and treenode
	//===============================================
	//for load  but only support element if the character out of rule will crash
	// SECTION 1. Create a DOM Document and load the XML data into it.
	//xmldoc->LoadXml(l_pString);

	//// SECTION 2. Initialize the TreeView control.
	//GCFORM::TreeView^l_pTreeView = gcnew GCFORM::TreeView();
	//TextEDitor_treeView->Nodes->Clear();
	//l_pTreeView->Nodes->Add(gcnew TreeNode(xmldoc->DocumentElement->Name));
	//TreeNode^ tNode = l_pTreeView->Nodes[0];
	//// SECTION 3. Populate the TreeView with the DOM nodes.
	//DNCT::AddNode(xmldoc->DocumentElement, tNode);
	System::Void AddNode(XmlNode^ inXmlNode, TreeNode^ inTreeNode);
	//for load all attributes as the abvoe just change AddNode to self
	void AddAttributesAndChildren(XmlNode^ inXmlNode, TreeNode^ inTreeNode);
	//for save
	//
	//XmlDocument^ d = gcnew XmlDocument();
	//XmlNode^ n = d->CreateNode(XmlNodeType::Element,"root","");
	//for each (TreeNode^ t in TextEDitor_treeView->Nodes)
	//{
	// n->AppendChild(DNCT::getXmlNode(t, d));
	//}
	// d->AppendChild(n);
	// d->Save(l_pString);
	XmlNode^ getXmlNode(TreeNode^ tnode,XmlDocument^ d);
	//u can use here to do some easy save and open file 
	//just use treenode to get xml data is good enough for easy save
	System::Void	SaveTreeNodeToFile(GCFORM::TreeNode^e_pTreeNode);
	System::Void	SaveTreeNodeToFile(GCFORM::TreeNode^e_pTreeNode,String^e_strFileName);
	System::Void	SaveTreeViewToFile(GCFORM::TreeView^e_pTreeView);
	System::Void	SaveTreeViewToFile(GCFORM::TreeView^e_pTreeView,String^e_strFileName);
	//if input NULL,it will automaticly open file dialog
	System::Xml::XmlDataDocument^OpenXmlFile(String^e_strFileName,bool e_bUnidoe = true);

	GCFORM::TreeNode^OpenXmlFileToNode(String^e_strFileName,bool e_bUnidoe = true);
}