#pragma once
#include "../../DotNetCommon/DotNetCommonTools.h"
namespace EngineTest {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Trigonometric
	/// </summary>
	public ref class Trigonometric : public System::Windows::Forms::Form
	{
	public:
		Trigonometric(void)
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
		~Trigonometric()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  Result_textBox;
	private: System::Windows::Forms::NumericUpDown^  Start_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  End_numericUpDown;
	protected: 

	protected: 


	private: System::Windows::Forms::Label^  start_label;
	private: System::Windows::Forms::Label^  End_label;


	private: System::Windows::Forms::Button^  sin_button;
	private: System::Windows::Forms::Button^  cos_button;



	private: System::Windows::Forms::Button^  tan_button;
	private: System::Windows::Forms::Button^  atan_button;


	private: System::Windows::Forms::Button^  button5;

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
			this->Result_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Start_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->End_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->start_label = (gcnew System::Windows::Forms::Label());
			this->End_label = (gcnew System::Windows::Forms::Label());
			this->sin_button = (gcnew System::Windows::Forms::Button());
			this->cos_button = (gcnew System::Windows::Forms::Button());
			this->tan_button = (gcnew System::Windows::Forms::Button());
			this->atan_button = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Start_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->End_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// Result_textBox
			// 
			this->Result_textBox->Location = System::Drawing::Point(276, 12);
			this->Result_textBox->Multiline = true;
			this->Result_textBox->Name = L"Result_textBox";
			this->Result_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->Result_textBox->Size = System::Drawing::Size(850, 610);
			this->Result_textBox->TabIndex = 0;
			// 
			// Start_numericUpDown
			// 
			this->Start_numericUpDown->Location = System::Drawing::Point(81, 211);
			this->Start_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, 0});
			this->Start_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, System::Int32::MinValue});
			this->Start_numericUpDown->Name = L"Start_numericUpDown";
			this->Start_numericUpDown->Size = System::Drawing::Size(130, 20);
			this->Start_numericUpDown->TabIndex = 1;
			// 
			// End_numericUpDown
			// 
			this->End_numericUpDown->Location = System::Drawing::Point(81, 254);
			this->End_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, 0});
			this->End_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999, 0, 0, System::Int32::MinValue});
			this->End_numericUpDown->Name = L"End_numericUpDown";
			this->End_numericUpDown->Size = System::Drawing::Size(130, 20);
			this->End_numericUpDown->TabIndex = 2;
			this->End_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {359, 0, 0, 0});
			// 
			// start_label
			// 
			this->start_label->AutoSize = true;
			this->start_label->Location = System::Drawing::Point(79, 187);
			this->start_label->Name = L"start_label";
			this->start_label->Size = System::Drawing::Size(29, 13);
			this->start_label->TabIndex = 3;
			this->start_label->Text = L"Start";
			// 
			// End_label
			// 
			this->End_label->AutoSize = true;
			this->End_label->Location = System::Drawing::Point(78, 238);
			this->End_label->Name = L"End_label";
			this->End_label->Size = System::Drawing::Size(26, 13);
			this->End_label->TabIndex = 4;
			this->End_label->Text = L"End";
			// 
			// sin_button
			// 
			this->sin_button->Location = System::Drawing::Point(54, 320);
			this->sin_button->Name = L"sin_button";
			this->sin_button->Size = System::Drawing::Size(75, 23);
			this->sin_button->TabIndex = 5;
			this->sin_button->Text = L"sin";
			this->sin_button->UseVisualStyleBackColor = true;
			this->sin_button->Click += gcnew System::EventHandler(this, &Trigonometric::button4_Click);
			// 
			// cos_button
			// 
			this->cos_button->Location = System::Drawing::Point(54, 349);
			this->cos_button->Name = L"cos_button";
			this->cos_button->Size = System::Drawing::Size(75, 23);
			this->cos_button->TabIndex = 6;
			this->cos_button->Text = L"cos";
			this->cos_button->UseVisualStyleBackColor = true;
			this->cos_button->Click += gcnew System::EventHandler(this, &Trigonometric::button4_Click);
			// 
			// tan_button
			// 
			this->tan_button->Location = System::Drawing::Point(56, 378);
			this->tan_button->Name = L"tan_button";
			this->tan_button->Size = System::Drawing::Size(75, 23);
			this->tan_button->TabIndex = 7;
			this->tan_button->Text = L"tan";
			this->tan_button->UseVisualStyleBackColor = true;
			this->tan_button->Click += gcnew System::EventHandler(this, &Trigonometric::button4_Click);
			// 
			// atan_button
			// 
			this->atan_button->Location = System::Drawing::Point(54, 407);
			this->atan_button->Name = L"atan_button";
			this->atan_button->Size = System::Drawing::Size(75, 23);
			this->atan_button->TabIndex = 8;
			this->atan_button->Text = L"atan";
			this->atan_button->UseVisualStyleBackColor = true;
			this->atan_button->Click += gcnew System::EventHandler(this, &Trigonometric::button4_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(54, 436);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->TabIndex = 9;
			this->button5->Text = L"button5";
			this->button5->UseVisualStyleBackColor = true;
			// 
			// Trigonometric
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1269, 729);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->atan_button);
			this->Controls->Add(this->tan_button);
			this->Controls->Add(this->cos_button);
			this->Controls->Add(this->sin_button);
			this->Controls->Add(this->End_label);
			this->Controls->Add(this->start_label);
			this->Controls->Add(this->End_numericUpDown);
			this->Controls->Add(this->Start_numericUpDown);
			this->Controls->Add(this->Result_textBox);
			this->Name = L"Trigonometric";
			this->Text = L"Trigonometric";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Start_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->End_numericUpDown))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 Result_textBox->Text = "";
				 int l_iStart = (int)this->Start_numericUpDown->Value;
				 int l_iEnd = (int)this->End_numericUpDown->Value;
				 char l_strTemp[999];
				 for( int i=l_iStart;i<l_iEnd;++i )
				 {
					//if( (i)%10 == 0 )
					 sprintf(l_strTemp,"%5d",i);
					 Result_textBox->Text  += gcnew String(l_strTemp);
					 float l_fRadius = D3DXToRadian(i);
					 if( sender == sin_button )
					 {
						 sprintf(l_strTemp,"%10.5f",sin(l_fRadius));
					 }
					 else
					 if( sender == cos_button )
					 {
						 sprintf(l_strTemp,"%10.5f",cos(l_fRadius));
					 }
					 else
					 if( sender == tan_button )
					 {
						 sprintf(l_strTemp,"%10.5f",tan(l_fRadius));
					 }
					 if( sender == atan_button )
					 {
						 sprintf(l_strTemp,"%10.5f",atan(l_fRadius));
					 }
					 else
					 {
					 }
					Result_textBox->Text += gcnew String(l_strTemp);
					//if( (i+1)%10 == 0 )
						Result_textBox->Text += DNCT::GetChanglineString();
					//else
						//Result_textBox->Text += ",";

				 }
			 }
};
}
