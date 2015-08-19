#pragma once
using namespace System;
using namespace System::Windows::Forms;

namespace DNCT
{
	using namespace System::Xml;
	#define	CHAR_TO_WCHAR( p,q )	 MultiByteToWideChar( CP_ACP, 0, p, -1, q, 512 );//q for return p for convert
	#define	WCHAR_TO_CHAR( p,q )  WideCharToMultiByte( CP_ACP, 0, p, -1, q, 256, NULL, NULL );//q for return p for convert
	//p System.String  q char*
	#define GCSTRING_TO_CHAR(p,q){assert(q && "NULL Execption");sprintf_s(q,MAX_PATH,"%s\0",p);}
	#define GCSTRING_TO_WCHAR(p,q){assert(q && "NULL Execption");char	l_pp[MAX_PATH];	GCSTRING_TO_CHAR(p,l_pp);	CHAR_TO_WCHAR(l_pp,q);}

	#define	TO_GCSTRING(p)gcnew System::String(p)

	#define	GCFORM System::Windows::Forms

	#define WARNING_MSG(p)GCFORM::MessageBox::Show(p,"waring",GCFORM::MessageBoxButtons::OK)
	#define WARING_YES_NO(p)GCFORM::MessageBox::Show(p,"waring",GCFORM::MessageBoxButtons::YesNo)
	#define WARING_YES_NO_TO_YES(p)if(WARING_YES_NO(p) == GCFORM::DialogResult::Yes)

	
	#define DIALOG_OK(p) if( p->ShowDialog() == System::Windows::Forms::DialogResult::OK )
	
	#define GCNEW(p,q) p^ q = gcnew p;//class and named object

	char*	GcStringToChar(System::String^e_str);
	WCHAR*	GcStringToWchar(System::String^e_str);
	//===============================================
	//eplace hashtable value by key
	//===============================================
	System::Void	ReplaceValuInHashTable(System::String^e_pKey,System::Object^e_pNewValue,System::Collections::Hashtable^e_pHashTable);
	//===============================================
	//
	//==============================================
	System::Windows::Forms::TextBox^	CreateDefaultTextBox(char*e_pName,int	e_iXPOs = 45,int e_iYPos = 45);
	//===============================================
	//
	//==============================================
	System::Windows::Forms::ComboBox^	CreateDefaultCombox(char*,System::Collections::ArrayList^ e_pStringForText,int	e_iXPOs = 45,int e_iYPos = 5);
	//===============================================
	// 1st listbox name
	// 2nd controlcollection to add listbox
	//===============================================
	System::Windows::Forms::ListBox^	CreateDefaultListBox(char*,System::Windows::Forms::Control::ControlCollection^ e_p,int e_iPosX = 90,int e_iPosY = 90);
	System::Windows::Forms::ListBox^	CreateDefaultListBox(System::String^,System::Windows::Forms::Control::ControlCollection^ e_p,int e_iPosX = 90,int e_iPosY = 90);
	GCFORM::ListBox^	GetActuallyListBoxFromHashTable(System::String^,System::Collections::Hashtable^,System::Windows::Forms::Control::ControlCollection^ e_p);
	//===============================================
	// for temp form to decide something to do ,ex decide listbox add or del
	//or textbox to input text to add data
	//===============================================
	System::Windows::Forms::Form^	CreateTempFromToDecideSelect( char* e_pAcceptString,char*e_pChanelString,System::Collections::ArrayList^);

	array<String^>^ OpenFileAndGetNames(char*e_pFileFilter = 0);
	System::String^	OpenFileAndGetName(char*e_pFileFilter = 0);
	System::String^	SaveFileAndGetName(char*e_pFileFilter = 0);
	System::String^	OpenFileGetText();
	System::String^	GetChanglineString();

	//===============================================
	//if contain the string will return true or not
	//===============================================
	bool	CheckListContainStringAndAdd(GCFORM::ListBox^,System::String^);
	//===============================================
	//if listbox contain the string than will return selectindex value or will return the -1
	//===============================================
	bool	ReplaceListContainString(GCFORM::ListBox^,System::String^);
	//===============================================
	//
	//===============================================
	System::Void	StreamWriteFileFromString(System::String^e_pString,System::String^e_pFileName="",System::Text::Encoding^ e_pEncoding = System::Text::Encoding::GetEncoding(TO_GCSTRING("UTF-16")));
	//===============================================
	//
	//===============================================
	System::Drawing::Font^ GetFontFromFontDialog();
	//===============================================
	//
	//===============================================
	bool	IsNodesContainName(System::String^e_pString,GCFORM::TreeNodeCollection^e_pNodeCollection);
	//===============================================
	//It will check the node exist in its children if no it will add and retur true or return false
	//===============================================	
	bool	TreeNodeAddChild(System::String^e_pString,GCFORM::TreeNode^e_pTreeNode);
	//===============================================
	//TreeView Add Root
	//===============================================	
	bool	TreeViewAddRoot(System::String^e_pString,GCFORM::TreeView^e_pTreeView);
	//===============================================
	//recrsive all child and slib
	//===============================================
	typedef void    (*TREE_NODE_CALL_BACK)( GCFORM::TreeNode^,void*e_pVoid);
	void	SearchAllNodes(GCFORM::TreeNode^e_pNode,void*e_pVoid,TREE_NODE_CALL_BACK);
	//===============================================
	//strip the string don't need,if e_bStripExtensionFileName = true the extension file will strip,ex: Abcd.bmp will become Abcd
	//===============================================
	System::String^	GetFileNameWithoutFullPath(System::String^e_pString,bool e_bStripExtensionFileName = false);
	//===============================================
	//how many slash does string have
	//===============================================
	int		GetSlashCount(System::String^e_pString);

	//===============================================
	//open xml file and return the TreeNode that contain all tag
	//===============================================
	GCFORM::TreeNode^	OpenXmlFileGetAllTagToNode(System::String^e_pString = "");
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
	GCFORM::TreeNode^	OpenXmlFileToNode();

	using namespace DNCT;
}