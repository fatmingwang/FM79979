#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm {

	/// <summary>
	/// PrtColorActBlending 的摘要
	/// </summary>
	public ref class PrtColorActBlending : public System::Windows::Forms::UserControl
	{
	public:
		PrtColorActBlending(float*e_pfR,float*e_pfG,float*e_pfB,float*e_pfA,bool *e_pbFade)
		{
			InitializeComponent();
			xyzwNumericalControl1->SetValue(e_pfR,e_pfG,e_pfB,e_pfA);
			m_pbFade = e_pbFade;
			ColorValueBehavior_checkBox->Checked = *m_pbFade;
			//
			//TODO: 在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~PrtColorActBlending()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ToolTip^  toolTip1;
	private: System::ComponentModel::IContainer^  components;
	public: 
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::CheckBox^  ColorValueBehavior_checkBox;
	private: DotNetComponent::XYZWNumericalControl^  xyzwNumericalControl1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label1;
	public: 
	public:bool*	m_pbFade;
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
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->ColorValueBehavior_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->xyzwNumericalControl1 = (gcnew DotNetComponent::XYZWNumericalControl());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->xyzwNumericalControl1->SuspendLayout();
			this->SuspendLayout();
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->label5->Location = System::Drawing::Point(12, 112);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(27, 12);
			this->label5->TabIndex = 6;
			this->label5->Text = L"Fade";
			this->toolTip1->SetToolTip(this->label5, L"while true color value is add I.E");
			// 
			// ColorValueBehavior_checkBox
			// 
			this->ColorValueBehavior_checkBox->AutoSize = true;
			this->ColorValueBehavior_checkBox->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->ColorValueBehavior_checkBox->Location = System::Drawing::Point(55, 113);
			this->ColorValueBehavior_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->ColorValueBehavior_checkBox->Name = L"ColorValueBehavior_checkBox";
			this->ColorValueBehavior_checkBox->Size = System::Drawing::Size(44, 16);
			this->ColorValueBehavior_checkBox->TabIndex = 7;
			this->ColorValueBehavior_checkBox->Text = L"Add";
			this->ColorValueBehavior_checkBox->UseVisualStyleBackColor = true;
			this->ColorValueBehavior_checkBox->CheckedChanged += gcnew System::EventHandler(this, &PrtColorActBlending::ColorValueBehavior_checkBox_CheckedChanged);
			// 
			// xyzwNumericalControl1
			// 
			this->xyzwNumericalControl1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->xyzwNumericalControl1->Controls->Add(this->label2);
			this->xyzwNumericalControl1->Controls->Add(this->label4);
			this->xyzwNumericalControl1->Controls->Add(this->label3);
			this->xyzwNumericalControl1->Controls->Add(this->label1);
			this->xyzwNumericalControl1->Location = System::Drawing::Point(4, 3);
			this->xyzwNumericalControl1->Name = L"xyzwNumericalControl1";
			this->xyzwNumericalControl1->Size = System::Drawing::Size(146, 105);
			this->xyzwNumericalControl1->TabIndex = 8;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->label2->Location = System::Drawing::Point(3, 57);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(27, 12);
			this->label2->TabIndex = 10;
			this->label2->Text = L"Blue";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->label4->Location = System::Drawing::Point(4, 80);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(33, 12);
			this->label4->TabIndex = 11;
			this->label4->Text = L"Alpha";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->label3->Location = System::Drawing::Point(4, 32);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(33, 12);
			this->label3->TabIndex = 12;
			this->label3->Text = L"Green";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->label1->Location = System::Drawing::Point(5, 7);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(24, 12);
			this->label1->TabIndex = 9;
			this->label1->Text = L"Red";
			// 
			// PrtColorActBlending
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Controls->Add(this->xyzwNumericalControl1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->ColorValueBehavior_checkBox);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"PrtColorActBlending";
			this->Size = System::Drawing::Size(147, 131);
			this->xyzwNumericalControl1->ResumeLayout(false);
			this->xyzwNumericalControl1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void ColorValueBehavior_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		{
			ColorValueBehavior_checkBox->Text = ColorValueBehavior_checkBox->Checked?"Add":"Minus";
			*m_pbFade = ColorValueBehavior_checkBox->Checked;
		}
};
}
