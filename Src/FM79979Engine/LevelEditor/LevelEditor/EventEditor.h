#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace XMLDataEditor;

#undef GetCurrentDirectory
namespace LevelEditor {

	/// <summary>
	/// Summary for EventEditor
	/// </summary>
	public ref class EventEditor : public System::Windows::Forms::UserControl
	{
	public:
		EventEditor(GCFORM::TabControl^e_pTabControl)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->Dock = GCFORM::DockStyle::Fill;
			GCFORM::TabPage^l_pPage = gcnew GCFORM::TabPage("EventEditor");
			l_pPage->Controls->Add(this);
			e_pTabControl->TabPages->Add(l_pPage);
			m_pTreeNodeWithName = gcnew System::Collections::Hashtable;

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EventEditor()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  EventName_textBox;
	protected: 

    protected: 
    private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  DeleteEvent_button;
	private: System::Windows::Forms::Button^  SaveEvent_button;
	private: System::Windows::Forms::TreeView^  AllEvent_treeView;
	private: XMLDataEditor::TreeViewToXMLNode^  treeViewToXMLNode1;
	private: System::Windows::Forms::Button^  ReeditEvent_button;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  ShowRelateEventByAttribute_ToolStripMenuItem;


	private: System::Windows::Forms::ToolStripMenuItem^  ClearShowRelateEventTreeView_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ShowRelateEventByElement_ToolStripMenuItem;


	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  Open_toolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  Save_toolStripMenuItem;


			 //my
			 System::Collections::Hashtable^m_pTreeNodeWithName;
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
			this->EventName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->DeleteEvent_button = (gcnew System::Windows::Forms::Button());
			this->SaveEvent_button = (gcnew System::Windows::Forms::Button());
			this->AllEvent_treeView = (gcnew System::Windows::Forms::TreeView());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->ShowRelateEventByElement_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ShowRelateEventByAttribute_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ClearShowRelateEventTreeView_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ReeditEvent_button = (gcnew System::Windows::Forms::Button());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Open_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Save_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->treeViewToXMLNode1 = (gcnew XMLDataEditor::TreeViewToXMLNode());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->contextMenuStrip1->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// EventName_textBox
			// 
			this->EventName_textBox->Location = System::Drawing::Point(841, 58);
			this->EventName_textBox->Name = L"EventName_textBox";
			this->EventName_textBox->Size = System::Drawing::Size(232, 22);
			this->EventName_textBox->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(844, 43);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(59, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"EventName";
			// 
			// DeleteEvent_button
			// 
			this->DeleteEvent_button->Location = System::Drawing::Point(917, 88);
			this->DeleteEvent_button->Name = L"DeleteEvent_button";
			this->DeleteEvent_button->Size = System::Drawing::Size(75, 20);
			this->DeleteEvent_button->TabIndex = 8;
			this->DeleteEvent_button->Text = L"Delete Event";
			this->DeleteEvent_button->UseVisualStyleBackColor = true;
			this->DeleteEvent_button->Click += gcnew System::EventHandler(this, &EventEditor::SaveEvent_button_Click);
			// 
			// SaveEvent_button
			// 
			this->SaveEvent_button->Location = System::Drawing::Point(840, 88);
			this->SaveEvent_button->Name = L"SaveEvent_button";
			this->SaveEvent_button->Size = System::Drawing::Size(71, 20);
			this->SaveEvent_button->TabIndex = 7;
			this->SaveEvent_button->Text = L"Save Event";
			this->SaveEvent_button->UseVisualStyleBackColor = true;
			this->SaveEvent_button->Click += gcnew System::EventHandler(this, &EventEditor::SaveEvent_button_Click);
			// 
			// AllEvent_treeView
			// 
			this->AllEvent_treeView->ContextMenuStrip = this->contextMenuStrip1;
			this->AllEvent_treeView->Location = System::Drawing::Point(841, 114);
			this->AllEvent_treeView->Name = L"AllEvent_treeView";
			this->AllEvent_treeView->Size = System::Drawing::Size(242, 303);
			this->AllEvent_treeView->TabIndex = 14;
			this->AllEvent_treeView->DoubleClick += gcnew System::EventHandler(this, &EventEditor::AllEvent_treeView_DoubleClick);
			this->AllEvent_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &EventEditor::AllEvent_treeView_AfterSelect);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->ShowRelateEventByElement_ToolStripMenuItem, 
				this->ShowRelateEventByAttribute_ToolStripMenuItem, this->ClearShowRelateEventTreeView_ToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(298, 70);
			this->contextMenuStrip1->Text = L"Show Related Event recursively by element";
			// 
			// ShowRelateEventByElement_ToolStripMenuItem
			// 
			this->ShowRelateEventByElement_ToolStripMenuItem->Name = L"ShowRelateEventByElement_ToolStripMenuItem";
			this->ShowRelateEventByElement_ToolStripMenuItem->Size = System::Drawing::Size(297, 22);
			this->ShowRelateEventByElement_ToolStripMenuItem->Text = L"Show relate event all attributes by element";
			this->ShowRelateEventByElement_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &EventEditor::showRelateEventRecursivelyByElementToolStripMenuItem_Click);
			// 
			// ShowRelateEventByAttribute_ToolStripMenuItem
			// 
			this->ShowRelateEventByAttribute_ToolStripMenuItem->Name = L"ShowRelateEventByAttribute_ToolStripMenuItem";
			this->ShowRelateEventByAttribute_ToolStripMenuItem->Size = System::Drawing::Size(297, 22);
			this->ShowRelateEventByAttribute_ToolStripMenuItem->Text = L"Show Relate event by Attribute Value ";
			this->ShowRelateEventByAttribute_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &EventEditor::showRelateEventRecursivelyByElementToolStripMenuItem_Click);
			// 
			// ClearShowRelateEventTreeView_ToolStripMenuItem
			// 
			this->ClearShowRelateEventTreeView_ToolStripMenuItem->Name = L"ClearShowRelateEventTreeView_ToolStripMenuItem";
			this->ClearShowRelateEventTreeView_ToolStripMenuItem->Size = System::Drawing::Size(297, 22);
			this->ClearShowRelateEventTreeView_ToolStripMenuItem->Text = L"Clear";
			this->ClearShowRelateEventTreeView_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &EventEditor::showRelateEventRecursivelyByElementToolStripMenuItem_Click);
			// 
			// ReeditEvent_button
			// 
			this->ReeditEvent_button->Location = System::Drawing::Point(999, 88);
			this->ReeditEvent_button->Name = L"ReeditEvent_button";
			this->ReeditEvent_button->Size = System::Drawing::Size(74, 20);
			this->ReeditEvent_button->TabIndex = 17;
			this->ReeditEvent_button->Text = L"Reedit";
			this->ReeditEvent_button->UseVisualStyleBackColor = true;
			this->ReeditEvent_button->Click += gcnew System::EventHandler(this, &EventEditor::ReeditEvent_button_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->ReeditEvent_button);
			this->splitContainer1->Panel1->Controls->Add(this->EventName_textBox);
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->AllEvent_treeView);
			this->splitContainer1->Panel1->Controls->Add(this->SaveEvent_button);
			this->splitContainer1->Panel1->Controls->Add(this->DeleteEvent_button);
			this->splitContainer1->Panel1->Controls->Add(this->menuStrip1);
			this->splitContainer1->Panel1->Controls->Add(this->treeViewToXMLNode1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->flowLayoutPanel1);
			this->splitContainer1->Size = System::Drawing::Size(1088, 900);
			this->splitContainer1->SplitterDistance = 428;
			this->splitContainer1->TabIndex = 18;
			this->splitContainer1->SplitterMoved += gcnew System::Windows::Forms::SplitterEventHandler(this, &EventEditor::splitContainer1_SplitterMoved);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1086, 24);
			this->menuStrip1->TabIndex = 18;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->Open_toolStripMenuItem, 
				this->Save_toolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// Open_toolStripMenuItem
			// 
			this->Open_toolStripMenuItem->Name = L"Open_toolStripMenuItem";
			this->Open_toolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->Open_toolStripMenuItem->Text = L"Open";
			this->Open_toolStripMenuItem->Click += gcnew System::EventHandler(this, &EventEditor::Save_toolStripMenuItem_Click);
			// 
			// Save_toolStripMenuItem
			// 
			this->Save_toolStripMenuItem->Name = L"Save_toolStripMenuItem";
			this->Save_toolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->Save_toolStripMenuItem->Text = L"Save";
			this->Save_toolStripMenuItem->Click += gcnew System::EventHandler(this, &EventEditor::Save_toolStripMenuItem_Click);
			// 
			// treeViewToXMLNode1
			// 
			this->treeViewToXMLNode1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->treeViewToXMLNode1->Location = System::Drawing::Point(0, 27);
			this->treeViewToXMLNode1->Name = L"treeViewToXMLNode1";
			this->treeViewToXMLNode1->Size = System::Drawing::Size(835, 390);
			this->treeViewToXMLNode1->TabIndex = 16;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(1086, 466);
			this->flowLayoutPanel1->TabIndex = 0;
			// 
			// EventEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer1);
			this->Name = L"EventEditor";
			this->Size = System::Drawing::Size(1088, 900);
			this->contextMenuStrip1->ResumeLayout(false);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void showRelateEventRecursivelyByElementToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 //281, 253
			 if( sender == ShowRelateEventByElement_ToolStripMenuItem )
			 {
				 if(this->AllEvent_treeView->SelectedNode)
				 {
					 System::Collections::ArrayList^l_pList = gcnew System::Collections::ArrayList;
					if( AllEvent_treeView->SelectedNode->Name->Length == 0 )//make sure it's attribute
					for each(GCFORM::TreeNode^l_pNode in AllEvent_treeView->SelectedNode->Nodes)
					{
						for each(GCFORM::TreeNode^l_pEventNode in AllEvent_treeView->Nodes)
						{
							if(l_pEventNode->Text->Equals(l_pNode->Name))
							{
								GCFORM::TreeView^l_pTreeView = gcnew GCFORM::TreeView();
								l_pTreeView->Nodes->Add((GCFORM::TreeNode^)l_pEventNode->Clone());
								l_pTreeView->Size = System::Drawing::Size(200,200);
								l_pList->Add(l_pTreeView);
								flowLayoutPanel1->Controls->Add(l_pTreeView);
							}
						}
					}
				 }
			 }
			 else
			 if( sender == ClearShowRelateEventTreeView_ToolStripMenuItem )
			 {
				 this->flowLayoutPanel1->Controls->Clear();
			 }
			 else
			 if( sender == ShowRelateEventByAttribute_ToolStripMenuItem )
			 {
				 if(this->AllEvent_treeView->SelectedNode)
				 {
					if( AllEvent_treeView->SelectedNode->Name->Length != 0 )//make sure it's attribute
					{
						for each(GCFORM::TreeNode^l_pEventNode in AllEvent_treeView->Nodes)
						{
							if(AllEvent_treeView->SelectedNode->Text->Equals(l_pEventNode->Text))
							{
								GCFORM::TreeView^l_pTreeView = gcnew GCFORM::TreeView();
								l_pTreeView->Nodes->Add((GCFORM::TreeNode^)l_pEventNode->Clone());
								flowLayoutPanel1->Controls->Add(l_pTreeView);
							}
						}
					}
				 }
			 }
		 }
