#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm {

	/// <summary>
	/// PrtVelocityInitSetVelocity 的摘要
	/// </summary>
	public ref class PrtVelocityInitSetVelocity : public System::Windows::Forms::UserControl
	{
	public:
		PrtVelocityInitSetVelocity(float *e_pfX,float *e_pfY,float *e_pfZ,bool *e_pbRandom,float *e_pRandomOffset)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_pfX = e_pfX;
			m_pfY = e_pfY;
			m_pfZ = e_pfZ;
			m_pbRandom = e_pbRandom;
			m_pRandomOffset = e_pRandomOffset;
			X_numericUpDown->Value = System::Decimal(*m_pfX);
			Y_numericUpDown->Value = System::Decimal(*m_pfY);
			Z_numericUpDown->Value = System::Decimal(*m_pfZ);
			Random_checkBox->Checked = (*m_pbRandom);
			Offset_numericUpDown->Value = System::Decimal(*m_pRandomOffset);
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~PrtVelocityInitSetVelocity()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	protected: 
	private: System::Windows::Forms::NumericUpDown^  Z_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  Y_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  X_numericUpDown;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	//my
	public:float*	m_pfX;
	public:float*	m_pfY;
	public:float*	m_pfZ;
    public:bool *	m_pbRandom;
	public:float*	m_pRandomOffset;
	private: System::Windows::Forms::CheckBox^  Random_checkBox;
	public: 
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  Offset_numericUpDown;

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
			this->Random_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->Z_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Y_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->X_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->Offset_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Z_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Y_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->X_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Offset_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->BackColor = System::Drawing::SystemColors::ControlDark;
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				33.59375F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				66.40625F)));
			this->tableLayoutPanel1->Controls->Add(this->Random_checkBox, 1, 3);
			this->tableLayoutPanel1->Controls->Add(this->label5, 0, 4);
			this->tableLayoutPanel1->Controls->Add(this->Z_numericUpDown, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->Y_numericUpDown, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->X_numericUpDown, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->label3, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->label2, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->label4, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->Offset_numericUpDown, 1, 4);
			this->tableLayoutPanel1->Location = System::Drawing::Point(2, 2);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 5;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 20)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(192, 116);
			this->tableLayoutPanel1->TabIndex = 3;
			// 
			// Random_checkBox
			// 
			this->Random_checkBox->AutoSize = true;
			this->Random_checkBox->ForeColor = System::Drawing::Color::White;
			this->Random_checkBox->Location = System::Drawing::Point(66, 71);
			this->Random_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->Random_checkBox->Name = L"Random_checkBox";
			this->Random_checkBox->Size = System::Drawing::Size(64, 16);
			this->Random_checkBox->TabIndex = 4;
			this->Random_checkBox->Text = L"Random";
			this->Random_checkBox->UseVisualStyleBackColor = true;
			this->Random_checkBox->CheckedChanged += gcnew System::EventHandler(this, &PrtVelocityInitSetVelocity::Random_checkBox_CheckedChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(2, 92);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(33, 12);
			this->label5->TabIndex = 7;
			this->label5->Text = L"Offset";
			// 
			// Z_numericUpDown
			// 
			this->Z_numericUpDown->DecimalPlaces = 3;
			this->Z_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->Z_numericUpDown->Location = System::Drawing::Point(64, 46);
			this->Z_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->Z_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->Z_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->Z_numericUpDown->Name = L"Z_numericUpDown";
			this->Z_numericUpDown->Size = System::Drawing::Size(94, 22);
			this->Z_numericUpDown->TabIndex = 2;
			this->Z_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtVelocityInitSetVelocity::Z_numericUpDown_ValueChanged);
			// 
			// Y_numericUpDown
			// 
			this->Y_numericUpDown->DecimalPlaces = 3;
			this->Y_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->Y_numericUpDown->Location = System::Drawing::Point(64, 23);
			this->Y_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->Y_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->Y_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->Y_numericUpDown->Name = L"Y_numericUpDown";
			this->Y_numericUpDown->Size = System::Drawing::Size(94, 22);
			this->Y_numericUpDown->TabIndex = 1;
			this->Y_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtVelocityInitSetVelocity::Y_numericUpDown_ValueChanged);
			// 
			// X_numericUpDown
			// 
			this->X_numericUpDown->DecimalPlaces = 3;
			this->X_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->X_numericUpDown->Location = System::Drawing::Point(64, 0);
			this->X_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->X_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->X_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->X_numericUpDown->Name = L"X_numericUpDown";
			this->X_numericUpDown->Size = System::Drawing::Size(94, 22);
			this->X_numericUpDown->TabIndex = 0;
			this->X_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtVelocityInitSetVelocity::X_numericUpDown_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(2, 0);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(13, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"X";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(2, 23);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(13, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Y";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(2, 46);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(12, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Z";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(2, 69);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(45, 12);
			this->label4->TabIndex = 5;
			this->label4->Text = L"Random";
			// 
			// Offset_numericUpDown
			// 
			this->Offset_numericUpDown->DecimalPlaces = 5;
			this->Offset_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->Offset_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->Offset_numericUpDown->Location = System::Drawing::Point(64, 92);
			this->Offset_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->Offset_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, 0});
			this->Offset_numericUpDown->Name = L"Offset_numericUpDown";
			this->Offset_numericUpDown->Size = System::Drawing::Size(94, 22);
			this->Offset_numericUpDown->TabIndex = 8;
			this->Offset_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtVelocityInitSetVelocity::Offset_numericUpDown_ValueChanged);
			// 
			// PrtVelocityInitSetVelocity
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->tableLayoutPanel1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"PrtVelocityInitSetVelocity";
			this->Size = System::Drawing::Size(162, 117);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Z_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Y_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->X_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Offset_numericUpDown))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void X_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 *m_pfX = (float)X_numericUpDown->Value;
		 }
private: System::Void Y_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pfY = (float)Y_numericUpDown->Value;
		 }
private: System::Void Z_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pfZ = (float)Z_numericUpDown->Value;
		 }
private: System::Void Random_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			*m_pbRandom = Random_checkBox->Checked;
		 }
private: System::Void Offset_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			*m_pRandomOffset = (float)Offset_numericUpDown->Value;
		 }
};
}
