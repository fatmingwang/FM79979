#pragma once
#include "EDitText.h"
#include "NodeSearchForm.h"
#include "TreeNodeInfo.h"
#include "time.h"
#include ".\XML\AtgXmlWriter.h"
#include "TextEditorLoad.h"
#include "NodeFile.h"
#include ".\Zip\ZipFile.h"
#include ".\Zip\EndianSwitch.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Xml;
namespace DrawFont {
	public ref class TextEditor : public System::Windows::Forms::Form
	{
	public:
		TextEditor(System::String	^e_pStringForExeFilePath)
		{
			m_pStringForExeFilePath = e_pStringForExeFilePath;
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_pNodeListForAppend = gcnew System::Collections::Hashtable();

			NodeFile^l_pNodeFile = gcnew NodeFile();
			l_pNodeFile->m_pHashTable = m_pNodeListForAppend;
			l_pNodeFile->LoadFile(TO_GCSTRING("default.fun"),m_pStringForExeFilePath);
			for each( System::Object^l_p in m_pNodeListForAppend->Keys )
				this->NodeList_comboBox->Items->Add(l_p->ToString());
		}
	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~TextEditor()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 

	private: System::Windows::Forms::ContextMenuStrip^  TextTreeMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  AddGroupToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  AddChildToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  DeleteGroupToolStripMenuItem;




	private: System::Windows::Forms::ToolStripMenuItem^  RenameToolStripMenuItem;

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  NewToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  SaveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  SaveAsToolStripMenuItem;


	private: System::Windows::Forms::ToolStripMenuItem^  ExitToolStripMenuItem;

	private: System::Windows::Forms::CheckBox^  NoSameName_checkBox;

	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::ToolStripMenuItem^  importFloderToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  StripFileExtensionName_checkBox;
	private: System::Windows::Forms::TreeView^  TextEDitor_treeView;
	private: System::Windows::Forms::NumericUpDown^  MaxLevel_numericUpDown;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ToolStripMenuItem^  ExporToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  EportAllToolStripMenuItem;
	private: System::Windows::Forms::ComboBox^  NodeList_comboBox;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  NodeListAddGroup_button;
	private: System::Windows::Forms::Button^  NodeListAddChild_button;
	private: System::Windows::Forms::ToolStripMenuItem^  toNodeListToolStripMenuItem;









	private: System::Windows::Forms::ToolTip^  toolTip;

//=================================================================
	private: System::Void	TextEditor::ReNameTreeNode();
	private: System::String^	TextEditor::GetTextStringFrom(String^e_str);
	private: System::Void	SaveGameFile(System::String^e_pString);
	private: bool			SaveXmlFile(System::Object^  sender);
	private: System::Void	OpenFile();
	private: System::Void	WriteInformation(ATG::XMLWriter*e_pWriter);
	private: System::Void	SearchNodes(GCFORM::TreeNode^e_pNode,System::String^e_pStringToCompare,bool* e_b );
	private: System::Void	AddChildIntoTreeNode();
	private: System::Void	AddGroupIntoTreeNode();
	private: System::Void DeleteTreeNode();
	private: System::String^m_pStringForFileName;
	//contain GCFORM::TreeNode,and use this to add to selected node
	private: System::Collections::Hashtable^	m_pNodeListForAppend;
	private: System::String	^m_pStringForExeFilePath;
	protected: 

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->TextTreeMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->AddGroupToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AddChildToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->DeleteGroupToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->RenameToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->importFloderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toNodeListToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->NewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SaveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SaveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ExporToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->EportAllToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ExitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->NoSameName_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->StripFileExtensionName_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->TextEDitor_treeView = (gcnew System::Windows::Forms::TreeView());
			this->MaxLevel_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->toolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->NodeList_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->NodeListAddGroup_button = (gcnew System::Windows::Forms::Button());
			this->NodeListAddChild_button = (gcnew System::Windows::Forms::Button());
			this->TextTreeMenuStrip->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MaxLevel_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// TextTreeMenuStrip
			// 
			this->TextTreeMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->AddGroupToolStripMenuItem, 
				this->AddChildToolStripMenuItem, this->DeleteGroupToolStripMenuItem, this->RenameToolStripMenuItem, this->importFloderToolStripMenuItem, 
				this->toNodeListToolStripMenuItem});
			this->TextTreeMenuStrip->Name = L"TextTreeMenuStrip";
			this->TextTreeMenuStrip->Size = System::Drawing::Size(128, 136);
			// 
			// AddGroupToolStripMenuItem
			// 
			this->AddGroupToolStripMenuItem->Name = L"AddGroupToolStripMenuItem";
			this->AddGroupToolStripMenuItem->Size = System::Drawing::Size(127, 22);
			this->AddGroupToolStripMenuItem->Text = L"Add Group";
			this->AddGroupToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::addGroupToolStripMenuItem_Click_1);
			// 
			// AddChildToolStripMenuItem
			// 
			this->AddChildToolStripMenuItem->Name = L"AddChildToolStripMenuItem";
			this->AddChildToolStripMenuItem->Size = System::Drawing::Size(127, 22);
			this->AddChildToolStripMenuItem->Text = L"AddChild";
			this->AddChildToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::addGroupToolStripMenuItem_Click_1);
			// 
			// DeleteGroupToolStripMenuItem
			// 
			this->DeleteGroupToolStripMenuItem->Name = L"DeleteGroupToolStripMenuItem";
			this->DeleteGroupToolStripMenuItem->Size = System::Drawing::Size(127, 22);
			this->DeleteGroupToolStripMenuItem->Text = L"Delete";
			this->DeleteGroupToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::addGroupToolStripMenuItem_Click_1);
			// 
			// RenameToolStripMenuItem
			// 
			this->RenameToolStripMenuItem->Name = L"RenameToolStripMenuItem";
			this->RenameToolStripMenuItem->Size = System::Drawing::Size(127, 22);
			this->RenameToolStripMenuItem->Text = L"Rename";
			this->RenameToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::addGroupToolStripMenuItem_Click_1);
			// 
			// importFloderToolStripMenuItem
			// 
			this->importFloderToolStripMenuItem->Name = L"importFloderToolStripMenuItem";
			this->importFloderToolStripMenuItem->Size = System::Drawing::Size(127, 22);
			this->importFloderToolStripMenuItem->Text = L"ImportFiles";
			this->importFloderToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::importFloderToolStripMenuItem_Click);
			// 
			// toNodeListToolStripMenuItem
			// 
			this->toNodeListToolStripMenuItem->Name = L"toNodeListToolStripMenuItem";
			this->toNodeListToolStripMenuItem->Size = System::Drawing::Size(127, 22);
			this->toNodeListToolStripMenuItem->Text = L"ToNodeList";
			this->toNodeListToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::toNodeListToolStripMenuItem_Click);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(492, 24);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->NewToolStripMenuItem, 
				this->openToolStripMenuItem, this->SaveToolStripMenuItem, this->SaveAsToolStripMenuItem, this->ExporToolStripMenuItem, this->EportAllToolStripMenuItem, 
				this->ExitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// NewToolStripMenuItem
			// 
			this->NewToolStripMenuItem->Name = L"NewToolStripMenuItem";
			this->NewToolStripMenuItem->Size = System::Drawing::Size(156, 22);
			this->NewToolStripMenuItem->Text = L"New";
			this->NewToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::saveToolStripMenuItem_Click);
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(156, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::saveToolStripMenuItem_Click);
			// 
			// SaveToolStripMenuItem
			// 
			this->SaveToolStripMenuItem->Name = L"SaveToolStripMenuItem";
			this->SaveToolStripMenuItem->Size = System::Drawing::Size(156, 22);
			this->SaveToolStripMenuItem->Text = L"Save";
			this->SaveToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::saveToolStripMenuItem_Click);
			// 
			// SaveAsToolStripMenuItem
			// 
			this->SaveAsToolStripMenuItem->Name = L"SaveAsToolStripMenuItem";
			this->SaveAsToolStripMenuItem->Size = System::Drawing::Size(156, 22);
			this->SaveAsToolStripMenuItem->Text = L"Save as";
			this->SaveAsToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::saveToolStripMenuItem_Click);
			// 
			// ExporToolStripMenuItem
			// 
			this->ExporToolStripMenuItem->Name = L"ExporToolStripMenuItem";
			this->ExporToolStripMenuItem->Size = System::Drawing::Size(156, 22);
			this->ExporToolStripMenuItem->Text = L"ExportGameData";
			this->ExporToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::saveToolStripMenuItem_Click);
			// 
			// EportAllToolStripMenuItem
			// 
			this->EportAllToolStripMenuItem->Name = L"EportAllToolStripMenuItem";
			this->EportAllToolStripMenuItem->Size = System::Drawing::Size(156, 22);
			this->EportAllToolStripMenuItem->Text = L"ExportAll";
			this->EportAllToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::saveToolStripMenuItem_Click);
			// 
			// ExitToolStripMenuItem
			// 
			this->ExitToolStripMenuItem->Name = L"ExitToolStripMenuItem";
			this->ExitToolStripMenuItem->Size = System::Drawing::Size(156, 22);
			this->ExitToolStripMenuItem->Text = L"Exit";
			this->ExitToolStripMenuItem->Click += gcnew System::EventHandler(this, &TextEditor::saveToolStripMenuItem_Click);
			// 
			// NoSameName_checkBox
			// 
			this->NoSameName_checkBox->AutoSize = true;
			this->NoSameName_checkBox->Location = System::Drawing::Point(6, 26);
			this->NoSameName_checkBox->Name = L"NoSameName_checkBox";
			this->NoSameName_checkBox->Size = System::Drawing::Size(184, 17);
			this->NoSameName_checkBox->TabIndex = 3;
			this->NoSameName_checkBox->Text = L"Do not allow the same name exist";
			this->NoSameName_checkBox->UseVisualStyleBackColor = true;
			// 
			// StripFileExtensionName_checkBox
			// 
			this->StripFileExtensionName_checkBox->AutoSize = true;
			this->StripFileExtensionName_checkBox->Location = System::Drawing::Point(6, 49);
			this->StripFileExtensionName_checkBox->Name = L"StripFileExtensionName_checkBox";
			this->StripFileExtensionName_checkBox->Size = System::Drawing::Size(135, 17);
			this->StripFileExtensionName_checkBox->TabIndex = 4;
			this->StripFileExtensionName_checkBox->Text = L"Hide extension file type";
			this->StripFileExtensionName_checkBox->UseVisualStyleBackColor = true;
			// 
			// TextEDitor_treeView
			// 
			this->TextEDitor_treeView->Location = System::Drawing::Point(6, 102);
			this->TextEDitor_treeView->Name = L"TextEDitor_treeView";
			this->TextEDitor_treeView->Size = System::Drawing::Size(480, 172);
			this->TextEDitor_treeView->TabIndex = 5;
			this->TextEDitor_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &TextEditor::TextEDitor_treeView_AfterSelect);
			this->TextEDitor_treeView->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &TextEditor::TextEDitor_treeView_MouseMove);
			this->TextEDitor_treeView->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &TextEditor::TextEDitor_treeView_KeyDown);
			// 
			// MaxLevel_numericUpDown
			// 
			this->MaxLevel_numericUpDown->Location = System::Drawing::Point(84, 74);
			this->MaxLevel_numericUpDown->Name = L"MaxLevel_numericUpDown";
			this->MaxLevel_numericUpDown->Size = System::Drawing::Size(71, 20);
			this->MaxLevel_numericUpDown->TabIndex = 6;
			this->MaxLevel_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 79);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(79, 13);
			this->label1->TabIndex = 7;
			this->label1->Text = L"NodeMaxLevel";
			this->label1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &TextEditor::label1_MouseMove);
			// 
			// toolTip
			// 
			this->toolTip->ShowAlways = true;
			this->toolTip->ToolTipIcon = System::Windows::Forms::ToolTipIcon::Info;
			// 
			// NodeList_comboBox
			// 
			this->NodeList_comboBox->FormattingEnabled = true;
			this->NodeList_comboBox->Location = System::Drawing::Point(370, 49);
			this->NodeList_comboBox->Name = L"NodeList_comboBox";
			this->NodeList_comboBox->Size = System::Drawing::Size(100, 21);
			this->NodeList_comboBox->TabIndex = 8;
			this->NodeList_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &TextEditor::NodeList_comboBox_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(303, 50);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(49, 13);
			this->label2->TabIndex = 9;
			this->label2->Text = L"NodeList";
			// 
			// NodeListAddGroup_button
			// 
			this->NodeListAddGroup_button->Enabled = false;
			this->NodeListAddGroup_button->Location = System::Drawing::Point(282, 75);
			this->NodeListAddGroup_button->Name = L"NodeListAddGroup_button";
			this->NodeListAddGroup_button->Size = System::Drawing::Size(96, 20);
			this->NodeListAddGroup_button->TabIndex = 10;
			this->NodeListAddGroup_button->Text = L"AddToGroup";
			this->NodeListAddGroup_button->UseVisualStyleBackColor = true;
			this->NodeListAddGroup_button->Click += gcnew System::EventHandler(this, &TextEditor::NodeListAddGroup_button_Click);
			// 
			// NodeListAddChild_button
			// 
			this->NodeListAddChild_button->Enabled = false;
			this->NodeListAddChild_button->Location = System::Drawing::Point(384, 75);
			this->NodeListAddChild_button->Name = L"NodeListAddChild_button";
			this->NodeListAddChild_button->Size = System::Drawing::Size(96, 20);
			this->NodeListAddChild_button->TabIndex = 11;
			this->NodeListAddChild_button->Text = L"AddToChild";
			this->NodeListAddChild_button->UseVisualStyleBackColor = true;
			this->NodeListAddChild_button->Click += gcnew System::EventHandler(this, &TextEditor::NodeListAddGroup_button_Click);
			// 
			// TextEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(492, 286);
			this->ContextMenuStrip = this->TextTreeMenuStrip;
			this->Controls->Add(this->NodeListAddChild_button);
			this->Controls->Add(this->NodeListAddGroup_button);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->NodeList_comboBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->MaxLevel_numericUpDown);
			this->Controls->Add(this->StripFileExtensionName_checkBox);
			this->Controls->Add(this->NoSameName_checkBox);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->TextEDitor_treeView);
			this->KeyPreview = true;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"TextEditor";
			this->Text = L"TextEditor";
			this->Resize += gcnew System::EventHandler(this, &TextEditor::TextEditor_Resize);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &TextEditor::TextEDitor_treeView_KeyDown);
			this->TextTreeMenuStrip->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MaxLevel_numericUpDown))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void TextEditor_Resize(System::Object^  sender, System::EventArgs^  e) {
				 TextEDitor_treeView->Size = System::Drawing::Size(
					 this->Width-TextEDitor_treeView->Location.X-20,
					 this->Height-TextEDitor_treeView->Location.Y-30);
			 }
