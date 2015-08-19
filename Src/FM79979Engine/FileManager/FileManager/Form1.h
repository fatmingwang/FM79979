#pragma once

#include "../../DotNetCommon/DotNetCommonTools.h"

namespace FileManager {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 的摘要
	///
	/// 警告: 如果您變更這個類別的名稱，就必須變更與這個類別所依據之所有 .resx 檔案關聯的
	///          Managed 資源編譯器工具的 'Resource File Name' 屬性。
	///          否則，這些設計工具
	///          將無法與這個表單關聯的當地語系化資源
	///          正確互動。
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			GCFORM::TreeNode^l_pXMLTreeNode = DNCT::OpenXmlFileToNode("Setup.xml");
			if( l_pXMLTreeNode )
			{
				if( l_pXMLTreeNode->Nodes->Count )
				{
					m_pXMLTreeNode = l_pXMLTreeNode->Nodes[0];
					for each(GCFORM::TreeNode^l_pCurrentNode in l_pXMLTreeNode->Nodes[0]->Nodes)
					{
						QuickSearchData_comboBox->Items->Add(l_pCurrentNode->Text);
					}
				}
			}

		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  AllFiles_listBox;
	private: System::Windows::Forms::TextBox^  SearchKeyWord_textBox;
	private: System::Windows::Forms::ComboBox^  QuickSearchData_comboBox;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  Delete_button;
	private: System::Windows::Forms::Button^  SelectFolder_button;

	private: System::Windows::Forms::SplitContainer^  splitContainer1;
			 GCFORM::TreeNode^m_pXMLTreeNode;
	private: System::Windows::Forms::TextBox^  AllFileName_textBox;
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
			this->AllFiles_listBox = (gcnew System::Windows::Forms::ListBox());
			this->SearchKeyWord_textBox = (gcnew System::Windows::Forms::TextBox());
			this->QuickSearchData_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->Delete_button = (gcnew System::Windows::Forms::Button());
			this->SelectFolder_button = (gcnew System::Windows::Forms::Button());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->AllFileName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			this->AllFiles_listBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->AllFiles_listBox->FormattingEnabled = true;
			this->AllFiles_listBox->ItemHeight = 12;
			this->AllFiles_listBox->Location = System::Drawing::Point(0, 0);
			this->AllFiles_listBox->Margin = System::Windows::Forms::Padding(2);
			this->AllFiles_listBox->Name = L"AllFiles_listBox";
			this->AllFiles_listBox->ScrollAlwaysVisible = true;
			this->AllFiles_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AllFiles_listBox->Size = System::Drawing::Size(350, 364);
			this->AllFiles_listBox->TabIndex = 1;
			this->SearchKeyWord_textBox->Location = System::Drawing::Point(80, 2);
			this->SearchKeyWord_textBox->Margin = System::Windows::Forms::Padding(2);
			this->SearchKeyWord_textBox->Name = L"SearchKeyWord_textBox";
			this->SearchKeyWord_textBox->Size = System::Drawing::Size(116, 22);
			this->SearchKeyWord_textBox->TabIndex = 3;
			this->SearchKeyWord_textBox->Text = L".cpp,.h,.mm";
			this->QuickSearchData_comboBox->FormattingEnabled = true;
			this->QuickSearchData_comboBox->Location = System::Drawing::Point(80, 30);
			this->QuickSearchData_comboBox->Margin = System::Windows::Forms::Padding(2);
			this->QuickSearchData_comboBox->Name = L"QuickSearchData_comboBox";
			this->QuickSearchData_comboBox->Size = System::Drawing::Size(121, 20);
			this->QuickSearchData_comboBox->TabIndex = 4;
			this->QuickSearchData_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::QuickSearchData_comboBox_SelectedIndexChanged);
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(15, 36);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(64, 12);
			this->label1->TabIndex = 5;
			this->label1->Text = L"QuickSearch";
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(15, 10);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(51, 12);
			this->label2->TabIndex = 6;
			this->label2->Text = L"KeyWord";
			this->Delete_button->Location = System::Drawing::Point(236, 36);
			this->Delete_button->Margin = System::Windows::Forms::Padding(2);
			this->Delete_button->Name = L"Delete_button";
			this->Delete_button->Size = System::Drawing::Size(64, 22);
			this->Delete_button->TabIndex = 8;
			this->Delete_button->Text = L"DeleteFiles";
			this->Delete_button->UseVisualStyleBackColor = true;
			this->Delete_button->Click += gcnew System::EventHandler(this, &Form1::Delete_button_Click);
			this->SelectFolder_button->Location = System::Drawing::Point(236, 6);
			this->SelectFolder_button->Margin = System::Windows::Forms::Padding(2);
			this->SelectFolder_button->Name = L"SelectFolder_button";
			this->SelectFolder_button->Size = System::Drawing::Size(64, 22);
			this->SelectFolder_button->TabIndex = 9;
			this->SelectFolder_button->Text = L"SelectFolder";
			this->SelectFolder_button->UseVisualStyleBackColor = true;
			this->SelectFolder_button->Click += gcnew System::EventHandler(this, &Form1::SelectFolder_button_Click);
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->splitContainer1->Location = System::Drawing::Point(17, 70);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Panel1->Controls->Add(this->AllFiles_listBox);
			this->splitContainer1->Panel2->Controls->Add(this->AllFileName_textBox);
			this->splitContainer1->Size = System::Drawing::Size(743, 376);
			this->splitContainer1->SplitterDistance = 352;
			this->splitContainer1->SplitterWidth = 3;
			this->splitContainer1->TabIndex = 11;
			this->AllFileName_textBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->AllFileName_textBox->Location = System::Drawing::Point(0, 0);
			this->AllFileName_textBox->Multiline = true;
			this->AllFileName_textBox->Name = L"AllFileName_textBox";
			this->AllFileName_textBox->Size = System::Drawing::Size(386, 374);
			this->AllFileName_textBox->TabIndex = 0;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1363, 456);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->SelectFolder_button);
			this->Controls->Add(this->Delete_button);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->QuickSearchData_comboBox);
			this->Controls->Add(this->SearchKeyWord_textBox);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->Panel2->PerformLayout();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void QuickSearchData_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				if( QuickSearchData_comboBox->SelectedIndex != -1)
				{
					SearchKeyWord_textBox->Text = m_pXMLTreeNode->Nodes[QuickSearchData_comboBox->SelectedIndex]->Name;
				}
			 }
