#pragma once

//#include "XMLForm.h"
//
//using namespace System;
//using namespace System::ComponentModel;
//using namespace System::Collections;
//using namespace System::Windows::Forms;
//using namespace System::Data;
//using namespace System::Drawing;
//
//
//namespace DotNetComponent {
//
//	/// <summary>
//	/// XmlNodeEditComponent 的摘要
//	/// </summary>
//	public ref class XmlNodeEditComponent : public System::Windows::Forms::UserControl
//	{
//	public:
//		XmlNodeEditComponent(void)
//		{
//			InitializeComponent();
//			//
//			//TODO: 在此加入建構函式程式碼
//			//
//			m_pbCut = new bool;
//			*m_pbCut = false;
//			m_RootName = "Root";
//		}
//
//	protected:
//		/// <summary>
//		/// 清除任何使用中的資源。
//		/// </summary>
//		~XmlNodeEditComponent()
//		{
//			delete m_pbCut;
//			if (components)
//			{
//				delete components;
//			}
//		}
//	private: System::Windows::Forms::Label^  label6;
//	public: System::Windows::Forms::TreeView^  CurrentAttribute_treeView;
//	public: System::Windows::Forms::TextBox^  Value_textBox;
//	private: System::Windows::Forms::Label^  label3;
//	public: System::Windows::Forms::TextBox^  Attribute_textBox;
//	private: System::Windows::Forms::Label^  label2;
//	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
//	private: System::Windows::Forms::ToolStripMenuItem^  deleteToolStripMenuItem;
//	private: System::Windows::Forms::ToolStripMenuItem^  cutToolStripMenuItem;
//	private: System::Windows::Forms::ToolStripMenuItem^  pasteToolStripMenuItem;
//	private: System::Windows::Forms::ToolStripMenuItem^  copyToolStripMenuItem;
//	private: System::Windows::Forms::Button^  Paste_button;
//	private: System::Windows::Forms::Button^  Cut_button;
//	private: System::Windows::Forms::Button^  Copy_button;
//	private: System::Windows::Forms::Button^  ReplaceAttribute_button;
//	private: System::Windows::Forms::Button^  AddElement_button;
//	private: System::Windows::Forms::Button^  DelAttribute_button;
//	private: System::Windows::Forms::Button^  AddNewAttribute_button;
//	private: System::ComponentModel::IContainer^  components;
//	private:
//		//my
//	public: String^m_RootName;
//		//cut or paste if cut is true remove connection while paste
//		bool	*m_pbCut;
//		//this one for cut and Copy and paste
//		System::Windows::Forms::TreeNode^m_pCurrentTempNode;
//		//clear all nodes and setup a new node
//		void	DoNewRootNodeBySelectedNodeFromTreeNode(System::Windows::Forms::TreeView^e_pTreeView);
//		void	DoNewRootNode(System::Windows::Forms::TreeNode^e_pNode);
//		void	DoPasteButton();
//		void	DoCutButton();
//		void	DoCopyButton();
//		void	DoReplaceButton();
//		void	DoAddElementButton();
//		void	DoDeleteButton();
//		void	DoAddNewAttributeButton();
//		public:	System::EventHandler^				m_TreeViewDeleteOrCreateFunction;
//
//
//#pragma region Windows Form Designer generated code
//		/// <summary>
//		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
//		///
//		/// </summary>
//		void InitializeComponent(void)
//		{
//			this->components = (gcnew System::ComponentModel::Container());
//			this->AddElement_button = (gcnew System::Windows::Forms::Button());
//			this->DelAttribute_button = (gcnew System::Windows::Forms::Button());
//			this->AddNewAttribute_button = (gcnew System::Windows::Forms::Button());
//			this->label6 = (gcnew System::Windows::Forms::Label());
//			this->ReplaceAttribute_button = (gcnew System::Windows::Forms::Button());
//			this->CurrentAttribute_treeView = (gcnew System::Windows::Forms::TreeView());
//			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
//			this->deleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
//			this->cutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
//			this->pasteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
//			this->copyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
//			this->Value_textBox = (gcnew System::Windows::Forms::TextBox());
//			this->label3 = (gcnew System::Windows::Forms::Label());
//			this->Attribute_textBox = (gcnew System::Windows::Forms::TextBox());
//			this->label2 = (gcnew System::Windows::Forms::Label());
//			this->Copy_button = (gcnew System::Windows::Forms::Button());
//			this->Paste_button = (gcnew System::Windows::Forms::Button());
//			this->Cut_button = (gcnew System::Windows::Forms::Button());
//			this->contextMenuStrip1->SuspendLayout();
//			this->SuspendLayout();
//			// 
//			// AddElement_button
//			// 
//			this->AddElement_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
//			this->AddElement_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
//			this->AddElement_button->Location = System::Drawing::Point(5, 83);
//			this->AddElement_button->Margin = System::Windows::Forms::Padding(2);
//			this->AddElement_button->Name = L"AddElement_button";
//			this->AddElement_button->Size = System::Drawing::Size(79, 22);
//			this->AddElement_button->TabIndex = 45;
//			this->AddElement_button->Text = L"AddElement";
//			this->AddElement_button->UseVisualStyleBackColor = false;
//			this->AddElement_button->Click += gcnew System::EventHandler(this, &XmlNodeEditComponent::AddNewAttribute_button_Click);
//			// 
//			// DelAttribute_button
//			// 
//			this->DelAttribute_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
//			this->DelAttribute_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
//			this->DelAttribute_button->Location = System::Drawing::Point(7, 162);
//			this->DelAttribute_button->Margin = System::Windows::Forms::Padding(2);
//			this->DelAttribute_button->Name = L"DelAttribute_button";
//			this->DelAttribute_button->Size = System::Drawing::Size(78, 22);
//			this->DelAttribute_button->TabIndex = 44;
//			this->DelAttribute_button->Text = L"Delete";
//			this->DelAttribute_button->UseVisualStyleBackColor = false;
//			this->DelAttribute_button->Click += gcnew System::EventHandler(this, &XmlNodeEditComponent::AddNewAttribute_button_Click);
//			// 
//			// AddNewAttribute_button
//			// 
//			this->AddNewAttribute_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
//			this->AddNewAttribute_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
//			this->AddNewAttribute_button->Location = System::Drawing::Point(7, 109);
//			this->AddNewAttribute_button->Margin = System::Windows::Forms::Padding(2);
//			this->AddNewAttribute_button->Name = L"AddNewAttribute_button";
//			this->AddNewAttribute_button->Size = System::Drawing::Size(80, 22);
//			this->AddNewAttribute_button->TabIndex = 43;
//			this->AddNewAttribute_button->Text = L"AddAttribute";
//			this->AddNewAttribute_button->UseVisualStyleBackColor = false;
//			this->AddNewAttribute_button->Click += gcnew System::EventHandler(this, &XmlNodeEditComponent::AddNewAttribute_button_Click);
//			// 
//			// label6
//			// 
//			this->label6->AutoSize = true;
//			this->label6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(64)));
//			this->label6->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(244)));
//			this->label6->Location = System::Drawing::Point(87, 2);
//			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
//			this->label6->Name = L"label6";
//			this->label6->Size = System::Drawing::Size(46, 12);
//			this->label6->TabIndex = 42;
//			this->label6->Text = L"XmlData";
//			// 
//			// ReplaceAttribute_button
//			// 
//			this->ReplaceAttribute_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
//			this->ReplaceAttribute_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
//			this->ReplaceAttribute_button->Location = System::Drawing::Point(5, 137);
//			this->ReplaceAttribute_button->Margin = System::Windows::Forms::Padding(2);
//			this->ReplaceAttribute_button->Name = L"ReplaceAttribute_button";
//			this->ReplaceAttribute_button->Size = System::Drawing::Size(80, 22);
//			this->ReplaceAttribute_button->TabIndex = 41;
//			this->ReplaceAttribute_button->Text = L"Replace";
//			this->ReplaceAttribute_button->UseVisualStyleBackColor = false;
//			this->ReplaceAttribute_button->Click += gcnew System::EventHandler(this, &XmlNodeEditComponent::AddNewAttribute_button_Click);
//			// 
//			// CurrentAttribute_treeView
//			// 
//			this->CurrentAttribute_treeView->AllowDrop = true;
//			this->CurrentAttribute_treeView->ContextMenuStrip = this->contextMenuStrip1;
//			this->CurrentAttribute_treeView->Location = System::Drawing::Point(89, 20);
//			this->CurrentAttribute_treeView->Margin = System::Windows::Forms::Padding(2);
//			this->CurrentAttribute_treeView->Name = L"CurrentAttribute_treeView";
//			this->CurrentAttribute_treeView->Size = System::Drawing::Size(195, 247);
//			this->CurrentAttribute_treeView->TabIndex = 40;
//			this->CurrentAttribute_treeView->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &XmlNodeEditComponent::CurrentAttribute_treeView_DragDrop);
//			this->CurrentAttribute_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &XmlNodeEditComponent::CurrentAttribute_treeView_AfterSelect);
//			this->CurrentAttribute_treeView->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &XmlNodeEditComponent::CurrentAttribute_treeView_DragEnter);
//			this->CurrentAttribute_treeView->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &XmlNodeEditComponent::CurrentAttribute_treeView_KeyUp);
//			// 
//			// contextMenuStrip1
//			// 
//			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->deleteToolStripMenuItem, 
//				this->cutToolStripMenuItem, this->pasteToolStripMenuItem, this->copyToolStripMenuItem});
//			this->contextMenuStrip1->Name = L"contextMenuStrip1";
//			this->contextMenuStrip1->Size = System::Drawing::Size(112, 92);
//			// 
//			// deleteToolStripMenuItem
//			// 
//			this->deleteToolStripMenuItem->Name = L"deleteToolStripMenuItem";
//			this->deleteToolStripMenuItem->Size = System::Drawing::Size(111, 22);
//			this->deleteToolStripMenuItem->Text = L"Delete";
//			this->deleteToolStripMenuItem->Click += gcnew System::EventHandler(this, &XmlNodeEditComponent::AddNewAttribute_button_Click);
//			// 
//			// cutToolStripMenuItem
//			// 
//			this->cutToolStripMenuItem->Name = L"cutToolStripMenuItem";
//			this->cutToolStripMenuItem->Size = System::Drawing::Size(111, 22);
//			this->cutToolStripMenuItem->Text = L"Cut";
//			this->cutToolStripMenuItem->Click += gcnew System::EventHandler(this, &XmlNodeEditComponent::AddNewAttribute_button_Click);
//			// 
//			// pasteToolStripMenuItem
//			// 
//			this->pasteToolStripMenuItem->Name = L"pasteToolStripMenuItem";
//			this->pasteToolStripMenuItem->Size = System::Drawing::Size(111, 22);
//			this->pasteToolStripMenuItem->Text = L"Paste";
//			this->pasteToolStripMenuItem->Click += gcnew System::EventHandler(this, &XmlNodeEditComponent::AddNewAttribute_button_Click);
//			// 
//			// copyToolStripMenuItem
//			// 
//			this->copyToolStripMenuItem->Name = L"copyToolStripMenuItem";
//			this->copyToolStripMenuItem->Size = System::Drawing::Size(111, 22);
//			this->copyToolStripMenuItem->Text = L"Copy";
//			this->copyToolStripMenuItem->Click += gcnew System::EventHandler(this, &XmlNodeEditComponent::AddNewAttribute_button_Click);
//			// 
//			// Value_textBox
//			// 
//			this->Value_textBox->Location = System::Drawing::Point(6, 57);
//			this->Value_textBox->Margin = System::Windows::Forms::Padding(2);
//			this->Value_textBox->Name = L"Value_textBox";
//			this->Value_textBox->Size = System::Drawing::Size(80, 22);
//			this->Value_textBox->TabIndex = 39;
//			// 
//			// label3
//			// 
//			this->label3->AutoSize = true;
//			this->label3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(64)));
//			this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(244)));
//			this->label3->Location = System::Drawing::Point(4, 42);
//			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
//			this->label3->Name = L"label3";
//			this->label3->Size = System::Drawing::Size(32, 12);
//			this->label3->TabIndex = 38;
//			this->label3->Text = L"Value";
//			// 
//			// Attribute_textBox
//			// 
//			this->Attribute_textBox->Location = System::Drawing::Point(6, 20);
//			this->Attribute_textBox->Margin = System::Windows::Forms::Padding(2);
//			this->Attribute_textBox->Name = L"Attribute_textBox";
//			this->Attribute_textBox->Size = System::Drawing::Size(80, 22);
//			this->Attribute_textBox->TabIndex = 37;
//			// 
//			// label2
//			// 
//			this->label2->AutoSize = true;
//			this->label2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(64)));
//			this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(244)));
//			this->label2->Location = System::Drawing::Point(4, 6);
//			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
//			this->label2->Name = L"label2";
//			this->label2->Size = System::Drawing::Size(46, 12);
//			this->label2->TabIndex = 36;
//			this->label2->Text = L"Attribute";
//			// 
//			// Copy_button
//			// 
//			this->Copy_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(64)));
//			this->Copy_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(244)));
//			this->Copy_button->Location = System::Drawing::Point(7, 191);
//			this->Copy_button->Margin = System::Windows::Forms::Padding(2);
//			this->Copy_button->Name = L"Copy_button";
//			this->Copy_button->Size = System::Drawing::Size(79, 22);
//			this->Copy_button->TabIndex = 47;
//			this->Copy_button->Text = L"Copy";
//			this->Copy_button->UseVisualStyleBackColor = false;
//			this->Copy_button->Click += gcnew System::EventHandler(this, &XmlNodeEditComponent::AddNewAttribute_button_Click);
//			// 
//			// Paste_button
//			// 
//			this->Paste_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(64)));
//			this->Paste_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(244)));
//			this->Paste_button->Location = System::Drawing::Point(7, 242);
//			this->Paste_button->Margin = System::Windows::Forms::Padding(2);
//			this->Paste_button->Name = L"Paste_button";
//			this->Paste_button->Size = System::Drawing::Size(79, 22);
//			this->Paste_button->TabIndex = 48;
//			this->Paste_button->Text = L"Paste";
//			this->Paste_button->UseVisualStyleBackColor = false;
//			this->Paste_button->Click += gcnew System::EventHandler(this, &XmlNodeEditComponent::AddNewAttribute_button_Click);
//			// 
//			// Cut_button
//			// 
//			this->Cut_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(64)));
//			this->Cut_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(244)));
//			this->Cut_button->Location = System::Drawing::Point(7, 217);
//			this->Cut_button->Margin = System::Windows::Forms::Padding(2);
//			this->Cut_button->Name = L"Cut_button";
//			this->Cut_button->Size = System::Drawing::Size(79, 22);
//			this->Cut_button->TabIndex = 49;
//			this->Cut_button->Text = L"Cut";
//			this->Cut_button->UseVisualStyleBackColor = false;
//			this->Cut_button->Click += gcnew System::EventHandler(this, &XmlNodeEditComponent::AddNewAttribute_button_Click);
//			// 
//			// XmlNodeEditComponent
//			// 
//			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
//			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
//			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(64)));
//			this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
//			this->Controls->Add(this->Cut_button);
//			this->Controls->Add(this->Paste_button);
//			this->Controls->Add(this->Copy_button);
//			this->Controls->Add(this->DelAttribute_button);
//			this->Controls->Add(this->AddElement_button);
//			this->Controls->Add(this->label6);
//			this->Controls->Add(this->ReplaceAttribute_button);
//			this->Controls->Add(this->AddNewAttribute_button);
//			this->Controls->Add(this->CurrentAttribute_treeView);
//			this->Controls->Add(this->Value_textBox);
//			this->Controls->Add(this->label3);
//			this->Controls->Add(this->Attribute_textBox);
//			this->Controls->Add(this->label2);
//			this->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
//				static_cast<System::Int32>(static_cast<System::Byte>(244)));
//			this->Margin = System::Windows::Forms::Padding(2);
//			this->Name = L"XmlNodeEditComponent";
//			this->Size = System::Drawing::Size(291, 269);
//			this->contextMenuStrip1->ResumeLayout(false);
//			this->ResumeLayout(false);
//			this->PerformLayout();
//
//		}
//#pragma endregion
//private: System::Void AddNewAttribute_button_Click(System::Object^  sender, System::EventArgs^  e)
//		 {
//			 if( sender == AddNewAttribute_button )
//			 {
//				DoAddNewAttributeButton(); 
//			 }
//			 else
//			 if( sender == DelAttribute_button || deleteToolStripMenuItem == sender )
//			 {
//				DoDeleteButton();
//			 }
//			 else
//			 if( sender == ReplaceAttribute_button )
//			 {
//				DoReplaceButton();
//			 }
//			 else
//			 if( sender == AddElement_button )
//			 {
//				DoAddElementButton(); 
//			 }
//			 else
//			 if( sender == Copy_button || copyToolStripMenuItem == sender )
//			 {
//				DoCopyButton();
//			 }
//			 else
//			 if( sender == Paste_button || pasteToolStripMenuItem == sender )
//			 {
//				DoPasteButton();
//			 }
//			 else
//			 if( sender == Cut_button || cutToolStripMenuItem == sender )
//			 {
//				DoCutButton();
//			 }
//			 if(m_TreeViewDeleteOrCreateFunction)
//				 m_TreeViewDeleteOrCreateFunction(sender,e);
//		 }
//private: System::Void CurrentAttribute_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
//		 {
//			 if( CurrentAttribute_treeView->SelectedNode )
//			 {//there is a parent and there is no child node
//				 System::Windows::Forms::TreeNode^l_pSelectedNode = CurrentAttribute_treeView->SelectedNode;
//				 if( l_pSelectedNode->Parent )
//				 {
//					 Attribute_textBox->Text = l_pSelectedNode->Text;
//					 Value_textBox->Text = l_pSelectedNode->Name;
//				 }
//			 }
//		 }
//private: System::Void CurrentAttribute_treeView_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
//		 {
//			 if( e->KeyCode == System::Windows::Forms::Keys::Delete)
//			 {
//				DoDeleteButton();
//				 if( m_TreeViewDeleteOrCreateFunction )
//				 {
//					m_TreeViewDeleteOrCreateFunction(sender,e);
//				 }
//			 }
//		 }
//private: System::Void CurrentAttribute_treeView_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
//		 {
//			 if( CurrentAttribute_treeView->Nodes->Count == 0 )
//			 {
//				 CurrentAttribute_treeView->Nodes->Add(m_RootName);
//			 }
//			 array<String^>^l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
//			 for each(String^l_str in l_strFileNames)
//			 {
//				 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_str);
//				 if( l_pNode )
//				 {
//					 this->Text += System::IO::Path::GetFileName(l_str);
//					 CurrentAttribute_treeView->Nodes[0]->Nodes->Add(l_pNode);
//				 }
//			 }
//			 if(m_TreeViewDeleteOrCreateFunction)
//				 m_TreeViewDeleteOrCreateFunction(sender,e);
//		 }
//private: System::Void CurrentAttribute_treeView_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
//		 {
//			 if ( e->Data->GetDataPresent( DataFormats::FileDrop ) )
//			 {
//				e->Effect = DragDropEffects::Copy;
//			 }
//			 else
//				 e->Effect = DragDropEffects::None;
//		 }
//};
//}
