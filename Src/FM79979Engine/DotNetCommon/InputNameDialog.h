#pragma once
#include <stdio.h>
#include <vector>
#include "DotNetCommonTools.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace DNCT 
{

	/// <summary>
	/// Summary for InputNameDialog
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class InputNameDialog : public System::Windows::Forms::Form
	{
	public:
		InputNameDialog(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			//m_strTextName = gcnew String("");
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~InputNameDialog()
		{
			if (components)
			{
				delete components;
			}
			delete m_strTextName;
		}
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  Ok_button;
	private: System::Windows::Forms::Button^  Chancel_button;	
	public:  String^	m_strTextName;
	public:	 System::Void	SetupTextBoxData(String^e_strName)
			{
				if( e_strName )
				{
					textBox1->Text = e_strName;
				}
			}
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
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->Ok_button = (gcnew System::Windows::Forms::Button());
			this->Chancel_button = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(110, 41);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(209, 22);
			this->textBox1->TabIndex = 0;
			this->textBox1->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &InputNameDialog::textBox1_KeyUp);
			// 
			// Ok_button
			// 
			this->Ok_button->Location = System::Drawing::Point(110, 77);
			this->Ok_button->Name = L"Ok_button";
			this->Ok_button->Size = System::Drawing::Size(84, 22);
			this->Ok_button->TabIndex = 1;
			this->Ok_button->Text = L"Ok";
			this->Ok_button->UseVisualStyleBackColor = true;
			this->Ok_button->Click += gcnew System::EventHandler(this, &InputNameDialog::Ok_button_Click);
			// 
			// Chancel_button
			// 
			this->Chancel_button->Location = System::Drawing::Point(235, 77);
			this->Chancel_button->Name = L"Chancel_button";
			this->Chancel_button->Size = System::Drawing::Size(84, 22);
			this->Chancel_button->TabIndex = 2;
			this->Chancel_button->Text = L"Chancel";
			this->Chancel_button->UseVisualStyleBackColor = true;
			this->Chancel_button->Click += gcnew System::EventHandler(this, &InputNameDialog::Chancel_button_Click);
			// 
			// InputNameDialog
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(429, 119);
			this->Controls->Add(this->Chancel_button);
			this->Controls->Add(this->Ok_button);
			this->Controls->Add(this->textBox1);
			this->Name = L"InputNameDialog";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"InputNameDialog";
			this->ResumeLayout(false);
			this->PerformLayout();
        }
#pragma endregion
	private: System::Void Ok_button_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 if( !textBox1->Text->Trim()->Length )
				 {
					WARING_YES_NO_TO_YES("would u like to leave without save?")
						this->Close();
					else
						return;
				 }
				 m_strTextName = textBox1->Text->ToString();
				 //const  char*l_Str = m_strTextName->c_str();
				 this->Close();
			 }
	private: System::Void Chancel_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }
	private: System::Void textBox1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				 if( e->KeyValue == 13 )//Enter
				 {
					m_strTextName = textBox1->Text;//DNCT::GcStringToChar(textBox1->Text->ToString());
					this->Close();
				 }
				 else
				 if( e->KeyValue == 27 )//Escape
				 {
					this->Close();
				 }
			 }
	};
}
