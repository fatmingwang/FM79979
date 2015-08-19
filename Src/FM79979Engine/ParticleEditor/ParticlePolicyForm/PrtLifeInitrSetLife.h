#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm {

	/// <summary>
	/// PrtLifeInitrSetLife 的摘要
	/// </summary>
	public ref class PrtLifeInitrSetLife : public System::Windows::Forms::UserControl
	{
	public:
		PrtLifeInitrSetLife(float *e_pfMinTime,float *e_pfLiveRangeTime,bool *e_pRandom)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_pfMinTime = e_pfMinTime;
			m_pfLiveRangeTime = e_pfLiveRangeTime;
			m_pRandom = e_pRandom;
			checkBox1->Checked = *m_pRandom;
			LifeMinTime_numericUpDown->Value = System::Decimal(*m_pfMinTime);
			LiveRangeTime_numericUpDown->Value = System::Decimal(*m_pfLiveRangeTime);
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~PrtLifeInitrSetLife()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::NumericUpDown^  LiveRangeTime_numericUpDown;
	protected: 


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  LifeMinTime_numericUpDown;

	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::ToolTip^  toolTip1;
	private: System::ComponentModel::IContainer^  components;
	//my
	private: float	*m_pfMinTime;
	private: float	*m_pfLiveRangeTime;
	private: bool	*m_pRandom;
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
			this->LiveRangeTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->LifeMinTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->LiveRangeTime_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->LifeMinTime_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->BackColor = System::Drawing::SystemColors::ControlDark;
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				40)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				60)));
			this->tableLayoutPanel1->Controls->Add(this->LiveRangeTime_numericUpDown, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->label3, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->label2, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->LifeMinTime_numericUpDown, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->checkBox1, 1, 2);
			this->tableLayoutPanel1->Location = System::Drawing::Point(2, 0);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 3;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 16)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(191, 101);
			this->tableLayoutPanel1->TabIndex = 4;
			// 
			// LiveRangeTime_numericUpDown
			// 
			this->LiveRangeTime_numericUpDown->DecimalPlaces = 3;
			this->LiveRangeTime_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->LiveRangeTime_numericUpDown->Location = System::Drawing::Point(76, 33);
			this->LiveRangeTime_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->LiveRangeTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->LiveRangeTime_numericUpDown->Name = L"LiveRangeTime_numericUpDown";
			this->LiveRangeTime_numericUpDown->Size = System::Drawing::Size(80, 22);
			this->LiveRangeTime_numericUpDown->TabIndex = 1;
			this->LiveRangeTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtLifeInitrSetLife::LiveRangeTime_numericUpDown_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(2, 0);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(67, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"LifeMinTime";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(2, 33);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(69, 24);
			this->label3->TabIndex = 4;
			this->label3->Text = L"LiveTimeRange";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(2, 66);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(45, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Random";
			this->toolTip1->SetToolTip(this->label2, L"while random is active,life time would be minLife time add liverange as random");
			// 
			// LifeMinTime_numericUpDown
			// 
			this->LifeMinTime_numericUpDown->DecimalPlaces = 3;
			this->LifeMinTime_numericUpDown->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->LifeMinTime_numericUpDown->Location = System::Drawing::Point(76, 0);
			this->LifeMinTime_numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->LifeMinTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->LifeMinTime_numericUpDown->Name = L"LifeMinTime_numericUpDown";
			this->LifeMinTime_numericUpDown->Size = System::Drawing::Size(80, 22);
			this->LifeMinTime_numericUpDown->TabIndex = 0;
			this->LifeMinTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &PrtLifeInitrSetLife::LifeMinTime_numericUpDown_ValueChanged);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->ForeColor = System::Drawing::Color::White;
			this->checkBox1->Location = System::Drawing::Point(78, 68);
			this->checkBox1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(64, 16);
			this->checkBox1->TabIndex = 5;
			this->checkBox1->Text = L"Random";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &PrtLifeInitrSetLife::checkBox1_CheckedChanged);
			// 
			// PrtLifeInitrSetLife
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->tableLayoutPanel1);
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"PrtLifeInitrSetLife";
			this->Size = System::Drawing::Size(192, 103);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->LiveRangeTime_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->LifeMinTime_numericUpDown))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pRandom = checkBox1->Checked;
		 }
private: System::Void LifeMinTime_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pfMinTime = (float)LifeMinTime_numericUpDown->Value;
		 }
private: System::Void LiveRangeTime_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pfLiveRangeTime = (float)LiveRangeTime_numericUpDown->Value;
		 }
};
}
