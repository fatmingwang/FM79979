#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm {

	/// <summary>
	/// PrtColorActBlendingBy2Color 的摘要
	/// </summary>
	public ref class PrtColorActBlendingBy2Color : public System::Windows::Forms::UserControl
	{
	public:
		PrtColorActBlendingBy2Color(float*e_pfR1,
									float*e_pfG1,
									float*e_pfB1,
									float*e_pfA1,
									float*e_pfR2,
									float*e_pfG2,
									float*e_pfB2,
									float*e_pfA2)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_pfR1 = e_pfR1;
			m_pfG1 = e_pfG1;
			m_pfB1 = e_pfB1;
			m_pfA1 = e_pfA1;
			m_pfR2 = e_pfR2;
			m_pfG2 = e_pfG2;
			m_pfB2 = e_pfB2;
			m_pfA2 = e_pfA2;
			R1_numericUpDown->Value = System::Decimal(*m_pfR1);
			G1_numericUpDown->Value = System::Decimal(*m_pfG1);
			B1_numericUpDown->Value = System::Decimal(*m_pfB1);
			A1_numericUpDown->Value = System::Decimal(*m_pfA1);
			R2_numericUpDown->Value = System::Decimal(*m_pfR2);
			G2_numericUpDown->Value = System::Decimal(*m_pfG2);
			B2_numericUpDown->Value = System::Decimal(*m_pfB2);
			A2_numericUpDown->Value = System::Decimal(*m_pfA2);
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~PrtColorActBlendingBy2Color()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::NumericUpDown^  B2_numericUpDown;
	protected: 

	private: System::Windows::Forms::NumericUpDown^  G2_numericUpDown;

	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::NumericUpDown^  R2_numericUpDown;

	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::NumericUpDown^  A1_numericUpDown;

	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::NumericUpDown^  B1_numericUpDown;

	private: System::Windows::Forms::NumericUpDown^  G1_numericUpDown;


	private: System::Windows::Forms::NumericUpDown^  R1_numericUpDown;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::NumericUpDown^  A2_numericUpDown;

	private: System::Windows::Forms::Label^  label4;
	//my
	public:	float*m_pfR1;
	public:	float*m_pfG1;
	public:	float*m_pfB1;
	public:	float*m_pfA1;
	public:	float*m_pfR2;
	public:	float*m_pfG2;
	public:	float*m_pfB2;
	public:	float*m_pfA2;
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
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->B1_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->G1_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->R1_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->A1_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->B2_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->A2_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->R2_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->G2_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->B1_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->G1_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->R1_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->A1_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->B2_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->A2_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->R2_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->G2_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				34.375F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				65.625F)));
			this->tableLayoutPanel1->Controls->Add(this->B1_numericUpDown, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->G1_numericUpDown, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->R1_numericUpDown, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->label3, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->label2, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->label4, 0, 4);
			this->tableLayoutPanel1->Controls->Add(this->A1_numericUpDown, 1, 3);
			this->tableLayoutPanel1->Controls->Add(this->label5, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->B2_numericUpDown, 1, 6);
			this->tableLayoutPanel1->Controls->Add(this->A2_numericUpDown, 1, 7);
			this->tableLayoutPanel1->Controls->Add(this->label8, 0, 7);
			this->tableLayoutPanel1->Controls->Add(this->R2_numericUpDown, 1, 4);
			this->tableLayoutPanel1->Controls->Add(this->G2_numericUpDown, 1, 5);
			this->tableLayoutPanel1->Controls->Add(this->label6, 0, 5);
			this->tableLayoutPanel1->Controls->Add(this->label7, 0, 6);
			this->tableLayoutPanel1->Location = System::Drawing::Point(2, 2);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 8;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(119, 170);
			this->tableLayoutPanel1->TabIndex = 4;
			// 
			// B1_numericUpDown
			// 
			this->B1_numericUpDown->BackColor = System::Drawing::SystemColors::MenuBar;
			this->B1_numericUpDown->DecimalPlaces = 5;
			this->B1_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->B1_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->B1_numericUpDown->Location = System::Drawing::Point(40, 42);
			this->B1_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->B1_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->B1_numericUpDown->Name = L"B1_numericUpDown";
			this->B1_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->B1_numericUpDown->TabIndex = 2;
			this->B1_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingBy2Color::B1_numericUpDown_ValueChanged);
			// 
			// G1_numericUpDown
			// 
			this->G1_numericUpDown->BackColor = System::Drawing::SystemColors::MenuBar;
			this->G1_numericUpDown->DecimalPlaces = 5;
			this->G1_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->G1_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->G1_numericUpDown->Location = System::Drawing::Point(40, 21);
			this->G1_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->G1_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->G1_numericUpDown->Name = L"G1_numericUpDown";
			this->G1_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->G1_numericUpDown->TabIndex = 1;
			this->G1_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingBy2Color::G1_numericUpDown_ValueChanged);
			// 
			// R1_numericUpDown
			// 
			this->R1_numericUpDown->BackColor = System::Drawing::SystemColors::MenuBar;
			this->R1_numericUpDown->DecimalPlaces = 5;
			this->R1_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->R1_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->R1_numericUpDown->Location = System::Drawing::Point(40, 0);
			this->R1_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->R1_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->R1_numericUpDown->Name = L"R1_numericUpDown";
			this->R1_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->R1_numericUpDown->TabIndex = 0;
			this->R1_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingBy2Color::R1_numericUpDown_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(2, 0);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(30, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Red1";
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
			this->label3->Text = L"Green1";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(2, 42);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(33, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Blue1";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(2, 84);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(30, 12);
			this->label4->TabIndex = 14;
			this->label4->Text = L"Red2";
			// 
			// A1_numericUpDown
			// 
			this->A1_numericUpDown->BackColor = System::Drawing::SystemColors::MenuBar;
			this->A1_numericUpDown->DecimalPlaces = 5;
			this->A1_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->A1_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->A1_numericUpDown->Location = System::Drawing::Point(40, 63);
			this->A1_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->A1_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->A1_numericUpDown->Name = L"A1_numericUpDown";
			this->A1_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->A1_numericUpDown->TabIndex = 8;
			this->A1_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingBy2Color::A1_numericUpDown_ValueChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(2, 63);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(33, 21);
			this->label5->TabIndex = 7;
			this->label5->Text = L"Alpha1";
			// 
			// B2_numericUpDown
			// 
			this->B2_numericUpDown->BackColor = System::Drawing::SystemColors::MenuBar;
			this->B2_numericUpDown->DecimalPlaces = 5;
			this->B2_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->B2_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->B2_numericUpDown->Location = System::Drawing::Point(40, 126);
			this->B2_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->B2_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->B2_numericUpDown->Name = L"B2_numericUpDown";
			this->B2_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->B2_numericUpDown->TabIndex = 15;
			this->B2_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingBy2Color::B2_numericUpDown_ValueChanged);
			// 
			// A2_numericUpDown
			// 
			this->A2_numericUpDown->BackColor = System::Drawing::SystemColors::MenuBar;
			this->A2_numericUpDown->DecimalPlaces = 5;
			this->A2_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->A2_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->A2_numericUpDown->Location = System::Drawing::Point(40, 147);
			this->A2_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->A2_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->A2_numericUpDown->Name = L"A2_numericUpDown";
			this->A2_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->A2_numericUpDown->TabIndex = 6;
			this->A2_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingBy2Color::A2_numericUpDown_ValueChanged);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->ForeColor = System::Drawing::Color::White;
			this->label8->Location = System::Drawing::Point(2, 147);
			this->label8->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(33, 23);
			this->label8->TabIndex = 13;
			this->label8->Text = L"Alpha2";
			// 
			// R2_numericUpDown
			// 
			this->R2_numericUpDown->BackColor = System::Drawing::SystemColors::MenuBar;
			this->R2_numericUpDown->DecimalPlaces = 5;
			this->R2_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->R2_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->R2_numericUpDown->Location = System::Drawing::Point(40, 84);
			this->R2_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->R2_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->R2_numericUpDown->Name = L"R2_numericUpDown";
			this->R2_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->R2_numericUpDown->TabIndex = 10;
			this->R2_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingBy2Color::R2_numericUpDown_ValueChanged);
			// 
			// G2_numericUpDown
			// 
			this->G2_numericUpDown->BackColor = System::Drawing::SystemColors::MenuBar;
			this->G2_numericUpDown->DecimalPlaces = 5;
			this->G2_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->G2_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->G2_numericUpDown->Location = System::Drawing::Point(40, 105);
			this->G2_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->G2_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->G2_numericUpDown->Name = L"G2_numericUpDown";
			this->G2_numericUpDown->Size = System::Drawing::Size(79, 22);
			this->G2_numericUpDown->TabIndex = 12;
			this->G2_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorActBlendingBy2Color::G2_numericUpDown_ValueChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(2, 105);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(33, 21);
			this->label6->TabIndex = 9;
			this->label6->Text = L"Green2";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->ForeColor = System::Drawing::Color::White;
			this->label7->Location = System::Drawing::Point(2, 126);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(33, 12);
			this->label7->TabIndex = 11;
			this->label7->Text = L"Blue2";
			// 
			// PrtColorActBlendingBy2Color
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlDark;
			this->Controls->Add(this->tableLayoutPanel1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"PrtColorActBlendingBy2Color";
			this->Size = System::Drawing::Size(124, 166);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->B1_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->G1_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->R1_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->A1_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->B2_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->A2_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->R2_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->G2_numericUpDown))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void R1_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			*m_pfR1 = (float)R1_numericUpDown->Value;

		 }
private: System::Void G1_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			*m_pfG1 = (float)G1_numericUpDown->Value;

		 }
private: System::Void B1_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pfB1 = (float)B1_numericUpDown->Value;
			
		 }
private: System::Void A1_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			*m_pfA1 = (float)A1_numericUpDown->Value;
			
		 }
private: System::Void R2_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			*m_pfR2 = (float)R2_numericUpDown->Value;
			
		 }
private: System::Void G2_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			*m_pfG2 = (float)G2_numericUpDown->Value;
			
		 }
private: System::Void B2_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			*m_pfB2 = (float)B2_numericUpDown->Value;
			
		 }
private: System::Void A2_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			*m_pfA2 = (float)A2_numericUpDown->Value;
		 }
};
}
