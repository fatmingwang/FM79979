#include "stdafx.h"
#include "DotNetCommonTools.h"
namespace DNCT
{
	char*	GcStringToChar(System::String^e_str)
	{
		if( !e_str )
			WARNING_MSG("input gcstring string is empty");
		static char	l_sStr[MAX_PATH];
		GCSTRING_TO_CHAR(e_str,l_sStr);
		return l_sStr;
	}
	WCHAR*	GcStringToWchar(System::String^e_str)
	{
		if( !e_str )
			WARNING_MSG("input gcstring string is empty");
		static WCHAR	l_sStr[MAX_PATH];
		GCSTRING_TO_WCHAR(e_str,l_sStr);
		return l_sStr;	
	}

	System::Object ^ HashTableGetValue(System::String^ e_p,System::Collections::Hashtable^ e_pHashTable)
	{
		assert(e_p);
		System::Object ^l_pObject;
		int	l_iKey = 0;
		if(e_pHashTable->ContainsKey(e_p))
		{
			for each (System::Object^ l_p in e_pHashTable->Keys)
			{
				l_iKey++;
				if(l_p->ToString()->Equals(e_p))		
					break;
			}
			for each (System::Object^ l_p in e_pHashTable->Values)
			{
				l_iKey--;
				if(!l_iKey)
					return l_p;
			}
		}
		return l_pObject;
	}

	System::Void	ReplaceValuInHashTable(System::String^e_pKey,System::Object^e_pNewValue,System::Collections::Hashtable^e_pHashTable)
	{
		if( !e_pHashTable[e_pKey] )
			assert(0&&"no such key");
		e_pHashTable->Remove(e_pKey);
		e_pHashTable->Add(e_pKey,e_pNewValue);
	}

	System::Windows::Forms::TextBox^	CreateDefaultTextBox(char*e_pName,int	e_iXPOs,int e_iYPos)
	{
		System::Windows::Forms::TextBox^	l_pTextBox = gcnew 	System::Windows::Forms::TextBox();
		l_pTextBox->Name = gcnew System::String(e_pName);
		l_pTextBox->Size = System::Drawing::Size(160,20);
		l_pTextBox->Location = System::Drawing::Point(e_iXPOs,e_iYPos);
		return l_pTextBox;
	}

	System::Windows::Forms::ComboBox^	CreateDefaultCombox(char*e_pName,System::Collections::ArrayList^ e_pStringForText,int	e_iPosX,int e_iPosY)
	{
		System::Windows::Forms::ComboBox^	l_pComboBox = gcnew System::Windows::Forms::ComboBox;
		l_pComboBox->Location = System::Drawing::Point(e_iPosX,e_iPosY);
		l_pComboBox->Name = TO_GCSTRING(e_pName);
		l_pComboBox->Size = System::Drawing::Size(100, 40);
		if(e_pStringForText)
		for each ( System::String^	l_pString in e_pStringForText)
			l_pComboBox->Items->Add( l_pString );
		return l_pComboBox;
	}
	System::Windows::Forms::ListBox^	CreateDefaultListBox(System::String^e_pString,System::Windows::Forms::Control::ControlCollection^ e_p,int e_iPosX,int e_iPosY)
	{
		System::Windows::Forms::ListBox^	l_pBox = gcnew System::Windows::Forms::ListBox();
		l_pBox->HorizontalScrollbar = true;
		l_pBox->ItemHeight = 12;
		int l_iPosX = 0;
		if(e_p)
			l_iPosX = e_p->Count*30;
		l_pBox->Location = System::Drawing::Point(e_iPosX+l_iPosX,e_iPosY);
		l_pBox->Name = e_pString;
		l_pBox->Size = System::Drawing::Size(150, 100);
		l_pBox->FormattingEnabled = true;
		l_pBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;	
		if(e_p)
			e_p->Add(l_pBox);
		return l_pBox;
	}


