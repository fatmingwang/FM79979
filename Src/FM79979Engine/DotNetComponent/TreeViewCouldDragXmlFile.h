#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace DotNetComponent {

	/// <summary>
	/// Summary for TreeViewCouldDragXmlFile
	/// </summary>
	public ref class TreeViewCouldDragXmlFile : public System::Windows::Forms::UserControl
	{
	public:
		TreeViewCouldDragXmlFile(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TreeViewCouldDragXmlFile()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TreeView^  treeView1;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->SuspendLayout();
			// 
			// treeView1
			// 
			this->treeView1->AllowDrop = true;
			this->treeView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->treeView1->Location = System::Drawing::Point(0, 0);
			this->treeView1->Name = L"treeView1";
			this->treeView1->Size = System::Drawing::Size(239, 188);
			this->treeView1->TabIndex = 0;
			this->treeView1->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &TreeViewCouldDragXmlFile::treeView1_DragDrop);
			this->treeView1->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &TreeViewCouldDragXmlFile::treeView1_DragEnter);
			// 
			// TreeViewCouldDragXmlFile
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->treeView1);
			this->Name = L"TreeViewCouldDragXmlFile";
			this->Size = System::Drawing::Size(239, 188);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void treeView1_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
			 {
				array<String^>^l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
				for each(String^l_str in l_strFileNames)
				{
					 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_str);
					 if( l_pNode )
					 {
						 this->Text += System::IO::Path::GetFileName(l_str);
						 treeView1->Nodes->Add(l_pNode);
					 }
				}
			 }
	private: System::Void treeView1_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
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