private: System::Void SelectFolder_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strDirectoryName = DNCT::SelectDirectory();
			 if( l_strDirectoryName )
			 {
				 AllFileName_textBox->Text = "";
				AllFiles_listBox->Items->Clear();
				System::Collections::ArrayList^l_pFileArrayList = gcnew System::Collections::ArrayList;
				System::Collections::ArrayList^l_pDirectoryArrayList = gcnew System::Collections::ArrayList;
				DNCT::GetDiectoriesAndFilesNameByRecursivelyDirectory(l_strDirectoryName,l_pFileArrayList,l_pDirectoryArrayList);
				array<Char>^l_Chars = {','};
				array<String^>^l_strExtensionNameSplit = SearchKeyWord_textBox->Text->Split( l_Chars );
				for each(String^l_FileName in l_pFileArrayList)
				{
					for each( String^l_strExtenName in l_strExtensionNameSplit )
					{
						if(l_FileName->Contains(l_strExtenName))
						{
							//AllFiles_listBox->Items->Add(l_FileName);
							AllFileName_textBox->Text += l_FileName;
							AllFileName_textBox->Text += "						/";
							AllFileName_textBox->Text += DNCT::GetChanglineString();
						}
					}
				}
				//for( int i=0;i<AllFiles_listBox->Items->Count;++i )
				//	AllFiles_listBox->SelectedIndices->Add(i);
			 }
		 }

private: System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e)
		 {
			 System::Drawing::Point	l_Location = splitContainer1->Location;
			 System::Drawing::Size	l_Size = this->Size;
			 splitContainer1->Size = System::Drawing::Size(l_Size.Width-l_Location.X-20,l_Size.Height-l_Location.Y-30);
		 }

private: System::Void Delete_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			//while(AllDirectories_listBox->SelectedIndices->Count)
			//{
			//	String^l_strDirectoryName = AllDirectories_listBox->Items[AllDirectories_listBox->SelectedIndices[0]]->ToString();
			//	try
			//	{
			//		if(System::IO::Directory::Exists(l_strDirectoryName))
			//		{
			//			System::Collections::ArrayList^l_pArrayList = gcnew System::Collections::ArrayList;
			//			DNCT::GetFilesNameByRecursivelyDirectory(l_strDirectoryName,l_pArrayList);
			//			for each(String^l_strFileName in l_pArrayList)
			//			{
			//				System::Security::AccessControl::FileSecurity^l_pgcnew 
			//				System::Security::AccessControl::AccessControlActions::None
			//				System::IO::File::SetAccessControl("",
			//				System::IO::FileInfo^l_FileInfo = gcnew System::IO::FileInfo(l_strFileName);
			//				System::IO::File::Delete(l_strFileName);
			//			}
			//			System::IO::Directory::Delete(l_strDirectoryName);
			//		}
			//	}
			//	catch(System::Exception^l_pExp)
			//	{
			//		WARNING_MSG(l_pExp->ToString());
			//	}
			//	AllDirectories_listBox->Items->RemoveAt(AllDirectories_listBox->SelectedIndices[0]);
			//}
			while(AllFiles_listBox->SelectedIndices->Count)
			{
				String^l_strFileName = AllFiles_listBox->Items[AllFiles_listBox->SelectedIndices[0]]->ToString();
				try
				{
					if(System::IO::File::Exists(l_strFileName))
					{
						System::IO::File::Delete(l_strFileName);
					}
				}
				catch(System::Exception^l_pExp)
				{
						WARNING_MSG(l_pExp->ToString());
				}
				AllFiles_listBox->Items->RemoveAt(AllFiles_listBox->SelectedIndices[0]);
			}
		 }
};
}