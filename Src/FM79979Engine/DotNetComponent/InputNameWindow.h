#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace DotNetComponent {

	/// <summary>
	/// Summary for InputNameWindow
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class InputNameWindow : public System::Windows::Forms::Form
	{
	public:
		InputNameWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			
			int boundWidth = System::Windows::Forms::Screen::PrimaryScreen->Bounds.Width;
			int boundHeight = System::Windows::Forms::Screen::PrimaryScreen->Bounds.Height;
			int x = boundWidth - this->Width;
			int y = boundHeight - this->Height;
			this->Location = Point(x / 2, y / 2);
			this->Name_textBox->Focus();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~InputNameWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  Ok_button;
	protected: 
	private: System::Windows::Forms::Button^  Chancel_button;
	public: System::Windows::Forms::TextBox^  Name_textBox;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		//my
		void	SetText(String^e_strName){ Name_textBox->Text = e_strName; }

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->Ok_button = (gcnew System::Windows::Forms::Button());
			this->Chancel_button = (gcnew System::Windows::Forms::Button());
			this->Name_textBox = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// Ok_button
			// 
			this->Ok_button->Location = System::Drawing::Point(12, 60);
			this->Ok_button->Name = L"Ok_button";
			this->Ok_button->Size = System::Drawing::Size(75, 22);
			this->Ok_button->TabIndex = 0;
			this->Ok_button->Text = L"Ok";
			this->Ok_button->UseVisualStyleBackColor = true;
			this->Ok_button->Click += gcnew System::EventHandler(this, &InputNameWindow::Ok_button_Click);
			// 
			// Chancel_button
			// 
			this->Chancel_button->Location = System::Drawing::Point(132, 60);
			this->Chancel_button->Name = L"Chancel_button";
			this->Chancel_button->Size = System::Drawing::Size(75, 22);
			this->Chancel_button->TabIndex = 1;
			this->Chancel_button->Text = L"Chancel";
			this->Chancel_button->UseVisualStyleBackColor = true;
			this->Chancel_button->Click += gcnew System::EventHandler(this, &InputNameWindow::Ok_button_Click);
			// 
			// Name_textBox
			// 
			this->Name_textBox->Location = System::Drawing::Point(33, 22);
			this->Name_textBox->Name = L"Name_textBox";
			this->Name_textBox->Size = System::Drawing::Size(142, 22);
			this->Name_textBox->TabIndex = 2;
			// 
			// InputNameWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(227, 96);
			this->Controls->Add(this->Name_textBox);
			this->Controls->Add(this->Chancel_button);
			this->Controls->Add(this->Ok_button);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->Name = L"InputNameWindow";
			this->Text = L"InputNameWindow";
			this->TopMost = true;
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Ok_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( sender == Chancel_button )
				 {
					 Name_textBox->Text = "";
				 }
				 this->Close();
			 }
	};
}
