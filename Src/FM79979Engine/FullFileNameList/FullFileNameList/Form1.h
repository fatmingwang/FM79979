#pragma once
#include "..\..\DotNetCommon\DotNetCommonTools.h"

namespace FullFileNameList {

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
			m_AllfileNameList = gcnew System::Collections::ArrayList;
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
	private: System::Windows::Forms::Button^  DirectorySelect_button;
	private: System::Windows::Forms::TextBox^  ExtensionName_textBox;
	private: System::Windows::Forms::TextBox^  AllFileList_textBox;
	private: System::Windows::Forms::CheckBox^  StripDirectory_checkBox;
	private: System::Windows::Forms::Label^  label1;
			 System::Collections::ArrayList^m_AllfileNameList;
	private: System::Windows::Forms::Button^  Current_button;
	private: System::Windows::Forms::CheckBox^  StripExtensionName_checkBox;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fontToolStripMenuItem;

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
			this->DirectorySelect_button = (gcnew System::Windows::Forms::Button());
			this->ExtensionName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->AllFileList_textBox = (gcnew System::Windows::Forms::TextBox());
			this->StripDirectory_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->Current_button = (gcnew System::Windows::Forms::Button());
			this->StripExtensionName_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fontToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// DirectorySelect_button
			// 
			this->DirectorySelect_button->Location = System::Drawing::Point(272, 10);
			this->DirectorySelect_button->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->DirectorySelect_button->Name = L"DirectorySelect_button";
			this->DirectorySelect_button->Size = System::Drawing::Size(85, 20);
			this->DirectorySelect_button->TabIndex = 0;
			this->DirectorySelect_button->Text = L"WholeDirectory";
			this->DirectorySelect_button->UseVisualStyleBackColor = true;
			this->DirectorySelect_button->Click += gcnew System::EventHandler(this, &Form1::DirectorySelect_button_Click);
			// 
			// ExtensionName_textBox
			// 
			this->ExtensionName_textBox->Location = System::Drawing::Point(92, 22);
			this->ExtensionName_textBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->ExtensionName_textBox->Name = L"ExtensionName_textBox";
			this->ExtensionName_textBox->Size = System::Drawing::Size(164, 22);
			this->ExtensionName_textBox->TabIndex = 1;
			this->ExtensionName_textBox->Text = L".c,.cpp,.mm";
			// 
			// AllFileList_textBox
			// 
			this->AllFileList_textBox->Location = System::Drawing::Point(-1, 57);
			this->AllFileList_textBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->AllFileList_textBox->Multiline = true;
			this->AllFileList_textBox->Name = L"AllFileList_textBox";
			this->AllFileList_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->AllFileList_textBox->Size = System::Drawing::Size(643, 363);
			this->AllFileList_textBox->TabIndex = 2;
			// 
			// StripDirectory_checkBox
			// 
			this->StripDirectory_checkBox->AutoSize = true;
			this->StripDirectory_checkBox->Location = System::Drawing::Point(406, 13);
			this->StripDirectory_checkBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->StripDirectory_checkBox->Name = L"StripDirectory_checkBox";
			this->StripDirectory_checkBox->Size = System::Drawing::Size(90, 16);
			this->StripDirectory_checkBox->TabIndex = 3;
			this->StripDirectory_checkBox->Text = L"StripDirectory";
			this->StripDirectory_checkBox->UseVisualStyleBackColor = true;
			this->StripDirectory_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::StripDirectory_checkBox_CheckedChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(17, 16);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 24);
			this->label1->TabIndex = 4;
			this->label1->Text = L"extension name\r\n\',\' to seperate";
			// 
			// Current_button
			// 
			this->Current_button->Location = System::Drawing::Point(272, 34);
			this->Current_button->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Current_button->Name = L"Current_button";
			this->Current_button->Size = System::Drawing::Size(85, 20);
			this->Current_button->TabIndex = 5;
			this->Current_button->Text = L"CurrentDirectory";
			this->Current_button->UseVisualStyleBackColor = true;
			this->Current_button->Click += gcnew System::EventHandler(this, &Form1::Current_button_Click);
			// 
			// StripExtensionName_checkBox
			// 
			this->StripExtensionName_checkBox->AutoSize = true;
			this->StripExtensionName_checkBox->Location = System::Drawing::Point(406, 37);
			this->StripExtensionName_checkBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->StripExtensionName_checkBox->Name = L"StripExtensionName_checkBox";
			this->StripExtensionName_checkBox->Size = System::Drawing::Size(119, 16);
			this->StripExtensionName_checkBox->TabIndex = 6;
			this->StripExtensionName_checkBox->Text = L"StripExtensionName";
			this->StripExtensionName_checkBox->UseVisualStyleBackColor = true;
			this->StripExtensionName_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::StripDirectory_checkBox_CheckedChanged);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fontToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(640, 24);
			this->menuStrip1->TabIndex = 7;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fontToolStripMenuItem
			// 
			this->fontToolStripMenuItem->Name = L"fontToolStripMenuItem";
			this->fontToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->fontToolStripMenuItem->Text = L"Font";
			this->fontToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::fontToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(640, 417);
			this->Controls->Add(this->StripExtensionName_checkBox);
			this->Controls->Add(this->Current_button);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->StripDirectory_checkBox);
			this->Controls->Add(this->AllFileList_textBox);
			this->Controls->Add(this->ExtensionName_textBox);
			this->Controls->Add(this->DirectorySelect_button);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"Form1";
			this->Text = L"FileNameList";
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e)
			 {
				 AllFileList_textBox->Size = System::Drawing::Size(this->Size.Width,this->Size.Height-AllFileList_textBox->Location.Y);
			 }

	private: System::Void	StringChangeToTextBox(System::Collections::ArrayList^e_pArrayList)
			 {
				AllFileList_textBox->Text = "";
				//search all file
				if( ExtensionName_textBox->Text->Trim()->Length == 0 )
				{
					for each(String^l_FileName in e_pArrayList)
					{
						if( StripDirectory_checkBox->Checked )
						{
							if( !StripExtensionName_checkBox->Checked )
								AllFileList_textBox->Text += System::IO::Path::GetFileName(l_FileName);
							else
								AllFileList_textBox->Text += System::IO::Path::GetFileNameWithoutExtension(l_FileName);
						}
						else
						{
							if( !StripExtensionName_checkBox->Checked )
								AllFileList_textBox->Text += l_FileName;
							else
								AllFileList_textBox->Text += System::IO::Path::GetFileNameWithoutExtension(l_FileName);
							
						}
						AllFileList_textBox->Text += DNCT::GetChanglineString();
					}
				}
				else
				{
					array<Char>^l_Chars = {','};
					array<String^>^l_strExtensionNameSplit = ExtensionName_textBox->Text->Split( l_Chars );
					for each(String^l_FileName in e_pArrayList)
					{
						for each( String^l_strExtenName in l_strExtensionNameSplit )
						{
							String^l_strDestxtensionName = System::IO::Path::GetExtension(l_FileName);
							if( l_strDestxtensionName->Length )
							{//same extension name
								if( l_strDestxtensionName->Equals(l_strExtenName) )
								{//keep directory exists
									if( StripDirectory_checkBox->Checked )
									{
										if( !StripExtensionName_checkBox->Checked )
											AllFileList_textBox->Text += System::IO::Path::GetFileName(l_FileName);
										else
											AllFileList_textBox->Text += System::IO::Path::GetFileNameWithoutExtension(l_FileName);
									}
									else//only file name
									{
										if( !StripExtensionName_checkBox->Checked )
											AllFileList_textBox->Text += l_FileName;
										else
											AllFileList_textBox->Text += System::IO::Path::GetFileNameWithoutExtension(l_FileName);
									}
									AllFileList_textBox->Text += DNCT::GetChanglineString();
								}
							}
						}
					}
				}				
			 }

	private: System::Void DirectorySelect_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 System::String^l_Directory = DNCT::SelectDirectory();
				 if( l_Directory )
				 {
					m_AllfileNameList->Clear();
					GetFilesNameByRecursivelyDirectory(l_Directory,m_AllfileNameList);
					StringChangeToTextBox(m_AllfileNameList);
				 }
			 }
