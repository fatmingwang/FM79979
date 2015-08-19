#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm {

	/// <summary>
	/// PrtColorInitrSetRandomColor 的摘要
	/// </summary>
	public ref class PrtColorInitrSetRandomColor : public System::Windows::Forms::UserControl
	{
	public:
		PrtColorInitrSetRandomColor(float*e_pfR,float*e_pfG,float*e_pfB,float*e_pfA)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_pfR = e_pfR;
			m_pfG = e_pfG;
			m_pfB = e_pfB;
			m_pfA = e_pfA;
			R_numericUpDown->Value = System::Decimal(*m_pfR);
			G_numericUpDown->Value = System::Decimal(*m_pfG);
			B_numericUpDown->Value = System::Decimal(*m_pfB);
			A_numericUpDown->Value = System::Decimal(*m_pfA);
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~PrtColorInitrSetRandomColor()
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


	private: System::Windows::Forms::ToolTip^  toolTip1;
	private: System::ComponentModel::IContainer^  components;
	//my
	public:float*	m_pfR;
	public:float*	m_pfG;
	public:float*	m_pfB;
	public:float*	m_pfA;
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->A_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->B_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->G_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->R_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
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
			this->tableLayoutPanel1->Location = System::Drawing::Point(3, 2);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 5;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 20)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(213, 191);
			this->tableLayoutPanel1->TabIndex = 3;
			// 
			// A_numericUpDown
			// 
			this->A_numericUpDown->DecimalPlaces = 5;
			this->A_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->A_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->A_numericUpDown->Location = System::Drawing::Point(73, 114);
			this->A_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->A_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->A_numericUpDown->Name = L"A_numericUpDown";
			this->A_numericUpDown->Size = System::Drawing::Size(105, 25);
			this->A_numericUpDown->TabIndex = 5;
			this->A_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorInitrSetRandomColor::A_numericUpDown_ValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(3, 114);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 15);
			this->label4->TabIndex = 4;
			this->label4->Text = L"Alpha";
			// 
			// B_numericUpDown
			// 
			this->B_numericUpDown->DecimalPlaces = 5;
			this->B_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->B_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->B_numericUpDown->Location = System::Drawing::Point(73, 76);
			this->B_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->B_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->B_numericUpDown->Name = L"B_numericUpDown";
			this->B_numericUpDown->Size = System::Drawing::Size(105, 25);
			this->B_numericUpDown->TabIndex = 2;
			this->B_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorInitrSetRandomColor::B_numericUpDown_ValueChanged);
			// 
			// G_numericUpDown
			// 
			this->G_numericUpDown->DecimalPlaces = 5;
			this->G_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->G_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->G_numericUpDown->Location = System::Drawing::Point(73, 38);
			this->G_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->G_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->G_numericUpDown->Name = L"G_numericUpDown";
			this->G_numericUpDown->Size = System::Drawing::Size(105, 25);
			this->G_numericUpDown->TabIndex = 1;
			this->G_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorInitrSetRandomColor::G_numericUpDown_ValueChanged);
			// 
			// R_numericUpDown
			// 
			this->R_numericUpDown->DecimalPlaces = 5;
			this->R_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->R_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->R_numericUpDown->Location = System::Drawing::Point(73, 0);
			this->R_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->R_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->R_numericUpDown->Name = L"R_numericUpDown";
			this->R_numericUpDown->Size = System::Drawing::Size(105, 25);
			this->R_numericUpDown->TabIndex = 0;
			this->R_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtColorInitrSetRandomColor::R_numericUpDown_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(3, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(29, 15);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Red";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(3, 38);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 15);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Green";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(3, 76);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(33, 15);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Blue";
			// 
			// PrtColorInitrSetRandomColor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->tableLayoutPanel1);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"PrtColorInitrSetRandomColor";
			this->Size = System::Drawing::Size(184, 156);
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
