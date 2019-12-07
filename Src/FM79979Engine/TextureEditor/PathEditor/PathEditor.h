#pragma once

#include "../../Core/GameplayUT/OpenGL/WindowsOpenGL.h"
namespace PathEditor 
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
	public ref class EditPath : public System::Windows::Forms::Form
	{
	public:
		//EditPath(GCFORM::TabControl^e_ptabControl,GCFORM::Form^e_pForm,String^e_strFileName)
		EditPath()
		{
			m_bInitOk = false;
			InitializeComponent();
			//const WCHAR*l_str = cBaseImage::TypeID;
			//this->Dock = GCFORM::DockStyle::Fill;
			//
			//TODO: ¦b¦¹¥[¤J«Øºc¨ç¦¡µ{¦¡½X
			//
			//if( e_ptabControl )
			//{
			//	e_ptabControl->TabPages->Add(m_pTabPage);
			//	m_pTabControl = e_ptabControl;
			//	m_pTabPage = gcnew GCFORM::TabPage();
			//	m_pTabPage->AutoScroll = true;
			//	m_pTabPage->Text = "EditPath/¸ô®|½s¿è";
			//	m_pTabPage->Controls->Add(this);
			//	e_ptabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &EditPath::tabControl_SelectedIndexChanged);
			//}
			//else
			//{
			//	m_pForm = e_pForm;
			//	e_pForm->Controls->Add(this);
			//}
			splitContainer2->Panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &EditPath::MyMouseMove);
			splitContainer2->Panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &EditPath::MyMouseDown);
			splitContainer2->Panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &EditPath::MyMouseUp);
			splitContainer2->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &EditPath::MyMouseUp);
			//splitContainer2->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &EditPath::MyKeyUp);
			//PathMode_comboBox->SelectedIndex = 0;

			m_bCtrlPressed = false;
			m_pOrthogonalCamera = new cOrthogonalCamera();
			m_pBaseImageListEP = new cNamedTypedObjectVector<cBaseImage>;
			m_pCurveManagerEP = new cCurveManager();
			m_pTimeAndFPS = new sTimeAndFPS;
			m_pvResolution = new Vector2(1920.f,1080.f);
			m_pvBGColor = new Vector4(0.f,0.f,0.f,1.f);

			m_pSplitScreenCamera = new cSplitScreenCamera();
			if (m_pSplitScreenCamera && m_pSplitScreenCamera->m_sAxisCamera)
			{
				int l_iCameraAxisIndex = m_pSplitScreenCamera->GetFocusPanelIndex();
				auto l_pSplitScreenCamera = m_pSplitScreenCamera;
				auto l_pAxisCamera2 = m_pSplitScreenCamera->m_sAxisCamera;
				auto l_pAxisCamera = l_pAxisCamera2[0];
			}

			LanguageSwitch(splitContainer1->Panel1->Controls,"/",this->Handle);
			LanguageSwitch(this->Controls,"/",this->Handle);
			m_HdcMV = GetDC((HWND)this->splitContainer2->Panel1->Handle.ToPointer());
			m_HGLRCMV = InitOpenGL((HWND)this->splitContainer2->Panel1->Handle.ToPointer(),true,m_HdcMV);
			//InitMultisample((HINSTANCE)System::Runtime::InteropServices::Marshal::GetHINSTANCE(splitContainer2->Panel1->GetType()->Module).ToPointer(),(HWND)this->splitContainer2->Panel1->Handle.ToPointer());
			//m_HdcMV = GetDC((HWND)splitContainer2->Panel1->Handle.ToPointer());
			wglMakeCurrent( m_HdcMV,m_HGLRCMV );
			//AutoScaleChildrenForm^l_pAutoScaleChildrenForm = gcnew AutoScaleChildrenForm(m_pTabPage,this);
			m_pDebugFont = 0;
			if(System::IO::File::Exists("Font.png"))
			{
				m_pDebugFont = new cGlyphFontRender("Font");
				if( !m_pDebugFont->GetFontImage() )
					SAFE_DELETE(m_pDebugFont);
			}
			m_iCurrentSelectedPathListIndex = -1;
			CreateShader(g_bCommonVSClientState,L"EditPathShader");
			CreateShader(g_bCommonVSNoTextureClientState,L"EditPath_NoTextureShader");
			wglMakeCurrent( m_HdcMV,m_HGLRCMV );
			m_bInitOk = true;
			//this->timer1->Enabled = true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EditPath()
		{
			if (components)
			{
				delete components;
			}
			DeleteShader(L"EditPathShader");
			DeleteShader(L"EditPath_NoTextureShader");
			SAFE_DELETE(m_pvBGColor);
			SAFE_DELETE(m_pTimeAndFPS);
			SAFE_DELETE(m_pDebugFont);
			SAFE_DELETE(m_pSplitScreenCamera);
			SAFE_DELETE(m_pCurveManagerEP);
			SAFE_DELETE(m_pBKImageEP);
			SAFE_DELETE(m_pBaseImageListEP);
			SAFE_DELETE(m_pvResolution);
			SAFE_DELETE(m_pOrthogonalCamera);
		}

	private: System::Void	RefershPathData();
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::CheckBox^  ShowImage_checkBox;
	private: System::Windows::Forms::ListBox^  ImageName_listBox;
	private: System::Windows::Forms::Button^  DelPoint_button;
	private: System::Windows::Forms::ComboBox^  PathMode_comboBox;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::NumericUpDown^  TotalPlayTime_numericUpDown;
	private: System::Windows::Forms::CheckBox^  Play_checkBox;
	private: System::Windows::Forms::CheckBox^  Loop_checkBox;
	private: System::Windows::Forms::Label^  ErrorMsg_label;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  PathLOD_numericUpDown;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  PathNodePosX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  PathNodePosY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  PathNodeTime_numericUpDown;
	private: System::Windows::Forms::ListBox^  PathNode_listBox;
	private: System::Windows::Forms::Button^  DelPath_button;
	private: System::Windows::Forms::ListBox^  WholePath_listBox;
	private: System::Windows::Forms::Button^  AddPath_button;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  PathEditFileOpen_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mergeToolStripMenuItem;
	private: System::Windows::Forms::Button^  BackgrioundImage_button;
	private: System::Windows::Forms::Button^  AddImage_button;
	private: System::Windows::Forms::Button^  InsertPoint_button;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::ToolStripMenuItem^  cameraToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  perspectiveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  orthgnalToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  hideEditPanelToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showSplitScreenToolStripMenuItem;
	private: System::Windows::Forms::NumericUpDown^  PathNodePosZ_numericUpDown;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::ToolStripMenuItem^  resetToolStripMenuItem;
	private: System::Windows::Forms::Button^  LoadModel_button;
	private: System::Windows::Forms::ToolStripMenuItem^  showPlaneToolStripMenuItem;
	//my
			 bool	m_bInitOk;
	private: GCFORM::Form^m_pForm;
	private: GCFORM::TabControl^m_pTabControl;				//to determin is tabpage clicked.
	private: GCFORM::TabPage^m_pTabPage;
			HGLRC	 m_HGLRCMV;
			HDC		 m_HdcMV;
			cGlyphFontRender*m_pDebugFont;
			cOrthogonalCamera*				m_pOrthogonalCamera;
			cCurveManager*					m_pCurveManagerEP;
			cBaseImage*						m_pBKImageEP;
			cNamedTypedObjectVector<cBaseImage>*	m_pBaseImageListEP;
			int								m_iCurrentSelectedPathListIndex;
			UT::sTimeAndFPS					*m_pTimeAndFPS;
			cSplitScreenCamera*	m_pSplitScreenCamera;
			bool				m_bCtrlPressed;
			Vector4*			m_pvBGColor;
			Vector2*			m_pvResolution;
	void	RenderMousePos();
	void	RenderSplit();
	//void	RenderSplitFrame();
	void	Render(cFrameCamera*e_pFrameCamera,cOrthogonalCamera*e_pOrthogonalCamera,POINT e_ViewportSize);
