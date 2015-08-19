#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

//#undef GetCurrentDirectory

#include "XMLForm.h"
namespace XMLDataEditor 
{
	/// <summary>
	/// Summary for TreeViewToXMLNode
	/// </summary>
	public ref class TreeViewToXMLNode : public System::Windows::Forms::UserControl
	{
	public:
		TreeViewToXMLNode(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			array<String^>^ l_strFiles = System::IO::Directory::GetFiles(System::IO::Directory::GetCurrentDirectory());
			for each(String^ l_strFileName in l_strFiles)
			{
				String^l_strFileName2 = System::IO::Path::GetFileName(l_strFileName);
				if( System::IO::Path::GetExtension(l_strFileName2->ToLower())->Equals(".xml") )
				{
					GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_strFileName);
					if( l_pNode )
					{
						treeView->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
					}
				}
				// now treat all subdirectory and their related files (called this routine recursively)
			}
			xmlForm1 = gcnew XMLForm();
			splitContainer1->Panel2->Controls->Add(xmlForm1);
			xmlForm1->Dock = GCFORM::DockStyle::Fill;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TreeViewToXMLNode()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::TreeView^  treeView;
	public:	XMLForm^xmlForm1;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::ComponentModel::IContainer^  components;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->treeView = (gcnew System::Windows::Forms::TreeView());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// treeView
			// 
			this->treeView->AllowDrop = true;
			this->treeView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->treeView->Location = System::Drawing::Point(0, 0);
			this->treeView->Name = L"treeView";
			this->treeView->Size = System::Drawing::Size(135, 321);
			this->treeView->TabIndex = 0;
			this->treeView->DoubleClick += gcnew System::EventHandler(this, &TreeViewToXMLNode::treeView_DoubleClick);
			this->treeView->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &TreeViewToXMLNode::treeView_DragDrop);
			this->treeView->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &TreeViewToXMLNode::treeView_DragEnter);
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->treeView);
			this->splitContainer1->Size = System::Drawing::Size(600, 325);
			this->splitContainer1->SplitterDistance = 139;
			this->splitContainer1->TabIndex = 1;
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(61, 4);
			// 
			// TreeViewToXMLNode
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer1);
			this->Name = L"TreeViewToXMLNode";
			this->Size = System::Drawing::Size(600, 325);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void treeView_DoubleClick(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(this->treeView->SelectedNode)
				 {
					 if(this->xmlForm1->CurrentAttribute_treeView->SelectedNode)
					 {
						 this->xmlForm1->CurrentAttribute_treeView->SelectedNode->Nodes->Add((GCFORM::TreeNode^)treeView->SelectedNode->Clone());
					 }
					 else
					 {
						 this->xmlForm1->CurrentAttribute_treeView->Nodes->Add((GCFORM::TreeNode^)treeView->SelectedNode->Clone());
					 }
				 }
			 }


	private: System::Void treeView_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
			 {
				array<String^>^l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
				for each(String^l_str in l_strFileNames)
				{
					 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_str);
					 if( l_pNode )
					 {
						 this->Text += System::IO::Path::GetFileName(l_str);
						 treeView->Nodes->Add(l_pNode);
					 }
				}
			 }
	private: System::Void treeView_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
			 {
				 if ( e->Data->GetDataPresent( DataFormats::FileDrop ) )
				 {
					e->Effect = DragDropEffects::Copy;
				 }
				 else
					 e->Effect = DragDropEffects::None;
			 }
};
}
