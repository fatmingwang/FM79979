#include "stdafx.h"
#include "XMLForm.h"
#include "time.h"
#include "../../DotNetCommon/DotNetXMLTools.h"
#include "TreeViewToXMLNode.h"
#define WRITE_GROUP_INOT_XML	l_pXMLWriter->StartElement(l_sName);// l_pXMLWriter->AddAttribute("Level",l_iTotalLevel);
#define WRITE_CHILD_INOT_XML    l_pXMLWriter->AddAttribute("Name",l_sName);
namespace XMLDataEditor
{

	void	XMLForm::DoNewRootNodeBySelectedNodeFromTreeNode(System::Windows::Forms::TreeView^e_pTreeView)
	{
		XML_DoNewRootNodeBySelectedNodeFromTreeNode(e_pTreeView);
	}

	void	XMLForm::DoNewRootNode(System::Windows::Forms::TreeNode^e_pNode)
	{
		XML_DoNewRootNode(e_pNode,CurrentAttribute_treeView);
	}

	void	XMLForm::DoPasteButton()
	{
		XML_DoPasteButton(m_pCurrentTempNode,CurrentAttribute_treeView,m_pbCut);
	}

	void	XMLForm::DoCutButton()
	{
		XML_DoCutButton(CurrentAttribute_treeView,m_pCurrentTempNode,m_pbCut);
	}

	void	XMLForm::DoCopyButton()
	{
		XML_DoCopyButton(CurrentAttribute_treeView,m_pCurrentTempNode,m_pbCut);
	}

	void	XMLForm::DoAddCommentButton()
	{
		XML_DoAddCommentButton(CurrentAttribute_treeView);
	}

	void	XMLForm::DoReplaceButton()
	{
		XML_DoReplaceButton(Attribute_textBox,CurrentAttribute_treeView,Value_textBox);	
	}

	void	XMLForm::DoAddElementButton()
	{
		XML_DoAddElementButton(Attribute_textBox,CurrentAttribute_treeView);
	}

	void	XMLForm::DoDeleteButton()
	{
		XML_DoDeleteButton(CurrentAttribute_treeView);
	}

	void	XMLForm::DoAddAttributeButton()
	{
		XML_DoAddAttributeButton(Attribute_textBox,Value_textBox,CurrentAttribute_treeView);
	}

	int		HowMuchEndShouldHas(GCFORM::TreeNode^e_pNode)
	{
		GCFORM::TreeNode^l_pNodeParent = e_pNode->Parent;
		GCFORM::TreeNode^l_pNodeCurrent = e_pNode;
		int	l_i = 0;
		while(l_pNodeParent)
		{//search all father  and self,is self last one
			if(  l_pNodeParent->Nodes[l_pNodeParent->Nodes->Count-1]->Equals(l_pNodeCurrent) )
			{
				l_pNodeCurrent = l_pNodeParent;
				l_pNodeParent = l_pNodeParent->Parent;
				l_i++;
			}
			else
				return l_i;
		}
		return l_i;
	}

	bool	IsThisIsALevel(GCFORM::TreeNode^e_pNode)
	{
		bool	l_bIsThisIsALevel = false;
		//search all parent's children
		GCFORM::TreeNode^l_pNode = e_pNode->Parent->Nodes[0];
		while( l_pNode )
		{	
			if( l_pNode->Nodes->Count )
				l_bIsThisIsALevel = true;
			l_pNode = l_pNode->NextNode;
		}
		return l_bIsThisIsALevel;
	}






