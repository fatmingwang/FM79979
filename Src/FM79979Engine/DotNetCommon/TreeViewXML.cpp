#include "stdAfx.h"
#include "DotNetCommonTools.h"
#include "TreeViewXML.h"
namespace DNCT
{
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

	XmlNode^ getXmlNode(TreeNode^ e_pTreeNode,XmlDocument^ e_pXmlDocument)
	{
		XmlNode^ l_pXmlNode;
		if( e_pTreeNode->Text->Equals(XML_COMMENT) )
		{
			l_pXmlNode = e_pXmlDocument->CreateComment(e_pTreeNode->Name);
		}
		else
		if( e_pTreeNode->Text->Equals(XML_TEXT) )
		{
			l_pXmlNode = e_pXmlDocument->CreateTextNode(e_pTreeNode->Name);
		}
		else
		{
			//it have name and text so it's attributes
			if( e_pTreeNode->Name->Length>=1 )
			{
				System::Xml::XmlAttribute^l_pAttributes = e_pXmlDocument->CreateAttribute(e_pTreeNode->Text);
				l_pAttributes->Value = e_pTreeNode->Name;
				//l_pAttributes->Name = e_pTreeNode->Text;
				l_pXmlNode = l_pAttributes;
			}
			else
			{
				l_pXmlNode = e_pXmlDocument->CreateNode(XmlNodeType::Element, e_pTreeNode->Text, "");
			}
		}
		if( e_pTreeNode->Nodes->Count )
		{
			System::Xml::XmlComment^l_pComment;
			System::Xml::XmlText^l_pText;
			System::Xml::XmlNode^l_pPerviousNode;
			for each (TreeNode^ t in e_pTreeNode->Nodes)
			{
				if( t->Nodes->Count )
				{
					l_pPerviousNode =  getXmlNode(t,e_pXmlDocument);
					l_pXmlNode->AppendChild(l_pPerviousNode);
				}
				else
				{
					if( t->Text->Equals(XML_COMMENT) )
					{
						l_pComment = e_pXmlDocument->CreateComment(t->Name);
						l_pXmlNode->InsertAfter(l_pComment,l_pPerviousNode);
					}
					if( t->Text->Equals(XML_TEXT) )
					{
						l_pText = e_pXmlDocument->CreateTextNode(t->Name);
						l_pXmlNode->InsertAfter(l_pText,l_pPerviousNode);
					}
					else
					{
						if( t->Name->Length > 0 )//make sure this is attribute
						{
							System::Xml::XmlAttribute^l_pAtt = e_pXmlDocument->CreateAttribute(t->Text);
							l_pAtt->Value = t->Name;
							l_pXmlNode->Attributes->Append(l_pAtt);
						}
						else
						{//it poss ible error if the user set wrong order(attribute after element)
							l_pPerviousNode =  getXmlNode(t,e_pXmlDocument);
							l_pXmlNode->AppendChild(l_pPerviousNode);
						}
					}
				}
			}
		}
		return l_pXmlNode;
	}