private: System::Void TextEDitor_treeView_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 if(e->Control)
			 {
				 if( e->KeyValue == 70 )//f
				 {
					NodeSearchForm ^l_p = gcnew NodeSearchForm(TextEDitor_treeView);
					l_p->Show();//
				 }
				 else
				 if( e->KeyValue == 113 )//f2
					ReNameTreeNode();
				 else//67 = c
				 if(e->Control&&e->KeyValue == 67)
					AddChildIntoTreeNode();
				 else//71 = g
				 if(e->Control&&e->KeyValue == 71)
					AddGroupIntoTreeNode();
				 else//46 = Del
				 if(e->Control&&e->KeyValue == 46)
					DeleteTreeNode();
				 else
				 if(e->Control&&e->KeyValue == 78)
					ReNameTreeNode();
			 }
			 if( e->KeyCode == GCFORM::Keys::F2 )
			 {
				ReNameTreeNode();
			 }
		 }

private: System::Void importFloderToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(!this->TextEDitor_treeView->SelectedNode)
				 return;
			 array<String^>^ l_pStringForFileName = DNCT::OpenFileAndGetNames();
			 for each ( System::String^l_pString in l_pStringForFileName)
			 {
				 System::String^l_pStringForFileName = DNCT::GetFileNameWithoutFullPath(l_pString,StripFileExtensionName_checkBox->Checked);
				 this->TextEDitor_treeView->SelectedNode->Nodes->Add(l_pStringForFileName);
			 }
		 }
