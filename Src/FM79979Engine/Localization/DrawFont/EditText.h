#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace DrawFont {

	/// <summary>
	/// EDitText 的摘要
	///
	/// 警告: 如果您變更這個類別的名稱，就必須變更與這個類別所依據之所有 .resx 檔案關聯的
	///          Managed 資源編譯器工具的 'Resource File Name' 屬性。
	///          否則，這些設計工具
	///          將無法與這個表單關聯的當地語系化資源
	///          正確互動。
	/// </summary>
	public ref class EDitText : public System::Windows::Forms::Form
	{
	public:
		EDitText(String^e_str)
		{
			InitializeComponent();
			if( e_str )
				this->Edit_textBox->Text = e_str;
			//
			//TODO: 在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~EDitText()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  Edit_textBox;
	protected: 

	protected: 
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Label^  label1;
	public:	 static   System::String^m_pString;
	protected: 

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->Edit_textBox = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// Edit_textBox
			// 
			this->Edit_textBox->Location = System::Drawing::Point(25, 19);
			this->Edit_textBox->Multiline = true;
			this->Edit_textBox->Name = L"Edit_textBox";
			this->Edit_textBox->Size = System::Drawing::Size(334, 68);
			this->Edit_textBox->TabIndex = 0;
			this->Edit_textBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &EDitText::textBox1_KeyUp);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(100, 103);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(48, 18);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Ok";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &EDitText::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(229, 99);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(48, 22);
			this->button2->TabIndex = 2;
			this->button2->Text = L"cancel";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &EDitText::button2_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(2, 1);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(123, 12);
			this->label1->TabIndex = 3;
			this->label1->Text = L"F1 key is ok Esc is cancel";
			// 
			// EDitText
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(393, 133);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->Edit_textBox);
			this->Name = L"EDitText";
			this->Text = L"EDitText";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void textBox1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				 if( e->KeyValue == 112 )//f1
					WriteText();
				 else
				 if( e->KeyCode == GCFORM::Keys::Escape )//Esc
					CancelWriteText();
			 }
	private: System::Void	WriteText()
			 {
				//int	l_iStartIndex = 0;
				//std::vector<int>	l_AllChangelineIndex;
				//int	l_NewIndex = 0;
				//while( l_NewIndex!=-1 )
				//{
				//	l_NewIndex = Edit_textBox->Text->IndexOf(DNCT::GetChanglineString(),l_iStartIndex);
				//	l_iStartIndex = l_NewIndex;
				//	l_AllChangelineIndex.push_back()
				//}
				//char	l_FinalString[2048];
				//memset(l_FinalString,0,sizeof(l_FinalString));
				//int	l_iTotalLength = 0;
				//for(int i=0;i<Edit_textBox->Lines->Length;++i)
				//{
				//	String^l_str = Edit_textBox->Lines[i];
				//	strcat_s(l_FinalString,2048,DNCT::GcStringToChar(l_str));
				//	l_iTotalLength += l_str->Length;
				//	l_FinalString[l_iTotalLength] = '\0';
				//	if( i!=Edit_textBox->Lines->Length-1 )
				//	{
				//		strcat_s(l_FinalString,"\n");
				//		l_iTotalLength++;
				//	}
				//}
				this->m_pString = TO_GCSTRING(Edit_textBox->Text);
				this->Close();
			 }
	private: System::Void	CancelWriteText()
			 {
					 this->Close();
					 m_pString = "";
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
					WriteText();
			 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 CancelWriteText();
		 }
};
}
