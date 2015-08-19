#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace XMLDataEditor {

	/// <summary>
	/// Summary for TextToXmlEditor
	/// </summary>
	public ref class TextToXmlEditor : public System::Windows::Forms::UserControl
	{
	public:
		TextToXmlEditor(void)
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
		~TextToXmlEditor()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::TextBox^  XmlText_textBox;
	private: System::Windows::Forms::TreeView^  XmlTreeView_treeView;
	private: System::Windows::Forms::TextBox^  ErrorText_textBox;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
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
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->XmlText_textBox = (gcnew System::Windows::Forms::TextBox());
			this->XmlTreeView_treeView = (gcnew System::Windows::Forms::TreeView());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->ErrorText_textBox = (gcnew System::Windows::Forms::TextBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->XmlText_textBox);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->XmlTreeView_treeView);
			this->splitContainer1->Panel2Collapsed = true;
			this->splitContainer1->Size = System::Drawing::Size(286, 192);
			this->splitContainer1->SplitterDistance = 136;
			this->splitContainer1->TabIndex = 0;
			// 
			// XmlText_textBox
			// 
			this->XmlText_textBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->XmlText_textBox->Location = System::Drawing::Point(0, 0);
			this->XmlText_textBox->Multiline = true;
			this->XmlText_textBox->Name = L"XmlText_textBox";
			this->XmlText_textBox->Size = System::Drawing::Size(286, 192);
			this->XmlText_textBox->TabIndex = 0;
			this->XmlText_textBox->TextChanged += gcnew System::EventHandler(this, &TextToXmlEditor::XmlText_textBox_TextChanged);
			// 
			// XmlTreeView_treeView
			// 
			this->XmlTreeView_treeView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->XmlTreeView_treeView->Location = System::Drawing::Point(0, 0);
			this->XmlTreeView_treeView->Name = L"XmlTreeView_treeView";
			this->XmlTreeView_treeView->Size = System::Drawing::Size(146, 192);
			this->XmlTreeView_treeView->TabIndex = 0;
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->splitContainer1);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->ErrorText_textBox);
			this->splitContainer2->Size = System::Drawing::Size(286, 221);
			this->splitContainer2->SplitterDistance = 192;
			this->splitContainer2->TabIndex = 1;
			// 
			// ErrorText_textBox
			// 
			this->ErrorText_textBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ErrorText_textBox->Location = System::Drawing::Point(0, 0);
			this->ErrorText_textBox->Multiline = true;
			this->ErrorText_textBox->Name = L"ErrorText_textBox";
			this->ErrorText_textBox->Size = System::Drawing::Size(286, 25);
			this->ErrorText_textBox->TabIndex = 0;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 2000;
			this->timer1->Tick += gcnew System::EventHandler(this, &TextToXmlEditor::timer1_Tick);
			// 
			// TextToXmlEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer2);
			this->Name = L"TextToXmlEditor";
			this->Size = System::Drawing::Size(286, 221);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->Panel2->PerformLayout();
			this->splitContainer2->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
				this->timer1->Enabled = false;
				System::Xml::XmlDataDocument^ xmldoc = gcnew System::Xml::XmlDataDocument();
				System::String^l_pString = XmlText_textBox->Text;
				try 
				{
					xmldoc->LoadXml(l_pString);
				}
				catch(XmlException^ xmlEx)
				{
					ErrorText_textBox->Text = xmlEx->ToString();
				}

		 }
private: System::Void XmlText_textBox_TextChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			this->timer1->Stop();
			this->timer1->Start();
			this->timer1->Enabled = true;
		 }
};
}
