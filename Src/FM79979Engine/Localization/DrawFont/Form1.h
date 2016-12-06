#pragma once
#include "ShowFontPic.h"
#include "TextEditor.h"
#define MAX_LENGTH_PERLINE 50
namespace DrawFont {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			//System::String^l_pString = DNCT::OpenFileAndGetName();
			//WCHAR	l_str[MAX_PATH];
			//GCSTRING_TO_WCHAR(l_pString,l_str);
			//CZipFile	l_CZipFile;
			//l_CZipFile.LoadZipFile(l_str);
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			textBox1->Text = gcnew  System::String("123456789");
			WCHAR	l_temp[MAX_PATH];
			GetCurrentDirectory( MAX_PATH,  l_temp);
			m_pStringForExeFilePath = TO_GCSTRING(l_temp);
			
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::MainMenu^  mainMenu1;
	private: System::Windows::Forms::MenuItem^  menuItem1;
	private: System::Windows::Forms::MenuItem^  menuItem2;
	private: System::Windows::Forms::MenuItem^  menuItem3;
	private: System::Windows::Forms::MenuItem^  menuItem4;
	private: System::Windows::Forms::MenuItem^  menuItem5;
	private: System::Windows::Forms::MenuItem^  menuItem6;
	private: System::Windows::Forms::MenuItem^  menuItem7;
	private: System::Windows::Forms::MenuItem^  menuItem8;
	private: System::ComponentModel::IContainer^  components;

