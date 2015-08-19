#pragma once

//#include "../../../include/IL/il.h"
//#include "ResolutionDialog.h"
#include "ProjectManager.h"
//#pragma comment(lib, "../../../Media/Intermedia/debug/Edit_Element.lib")
//#pragma comment(lib, "../../lib/ogg.lib")
//#pragma comment(lib, "../../lib/vorbisfile.lib")
//extern bool	g_bSupportNonPowerOfTwoTexture;
namespace FATMING_CORE
{
	extern GLfloat	g_fMAG_FILTERValue;
	extern GLfloat	g_fMIN_FILTERValue;
}
namespace TextureEditor 
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	///
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			if(System::IO::File::Exists("Icon.ico"))
			{
				System::Drawing::Bitmap^l_p = gcnew System::Drawing::Bitmap("Icon.ico");
				this->Icon = System::Drawing::Icon::FromHandle(l_p->GetHicon());
			}
			FATMING_CORE::g_bSupportNonPowerOfTwoTexture = false;
			g_bLanguageChinese = false;
			if(System::IO::File::Exists("Setup.xml"))
			{
				cNodeISAX	*l_pNodeISAX = new cNodeISAX();
				bool	l_b = l_pNodeISAX->ParseDataIntoXMLNode("Setup.xml");
				if(l_b)
				{
					TiXmlElement*l_pTiXmlElement = l_pNodeISAX->GetRootElement();
					const WCHAR*l_strChinese = l_pTiXmlElement->Attribute(L"Chinese");
					if( l_strChinese )
					{
						//if(!wcscmp(l_strChinese,L"true"))
						//	g_bLanguageChinese = true;
					}
					const WCHAR*l_strSupportNonPowerOfTwo = l_pTiXmlElement->Attribute(L"NonPowerOfTwoSupport");
					if( l_strSupportNonPowerOfTwo )
					{
						if(!wcscmp(l_strSupportNonPowerOfTwo,L"false"))
							g_bSupportNonPowerOfTwoTexture = false;
					}
				}
				SAFE_DELETE(l_pNodeISAX);
			}
			else
			{
				System::String^l_str3 = System::Globalization::CultureInfo::CurrentCulture->EnglishName;
				if(l_str3->Contains("Taiwan"))
				{
					//g_bLanguageChinese = true;
				}			
			}
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			//MainTabl_tabControl2->TabPages->Remove(this->Collpase);
			//glViewport(0,0,1280,720);
			//ilInit();
			//alutInit (0,0);
			//GCFORM::TabControl^l_ptabControl;
			//FMCEditor::ProjectManager^l_pMPDI = gcnew FMCEditor::ProjectManager(this->splitContainer1);
			//System::EventArgs^  e;
			//l_pMPDI->NewMPDI(this,e);
			//StartAllPages(this->splitContainer1,this);
			//MainTabl_tabControl2->SelectedIndex = 0;
			double gl12Supported = 0.f;
			//char*l_strVecsion = (char*)glGetString(GL_VERSION);
			//gl12Supported = atof(l_strVecsion);
			//glShadeModel(GL_SMOOTH);									// Select Smooth Shading
			gLLINEARToolStripMenuItem->Checked = true;
			this->Text = "FatmingEditor V1.3";

			//menuStrip1->Visible = false;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			delete m_pProjectManager;
			//alutExit();
		}
	protected:
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  resolutionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x1080ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x720ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x768ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x480ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  customToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem;
	private: System::Windows::Forms::ToolStripPanel^  BottomToolStripPanel;
	private: System::Windows::Forms::ToolStripPanel^  TopToolStripPanel;
	private: System::Windows::Forms::ToolStripPanel^  RightToolStripPanel;
	private: System::Windows::Forms::ToolStripPanel^  LeftToolStripPanel;
	private: System::Windows::Forms::ToolStripContentPanel^  ContentPanel;
	private: System::Windows::Forms::ToolStripMenuItem^  cameraResetToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x320ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  VisibleontrolToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  topmostToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  halfWidthToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  halfHeightToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  backgroundColorToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  textureFilterToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  gLNEARESTToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  gLLINEARToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x600ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  altEnterToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x768_1024_toolStripMenuItem;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	//
	ProjectManager^m_pProjectManager;
	private: System::Windows::Forms::ToolStripMenuItem^  treeFolderCollpaseToolStripMenuItem;
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
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->resolutionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x1080ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x720ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x768ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x768_1024_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x600ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x480ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x320ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->customToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cameraResetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->VisibleontrolToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->topmostToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->halfWidthToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->halfHeightToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->backgroundColorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->textureFilterToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->gLNEARESTToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->gLLINEARToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->altEnterToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->treeFolderCollpaseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->BottomToolStripPanel = (gcnew System::Windows::Forms::ToolStripPanel());
			this->TopToolStripPanel = (gcnew System::Windows::Forms::ToolStripPanel());
			this->RightToolStripPanel = (gcnew System::Windows::Forms::ToolStripPanel());
			this->LeftToolStripPanel = (gcnew System::Windows::Forms::ToolStripPanel());
			this->ContentPanel = (gcnew System::Windows::Forms::ToolStripContentPanel());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->menuStrip1->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Visible;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->resolutionToolStripMenuItem, 
				this->cameraResetToolStripMenuItem, this->VisibleontrolToolStripMenuItem, this->backgroundColorToolStripMenuItem, this->textureFilterToolStripMenuItem, 
				this->altEnterToolStripMenuItem, this->treeFolderCollpaseToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->menuStrip1->Size = System::Drawing::Size(304, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// resolutionToolStripMenuItem
			// 
			this->resolutionToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {this->x1080ToolStripMenuItem, 
				this->toolStripMenuItem, this->x720ToolStripMenuItem, this->x768ToolStripMenuItem, this->x768_1024_toolStripMenuItem, this->x600ToolStripMenuItem, 
				this->x480ToolStripMenuItem, this->x320ToolStripMenuItem, this->customToolStripMenuItem});
			this->resolutionToolStripMenuItem->Name = L"resolutionToolStripMenuItem";
			this->resolutionToolStripMenuItem->Size = System::Drawing::Size(79, 20);
			this->resolutionToolStripMenuItem->Text = L"Resolution";
			// 
			// x1080ToolStripMenuItem
			// 
			this->x1080ToolStripMenuItem->Name = L"x1080ToolStripMenuItem";
			this->x1080ToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->x1080ToolStripMenuItem->Text = L"1920,1080";
			this->x1080ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// toolStripMenuItem
			// 
			this->toolStripMenuItem->Name = L"toolStripMenuItem";
			this->toolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->toolStripMenuItem->Text = L"1280,1024";
			this->toolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// x720ToolStripMenuItem
			// 
			this->x720ToolStripMenuItem->Name = L"x720ToolStripMenuItem";
			this->x720ToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->x720ToolStripMenuItem->Text = L"1280,720";
			this->x720ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// x768ToolStripMenuItem
			// 
			this->x768ToolStripMenuItem->Name = L"x768ToolStripMenuItem";
			this->x768ToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->x768ToolStripMenuItem->Text = L"1024,768";
			this->x768ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// x768_1024_toolStripMenuItem
			// 
			this->x768_1024_toolStripMenuItem->Name = L"x768_1024_toolStripMenuItem";
			this->x768_1024_toolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->x768_1024_toolStripMenuItem->Text = L"768x1024(iPad)";
			this->x768_1024_toolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// x600ToolStripMenuItem
			// 
			this->x600ToolStripMenuItem->Name = L"x600ToolStripMenuItem";
			this->x600ToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->x600ToolStripMenuItem->Text = L"800,600";
			this->x600ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// x480ToolStripMenuItem
			// 
			this->x480ToolStripMenuItem->Name = L"x480ToolStripMenuItem";
			this->x480ToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->x480ToolStripMenuItem->Text = L"640,480";
			this->x480ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// x320ToolStripMenuItem
			// 
			this->x320ToolStripMenuItem->Name = L"x320ToolStripMenuItem";
			this->x320ToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->x320ToolStripMenuItem->Text = L"480,320";
			this->x320ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// customToolStripMenuItem
			// 
			this->customToolStripMenuItem->Name = L"customToolStripMenuItem";
			this->customToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->customToolStripMenuItem->Text = L"Custom";
			this->customToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// cameraResetToolStripMenuItem
			// 
			this->cameraResetToolStripMenuItem->Name = L"cameraResetToolStripMenuItem";
			this->cameraResetToolStripMenuItem->Size = System::Drawing::Size(94, 20);
			this->cameraResetToolStripMenuItem->Text = L"CameraReset";
			this->cameraResetToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::cameraResetToolStripMenuItem_Click);
			// 
			// VisibleontrolToolStripMenuItem
			// 
			this->VisibleontrolToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->topmostToolStripMenuItem, 
				this->halfWidthToolStripMenuItem, this->halfHeightToolStripMenuItem});
			this->VisibleontrolToolStripMenuItem->Name = L"VisibleontrolToolStripMenuItem";
			this->VisibleontrolToolStripMenuItem->Size = System::Drawing::Size(98, 20);
			this->VisibleontrolToolStripMenuItem->Text = L"VisibleControl";
			// 
			// topmostToolStripMenuItem
			// 
			this->topmostToolStripMenuItem->Name = L"topmostToolStripMenuItem";
			this->topmostToolStripMenuItem->Size = System::Drawing::Size(135, 22);
			this->topmostToolStripMenuItem->Text = L"TopMost";
			this->topmostToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::topmostToolStripMenuItem_Click);
			// 
			// halfWidthToolStripMenuItem
			// 
			this->halfWidthToolStripMenuItem->Name = L"halfWidthToolStripMenuItem";
			this->halfWidthToolStripMenuItem->Size = System::Drawing::Size(135, 22);
			this->halfWidthToolStripMenuItem->Text = L"HalfWidth";
			this->halfWidthToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::halfWidthToolStripMenuItem_Click);
			// 
			// halfHeightToolStripMenuItem
			// 
			this->halfHeightToolStripMenuItem->Name = L"halfHeightToolStripMenuItem";
			this->halfHeightToolStripMenuItem->Size = System::Drawing::Size(135, 22);
			this->halfHeightToolStripMenuItem->Text = L"HalfHeight";
			this->halfHeightToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::halfWidthToolStripMenuItem_Click);
			// 
			// backgroundColorToolStripMenuItem
			// 
			this->backgroundColorToolStripMenuItem->Name = L"backgroundColorToolStripMenuItem";
			this->backgroundColorToolStripMenuItem->Size = System::Drawing::Size(118, 20);
			this->backgroundColorToolStripMenuItem->Text = L"BackgroundColor";
			this->backgroundColorToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::backgroundColorToolStripMenuItem_Click);
			// 
			// textureFilterToolStripMenuItem
			// 
			this->textureFilterToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->gLNEARESTToolStripMenuItem, 
				this->gLLINEARToolStripMenuItem});
			this->textureFilterToolStripMenuItem->Name = L"textureFilterToolStripMenuItem";
			this->textureFilterToolStripMenuItem->Size = System::Drawing::Size(88, 20);
			this->textureFilterToolStripMenuItem->Text = L"TextureFilter";
			// 
			// gLNEARESTToolStripMenuItem
			// 
			this->gLNEARESTToolStripMenuItem->Name = L"gLNEARESTToolStripMenuItem";
			this->gLNEARESTToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->gLNEARESTToolStripMenuItem->Text = L"GL_NEAREST";
			this->gLNEARESTToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::gLNEARESTToolStripMenuItem_Click);
			// 
			// gLLINEARToolStripMenuItem
			// 
			this->gLLINEARToolStripMenuItem->Name = L"gLLINEARToolStripMenuItem";
			this->gLLINEARToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->gLLINEARToolStripMenuItem->Text = L"GL_LINEAR";
			this->gLLINEARToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::gLNEARESTToolStripMenuItem_Click);
			// 
			// altEnterToolStripMenuItem
			// 
			this->altEnterToolStripMenuItem->Name = L"altEnterToolStripMenuItem";
			this->altEnterToolStripMenuItem->Size = System::Drawing::Size(72, 20);
			this->altEnterToolStripMenuItem->Text = L"Alt+Enter";
			this->altEnterToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::altEnterToolStripMenuItem_Click);
			// 
			// treeFolderCollpaseToolStripMenuItem
			// 
			this->treeFolderCollpaseToolStripMenuItem->Name = L"treeFolderCollpaseToolStripMenuItem";
			this->treeFolderCollpaseToolStripMenuItem->Size = System::Drawing::Size(129, 20);
			this->treeFolderCollpaseToolStripMenuItem->Text = L"TreeFolderCollpase";
			this->treeFolderCollpaseToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::treeFolderCollpaseToolStripMenuItem_Click);
			// 
			// BottomToolStripPanel
			// 
			this->BottomToolStripPanel->Location = System::Drawing::Point(0, 0);
			this->BottomToolStripPanel->Name = L"BottomToolStripPanel";
			this->BottomToolStripPanel->Orientation = System::Windows::Forms::Orientation::Horizontal;
			this->BottomToolStripPanel->RowMargin = System::Windows::Forms::Padding(3, 0, 0, 0);
			this->BottomToolStripPanel->Size = System::Drawing::Size(0, 0);
			// 
			// TopToolStripPanel
			// 
			this->TopToolStripPanel->Location = System::Drawing::Point(0, 0);
			this->TopToolStripPanel->Name = L"TopToolStripPanel";
			this->TopToolStripPanel->Orientation = System::Windows::Forms::Orientation::Horizontal;
			this->TopToolStripPanel->RowMargin = System::Windows::Forms::Padding(3, 0, 0, 0);
			this->TopToolStripPanel->Size = System::Drawing::Size(0, 0);
			// 
			// RightToolStripPanel
			// 
			this->RightToolStripPanel->Location = System::Drawing::Point(0, 0);
			this->RightToolStripPanel->Name = L"RightToolStripPanel";
			this->RightToolStripPanel->Orientation = System::Windows::Forms::Orientation::Horizontal;
			this->RightToolStripPanel->RowMargin = System::Windows::Forms::Padding(3, 0, 0, 0);
			this->RightToolStripPanel->Size = System::Drawing::Size(0, 0);
			// 
			// LeftToolStripPanel
			// 
			this->LeftToolStripPanel->Location = System::Drawing::Point(0, 0);
			this->LeftToolStripPanel->Name = L"LeftToolStripPanel";
			this->LeftToolStripPanel->Orientation = System::Windows::Forms::Orientation::Horizontal;
			this->LeftToolStripPanel->RowMargin = System::Windows::Forms::Padding(3, 0, 0, 0);
			this->LeftToolStripPanel->Size = System::Drawing::Size(0, 0);
			// 
			// ContentPanel
			// 
			this->ContentPanel->Margin = System::Windows::Forms::Padding(4);
			this->ContentPanel->Size = System::Drawing::Size(1521, 934);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 24);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Panel2Collapsed = true;
			this->splitContainer1->Size = System::Drawing::Size(304, 657);
			this->splitContainer1->SplitterDistance = 192;
			this->splitContainer1->TabIndex = 3;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(304, 681);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->IsMdiContainer = true;
			this->KeyPreview = true;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->WindowState = System::Windows::Forms::FormWindowState::Normal;
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyUp);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyDown);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void x1080ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //if( sender == x1080ToolStripMenuItem )
			 //{
				//DRAW_PANEL_RESOLUTION_WIDTH = 1920.f;
				//DRAW_PANEL_RESOLUTION_HEIGHT = 1080.f;
			 //}
			 //else
			 //if( sender == x720ToolStripMenuItem )
			 //{
				//DRAW_PANEL_RESOLUTION_WIDTH = 1280.f;
				//DRAW_PANEL_RESOLUTION_HEIGHT = 720.f;
			 //}
			 //else
			 //if( sender == x768ToolStripMenuItem )
			 //{
				//DRAW_PANEL_RESOLUTION_WIDTH = 1024.f;
				//DRAW_PANEL_RESOLUTION_HEIGHT = 768.f;
			 //}
			 //else
			 //if( sender == x480ToolStripMenuItem )
			 //{
				//DRAW_PANEL_RESOLUTION_WIDTH = 640.f;
				//DRAW_PANEL_RESOLUTION_HEIGHT = 480.f;
			 //}
			 //else
			 //if( sender == toolStripMenuItem )
			 //{
				//DRAW_PANEL_RESOLUTION_WIDTH = 1280.f;
				//DRAW_PANEL_RESOLUTION_HEIGHT = 1024.f;
			 //}
			 //if( sender == x320ToolStripMenuItem )
			 //{
				//DRAW_PANEL_RESOLUTION_WIDTH = 480.f;
				//DRAW_PANEL_RESOLUTION_HEIGHT = 320.f;
			 //}
			 //else
			 //if( sender == x600ToolStripMenuItem )
			 //{
				//DRAW_PANEL_RESOLUTION_WIDTH = 800.f;
				//DRAW_PANEL_RESOLUTION_HEIGHT = 600.f;
			 //}
			 //else
			 //if( sender == x768_1024_toolStripMenuItem )
			 //{
				//DRAW_PANEL_RESOLUTION_WIDTH = 768.f;
				//DRAW_PANEL_RESOLUTION_HEIGHT = 1024.f;
			 //}
			 //else
			 //if( sender == customToolStripMenuItem )
			 //{
				// ResolutionDialog^l_pResolutionDialog = gcnew ResolutionDialog();
				// l_pResolutionDialog->ShowDialog();
			 //}
			 //g_bResolutionChanged = true;
			 //g_pOrthogonalCamera->SetResolution(Vector2(DRAW_PANEL_RESOLUTION_WIDTH,DRAW_PANEL_RESOLUTION_HEIGHT));
			 //g_pOrthogonalCamera->Reset();
		 }