private: System::Void addGroupToolStripMenuItem_Click_1(System::Object^  sender, System::EventArgs^  e) {
			 if( sender == RenameToolStripMenuItem )
				ReNameTreeNode();
			 else
			 if( sender == AddGroupToolStripMenuItem )
				AddGroupIntoTreeNode();
			 else
			 if( sender == AddChildToolStripMenuItem )
				AddChildIntoTreeNode();
			 else
			 if( sender == DeleteGroupToolStripMenuItem )
				DeleteTreeNode();
		 }
private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if( sender == NewToolStripMenuItem )
			 {
				this->TextEDitor_treeView->Nodes->Clear();
				m_pStringForFileName = "";
			 }
			 else
			 if(sender == openToolStripMenuItem)
				OpenFile();
			 else
			 if( sender == SaveToolStripMenuItem ||  sender == SaveAsToolStripMenuItem )
			 {
				 if(SaveXmlFile(sender))
				 {
					 NodeFile^ l_pFile = gcnew NodeFile();
					 l_pFile->m_pHashTable = this->m_pNodeListForAppend;
					 l_pFile->WriteFile(m_pStringForExeFilePath);
				 }
			 }
			 else
			 if(sender == ExitToolStripMenuItem)
				this->Close();
			 else
			 if( sender == ExporToolStripMenuItem )
				SaveGameFile("");
			 else
			 if( sender == EportAllToolStripMenuItem )
			 {
				if(SaveXmlFile(sender))
				{
					SaveGameFile(m_pStringForFileName);
					NodeFile^ l_pFile = gcnew NodeFile();
					l_pFile->m_pHashTable = this->m_pNodeListForAppend;
					l_pFile->WriteFile(m_pStringForExeFilePath);
				}
			 }
		 }

