#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace DrawFont {

	/// <summary>
	/// NodeSearchForm 的摘要
	///
	/// 警告: 如果您變更這個類別的名稱，就必須變更與這個類別所依據之所有 .resx 檔案關聯的
	///          Managed 資源編譯器工具的 'Resource File Name' 屬性。
	///          否則，這些設計工具
	///          將無法與這個表單關聯的當地語系化資源
	///          正確互動。
	/// </summary>
	public ref class NodeSearchForm : public System::Windows::Forms::Form
	{
	public:
		NodeSearchForm(GCFORM::TreeView^e_pTreeView)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			Down_radioButton->Checked = true;
			m_pTreeView = e_pTreeView;
			FindNext_button->Enabled = false;
			m_pNodesForSearch = gcnew System::Collections::ArrayList();
			m_iCurrentFind = 0;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~NodeSearchForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::CheckBox^  Group_checkBox;
	private: System::Windows::Forms::CheckBox^  Child_checkBox;


	private: System::Windows::Forms::GroupBox^  groupBox1;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  FindNext_button;

	private: System::Windows::Forms::Button^  button2;
	private: GCFORM::TreeView^	m_pTreeView;
	private: System::Windows::Forms::RadioButton^  Up_radioButton;

	private: System::Windows::Forms::RadioButton^  Down_radioButton;
	private: System::Collections::ArrayList^	m_pNodesForSearch;
	private: int m_iCurrentFind;

	protected: 

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->Group_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Child_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->Up_radioButton = (gcnew System::Windows::Forms::RadioButton());
			this->Down_radioButton = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->FindNext_button = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(79, 12);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(235, 20);
			this->textBox1->TabIndex = 0;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &NodeSearchForm::textBox1_TextChanged);
			// 
			// Group_checkBox
			// 
			this->Group_checkBox->AutoSize = true;
			this->Group_checkBox->Checked = true;
			this->Group_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->Group_checkBox->Location = System::Drawing::Point(20, 48);
			this->Group_checkBox->Name = L"Group_checkBox";
			this->Group_checkBox->Size = System::Drawing::Size(55, 17);
			this->Group_checkBox->TabIndex = 1;
			this->Group_checkBox->Text = L"Group";
			this->Group_checkBox->UseVisualStyleBackColor = true;
			// 
			// Child_checkBox
			// 
			this->Child_checkBox->AutoSize = true;
			this->Child_checkBox->Checked = true;
			this->Child_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->Child_checkBox->Location = System::Drawing::Point(20, 72);
			this->Child_checkBox->Name = L"Child_checkBox";
			this->Child_checkBox->Size = System::Drawing::Size(49, 17);
			this->Child_checkBox->TabIndex = 2;
			this->Child_checkBox->Text = L"Child";
			this->Child_checkBox->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->Up_radioButton);
			this->groupBox1->Controls->Add(this->Down_radioButton);
			this->groupBox1->Location = System::Drawing::Point(129, 40);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(167, 65);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Direction";
			// 
			// Up_radioButton
			// 
			this->Up_radioButton->AutoSize = true;
			this->Up_radioButton->Location = System::Drawing::Point(6, 13);
			this->Up_radioButton->Name = L"Up_radioButton";
			this->Up_radioButton->Size = System::Drawing::Size(39, 17);
			this->Up_radioButton->TabIndex = 1;
			this->Up_radioButton->Text = L"Up";
			this->Up_radioButton->UseVisualStyleBackColor = true;
			// 
			// Down_radioButton
			// 
			this->Down_radioButton->AutoSize = true;
			this->Down_radioButton->Checked = true;
			this->Down_radioButton->Location = System::Drawing::Point(6, 41);
			this->Down_radioButton->Name = L"Down_radioButton";
			this->Down_radioButton->Size = System::Drawing::Size(53, 17);
			this->Down_radioButton->TabIndex = 0;
			this->Down_radioButton->TabStop = true;
			this->Down_radioButton->Text = L"Down";
			this->Down_radioButton->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(17, 14);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(56, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Find waht:";
			// 
			// FindNext_button
			// 
			this->FindNext_button->Location = System::Drawing::Point(331, 14);
			this->FindNext_button->Name = L"FindNext_button";
			this->FindNext_button->Size = System::Drawing::Size(80, 23);
			this->FindNext_button->TabIndex = 6;
			this->FindNext_button->Text = L"Find Next";
			this->FindNext_button->UseVisualStyleBackColor = true;
			this->FindNext_button->Click += gcnew System::EventHandler(this, &NodeSearchForm::FindNext_button_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(331, 48);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(80, 23);
			this->button2->TabIndex = 7;
			this->button2->Text = L"Cancel";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &NodeSearchForm::button2_Click);
			// 
			// NodeSearchForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(438, 117);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->FindNext_button);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->Child_checkBox);
			this->Controls->Add(this->Group_checkBox);
			this->Controls->Add(this->textBox1);
			this->Name = L"NodeSearchForm";
			this->Text = L"NodeSearchForm";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 if( textBox1->Text->Length )
				 FindNext_button->Enabled = true;
			 else
				 FindNext_button->Enabled = false;
			 m_iCurrentFind = 0;
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void FindNext_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 m_pNodesForSearch->Clear();
			 if( this->m_pTreeView->Nodes->Count )
			 {
				 SearchNodes(this->m_pTreeView->Nodes[0],m_pNodesForSearch,textBox1->Text->ToString());
				 if( m_pNodesForSearch->Count )
				 {
					 m_pTreeView->SelectedNode->BackColor = System::Drawing::Color::White;
					 m_pTreeView->SelectedNode = static_cast<GCFORM::TreeNode^>(m_pNodesForSearch[m_pNodesForSearch->Count-1-m_iCurrentFind]);
					 m_pTreeView->SelectedNode->BackColor = System::Drawing::Color::Gold;
					 Up_radioButton->Checked?m_iCurrentFind--:m_iCurrentFind++;
					 if(m_iCurrentFind<0)
					 {
						m_iCurrentFind = 0;
						WARNING_MSG("on the top");
					 }
					 if(m_iCurrentFind >= m_pNodesForSearch->Count)
					 {
						m_iCurrentFind = m_pNodesForSearch->Count-1;
						WARNING_MSG("on the Down");
					 }
				 }
				 else
				 {
					WARNING_MSG("Nothing Find");
				 }
			 }
			 
		 }
