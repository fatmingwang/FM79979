#pragma once

namespace XMLDataEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for DataCurve
	/// </summary>
	public ref class DataCurve : public System::Windows::Forms::Form
	{
	public:
		DataCurve(void)
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
		~DataCurve()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  SimpleData_listBox;
	protected: 
	private: System::Windows::Forms::Button^  OpenFile_button;
	private: System::Windows::Forms::TextBox^  FileName_textBox;
	private: System::Windows::Forms::Panel^  panel1;

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
			this->SimpleData_listBox = (gcnew System::Windows::Forms::ListBox());
			this->OpenFile_button = (gcnew System::Windows::Forms::Button());
			this->FileName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->SuspendLayout();
			// 
			// SimpleData_listBox
			// 
			this->SimpleData_listBox->FormattingEnabled = true;
			this->SimpleData_listBox->Location = System::Drawing::Point(12, 112);
			this->SimpleData_listBox->Name = L"SimpleData_listBox";
			this->SimpleData_listBox->Size = System::Drawing::Size(204, 407);
			this->SimpleData_listBox->TabIndex = 0;
			// 
			// OpenFile_button
			// 
			this->OpenFile_button->Location = System::Drawing::Point(12, 83);
			this->OpenFile_button->Name = L"OpenFile_button";
			this->OpenFile_button->Size = System::Drawing::Size(117, 23);
			this->OpenFile_button->TabIndex = 1;
			this->OpenFile_button->Text = L"OpenFile";
			this->OpenFile_button->UseVisualStyleBackColor = true;
			this->OpenFile_button->Click += gcnew System::EventHandler(this, &DataCurve::OpenFile_button_Click);
			// 
			// FileName_textBox
			// 
			this->FileName_textBox->Location = System::Drawing::Point(12, 43);
			this->FileName_textBox->Name = L"FileName_textBox";
			this->FileName_textBox->Size = System::Drawing::Size(204, 20);
			this->FileName_textBox->TabIndex = 2;
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(301, 13);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(1556, 914);
			this->panel1->TabIndex = 3;
			// 
			// DataCurve
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1884, 962);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->FileName_textBox);
			this->Controls->Add(this->OpenFile_button);
			this->Controls->Add(this->SimpleData_listBox);
			this->Name = L"DataCurve";
			this->Text = L"DataCurve";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void OpenFile_button_Click(System::Object^  sender, System::EventArgs^  e) 
			 {

			 }
	};
}