	XmlNode^ WriteAllNodes(TreeNode^ tnode,XmlDocument^ d,void*e_pVoid)
	{	
		XmlNode^ n = d->CreateNode(XmlNodeType::Element, tnode->Text, "");
		ATG::XMLWriter*     l_pXMLWriter = (ATG::XMLWriter*)e_pVoid;

		char	l_sName[MAX_PATH];
		GCSTRING_TO_CHAR(tnode->Text->ToString(),l_sName);
		if( !tnode->Nodes->Count )
		{
			//this one is a attribute
			if( tnode->Parent&&!IsThisIsALevel(tnode) )
			{
				WRITE_CHILD_INOT_XML;
				System::Xml::XmlAttribute^l_pAtt = d->CreateAttribute("Name");
				l_pAtt->Value = tnode->Text;
				n->Attributes->Append(l_pAtt);
			}
			else
				l_pXMLWriter->StartElement(l_sName);
		}
		else
			l_pXMLWriter->StartElement(l_sName);

		if( tnode->Nodes->Count )
		{
			for each (TreeNode^ t in tnode->Nodes)
			{
				if( t->Nodes->Count )
					n->AppendChild(WriteAllNodes(t,d,e_pVoid));
				else
				{
					GCSTRING_TO_CHAR(t->Text->ToString(),l_sName);
					WRITE_CHILD_INOT_XML;
					System::Xml::XmlAttribute^l_pAtt = d->CreateAttribute("Name");
					l_pAtt->Value = t->Text;
					n->Attributes->Append(l_pAtt);			
				}

			}
			l_pXMLWriter->EndElement();
		}
		return n;
	}
	System::Void	XMLForm::AddAttributeIntoTreeNode()
	{
		 GCFORM::TreeNode^l_pSelectedNode = CurrentAttribute_treeView->SelectedNode;
		 if( !l_pSelectedNode )
		 {
			WARNING_MSG("please select node");
			return;
		 }
		 System::String^l_pNullString;
		 System::String^l_pString = GetTextStringFrom(l_pNullString);
		 if(!l_pString->Length)
			 return;
		 if( l_pSelectedNode->Name->Length )
		 {
				WARNING_MSG("attribute can't add attribute");
				return;
		 }
		if( l_pString->Contains(DNCT::GetChanglineString()) )
		{
			WARNING_MSG("name can't have over 2 linse");
			return;
		}
		 if( l_pString[0]>=L'0'&&l_pString[0]<=L'9' )
		 {
			WARNING_MSG("name of first word can't be number");
			return;
		 }
		 //find last attribute in this element
		 GCFORM::TreeNode^l_pWantNode;
		 for each(GCFORM::TreeNode^l_pCurrentNode in l_pSelectedNode->Nodes)
		 {
			 if( l_pCurrentNode->Text->Equals(l_pString) )
			 {
					WARNING_MSG("attribute already exited");
					return;		 
			 }
			 if( l_pCurrentNode->Name->Length )
			{
				l_pWantNode = l_pCurrentNode;
			}
			else//has child this one is element
			{
				break; 
			}
		 }
		 GCFORM::TreeNode^l_pNewNode = gcnew GCFORM::TreeNode;
		 l_pNewNode->Name = l_pString;		 
		 l_pNewNode->Text = l_pString;
		 //inser into first attribute
		 l_pSelectedNode->Nodes->Add(l_pNewNode);
	}

	System::Void	XMLForm::AddTextIntoTreeNode()
	{
		GCFORM::TreeNode^l_pSelectedNode = CurrentAttribute_treeView->SelectedNode;
		if( !l_pSelectedNode )
		{
			WARNING_MSG("please select node");
			return;
		}
		System::String^l_pNullString;
		System::String^l_pString = GetTextStringFrom(l_pNullString);
		if( l_pString->Length )
		{
			l_pString = l_pString->Trim();
		}
		else
			return;
		//the node to add new comment
		GCFORM::TreeNode^l_pDestNode = gcnew GCFORM::TreeNode();
		l_pDestNode->Name = l_pString;
		l_pDestNode->Text = XML_TEXT;
		if( l_pSelectedNode->Name->Length )
		{
			WARNING_MSG("attribute can't add text");
			return;			
		}
		GCFORM::TreeNode^l_pWantNode;
		//text must betwnn element and element
		for each(GCFORM::TreeNode^l_pCurrentNode in l_pSelectedNode->Nodes)
		{
			if( l_pCurrentNode->Text->Equals(XML_TEXT) )
			{
				WARNING_MSG("text already existed");
				return;
			}
			else//this one is element
			if(l_pCurrentNode->Nodes->Count)
			{
				l_pWantNode = l_pCurrentNode;
				break;
			}
		}
		if( !l_pWantNode )
		{
				WARNING_MSG("text must betwnn element and element");
				return;			
		}
		l_pDestNode->ForeColor = System::Drawing::Color::RoyalBlue;
		l_pSelectedNode->Nodes->Insert(l_pSelectedNode->Nodes->IndexOf(l_pWantNode),l_pDestNode);
	}

