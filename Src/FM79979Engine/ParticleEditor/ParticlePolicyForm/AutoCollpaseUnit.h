#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm 
{
	/// <summary>
	/// AutoCollpaseUnit 的摘要
	/// </summary>
	public ref class AutoCollpaseUnit : public System::Windows::Forms::UserControl
	{
	public:
		//input the name will show on groupbox,the size that dest object occupied.
		AutoCollpaseUnit(String^e_strName,System::Drawing::Size e_DestSize,System::Windows::Forms::Control^e_pControl)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			this->Collapase_button->Text = e_strName;
			this->Collapase_button->Size = System::Drawing::Size(e_DestSize.Width,this->Collapase_button->Size.Height);
			e_pControl->Location = System::Drawing::Point(0,Collapase_button->Size.Height);
			m_OriginalSize = System::Drawing::Size(e_DestSize.Width,e_DestSize.Height+Collapase_button->Location.Y+Collapase_button->Size.Height);
			this->Size = m_OriginalSize;
			Collapase_button->Location = System::Drawing::Point(0,0);
			this->Controls->Add(e_pControl);
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~AutoCollpaseUnit()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  Collapase_button;

	protected: 

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;
		//my
		System::Drawing::Size m_OriginalSize;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->Collapase_button = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// Collapase_button
			// 
			this->Collapase_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->Collapase_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Collapase_button->Location = System::Drawing::Point(11, 0);
			this->Collapase_button->Margin = System::Windows::Forms::Padding(0);
			this->Collapase_button->Name = L"Collapase_button";
			this->Collapase_button->Size = System::Drawing::Size(184, 24);
			this->Collapase_button->TabIndex = 0;
			this->Collapase_button->UseVisualStyleBackColor = false;
			this->Collapase_button->Click += gcnew System::EventHandler(this, &AutoCollpaseUnit::Collapase_button_Click);
			// 
			// AutoCollpaseUnit
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->Collapase_button);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"AutoCollpaseUnit";
			this->Size = System::Drawing::Size(203, 117);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Collapase_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( this->Size.Height == m_OriginalSize.Height )
					this->Size = this->Collapase_button->Size;
				 else
					 this->Size = this->m_OriginalSize;
			 }
	};
}
