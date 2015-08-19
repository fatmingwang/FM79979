#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace TextureEditor 
{

	/// <summary>
	/// Summary for ResolutionDialog
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ResolutionDialog : public System::Windows::Forms::Form
	{
	public:
		ResolutionDialog(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			xyNumericalControl1->numericUpDown1->Maximum = 4096;
			xyNumericalControl1->numericUpDown1->Minimum = 1;
			xyNumericalControl1->numericUpDown2->Maximum = 4096;
			xyNumericalControl1->numericUpDown2->Minimum = 1;
			//xyNumericalControl1->numericUpDown2->Value = System::Decimal(DRAW_PANEL_RESOLUTION_HEIGHT);
			//xyNumericalControl1->SetValue(&DRAW_PANEL_RESOLUTION_WIDTH,&DRAW_PANEL_RESOLUTION_HEIGHT);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ResolutionDialog()
		{
			if (components)
			{
				delete components;
			}
		}
	private: DotNetComponent::XYNumericalControl^  xyNumericalControl1;
	private: System::Windows::Forms::Label^  Width_Label;
	private: System::Windows::Forms::Label^  label1;
	protected: 

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
			this->xyNumericalControl1 = (gcnew DotNetComponent::XYNumericalControl());
			this->Width_Label = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->xyNumericalControl1->SuspendLayout();
			this->SuspendLayout();
			// 
			// xyNumericalControl1
			// 
			this->xyNumericalControl1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->xyNumericalControl1->Controls->Add(this->Width_Label);
			this->xyNumericalControl1->Controls->Add(this->label1);
			this->xyNumericalControl1->Location = System::Drawing::Point(16, 9);
			this->xyNumericalControl1->Margin = System::Windows::Forms::Padding(0);
			this->xyNumericalControl1->Name = L"xyNumericalControl1";
			this->xyNumericalControl1->Size = System::Drawing::Size(176, 50);
			this->xyNumericalControl1->TabIndex = 0;
			// 
			// Width_Label
			// 
			this->Width_Label->AutoSize = true;
			this->Width_Label->Location = System::Drawing::Point(14, 3);
			this->Width_Label->Name = L"Width_Label";
			this->Width_Label->Size = System::Drawing::Size(34, 12);
			this->Width_Label->TabIndex = 1;
			this->Width_Label->Text = L"Width";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(15, 31);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(36, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Height";
			// 
			// ResolutionDialog
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(206, 69);
			this->Controls->Add(this->xyNumericalControl1);
			this->Name = L"ResolutionDialog";
			this->Text = L"ResolutionDialog";
			this->xyNumericalControl1->ResumeLayout(false);
			this->xyNumericalControl1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