//for PathEdit
	private: System::Void AddPath_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void WholePath_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void PathNodePosX_numericUpDown_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void PathNodeTime_numericUpDown_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void PathLOD_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ColorR_numericUpDown_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void ColorR_numericUpDown_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void PathMode_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void DelPoint_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Play_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Loop_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void TotalPlayTime_numericUpDown_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void TotalPlayTime_numericUpDown_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void PathEditFileOpen_ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	void	PathEditElementControl(bool e_b);
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
			this->ShowImage_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ImageName_listBox = (gcnew System::Windows::Forms::ListBox());
			this->DelPoint_button = (gcnew System::Windows::Forms::Button());
			this->PathMode_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->TotalPlayTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Play_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Loop_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ErrorMsg_label = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->PathLOD_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->PathNodePosX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->PathNodePosY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->PathNodeTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->PathNode_listBox = (gcnew System::Windows::Forms::ListBox());
			this->DelPath_button = (gcnew System::Windows::Forms::Button());
			this->WholePath_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AddPath_button = (gcnew System::Windows::Forms::Button());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->PathEditFileOpen_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mergeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cameraToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->perspectiveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->orthgnalToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->hideEditPanelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showSplitScreenToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showPlaneToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->BackgrioundImage_button = (gcnew System::Windows::Forms::Button());
			this->AddImage_button = (gcnew System::Windows::Forms::Button());
			this->InsertPoint_button = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->LoadModel_button = (gcnew System::Windows::Forms::Button());
			this->PathNodePosZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TotalPlayTime_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathLOD_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodeTime_numericUpDown))->BeginInit();
			this->menuStrip1->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosZ_numericUpDown))->BeginInit();
			this->splitContainer2->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 16;
			this->timer1->Tick += gcnew System::EventHandler(this, &EditPath::timer1_Tick);
			// 
			// ShowImage_checkBox
			// 
			this->ShowImage_checkBox->AutoSize = true;
			this->ShowImage_checkBox->Checked = true;
			this->ShowImage_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ShowImage_checkBox->Location = System::Drawing::Point(145, -14);
			this->ShowImage_checkBox->Name = L"ShowImage_checkBox";
			this->ShowImage_checkBox->Size = System::Drawing::Size(130, 16);
			this->ShowImage_checkBox->TabIndex = 59;
			this->ShowImage_checkBox->Text = L"ShowImage/Åã¥Ü¹Ï¤ù";
			this->ShowImage_checkBox->UseVisualStyleBackColor = true;
			// 
			// ImageName_listBox
			// 
			this->ImageName_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)), 
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->ImageName_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageName_listBox->FormattingEnabled = true;
			this->ImageName_listBox->ItemHeight = 12;
			this->ImageName_listBox->Location = System::Drawing::Point(145, 50);
			this->ImageName_listBox->MinimumSize = System::Drawing::Size(4, 74);
			this->ImageName_listBox->Name = L"ImageName_listBox";
			this->ImageName_listBox->Size = System::Drawing::Size(170, 148);
			this->ImageName_listBox->TabIndex = 58;
			this->ImageName_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditPath::ImageName_listBox_SelectedIndexChanged);
			// 
			// DelPoint_button
			// 
			this->DelPoint_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelPoint_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelPoint_button->Location = System::Drawing::Point(568, 28);
			this->DelPoint_button->Name = L"DelPoint_button";
			this->DelPoint_button->Size = System::Drawing::Size(114, 27);
			this->DelPoint_button->TabIndex = 56;
			this->DelPoint_button->Text = L"DelPoint/§R°£¸ÓÂI";
			this->DelPoint_button->UseVisualStyleBackColor = false;
			this->DelPoint_button->Click += gcnew System::EventHandler(this, &EditPath::DelPoint_button_Click);
			// 
			// PathMode_comboBox
			// 
			this->PathMode_comboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathMode_comboBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathMode_comboBox->FormattingEnabled = true;
			this->PathMode_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"AddPointMode", L"FixPointMode"});
			this->PathMode_comboBox->Location = System::Drawing::Point(390, 2);
			this->PathMode_comboBox->Name = L"PathMode_comboBox";
			this->PathMode_comboBox->Size = System::Drawing::Size(101, 20);
			this->PathMode_comboBox->TabIndex = 55;
			this->PathMode_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditPath::PathMode_comboBox_SelectedIndexChanged);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label8->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label8->Location = System::Drawing::Point(633, 166);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(124, 12);
			this->label8->TabIndex = 54;
			this->label8->Text = L"TotalPlayTime/¥þ³¡®É¶¡";
			// 
			// TotalPlayTime_numericUpDown
			// 
			this->TotalPlayTime_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->TotalPlayTime_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->TotalPlayTime_numericUpDown->Location = System::Drawing::Point(765, 165);
			this->TotalPlayTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999999, 0, 0, 0});
			this->TotalPlayTime_numericUpDown->Name = L"TotalPlayTime_numericUpDown";
			this->TotalPlayTime_numericUpDown->Size = System::Drawing::Size(96, 22);
			this->TotalPlayTime_numericUpDown->TabIndex = 53;
			this->TotalPlayTime_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &EditPath::TotalPlayTime_numericUpDown_KeyUp);
			this->TotalPlayTime_numericUpDown->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &EditPath::TotalPlayTime_numericUpDown_MouseUp);
			// 
			// Play_checkBox
			// 
			this->Play_checkBox->AutoSize = true;
			this->Play_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Play_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Play_checkBox->Location = System::Drawing::Point(12, 200);
			this->Play_checkBox->Name = L"Play_checkBox";
			this->Play_checkBox->Size = System::Drawing::Size(71, 16);
			this->Play_checkBox->TabIndex = 52;
			this->Play_checkBox->Text = L"Play/¼·©ñ";
			this->Play_checkBox->UseVisualStyleBackColor = false;
			this->Play_checkBox->CheckedChanged += gcnew System::EventHandler(this, &EditPath::Play_checkBox_CheckedChanged);
			// 
			// Loop_checkBox
			// 
			this->Loop_checkBox->AutoSize = true;
			this->Loop_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Loop_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Loop_checkBox->Location = System::Drawing::Point(12, 178);
			this->Loop_checkBox->Name = L"Loop_checkBox";
			this->Loop_checkBox->Size = System::Drawing::Size(76, 16);
			this->Loop_checkBox->TabIndex = 45;
			this->Loop_checkBox->Text = L"Loop/µL­­";
			this->Loop_checkBox->UseVisualStyleBackColor = false;
			this->Loop_checkBox->CheckedChanged += gcnew System::EventHandler(this, &EditPath::Loop_checkBox_CheckedChanged);
			// 
			// ErrorMsg_label
			// 
			this->ErrorMsg_label->AutoSize = true;
			this->ErrorMsg_label->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ErrorMsg_label->Location = System::Drawing::Point(557, 121);
			this->ErrorMsg_label->Name = L"ErrorMsg_label";
			this->ErrorMsg_label->Size = System::Drawing::Size(0, 12);
			this->ErrorMsg_label->TabIndex = 44;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label4->Location = System::Drawing::Point(10, 157);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(48, 12);
			this->label4->TabIndex = 43;
			this->label4->Text = L"PathLOD";
			// 
			// PathLOD_numericUpDown
			// 
			this->PathLOD_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathLOD_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathLOD_numericUpDown->Location = System::Drawing::Point(64, 150);
			this->PathLOD_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {7, 0, 0, 0});
			this->PathLOD_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->PathLOD_numericUpDown->Name = L"PathLOD_numericUpDown";
			this->PathLOD_numericUpDown->Size = System::Drawing::Size(75, 22);
			this->PathLOD_numericUpDown->TabIndex = 42;
			this->PathLOD_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->PathLOD_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &EditPath::PathLOD_numericUpDown_ValueChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label3->Location = System::Drawing::Point(722, 52);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(29, 12);
			this->label3->TabIndex = 41;
			this->label3->Text = L"PosX";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label2->Location = System::Drawing::Point(722, 80);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 12);
			this->label2->TabIndex = 40;
			this->label2->Text = L"PosY";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label1->Location = System::Drawing::Point(674, 138);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(80, 12);
			this->label1->TabIndex = 39;
			this->label1->Text = L"Time/¸ÓÂI®É¶¡";
			// 
			// PathNodePosX_numericUpDown
			// 
			this->PathNodePosX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNodePosX_numericUpDown->DecimalPlaces = 3;
			this->PathNodePosX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNodePosX_numericUpDown->Location = System::Drawing::Point(765, 50);
			this->PathNodePosX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->PathNodePosX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
			this->PathNodePosX_numericUpDown->Name = L"PathNodePosX_numericUpDown";
			this->PathNodePosX_numericUpDown->Size = System::Drawing::Size(96, 22);
			this->PathNodePosX_numericUpDown->TabIndex = 38;
			this->PathNodePosX_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &EditPath::PathNodePosX_numericUpDown_KeyUp);
			this->PathNodePosX_numericUpDown->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &EditPath::PathNodeTime_numericUpDown_MouseUp);
			// 
			// PathNodePosY_numericUpDown
			// 
			this->PathNodePosY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNodePosY_numericUpDown->DecimalPlaces = 3;
			this->PathNodePosY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNodePosY_numericUpDown->Location = System::Drawing::Point(765, 78);
			this->PathNodePosY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->PathNodePosY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
			this->PathNodePosY_numericUpDown->Name = L"PathNodePosY_numericUpDown";
			this->PathNodePosY_numericUpDown->Size = System::Drawing::Size(96, 22);
			this->PathNodePosY_numericUpDown->TabIndex = 37;
			this->PathNodePosY_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &EditPath::PathNodePosX_numericUpDown_KeyUp);
			this->PathNodePosY_numericUpDown->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &EditPath::PathNodeTime_numericUpDown_MouseUp);
			// 
			// PathNodeTime_numericUpDown
			// 
			this->PathNodeTime_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNodeTime_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNodeTime_numericUpDown->Location = System::Drawing::Point(765, 136);
			this->PathNodeTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000000, 0, 0, 0});
			this->PathNodeTime_numericUpDown->Name = L"PathNodeTime_numericUpDown";
			this->PathNodeTime_numericUpDown->Size = System::Drawing::Size(96, 22);
			this->PathNodeTime_numericUpDown->TabIndex = 36;
			this->PathNodeTime_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &EditPath::PathNodePosX_numericUpDown_KeyUp);
			this->PathNodeTime_numericUpDown->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &EditPath::PathNodeTime_numericUpDown_MouseUp);
			// 
			// PathNode_listBox
			// 
			this->PathNode_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)), 
				static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->PathNode_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNode_listBox->FormattingEnabled = true;
			this->PathNode_listBox->ItemHeight = 12;
			this->PathNode_listBox->Location = System::Drawing::Point(333, 26);
			this->PathNode_listBox->MinimumSize = System::Drawing::Size(4, 86);
			this->PathNode_listBox->Name = L"PathNode_listBox";
			this->PathNode_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->PathNode_listBox->Size = System::Drawing::Size(224, 172);
			this->PathNode_listBox->TabIndex = 35;
			this->PathNode_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditPath::WholePath_listBox_SelectedIndexChanged);
			// 
			// DelPath_button
			// 
			this->DelPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelPath_button->Location = System::Drawing::Point(77, 103);
			this->DelPath_button->Name = L"DelPath_button";
			this->DelPath_button->Size = System::Drawing::Size(58, 40);
			this->DelPath_button->TabIndex = 34;
			this->DelPath_button->Text = L"DelPath/§R°£¸ô®|";
			this->DelPath_button->UseVisualStyleBackColor = false;
			this->DelPath_button->Click += gcnew System::EventHandler(this, &EditPath::AddPath_button_Click);
			// 
			// WholePath_listBox
			// 
			this->WholePath_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)), 
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->WholePath_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholePath_listBox->FormattingEnabled = true;
			this->WholePath_listBox->ItemHeight = 12;
			this->WholePath_listBox->Location = System::Drawing::Point(12, 12);
			this->WholePath_listBox->MinimumSize = System::Drawing::Size(4, 45);
			this->WholePath_listBox->Name = L"WholePath_listBox";
			this->WholePath_listBox->Size = System::Drawing::Size(127, 88);
			this->WholePath_listBox->TabIndex = 32;
			this->WholePath_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EditPath::WholePath_listBox_SelectedIndexChanged);
			// 
			// AddPath_button
			// 
			this->AddPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddPath_button->Location = System::Drawing::Point(12, 103);
			this->AddPath_button->Name = L"AddPath_button";
			this->AddPath_button->Size = System::Drawing::Size(59, 40);
			this->AddPath_button->TabIndex = 33;
			this->AddPath_button->Text = L"AddPath/¼W¥[¸ô®|";
			this->AddPath_button->UseVisualStyleBackColor = false;
			this->AddPath_button->Click += gcnew System::EventHandler(this, &EditPath::AddPath_button_Click);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->fileToolStripMenuItem, 
				this->cameraToolStripMenuItem, this->viewToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1273, 24);
			this->menuStrip1->TabIndex = 57;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->PathEditFileOpen_ToolStripMenuItem, 
				this->saveToolStripMenuItem, this->mergeToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(34, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// PathEditFileOpen_ToolStripMenuItem
			// 
			this->PathEditFileOpen_ToolStripMenuItem->Name = L"PathEditFileOpen_ToolStripMenuItem";
			this->PathEditFileOpen_ToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->PathEditFileOpen_ToolStripMenuItem->Text = L"Open";
			this->PathEditFileOpen_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditPath::PathEditFileOpen_ToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditPath::PathEditFileOpen_ToolStripMenuItem_Click);
			// 
			// mergeToolStripMenuItem
			// 
			this->mergeToolStripMenuItem->Name = L"mergeToolStripMenuItem";
			this->mergeToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->mergeToolStripMenuItem->Text = L"Merge";
			this->mergeToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditPath::PathEditFileOpen_ToolStripMenuItem_Click);
			// 
			// cameraToolStripMenuItem
			// 
			this->cameraToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->perspectiveToolStripMenuItem, 
				this->orthgnalToolStripMenuItem, this->resetToolStripMenuItem});
			this->cameraToolStripMenuItem->Name = L"cameraToolStripMenuItem";
			this->cameraToolStripMenuItem->Size = System::Drawing::Size(53, 20);
			this->cameraToolStripMenuItem->Text = L"Camera";
			// 
			// perspectiveToolStripMenuItem
			// 
			this->perspectiveToolStripMenuItem->Name = L"perspectiveToolStripMenuItem";
			this->perspectiveToolStripMenuItem->Size = System::Drawing::Size(122, 22);
			this->perspectiveToolStripMenuItem->Text = L"Perspective";
			this->perspectiveToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditPath::orthgnalToolStripMenuItem_Click);
			// 
			// orthgnalToolStripMenuItem
			// 
			this->orthgnalToolStripMenuItem->Checked = true;
			this->orthgnalToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->orthgnalToolStripMenuItem->Name = L"orthgnalToolStripMenuItem";
			this->orthgnalToolStripMenuItem->Size = System::Drawing::Size(122, 22);
			this->orthgnalToolStripMenuItem->Text = L"Orthgnal";
			this->orthgnalToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditPath::orthgnalToolStripMenuItem_Click);
			// 
			// resetToolStripMenuItem
			// 
			this->resetToolStripMenuItem->Name = L"resetToolStripMenuItem";
			this->resetToolStripMenuItem->Size = System::Drawing::Size(122, 22);
			this->resetToolStripMenuItem->Text = L"Reset";
			this->resetToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditPath::CameraReset_button_Click);
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->hideEditPanelToolStripMenuItem, 
				this->showSplitScreenToolStripMenuItem, this->showPlaneToolStripMenuItem});
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->viewToolStripMenuItem->Text = L"View";
			// 
			// hideEditPanelToolStripMenuItem
			// 
			this->hideEditPanelToolStripMenuItem->Name = L"hideEditPanelToolStripMenuItem";
			this->hideEditPanelToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->hideEditPanelToolStripMenuItem->Text = L"HideEditPanel";
			this->hideEditPanelToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditPath::hideEditPanelToolStripMenuItem_Click);
			// 
			// showSplitScreenToolStripMenuItem
			// 
			this->showSplitScreenToolStripMenuItem->Name = L"showSplitScreenToolStripMenuItem";
			this->showSplitScreenToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->showSplitScreenToolStripMenuItem->Text = L"ShowSplitScreen";
			this->showSplitScreenToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditPath::hideEditPanelToolStripMenuItem_Click);
			// 
			// showPlaneToolStripMenuItem
			// 
			this->showPlaneToolStripMenuItem->Name = L"showPlaneToolStripMenuItem";
			this->showPlaneToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->showPlaneToolStripMenuItem->Text = L"ShowPlane";
			this->showPlaneToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditPath::hideEditPanelToolStripMenuItem_Click);
			// 
			// BackgrioundImage_button
			// 
			this->BackgrioundImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->BackgrioundImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->BackgrioundImage_button->Location = System::Drawing::Point(145, 7);
			this->BackgrioundImage_button->Name = L"BackgrioundImage_button";
			this->BackgrioundImage_button->Size = System::Drawing::Size(99, 41);
			this->BackgrioundImage_button->TabIndex = 60;
			this->BackgrioundImage_button->Text = L"BackgroundImage/­I´º¹Ï¤ù";
			this->BackgrioundImage_button->UseVisualStyleBackColor = false;
			this->BackgrioundImage_button->Click += gcnew System::EventHandler(this, &EditPath::BackgrioundImage_button_Click);
			// 
			// AddImage_button
			// 
			this->AddImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddImage_button->Location = System::Drawing::Point(250, 7);
			this->AddImage_button->Name = L"AddImage_button";
			this->AddImage_button->Size = System::Drawing::Size(65, 41);
			this->AddImage_button->TabIndex = 61;
			this->AddImage_button->Text = L"AddImage/¼W¥[¹Ï¤ù";
			this->AddImage_button->UseVisualStyleBackColor = false;
			this->AddImage_button->Click += gcnew System::EventHandler(this, &EditPath::AddImage_button_Click);
			// 
			// InsertPoint_button
			// 
			this->InsertPoint_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->InsertPoint_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->InsertPoint_button->Location = System::Drawing::Point(568, 73);
			this->InsertPoint_button->Name = L"InsertPoint_button";
			this->InsertPoint_button->Size = System::Drawing::Size(114, 27);
			this->InsertPoint_button->TabIndex = 62;
			this->InsertPoint_button->Text = L"InsertPoint/“V¤J¤@ÂI";
			this->InsertPoint_button->UseVisualStyleBackColor = false;
			this->InsertPoint_button->Click += gcnew System::EventHandler(this, &EditPath::DelPoint_button_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label5->Location = System::Drawing::Point(331, 4);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(59, 12);
			this->label5->TabIndex = 63;
			this->label5->Text = L"Mode/¼Ò¦¡";
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 24);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->splitContainer1->Panel1->Controls->Add(this->LoadModel_button);
			this->splitContainer1->Panel1->Controls->Add(this->PathNodePosZ_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->label6);
			this->splitContainer1->Panel1->Controls->Add(this->WholePath_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->label5);
			this->splitContainer1->Panel1->Controls->Add(this->AddPath_button);
			this->splitContainer1->Panel1->Controls->Add(this->InsertPoint_button);
			this->splitContainer1->Panel1->Controls->Add(this->DelPath_button);
			this->splitContainer1->Panel1->Controls->Add(this->AddImage_button);
			this->splitContainer1->Panel1->Controls->Add(this->PathNode_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->BackgrioundImage_button);
			this->splitContainer1->Panel1->Controls->Add(this->PathNodeTime_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->ShowImage_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->PathNodePosY_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->ImageName_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->PathNodePosX_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->DelPoint_button);
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->PathMode_comboBox);
			this->splitContainer1->Panel1->Controls->Add(this->label2);
			this->splitContainer1->Panel1->Controls->Add(this->label8);
			this->splitContainer1->Panel1->Controls->Add(this->label3);
			this->splitContainer1->Panel1->Controls->Add(this->TotalPlayTime_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->PathLOD_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->Play_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->label4);
			this->splitContainer1->Panel1->Controls->Add(this->Loop_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->ErrorMsg_label);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
			this->splitContainer1->Size = System::Drawing::Size(1273, 590);
			this->splitContainer1->SplitterDistance = 221;
			this->splitContainer1->SplitterWidth = 3;
			this->splitContainer1->TabIndex = 64;
			// 
			// LoadModel_button
			// 
			this->LoadModel_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->LoadModel_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->LoadModel_button->Location = System::Drawing::Point(879, 19);
			this->LoadModel_button->Name = L"LoadModel_button";
			this->LoadModel_button->Size = System::Drawing::Size(75, 45);
			this->LoadModel_button->TabIndex = 67;
			this->LoadModel_button->Text = L"LoadModel";
			this->LoadModel_button->UseVisualStyleBackColor = false;
			this->LoadModel_button->Click += gcnew System::EventHandler(this, &EditPath::LoadModel_button_Click);
			// 
			// PathNodePosZ_numericUpDown
			// 
			this->PathNodePosZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNodePosZ_numericUpDown->DecimalPlaces = 3;
			this->PathNodePosZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNodePosZ_numericUpDown->Location = System::Drawing::Point(765, 103);
			this->PathNodePosZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->PathNodePosZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
			this->PathNodePosZ_numericUpDown->Name = L"PathNodePosZ_numericUpDown";
			this->PathNodePosZ_numericUpDown->Size = System::Drawing::Size(96, 22);
			this->PathNodePosZ_numericUpDown->TabIndex = 65;
			this->PathNodePosZ_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &EditPath::PathNodePosX_numericUpDown_KeyUp);
			this->PathNodePosZ_numericUpDown->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &EditPath::PathNodeTime_numericUpDown_MouseUp);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label6->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label6->Location = System::Drawing::Point(722, 105);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(28, 12);
			this->label6->TabIndex = 66;
			this->label6->Text = L"PosZ";
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Panel2Collapsed = true;
			this->splitContainer2->Size = System::Drawing::Size(1269, 362);
			this->splitContainer2->SplitterDistance = 486;
			this->splitContainer2->SplitterWidth = 3;
			this->splitContainer2->TabIndex = 0;
			// 
			// EditPath
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->Name = L"EditPath";
			this->Size = System::Drawing::Size(1273, 614);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TotalPlayTime_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathLOD_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodeTime_numericUpDown))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosZ_numericUpDown))->EndInit();
			this->splitContainer2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( !m_bInitOk )
				 return;
			 if( !this->Visible )
			 {
				 ::Threading::Thread::Sleep(100);
			 }
			wglMakeCurrent( m_HdcMV,m_HGLRCMV );
			DEFAULT_SHADER = L"EditPathShader";
			NO_TEXTURE_SHADER = L"EditPath_NoTextureShader";
			UseShaderProgram();
			//glViewport(0,0,splitContainer2->Panel1->Width,splitContainer2->Panel1->Height);
			//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
			//glClearColor( g_vBackgroundColor.x,g_vBackgroundColor.y,g_vBackgroundColor.z,g_vBackgroundColor.w );
			//glClearDepth(1.0f);											// Depth Buffer Setup
			//glEnable2D(1920,1080);
			//POINT	l_tt;
			//l_tt.x = l_tt.y = 0;

			//GLRender::RenderRectangle(l_tt,600,480,Vector4(1,1,1,1));
			//RenderArrow(Vector3(300,300,0),Vector3(100,0,0),6,Vector4(0,1,1,1));
			//RenderPoint(Vector2(100,100),15,150);

			//glDisable2D();
			//SwapBuffers (m_HdcMV);

			// return;
			int l_iCameraAxisIndex = m_pSplitScreenCamera->GetFocusPanelIndex();
			auto l_pAxisCamera = m_pSplitScreenCamera->m_sAxisCamera[l_iCameraAxisIndex];
			cCameraBehaveByMouseBehave*l_pCameraBehaveByMouseBehave = m_pSplitScreenCamera->m_sAxisCamera[l_iCameraAxisIndex].pCameraBehaveByMouseBehave;
			l_pCameraBehaveByMouseBehave->SetMouseMoveSpeed(1.f);
			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
			glClearColor( m_pvBGColor->x,m_pvBGColor->y,m_pvBGColor->z,m_pvBGColor->w );
			glClearDepth(1.0f);											// Depth Buffer Setup
			m_pTimeAndFPS->Update();
			if(showSplitScreenToolStripMenuItem->Checked)
			{
				RenderSplit();
			}
			else
			{
				//Vector3	l_vMousePos(m_pSplitScreenCamera->m_sAxisCamera[3].OrthogonalCamera.GetMouseWorldPos().x,
				//m_pSplitScreenCamera->m_sAxisCamera[3].OrthogonalCamera.GetMouseWorldPos().y,
				//m_pSplitScreenCamera->m_sAxisCamera[0].OrthogonalCamera.GetMouseWorldPos().x);
				cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 0.f;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 0.f;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)splitContainer2->Panel1->Width;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)splitContainer2->Panel1->Height;
				cGameApp::ApplyViewPort();
				POINT	l_VeiewPort = {splitContainer2->Panel1->Width,splitContainer2->Panel1->Height};
				//glDepthMask(GL_FALSE);
				//glDisable(GL_DEPTH_TEST);
				//glDepthMask(GL_TRUE);
				m_pSplitScreenCamera->Render(l_VeiewPort);
				//glDisable(GL_DEPTH_TEST);
				//glEnable(GL_DEPTH_TEST);
				Render(&m_pSplitScreenCamera->m_sAxisCamera[3].FrameCamera,&m_pSplitScreenCamera->m_sAxisCamera[3].OrthogonalCamera,l_VeiewPort);
				RenderMousePos();
			}
			SwapBuffers (m_HdcMV);
			//wglMakeCurrent( 0,0 );
		 }
	private: System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 if(!timer1->Enabled)
					return;
				GCFORM::MouseButtons l_MouseButton = e->Button;
				bool	l_bLeftMouseButton = l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false;
				if(this->m_pSplitScreenCamera->m_eProjectionType ==Projection::Perspective&&!m_bCtrlPressed)
				{
					l_bLeftMouseButton = false;
				}
				Vector2	l_vVeiewPort((float)splitContainer2->Panel1->Width,(float)splitContainer2->Panel1->Height);
				m_pSplitScreenCamera->MouseEvent(l_bLeftMouseButton,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,l_vVeiewPort,m_pTimeAndFPS->fElpaseTime);
				 if( perspectiveToolStripMenuItem->Checked )
				 {
					int	l_iWidth = splitContainer2->Panel1->Width;
					int	l_iHeight = splitContainer2->Panel1->Height;
					int	l_ViewPort[4];
					l_ViewPort[0] = 0;
					l_ViewPort[1] = 0;
					if( m_pSplitScreenCamera->m_bShowSplitScreen )
					{
						l_ViewPort[2] = l_iWidth/2;
						l_ViewPort[3] = l_iHeight/2;
					}
					else
					{
						l_ViewPort[2] = l_iWidth;
						l_ViewPort[3] = l_iHeight;
					}
					cFrameCamera*l_pFrameCamera = &m_pSplitScreenCamera->m_sAxisCamera[m_pSplitScreenCamera->GetFocusPanelIndex()].FrameCamera;
					Vector3	l_vMouseWorldPos = m_pSplitScreenCamera->GetCurrentMousePos();
					m_pCurveManagerEP->MouseMove(l_vMouseWorldPos,MyMath::W,l_pFrameCamera->GetProjection().GetMatrix(),l_pFrameCamera->GetWorldView(),l_ViewPort);
				 }
				 else
				 {
					if( PathMode_comboBox->SelectedIndex == 1 )//1 for fix point, 0 for add point
					{
						Vector3	l_vMouseWorldPos = m_pSplitScreenCamera->GetCurrentMousePos();
						int	l_iIndex = m_pSplitScreenCamera->GetFocusPanelIndex();
						if(l_iIndex>2)
							l_iIndex = 2;
						m_pCurveManagerEP->MouseMove(l_vMouseWorldPos,(MyMath::AXIS)l_iIndex);
					}
				 }
			 }

	private: System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 if(!timer1->Enabled)
					return;
				 splitContainer2->Panel1->Focus();
				 GCFORM::MouseButtons l_MouseButton = e->Button;
				 Vector2	l_vVeiewPort((float)splitContainer2->Panel1->Width,(float)splitContainer2->Panel1->Height);
					m_pSplitScreenCamera->MouseEvent(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
						,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,l_vVeiewPort,m_pTimeAndFPS->fElpaseTime);
				 if( e->Button != GCFORM::MouseButtons::Left )
					return;
				 Vector3	l_vMouseWorldPos = m_pSplitScreenCamera->GetCurrentMousePos();
				 if( perspectiveToolStripMenuItem->Checked )
				 {
					if(m_bCtrlPressed)
						return;
					cFrameCamera*l_pFrameCamera = &m_pSplitScreenCamera->m_sAxisCamera[m_pSplitScreenCamera->GetFocusPanelIndex()].FrameCamera;
					if( PathMode_comboBox->SelectedIndex != 1 )//1 for fix point, 0 for add point
					{
						if(WholePath_listBox->SelectedIndex!=-1)
						{
							if(this->m_pSplitScreenCamera->m_bShowSplitScreen)
								l_vVeiewPort *= 0.5f;
							//l_vMouseWorldPos.z = (l_vMouseWorldPos.y/l_vVeiewPort.y);
							l_vMouseWorldPos.z = 1-(l_pFrameCamera->GetWorldPosition().Length()/l_pFrameCamera->GetProjection().GetZFar());
							Vector3	l_vNewPos = ScreenToWorld(l_vMouseWorldPos,l_pFrameCamera->GetProjection().GetMatrix()*l_pFrameCamera->GetWorldView(),l_vVeiewPort);
							//l_vNewPos -= l_pFrameCamera->GetWorldDirection()*l_pFrameCamera->GetWorldPosition().Length();
							m_pCurveManagerEP->GetObject(WholePath_listBox->SelectedIndex)->AddPoint(l_vNewPos,0);
							System::EventArgs^  e3;
							WholePath_listBox_SelectedIndexChanged(WholePath_listBox,e3);
						}
					}
					else
					{
							m_pCurveManagerEP->MouseDown(l_vMouseWorldPos);
					}
				 }
				 else
				 {
					if( PathMode_comboBox->SelectedIndex != 1 )//1 for fix point, 0 for add point
					{
						if(WholePath_listBox->SelectedIndex!=-1)
						{
							m_pCurveManagerEP->GetObject(WholePath_listBox->SelectedIndex)->AddPoint(l_vMouseWorldPos,0);
							System::EventArgs^  e3;
							WholePath_listBox_SelectedIndexChanged(WholePath_listBox,e3);
						}
					}
					else
					{
							m_pCurveManagerEP->MouseDown(l_vMouseWorldPos);
					}
				 }
			 }

	private: System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
			 {
				 if(!timer1->Enabled)
					return;
				 GCFORM::MouseButtons l_MouseButton = e->Button;
				 Vector2	l_vVeiewPort((float)splitContainer2->Panel1->Width,(float)splitContainer2->Panel1->Height);
				 m_pSplitScreenCamera->MouseEvent(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
					,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,l_vVeiewPort,m_pTimeAndFPS->fElpaseTime);
				 if( perspectiveToolStripMenuItem->Checked )
				 {

				 }
				 else
				 {
					 Vector3	l_vMouseWorldPos = this->m_pSplitScreenCamera->GetCurrentMousePos();
					 m_pCurveManagerEP->MouseUp(l_vMouseWorldPos);
				 }
			 }