private: System::Void TextEDitor_treeView_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			//toolTip->SetToolTip(TextEDitor_treeView,"Qucik Achieve\nCtrl+C AddChild\nCtrl+G AddGroup\nCtrl+N ReName\nCtrl+F Search");
		 }
private: System::Void label1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			toolTip->SetToolTip(safe_cast<System::Windows::Forms::Control^>(sender),"Tree Node Max Count");
		 }

private: System::Void NodeListAddGroup_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 GCFORM::TreeNodeCollection^ l_pNodeCollectionForAddNode;			 
			 if( sender == NodeListAddChild_button )
				 l_pNodeCollectionForAddNode = this->TextEDitor_treeView->SelectedNode->Nodes;
			 else
				 l_pNodeCollectionForAddNode = this->TextEDitor_treeView->Nodes;

			 GCFORM::TreeNode^ l_p = (GCFORM::TreeNode^)((GCFORM::TreeNode^)(this->m_pNodeListForAppend[NodeList_comboBox->SelectedItem->ToString()]))->Clone();
			 //old node from file
			 if( l_p->Text->ToString()->Equals("root") )
			 {
				 for each(GCFORM::TreeNode^l_p2 in l_p->Nodes)
					l_pNodeCollectionForAddNode->Add( (GCFORM::TreeNode^)l_p2->Clone() );
			 }
			 else//new node from treeview
				 l_pNodeCollectionForAddNode->Add( (GCFORM::TreeNode^)l_p->Clone() );
		 }
