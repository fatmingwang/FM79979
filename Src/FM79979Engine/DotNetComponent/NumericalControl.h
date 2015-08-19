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
	/// Summary for NumericalControl
	/// </summary>
	[Designer("System.Windows.Forms.Design.ParentControlDesigner, System.Design", IDesigner::typeid)] 
	public ref class NumericalControl : public System::Windows::Forms::UserControl
	{
	public:
		NumericalControl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pValue = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~NumericalControl()
		{
			if (components)
			{
				delete components;
			}
		}
    PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,numericUpDown,MyNumericUpDown,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");
	//public: System::Windows::Forms::NumericUpDown^  numericUpDown;
	public:float*	m_pValue;
		   //if true it will convert degree to radius
	public:bool*	m_bRadius;
	public:void	SetValue(float*e_fX)
		   {
				m_pValue = e_fX;
				AssignValueToUI();
		   }
	public:void	AssignValueToUI()
		   {
				if( m_pValue )
				{
					if( m_bRadius )
						numericUpDown->Value = System::Decimal(RadianToDegree(*m_pValue));
					else
						numericUpDown->Value = System::Decimal(*m_pValue);
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
			this->numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// numericUpDown
			// 
			this->numericUpDown->DecimalPlaces = 3;
			this->numericUpDown->Dock = System::Windows::Forms::DockStyle::Right;
			this->numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numericUpDown->Location = System::Drawing::Point(51, 0);
			this->numericUpDown->Margin = System::Windows::Forms::Padding(0);
			this->numericUpDown->Name = L"numericUpDown";
			this->numericUpDown->Size = System::Drawing::Size(121, 22);
			this->numericUpDown->TabIndex = 0;
			this->numericUpDown->ValueChanged += gcnew System::EventHandler(this, &NumericalControl::numericUpDown_ValueChanged);
			// 
			// NumericalControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->Controls->Add(this->numericUpDown);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"NumericalControl";
			this->Size = System::Drawing::Size(172, 19);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				if( m_pValue )
				{
					if( m_bRadius )
						*m_pValue = (float)DegreeToRadian((float)numericUpDown->Value);
					else
						*m_pValue = (float)numericUpDown->Value;
				}
			 }
	};
}