private: System::Void cameraResetToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			//g_pOrthogonalCamera->Reset();
		 }
private: System::Void topmostToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 topmostToolStripMenuItem->Checked = !topmostToolStripMenuItem->Checked;
			 this->TopMost = topmostToolStripMenuItem->Checked;
			 if( topmostToolStripMenuItem->Checked )
				 topmostToolStripMenuItem->Text = "OnTopmost";
			 else
				 topmostToolStripMenuItem->Text = "NotOnTopmost";

		 }
private: System::Void halfWidthToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->WindowState = GCFORM::FormWindowState::Normal;
			 this->Location = System::Drawing::Point(0,0);
			 if( sender == halfWidthToolStripMenuItem )
			 {
				 this->Width = System::Windows::Forms::Screen::PrimaryScreen->Bounds.Width/2;
				 this->Height = System::Windows::Forms::Screen::PrimaryScreen->Bounds.Height;
			 }
			 else
			 if( sender == halfHeightToolStripMenuItem )
			 {
				 this->Height = System::Windows::Forms::Screen::PrimaryScreen->Bounds.Height/2;
				 this->Width = System::Windows::Forms::Screen::PrimaryScreen->Bounds.Width;
			 }
		 }
private: System::Void backgroundColorToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 GCFORM::ColorDialog^l_pColod = gcnew GCFORM::ColorDialog;
			 if( l_pColod->ShowDialog() == System::Windows::Forms::DialogResult::OK )
			 {
				 //g_vBackgroundColor.x = l_pColod->Color.R/255.f;
				 //g_vBackgroundColor.y = l_pColod->Color.G/255.f;
				 //g_vBackgroundColor.z = l_pColod->Color.B/255.f;
				 //g_vBackgroundColor.w = l_pColod->Color.A/255.f;
			 }
			 
		 }
