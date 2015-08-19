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
	/// Summary for XYNumericalControl
	/// </summary>
	[Designer("System.Windows.Forms.Design.ParentControlDesigner, System.Design", IDesigner::typeid)] 
	public ref class XYNumericalControl : public System::Windows::Forms::UserControl
	{
	public:
		XYNumericalControl(void)
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
		~XYNumericalControl()
		{
			if (components)
			{
				delete components;
			}
		}
	//PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,numericUpDown1,MyNumericUpDown1,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");
	//PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,numericUpDown2,MyNumericUpDown2,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");

	//public: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	//public: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,numericUpDown1,MyNumericUpDown1,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");
	PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,numericUpDown2,MyNumericUpDown2,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");
	private: System::Windows::Forms::Panel^  panel1;
	//my
	public:	bool	m_bValueChanged;//if value is changed m_bValueChanged will be true but u have to set m_bValueChanged as false again if u need to use this
	public:float*m_pValue1;
	public:float*m_pValue2;
	public:void	SetValue(float*e_fX,float*e_fY)
		   {
				m_pValue1 = e_fX;
				m_pValue2 = e_fY;
				AssignValueToUI();
		   }
	public:void	AssignValueToUI()
		   {
				if( m_pValue1 )
				{
					numericUpDown1->Value = System::Decimal(*m_pValue1);
					numericUpDown2->Value = System::Decimal(*m_pValue2);
				}
		   }
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
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->DecimalPlaces = 3;
			this->numericUpDown2->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numericUpDown2->Location = System::Drawing::Point(0, 27);
			this->numericUpDown2->Margin = System::Windows::Forms::Padding(0);
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(120, 22);
			this->numericUpDown2->TabIndex = 1;
			this->numericUpDown2->ValueChanged += gcnew System::EventHandler(this, &XYNumericalControl::numericUpDown1_ValueChanged);
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->DecimalPlaces = 3;
			this->numericUpDown1->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->numericUpDown1->Location = System::Drawing::Point(0, 0);
			this->numericUpDown1->Margin = System::Windows::Forms::Padding(0);
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(120, 22);
			this->numericUpDown1->TabIndex = 2;
			this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &XYNumericalControl::numericUpDown1_ValueChanged);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->numericUpDown1);
			this->panel1->Controls->Add(this->numericUpDown2);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Right;
			this->panel1->Location = System::Drawing::Point(3, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(120, 49);
			this->panel1->TabIndex = 3;
			// 
			// XYNumericalControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->Controls->Add(this->panel1);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"XYNumericalControl";
			this->Size = System::Drawing::Size(123, 49);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			this->panel1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				if( m_pValue1 )
					*m_pValue1 = (float)numericUpDown1->Value;
				if( m_pValue2 )
					*m_pValue2 = (float)numericUpDown2->Value;
				m_bValueChanged = true;
			 }
	};
}
