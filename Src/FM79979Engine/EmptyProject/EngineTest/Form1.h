#pragma once

#include "GameApp.h"
#include "../../include/IL/il.h"
#include "../../DotNetCommon/DotNetCommonTools.h"
#include "../../Core/GLSL/TunnelEffect.h"
#include "Trigonometric.h"
extern cTunnelEffect*g_pTunnelEffect;
extern cBaseImage*g_pBGImage;
namespace FATMING_CORE
{
	extern bool	g_bShowErrorMessageBoxIfShaderIsWrong;
}
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
			m_pPanel = splitContainer1->Panel1;
			m_pGameApp = new cEngineTestApp((HWND)m_pPanel->Handle.ToPointer(),Vector2(1024,768),Vector2(1024,768));
			cGameApp::ResoluctionParse("EngineTestSetup.xml");
			m_pGameApp->Init();
			this->splitContainer1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->m_pPanel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->m_pPanel->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->m_pPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->m_pPanel->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			//this->Size = System::Drawing::Size((int)m_pGameApp->m_svViewPortSize.x,(int)m_pGameApp->m_svViewPortSize.y);
			//this->m_pPanelCollapsed = true;
			g_bShowErrorMessageBoxIfShaderIsWrong = false;
			m_fTimeForShaderReload = 0.f;
			m_CurrentShaderType = 0;
			AssignShader(VSShaderFile_textBox,VSShaderContent_textBox);
			AssignShader(PSShaderFile_textBox,PSShaderContent_textBox);
			m_fTimeForShaderReload = 0.f;
			Trigonometric^l_pTrigonometric = gcnew Trigonometric();
			l_pTrigonometric->Show();
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
	private: System::Windows::Forms::TextBox^  VSLog_textBox;
	private: System::Windows::Forms::TextBox^  VSShaderContent_textBox;
	private: System::Windows::Forms::TabControl^  tabControl;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TextBox^  VSShaderFile_textBox;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TextBox^  PSShaderContent_textBox;
	private: System::Windows::Forms::TextBox^  PSShaderFile_textBox;
	private: System::Windows::Forms::TextBox^  PSLog_textBox;
			 //my
	   	    cEngineTestApp*m_pGameApp;
			float			m_fTimeForShaderReload;
			//0 for VS,1 for PS
			int				m_CurrentShaderType;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
			 GCFORM::Control^m_pPanel;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->tabControl = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->VSShaderContent_textBox = (gcnew System::Windows::Forms::TextBox());
			this->VSShaderFile_textBox = (gcnew System::Windows::Forms::TextBox());
			this->VSLog_textBox = (gcnew System::Windows::Forms::TextBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->PSShaderContent_textBox = (gcnew System::Windows::Forms::TextBox());
			this->PSShaderFile_textBox = (gcnew System::Windows::Forms::TextBox());
			this->PSLog_textBox = (gcnew System::Windows::Forms::TextBox());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->tabControl->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->toolStrip1->SuspendLayout();
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
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->tabControl);
			this->splitContainer1->Size = System::Drawing::Size(1013, 692);
			this->splitContainer1->SplitterDistance = 395;
			this->splitContainer1->TabIndex = 2;
			// 
			// tabControl
			// 
			this->tabControl->Controls->Add(this->tabPage1);
			this->tabControl->Controls->Add(this->tabPage2);
			this->tabControl->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl->Location = System::Drawing::Point(0, 0);
			this->tabControl->Name = L"tabControl";
			this->tabControl->SelectedIndex = 0;
			this->tabControl->Size = System::Drawing::Size(612, 690);
			this->tabControl->TabIndex = 3;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->VSShaderContent_textBox);
			this->tabPage1->Controls->Add(this->VSShaderFile_textBox);
			this->tabPage1->Controls->Add(this->VSLog_textBox);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(604, 664);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"vs";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// VSShaderContent_textBox
			// 
			this->VSShaderContent_textBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->VSShaderContent_textBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->VSShaderContent_textBox->Location = System::Drawing::Point(3, 23);
			this->VSShaderContent_textBox->Multiline = true;
			this->VSShaderContent_textBox->Name = L"VSShaderContent_textBox";
			this->VSShaderContent_textBox->Size = System::Drawing::Size(598, 518);
			this->VSShaderContent_textBox->TabIndex = 0;
			this->VSShaderContent_textBox->TextChanged += gcnew System::EventHandler(this, &Form1::VSShaderContent_textBox_TextChanged);
			// 
			// VSShaderFile_textBox
			// 
			this->VSShaderFile_textBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->VSShaderFile_textBox->Location = System::Drawing::Point(3, 3);
			this->VSShaderFile_textBox->Name = L"VSShaderFile_textBox";
			this->VSShaderFile_textBox->Size = System::Drawing::Size(598, 20);
			this->VSShaderFile_textBox->TabIndex = 2;
			this->VSShaderFile_textBox->Text = L"shader/TunnelEffect.vs";
			// 
			// VSLog_textBox
			// 
			this->VSLog_textBox->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->VSLog_textBox->Location = System::Drawing::Point(3, 541);
			this->VSLog_textBox->Multiline = true;
			this->VSLog_textBox->Name = L"VSLog_textBox";
			this->VSLog_textBox->Size = System::Drawing::Size(598, 120);
			this->VSLog_textBox->TabIndex = 1;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->PSShaderContent_textBox);
			this->tabPage2->Controls->Add(this->PSShaderFile_textBox);
			this->tabPage2->Controls->Add(this->PSLog_textBox);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(604, 664);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"ps";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// PSShaderContent_textBox
			// 
			this->PSShaderContent_textBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PSShaderContent_textBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->PSShaderContent_textBox->Location = System::Drawing::Point(3, 23);
			this->PSShaderContent_textBox->Multiline = true;
			this->PSShaderContent_textBox->Name = L"PSShaderContent_textBox";
			this->PSShaderContent_textBox->Size = System::Drawing::Size(598, 551);
			this->PSShaderContent_textBox->TabIndex = 3;
			this->PSShaderContent_textBox->TextChanged += gcnew System::EventHandler(this, &Form1::PSShaderContent_textBox_TextChanged);
			// 
			// PSShaderFile_textBox
			// 
			this->PSShaderFile_textBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->PSShaderFile_textBox->Location = System::Drawing::Point(3, 3);
			this->PSShaderFile_textBox->Name = L"PSShaderFile_textBox";
			this->PSShaderFile_textBox->Size = System::Drawing::Size(598, 20);
			this->PSShaderFile_textBox->TabIndex = 5;
			this->PSShaderFile_textBox->Text = L"shader/TunnelEffect.ps";
			// 
			// PSLog_textBox
			// 
			this->PSLog_textBox->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->PSLog_textBox->Location = System::Drawing::Point(3, 574);
			this->PSLog_textBox->Multiline = true;
			this->PSLog_textBox->Name = L"PSLog_textBox";
			this->PSLog_textBox->Size = System::Drawing::Size(598, 87);
			this->PSLog_textBox->TabIndex = 4;
			// 
			// toolStrip1
			// 
			this->toolStrip1->AllowItemReorder = true;
			this->toolStrip1->Dock = System::Windows::Forms::DockStyle::Left;
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripButton1});
			this->toolStrip1->Location = System::Drawing::Point(0, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(24, 692);
			this->toolStrip1->TabIndex = 3;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripButton1
			// 
			this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton1.Image")));
			this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton1->Name = L"toolStripButton1";
			this->toolStripButton1->Size = System::Drawing::Size(21, 20);
			this->toolStripButton1->Text = L"toolStripButton1";
			this->toolStripButton1->Click += gcnew System::EventHandler(this, &Form1::toolStripButton1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1013, 692);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->splitContainer1);
			this->KeyPreview = true;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyDown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyUp);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->tabControl->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_fTimeForShaderReload != 0.f )
			 {
				m_fTimeForShaderReload += 0.016f;
				if( m_fTimeForShaderReload >1.f )
				{
					String^l_strFileName;
					String^l_strShaderContent;
					if( m_CurrentShaderType == 1 )
					{
						l_strFileName = PSShaderFile_textBox->Text;
						l_strShaderContent = PSShaderContent_textBox->Text;
					}
					else
					{
						l_strFileName = VSShaderFile_textBox->Text;
						l_strShaderContent = VSShaderContent_textBox->Text;
					}
					if( System::IO::File::Exists(l_strFileName) )
					{
						std::string	l_strShaderFileName;
						l_strShaderFileName = DNCT::GcStringToChar(l_strFileName);
						std::string l_strContent = DNCT::GcStringToChar(l_strShaderContent);
						bool l_bFromFile = true;
						//SaveTxtToFile(l_strShaderFileName.c_str(),l_strContent.c_str(),l_strShaderContent->Length);
						if( l_bFromFile )
							StreamWriteFileFromString(l_strShaderContent,l_strFileName,System::Text::Encoding::GetEncoding("big5"));
						if( g_pTunnelEffect )
						{
							std::wstring l_strShaderName = g_pTunnelEffect->GetName();
							SAFE_DELETE(g_pTunnelEffect);
							if( l_bFromFile )//from file
							{
								std::string l_strVSFileName = DNCT::GcStringToChar(VSShaderFile_textBox->Text);
								std::string l_strPSFileName = DNCT::GcStringToChar(PSShaderFile_textBox->Text);
								g_pTunnelEffect = cTunnelEffect::CreateShader(l_strVSFileName.c_str(),l_strPSFileName.c_str(),l_strShaderName.c_str());
							}
							else
							{
								std::string l_strVSContent = DNCT::GcStringToChar(VSShaderContent_textBox->Text);
								std::string l_strPSContent = DNCT::GcStringToChar(PSShaderContent_textBox->Text);
								g_pTunnelEffect = new cTunnelEffect(l_strVSContent.c_str(),l_strPSContent.c_str(),l_strShaderName.c_str());
							}

							if(g_pTunnelEffect->m_strShaderLog.length())
							{
								VSLog_textBox->Text = gcnew String(g_pTunnelEffect->m_strShaderLog.c_str());
								PSLog_textBox->Text = gcnew String(g_pTunnelEffect->m_strShaderLog.c_str());
							}
						}
					}
					m_fTimeForShaderReload = 0.f;
				}
			 }
			 if( m_pGameApp )
			 {
				 m_pGameApp->m_svViewPortSize = Vector4(0,0,(float)this->m_pPanel->Width,(float)this->m_pPanel->Height);
				 m_pGameApp->Run();
			 }
		 }