private: System::Void tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			if( m_pTabPage != m_pTabControl->SelectedTab)
				timer1->Enabled = false;
			else
			{
				wglMakeCurrent( m_HdcMV,m_HGLRCMV );
				glViewport(0,0,splitContainer2->Panel1->Width,splitContainer2->Panel1->Height);
				timer1->Enabled = true;
			}
		 }
	private: System::Void BackgrioundImage_button_Click(System::Object^  sender, System::EventArgs^  e)
		{
			 String^l_str = DNCT::OpenFileAndGetName();
			 if( l_str )
			 {
				 SAFE_DELETE(m_pBKImageEP);
				 m_pBKImageEP = new cBaseImage(DNCT::GcStringToChar(l_str).c_str());
			 }
		}
private: System::Void AddImage_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 cli::array<String^>^l_str = DNCT::OpenFileAndGetNames();
			 if( l_str )
			 for each(String^l_strName in l_str )
			 {
				 cBaseImage*l_pImage = new cBaseImage(DNCT::GcStringToChar(l_strName).c_str());
				 bool	l_b = m_pBaseImageListEP->AddObject(l_pImage);
				 if( !l_b )
					 m_pBaseImageListEP->RemoveObject(l_pImage);
				 else
				 {
					 ImageName_listBox->Items->Add( String( UT::WcharToChar(l_pImage->GetName()).c_str()).ToString());
				 }
			 }
		 }
