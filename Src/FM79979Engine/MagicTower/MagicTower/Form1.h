#pragma once
#include "../../../include/IL/il.h"
#include "GameApp.h"
//#pragma comment(lib, "../../lib/unicode/Devil.lib")
//#pragma comment(lib, "../../lib/OpenAL32.lib")
//#pragma comment(lib, "../../lib/alut.lib")
//#pragma comment(lib, "../../lib/ogg.lib")//I have build my self OGG lib here
//#pragma comment(lib, "../../lib/glew32.lib")
namespace MagicTower {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			m_pGameApp = 0;
			//cNodeISAX	l_cNodeISAX;
			//l_cNodeISAX.ReplaceNodeDataByTemplateNode("C:/Users/Fatming/Desktop/FM79979/Media/MagicTower/Data/Test.xml","C:/Users/Fatming/Desktop/FM79979/Media/MagicTower/Data/bibo.xml","C:/Users/Fatming/Desktop/FM79979/Media/MagicTower/Data/AZXC.xml");
			InitializeComponent();
			//m_Hdc = GetDC((HWND)splitContainer1->Panel2->Handle.ToPointer());
			//m_HGLRC = UT::InitOpenGL((HWND)splitContainer1->Panel2->Handle.ToPointer(),true,m_Hdc);
			//wglMakeCurrent( m_Hdc,m_HGLRC );
			splitContainer1->Panel2->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseMove);
			splitContainer1->Panel2->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseDown);
			splitContainer1->Panel2->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
			splitContainer1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
			m_pGameApp = new cMagicTowerApp((HWND)splitContainer1->Panel2->Handle.ToPointer(),Vector2(IPHONE_5_RESOLUTION_HEIGHT,IPHONE_5_RESOLUTION_WIDTH));
			m_pGameApp->Init();
			m_pOrthogonalCamera = cMagicTowerApp::m_sp2DCamera;
			if( m_pOrthogonalCamera )
			{
				m_fResolutionWidth = 960;
				m_fResolutionHeight = 480.f;
				m_pOrthogonalCamera->SetResolution(Vector2(m_fResolutionWidth,m_fResolutionHeight));
				m_pOrthogonalCamera->Reset();
			}
			timer1->Enabled = true;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~Form1()
		{
			SAFE_DELETE(m_pGameApp);
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  resolutionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x320ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x320ToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  x320ToolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  x320ToolStripMenuItem3;
	private: System::Windows::Forms::ToolStripMenuItem^  x320ToolStripMenuItem4;
		//my
		//HDC		 m_Hdc;
		//HGLRC	 m_HGLRC;
		cMagicTowerApp*m_pGameApp;
		float	m_fResolutionWidth;
		float	m_fResolutionHeight;
	private: System::Windows::Forms::Button^  ChangeWalkingView_button;

			 cOrthogonalCamera*m_pOrthogonalCamera;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->ChangeWalkingView_button = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resolutionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x320ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x320ToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x320ToolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x320ToolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x320ToolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 26);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->ChangeWalkingView_button);
			this->splitContainer1->Size = System::Drawing::Size(819, 499);
			this->splitContainer1->SplitterDistance = 273;
			this->splitContainer1->TabIndex = 0;
			// 
			// ChangeWalkingView_button
			// 
			this->ChangeWalkingView_button->Location = System::Drawing::Point(10, 24);
			this->ChangeWalkingView_button->Name = L"ChangeWalkingView_button";
			this->ChangeWalkingView_button->Size = System::Drawing::Size(152, 52);
			this->ChangeWalkingView_button->TabIndex = 0;
			this->ChangeWalkingView_button->Text = L"ChangeWalkingView";
			this->ChangeWalkingView_button->UseVisualStyleBackColor = true;
			this->ChangeWalkingView_button->Click += gcnew System::EventHandler(this, &Form1::ChangeWalkingView_button_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->resolutionToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(819, 26);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(40, 22);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// resolutionToolStripMenuItem
			// 
			this->resolutionToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->x320ToolStripMenuItem, 
				this->x320ToolStripMenuItem1, this->x320ToolStripMenuItem2, this->x320ToolStripMenuItem3, this->x320ToolStripMenuItem4});
			this->resolutionToolStripMenuItem->Name = L"resolutionToolStripMenuItem";
			this->resolutionToolStripMenuItem->Size = System::Drawing::Size(85, 22);
			this->resolutionToolStripMenuItem->Text = L"Resolution";
			// 
			// x320ToolStripMenuItem
			// 
			this->x320ToolStripMenuItem->Name = L"x320ToolStripMenuItem";
			this->x320ToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->x320ToolStripMenuItem->Text = L"480x320";
			this->x320ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x320ToolStripMenuItem_Click);
			// 
			// x320ToolStripMenuItem1
			// 
			this->x320ToolStripMenuItem1->Name = L"x320ToolStripMenuItem1";
			this->x320ToolStripMenuItem1->Size = System::Drawing::Size(148, 22);
			this->x320ToolStripMenuItem1->Text = L"640x480";
			this->x320ToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::x320ToolStripMenuItem_Click);
			// 
			// x320ToolStripMenuItem2
			// 
			this->x320ToolStripMenuItem2->Name = L"x320ToolStripMenuItem2";
			this->x320ToolStripMenuItem2->Size = System::Drawing::Size(148, 22);
			this->x320ToolStripMenuItem2->Text = L"1280x720";
			this->x320ToolStripMenuItem2->Click += gcnew System::EventHandler(this, &Form1::x320ToolStripMenuItem_Click);
			// 
			// x320ToolStripMenuItem3
			// 
			this->x320ToolStripMenuItem3->Name = L"x320ToolStripMenuItem3";
			this->x320ToolStripMenuItem3->Size = System::Drawing::Size(148, 22);
			this->x320ToolStripMenuItem3->Text = L"1280x1024";
			this->x320ToolStripMenuItem3->Click += gcnew System::EventHandler(this, &Form1::x320ToolStripMenuItem_Click);
			// 
			// x320ToolStripMenuItem4
			// 
			this->x320ToolStripMenuItem4->Name = L"x320ToolStripMenuItem4";
			this->x320ToolStripMenuItem4->Size = System::Drawing::Size(148, 22);
			this->x320ToolStripMenuItem4->Text = L"1920x1024";
			this->x320ToolStripMenuItem4->Click += gcnew System::EventHandler(this, &Form1::x320ToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(819, 525);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->KeyPreview = true;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::splitContainer1_KeyUp);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void CameraUpdate(System::Windows::Forms::MouseEventArgs^  e,System::Windows::Forms::Panel^e_pPanel,cOrthogonalCamera*e_pOrthogonalCamera)
	{
		 sMouseState	l_sMouseState;
		 memset(&l_sMouseState,0,sizeof(sMouseState));
		 l_sMouseState.iMoveX = e->X - e_pOrthogonalCamera->GetMouseBehave()->GetMouseState().iCurrentX;
		 l_sMouseState.iMoveY = e->Y - e_pOrthogonalCamera->GetMouseBehave()->GetMouseState().iCurrentY;
		 l_sMouseState.iCurrentX = e->X;
		 l_sMouseState.iCurrentY = e->Y;
		 if( e->Delta != 0 )
		 //if(e->Button == System::Windows::Forms::MouseButtons::Middle)
		 {
			 l_sMouseState.iMoveZ = e->Delta/120;
			 l_sMouseState.bMidButton = true;
		 }
		 else
		 if ( e->Button == System::Windows::Forms::MouseButtons::Left )
			 l_sMouseState.bLefButton = true;
		 else
			 if(e->Button == System::Windows::Forms::MouseButtons::Right)
		 //if (e->Button == System::Windows::Forms::MouseButtons::Right)
			 l_sMouseState.bRigButton = true;
		 e_pOrthogonalCamera->GetMouseBehave()->Update(l_sMouseState);
		 POINT l_ViewportSize = { e_pPanel->Width,e_pPanel->Height };
		 if( m_pOrthogonalCamera )
			m_pOrthogonalCamera->Render();
		 //e_pOrthogonalCamera->Update(l_ViewportSize);
		 l_sMouseState.bLefButton = false;
		 l_sMouseState.bMidButton = false;;
		 l_sMouseState.bRigButton = false;;
		 e_pOrthogonalCamera->GetMouseBehave()->Update(l_sMouseState);
	}
	private: System::Void splitContainer1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
			 {
				 if( m_pGameApp )
					m_pGameApp->KeyDown((char)e->KeyData);\
			 }
	private: System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 splitContainer1->Panel2->Focus();
				 if( m_pOrthogonalCamera )
					CameraUpdate(e,splitContainer1->Panel2,m_pOrthogonalCamera);
				 //m_pGameApp->MouseUp((int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y);
				 if( m_pGameApp )
					m_pGameApp->MouseUp(e->X,e->Y);
			 }
	private: System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 splitContainer1->Panel2->Focus();
				 if( m_pOrthogonalCamera )
					CameraUpdate(e,splitContainer1->Panel2,m_pOrthogonalCamera);
				 //m_pGameApp->MouseDown((int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y);
				 if( m_pGameApp )
					m_pGameApp->MouseDown(e->X,e->Y);
			 }
	private: System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 splitContainer1->Panel2->Focus();
				 if( m_pOrthogonalCamera )
					CameraUpdate(e,splitContainer1->Panel2,m_pOrthogonalCamera);
				//m_pGameApp->MouseMove((int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y);
				if( m_pGameApp )
					m_pGameApp->MouseMove(e->X,e->Y);
			 }
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
				 cGameApp::m_svViewPortSize.x = 0;
				 cGameApp::m_svViewPortSize.y = 0;
				 cGameApp::m_svViewPortSize.z = (float)splitContainer1->Panel2->Width;
				 cGameApp::m_svViewPortSize.w = (float)splitContainer1->Panel2->Height;
				 //glViewport(0,0,splitContainer1->Panel2->Width,splitContainer1->Panel2->Height);
				 POINT	l_ViewSize = {splitContainer1->Panel2->Width,splitContainer1->Panel2->Height};
				 //m_pOrthogonalCamera->Update(l_ViewSize);
				 if( m_pGameApp )
					m_pGameApp->Run();
				 //this->Text = ((int)(m_pOrthogonalCamera->GetMouseWorldPos().x)).ToString()+","+
				//			  ((int)(m_pOrthogonalCamera->GetMouseWorldPos().y)).ToString();
			 }
	private: System::Void x320ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				if( sender == x320ToolStripMenuItem )
				{
					m_fResolutionWidth = 480.f;
					m_fResolutionHeight = 320.f;
				}
				else
				if( sender == x320ToolStripMenuItem1 )
				{
					m_fResolutionWidth = 640.f;
					m_fResolutionHeight = 480.f;
				}
				else
				if( sender == x320ToolStripMenuItem2 )
				{
					m_fResolutionWidth = 1280.f;
					m_fResolutionHeight = 720.f;
				}
				else
				if( sender == x320ToolStripMenuItem3 )
				{
					m_fResolutionWidth = 1280.f;
					m_fResolutionHeight = 720.f;				
				}
				else
				if( sender == x320ToolStripMenuItem4 )
				{
					m_fResolutionWidth = 1920.f;
					m_fResolutionHeight = 1280.f;
				}
				if( m_pOrthogonalCamera )
					this->m_pOrthogonalCamera->SetResolution(Vector2(m_fResolutionWidth,m_fResolutionHeight));
			 }
private: System::Void ChangeWalkingView_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pGameApp )
				m_pGameApp->ChangeWalkingView();
		 }
};
}

