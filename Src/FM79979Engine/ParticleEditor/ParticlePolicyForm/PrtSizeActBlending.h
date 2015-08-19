#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm {

	/// <summary>
	/// PrtSizeActBlending 的摘要
	/// </summary>
	public ref class PrtSizeActBlending : public System::Windows::Forms::UserControl
	{
	public:
		PrtSizeActBlending(float*e_pfWidth,
						   float*e_pfHeight,
						   bool*e_pbAdd)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_pfWidth = e_pfWidth;
			m_pfHeight = e_pfHeight;
			m_pbAdd = e_pbAdd;
			Width_numericUpDown->Value = System::Decimal(*m_pfWidth);
			Height_numericUpDown->Value = System::Decimal(*m_pfHeight);
			SizeChangeBehavior_checkBox->Checked = *m_pbAdd;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~PrtSizeActBlending()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	protected: 
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  Height_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  Width_numericUpDown;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::CheckBox^  SizeChangeBehavior_checkBox;
	//my
	public:float* m_pfWidth;
    public:float* m_pfHeight;
    public:bool* m_pbAdd;

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
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->Height_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Width_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SizeChangeBehavior_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->BackColor = System::Drawing::SystemColors::ControlDark;
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				33.125F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				66.875F)));
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->label3, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->Height_numericUpDown, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->Width_numericUpDown, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->label2, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->SizeChangeBehavior_checkBox, 1, 2);
			this->tableLayoutPanel1->Location = System::Drawing::Point(2, 2);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 3;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(118, 65);
			this->tableLayoutPanel1->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(2, 0);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(34, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Width";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(2, 21);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(33, 21);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Height";
			// 
			// Height_numericUpDown
			// 
			this->Height_numericUpDown->DecimalPlaces = 3;
			this->Height_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->Height_numericUpDown->Location = System::Drawing::Point(39, 21);
			this->Height_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->Height_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->Height_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->Height_numericUpDown->Name = L"Height_numericUpDown";
			this->Height_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->Height_numericUpDown->TabIndex = 1;
			this->Height_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtSizeActBlending::Height_numericUpDown_ValueChanged);
			// 
			// Width_numericUpDown
			// 
			this->Width_numericUpDown->DecimalPlaces = 3;
			this->Width_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->Width_numericUpDown->Location = System::Drawing::Point(39, 0);
			this->Width_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->Width_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->Width_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->Width_numericUpDown->Name = L"Width_numericUpDown";
			this->Width_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->Width_numericUpDown->TabIndex = 0;
			this->Width_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtSizeActBlending::Width_numericUpDown_ValueChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(2, 42);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(0, 12);
			this->label2->TabIndex = 5;
			// 
			// SizeChangeBehavior_checkBox
			// 
			this->SizeChangeBehavior_checkBox->AutoSize = true;
			this->SizeChangeBehavior_checkBox->ForeColor = System::Drawing::Color::White;
			this->SizeChangeBehavior_checkBox->Location = System::Drawing::Point(41, 44);
			this->SizeChangeBehavior_checkBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->SizeChangeBehavior_checkBox->Name = L"SizeChangeBehavior_checkBox";
			this->SizeChangeBehavior_checkBox->Size = System::Drawing::Size(44, 16);
			this->SizeChangeBehavior_checkBox->TabIndex = 6;
			this->SizeChangeBehavior_checkBox->Text = L"Add";
			this->SizeChangeBehavior_checkBox->UseVisualStyleBackColor = true;
			this->SizeChangeBehavior_checkBox->CheckedChanged += gcnew System::EventHandler(this, &PrtSizeActBlending::SizeChangeBehavior_checkBox_CheckedChanged);
			// 
			// PrtSizeActBlending
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->tableLayoutPanel1);
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"PrtSizeActBlending";
			this->Size = System::Drawing::Size(121, 69);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void SizeChangeBehavior_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 SizeChangeBehavior_checkBox->Checked?SizeChangeBehavior_checkBox->Text="Add":SizeChangeBehavior_checkBox->Text="Minus";
			 *m_pbAdd = SizeChangeBehavior_checkBox->Checked;
		 }
private: System::Void Width_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pfWidth = (float)Width_numericUpDown->Value;
		 }
private: System::Void Height_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pfHeight= (float)Height_numericUpDown->Value;
		 }
};
}
