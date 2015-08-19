#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#ifndef DegreeToRadian
#define DegreeToRadian(degree) ((degree) * (3.14159265358979323846f / 180.0f))
#endif
#ifndef RadianToDegree
#define RadianToDegree( radian ) ((radian) * (180.0f / 3.14159265358979323846f))
#endif

namespace ParticlePolicyForm {

	/// <summary>
	/// RotateActRotate 的摘要
	/// </summary>
	public ref class RotateActRotate : public System::Windows::Forms::UserControl
	{
	public:
		RotateActRotate(float*	e_pfXPivot,
	float*	e_pfYPivot,
	float*	e_pfZPivot,
	int*	e_piRotateType,
	bool*	e_pbRandom,
	float*	e_fRandomValue,
	bool*e_pbNagtive)
		{
			InitializeComponent();
			//
			xyzNumeriaclControl1->m_bToRadius = true;
			xyzNumeriaclControl1->SetValue(e_pfXPivot,e_pfYPivot,e_pfZPivot);
			m_piRotateType = e_piRotateType;
			m_pbRandom = e_pbRandom;
			m_fRandomValue = e_fRandomValue;
			m_pbNagtive = e_pbNagtive;
			Random_checkBox->Checked = *m_pbRandom;
			numericUpDown1->Value = System::Decimal(*m_fRandomValue);
			RotateType_comboBox->SelectedIndex = *e_piRotateType;
			Negative_checkBox->Checked = *m_pbNagtive;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~RotateActRotate()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	protected: 






	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ComboBox^  RotateType_comboBox;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ToolTip^  toolTip1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::CheckBox^  Random_checkBox;
	private: System::ComponentModel::IContainer^  components;
	//my
	public: int*	m_piRotateType;
	public: bool*	m_pbRandom;
	public: float*	m_fRandomValue;
	private: System::Windows::Forms::CheckBox^  Negative_checkBox;
	private: DotNetComponent::XYZNumeriaclControl^  xyzNumeriaclControl1;
	private: System::Windows::Forms::Label^  label1;
	public: 
	public: bool*	m_pbNagtive;
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
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->RotateType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->Random_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Negative_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->xyzNumeriaclControl1 = (gcnew DotNetComponent::XYZNumeriaclControl());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->BackColor = System::Drawing::SystemColors::ControlDark;
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				35.07692F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				64.92308F)));
			this->tableLayoutPanel1->Controls->Add(this->numericUpDown1, 1, 3);
			this->tableLayoutPanel1->Controls->Add(this->label5, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->label4, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->RotateType_comboBox, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->label7, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->Random_checkBox, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->Negative_checkBox, 1, 4);
			this->tableLayoutPanel1->Controls->Add(this->xyzNumeriaclControl1, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Location = System::Drawing::Point(2, 2);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 5;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 85)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(218, 193);
			this->tableLayoutPanel1->TabIndex = 4;
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->DecimalPlaces = 3;
			this->numericUpDown1->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->numericUpDown1->Location = System::Drawing::Point(76, 139);
			this->numericUpDown1->Margin = System::Windows::Forms::Padding(0);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(94, 22);
			this->numericUpDown1->TabIndex = 12;
			this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &RotateActRotate::numericUpDown1_ValueChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(2, 139);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(72, 12);
			this->label5->TabIndex = 10;
			this->label5->Text = L"RandomValue";
			this->toolTip1->SetToolTip(this->label5, L"more effictive if value is big");
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(2, 85);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(59, 12);
			this->label4->TabIndex = 5;
			this->label4->Text = L"RotateType";
			// 
			// RotateType_comboBox
			// 
			this->RotateType_comboBox->FormattingEnabled = true;
			this->RotateType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"None", L"Self", L"World"});
			this->RotateType_comboBox->Location = System::Drawing::Point(78, 87);
			this->RotateType_comboBox->Margin = System::Windows::Forms::Padding(2);
			this->RotateType_comboBox->Name = L"RotateType_comboBox";
			this->RotateType_comboBox->Size = System::Drawing::Size(92, 20);
			this->RotateType_comboBox->TabIndex = 6;
			this->RotateType_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &RotateActRotate::RotateType_comboBox_SelectedIndexChanged);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->ForeColor = System::Drawing::Color::White;
			this->label7->Location = System::Drawing::Point(2, 112);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(45, 12);
			this->label7->TabIndex = 9;
			this->label7->Text = L"Random";
			this->toolTip1->SetToolTip(this->label7, L"if true it will going to add a random value");
			// 
			// Random_checkBox
			// 
			this->Random_checkBox->AutoSize = true;
			this->Random_checkBox->ForeColor = System::Drawing::Color::White;
			this->Random_checkBox->Location = System::Drawing::Point(78, 114);
			this->Random_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->Random_checkBox->Name = L"Random_checkBox";
			this->Random_checkBox->Size = System::Drawing::Size(64, 16);
			this->Random_checkBox->TabIndex = 11;
			this->Random_checkBox->Text = L"Random";
			this->Random_checkBox->UseVisualStyleBackColor = true;
			this->Random_checkBox->CheckedChanged += gcnew System::EventHandler(this, &RotateActRotate::Random_checkBox_CheckedChanged);
			// 
			// Negative_checkBox
			// 
			this->Negative_checkBox->AutoSize = true;
			this->Negative_checkBox->ForeColor = System::Drawing::Color::White;
			this->Negative_checkBox->Location = System::Drawing::Point(78, 168);
			this->Negative_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->Negative_checkBox->Name = L"Negative_checkBox";
			this->Negative_checkBox->Size = System::Drawing::Size(65, 16);
			this->Negative_checkBox->TabIndex = 14;
			this->Negative_checkBox->Text = L"Negative";
			this->Negative_checkBox->UseVisualStyleBackColor = true;
			this->Negative_checkBox->CheckedChanged += gcnew System::EventHandler(this, &RotateActRotate::Negative_checkBox_CheckedChanged);
			// 
			// xyzNumeriaclControl1
			// 
			this->xyzNumeriaclControl1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->xyzNumeriaclControl1->Location = System::Drawing::Point(76, 0);
			this->xyzNumeriaclControl1->Margin = System::Windows::Forms::Padding(0);
			this->xyzNumeriaclControl1->Name = L"xyzNumeriaclControl1";
			this->xyzNumeriaclControl1->Size = System::Drawing::Size(127, 83);
			this->xyzNumeriaclControl1->TabIndex = 15;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(2, 0);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(11, 60);
			this->label1->TabIndex = 16;
			this->label1->Text = L"x\r\n\r\ny\r\n\r\nz";
			// 
			// RotateActRotate
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->tableLayoutPanel1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"RotateActRotate";
			this->Size = System::Drawing::Size(210, 196);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void RotateType_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(RotateType_comboBox->SelectedIndex!=-1)
				*m_piRotateType = RotateType_comboBox->SelectedIndex;
		 }
private: System::Void Random_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			*m_pbRandom = Random_checkBox->Checked;
		 }
private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_fRandomValue = (float)numericUpDown1->Value;
		 }
private: System::Void Negative_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pbNagtive = Negative_checkBox->Checked;
		 }
};
}