	GCFORM::ListBox^	GetActuallyListBoxFromHashTable(System::String^e_pString,System::Collections::Hashtable^ e_pHashTable,System::Windows::Forms::Control::ControlCollection^ e_p)
	{
		GCFORM::ListBox^	l_pListBox = (GCFORM::ListBox^)DNCT::HashTableGetValue(e_pString,e_pHashTable);
		if(!l_pListBox)
		{
			l_pListBox = CreateDefaultListBox(e_pString,e_p);
			e_pHashTable->Add(e_pString,l_pListBox);
			return l_pListBox;
		}
		return l_pListBox;
	}

	System::Windows::Forms::ListBox^	CreateDefaultListBox(char*e_pString,System::Windows::Forms::Control::ControlCollection^ e_p,int e_iPosX,int e_iPosY)
	{
		return CreateDefaultListBox(gcnew System::String(e_pString),e_p,e_iPosX,e_iPosY);
	}

	System::Windows::Forms::Form^	CreateTempFromToDecideSelect( char*e_pAcceptString,char*e_pChanelString,System::Collections::ArrayList^ e_pArrayList)
	{	
	// Create a new instance of the form.
	   System::Windows::Forms::Form^ form1 = gcnew System::Windows::Forms::Form;
	   // Create two buttons to use as the accept and cancel buttons.
	   System::Windows::Forms::Button^ button1 = gcnew System::Windows::Forms::Button ();
	   System::Windows::Forms::Button^ button2 = gcnew System::Windows::Forms::Button ();
	  
	  // Set the text of button1 to "OK".
	   button1->Text = gcnew System::String( e_pAcceptString );
	  // Set the position of the button on the form.
	  button1->Location = System::Drawing::Point(70,80);
	  // Set the text of button2 to "Cancel".
	  button2->Text = gcnew System::String( e_pChanelString );
	  // Set the position of the button based on the location of button1.
	  button2->Location =
		 System::Drawing::Point( button1->Left, button1->Height + button1->Top + 10 );
	  // Set the caption bar text of the form.   
	  form1->Text = "Dialog Box";
	  // Display a help button on the form.
	  form1->HelpButton = true;
	  
	  // Define the border style of the form to a dialog box.
	  form1->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
	  // Set the MaximizeBox to false to remove the maximize box.
	  form1->MaximizeBox = false;      
	  // Set the MinimizeBox to false to remove the minimize box.
	  form1->MinimizeBox = false;
	  // Set the accept button of the form to button1.
	  form1->AcceptButton = button1;
	  // Set the cancel button of the form to button2.
	  form1->CancelButton = button2;
	  // Set the start position of the form to the center of the screen.
	  form1->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
	  
	  // Add button1 to the form.
	  form1->Controls->Add( button1 );
	  // Add button2 to the form.
	  form1->Controls->Add( button2 );
	  // Display the form as a modal dialog box.  
	  int i=0;
	  for each ( System::Windows::Forms::Control^	l_pControl in e_pArrayList)
	  {	  
		  form1->Controls->Add( (System::Windows::Forms::Control^)e_pArrayList[i] );
		  i++;
	  }
	//      form1->ShowDialog();
	  return form1;

	}

	System::String^	OpenFileAndGetName(char*e_pFileFilter)
	{
		System::Windows::Forms::OpenFileDialog^	openFileDialog1 = gcnew System::Windows::Forms::OpenFileDialog();
		openFileDialog1->Multiselect = false;
		if(e_pFileFilter)
			openFileDialog1->Filter = TO_GCSTRING(e_pFileFilter);

		if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			return openFileDialog1->FileName;
		}
		System::String ^l_String = "";
		return l_String;
	}

	array<String^>^OpenFileAndGetNames(char*e_pFileFilter)
	{
		System::Windows::Forms::OpenFileDialog^	openFileDialog1 = gcnew System::Windows::Forms::OpenFileDialog();
		openFileDialog1->Multiselect = true;
		if(e_pFileFilter)
			openFileDialog1->Filter = TO_GCSTRING(e_pFileFilter);

		if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			return openFileDialog1->FileNames;
		}
		array<String^>^ l_String;
		return l_String;
	}


	System::String^	SaveFileAndGetName(char*e_pFileFilter)
	{
		System::Windows::Forms::SaveFileDialog^	SaveFileDialog1 = gcnew System::Windows::Forms::SaveFileDialog();
		if(e_pFileFilter)
			SaveFileDialog1->Filter = TO_GCSTRING(e_pFileFilter);

		if(SaveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			return SaveFileDialog1->FileName;
		}
		System::String ^l_String = "";
		return l_String;
	}