private: System::Void gLNEARESTToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 System::Windows::Forms::ToolStripMenuItem^l_pSender = (System::Windows::Forms::ToolStripMenuItem^)sender;
			 l_pSender->Checked = true;
			 if( sender == gLNEARESTToolStripMenuItem )
			 {
				g_fMAG_FILTERValue = GL_NEAREST;
				g_fMIN_FILTERValue = GL_NEAREST;
				gLLINEARToolStripMenuItem->Checked = false;
			 }
			 else
			 {
				g_fMAG_FILTERValue = GL_LINEAR;
				g_fMIN_FILTERValue = GL_LINEAR;
				gLNEARESTToolStripMenuItem->Checked = false;
			 }
		 }
private: System::Void Form1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 //System::Diagnostics::Process^p =   System::Diagnostics::Process::Start("notepad.exe");
			 //System::Threading::Thread::Sleep(500); 
			 //SetParent((HWND)p->MainWindowHandle.ToPointer(), (HWND)this->Collpase->Handle.ToPointer());
			 if(e->KeyCode == GCFORM::Keys::F11)
			 {
				 menuStrip1->Visible = !menuStrip1->Visible;
				 altEnterToolStripMenuItem_Click(sender,e);
			 }
			 //if(e->KeyCode == GCFORM::Keys::ControlKey)
			//	g_bCtrlPressed = false;
			 if(e->Control)
			 {
				 //if( e->KeyValue == 107 )//'+'
					//g_fTestValue *=2.f;
				 //else
				 //if( e->KeyValue == 109 )//'-'
					// g_fTestValue /=2.f;
				 if( e->KeyCode == GCFORM::Keys::Enter )
					 altEnterToolStripMenuItem_Click(sender,e);
			 }
		 }
private: System::Void Form1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 //if(e->KeyCode == GCFORM::Keys::ControlKey)
			//	g_bCtrlPressed = true;
		 }
private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
		 {
			 //if( g_bDataChanged )
			 //{
			 //   WARING_YES_NO_TO_NO("do not save changed data?")
				//	e->Cancel = true;
			 //}
		 }
private: System::Void altEnterToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(this->FormBorderStyle == GCFORM::FormBorderStyle::None )
			 {
				 FormBorderStyle = GCFORM::FormBorderStyle::SizableToolWindow;
			 }
			 else
			 {
				 FormBorderStyle = GCFORM::FormBorderStyle::None;
			 }
			 if(this->WindowState != GCFORM::FormWindowState::Maximized )
			 {
				this->WindowState = GCFORM::FormWindowState::Maximized;
			 }
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e)
		 {
			m_pProjectManager = gcnew ProjectManager(this->splitContainer1);
			//m_pProjectManager->NewMPDI(this,e);
		 }
private: System::Void treeFolderCollpaseToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(this->splitContainer1->Panel1Collapsed)
				 this->splitContainer1->Panel1Collapsed = false;
			 else
				 this->splitContainer1->Panel1Collapsed = true;
		 }
};
}

