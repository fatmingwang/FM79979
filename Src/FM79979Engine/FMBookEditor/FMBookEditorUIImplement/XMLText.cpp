#include "StdAfx.h"
#include "XMLText.h"

namespace FMBookEditor
{
	System::Void XMLText::AddText_button_Click(System::Object^  sender, System::EventArgs^  e)
    {
		array<String^>^l_strFileNames = OpenFileAndGetNames("xml files (*.xml)|*.xml|All files (*.*)|*.*");
		for each(String^ l_strFileName in l_strFileNames)
		{
			String^l_strFileName2 = System::IO::Path::GetFileName(l_strFileName);
			//if( System::IO::Path::GetExtension(l_strFileName2->ToLower())->Equals(".xml") )
			{
				GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_strFileName);
				if( l_pNode )
				{
					if( !m_pFileList->Items->Contains(l_strFileName2) )
					{
						m_pTreeViewList->Add(l_pNode);
						m_pFileList->Items->Add(l_strFileName2);
					}
					else
					{
						//WARNING_MSG("exists",l_strFileName2);
					}
				}
			}
		}
    }
	System::Void XMLText::DelText_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if( m_pFileList->SelectedItem )
		{
			int	l_iIndex = m_pFileList->Items->IndexOf(m_pFileList->SelectedItem);
			m_pTreeViewList->RemoveAt(l_iIndex);
			m_pFileList->Items->RemoveAt(l_iIndex);	
		}
	}

	System::Void XMLText::Text_ListBox_SelectedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if( m_pFileList->SelectedItem )
		{
			m_pTreeView->Nodes->Clear();
			int	l_iIndex = m_pFileList->Items->IndexOf(m_pFileList->SelectedItem);
			GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)m_pTreeViewList[l_iIndex];
			m_pTreeView->Nodes->Add(l_pNode);
		}
		else
		{
			m_pTreeView->Nodes->Clear();
		}
	}

	System::Void XMLText::TreeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
	{
		if( m_pTreeView->SelectedNode )
		{
			String^l_strFullPath = m_pTreeView->SelectedNode->FullPath;
			m_TextBox->Text = m_pTreeView->SelectedNode->Name;

			*m_pstrCurrentText = DNCT::GcStringToWchar(m_TextBox->Text);
			*m_pstrCurrentFileName = DNCT::GcStringToWchar(m_pFileList->SelectedItem->ToString());
			*m_pstrCurrentNode = DNCT::GcStringToWchar(l_strFullPath);
		}
		else
		{
			m_TextBox->Text = "\0";
			*m_pstrCurrentText = L"";
			*m_pstrCurrentFileName = L"";
			*m_pstrCurrentNode = L"";
		}
		if( TextChanged )
			TextChanged(sender,e);
	}

	void	XMLText::Refresh()
	 {
		m_TextBox->Text = "";
		m_pTreeView->Nodes->Clear();
		m_pFileList->Items->Clear();
		m_pTreeViewList->Clear();
	 }
}