private: System::Void Form1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( m_pGameApp )
				 m_pGameApp->KeyDown((char)e->KeyCode);
			 //if( e->KeyChar == 'q' )
				//this->splitContainer1->Panel1Collapsed = !this->splitContainer1->Panel1Collapsed;
		 }

private: System::Void Form1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( m_pGameApp )
				m_pGameApp->KeyUp((char)e->KeyCode);
		 }


private: System::Void MyMouseEvent(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 if( this->timer1->Enabled && m_pGameApp )
			 {
				m_pPanel->Focus();
				GCFORM::MouseButtons l_MouseButton = e->Button;
				if( l_MouseButton==GCFORM::MouseButtons::Left )
				    m_pGameApp->TouchSignalProcess((int)(e->X),(int)(e->Y),true);
				else
				    m_pGameApp->TouchSignalProcess((int)(e->X),(int)(e->Y),false);
			 }
		 }

private: System::Void PSShaderContent_textBox_TextChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_fTimeForShaderReload = 0.0001f;
			 m_CurrentShaderType = 1;
		 }
private: System::Void VSShaderContent_textBox_TextChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_fTimeForShaderReload = 0.0001f;
			 m_CurrentShaderType = 0;
		 }
private: void AssignShader(GCFORM::TextBox^e_pstrShaderFileName,GCFORM::TextBox^e_pstrShaderContent)
		 {
			if( e_pstrShaderFileName && e_pstrShaderContent )
			{
				String^l_strContent = DNCT::OpenFileGetText(e_pstrShaderFileName->Text,nullptr);
				if( l_strContent )
					e_pstrShaderContent->Text = l_strContent;
			}
		 }
private: System::Void toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			SAFE_DELETE(g_pBGImage);
			g_pBGImage = new cBaseImage("DownloadFont.png");
			g_pBGImage->SetWidth((int)cGameApp::m_svGameResolution.x);
			g_pBGImage->SetHeight((int)cGameApp::m_svGameResolution.y);
		 }
};
}