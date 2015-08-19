#pragma once

#include "GameApp.h"
#include "../../include/IL/il.h"
#include "../../DotNetCommon/DotNetCommonTools.h"
namespace EngineTest
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pGameApp = new cEngineTestApp((HWND)splitContainer1->Panel2->Handle.ToPointer(),Vector2(1024,768),Vector2(1024,768));
			cGameApp::ResoluctionParse("EngineTestSetup.xml");
			m_pGameApp->Init();
			this->splitContainer1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->splitContainer1->Panel2->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->splitContainer1->Panel2->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->splitContainer1->Panel2->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->splitContainer1->Panel2->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->Size = System::Drawing::Size((int)m_pGameApp->m_svViewPortSize.x,(int)m_pGameApp->m_svViewPortSize.y);
			this->splitContainer1->Panel1Collapsed = true;
			this->timer1->Enabled = true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			delete m_pGameApp;
			if (components)
			{
				delete components;
			}
		}
	protected: 
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::ComponentModel::IContainer^  components;
			 //my
	   	    cEngineTestApp*m_pGameApp;
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
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Size = System::Drawing::Size(1013, 639);
			this->splitContainer1->SplitterDistance = 395;
			this->splitContainer1->TabIndex = 2;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1013, 639);
			this->Controls->Add(this->splitContainer1);
			this->KeyPreview = true;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::Form1_KeyPress);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyUp);
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_pGameApp->m_svViewPortSize = Vector2((float)this->splitContainer1->Panel2->Width,(float)this->splitContainer1->Panel2->Height);
			 m_pGameApp->Run();
		 }

private: System::Void Form1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			m_pGameApp->KeyUp((char)e->KeyCode);
		 }


private: System::Void MyMouseEvent(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 if( this->timer1->Enabled )
			 {
				splitContainer1->Panel2->Focus();
				GCFORM::MouseButtons l_MouseButton = e->Button;
				if( l_MouseButton==GCFORM::MouseButtons::Left )
				    m_pGameApp->TouchSignalProcess((int)(e->X),(int)(e->Y),true);
				else
				    m_pGameApp->TouchSignalProcess((int)(e->X),(int)(e->Y),false);
			 }
		 }


private: System::Void Form1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
		 {
			 m_pGameApp->KeyDown((char)e->KeyChar);
			 if( e->KeyChar == 'q' )
				this->splitContainer1->Panel1Collapsed = !this->splitContainer1->Panel1Collapsed;
		 }
};
}