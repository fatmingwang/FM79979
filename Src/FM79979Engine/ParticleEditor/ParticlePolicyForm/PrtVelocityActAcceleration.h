#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm {

	/// <summary>
	/// PrtVelocityActAcceleration 的摘要
	/// </summary>
	public ref class PrtVelocityActAcceleration : public System::Windows::Forms::UserControl
	{
	public:
		PrtVelocityActAcceleration(float*e_pfSpeed)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_pfSpeed = e_pfSpeed;
			Speed_numericUpDown->Value = System::Decimal(*m_pfSpeed);
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~PrtVelocityActAcceleration()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::NumericUpDown^  Speed_numericUpDown;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;

	//my
	public:float*	m_pfSpeed;
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
			this->Speed_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Speed_numericUpDown))->BeginInit();
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
			this->tableLayoutPanel1->Controls->Add(this->Speed_numericUpDown, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->label2, 0, 1);
			this->tableLayoutPanel1->Location = System::Drawing::Point(2, 2);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(142, 50);
			this->tableLayoutPanel1->TabIndex = 3;
			// 
			// Speed_numericUpDown
			// 
			this->Speed_numericUpDown->DecimalPlaces = 3;
			this->Speed_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->Speed_numericUpDown->Location = System::Drawing::Point(47, 0);
			this->Speed_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->Speed_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->Speed_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->Speed_numericUpDown->Name = L"Speed_numericUpDown";
			this->Speed_numericUpDown->Size = System::Drawing::Size(94, 22);
			this->Speed_numericUpDown->TabIndex = 0;
			this->Speed_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtVelocityActAcceleration::X_numericUpDown_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(2, 0);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(33, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Speed";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(2, 25);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(38, 12);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Ristrict";
			// 
			// PrtVelocityActAcceleration
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->tableLayoutPanel1);
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"PrtVelocityActAcceleration";
			this->Size = System::Drawing::Size(146, 54);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Speed_numericUpDown))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void X_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 *m_pfSpeed = (float)Speed_numericUpDown->Value;
		 }
};
}
