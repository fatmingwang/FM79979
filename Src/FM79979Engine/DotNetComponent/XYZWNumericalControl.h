#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::ComponentModel::Design;

namespace DotNetComponent {

	/// <summary>
	/// Summary for XYZWNumericalControl
	/// </summary>
	[Designer("System.Windows.Forms.Design.ParentControlDesigner, System.Design", IDesigner::typeid)] 
	public ref class XYZWNumericalControl : public System::Windows::Forms::UserControl
	{
	public:	
		XYZWNumericalControl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pfX = 0;
			m_pfY = 0;
			m_pfZ = 0;
			m_pfW = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~XYZWNumericalControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  panel1;
	PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,X_numericUpDown,MyNumericUpDown1,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");
	PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,Y_numericUpDown,MyNumericUpDown2,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");
	PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,Z_numericUpDown,MyNumericUpDown3,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");
	PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,W_numericUpDown,MyNumericUpDown4,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");
	//private: System::Windows::Forms::NumericUpDown^  Z_numericUpDown;
	//private: System::Windows::Forms::NumericUpDown^  Y_numericUpDown;
	//private: System::Windows::Forms::NumericUpDown^  X_numericUpDown;
	//private: System::Windows::Forms::NumericUpDown^  W_numericUpDown;
	//my
	public:	bool	m_bValueChanged;//if value is changed m_bValueChanged will be true but u have to set m_bValueChanged as false again if u need to use this
	public:	float*m_pfX;
	public:	float*m_pfY;
	public:	float*m_pfZ;
	public:	float*m_pfW;
	public:void	SetValue(void*e_pData)
		   {
			   float*l_pValue = (float*)e_pData;
			   SetValue(&l_pValue[0],&l_pValue[1],&l_pValue[2],&l_pValue[3]);
		   }
	public:void	SetValue(float*e_fX,float*e_fY,float*e_fZ,float*e_fW)
		   {
				m_pfX = e_fX;
				m_pfY = e_fY;
				m_pfZ = e_fZ;
				m_pfW = e_fW;
				AssignValueToUI();
		   }
	public:void	AssignValueToUI()
		   {
				if( m_pfX )
				{
					X_numericUpDown->Value = System::Decimal(*m_pfX);
					Y_numericUpDown->Value = System::Decimal(*m_pfY);
					Z_numericUpDown->Value = System::Decimal(*m_pfZ);
					W_numericUpDown->Value = System::Decimal(*m_pfW);
				}
		   }
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
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->W_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Z_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Y_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->X_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->W_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Z_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Y_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->X_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->W_numericUpDown);
			this->panel1->Controls->Add(this->Z_numericUpDown);
			this->panel1->Controls->Add(this->Y_numericUpDown);
			this->panel1->Controls->Add(this->X_numericUpDown);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Right;
			this->panel1->Location = System::Drawing::Point(4, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(108, 106);
			this->panel1->TabIndex = 4;
			// 
			// W_numericUpDown
			// 
			this->W_numericUpDown->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->W_numericUpDown->DecimalPlaces = 3;
			this->W_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->W_numericUpDown->Location = System::Drawing::Point(7, 79);
			this->W_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->W_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->W_numericUpDown->Name = L"W_numericUpDown";
			this->W_numericUpDown->Size = System::Drawing::Size(94, 22);
			this->W_numericUpDown->TabIndex = 3;
			this->W_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &XYZWNumericalControl::X_numericUpDown_ValueChanged);
			// 
			// Z_numericUpDown
			// 
			this->Z_numericUpDown->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->Z_numericUpDown->DecimalPlaces = 3;
			this->Z_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->Z_numericUpDown->Location = System::Drawing::Point(7, 54);
			this->Z_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->Z_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->Z_numericUpDown->Name = L"Z_numericUpDown";
			this->Z_numericUpDown->Size = System::Drawing::Size(94, 22);
			this->Z_numericUpDown->TabIndex = 1;
			this->Z_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &XYZWNumericalControl::X_numericUpDown_ValueChanged);
			// 
			// Y_numericUpDown
			// 
			this->Y_numericUpDown->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->Y_numericUpDown->DecimalPlaces = 3;
			this->Y_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->Y_numericUpDown->Location = System::Drawing::Point(7, 30);
			this->Y_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->Y_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->Y_numericUpDown->Name = L"Y_numericUpDown";
			this->Y_numericUpDown->Size = System::Drawing::Size(94, 22);
			this->Y_numericUpDown->TabIndex = 2;
			this->Y_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &XYZWNumericalControl::X_numericUpDown_ValueChanged);
			// 
			// X_numericUpDown
			// 
			this->X_numericUpDown->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->X_numericUpDown->DecimalPlaces = 3;
			this->X_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->X_numericUpDown->Location = System::Drawing::Point(7, 4);
			this->X_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->X_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->X_numericUpDown->Name = L"X_numericUpDown";
			this->X_numericUpDown->Size = System::Drawing::Size(94, 22);
			this->X_numericUpDown->TabIndex = 0;
			this->X_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &XYZWNumericalControl::X_numericUpDown_ValueChanged);
			// 
			// XYZWNumericalControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->panel1);
			this->Name = L"XYZWNumericalControl";
			this->Size = System::Drawing::Size(112, 106);
			this->panel1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->W_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Z_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Y_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->X_numericUpDown))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void X_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			if(m_pfX&& sender == X_numericUpDown)
				*m_pfX = (float)X_numericUpDown->Value;
			if(m_pfY&& sender == Y_numericUpDown)
				*m_pfY = (float)Y_numericUpDown->Value;
			if(m_pfZ&& sender == Z_numericUpDown)
				*m_pfZ = (float)Z_numericUpDown->Value;
			if(m_pfW&& sender == W_numericUpDown)
				*m_pfW = (float)W_numericUpDown->Value;
			m_bValueChanged = true;
		 }
};
}