	void AddAttributesAndChildren(XmlNode^ inXmlNode, TreeNode^ inTreeNode)
	{
		XmlAttributeCollection^ atts = inXmlNode->Attributes;

        if (atts && atts->Count != 0)
		{
            //TreeNode^ tNode = gcnew TreeNode("(Attributes)");
            for each (XmlAttribute ^att in atts)
			{
				TreeNode^l_pTreeNode = gcnew TreeNode();
				l_pTreeNode->Name = att->Value;
				l_pTreeNode->Text = att->Name;
				inTreeNode->Nodes->Add(l_pTreeNode);
            }
        }

        for each (XmlNode^ xNode in inXmlNode->ChildNodes)
		{
            TreeNode^ tNode;
            switch (xNode->NodeType)
			{
			case XmlNodeType::Element :
                tNode = gcnew TreeNode(xNode->Name);
                AddAttributesAndChildren(xNode, tNode);
                break;
			case XmlNodeType::Text:
                tNode = gcnew TreeNode();
				tNode->ForeColor = System::Drawing::Color::RoyalBlue;
				tNode->Name = xNode->Value;
				tNode->Text = XML_TEXT;
                break;
            case XmlNodeType::Comment:
                tNode = gcnew TreeNode();
				tNode->ForeColor = System::Drawing::Color::Green;
				tNode->Name = xNode->Value;
				tNode->Text = XML_COMMENT;
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

	System::Void	SaveTreeViewToFile(GCFORM::TreeView^e_pTreeView)
	{
		System::String^l_pString = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
		SaveTreeViewToFile(e_pTreeView,l_pString);		
	}

	System::Void	SaveTreeViewToFile(GCFORM::TreeView^e_pTreeView,String^e_strFileName)
	{
		System::String^l_pString = e_strFileName;
		if( l_pString )
		{
			XmlDocument^ d = gcnew XmlDocument();
			XmlNode^ n;
			if( e_pTreeView->Nodes->Count == 1 )
			{
			    n = d->CreateNode(XmlNodeType::Element,e_pTreeView->Nodes[0]->Text,"");
			    for each (TreeNode^ t in e_pTreeView->Nodes[0]->Nodes)
			    {
			        System::Xml::XmlNode^l_pNode = DNCT::getXmlNode(t, d);
					if( l_pNode )
					{
						String^l_strType = l_pNode->ToString();
						if(  l_strType->CompareTo("System.Xml.XmlAttribute") == 0 )
						{
							n->Attributes->Append((System::Xml::XmlAttribute^)l_pNode);
						}
						else
							n->AppendChild(l_pNode);
					}
			    }			    
			}
			else
			{
			    n = d->CreateNode(XmlNodeType::Element,"Root","");
			    for each (TreeNode^ t in e_pTreeView->Nodes)
			    {
			        System::Xml::XmlNode^l_pNode = DNCT::getXmlNode(t, d);
				    n->AppendChild(l_pNode);
			    }
			}
			d->AppendChild(n);
			TextWriter^ sw = gcnew StreamWriter(l_pString, false, System::Text::Encoding::Unicode);
			{
				d->Save(sw);
			}
			sw->Close();
			////d->Save(l_pString);
		}	
	}

	System::Void	SaveTreeNodeToFile(GCFORM::TreeNode^e_pTreeNode)
	{
		System::String^l_pString = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
		SaveTreeNodeToFile(e_pTreeNode,l_pString);		
	}

	System::Void	SaveTreeNodeToFile(GCFORM::TreeNode^e_pTreeNode,String^e_strFileName)
	{
		if( e_strFileName->Length )
		{
			XmlDocument^ d = gcnew XmlDocument();
			XmlNode^ n = d->CreateNode(XmlNodeType::Element,"root","");
			for each (TreeNode^ t in e_pTreeNode->Nodes)
			{
				n->AppendChild(DNCT::getXmlNode(t, d));
			}
			d->AppendChild(n);
			TextWriter^ sw = gcnew StreamWriter(e_strFileName, false, System::Text::Encoding::Unicode);
			{
				d->Save(sw);
			}
			sw->Close();
			//d->Save(e_strFileName);
		}	
	}

	System::Xml::XmlDataDocument^OpenXmlFile(String^e_strFileName,bool e_bUnidoe)
	{
		System::String^l_pStringForFileName;
		if( !e_strFileName )
			l_pStringForFileName = DNCT::OpenFileAndGetName("xml files (*.path)|*.path|xml files (*.xml)|*.xml|All files (*.*)|*.*");
		else
			l_pStringForFileName = e_strFileName;
		if( l_pStringForFileName )
		{
			if(!System::IO::File::Exists(l_pStringForFileName))
				return nullptr;
			System::Xml::XmlDataDocument^ xmldoc = gcnew System::Xml::XmlDataDocument();
			System::IO::StreamReader ^sr = gcnew  System::IO::StreamReader(l_pStringForFileName,System::Text::Encoding::ASCII);
			System::String^l_pString = sr->ReadToEnd();
			try 
			{
				xmldoc->LoadXml(l_pString);
			}
			catch(XmlException^ xmlEx)
			{
				xmlEx->Message;
				sr->Close();
				delete sr;
				try
				{
					sr = gcnew  System::IO::StreamReader(l_pStringForFileName,System::Text::Encoding::Unicode);
					l_pString = sr->ReadToEnd();
					xmldoc->LoadXml(l_pString);
				}
				catch(Exception^ ex)
				{
					sr->Close();
					delete sr;
					WARNING_MSG(ex->Message);
					return nullptr;
				}
				if( !xmldoc->DocumentElement )
					WARNING_MSG(xmlEx->Message);
			}
			sr->Close();
			delete sr;
			return xmldoc;
		}
		return nullptr;
	}

	GCFORM::TreeNode^	OpenXmlFileToNode(String^e_strFileName,bool e_bUnidoe)
	{
		TreeNode^ tNode;
		System::String^l_pStringForFileName;
		if( !e_strFileName )
			l_pStringForFileName = DNCT::OpenFileAndGetName("xml files (*.path)|*.path|xml files (*.xml)|*.xml|All files (*.*)|*.*");
		else
			l_pStringForFileName = e_strFileName;
		if( l_pStringForFileName )
		{
			
			System::Xml::XmlDataDocument^ xmldoc = OpenXmlFile(e_strFileName,e_bUnidoe);
			if( xmldoc && xmldoc->DocumentElement )
			{
				tNode = gcnew GCFORM::TreeNode(xmldoc->DocumentElement->Name);
				String^l_strName = xmldoc->DocumentElement->Name;
				if( xmldoc->DocumentElement->Attributes->Count == 0 )//skip root start from its children
				{
					for each( System::Xml::XmlNode^l_pXmlNode in xmldoc->DocumentElement->ChildNodes)
					{
						GCFORM::TreeNode^l_pNode = gcnew GCFORM::TreeNode(l_pXmlNode->Name);
						DNCT::AddAttributesAndChildren(l_pXmlNode, l_pNode);
						tNode->Nodes->Add(l_pNode);
					}
				}
				else
					DNCT::AddAttributesAndChildren(xmldoc->DocumentElement, tNode);
				delete xmldoc;
			}
		}				
		return tNode;
	}
}