private: System::Void ImageName_listBox_SelectedIndexChanged_1(System::Object^  sender, System::EventArgs^  e) {
			 if( ImageName_listBox->SelectedIndex != -1)
			 {
				 m_pCurveManagerEP->m_pBasemageForShow = (*m_pBaseImageListEP)[ImageName_listBox->SelectedIndex];
			 }
		 }
private: System::Void ImageName_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			if(ImageName_listBox->SelectedIndex!=-1)
				m_pCurveManagerEP->m_pBasemageForShow = dynamic_cast<cUIImage*>(m_pBaseImageListEP->GetObject(DNCT::GcStringToWchar(ImageName_listBox->SelectedItem->ToString())));
		 }
public: void	Active(){wglMakeCurrent( m_HdcMV,m_HGLRCMV ); this->timer1->Enabled = true; }
private: System::Void orthgnalToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( sender == perspectiveToolStripMenuItem )
			{
				perspectiveToolStripMenuItem->Checked = true;
				orthgnalToolStripMenuItem->Checked = false;
			}
			else
			{
				perspectiveToolStripMenuItem->Checked = false;
				orthgnalToolStripMenuItem->Checked = true;
			}
			if( perspectiveToolStripMenuItem->Checked == true )
				m_pSplitScreenCamera->m_eProjectionType = Projection::Perspective;
			else
				m_pSplitScreenCamera->m_eProjectionType = Projection::Orthographic;
		 }
