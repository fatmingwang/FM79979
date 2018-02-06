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
	/// Summary for XYZNumeriaclControl
	/// </summary>
	[Designer("System.Windows.Forms.Design.ParentControlDesigner, System.Design", IDesigner::typeid)] 
	public ref class XYZNumeriaclControl : public System::Windows::Forms::UserControl
	{
	public:
		XYZNumeriaclControl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pfX = 0;
			m_pfY = 0;
			m_pfZ = 0;
			m_bToRadius = false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~XYZNumeriaclControl()
		{
			if (components)
			{
				delete components;
			}
		}





	//public: System::Windows::Forms::NumericUpDown^  X_numericUpDown;
	//public: System::Windows::Forms::NumericUpDown^  Y_numericUpDown;
	//public: System::Windows::Forms::NumericUpDown^  Z_numericUpDown;
	//my
	public:	bool	m_bValueChanged;//if value is changed m_bValueChanged will be true but u have to set m_bValueChanged as false again if u need to use this
	public:	float*m_pfX;
	public:	float*m_pfY;
	public:	float*m_pfZ;
	PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,X_numericUpDown,MyNumericUpDown1,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");
	PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,Y_numericUpDown,MyNumericUpDown2,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");
	PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::NumericUpDown^,Z_numericUpDown,MyNumericUpDown3,"My",System::Windows::Forms::NumericUpDown,"Fatming qoo");
	//public: System::Windows::Forms::NumericUpDown^  Z_numericUpDown;
	//public: System::Windows::Forms::NumericUpDown^  Y_numericUpDown;
	//public: System::Windows::Forms::NumericUpDown^  X_numericUpDown;
			//if true the angle would convert to radian,but the value of NumericUpDown will show degree
	public:	bool m_bToRadius;
	public:void	SetValue(void*e_pData)
		   {
			   float*l_pValue = (float*)e_pData;
			   SetValue(&l_pValue[0],&l_pValue[1],&l_pValue[2]);
		   }
	public:void	SetValue(float*e_fX,float*e_fY,float*e_fZ)
		   {
			   if( m_bToRadius )
			   {
				   X_numericUpDown->DecimalPlaces = 1;
				   Y_numericUpDown->DecimalPlaces = 1;
				   Z_numericUpDown->DecimalPlaces = 1;
				   X_numericUpDown->Increment = 1;
				   Y_numericUpDown->Increment = 1;
				   Z_numericUpDown->Increment = 1;
			   }
				m_pfX = e_fX;
				m_pfY = e_fY;
				m_pfZ = e_fZ;
				AssignValueToUI();
		   }
	public:void	AssignValueToUI()
		   {
				if( m_pfX&&m_pfY&&m_pfZ )
				{
					if( m_bToRadius )
					{
						X_numericUpDown->Value = System::Decimal(RadianToDegree(*m_pfX));
						Y_numericUpDown->Value = System::Decimal(RadianToDegree(*m_pfY));
						Z_numericUpDown->Value = System::Decimal(RadianToDegree(*m_pfZ));
					}
					else
					{
						try
						{
							X_numericUpDown->Value = System::Decimal(*m_pfX);
							Y_numericUpDown->Value = System::Decimal(*m_pfY);
							Z_numericUpDown->Value = System::Decimal(*m_pfZ);
						}
						catch (System::Exception^e)
						{
							WARNING_MSG("XYZNumeriaclControl AssignValueToUI value is bigger than max or small than min");
						}
					}
				}
		   }
	public: 
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Z_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Y_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->X_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// Z_numericUpDown
			// 
			this->Z_numericUpDown->DecimalPlaces = 3;
			this->Z_numericUpDown->Dock = System::Windows::Forms::DockStyle::Top;
			this->Z_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->Z_numericUpDown->Location = System::Drawing::Point(0, 50);
			this->Z_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->Z_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->Z_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->Z_numericUpDown->Name = L"Z_numericUpDown";
			this->Z_numericUpDown->Size = System::Drawing::Size(70, 25);
			this->Z_numericUpDown->TabIndex = 4;
			this->Z_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &XYZNumeriaclControl::numericUpDown1_ValueChanged);
			// 
			// Y_numericUpDown
			// 
			this->Y_numericUpDown->DecimalPlaces = 3;
			this->Y_numericUpDown->Dock = System::Windows::Forms::DockStyle::Top;
			this->Y_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->Y_numericUpDown->Location = System::Drawing::Point(0, 25);
			this->Y_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->Y_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->Y_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->Y_numericUpDown->Name = L"Y_numericUpDown";
			this->Y_numericUpDown->Size = System::Drawing::Size(70, 25);
			this->Y_numericUpDown->TabIndex = 5;
			this->Y_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &XYZNumeriaclControl::numericUpDown1_ValueChanged);
			// 
			// X_numericUpDown
			// 
			this->X_numericUpDown->DecimalPlaces = 3;
			this->X_numericUpDown->Dock = System::Windows::Forms::DockStyle::Top;
			this->X_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->X_numericUpDown->Location = System::Drawing::Point(0, 0);
			this->X_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->X_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->X_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->X_numericUpDown->Name = L"X_numericUpDown";
			this->X_numericUpDown->Size = System::Drawing::Size(70, 25);
			this->X_numericUpDown->TabIndex = 3;
			this->X_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &XYZNumeriaclControl::numericUpDown1_ValueChanged);
			// 
			// XYZNumeriaclControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->Controls->Add(this->Z_numericUpDown);
			this->Controls->Add(this->Y_numericUpDown);
			this->Controls->Add(this->X_numericUpDown);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"XYZNumeriaclControl";
			this->Size = System::Drawing::Size(70, 73);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Z_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Y_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->X_numericUpDown))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 m_bValueChanged = true;
				if( sender == X_numericUpDown )
				{
					if(m_pfX)
					{
						if( m_bToRadius )
							*m_pfX = (float)DegreeToRadian((float)X_numericUpDown->Value);
						else
							*m_pfX = (float)X_numericUpDown->Value;
					}
				}
				else
				if( sender == Y_numericUpDown )
				{
					if(m_pfY)
					{
						if( m_bToRadius )
							*m_pfY = (float)DegreeToRadian((float)Y_numericUpDown->Value);
						else
							*m_pfY = (float)Y_numericUpDown->Value;				
					}
				}
				else
				if( sender == Z_numericUpDown )
				{
					if(m_pfZ)
					{
						if( m_bToRadius )
							*m_pfZ = (float)DegreeToRadian((float)Z_numericUpDown->Value);
						else
							*m_pfZ = (float)Z_numericUpDown->Value;
					}
				}
			 }
};
}