private: System::Void TextEDitor_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
			 this->TextEDitor_treeView->SelectedNode?NodeListAddChild_button->Enabled = true:NodeListAddChild_button->Enabled = false;
		 }
private: System::Void NodeList_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 NodeList_comboBox->SelectedIndex!=-1?NodeListAddGroup_button->Enabled = true:NodeListAddGroup_button->Enabled = false;

		 }
private: System::Void toNodeListToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(this->TextEDitor_treeView->SelectedNode)
			 {
				 bool	l_bSame = false;
				 System::String^l_pSelectNode = this->TextEDitor_treeView->SelectedNode->Text->ToString();
				 for(int i=0;i<this->NodeList_comboBox->Items->Count;i++)
				 {
					 System::String^l_pStringForItem = NodeList_comboBox->Items[i]->ToString();
					 if( l_pStringForItem->Equals(l_pSelectNode) )
					 {
						WARING_YES_NO_TO_YES("Would you like to replace?")
						{
							DNCT::ReplaceValuInHashTable(l_pStringForItem,TextEDitor_treeView->SelectedNode->Clone(),m_pNodeListForAppend);							
							return;
						}
						l_bSame = true;
					 }
				 }
				 if(!l_bSame)
				 {
					this->m_pNodeListForAppend->Add(l_pSelectNode,this->TextEDitor_treeView->SelectedNode->Clone());
					NodeList_comboBox->Items->Add(l_pSelectNode);
				 }
			 }
			 else
				 WARNING_MSG("please select one node");
		 }
};
}
