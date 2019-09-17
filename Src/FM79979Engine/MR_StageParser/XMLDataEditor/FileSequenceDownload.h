#pragma once

#include <algorithm>
#include <string>
#include <iostream>
#include <cctype>

namespace XMLDataEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FileSequenceDownload
	/// </summary>
	public ref class FileSequenceDownload : public System::Windows::Forms::Form
	{
	public:
		FileSequenceDownload(void)
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
		~FileSequenceDownload()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 
	private: System::Windows::Forms::Button^  OpenFile_button;
	private: System::Windows::Forms::TextBox^  Prefix_textBox;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::TextBox^  End_textBox;
	private: System::Windows::Forms::TextBox^  FolderName_textBox;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::CheckBox^  AddSlash_checkBox;
	private: System::Windows::Forms::TextBox^  FileExtensionName_textBox;
	private: System::Windows::Forms::Label^  label5;



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
			this->OpenFile_button = (gcnew System::Windows::Forms::Button());
			this->Prefix_textBox = (gcnew System::Windows::Forms::TextBox());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->End_textBox = (gcnew System::Windows::Forms::TextBox());
			this->FolderName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->AddSlash_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->FileExtensionName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			this->SuspendLayout();
			// 
			// OpenFile_button
			// 
			this->OpenFile_button->Location = System::Drawing::Point(29, 474);
			this->OpenFile_button->Name = L"OpenFile_button";
			this->OpenFile_button->Size = System::Drawing::Size(117, 23);
			this->OpenFile_button->TabIndex = 1;
			this->OpenFile_button->Text = L"OpenFile";
			this->OpenFile_button->UseVisualStyleBackColor = true;
			this->OpenFile_button->Click += gcnew System::EventHandler(this, &FileSequenceDownload::OpenFile_button_Click);
			// 
			// Prefix_textBox
			// 
			this->Prefix_textBox->Location = System::Drawing::Point(29, 68);
			this->Prefix_textBox->Name = L"Prefix_textBox";
			this->Prefix_textBox->Size = System::Drawing::Size(347, 20);
			this->Prefix_textBox->TabIndex = 2;
			this->Prefix_textBox->Text = L"http://project.acg001.com/jx3/resource/assets/";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(36, 367);
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(46, 20);
			this->numericUpDown1->TabIndex = 4;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {50, 0, 0, 0});
			// 
			// End_textBox
			// 
			this->End_textBox->Location = System::Drawing::Point(29, 413);
			this->End_textBox->Name = L"End_textBox";
			this->End_textBox->Size = System::Drawing::Size(158, 20);
			this->End_textBox->TabIndex = 5;
			this->End_textBox->Text = L".jpg";
			// 
			// FolderName_textBox
			// 
			this->FolderName_textBox->Location = System::Drawing::Point(29, 147);
			this->FolderName_textBox->Multiline = true;
			this->FolderName_textBox->Name = L"FolderName_textBox";
			this->FolderName_textBox->Size = System::Drawing::Size(238, 180);
			this->FolderName_textBox->TabIndex = 6;
			this->FolderName_textBox->Text = L"cangjian\r\ncanyon\r\nchange\r\nchunyang\r\ngaibang\r\nmingjiao\r\nqixiu\r\nshaolin\r\ntangmen\r\nt" 
				L"iance\r\nwanhua\r\nkudu\r\nwudu";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(33, 33);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(29, 13);
			this->label1->TabIndex = 7;
			this->label1->Text = L"Start";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(33, 397);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(26, 13);
			this->label2->TabIndex = 8;
			this->label2->Text = L"End";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(26, 131);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(121, 13);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Dynamic for each Name";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(33, 351);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(35, 13);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Count";
			// 
			// AddSlash_checkBox
			// 
			this->AddSlash_checkBox->AutoSize = true;
			this->AddSlash_checkBox->Checked = true;
			this->AddSlash_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->AddSlash_checkBox->Location = System::Drawing::Point(299, 197);
			this->AddSlash_checkBox->Name = L"AddSlash_checkBox";
			this->AddSlash_checkBox->Size = System::Drawing::Size(137, 17);
			this->AddSlash_checkBox->TabIndex = 11;
			this->AddSlash_checkBox->Text = L"add slash after dynamic";
			this->AddSlash_checkBox->UseVisualStyleBackColor = true;
			// 
			// FileExtensionName_textBox
			// 
			this->FileExtensionName_textBox->Location = System::Drawing::Point(278, 413);
			this->FileExtensionName_textBox->Name = L"FileExtensionName_textBox";
			this->FileExtensionName_textBox->Size = System::Drawing::Size(158, 20);
			this->FileExtensionName_textBox->TabIndex = 12;
			this->FileExtensionName_textBox->Text = L".jpg";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(276, 397);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(97, 13);
			this->label5->TabIndex = 13;
			this->label5->Text = L"file extension name";
			// 
			// FileSequenceDownload
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1884, 962);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->FileExtensionName_textBox);
			this->Controls->Add(this->AddSlash_checkBox);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->FolderName_textBox);
			this->Controls->Add(this->End_textBox);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->Prefix_textBox);
			this->Controls->Add(this->OpenFile_button);
			this->Name = L"FileSequenceDownload";
			this->Text = L"FileSequenceDownload";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void OpenFile_button_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 String^l_strDirectory = DNCT::SelectDirectory();
				 if( !l_strDirectory )
				 {
					l_strDirectory = L"";
				 }
				 String^l_str79979;// = "cangjian,cangyun,change,chunyang,gaibang,mingjiao,qixiu,shaolin,tangmen,tiance,wanhua,kudu,wudu";
				 cli::array<String^>^l_strArray;
				 if( !l_str79979 )
					 l_strArray = FolderName_textBox->Text->Split(L'\n');
				 else
					 l_strArray = l_str79979->Split(L',');
				 String^l_strPrefix = Prefix_textBox->Text;
				 std::wstring l_strPreFix = DNCT::GcStringToWchar(this->Prefix_textBox->Text);
				 std::wstring l_strEnd = DNCT::GcStringToWchar(this->End_textBox->Text);
				 for each(String^l_str in l_strArray  )
				 {
					 int l_iLength = l_str->Length;
					 int l_iIndex = l_str->IndexOf(13);
					 if( l_iIndex != -1 )
						l_str->Remove(l_iIndex,1);
					 std::wstring l_strPreFix2 = l_strPreFix;
					 l_strPreFix2 += DNCT::GcStringToWchar(l_str);
					 //for change line..fuck
					 char l_strChangeLine = '\n';
					 l_strPreFix2.erase(std::remove(l_strPreFix2.begin(), l_strPreFix2.end(), 13),l_strPreFix2.end());
					 l_strPreFix2.erase(std::remove(l_strPreFix2.begin(), l_strPreFix2.end(), l_strChangeLine),l_strPreFix2.end());
					 if( AddSlash_checkBox->Checked )
						l_strPreFix2 += L"/";
					 int l_iNum = (int)numericUpDown1->Value;
					 for( int i=0;i<l_iNum;++i )
					 {
						std::wstring l_strResult = l_strPreFix2;
						l_strResult += DNCT::GcStringToWchar(i.ToString());
						l_strResult += l_strEnd;
						std::wstring l_strOutputFileName = L"";
						if( l_strDirectory && l_strDirectory->Length )
						{
							l_strOutputFileName += DNCT::GcStringToWchar(l_strDirectory);
						}
						l_strOutputFileName += DNCT::GcStringToWchar(gcnew String(l_str)+i.ToString());
						if( FileExtensionName_textBox->Text->Length )
							l_strOutputFileName += DNCT::GcStringToWchar(FileExtensionName_textBox->Text);
						//for change line..fuck
						l_strOutputFileName.erase(std::remove(l_strOutputFileName.begin(), l_strOutputFileName.end(), 13),l_strOutputFileName.end());
						l_strOutputFileName.erase(std::remove(l_strOutputFileName.begin(), l_strOutputFileName.end(), l_strChangeLine),l_strOutputFileName.end());
						//cGameApp::OutputDebugInfoString(L"url");
						//cGameApp::OutputDebugInfoString(l_strResult);
						//cGameApp::OutputDebugInfoString(L"fileName");
						//cGameApp::OutputDebugInfoString(l_strOutputFileName);
						cNetwork::DownloadFile((WCHAR*)l_strResult.c_str(),(WCHAR*)l_strOutputFileName.c_str());
					 }
				 }
			 }
	};
}
//
