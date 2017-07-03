#pragma once

namespace EditorSample {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for SoundFrequencyAnalyizer
	/// </summary>
	public ref class SoundFrequencyAnalyizer : public System::Windows::Forms::Form
	{
	public:
		SoundFrequencyAnalyizer(void)
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
		~SoundFrequencyAnalyizer()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::Panel^  OpenGL_panel;
	private: System::Windows::Forms::Button^  StartToRecord_button;
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
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->OpenGL_panel = (gcnew System::Windows::Forms::Panel());
			this->StartToRecord_button = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->OpenGL_panel);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->StartToRecord_button);
			this->splitContainer1->Size = System::Drawing::Size(1264, 681);
			this->splitContainer1->SplitterDistance = 397;
			this->splitContainer1->TabIndex = 0;
			// 
			// OpenGL_panel
			// 
			this->OpenGL_panel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->OpenGL_panel->Location = System::Drawing::Point(0, 0);
			this->OpenGL_panel->Name = L"OpenGL_panel";
			this->OpenGL_panel->Size = System::Drawing::Size(1264, 397);
			this->OpenGL_panel->TabIndex = 0;
			// 
			// StartToRecord_button
			// 
			this->StartToRecord_button->Location = System::Drawing::Point(57, 26);
			this->StartToRecord_button->Name = L"StartToRecord_button";
			this->StartToRecord_button->Size = System::Drawing::Size(104, 54);
			this->StartToRecord_button->TabIndex = 0;
			this->StartToRecord_button->Text = L"StartToRecord";
			this->StartToRecord_button->UseVisualStyleBackColor = true;
			// 
			// SoundFrequencyAnalyizer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1264, 681);
			this->Controls->Add(this->splitContainer1);
			this->Name = L"SoundFrequencyAnalyizer";
			this->Text = L"SoundFrequencyAnalyizer";
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
