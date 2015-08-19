#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace DotNetComponent {

	/// <summary>
	/// Summary for CheckBoxControl
	/// </summary>
	public ref class CheckBoxControl : public System::Windows::Forms::UserControl
	{
	public:
		CheckBoxControl(void)
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
		~CheckBoxControl()
		{
			if (components)
			{
				delete components;
			}
		}
	public:[TypeConverterAttribute(String::typeid),DescriptionAttribute("CheckboxTextDescriptionAttribute")]
		   [CategoryAttribute("My")]
		   property String^ CheckboxName
		   {
				String^ get(){ return checkBox->Text; }
				System::Void set(String^ value){ checkBox->Text = value; }
		   }
	//PUBLIC_OBJECT_CATE_AND_CONVER(System::Windows::Forms::CheckBox^,checkBox,MyCheckBox,"My",System::Windows::Forms::CheckBox,"Fatming qoo");
	public: System::Windows::Forms::CheckBox^  checkBox;
	//my
	public:bool*m_pValue;
	public:void	SetValue(bool*e_pbValue)
		   {
				m_pValue = e_pbValue;
				AssignValueToUI();
		   }
	public:void	AssignValueToUI()
		   {
				if( m_pValue )
				{
					checkBox->Checked = *m_pValue;
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
			this->checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// checkBox
			// 
			this->checkBox->AutoSize = true;
			this->checkBox->Location = System::Drawing::Point(3, 3);
			this->checkBox->Name = L"checkBox";
			this->checkBox->Size = System::Drawing::Size(77, 16);
			this->checkBox->TabIndex = 0;
			this->checkBox->Text = L"checkBox1";
			this->checkBox->UseVisualStyleBackColor = true;
			this->checkBox->CheckedChanged += gcnew System::EventHandler(this, &CheckBoxControl::checkBox_CheckedChanged);
			// 
			// CheckBoxControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->checkBox);
			this->Name = L"CheckBoxControl";
			this->Size = System::Drawing::Size(129, 24);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				if( m_pValue )
					*m_pValue = checkBox->Checked;
			 }
	};
}
