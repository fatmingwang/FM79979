#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace XMLDataEditor {

	/// <summary>
	/// Summary for EncryptStringConvert
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class EncryptStringConvert : public System::Windows::Forms::Form
	{
	public:
		EncryptStringConvert(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			InputData_textBox->Text = "C:\\Documents and Settings\\Administrator";
			m_cSplitCharacter = ',';
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EncryptStringConvert()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  InputData_textBox;
	private: System::Windows::Forms::TextBox^  Result_textBox;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  Encode_button;
	private: System::Windows::Forms::Button^  Decode_button;
	//my
	 char	m_cSplitCharacter;

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
			this->InputData_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Result_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->Encode_button = (gcnew System::Windows::Forms::Button());
			this->Decode_button = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// InputData_textBox
			// 
			this->InputData_textBox->Location = System::Drawing::Point(27, 34);
			this->InputData_textBox->Multiline = true;
			this->InputData_textBox->Name = L"InputData_textBox";
			this->InputData_textBox->Size = System::Drawing::Size(451, 140);
			this->InputData_textBox->TabIndex = 0;
			// 
			// Result_textBox
			// 
			this->Result_textBox->Location = System::Drawing::Point(27, 224);
			this->Result_textBox->Multiline = true;
			this->Result_textBox->Name = L"Result_textBox";
			this->Result_textBox->Size = System::Drawing::Size(451, 140);
			this->Result_textBox->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(44, 19);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(54, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Input Data";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(25, 201);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(34, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Result";
			// 
			// Encode_button
			// 
			this->Encode_button->Location = System::Drawing::Point(538, 76);
			this->Encode_button->Name = L"Encode_button";
			this->Encode_button->Size = System::Drawing::Size(65, 28);
			this->Encode_button->TabIndex = 4;
			this->Encode_button->Text = L"Encode";
			this->Encode_button->UseVisualStyleBackColor = true;
			this->Encode_button->Click += gcnew System::EventHandler(this, &EncryptStringConvert::Encode_button_Click);
			// 
			// Decode_button
			// 
			this->Decode_button->Location = System::Drawing::Point(538, 201);
			this->Decode_button->Name = L"Decode_button";
			this->Decode_button->Size = System::Drawing::Size(65, 28);
			this->Decode_button->TabIndex = 5;
			this->Decode_button->Text = L"Decode";
			this->Decode_button->UseVisualStyleBackColor = true;
			this->Decode_button->Click += gcnew System::EventHandler(this, &EncryptStringConvert::Decode_button_Click);
			// 
			// EncryptStringConvert
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(722, 376);
			this->Controls->Add(this->Decode_button);
			this->Controls->Add(this->Encode_button);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Result_textBox);
			this->Controls->Add(this->InputData_textBox);
			this->Name = L"EncryptStringConvert";
			this->Text = L"EncryptStringConvert";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void Encode_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				std::string	l_str = DNCT::GcStringToChar(InputData_textBox->Text);

				std::wstring	lll = EncodeStringToValue(DNCT::GcStringToWchar(InputData_textBox->Text),L",");
				Result_textBox->Text = String(lll.c_str()).ToString();
				return ;
				unsigned char *pDes;
				int nDesLen;
				if( CompressHuffman((unsigned char*)l_str.c_str(),l_str.length(), pDes, nDesLen) )
				{
					Result_textBox->Text = "";
					char l_strSplit[2] = {m_cSplitCharacter,0};
					for( int i=0;i<nDesLen;++i )
					{
						Result_textBox->Text += pDes[i].ToString();
						if( i != nDesLen-1 )
						{
							Result_textBox->Text += String(l_strSplit).ToString();
						}
					}
					delete pDes;
				}
				else
				{
					WARNING_MSG("compress failed!");
				}
			 }


private: System::Void Decode_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 std::wstring	llll = DecodeStringFromeValue(DNCT::GcStringToWchar(InputData_textBox->Text),L",");
			 Result_textBox->Text = String(llll.c_str()).ToString();
			 return;
			 
				cli::array<String^>^l_strSplitString = InputData_textBox->Text->Split(m_cSplitCharacter);
				unsigned char	l_strtemp[79979];
				int	l_iCount = 0;
				for each(System::String^l_strValue in l_strSplitString)
				{
					l_strtemp[l_iCount] = (unsigned char)System::Int16::Parse(l_strValue);
					++l_iCount;
				}
				l_strtemp[l_iCount] = 0;
				unsigned char *pDes;
				int nDesLen;
				if(DecompressHuffman(l_strtemp, l_iCount, pDes, nDesLen))
				{
					char*l_strResult = new char[nDesLen+1];
					memcpy(l_strResult,pDes,nDesLen);
					l_strResult[nDesLen] = 0;
					Result_textBox->Text = gcnew String(l_strResult);
					delete l_strResult;
					delete pDes;
				}
				else
				{
					WARNING_MSG("decompress failed!");
				}
		 }
};
}
