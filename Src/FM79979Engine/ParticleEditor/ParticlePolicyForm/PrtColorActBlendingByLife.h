#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm {

	/// <summary>
	/// PrtColorActBlendingByLife ���K�n
	/// </summary>
	public ref class PrtColorActBlendingByLife : public System::Windows::Forms::UserControl
	{
	public:
		PrtColorActBlendingByLife(float*e_pfR,float*e_pfG,float*e_pfB,float*e_pfA)
		{
			InitializeComponent();
			m_pfR = e_pfR;
			m_pfG = e_pfG;
			m_pfB = e_pfB;
			m_pfA = e_pfA;
			R_numericUpDown->Value = System::Decimal(*m_pfR);
			G_numericUpDown->Value = System::Decimal(*m_pfG);
			B_numericUpDown->Value = System::Decimal(*m_pfB);
			A_numericUpDown->Value = System::Decimal(*m_pfA);
			//
			//TODO: �b���[�J�غc�禡�{���X
			//
		}

	protected:
		/// <summary>
		/// �M������ϥΤ����귽�C
		/// </summary>
		~PrtColorActBlendingByLife()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	protected: 
	private: System::Windows::Forms::NumericUpDown^  A_numericUpDown;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  B_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  G_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  R_numericUpDown;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	//my
	public:float*	m_pfR;
	public:float*	m_pfG;
	public:float*	m_pfB;
	public:float*	m_pfA;
		/// <summary>
		/// �]�p�u��һݪ��ܼơC
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����]�p�u��䴩�һݪ���k - �ФŨϥε{���X�s�边�ק�o�Ӥ�k�����e�C
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->A_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->B_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->G_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->R_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->A_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->B_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->G_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->R_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->BackColor = System::Drawing::SystemColors::ControlDark;
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				34.375F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				65.625F)));
			this->tableLayoutPanel1->Controls->Add(this->A_numericUpDown, 1, 3);
			this->tableLayoutPanel1->Controls->Add(this->label4, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->B_numericUpDown, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->G_numericUpDown, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->R_numericUpDown, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->label3, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->label2, 0, 2);
			this->tableLayoutPanel1->Location = System::Drawing::Point(2, 2);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 4;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 16)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(120, 101);
			this->tableLayoutPanel1->TabIndex = 3;
			// 
			// A_numericUpDown
			// 
			this->A_numericUpDown->DecimalPlaces = 5;
			this->A_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->A_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->A_numericUpDown->Location = System::Drawing::Point(41, 84);
			this->A_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->A_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->A_numericUpDown->Name = L"A_numericUpDown";
			this->A_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->A_numericUpDown->TabIndex = 5;
			this->A_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingByLife::A_numericUpDown_ValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(2, 84);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(33, 12);
			this->label4->TabIndex = 4;
			this->label4->Text = L"Alpha";
			// 
			// B_numericUpDown
			// 
			this->B_numericUpDown->DecimalPlaces = 5;
			this->B_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->B_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->B_numericUpDown->Location = System::Drawing::Point(41, 56);
			this->B_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->B_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->B_numericUpDown->Name = L"B_numericUpDown";
			this->B_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->B_numericUpDown->TabIndex = 2;
			this->B_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingByLife::B_numericUpDown_ValueChanged);
			// 
			// G_numericUpDown
			// 
			this->G_numericUpDown->DecimalPlaces = 5;
			this->G_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->G_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->G_numericUpDown->Location = System::Drawing::Point(41, 28);
			this->G_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->G_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->G_numericUpDown->Name = L"G_numericUpDown";
			this->G_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->G_numericUpDown->TabIndex = 1;
			this->G_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingByLife::G_numericUpDown_ValueChanged);
			// 
			// R_numericUpDown
			// 
			this->R_numericUpDown->DecimalPlaces = 5;
			this->R_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->R_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->R_numericUpDown->Location = System::Drawing::Point(41, 0);
			this->R_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->R_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->R_numericUpDown->Name = L"R_numericUpDown";
			this->R_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->R_numericUpDown->TabIndex = 0;
			this->R_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingByLife::R_numericUpDown_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(2, 0);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(24, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Red";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(2, 28);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(33, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Green";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(2, 56);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(27, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Blue";
			// 
			// PrtColorActBlendingByLife
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->tableLayoutPanel1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"PrtColorActBlendingByLife";
			this->Size = System::Drawing::Size(123, 106);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->A_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->B_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->G_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->R_numericUpDown))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void R_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pfR = (float)R_numericUpDown->Value;
		 }
private: System::Void G_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pfG = (float)G_numericUpDown->Value;
		 }
private: System::Void B_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pfB = (float)B_numericUpDown->Value;
		 }
private: System::Void A_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pfA = (float)A_numericUpDown->Value;
		 }
	};
}