	System::Void	XMLForm::AddComment()
	{
		GCFORM::TreeNode^l_pSelectedNode = CurrentAttribute_treeView->SelectedNode;
		if( !l_pSelectedNode )
		{
			WARNING_MSG("please select node");
			return;
		}
		System::String^l_pNullString;
		System::String^l_pString = GetTextStringFrom(l_pNullString);
		if( l_pString->Length )
		{
			l_pString = l_pString->Trim();
		}
		else
			return;
		//the node to add new comment
		GCFORM::TreeNode^l_pDestNode = gcnew GCFORM::TreeNode();
		l_pDestNode->Name = l_pString;
		l_pDestNode->Text = XML_COMMENT;
		//root,add to fisrt fhild
		if( !l_pSelectedNode->Parent )
		{
			WARNING_MSG("can't add comment on root");
			return;
		}//find parent children than compare if true insert before
		else
		{
			if( l_pSelectedNode->Name->Length )
			{
				WARNING_MSG("attribute can't add comment");
				return;			
			}
			GCFORM::TreeNode^l_pWantNode;
			for each(GCFORM::TreeNode^l_pCurrentNode in l_pSelectedNode->Parent->Nodes)
			{
				if( l_pCurrentNode == l_pSelectedNode )
				{
					break;
				}
				else
					l_pWantNode = l_pCurrentNode;
			}
			l_pDestNode->ForeColor = System::Drawing::Color::DarkGreen;
			l_pSelectedNode->Parent->Nodes->Insert(l_pSelectedNode->Parent->Nodes->IndexOf(l_pWantNode),l_pDestNode);
		}
	}

	System::Void	XMLForm::AddElementIntoTreeNode()
	{
		GCFORM::TreeNode^l_pSelectedNode = this->CurrentAttribute_treeView->SelectedNode;
		System::String^l_pNullString;
 		System::String^l_pString = GetTextStringFrom(l_pNullString);
		l_pString = l_pString->Trim();
		if( !l_pString->Length )
			return;
		if( l_pString->Contains(DNCT::GetChanglineString()) )
		{
			WARNING_MSG("name can't have over 2 linse");
			return;
		}
		if( l_pString[0]>=L'0'&&l_pString[0]<=L'9' )
		{
			WARNING_MSG("name of first word can't be number");
			return;
		}
		if( !l_pSelectedNode )
		{
			GCFORM::TreeNode^l_pNewNode = gcnew GCFORM::TreeNode;
			l_pNewNode->Text = l_pString;
			l_pNewNode->Name = "";
			CurrentAttribute_treeView->Nodes->Add(l_pNewNode);
			return;
		}
		//check name has value or not if it contain,do not accept
		if( l_pSelectedNode->Name->Length != 0 )
		{
			WARNING_MSG("attribute and comment can't add element");
			return;
		}
		GCFORM::TreeNode^l_pWantNode;
		//find out first lement,insert it after attrubute
		for each(GCFORM::TreeNode^l_pCurrentNode in l_pSelectedNode->Nodes)
		{
			if( l_pCurrentNode->Nodes->Count )
			{
				l_pWantNode = l_pCurrentNode;
				break;
			}
		}
		GCFORM::TreeNode^l_pNewNode = gcnew GCFORM::TreeNode;
		l_pNewNode->Text = l_pString;
		l_pNewNode->Name = "";
		//there is no element just add it
		if( !l_pWantNode )
			l_pSelectedNode->Nodes->Add(l_pNewNode);
		else
		{
			l_pSelectedNode->Nodes->Insert(l_pSelectedNode->Nodes->IndexOf(l_pWantNode),l_pNewNode);
		}
	}

	System::Void	XMLForm::DeleteTreeNode()
	{
		if( !CurrentAttribute_treeView->SelectedNode )
			return;
		if( WARING_YES_NO("This will delete all nodes child!!") == GCFORM::DialogResult::Yes )
			this->CurrentAttribute_treeView->SelectedNode->Remove();
	}
	System::Void	XMLForm::ModifyTreeNodeData()
	{
		 if( !this->CurrentAttribute_treeView->SelectedNode )
			return;
		 if( !this->CurrentAttribute_treeView->SelectedNode->Name->Length )
		 {
			 WARNING_MSG("Element doesn't has attribute");
			 return;
		 }
		 System::String^l_pString = GetTextStringFrom(CurrentAttribute_treeView->SelectedNode->Name);
		 if( l_pString->Length )
		 {
			CurrentAttribute_treeView->SelectedNode->Name= l_pString;
		 }	
	}