private: System::Void StripDirectory_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_AllfileNameList->Count )
				StringChangeToTextBox(m_AllfileNameList);
		 }
private: System::Void Current_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 static bool	l_b = false;
			 if( l_b )
				 return;
			 l_b = true;
			 System::String^l_Directory = DNCT::SelectDirectory();
			 if( l_Directory )
			 {
				m_AllfileNameList->Clear();
				GetFilesNameByRecursivelyDirectory(l_Directory,m_AllfileNameList);
				int	l_iCurrentirectoryNameLength = 0;
				for(int i=l_Directory->Length-2;i>0;--i)
				{
					if(l_Directory[i] == L'\\'||l_Directory[i] == L'/' )
					{
						i = -1;
					}
					l_iCurrentirectoryNameLength++;
				}
				int	l_iLength = l_Directory->Length-l_iCurrentirectoryNameLength;
				for (int i=0;i<m_AllfileNameList->Count;++i)
				{
					String^l_strNewFileName = (String^)m_AllfileNameList[i];
					m_AllfileNameList[i] = l_strNewFileName->Substring(l_iLength);
				}
				StringChangeToTextBox(m_AllfileNameList);
			 }
			 l_b = false;
		 }
private: System::Void fontToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 System::Drawing::Font^l_pFont = DNCT::GetFontFromFontDialog();
			 if( l_pFont )
				AllFileList_textBox->Font = l_pFont;
		 }
};
}

