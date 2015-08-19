#include "stdafx.h"
#include "Form1.h"
#include "time.h"
namespace XMLDataEditor
{
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


	System::Void	Form1::AddAttributeIntoTreeNode()
	{
		 GCFORM::TreeNode^l_pSelectedNode = MainNode_treeView->SelectedNode;
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

	System::Void	Form1::AddTextIntoTreeNode()
	{
		GCFORM::TreeNode^l_pSelectedNode = MainNode_treeView->SelectedNode;
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

	System::Void	Form1::AddComment()
	{
		GCFORM::TreeNode^l_pSelectedNode = MainNode_treeView->SelectedNode;
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

	System::Void	Form1::AddElementIntoTreeNode()
	{
		GCFORM::TreeNode^l_pSelectedNode = this->MainNode_treeView->SelectedNode;
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
			MainNode_treeView->Nodes->Add(l_pNewNode);
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

	System::Void	Form1::DeleteTreeNode()
	{
		if( !MainNode_treeView->SelectedNode )
			return;
		if( WARING_YES_NO("This will delete all nodes child!!") == GCFORM::DialogResult::Yes )
			this->MainNode_treeView->SelectedNode->Remove();
	}
	System::Void	Form1::ModifyTreeNodeData()
	{
		 if( !this->MainNode_treeView->SelectedNode )
			return;
		 if( !this->MainNode_treeView->SelectedNode->Name->Length )
		 {
			 WARNING_MSG("Element doesn't has attribute");
			 return;
		 }
		 System::String^l_pString = GetTextStringFrom(MainNode_treeView->SelectedNode->Name);
		 if( l_pString->Length )
		 {
			MainNode_treeView->SelectedNode->Name= l_pString;
		 }	
	}

	System::Void	Form1::ReNameTreeNode()
	{
		 if( !this->MainNode_treeView->SelectedNode )
			return;
		 System::String^l_pString = GetTextStringFrom(MainNode_treeView->SelectedNode->Text);
		 if( l_pString->Length )
		 {
			if( l_pString->Contains(DNCT::GetChanglineString()) )
			{
				WARNING_MSG("name can't have over 2 linse");
				return;
			}
			//not on top
			if( MainNode_treeView->SelectedNode->Parent )
			{
				if(!DNCT::IsNodesContainName(l_pString,this->MainNode_treeView->SelectedNode->Parent->Nodes))
					MainNode_treeView->SelectedNode->Text = l_pString;
				else
					WARNING_MSG("name exist");
			}
			else
			{
				if(!DNCT::IsNodesContainName(l_pString,this->MainNode_treeView->Nodes))
					MainNode_treeView->SelectedNode->Text = l_pString;
				else
					WARNING_MSG("name exist");
			}
		 }
	}

	System::String^	Form1::GetTextStringFrom(String^e_str)
	{
		ValueInputForm^	l_p = gcnew ValueInputForm(e_str);
		l_p->ShowDialog();
			return l_p->m_pString;
	}


	System::Void	Form1::SearchNodes(GCFORM::TreeNode^e_pNode,System::String^e_pStringToCompare,bool* e_b )
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

	GCFORM::TreeNode^Form1::OpenFileGetTreeNode(String^e_strFileName)
	{
		GCFORM::TreeNode^l_pNode;
		String^l_strFileName = e_strFileName;
		if( !l_strFileName )
			 return l_pNode;
		l_pNode = DNCT::OpenXmlFileToNode(l_strFileName,this->Unicode_checkBox->Checked);
		if( l_pNode )
		{
			 return l_pNode;
		}
		else
		{
			 std::string	l_str = DNCT::GcStringToChar(l_strFileName);
			 String^l_strTempFileName = l_strFileName+"b";
			 std::string	 l_strTempFileName2 = DNCT::GcStringToChar(l_strTempFileName);
			 if(FileDempressHuffman(l_str.c_str(),l_strTempFileName2.c_str(),this->m_iBinaryHeaderID))
			 {
				 l_pNode = DNCT::OpenXmlFileToNode(l_strTempFileName,this->Unicode_checkBox->Checked);
				 System::IO::File::Delete(l_strTempFileName);
			 }
		}
		return l_pNode;
	}

	bool	Form1::SaveXmlFile(System::Object^  sender)
	{
		m_bFileChanged = false;
		if( System::IO::File::Exists(this->Text) && (sender == Save_toolStripButton || SaveToolStripMenuItem == sender) )
		{
			DNCT::SaveTreeViewToFile(MainNode_treeView,this->Text);
		}
		else
			DNCT::SaveTreeViewToFile(MainNode_treeView);
		 //System::String^l_pString;
		 //if( SaveToolStripMenuItem == sender )
		 //{
			// if( m_pStringForFileName && m_pStringForFileName->Length)
			//	 l_pString = m_pStringForFileName;
			// else
			// {
			//	m_pStringForFileName = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
			//	l_pString = m_pStringForFileName;
			// }
		 //}
		 //else
		 //{
			//l_pString = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
			//if(!m_pStringForFileName||!m_pStringForFileName->Length)
			//	m_pStringForFileName = l_pString;
		 //}
		 //if( !l_pString||!l_pString->Length )
			// return false;
		 //XmlDocument^ d = gcnew XmlDocument();
		 //XmlNode^ n;
		 //n = d->CreateNode(XmlNodeType::Element,"root","");
		 //for each (TreeNode^ t in MainNode_treeView->Nodes)
		 //{
			// n->AppendChild(DNCT::getXmlNode(t, d));
		 //}
		 //for each(XmlNode^ e_ChildNode in n->ChildNodes)
			//d->AppendChild(e_ChildNode);
		 ////d->Save(l_pString);
		 //System::IO::StreamWriter^l_pStreamWriter = gcnew System::IO::StreamWriter(l_pString,false,Unicode_checkBox->Checked?System::Text::Encoding::Unicode:System::Text::Encoding::ASCII);
		 //d->Save(l_pStreamWriter);
		 //l_pStreamWriter->Close();
		 return true;
	}


//end namespace DrawFont
}