//==================================================
//
//==================================================
	System::String^	OpenFileGetText()
	{
		String^l_pString = DNCT::OpenFileAndGetName();
		System::IO::StreamReader^l_pReader = gcnew System::IO::StreamReader(l_pString,System::Text::Encoding::GetEncoding(TO_GCSTRING("UTF-16")));
		return l_pReader->ReadToEnd();
	}

	System::String^	GetChanglineString()
	{
		wchar_t l_temp[3] = {13,10,'\0'};
		return TO_GCSTRING(l_temp);
	}

	bool	CheckListContainStringAndAdd(GCFORM::ListBox^e_pListBox,System::String^e_pString)
	{
		if(!e_pListBox->Items->Contains(e_pString))
		{
			e_pListBox->Items->Add(e_pString);
			return false;
		}
		return true;
	}


	bool	ReplaceListContainString(GCFORM::ListBox^e_pListBox,System::String^e_pString)
	{
		int l_i = e_pListBox->SelectedIndex;
		if( e_pListBox->Items->Contains(e_pString)||l_i==-1 )
			return false;
		e_pListBox->Items->RemoveAt(l_i);
		e_pListBox->Items->Insert(l_i,e_pString);
		return true;
	}

	System::Void	StreamWriteFileFromString(System::String^ e_pString,System::String^e_pFileName,System::Text::Encoding^ e_pEncoding)
	{
		System::IO::FileStream ^sb;
		if(e_pFileName!="")
			sb = gcnew System::IO::FileStream(e_pFileName, System::IO::FileMode::OpenOrCreate);
		else
		{
			System::String^l_pStringFileName = DNCT::SaveFileAndGetName(0);
			if( !l_pStringFileName->Length )
				return;
			sb = gcnew System::IO::FileStream(l_pStringFileName, FileMode::Create);
		}
		System::IO::StreamWriter ^l_pStreamWriter = gcnew  System::IO::StreamWriter(sb,e_pEncoding);
		l_pStreamWriter->Write(e_pString);
		l_pStreamWriter->Close();
	}

	System::Drawing::Font^ GetFontFromFontDialog()
	{
		GCFORM::FontDialog^l_pFontDialog = gcnew GCFORM::FontDialog();
		DIALOG_OK(l_pFontDialog)
		{
			return l_pFontDialog->Font;
		}
		System::Drawing::Font^l_pFont;
		return l_pFont;
	}
	//===============================================
	//
	//===============================================
	bool	IsNodesContainName(System::String^e_pString,GCFORM::TreeNodeCollection^e_pNodeCollection)
	{
		for each (GCFORM::TreeNode^l_pNode in e_pNodeCollection)
		{
			if( e_pString->Equals( l_pNode->Text ) )
				return true;
		}
		return false;
	}

	bool	TreeNodeAddChild(System::String^e_pString,GCFORM::TreeNode^e_pTreeNode)
	{
		if(!e_pTreeNode)
			return false;
		if(!IsNodesContainName(e_pString,e_pTreeNode->Nodes))
		{
			e_pTreeNode->Nodes->Add(e_pString);
			return true;
		}
		return false;
	}

	bool	TreeViewAddRoot(System::String^e_pString,GCFORM::TreeView^e_pTreeView)
	{
		if(!IsNodesContainName(e_pString,e_pTreeView->Nodes))
		{
			e_pTreeView->Nodes->Add(e_pString);
			return true;
		}
		return false;
	}

	void	SearchAllNodes(GCFORM::TreeNode^e_pNode,void*e_pVoid,TREE_NODE_CALL_BACK e_CallBack)
	{
		e_CallBack(e_pNode,e_pVoid);
		if (e_pNode->Nodes->Count)
		{
			SearchAllNodes(e_pNode->Nodes[0],e_pVoid,e_CallBack);
		}

		if (e_pNode->NextNode)
		{
			SearchAllNodes(e_pNode->NextNode,e_pVoid,e_CallBack);
		}
	}

	System::String^	GetFileNameWithoutFullPath(System::String^e_pString,bool e_bStripExtensionFileName)
	{
		System::String^l_pString;
		int l_iForStripExtensionFileName = 0;
		for(int i=e_pString->Length-1;i!=0;--i)
		{
			if( e_bStripExtensionFileName )
			{
				if( e_pString[i] == L'.' )
					l_iForStripExtensionFileName = e_pString->Length-i;
			}

			if( e_pString[i] == L'\\' )
			{
				l_pString = e_pString->Substring(i+1,e_pString->Length-i-1-l_iForStripExtensionFileName);
				return l_pString;
			}
		}		
		return l_pString;
	}

	int		GetSlashCount(System::String^e_pString)
	{
		int l_iCount = 0;
		for( int i = 0;i<e_pString->Length;i++ )
		{
			if( e_pString[i] == L'\\' )
				l_iCount++;
		}
		return l_iCount;
	}

	GCFORM::TreeNode^	OpenXmlFileGetAllTagToNode(System::String^e_pString)
	{
		GCFORM::TreeNode^l_pTreeNode;
		System::String^l_pStringForFileName;
		if( e_pString->Length )
			l_pStringForFileName = e_pString;
		else
			l_pStringForFileName = DNCT::OpenFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
		System::Xml::XmlDataDocument^ xmldoc = gcnew System::Xml::XmlDataDocument();
		System::IO::StreamReader ^sr = gcnew  System::IO::StreamReader(l_pStringForFileName);
		System::String^l_pString = sr->ReadToEnd();
		try 
		{
			// SECTION 1. Create a DOM Document and load the XML data into it.
			xmldoc->LoadXml(l_pString);

			// SECTION 2. Initialize the TreeNode control.
			l_pTreeNode = gcnew TreeNode(xmldoc->DocumentElement->Name);
			// SECTION 3. Populate the TreeView with the DOM nodes.
			AddAttributesAndChildren(xmldoc->DocumentElement, l_pTreeNode);			
		}
		catch(XmlException^ xmlEx)
		{
			WARNING_MSG(xmlEx->Message);
		}
		catch(Exception^ ex)
		{
			WARNING_MSG(ex->Message);
		}
		return l_pTreeNode;
	}

	System::Void AddNode(XmlNode^ inXmlNode, TreeNode^ inTreeNode)
	{
         XmlNode^ xNode;
         TreeNode^ tNode;
         XmlNodeList^ nodeList;
         int i;

         // Loop through the XML nodes until the leaf is reached.
         // Add the nodes to the TreeView during the looping process.
         if (inXmlNode->HasChildNodes)
         {
            nodeList = inXmlNode->ChildNodes;
            for(i = 0; i<=nodeList->Count - 1; i++)
            {
               xNode = inXmlNode->ChildNodes[i];
			   inTreeNode->Nodes->Add(gcnew TreeNode(xNode->Name));
			   GCFORM::TreeNode^l_pNode = inTreeNode->Nodes[inTreeNode->Nodes->Count-1];
			   l_pNode->Text = xNode->Name;
               tNode = inTreeNode->Nodes[i];
               AddNode(xNode, tNode);
            }
         }
         else
         {
            // Here you need to pull the data from the XmlNode based on the
            // type of node, whether attribute values are required, and so forth.
            //inTreeNode->Text = (inXmlNode->OuterXml)->Trim();
			 inTreeNode->Text = (inXmlNode->Name)->Trim();

         }
      }

	XmlNode^ getXmlNode(TreeNode^ tnode,XmlDocument^ d)
	{
		XmlNode^ n = d->CreateNode(XmlNodeType::Element, tnode->Text, "");
		if( tnode->Nodes->Count )
		{
			int	l_i = 0;
			for each (TreeNode^ t in tnode->Nodes)
			{
				if( t->Nodes->Count )
					n->AppendChild(getXmlNode(t,d));
				else
				{
					GCFORM::TreeNode^l_pNodeParent = t->Parent->Nodes[0];
					bool	l_bLevel = false;
					while(l_pNodeParent)
					{
						if(l_pNodeParent->Nodes->Count)
							l_bLevel = true;
						l_pNodeParent = l_pNodeParent->NextNode;
					}
					if( l_bLevel )
					{
						n->AppendChild(d->CreateNode(XmlNodeType::Element, t->Text, ""));
					}
					else
					{
						char	l_temp[MAX_PATH];
						sprintf_s(l_temp,MAX_PATH,"Name%d\0",l_i);
						System::Xml::XmlAttribute^l_pAtt = d->CreateAttribute(gcnew System::String(l_temp));
						l_pAtt->Value = t->Text;
						n->Attributes->Append(l_pAtt);
						l_i++;
					}
				}
			}
		}
		return n;
	}

	void AddAttributesAndChildren(XmlNode^ inXmlNode, TreeNode^ inTreeNode) 
	{
		XmlAttributeCollection^ atts = inXmlNode->Attributes;

        if (atts->Count != 0) {
            //TreeNode^ tNode = gcnew TreeNode("(Attributes)");
            for each (XmlAttribute ^att in atts) {
				inTreeNode->Nodes->Add(gcnew TreeNode(att->Value));
            }
        }

        for each (XmlNode^ xNode in inXmlNode->ChildNodes) {
            TreeNode^ tNode;
            switch (xNode->NodeType) {
			case XmlNodeType::Element :
                tNode = gcnew TreeNode(xNode->Name);
                AddAttributesAndChildren(xNode, tNode);
                break;
			case XmlNodeType::Text:
                tNode = gcnew TreeNode(xNode->Value);
                break;
            case XmlNodeType::Comment:
                tNode = gcnew TreeNode(xNode->Value);
                break;
            case XmlNodeType::ProcessingInstruction:
                tNode = gcnew TreeNode(xNode->Value);
                break;
            case XmlNodeType::CDATA:
                tNode = gcnew TreeNode(xNode->Value);
                break;				
            default :
				assert(0);
//                throw gcnew Exception("Unexpected NodeType: " + xNode->NodeType->ToString());
            }
            inTreeNode->Nodes->Add(tNode);
        }
    }

	System::Void	SaveTreeNodeToFile(GCFORM::TreeNode^e_pTreeNode)
	{
		System::String^l_pString = DNCT::SaveFileAndGetName();
		if( l_pString->Length )
		{
			XmlDocument^ d = gcnew XmlDocument();
			XmlNode^ n = d->CreateNode(XmlNodeType::Element,"root","");
			for each (TreeNode^ t in e_pTreeNode->Nodes)
			{
				n->AppendChild(DNCT::getXmlNode(t, d));
			}
			d->AppendChild(n);
			d->Save(l_pString);
		}
	}

	GCFORM::TreeNode^	OpenXmlFileToNode()
	{
		TreeNode^ tNode;
		System::String^l_pStringForFileName = DNCT::OpenFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
		if( l_pStringForFileName->Length>0 )
		{
			System::Xml::XmlDataDocument^ xmldoc = gcnew System::Xml::XmlDataDocument();
			System::IO::StreamReader ^sr = gcnew  System::IO::StreamReader(l_pStringForFileName);
			System::String^l_pString = sr->ReadToEnd();
			try 
			{
				// SECTION 1. Create a DOM Document and load the XML data into it.
				xmldoc->LoadXml(l_pString);

				// SECTION 2. Initialize the TreeView control.
				GCFORM::TreeView^l_pTreeView = gcnew GCFORM::TreeView();
				l_pTreeView->Nodes->Add(gcnew TreeNode(xmldoc->DocumentElement->Name));
				tNode = l_pTreeView->Nodes[0];
				// SECTION 3. Populate the TreeView with the DOM nodes.
				DNCT::AddAttributesAndChildren(xmldoc->DocumentElement, tNode);
			}
			catch(XmlException^ xmlEx)
			{
				WARNING_MSG(xmlEx->Message);
			}
			catch(Exception^ ex)
			{
				WARNING_MSG(ex->Message);
			}
			sr->Close();
		}				
		return tNode;
	}

//end DNCT
}