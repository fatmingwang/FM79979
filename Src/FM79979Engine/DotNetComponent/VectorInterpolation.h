#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace DotNetComponent {

	/// <summary>
	/// Summary for VectorInterpolation
	/// </summary>
	public ref class VectorInterpolation : public System::Windows::Forms::UserControl
	{
	public:
		VectorInterpolation(void)
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
		~VectorInterpolation()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::NumericUpDown^  Z_numericUpDown;
	protected: 
	public: System::Windows::Forms::NumericUpDown^  Y_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  X_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown3;
	private: System::Windows::Forms::ComboBox^  Shape_comboBox;
	public: 

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
			this->Z_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Y_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->X_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->Shape_comboBox = (gcnew System::Windows::Forms::ComboBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Z_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Y_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->X_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->BeginInit();
			this->SuspendLayout();
			// 
			// Z_numericUpDown
			// 
			this->Z_numericUpDown->DecimalPlaces = 3;
			this->Z_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->Z_numericUpDown->Location = System::Drawing::Point(49, 70);
			this->Z_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->Z_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->Z_numericUpDown->Name = L"Z_numericUpDown";
			this->Z_numericUpDown->Size = System::Drawing::Size(120, 22);
			this->Z_numericUpDown->TabIndex = 4;
			// 
			// Y_numericUpDown
			// 
			this->Y_numericUpDown->DecimalPlaces = 3;
			this->Y_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->Y_numericUpDown->Location = System::Drawing::Point(49, 42);
			this->Y_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->Y_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->Y_numericUpDown->Name = L"Y_numericUpDown";
			this->Y_numericUpDown->Size = System::Drawing::Size(120, 22);
			this->Y_numericUpDown->TabIndex = 5;
			// 
			// X_numericUpDown
			// 
			this->X_numericUpDown->DecimalPlaces = 3;
			this->X_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->X_numericUpDown->Location = System::Drawing::Point(49, 14);
			this->X_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->X_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->X_numericUpDown->Name = L"X_numericUpDown";
			this->X_numericUpDown->Size = System::Drawing::Size(120, 22);
			this->X_numericUpDown->TabIndex = 3;
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->DecimalPlaces = 3;
			this->numericUpDown1->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numericUpDown1->Location = System::Drawing::Point(186, 70);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(120, 22);
			this->numericUpDown1->TabIndex = 7;
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->DecimalPlaces = 3;
			this->numericUpDown2->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numericUpDown2->Location = System::Drawing::Point(186, 42);
			this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->numericUpDown2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(120, 22);
			this->numericUpDown2->TabIndex = 8;
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->DecimalPlaces = 3;
			this->numericUpDown3->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numericUpDown3->Location = System::Drawing::Point(186, 14);
			this->numericUpDown3->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->numericUpDown3->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(120, 22);
			this->numericUpDown3->TabIndex = 6;
			// 
			// Shape_comboBox
			// 
			this->Shape_comboBox->FormattingEnabled = true;
			this->Shape_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"Sphere/¶ê§Î"});
			this->Shape_comboBox->Location = System::Drawing::Point(114, 116);
			this->Shape_comboBox->Name = L"Shape_comboBox";
			this->Shape_comboBox->Size = System::Drawing::Size(121, 20);
			this->Shape_comboBox->TabIndex = 9;
			// 
			// VectorInterpolation
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->Shape_comboBox);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->numericUpDown2);
			this->Controls->Add(this->numericUpDown3);
			this->Controls->Add(this->Z_numericUpDown);
			this->Controls->Add(this->Y_numericUpDown);
			this->Controls->Add(this->X_numericUpDown);
			this->Name = L"VectorInterpolation";
			this->Size = System::Drawing::Size(545, 169);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Z_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Y_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->X_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
