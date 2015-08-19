#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm {

	/// <summary>
	/// Summary for PrtVelocityActDircctionChange
	/// </summary>
	public ref class PrtVelocityActDircctionChange : public System::Windows::Forms::UserControl
	{
	public:
		PrtVelocityActDircctionChange(Vector3*e_pvStopTime,Vector3*e_pvNewSpeed,bool *e_pbAcceleration	)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			StopTime_xyzNumeriaclControl->X_numericUpDown->Minimum = 0;
			StopTime_xyzNumeriaclControl->Y_numericUpDown->Minimum = 0;
			StopTime_xyzNumeriaclControl->Z_numericUpDown->Minimum = 0;
			NewDirection_xyzNumeriaclControl->SetValue(e_pvNewSpeed);
			StopTime_xyzNumeriaclControl->SetValue(e_pvStopTime);
			Acceleration_checkBoxControl->SetValue(e_pbAcceleration);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PrtVelocityActDircctionChange()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: DotNetComponent::XYZNumeriaclControl^  NewDirection_xyzNumeriaclControl;
	private: DotNetComponent::XYZNumeriaclControl^  StopTime_xyzNumeriaclControl;
	private: DotNetComponent::CheckBoxControl^  Acceleration_checkBoxControl;


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
			this->StopTime_xyzNumeriaclControl = (gcnew DotNetComponent::XYZNumeriaclControl());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->NewDirection_xyzNumeriaclControl = (gcnew DotNetComponent::XYZNumeriaclControl());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->Acceleration_checkBoxControl = (gcnew DotNetComponent::CheckBoxControl());
			this->SuspendLayout();
			// 
			// StopTime_xyzNumeriaclControl
			// 
			this->StopTime_xyzNumeriaclControl->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->StopTime_xyzNumeriaclControl->Location = System::Drawing::Point(1, 16);
			this->StopTime_xyzNumeriaclControl->Margin = System::Windows::Forms::Padding(0);
			this->StopTime_xyzNumeriaclControl->Name = L"StopTime_xyzNumeriaclControl";
			this->StopTime_xyzNumeriaclControl->Size = System::Drawing::Size(79, 80);
			this->StopTime_xyzNumeriaclControl->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(4, 1);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(63, 15);
			this->label1->TabIndex = 6;
			this->label1->Text = L"StopTime";
			// 
			// NewDirection_xyzNumeriaclControl
			// 
			this->NewDirection_xyzNumeriaclControl->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->NewDirection_xyzNumeriaclControl->Location = System::Drawing::Point(81, 15);
			this->NewDirection_xyzNumeriaclControl->Margin = System::Windows::Forms::Padding(0);
			this->NewDirection_xyzNumeriaclControl->Name = L"NewDirection_xyzNumeriaclControl";
			this->NewDirection_xyzNumeriaclControl->Size = System::Drawing::Size(79, 81);
			this->NewDirection_xyzNumeriaclControl->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(79, 0);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(86, 15);
			this->label2->TabIndex = 7;
			this->label2->Text = L"NewDirection";
			// 
			// Acceleration_checkBoxControl
			// 
			this->Acceleration_checkBoxControl->CheckboxName = L"Acceleration";
			this->Acceleration_checkBoxControl->ForeColor = System::Drawing::Color::White;
			this->Acceleration_checkBoxControl->Location = System::Drawing::Point(0, 101);
			this->Acceleration_checkBoxControl->Margin = System::Windows::Forms::Padding(5, 5, 5, 5);
			this->Acceleration_checkBoxControl->Name = L"Acceleration_checkBoxControl";
			this->Acceleration_checkBoxControl->Size = System::Drawing::Size(148, 26);
			this->Acceleration_checkBoxControl->TabIndex = 2;
			// 
			// PrtVelocityActDircctionChange
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlDark;
			this->Controls->Add(this->label2);
			this->Controls->Add(this->Acceleration_checkBoxControl);
			this->Controls->Add(this->NewDirection_xyzNumeriaclControl);
			this->Controls->Add(this->StopTime_xyzNumeriaclControl);
			this->Controls->Add(this->label1);
			this->ForeColor = System::Drawing::SystemColors::ControlText;
			this->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->Name = L"PrtVelocityActDircctionChange";
			this->Size = System::Drawing::Size(163, 130);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