private: System::Void hideEditPanelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( sender == hideEditPanelToolStripMenuItem )
			{
				splitContainer1->Panel1Collapsed = !splitContainer1->Panel1Collapsed;
			}
			else
			if( sender == showSplitScreenToolStripMenuItem )
			{
				showSplitScreenToolStripMenuItem->Checked = !showSplitScreenToolStripMenuItem->Checked;
				//m_pSplitScreenCamera->m_bShowPlane = showSplitScreenToolStripMenuItem->Checked;
				m_pSplitScreenCamera->m_bShowSplitScreen = showSplitScreenToolStripMenuItem->Checked;
			}
			else
			{
				showPlaneToolStripMenuItem->Checked = !showPlaneToolStripMenuItem->Checked;
				this->m_pSplitScreenCamera->m_bShowPlane = showPlaneToolStripMenuItem->Checked;
			}
		 }
private: System::Void CameraReset_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_pSplitScreenCamera->Reset();
		 }
private: System::Void LoadModel_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_str = DNCT::OpenFileAndGetName("Model Files(*.dae;*.xml;*.path)|*.dae;*.xml;*.path|All files (*.*)|*.*");
			 if( l_str )
			 {//load model

			 }
		 }
public:System::Void MyKeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	   {
			if(e->Control)
				m_bCtrlPressed = true;
			else
				m_bCtrlPressed = false;
	   }
public:System::Void MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	   {
			m_bCtrlPressed = false;	   
	   }
};
}