	System::Void	XMLForm::ReNameTreeNode()
	{
		 if( !this->CurrentAttribute_treeView->SelectedNode )
			return;
		 System::String^l_pString = GetTextStringFrom(CurrentAttribute_treeView->SelectedNode->Text);
		 if( l_pString->Length )
		 {
			if( l_pString->Contains(DNCT::GetChanglineString()) )
			{
				WARNING_MSG("name can't have over 2 linse");
				return;
			}
			//not on top
			if( CurrentAttribute_treeView->SelectedNode->Parent )
			{
				if(!DNCT::IsNodesContainName(l_pString,this->CurrentAttribute_treeView->SelectedNode->Parent->Nodes))
					CurrentAttribute_treeView->SelectedNode->Text = l_pString;
				else
					WARNING_MSG("name exist");
			}
			else
			{
				if(!DNCT::IsNodesContainName(l_pString,this->CurrentAttribute_treeView->Nodes))
					CurrentAttribute_treeView->SelectedNode->Text = l_pString;
				else
					WARNING_MSG("name exist");
			}
		 }
	}

	System::String^	XMLForm::GetTextStringFrom(String^e_str)
	{
		ValueInputForm^	l_p = gcnew ValueInputForm(e_str);
		l_p->ShowDialog();
			return l_p->m_pString;
	}


	System::Void	XMLForm::SearchNodes(GCFORM::TreeNode^e_pNode,System::String^e_pStringToCompare,bool* e_b )
	{
		if( e_pNode->Text->Equals(e_pStringToCompare) )
			*e_b = true;

		if (e_pNode->NextNode)
		{
			SearchNodes(e_pNode->NextNode,e_pStringToCompare,e_b);
		}

		if (e_pNode->Nodes->Count)
		{
			SearchNodes(e_pNode->Nodes[0],e_pStringToCompare,e_b);
		}
	}


	System::Void	XMLForm::WriteInformation(ATG::XMLWriter*e_pWriter)
	{
		__time64_t	l_ExportTime;
		l_ExportTime = _time64( nullptr );
		CHAR* strTime = _ctime64( &( l_ExportTime ) );

		CHAR* pLF = strchr( strTime, '\n' );
		if( pLF != nullptr )
			*pLF = '\0';
		e_pWriter->StartComment();
		e_pWriter->WriteStringFormat( "Time: %s", strTime );
		e_pWriter->EndComment();

		e_pWriter->StartComment();
		e_pWriter->WriteStringFormat( "Exporter: %s",  "Fatming" );
		e_pWriter->EndComment();

		e_pWriter->StartComment();
		char	l_temp[MAX_PATH];
		sprintf(l_temp,"%d\0",this->CurrentAttribute_treeView->GetNodeCount(true));
		e_pWriter->WriteStringFormat( "TotalCount: %s",  l_temp );
		e_pWriter->EndComment();
	}




	System::Void	XMLForm::SaveGameFile(System::String^e_pString)
	{
		char	l_temp[MAX_PATH];
		if(e_pString->Length)
		{
			e_pString = DNCT::GetFileNameWithoutFullPath(e_pString,true);
			e_pString+=".fat";
			GCSTRING_TO_CHAR(e_pString,l_temp);
		}
		else
		{
			System::String^l_pString = DNCT::SaveFileAndGetName();	
			if( l_pString )
			{
				GCSTRING_TO_CHAR(l_pString,l_temp);
			}
			else
				return;
		}
		ATG::XMLWriter     l_XMLWriter(l_temp);
		WriteInformation(&l_XMLWriter);
		l_XMLWriter.StartElement("GameData");
		if(  this->CurrentAttribute_treeView->Nodes->Count )
		{
			 XmlDocument^ d = gcnew XmlDocument();
			 XmlNode^ n = d->CreateNode(XmlNodeType::Element,"root","");
			 for each (TreeNode^ t in CurrentAttribute_treeView->Nodes)
			   n->AppendChild(WriteAllNodes(t, d,(void*)&l_XMLWriter));
		
		}
		l_XMLWriter.EndElement();
		l_XMLWriter.Close();
	}

	bool	XMLForm::SaveXmlFile(System::Object^  sender)
	{
		 System::String^l_pString = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
		if(!m_pStringForFileName||!m_pStringForFileName->Length)
			m_pStringForFileName = l_pString;
		 if( !l_pString||!l_pString->Length )
			 return false;
		 XmlDocument^ d = gcnew XmlDocument();
		 XmlNode^ n;
		 n = d->CreateNode(XmlNodeType::Element,"root","");
		 for each (TreeNode^ t in CurrentAttribute_treeView->Nodes)
		 {
			 n->AppendChild(DNCT::getXmlNode(t, d));
		 }
		 for each(XmlNode^ e_ChildNode in n->ChildNodes)
			d->AppendChild(e_ChildNode);
		  d->Save(l_pString);
		  return true;
	}


//end namespace DrawFont
}



