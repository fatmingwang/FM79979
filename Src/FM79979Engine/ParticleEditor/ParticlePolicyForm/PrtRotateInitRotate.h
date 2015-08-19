#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm {

	/// <summary>
	/// Summary for RotateInitRotate
	/// </summary>
	public ref class RotateInitRotate : public System::Windows::Forms::UserControl
	{
	public:
		RotateInitRotate(float*	e_pfXPivot,	float*	e_pfYPivot,	float*	e_pfZPivot)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			xyzNumeriaclControl1->m_bToRadius = true;
			xyzNumeriaclControl1->X_numericUpDown->Increment = 1;
			xyzNumeriaclControl1->Y_numericUpDown->Increment = 1;
			xyzNumeriaclControl1->Z_numericUpDown->Increment = 1;
			xyzNumeriaclControl1->X_numericUpDown->DecimalPlaces = 1;
			xyzNumeriaclControl1->Y_numericUpDown->DecimalPlaces = 1;
			xyzNumeriaclControl1->Z_numericUpDown->DecimalPlaces = 1;
			xyzNumeriaclControl1->SetValue(e_pfXPivot,e_pfYPivot,e_pfZPivot);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RotateInitRotate()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 

	private: System::Windows::Forms::ToolTip^  toolTip1;
	private: DotNetComponent::XYZNumeriaclControl^  xyzNumeriaclControl1;
	private: System::Windows::Forms::Label^  label1;
	private: System::ComponentModel::IContainer^  components;
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->xyzNumeriaclControl1 = (gcnew DotNetComponent::XYZNumeriaclControl());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// xyzNumeriaclControl1
			// 
			this->xyzNumeriaclControl1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->xyzNumeriaclControl1->Location = System::Drawing::Point(17, 0);
			this->xyzNumeriaclControl1->Margin = System::Windows::Forms::Padding(0);
			this->xyzNumeriaclControl1->Name = L"xyzNumeriaclControl1";
			this->xyzNumeriaclControl1->Size = System::Drawing::Size(127, 83);
			this->xyzNumeriaclControl1->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(11, 84);
			this->label1->TabIndex = 1;
			this->label1->Text = L"x\r\n\r\n\r\ny\r\n\r\n\r\nz";
			// 
			// RotateInitRotate
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->label1);
			this->Controls->Add(this->xyzNumeriaclControl1);
			this->Name = L"RotateInitRotate";
			this->Size = System::Drawing::Size(152, 90);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