			 //===============================================
	private: System::String	^m_pStringForExeFilePath;
	protected: 

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
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->mainMenu1 = (gcnew System::Windows::Forms::MainMenu(this->components));
			this->menuItem1 = (gcnew System::Windows::Forms::MenuItem());
			this->menuItem2 = (gcnew System::Windows::Forms::MenuItem());
			this->menuItem3 = (gcnew System::Windows::Forms::MenuItem());
			this->menuItem4 = (gcnew System::Windows::Forms::MenuItem());
			this->menuItem5 = (gcnew System::Windows::Forms::MenuItem());
			this->menuItem6 = (gcnew System::Windows::Forms::MenuItem());
			this->menuItem8 = (gcnew System::Windows::Forms::MenuItem());
			this->menuItem7 = (gcnew System::Windows::Forms::MenuItem());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->AcceptsReturn = true;
			this->textBox1->AcceptsTab = true;
			this->textBox1->AllowDrop = true;
			this->textBox1->Location = System::Drawing::Point(12, 12);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->textBox1->Size = System::Drawing::Size(400, 275);
			this->textBox1->TabIndex = 3;
			this->textBox1->WordWrap = false;
			// 
			// mainMenu1
			// 
			this->mainMenu1->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(3) {this->menuItem1, this->menuItem4, 
				this->menuItem7});
			// 
			// menuItem1
			// 
			this->menuItem1->Index = 0;
			this->menuItem1->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(2) {this->menuItem2, this->menuItem3});
			this->menuItem1->Text = L"FontPic";
			// 
			// menuItem2
			// 
			this->menuItem2->Index = 0;
			this->menuItem2->Text = L"OpenText";
			this->menuItem2->Click += gcnew System::EventHandler(this, &Form1::SelectDefaultTextFile);
			// 
			// menuItem3
			// 
			this->menuItem3->Index = 1;
			this->menuItem3->Text = L"SaveBmp";
			this->menuItem3->Click += gcnew System::EventHandler(this, &Form1::SaveBMPFile);
			// 
			// menuItem4
			// 
			this->menuItem4->Index = 1;
			this->menuItem4->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(3) {this->menuItem5, this->menuItem6, 
				this->menuItem8});
			this->menuItem4->Text = L"Parse Character";
			// 
			// menuItem5
			// 
			this->menuItem5->Index = 0;
			this->menuItem5->Text = L"OpenFileToScan";
			this->menuItem5->Click += gcnew System::EventHandler(this, &Form1::menuItem5_Click);
			// 
			// menuItem6
			// 
			this->menuItem6->Index = 1;
			this->menuItem6->Text = L"Save";
			this->menuItem6->Click += gcnew System::EventHandler(this, &Form1::menuItem6_Click);
			// 
			// menuItem8
			// 
			this->menuItem8->Index = 2;
			this->menuItem8->Text = L"CompareCharacter";
			this->menuItem8->Click += gcnew System::EventHandler(this, &Form1::menuItem8_Click);
			// 
			// menuItem7
			// 
			this->menuItem7->Index = 2;
			this->menuItem7->Text = L"TextEditor";
			this->menuItem7->Click += gcnew System::EventHandler(this, &Form1::menuItem7_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(442, 298);
			this->Controls->Add(this->textBox1);
			this->Menu = this->mainMenu1;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->SizeChanged += gcnew System::EventHandler(this, &Form1::Form1_SizeChanged);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void SaveBMPFile(System::Object ^  sender, System::EventArgs ^  e)
			 {
				if(textBox1->Text&&!textBox1->Text->Length)
					return;
				ShowFontPic^	l_p = gcnew  ShowFontPic(textBox1->Text);
				l_p->Show();
			 }

	private: System::Void SelectDefaultTextFile(System::Object ^  sender, System::EventArgs ^  e)
			 {
				 System::Windows::Forms::OpenFileDialog^	l_pOpenFile = gcnew  System::Windows::Forms::OpenFileDialog();
				 if( l_pOpenFile->ShowDialog() == System::Windows::Forms::DialogResult::OK )
				 {
					System::IO::StreamReader ^sr = gcnew  System::IO::StreamReader(l_pOpenFile->FileName);
					textBox1->Text = sr->ReadToEnd();
					//unicode problem!!
					if( textBox1->Text->Equals(L" ") ||textBox1->Text->Equals(L"0")||
						textBox1->Text->Equals(" ") ||textBox1->Text->Equals("0"))
					{
						System::IO::StreamReader ^sr2 = gcnew  System::IO::StreamReader(l_pOpenFile->FileName,System::Text::Encoding::GetEncoding("UTF-16"));
						textBox1->Text = sr2->ReadToEnd();
						sr2->Close();
					}					
					sr->Close();
				 }
			 }

private: System::Void menuItem5_Click(System::Object ^  sender, System::EventArgs ^  e)
		 {
			array<String^>^l_pStrFilesName = DNCT::OpenFileAndGetNames();
			const int l_iCharactersCount = 65536;
			std::wstring	l_StringorAll;
			bool	l_bValidGlyphs[l_iCharactersCount];//all 
			String^l_strDefaultString = "1234567890/*-+.qwertyui9o0p-[=]\asdfghjkl;zxcvbnm,./`1234567890-~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:""ZXCVBNM<>?	\n\r\t";
			textBox1->Text = "";
			for each(auto l_strFileName in l_pStrFilesName)
			{
				System::IO::StreamReader ^sr = gcnew  System::IO::StreamReader(l_strFileName);
				textBox1->Text += sr->ReadToEnd();
				if( textBox1->Text->Equals(L" ") ||textBox1->Text->Equals(L"0")||
					textBox1->Text->Equals(" ") ||textBox1->Text->Equals("0"))
				{
					System::IO::StreamReader ^sr2 = gcnew  System::IO::StreamReader(l_strFileName,System::Text::Encoding::GetEncoding("UTF-16"));
					textBox1->Text = sr2->ReadToEnd();
					sr2->Close();
				}
				if( textBox1->Text->Equals(L" ") ||textBox1->Text->Equals(L"0")||
					textBox1->Text->Equals(" ") ||textBox1->Text->Equals("0"))
				{
					System::IO::StreamReader ^sr2 = gcnew  System::IO::StreamReader(l_strFileName,System::Text::Encoding::GetEncoding("UTF-8"));
					textBox1->Text = sr2->ReadToEnd();
					sr2->Close();
				}
				System::String^l_pString = textBox1->Text+l_strDefaultString;
				int l_i = l_pString->Length;
				UINT	l_uiNumGlyphs = 0;
				ZeroMemory( l_bValidGlyphs, l_iCharactersCount );
				bool	l_bCharacterOutOfRange = false;
				for( int i=0; i<l_i; ++i )//filled
				{
					if( !l_bValidGlyphs[l_pString[i]] )
					{
						if( l_pString[i]>=l_iCharactersCount )
						{
							if( !l_bCharacterOutOfRange )
							{
								WARNING_MSG("total character is bigger than 65536 call fatming to fix this problem");
								l_bCharacterOutOfRange = true;
							}
						}
						l_bValidGlyphs[l_pString[i]] = true;
						++l_uiNumGlyphs;
					}
				}
				sr->Close();
			}
			for( int i=0;i<l_iCharactersCount;++i )
			{
				if( l_bValidGlyphs[i] )
				{
					l_StringorAll.push_back((WCHAR)i);
				}
			}
			textBox1->Text =  TO_GCSTRING(l_StringorAll.c_str());
		 }

private: System::Void menuItem6_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::String^l_pString = DNCT::SaveFileAndGetName(0);
			 if(l_pString->Length)
			 {
				System::IO::FileStream ^sb = gcnew System::IO::FileStream(l_pString, FileMode::Create);
				System::IO::StreamWriter^	l_pStringWriter = gcnew System::IO::StreamWriter(sb,System::Text::Encoding::GetEncoding(TO_GCSTRING("UTF-16"))  );
				l_pStringWriter->Write(textBox1->Text);
				l_pStringWriter->Close();
			 }
		 }
private: System::Void Form1_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
			 textBox1->Size = System::Drawing::Size(this->Width - 50,this->Height - 70);
		 }
private: System::Void menuItem7_Click(System::Object^  sender, System::EventArgs^  e) {
			 TextEditor^ l_p = gcnew TextEditor(m_pStringForExeFilePath);
			 l_p->Show();
		 }
private: System::Void menuItem8_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::String^l_pDifferentString = gcnew System::String("Open file have no those characters in textbox     ");
			 String^l_pString = DNCT::OpenFileGetText();
			 if(l_pString->Equals(this->textBox1->Text->ToString()))
				 WARNING_MSG("same");
			 else
			 {
				 for( int i=0;i<textBox1->Text->Length;i++ )
				 {
					 if( !l_pString->Contains(textBox1->Text[i].ToString()) )
						l_pDifferentString+=textBox1->Text[i];
				 }
				 l_pDifferentString = l_pDifferentString+DNCT::GetChanglineString()+"textbox have no those characters in Open file    ";
				 for( int i=0;i<l_pString->Length;i++ )
				 {
					 if( !textBox1->Text->Contains(l_pString[i].ToString()) )
						l_pDifferentString+=l_pString[i];
				 }

				 WARNING_MSG(l_pDifferentString);
			 }
		 }
};
}

