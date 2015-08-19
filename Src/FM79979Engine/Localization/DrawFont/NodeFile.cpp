#include "StdAfx.h"
#include "NodeFile.h"

System::Void	NodeFile::LoadFile(System::String^e_pFileName,System::String^e_pStringForPath)
{
	System::String^l_pStringForCurrentDirectory = (e_pStringForPath+="\\Fun\\");
	System::String^l_pStringForFileName = l_pStringForCurrentDirectory+e_pFileName;
	try
	{
		System::IO::StreamReader ^sr = gcnew  System::IO::StreamReader(l_pStringForFileName);
		System::String^l_pString = sr->ReadLine();
		while(l_pString)
		{				
			System::String^l_pStringForListName = l_pStringForCurrentDirectory+l_pString;
			GCFORM::TreeNode^l_pTreeNode = DNCT::OpenXmlFileGetAllTagToNode(l_pStringForCurrentDirectory+l_pString);
			l_pTreeNode->Name = DNCT::GetFileNameWithoutFullPath(l_pStringForListName,true);
			m_pHashTable->Add(l_pTreeNode->Name,l_pTreeNode);
			l_pString = sr->ReadLine();
		}
	}
	catch(Exception^ ex)
	{
		WARNING_MSG("Default.fun not exist or direction error \n  \n  \n  \n  \n "+ex->ToString());
	}	
}

System::Void	NodeFile::WriteFile(System::String^e_pStringForPath)
{
	System::String^l_pStringForCurrentDirectory = e_pStringForPath;
	l_pStringForCurrentDirectory+="\\Fun";
	System::String^	l_pStringForDefault = gcnew System::String("");
	System::String^l_pStringForOutputName;
	System::IO::Directory::CreateDirectory(l_pStringForCurrentDirectory);
	l_pStringForCurrentDirectory+="\\";
	for each ( GCFORM::TreeNode^l_pNode  in m_pHashTable->Values )
	{
		XmlDocument^ d = gcnew XmlDocument();
		XmlNode^ n = d->CreateNode(XmlNodeType::Element,"root","");
		if(!l_pNode->Text->Equals("root"))
		{//new one have no self tag so add from himself
			n->AppendChild(DNCT::getXmlNode(l_pNode, d));
			
			l_pStringForOutputName = l_pStringForCurrentDirectory+l_pNode->Text+TREENODE_PLUG_EXTENSION_FILE_TYPE;
		}
		else
		{//old one it contained the tag so just add all his child
			for each (TreeNode^ t in l_pNode->Nodes)
			{
				GCFORM::TreeNode^	l_p = t;
				n->AppendChild(DNCT::getXmlNode(t, d));
			}
			l_pStringForOutputName = l_pStringForCurrentDirectory+l_pNode->Nodes[0]->Text+TREENODE_PLUG_EXTENSION_FILE_TYPE;
		}
		d->AppendChild(n);
		d->Save(l_pStringForOutputName);
		l_pStringForDefault = l_pStringForDefault+ DNCT::GetFileNameWithoutFullPath(l_pStringForOutputName,true)+TREENODE_PLUG_EXTENSION_FILE_TYPE+DNCT::GetChanglineString();
	}
	DNCT::StreamWriteFileFromString(l_pStringForDefault,l_pStringForCurrentDirectory+"default"+TREENODE_PLUG_EXTENSION_FILE_TYPE,System::Text::Encoding::Unicode);
}