private: System::Void SearchNodes(GCFORM::TreeNode^e_pNode,System::Collections::ArrayList^e_pArrayList,System::String^e_pStringToCompare)
{
	if (e_pNode->NextNode)
	{
		SearchNodes(e_pNode->NextNode,e_pArrayList,e_pStringToCompare);
	}

	if (e_pNode->Nodes->Count)
	{
		SearchNodes(e_pNode->Nodes[0],e_pArrayList,e_pStringToCompare);
	}

	if( Child_checkBox->Checked )
	{//has parent and no child
		if( e_pNode->Parent&&!e_pNode->Nodes->Count )
		{
			if( e_pNode->Text->Contains(e_pStringToCompare) )
				e_pArrayList->Add(e_pNode);			
		}
	}

	if( Group_checkBox->Checked )
	{
		if( e_pNode->Nodes->Count )
		{
			if( !e_pArrayList->Contains(e_pNode) )
				if( e_pNode->Text->Contains(e_pStringToCompare) )
					e_pArrayList->Add(e_pNode);		
		}
		//for no parent and no children
		if( !e_pNode->Nodes->Count && !e_pNode->Parent )
			if( !e_pArrayList->Contains(e_pNode) )
				if( e_pNode->Text->Contains(e_pStringToCompare) )
					e_pArrayList->Add(e_pNode);
	}
}

};
}