private: System::Void SaveEvent_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == DeleteEvent_button )
			 {
				 if(AllEvent_treeView->SelectedNode)
				 {
					 if(AllEvent_treeView->SelectedNode->Parent)
					 {
						WARING_YES_NO_TO_NO("please select event node!")
						return;
					 }
					 m_pTreeNodeWithName->Remove(AllEvent_treeView->SelectedNode->Text);
					 AllEvent_treeView->SelectedNode->Remove();
				 }
			 }
			 else
			 {
				 if(treeViewToXMLNode1->xmlForm1->CurrentAttribute_treeView->Nodes->Count>0)
				 {
					 if(DNCT::IsTextLegalForXMLElement(EventName_textBox))
					 {
						 if(IsTreeNodeContainText(AllEvent_treeView->Nodes,EventName_textBox->Text))
						 {
							WARING_YES_NO_TO_NO("replace exist data!?")
								return;
							GCFORM::TreeNode^l_pExistNode = (GCFORM::TreeNode^)m_pTreeNodeWithName[EventName_textBox->Text];
							assert(l_pExistNode);
							AllEvent_treeView->Nodes->Remove(l_pExistNode);
							m_pTreeNodeWithName->Remove(EventName_textBox->Text);
						 }
						 GCFORM::TreeNode^l_pNewNode = gcnew GCFORM::TreeNode(EventName_textBox->Text);
						 //first node is root
						 for each(GCFORM::TreeNode^l_pNode in treeViewToXMLNode1->xmlForm1->CurrentAttribute_treeView->Nodes[0]->Nodes)
						 //for each(GCFORM::TreeNode^l_pNode in treeViewToXMLNode1->xmlForm1->CurrentAttribute_treeView->Nodes)
						 {
							l_pNewNode->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
						 }
						 this->m_pTreeNodeWithName[EventName_textBox->Text] = l_pNewNode;
						 AllEvent_treeView->Nodes->Add(l_pNewNode);
					 }
				 }
				 else
				 {
					WARNING_MSG("please add data!!");
				 }
			 }
		 }
private: System::Void ReeditEvent_button_Click(System::Object^  sender, System::EventArgs^  e)
         {
            if(AllEvent_treeView->SelectedNode)
            {
				treeViewToXMLNode1->xmlForm1->CurrentAttribute_treeView->Nodes->Clear();
                treeViewToXMLNode1->xmlForm1->CurrentAttribute_treeView->Nodes->Add((GCFORM::TreeNode^)AllEvent_treeView->SelectedNode->Clone());
            }
         }
private: System::Void AllEvent_treeView_DoubleClick(System::Object^  sender, System::EventArgs^  e)
         {
            ReeditEvent_button_Click(sender,e);
         }
private: System::Void AllEvent_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		 {
			 if( AllEvent_treeView->SelectedNode )
				 EventName_textBox->Text = AllEvent_treeView->SelectedNode->Text;
		 }
private: System::Void splitContainer1_SplitterMoved(System::Object^  sender, System::Windows::Forms::SplitterEventArgs^  e)
		 {
			treeViewToXMLNode1->Size = System::Drawing::Size(treeViewToXMLNode1->Size.Width,splitContainer1->SplitterDistance-1);
		 }
private: System::Void Save_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( sender == Save_toolStripMenuItem )
			{
				SaveTreeViewToFile(AllEvent_treeView);
			}
			else
			{
				GCFORM::TreeNode^l_pTreeNode = DNCT::OpenXmlFileToNode(nullptr);
				if( l_pTreeNode )
				//if( l_pTreeNode->Text->ToLower()->Equals("root") )
				{
					AllEvent_treeView->Nodes->Clear();
					m_pTreeNodeWithName->Clear();
					for each( GCFORM::TreeNode^l_pMode in l_pTreeNode->Nodes)
					{
						GCFORM::TreeNode^l_pNewNode = (GCFORM::TreeNode^)l_pMode->Clone();
						AllEvent_treeView->Nodes->Add(l_pNewNode);
						this->m_pTreeNodeWithName[l_pNewNode->Text] = l_pNewNode;
					}
				}
			}
		 }
};
}
