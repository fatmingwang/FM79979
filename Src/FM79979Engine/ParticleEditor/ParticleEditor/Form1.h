#pragma once
//#include "../../../include/IL/il.h"
#include "EditState.h"
#include "MeshViewer.h"
#include "EmitterEditor.h"
extern cPaticleManager*g_pPaticleManager;
extern cPrtEmitter*g_pPrtEmitter;
extern UT::sTimeAndFPS g_sTimeAndFPS;
extern cOrthogonalCamera	*g_pOrthogonalCamera;
extern GLenum g_sfactor;
extern GLenum g_dfactor;
extern float	DRAW_PANEL_RESOLUTION_WIDTH;
extern float	DRAW_PANEL_RESOLUTION_HEIGHT;
extern bool	g_bCameraResetClick;
extern cBaseImage*g_pBKImage;
//extern cColladaParser*	g_pParticleColladaParser;
char*	GetUsageExplanaing(const WCHAR*e_str);
namespace ParticalEditor
{
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
     // Create a new form to represent the child form.
      //Form^ child = gcnew Form;

      // Increment the private child count.
      //childCount++;

      // Set the text of the child form using the count of child forms.
//     String^ formText = String::Format( "Child {0}", childCount );
  //    child->Text = formText;

      // Make the new form a child form.
      //child->MdiParent = this;

      // Display the child form.
      //child->Show();
			InitializeComponent();
			g_pOrthogonalCamera = 0;
		    m_pbParticleUpdate = 0;
			g_pPaticleManager = 0;
			g_pPrtEmitter = 0;
			m_pCameraBehaveByMouseBehave = 0;
			m_pFrameCamera = 0;
			m_Hdc = 0;
			m_pHGLRC = 0;
			cGameApp::m_spImageParser = new cImageParser();
			g_pOrthogonalCamera = new cOrthogonalCamera;
			m_pFrameCamera = new cFrameCamera();
			Projection	l_ProjectionMV;
			l_ProjectionMV.SetFovYAspect( XM_PIDIV4 ,1920/1080.f, 1000.0f * 1e-4f,2000000.f);
			VECTOR4	l_vPos = {0,1,6.5,1};
			m_pFrameCamera->SetWorldTransform(cMatrix44::Identity);
			m_pFrameCamera->SetWorldPosition(l_vPos);
			m_pFrameCamera->SetProjection(l_ProjectionMV);
			m_pCameraBehaveByMouseBehave = new cCameraBehaveByMouseBehave(m_pFrameCamera);
			Mouse_toolStripButton->BackColor = System::Drawing::Color::LightBlue;
			ParticleType_listBox->SelectedIndex = 1;
			SrcBlending_listBox->SelectedIndex = 5;
			DestBlending_listBox->SelectedIndex = 6;
			InitPolicy_flowLayoutPanel->VerticalScroll->Enabled = true;
			this->timer1->Enabled = false;
			g_eEditState = eES_MOUSE;
			m_pRenderPanel = RenderingPlaneAndPolicyPlane_splitContainer->Panel1;
			m_Hdc = GetDC((HWND)m_pRenderPanel->Handle.ToPointer());
			m_pHGLRC = UT::InitOpenGL((HWND)m_pRenderPanel->Handle.ToPointer(),true,m_Hdc);
			m_pInitTabpageWithListBox = gcnew System::Collections::Hashtable;
			m_pActTabpageWithListBox = gcnew System::Collections::Hashtable;
			m_pActUserControlList = gcnew System::Collections::ArrayList;
			m_pInitUserControlList = gcnew System::Collections::ArrayList;
			this->RenderingPlaneAndPolicyPlane_splitContainer->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->m_pRenderPanel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->m_pRenderPanel->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			this->m_pRenderPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseEvent);
			InitPolicyToListBox();
			g_pPaticleManager = new cPaticleManager();
			//call again ensure the wgl content is this form
			wglMakeCurrent( m_Hdc,m_pHGLRC );
			InitEmiter(true);
			m_pbParticleUpdate = new bool;
			*m_pbParticleUpdate = false;
			m_pEmitterEditor = gcnew EmitterEditor(MainHorizontal_splitContainer->Panel1,timer1,m_pbParticleUpdate);
			FATMING_CORE::CreateShader(g_bCommonVSClientState,L"MyPrtShader");
			FATMING_CORE::CreateShader(g_bCommonVSNoTextureClientState,L"MyPrtShaderNoTexture");
			this->WindowState = GCFORM::FormWindowState::Maximized;

			MeshViewer^l_pMeshViewer = gcnew MeshViewer(ParticleControl_tabControl);
			if( 0 )//particle fomr
			{
				wglMakeCurrent( m_Hdc,m_pHGLRC );
				this->timer1->Enabled = true;
			}
			else
			{//mesh viewer form
				l_pMeshViewer->MakeCurrentTabPage();
			}
		}

	protected:
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			SAFE_DELETE(g_pOrthogonalCamera);
		    SAFE_DELETE(m_pbParticleUpdate);
			SAFE_DELETE(g_pPrtEmitter);
			SAFE_DELETE(g_pPaticleManager);
			SAFE_DELETE(m_pCameraBehaveByMouseBehave);
			SAFE_DELETE(m_pFrameCamera);
			FATMING_CORE::DeleteShader(L"MyPrtShader");
			FATMING_CORE::DeleteShader(L"MyPrtShaderNoTexture");
			//m_pEmitterEditor->MyDestroy();
			m_pEmitterEditor->~EmitterEditor();
			SAFE_DELETE(cGameApp::m_spImageParser);
		}
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  Mouse_toolStripButton;
	private: System::Windows::Forms::ToolStripButton^  Translation_toolStripButton;
	private: System::Windows::Forms::ToolStripButton^  Rotation_toolStripButton;
	private: System::Windows::Forms::TabPage^  Particle;
	private: System::Windows::Forms::SplitContainer^  MainHorizontal_splitContainer;
	private: System::Windows::Forms::SplitContainer^  InitAndAct_Policy_splitContainer;
	private: System::Windows::Forms::SplitContainer^  InitPolicyWithInitEditFlowPanel_splitContainer;
	private: System::Windows::Forms::TabControl^  InitPolicy_tabControl;
	private: System::Windows::Forms::Button^  AddInitPolicy_button;
	private: System::Windows::Forms::Button^  DeleteInitPolicy_button;
	private: System::Windows::Forms::ListBox^  AllInitPolicy_listBox;
	private: System::Windows::Forms::FlowLayoutPanel^  InitPolicy_flowLayoutPanel;
	private: System::Windows::Forms::SplitContainer^  ActtPolicyWithActEditFlowPanel_splitContainer;
	private: System::Windows::Forms::ListBox^  AllActPolicy_listBox;
	private: System::Windows::Forms::TabControl^  ActPolicy_tabControl;
	private: System::Windows::Forms::Button^  AddActPolicy_button;
	private: System::Windows::Forms::Button^  DelActPolicy_button;
	private: System::Windows::Forms::FlowLayoutPanel^  ActPolicy_flowLayoutPanel;
	private: System::Windows::Forms::SplitContainer^  EmiterListAndDrawPanel_splitContainer;
	private: System::Windows::Forms::CheckBox^  KillOutRangeParticle_checkBox;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::ListBox^  ParticleType_listBox;
	private: System::Windows::Forms::Button^  DisableTexture_button;
	private: System::Windows::Forms::Label^  FPS_label;
	private: System::Windows::Forms::ListBox^  DestBlending_listBox;
	private: System::Windows::Forms::ListBox^  SrcBlending_listBox;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  MaxParticleCount_numericUpDown;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::NumericUpDown^  EmitCount_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ParticleCount_numericUpDown;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::NumericUpDown^  EmitTimeDis_numericUpDown;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::Button^  Stop_button;
	private: System::Windows::Forms::Button^  Emit_button;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  EmiterName_textBox;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  AddTexture_button;
	private: System::Windows::Forms::Button^  DeleteTexture_button;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  DataInvert_button;
	private: System::Windows::Forms::Button^  AddEmiter_button;
	private: System::Windows::Forms::Button^  DeleteEmiter_button;
	private: System::Windows::Forms::ListBox^  EmiteListr_listBox;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  assignDefaultDataToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  backGroundToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  resolutionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x1080ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x720ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x768ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x480ToolStripMenuItem;
	private: System::Windows::Forms::TabControl^  ParticleControl_tabControl;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
	private: System::Windows::Forms::CheckBox^  EmitterDirection_checkBox;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::ToolStripMenuItem^  cameraTypeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  Orthgnal_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  Perspective_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ParticlePolicyPlane_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ParticleDataPlane_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  EmiterGroupPlane_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveParticleEmitterGroupToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openParticleEmitterGroupToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  assistanceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showLineToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showFPSToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showPlaneToolStripMenuItem;
	private: System::Windows::Forms::SplitContainer^  RenderingPlaneAndPolicyPlane_splitContainer;
	private: System::Windows::Forms::Timer^  MakePanelFor19;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel2;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::ToolStripMenuItem^  showEmitterToolStripMenuItem;
    private: System::Windows::Forms::TreeView^  Texture_treeView;
	private: System::Windows::Forms::ToolTip^  PolictHin_toolTip;
	private: System::Windows::Forms::ToolStripMenuItem^  x320ToolStripMenuItem;
         //my
			 //for dynamic texture...it's real tricky but fucking works!
			 ParticlePolicyForm::DynamicTexture^m_pCurrentDynamicTexture;
	private: void	TextureRefresh();
	private: System::Void	MyInitPolicyToListBox(const WCHAR***e_strDataList,GCFORM::TabControl^e_pTabControl,System::Collections::Hashtable^e_pHashTable);
	private: System::Collections::Hashtable^m_pInitTabpageWithListBox;//tabPage,listbox
	private: System::Collections::Hashtable^m_pActTabpageWithListBox;//tabPage,listbox
	private: System::Collections::ArrayList^m_pInitUserControlList;
	private: System::Collections::ArrayList^m_pActUserControlList;//ListBox items Object,user control
			 //add some tricky data at there!
	private: AutoCollpaseUnit^	Form1::GetPolicyFormByParticleBase(cParticleBase*e_pParticleBase);
	private: System::Void	InitPolicyToListBox();
	private: System::Void	AddInitPolicy();
	private: System::Void	DelInitPolicy();
	private: System::Void	AddActPolicy();
	private: System::Void	DelActPolicy();
	private: System::Void	InitEmiter(bool e_bAssignDefultParticleData);
			 //for immediately add policy,not for user add
	private: System::Void	AddPolicyIntoData(cParticleBase*e_pParticleBase,bool e_bInitPolicy);
			 EmitterEditor^m_pEmitterEditor;
			 HDC	m_Hdc;
			 HGLRC	m_pHGLRC;
			 bool	*m_pbParticleUpdate;
			 GCFORM::SplitterPanel^m_pRenderPanel;
			 cCameraBehaveByMouseBehave*m_pCameraBehaveByMouseBehave;
			 cFrameCamera	*m_pFrameCamera;
			 /// <summary>
		/// 設計工具所需的變數。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->Mouse_toolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->Translation_toolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->Rotation_toolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->Particle = (gcnew System::Windows::Forms::TabPage());
			this->MainHorizontal_splitContainer = (gcnew System::Windows::Forms::SplitContainer());
			this->EmiterListAndDrawPanel_splitContainer = (gcnew System::Windows::Forms::SplitContainer());
			this->Texture_treeView = (gcnew System::Windows::Forms::TreeView());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->EmitterDirection_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->KillOutRangeParticle_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->ParticleType_listBox = (gcnew System::Windows::Forms::ListBox());
			this->DisableTexture_button = (gcnew System::Windows::Forms::Button());
			this->FPS_label = (gcnew System::Windows::Forms::Label());
			this->DestBlending_listBox = (gcnew System::Windows::Forms::ListBox());
			this->SrcBlending_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->MaxParticleCount_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->EmitCount_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ParticleCount_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->EmitTimeDis_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->Stop_button = (gcnew System::Windows::Forms::Button());
			this->Emit_button = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->EmiterName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->AddTexture_button = (gcnew System::Windows::Forms::Button());
			this->DeleteTexture_button = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->DataInvert_button = (gcnew System::Windows::Forms::Button());
			this->AddEmiter_button = (gcnew System::Windows::Forms::Button());
			this->DeleteEmiter_button = (gcnew System::Windows::Forms::Button());
			this->EmiteListr_listBox = (gcnew System::Windows::Forms::ListBox());
			this->RenderingPlaneAndPolicyPlane_splitContainer = (gcnew System::Windows::Forms::SplitContainer());
			this->InitAndAct_Policy_splitContainer = (gcnew System::Windows::Forms::SplitContainer());
			this->InitPolicyWithInitEditFlowPanel_splitContainer = (gcnew System::Windows::Forms::SplitContainer());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->DeleteInitPolicy_button = (gcnew System::Windows::Forms::Button());
			this->AddInitPolicy_button = (gcnew System::Windows::Forms::Button());
			this->AllInitPolicy_listBox = (gcnew System::Windows::Forms::ListBox());
			this->InitPolicy_tabControl = (gcnew System::Windows::Forms::TabControl());
			this->InitPolicy_flowLayoutPanel = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->ActtPolicyWithActEditFlowPanel_splitContainer = (gcnew System::Windows::Forms::SplitContainer());
			this->tableLayoutPanel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->AllActPolicy_listBox = (gcnew System::Windows::Forms::ListBox());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->AddActPolicy_button = (gcnew System::Windows::Forms::Button());
			this->DelActPolicy_button = (gcnew System::Windows::Forms::Button());
			this->ActPolicy_tabControl = (gcnew System::Windows::Forms::TabControl());
			this->ActPolicy_flowLayoutPanel = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openParticleEmitterGroupToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveParticleEmitterGroupToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->assignDefaultDataToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->backGroundToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resolutionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x1080ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x720ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x768ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x480ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x320ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cameraTypeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Orthgnal_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Perspective_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ParticlePolicyPlane_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ParticleDataPlane_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->EmiterGroupPlane_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->assistanceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showLineToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showFPSToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showPlaneToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showEmitterToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ParticleControl_tabControl = (gcnew System::Windows::Forms::TabControl());
			this->PolictHin_toolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->MakePanelFor19 = (gcnew System::Windows::Forms::Timer(this->components));
			this->toolStrip1->SuspendLayout();
			this->Particle->SuspendLayout();
			this->MainHorizontal_splitContainer->Panel2->SuspendLayout();
			this->MainHorizontal_splitContainer->SuspendLayout();
			this->EmiterListAndDrawPanel_splitContainer->Panel1->SuspendLayout();
			this->EmiterListAndDrawPanel_splitContainer->Panel2->SuspendLayout();
			this->EmiterListAndDrawPanel_splitContainer->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MaxParticleCount_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->EmitCount_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ParticleCount_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->EmitTimeDis_numericUpDown))->BeginInit();
			this->RenderingPlaneAndPolicyPlane_splitContainer->Panel2->SuspendLayout();
			this->RenderingPlaneAndPolicyPlane_splitContainer->SuspendLayout();
			this->InitAndAct_Policy_splitContainer->Panel1->SuspendLayout();
			this->InitAndAct_Policy_splitContainer->Panel2->SuspendLayout();
			this->InitAndAct_Policy_splitContainer->SuspendLayout();
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Panel1->SuspendLayout();
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Panel2->SuspendLayout();
			this->InitPolicyWithInitEditFlowPanel_splitContainer->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->panel1->SuspendLayout();
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Panel1->SuspendLayout();
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Panel2->SuspendLayout();
			this->ActtPolicyWithActEditFlowPanel_splitContainer->SuspendLayout();
			this->tableLayoutPanel2->SuspendLayout();
			this->panel2->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->ParticleControl_tabControl->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// toolStrip1
			// 
			this->toolStrip1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->toolStrip1->ImageScalingSize = System::Drawing::Size(32, 32);
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->Mouse_toolStripButton, 
				this->Translation_toolStripButton, this->Rotation_toolStripButton, this->toolStripButton1});
			this->toolStrip1->Location = System::Drawing::Point(0, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(1618, 39);
			this->toolStrip1->TabIndex = 3;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// Mouse_toolStripButton
			// 
			this->Mouse_toolStripButton->BackColor = System::Drawing::Color::Transparent;
			this->Mouse_toolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->Mouse_toolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Mouse_toolStripButton.Image")));
			this->Mouse_toolStripButton->ImageTransparentColor = System::Drawing::Color::Black;
			this->Mouse_toolStripButton->Name = L"Mouse_toolStripButton";
			this->Mouse_toolStripButton->Size = System::Drawing::Size(36, 36);
			this->Mouse_toolStripButton->Text = L"Mouse";
			this->Mouse_toolStripButton->Click += gcnew System::EventHandler(this, &Form1::Mouse_toolStripButton_Click);
			// 
			// Translation_toolStripButton
			// 
			this->Translation_toolStripButton->BackColor = System::Drawing::Color::Transparent;
			this->Translation_toolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->Translation_toolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Translation_toolStripButton.Image")));
			this->Translation_toolStripButton->ImageTransparentColor = System::Drawing::Color::Black;
			this->Translation_toolStripButton->Name = L"Translation_toolStripButton";
			this->Translation_toolStripButton->Size = System::Drawing::Size(36, 36);
			this->Translation_toolStripButton->Text = L"Translation";
			this->Translation_toolStripButton->Click += gcnew System::EventHandler(this, &Form1::Mouse_toolStripButton_Click);
			// 
			// Rotation_toolStripButton
			// 
			this->Rotation_toolStripButton->BackColor = System::Drawing::Color::Transparent;
			this->Rotation_toolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->Rotation_toolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Rotation_toolStripButton.Image")));
			this->Rotation_toolStripButton->ImageTransparentColor = System::Drawing::Color::Black;
			this->Rotation_toolStripButton->Name = L"Rotation_toolStripButton";
			this->Rotation_toolStripButton->Size = System::Drawing::Size(36, 36);
			this->Rotation_toolStripButton->Text = L"Rotation X";
			this->Rotation_toolStripButton->Click += gcnew System::EventHandler(this, &Form1::Mouse_toolStripButton_Click);
			// 
			// toolStripButton1
			// 
			this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton1.Image")));
			this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Black;
			this->toolStripButton1->Name = L"toolStripButton1";
			this->toolStripButton1->Size = System::Drawing::Size(36, 36);
			this->toolStripButton1->Text = L"toolStripButton1";
			this->toolStripButton1->Click += gcnew System::EventHandler(this, &Form1::toolStripButton1_Click);
			// 
			// Particle
			// 
			this->Particle->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->Particle->Controls->Add(this->MainHorizontal_splitContainer);
			this->Particle->Controls->Add(this->menuStrip1);
			this->Particle->Location = System::Drawing::Point(4, 25);
			this->Particle->Margin = System::Windows::Forms::Padding(2);
			this->Particle->Name = L"Particle";
			this->Particle->Padding = System::Windows::Forms::Padding(2);
			this->Particle->Size = System::Drawing::Size(1610, 614);
			this->Particle->TabIndex = 0;
			this->Particle->Text = L"Particle";
			this->Particle->UseVisualStyleBackColor = true;
			// 
			// MainHorizontal_splitContainer
			// 
			this->MainHorizontal_splitContainer->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->MainHorizontal_splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->MainHorizontal_splitContainer->Location = System::Drawing::Point(2, 26);
			this->MainHorizontal_splitContainer->Margin = System::Windows::Forms::Padding(2);
			this->MainHorizontal_splitContainer->Name = L"MainHorizontal_splitContainer";
			// 
			// MainHorizontal_splitContainer.Panel2
			// 
			this->MainHorizontal_splitContainer->Panel2->Controls->Add(this->EmiterListAndDrawPanel_splitContainer);
			this->MainHorizontal_splitContainer->Size = System::Drawing::Size(1606, 586);
			this->MainHorizontal_splitContainer->SplitterDistance = 351;
			this->MainHorizontal_splitContainer->SplitterWidth = 3;
			this->MainHorizontal_splitContainer->TabIndex = 0;
			// 
			// EmiterListAndDrawPanel_splitContainer
			// 
			this->EmiterListAndDrawPanel_splitContainer->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->EmiterListAndDrawPanel_splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->EmiterListAndDrawPanel_splitContainer->Location = System::Drawing::Point(0, 0);
			this->EmiterListAndDrawPanel_splitContainer->Margin = System::Windows::Forms::Padding(0);
			this->EmiterListAndDrawPanel_splitContainer->Name = L"EmiterListAndDrawPanel_splitContainer";
			this->EmiterListAndDrawPanel_splitContainer->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// EmiterListAndDrawPanel_splitContainer.Panel1
			// 
			this->EmiterListAndDrawPanel_splitContainer->Panel1->AutoScroll = true;
			this->EmiterListAndDrawPanel_splitContainer->Panel1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->Texture_treeView);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->label8);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->label6);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->EmitterDirection_checkBox);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->KillOutRangeParticle_checkBox);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->label5);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->ParticleType_listBox);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->DisableTexture_button);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->FPS_label);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->DestBlending_listBox);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->SrcBlending_listBox);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->label4);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->MaxParticleCount_numericUpDown);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->label7);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->EmitCount_numericUpDown);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->ParticleCount_numericUpDown);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->label10);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->EmitTimeDis_numericUpDown);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->label19);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->Stop_button);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->Emit_button);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->label3);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->EmiterName_textBox);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->label2);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->AddTexture_button);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->DeleteTexture_button);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->label1);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->DataInvert_button);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->AddEmiter_button);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->DeleteEmiter_button);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->Controls->Add(this->EmiteListr_listBox);
			this->EmiterListAndDrawPanel_splitContainer->Panel1MinSize = 0;
			// 
			// EmiterListAndDrawPanel_splitContainer.Panel2
			// 
			this->EmiterListAndDrawPanel_splitContainer->Panel2->Controls->Add(this->RenderingPlaneAndPolicyPlane_splitContainer);
			this->EmiterListAndDrawPanel_splitContainer->Size = System::Drawing::Size(1250, 584);
			this->EmiterListAndDrawPanel_splitContainer->SplitterDistance = 186;
			this->EmiterListAndDrawPanel_splitContainer->SplitterWidth = 3;
			this->EmiterListAndDrawPanel_splitContainer->TabIndex = 1;
			this->EmiterListAndDrawPanel_splitContainer->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::EmiterListAndDrawPanel_splitContainer_KeyDown);
			// 
			// Texture_treeView
			// 
			this->Texture_treeView->Location = System::Drawing::Point(176, 54);
			this->Texture_treeView->Name = L"Texture_treeView";
			this->Texture_treeView->Size = System::Drawing::Size(143, 97);
			this->Texture_treeView->TabIndex = 75;
			this->Texture_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &Form1::Texture_treeView_AfterSelect);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->label8->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label8->Location = System::Drawing::Point(542, 25);
			this->label8->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(25, 12);
			this->label8->TabIndex = 74;
			this->label8->Text = L"Dest";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->label6->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label6->Location = System::Drawing::Point(397, 25);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(37, 12);
			this->label6->TabIndex = 73;
			this->label6->Text = L"Source";
			// 
			// EmitterDirection_checkBox
			// 
			this->EmitterDirection_checkBox->AutoSize = true;
			this->EmitterDirection_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->EmitterDirection_checkBox->Checked = true;
			this->EmitterDirection_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->EmitterDirection_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->EmitterDirection_checkBox->Location = System::Drawing::Point(839, 115);
			this->EmitterDirection_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->EmitterDirection_checkBox->Name = L"EmitterDirection_checkBox";
			this->EmitterDirection_checkBox->Size = System::Drawing::Size(164, 16);
			this->EmitterDirection_checkBox->TabIndex = 71;
			this->EmitterDirection_checkBox->Text = L"EmitterDirection/發射器方向";
			this->EmitterDirection_checkBox->UseVisualStyleBackColor = false;
			// 
			// KillOutRangeParticle_checkBox
			// 
			this->KillOutRangeParticle_checkBox->AutoSize = true;
			this->KillOutRangeParticle_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->KillOutRangeParticle_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->KillOutRangeParticle_checkBox->Location = System::Drawing::Point(839, 138);
			this->KillOutRangeParticle_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->KillOutRangeParticle_checkBox->Name = L"KillOutRangeParticle_checkBox";
			this->KillOutRangeParticle_checkBox->Size = System::Drawing::Size(88, 16);
			this->KillOutRangeParticle_checkBox->TabIndex = 70;
			this->KillOutRangeParticle_checkBox->Text = L"KillOutRange";
			this->KillOutRangeParticle_checkBox->UseVisualStyleBackColor = false;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->label5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label5->Location = System::Drawing::Point(839, 9);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(63, 12);
			this->label5->TabIndex = 69;
			this->label5->Text = L"ParticleType";
			// 
			// ParticleType_listBox
			// 
			this->ParticleType_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->ParticleType_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ParticleType_listBox->FormattingEnabled = true;
			this->ParticleType_listBox->ItemHeight = 12;
			this->ParticleType_listBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"POINTS", L"QUADS"});
			this->ParticleType_listBox->Location = System::Drawing::Point(841, 23);
			this->ParticleType_listBox->Margin = System::Windows::Forms::Padding(2);
			this->ParticleType_listBox->Name = L"ParticleType_listBox";
			this->ParticleType_listBox->Size = System::Drawing::Size(82, 28);
			this->ParticleType_listBox->TabIndex = 68;
			this->ParticleType_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::ParticleType_listBox_SelectedIndexChanged);
			// 
			// DisableTexture_button
			// 
			this->DisableTexture_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->DisableTexture_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DisableTexture_button->Location = System::Drawing::Point(180, 154);
			this->DisableTexture_button->Margin = System::Windows::Forms::Padding(2);
			this->DisableTexture_button->Name = L"DisableTexture_button";
			this->DisableTexture_button->Size = System::Drawing::Size(139, 22);
			this->DisableTexture_button->TabIndex = 67;
			this->DisableTexture_button->Text = L"DisableTexture";
			this->DisableTexture_button->UseVisualStyleBackColor = false;
			this->DisableTexture_button->Click += gcnew System::EventHandler(this, &Form1::DisableTexture_button_Click);
			// 
			// FPS_label
			// 
			this->FPS_label->AutoSize = true;
			this->FPS_label->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->FPS_label->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->FPS_label->Location = System::Drawing::Point(932, 15);
			this->FPS_label->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->FPS_label->Name = L"FPS_label";
			this->FPS_label->Size = System::Drawing::Size(23, 12);
			this->FPS_label->TabIndex = 66;
			this->FPS_label->Text = L"FPS";
			// 
			// DestBlending_listBox
			// 
			this->DestBlending_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->DestBlending_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DestBlending_listBox->FormattingEnabled = true;
			this->DestBlending_listBox->ItemHeight = 12;
			this->DestBlending_listBox->Items->AddRange(gcnew cli::array< System::Object^  >(9) {L"GL_ZERO", L"GL_ONE", L"GL_ONE_MINUS_SRC_COLOR", 
				L"GL_DST_COLOR", L"GL_ONE_MINUS_DST_COLOR", L"GL_SRC_ALPHA", L"GL_ONE_MINUS_SRC_ALPHA", L"GL_DST_ALPHA", L"GL_ONE_MINUS_DST_ALPHA"});
			this->DestBlending_listBox->Location = System::Drawing::Point(504, 49);
			this->DestBlending_listBox->Margin = System::Windows::Forms::Padding(2);
			this->DestBlending_listBox->Name = L"DestBlending_listBox";
			this->DestBlending_listBox->Size = System::Drawing::Size(156, 124);
			this->DestBlending_listBox->TabIndex = 65;
			this->DestBlending_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::DestBlending_listBox_SelectedIndexChanged);
			// 
			// SrcBlending_listBox
			// 
			this->SrcBlending_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->SrcBlending_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->SrcBlending_listBox->FormattingEnabled = true;
			this->SrcBlending_listBox->ItemHeight = 12;
			this->SrcBlending_listBox->Items->AddRange(gcnew cli::array< System::Object^  >(9) {L"GL_ZERO", L"GL_ONE", L"GL_ONE_MINUS_SRC_COLOR", 
				L"GL_DST_COLOR", L"GL_ONE_MINUS_DST_COLOR", L"GL_SRC_ALPHA", L"GL_ONE_MINUS_SRC_ALPHA", L"GL_DST_ALPHA", L"GL_ONE_MINUS_DST_ALPHA"});
			this->SrcBlending_listBox->Location = System::Drawing::Point(337, 49);
			this->SrcBlending_listBox->Margin = System::Windows::Forms::Padding(2);
			this->SrcBlending_listBox->Name = L"SrcBlending_listBox";
			this->SrcBlending_listBox->Size = System::Drawing::Size(161, 124);
			this->SrcBlending_listBox->TabIndex = 64;
			this->SrcBlending_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::SrcBlending_listBox_SelectedIndexChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->label4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label4->Location = System::Drawing::Point(667, 8);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(164, 12);
			this->label4->TabIndex = 63;
			this->label4->Text = L"MaxParticleCount/粒子上限數量";
			// 
			// MaxParticleCount_numericUpDown
			// 
			this->MaxParticleCount_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->MaxParticleCount_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->MaxParticleCount_numericUpDown->Location = System::Drawing::Point(669, 23);
			this->MaxParticleCount_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->MaxParticleCount_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->MaxParticleCount_numericUpDown->Name = L"MaxParticleCount_numericUpDown";
			this->MaxParticleCount_numericUpDown->Size = System::Drawing::Size(81, 22);
			this->MaxParticleCount_numericUpDown->TabIndex = 62;
			this->MaxParticleCount_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->MaxParticleCount_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::MaxParticleCount_numericUpDown_ValueChanged);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->label7->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label7->Location = System::Drawing::Point(667, 138);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(165, 12);
			this->label7->TabIndex = 61;
			this->label7->Text = L"EmitParticleCount/每次噴發數量";
			// 
			// EmitCount_numericUpDown
			// 
			this->EmitCount_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->EmitCount_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->EmitCount_numericUpDown->Location = System::Drawing::Point(671, 65);
			this->EmitCount_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->EmitCount_numericUpDown->Name = L"EmitCount_numericUpDown";
			this->EmitCount_numericUpDown->Size = System::Drawing::Size(81, 22);
			this->EmitCount_numericUpDown->TabIndex = 56;
			this->EmitCount_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->EmitCount_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::EmitCount_numericUpDown_ValueChanged);
			// 
			// ParticleCount_numericUpDown
			// 
			this->ParticleCount_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->ParticleCount_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ParticleCount_numericUpDown->Location = System::Drawing::Point(669, 153);
			this->ParticleCount_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->ParticleCount_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->ParticleCount_numericUpDown->Name = L"ParticleCount_numericUpDown";
			this->ParticleCount_numericUpDown->Size = System::Drawing::Size(81, 22);
			this->ParticleCount_numericUpDown->TabIndex = 60;
			this->ParticleCount_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
			this->ParticleCount_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::ParticleCount_numericUpDown_ValueChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->label10->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label10->Location = System::Drawing::Point(671, 52);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(107, 12);
			this->label10->TabIndex = 57;
			this->label10->Text = L"EmitCount/噴發次數";
			// 
			// EmitTimeDis_numericUpDown
			// 
			this->EmitTimeDis_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->EmitTimeDis_numericUpDown->DecimalPlaces = 3;
			this->EmitTimeDis_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->EmitTimeDis_numericUpDown->Location = System::Drawing::Point(669, 108);
			this->EmitTimeDis_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->EmitTimeDis_numericUpDown->Name = L"EmitTimeDis_numericUpDown";
			this->EmitTimeDis_numericUpDown->Size = System::Drawing::Size(81, 22);
			this->EmitTimeDis_numericUpDown->TabIndex = 58;
			this->EmitTimeDis_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 65536});
			this->EmitTimeDis_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::EmitTimeDis_numericUpDown_ValueChanged);
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->label19->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label19->Location = System::Drawing::Point(668, 93);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(117, 12);
			this->label19->TabIndex = 59;
			this->label19->Text = L"EmitTimeDis/噴發間距";
			// 
			// Stop_button
			// 
			this->Stop_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->Stop_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Stop_button->Location = System::Drawing::Point(841, 92);
			this->Stop_button->Name = L"Stop_button";
			this->Stop_button->Size = System::Drawing::Size(80, 18);
			this->Stop_button->TabIndex = 55;
			this->Stop_button->Text = L"Stop";
			this->Stop_button->UseVisualStyleBackColor = false;
			this->Stop_button->Click += gcnew System::EventHandler(this, &Form1::Stop_button_Click);
			// 
			// Emit_button
			// 
			this->Emit_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->Emit_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Emit_button->Location = System::Drawing::Point(841, 68);
			this->Emit_button->Name = L"Emit_button";
			this->Emit_button->Size = System::Drawing::Size(81, 18);
			this->Emit_button->TabIndex = 53;
			this->Emit_button->Text = L"Emit";
			this->Emit_button->UseVisualStyleBackColor = false;
			this->Emit_button->Click += gcnew System::EventHandler(this, &Form1::Emit_button_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label3->Location = System::Drawing::Point(4, 11);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(32, 12);
			this->label3->TabIndex = 14;
			this->label3->Text = L"Name";
			// 
			// EmiterName_textBox
			// 
			this->EmiterName_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->EmiterName_textBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->EmiterName_textBox->Location = System::Drawing::Point(40, 8);
			this->EmiterName_textBox->Margin = System::Windows::Forms::Padding(2);
			this->EmiterName_textBox->Name = L"EmiterName_textBox";
			this->EmiterName_textBox->Size = System::Drawing::Size(54, 22);
			this->EmiterName_textBox->TabIndex = 13;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(173, 37);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(68, 12);
			this->label2->TabIndex = 11;
			this->label2->Text = L"Texture/材質";
			// 
			// AddTexture_button
			// 
			this->AddTexture_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->AddTexture_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddTexture_button->Location = System::Drawing::Point(176, 11);
			this->AddTexture_button->Margin = System::Windows::Forms::Padding(2);
			this->AddTexture_button->Name = L"AddTexture_button";
			this->AddTexture_button->Size = System::Drawing::Size(56, 22);
			this->AddTexture_button->TabIndex = 10;
			this->AddTexture_button->Text = L"Add/增加";
			this->AddTexture_button->UseVisualStyleBackColor = false;
			this->AddTexture_button->Click += gcnew System::EventHandler(this, &Form1::AddTexture_button_Click);
			// 
			// DeleteTexture_button
			// 
			this->DeleteTexture_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->DeleteTexture_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DeleteTexture_button->Location = System::Drawing::Point(246, 11);
			this->DeleteTexture_button->Margin = System::Windows::Forms::Padding(2);
			this->DeleteTexture_button->Name = L"DeleteTexture_button";
			this->DeleteTexture_button->Size = System::Drawing::Size(66, 22);
			this->DeleteTexture_button->TabIndex = 9;
			this->DeleteTexture_button->Text = L"Delete/減少";
			this->DeleteTexture_button->UseVisualStyleBackColor = false;
			this->DeleteTexture_button->Click += gcnew System::EventHandler(this, &Form1::DeleteTexture_button_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(21, 44);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(116, 12);
			this->label1->TabIndex = 7;
			this->label1->Text = L"EmiterList/發射器列表";
			// 
			// DataInvert_button
			// 
			this->DataInvert_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->DataInvert_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DataInvert_button->Location = System::Drawing::Point(10, 156);
			this->DataInvert_button->Margin = System::Windows::Forms::Padding(2);
			this->DataInvert_button->Name = L"DataInvert_button";
			this->DataInvert_button->Size = System::Drawing::Size(89, 19);
			this->DataInvert_button->TabIndex = 3;
			this->DataInvert_button->Text = L"ReEdit/重新編輯";
			this->DataInvert_button->UseVisualStyleBackColor = false;
			this->DataInvert_button->Click += gcnew System::EventHandler(this, &Form1::DataInvert_button_Click);
			// 
			// AddEmiter_button
			// 
			this->AddEmiter_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->AddEmiter_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddEmiter_button->Location = System::Drawing::Point(104, 12);
			this->AddEmiter_button->Margin = System::Windows::Forms::Padding(2);
			this->AddEmiter_button->Name = L"AddEmiter_button";
			this->AddEmiter_button->Size = System::Drawing::Size(68, 18);
			this->AddEmiter_button->TabIndex = 2;
			this->AddEmiter_button->Text = L"Add/增加";
			this->AddEmiter_button->UseVisualStyleBackColor = false;
			this->AddEmiter_button->Click += gcnew System::EventHandler(this, &Form1::AddEmiter_button_Click);
			// 
			// DeleteEmiter_button
			// 
			this->DeleteEmiter_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->DeleteEmiter_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DeleteEmiter_button->Location = System::Drawing::Point(104, 156);
			this->DeleteEmiter_button->Margin = System::Windows::Forms::Padding(2);
			this->DeleteEmiter_button->Name = L"DeleteEmiter_button";
			this->DeleteEmiter_button->Size = System::Drawing::Size(66, 18);
			this->DeleteEmiter_button->TabIndex = 1;
			this->DeleteEmiter_button->Text = L"Delete/減少";
			this->DeleteEmiter_button->UseVisualStyleBackColor = false;
			this->DeleteEmiter_button->Click += gcnew System::EventHandler(this, &Form1::DeleteEmiter_button_Click);
			// 
			// EmiteListr_listBox
			// 
			this->EmiteListr_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->EmiteListr_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->EmiteListr_listBox->FormattingEnabled = true;
			this->EmiteListr_listBox->ItemHeight = 12;
			this->EmiteListr_listBox->Location = System::Drawing::Point(8, 64);
			this->EmiteListr_listBox->Margin = System::Windows::Forms::Padding(2);
			this->EmiteListr_listBox->Name = L"EmiteListr_listBox";
			this->EmiteListr_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->EmiteListr_listBox->Size = System::Drawing::Size(161, 88);
			this->EmiteListr_listBox->TabIndex = 0;
			this->EmiteListr_listBox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::EmiteListr_listBox_MouseDoubleClick);
			this->EmiteListr_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::EmiteListr_listBox_SelectedIndexChanged);
			this->EmiteListr_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::EmiteListr_listBox_KeyUp);
			// 
			// RenderingPlaneAndPolicyPlane_splitContainer
			// 
			this->RenderingPlaneAndPolicyPlane_splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->RenderingPlaneAndPolicyPlane_splitContainer->Location = System::Drawing::Point(0, 0);
			this->RenderingPlaneAndPolicyPlane_splitContainer->Name = L"RenderingPlaneAndPolicyPlane_splitContainer";
			// 
			// RenderingPlaneAndPolicyPlane_splitContainer.Panel1
			// 
			this->RenderingPlaneAndPolicyPlane_splitContainer->Panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::EmiterListAndDrawPanel_splitContainer_Panel2_MouseMove);
			// 
			// RenderingPlaneAndPolicyPlane_splitContainer.Panel2
			// 
			this->RenderingPlaneAndPolicyPlane_splitContainer->Panel2->Controls->Add(this->InitAndAct_Policy_splitContainer);
			this->RenderingPlaneAndPolicyPlane_splitContainer->Size = System::Drawing::Size(1246, 391);
			this->RenderingPlaneAndPolicyPlane_splitContainer->SplitterDistance = 777;
			this->RenderingPlaneAndPolicyPlane_splitContainer->TabIndex = 3;
			// 
			// InitAndAct_Policy_splitContainer
			// 
			this->InitAndAct_Policy_splitContainer->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), 
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->InitAndAct_Policy_splitContainer->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->InitAndAct_Policy_splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->InitAndAct_Policy_splitContainer->Location = System::Drawing::Point(0, 0);
			this->InitAndAct_Policy_splitContainer->Margin = System::Windows::Forms::Padding(2);
			this->InitAndAct_Policy_splitContainer->Name = L"InitAndAct_Policy_splitContainer";
			this->InitAndAct_Policy_splitContainer->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// InitAndAct_Policy_splitContainer.Panel1
			// 
			this->InitAndAct_Policy_splitContainer->Panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->InitAndAct_Policy_splitContainer->Panel1->Controls->Add(this->InitPolicyWithInitEditFlowPanel_splitContainer);
			this->InitAndAct_Policy_splitContainer->Panel1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			// 
			// InitAndAct_Policy_splitContainer.Panel2
			// 
			this->InitAndAct_Policy_splitContainer->Panel2->Controls->Add(this->ActtPolicyWithActEditFlowPanel_splitContainer);
			this->InitAndAct_Policy_splitContainer->Size = System::Drawing::Size(465, 391);
			this->InitAndAct_Policy_splitContainer->SplitterDistance = 190;
			this->InitAndAct_Policy_splitContainer->TabIndex = 2;
			// 
			// InitPolicyWithInitEditFlowPanel_splitContainer
			// 
			this->InitPolicyWithInitEditFlowPanel_splitContainer->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->InitPolicyWithInitEditFlowPanel_splitContainer->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Location = System::Drawing::Point(0, 0);
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Margin = System::Windows::Forms::Padding(2);
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Name = L"InitPolicyWithInitEditFlowPanel_splitContainer";
			// 
			// InitPolicyWithInitEditFlowPanel_splitContainer.Panel1
			// 
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Panel1->Controls->Add(this->tableLayoutPanel1);
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Panel1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			// 
			// InitPolicyWithInitEditFlowPanel_splitContainer.Panel2
			// 
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Panel2->Controls->Add(this->InitPolicy_flowLayoutPanel);
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Size = System::Drawing::Size(465, 190);
			this->InitPolicyWithInitEditFlowPanel_splitContainer->SplitterDistance = 300;
			this->InitPolicyWithInitEditFlowPanel_splitContainer->SplitterWidth = 3;
			this->InitPolicyWithInitEditFlowPanel_splitContainer->TabIndex = 0;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				100)));
			this->tableLayoutPanel1->Controls->Add(this->panel1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->AllInitPolicy_listBox, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->InitPolicy_tabControl, 0, 0);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 3;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 48.33948F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 28)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 51.66052F)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(296, 186);
			this->tableLayoutPanel1->TabIndex = 0;
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->DeleteInitPolicy_button);
			this->panel1->Controls->Add(this->AddInitPolicy_button);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel1->Location = System::Drawing::Point(3, 79);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(290, 22);
			this->panel1->TabIndex = 15;
			// 
			// DeleteInitPolicy_button
			// 
			this->DeleteInitPolicy_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DeleteInitPolicy_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DeleteInitPolicy_button->Location = System::Drawing::Point(165, 3);
			this->DeleteInitPolicy_button->Margin = System::Windows::Forms::Padding(2);
			this->DeleteInitPolicy_button->Name = L"DeleteInitPolicy_button";
			this->DeleteInitPolicy_button->Size = System::Drawing::Size(45, 22);
			this->DeleteInitPolicy_button->TabIndex = 13;
			this->DeleteInitPolicy_button->Text = L"Del";
			this->DeleteInitPolicy_button->UseVisualStyleBackColor = false;
			this->DeleteInitPolicy_button->Click += gcnew System::EventHandler(this, &Form1::AddInitPolicy_button_Click);
			// 
			// AddInitPolicy_button
			// 
			this->AddInitPolicy_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddInitPolicy_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddInitPolicy_button->Location = System::Drawing::Point(63, 4);
			this->AddInitPolicy_button->Margin = System::Windows::Forms::Padding(2);
			this->AddInitPolicy_button->Name = L"AddInitPolicy_button";
			this->AddInitPolicy_button->Size = System::Drawing::Size(45, 20);
			this->AddInitPolicy_button->TabIndex = 14;
			this->AddInitPolicy_button->Text = L"Add";
			this->AddInitPolicy_button->UseVisualStyleBackColor = false;
			this->AddInitPolicy_button->Click += gcnew System::EventHandler(this, &Form1::AddInitPolicy_button_Click);
			// 
			// AllInitPolicy_listBox
			// 
			this->AllInitPolicy_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->AllInitPolicy_listBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->AllInitPolicy_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllInitPolicy_listBox->FormattingEnabled = true;
			this->AllInitPolicy_listBox->ItemHeight = 12;
			this->AllInitPolicy_listBox->Location = System::Drawing::Point(2, 106);
			this->AllInitPolicy_listBox->Margin = System::Windows::Forms::Padding(2);
			this->AllInitPolicy_listBox->Name = L"AllInitPolicy_listBox";
			this->AllInitPolicy_listBox->Size = System::Drawing::Size(292, 76);
			this->AllInitPolicy_listBox->TabIndex = 12;
			// 
			// InitPolicy_tabControl
			// 
			this->InitPolicy_tabControl->Appearance = System::Windows::Forms::TabAppearance::Buttons;
			this->InitPolicy_tabControl->Dock = System::Windows::Forms::DockStyle::Fill;
			this->InitPolicy_tabControl->Location = System::Drawing::Point(2, 2);
			this->InitPolicy_tabControl->Margin = System::Windows::Forms::Padding(2);
			this->InitPolicy_tabControl->Name = L"InitPolicy_tabControl";
			this->InitPolicy_tabControl->SelectedIndex = 0;
			this->InitPolicy_tabControl->Size = System::Drawing::Size(292, 72);
			this->InitPolicy_tabControl->TabIndex = 15;
			// 
			// InitPolicy_flowLayoutPanel
			// 
			this->InitPolicy_flowLayoutPanel->AutoScroll = true;
			this->InitPolicy_flowLayoutPanel->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->InitPolicy_flowLayoutPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->InitPolicy_flowLayoutPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->InitPolicy_flowLayoutPanel->Location = System::Drawing::Point(0, 0);
			this->InitPolicy_flowLayoutPanel->Margin = System::Windows::Forms::Padding(0);
			this->InitPolicy_flowLayoutPanel->Name = L"InitPolicy_flowLayoutPanel";
			this->InitPolicy_flowLayoutPanel->Size = System::Drawing::Size(158, 186);
			this->InitPolicy_flowLayoutPanel->TabIndex = 16;
			// 
			// ActtPolicyWithActEditFlowPanel_splitContainer
			// 
			this->ActtPolicyWithActEditFlowPanel_splitContainer->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ActtPolicyWithActEditFlowPanel_splitContainer->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Location = System::Drawing::Point(0, 0);
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Margin = System::Windows::Forms::Padding(2);
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Name = L"ActtPolicyWithActEditFlowPanel_splitContainer";
			// 
			// ActtPolicyWithActEditFlowPanel_splitContainer.Panel1
			// 
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Panel1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Panel1->Controls->Add(this->tableLayoutPanel2);
			// 
			// ActtPolicyWithActEditFlowPanel_splitContainer.Panel2
			// 
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Panel2->AutoScroll = true;
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Panel2->Controls->Add(this->ActPolicy_flowLayoutPanel);
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Size = System::Drawing::Size(465, 197);
			this->ActtPolicyWithActEditFlowPanel_splitContainer->SplitterDistance = 297;
			this->ActtPolicyWithActEditFlowPanel_splitContainer->SplitterWidth = 3;
			this->ActtPolicyWithActEditFlowPanel_splitContainer->TabIndex = 0;
			// 
			// tableLayoutPanel2
			// 
			this->tableLayoutPanel2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->tableLayoutPanel2->ColumnCount = 1;
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				100)));
			this->tableLayoutPanel2->Controls->Add(this->AllActPolicy_listBox, 0, 2);
			this->tableLayoutPanel2->Controls->Add(this->panel2, 0, 1);
			this->tableLayoutPanel2->Controls->Add(this->ActPolicy_tabControl, 0, 0);
			this->tableLayoutPanel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->tableLayoutPanel2->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel2->Name = L"tableLayoutPanel2";
			this->tableLayoutPanel2->RowCount = 3;
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 48.33948F)));
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 28)));
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 51.66052F)));
			this->tableLayoutPanel2->Size = System::Drawing::Size(293, 193);
			this->tableLayoutPanel2->TabIndex = 1;
			// 
			// AllActPolicy_listBox
			// 
			this->AllActPolicy_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->AllActPolicy_listBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->AllActPolicy_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllActPolicy_listBox->FormattingEnabled = true;
			this->AllActPolicy_listBox->ItemHeight = 12;
			this->AllActPolicy_listBox->Location = System::Drawing::Point(2, 109);
			this->AllActPolicy_listBox->Margin = System::Windows::Forms::Padding(2);
			this->AllActPolicy_listBox->Name = L"AllActPolicy_listBox";
			this->AllActPolicy_listBox->Size = System::Drawing::Size(289, 76);
			this->AllActPolicy_listBox->TabIndex = 20;
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->AddActPolicy_button);
			this->panel2->Controls->Add(this->DelActPolicy_button);
			this->panel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel2->Location = System::Drawing::Point(3, 82);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(287, 22);
			this->panel2->TabIndex = 15;
			// 
			// AddActPolicy_button
			// 
			this->AddActPolicy_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddActPolicy_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddActPolicy_button->Location = System::Drawing::Point(88, 2);
			this->AddActPolicy_button->Margin = System::Windows::Forms::Padding(2);
			this->AddActPolicy_button->Name = L"AddActPolicy_button";
			this->AddActPolicy_button->Size = System::Drawing::Size(45, 24);
			this->AddActPolicy_button->TabIndex = 18;
			this->AddActPolicy_button->Text = L"Add";
			this->AddActPolicy_button->UseVisualStyleBackColor = false;
			this->AddActPolicy_button->Click += gcnew System::EventHandler(this, &Form1::AddInitPolicy_button_Click);
			// 
			// DelActPolicy_button
			// 
			this->DelActPolicy_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelActPolicy_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelActPolicy_button->Location = System::Drawing::Point(168, 2);
			this->DelActPolicy_button->Margin = System::Windows::Forms::Padding(2);
			this->DelActPolicy_button->Name = L"DelActPolicy_button";
			this->DelActPolicy_button->Size = System::Drawing::Size(45, 24);
			this->DelActPolicy_button->TabIndex = 17;
			this->DelActPolicy_button->Text = L"Del";
			this->DelActPolicy_button->UseVisualStyleBackColor = false;
			this->DelActPolicy_button->Click += gcnew System::EventHandler(this, &Form1::AddInitPolicy_button_Click);
			// 
			// ActPolicy_tabControl
			// 
			this->ActPolicy_tabControl->Appearance = System::Windows::Forms::TabAppearance::Buttons;
			this->ActPolicy_tabControl->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ActPolicy_tabControl->Location = System::Drawing::Point(2, 2);
			this->ActPolicy_tabControl->Margin = System::Windows::Forms::Padding(2);
			this->ActPolicy_tabControl->Name = L"ActPolicy_tabControl";
			this->ActPolicy_tabControl->SelectedIndex = 0;
			this->ActPolicy_tabControl->Size = System::Drawing::Size(289, 75);
			this->ActPolicy_tabControl->TabIndex = 19;
			// 
			// ActPolicy_flowLayoutPanel
			// 
			this->ActPolicy_flowLayoutPanel->AutoScroll = true;
			this->ActPolicy_flowLayoutPanel->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->ActPolicy_flowLayoutPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->ActPolicy_flowLayoutPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ActPolicy_flowLayoutPanel->Location = System::Drawing::Point(0, 0);
			this->ActPolicy_flowLayoutPanel->Margin = System::Windows::Forms::Padding(2);
			this->ActPolicy_flowLayoutPanel->Name = L"ActPolicy_flowLayoutPanel";
			this->ActPolicy_flowLayoutPanel->Size = System::Drawing::Size(161, 193);
			this->ActPolicy_flowLayoutPanel->TabIndex = 17;
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::SystemColors::ControlDark;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->fileToolStripMenuItem, 
				this->assignDefaultDataToolStripMenuItem, this->backGroundToolStripMenuItem, this->resolutionToolStripMenuItem, this->cameraTypeToolStripMenuItem, 
				this->viewToolStripMenuItem, this->assistanceToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(2, 2);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(1606, 24);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->openToolStripMenuItem, 
				this->openParticleEmitterGroupToolStripMenuItem, this->saveToolStripMenuItem, this->saveParticleEmitterGroupToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(39, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(331, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// openParticleEmitterGroupToolStripMenuItem
			// 
			this->openParticleEmitterGroupToolStripMenuItem->Name = L"openParticleEmitterGroupToolStripMenuItem";
			this->openParticleEmitterGroupToolStripMenuItem->Size = System::Drawing::Size(331, 22);
			this->openParticleEmitterGroupToolStripMenuItem->Text = L"OpenParticleEmitterGroup//打開粒子群組存檔";
			this->openParticleEmitterGroupToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(331, 22);
			this->saveToolStripMenuItem->Text = L"Save/粒子存檔";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
			// 
			// saveParticleEmitterGroupToolStripMenuItem
			// 
			this->saveParticleEmitterGroupToolStripMenuItem->Name = L"saveParticleEmitterGroupToolStripMenuItem";
			this->saveParticleEmitterGroupToolStripMenuItem->Size = System::Drawing::Size(331, 22);
			this->saveParticleEmitterGroupToolStripMenuItem->Text = L"SaveParticleEmitterGroup/粒子群組存檔";
			this->saveParticleEmitterGroupToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
			// 
			// assignDefaultDataToolStripMenuItem
			// 
			this->assignDefaultDataToolStripMenuItem->Name = L"assignDefaultDataToolStripMenuItem";
			this->assignDefaultDataToolStripMenuItem->Size = System::Drawing::Size(124, 20);
			this->assignDefaultDataToolStripMenuItem->Text = L"AssignDefaultData";
			this->assignDefaultDataToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::assignDefaultDataToolStripMenuItem_Click);
			// 
			// backGroundToolStripMenuItem
			// 
			this->backGroundToolStripMenuItem->Name = L"backGroundToolStripMenuItem";
			this->backGroundToolStripMenuItem->Size = System::Drawing::Size(89, 20);
			this->backGroundToolStripMenuItem->Text = L"BackGround";
			this->backGroundToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::backGroundToolStripMenuItem_Click);
			// 
			// resolutionToolStripMenuItem
			// 
			this->resolutionToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->x1080ToolStripMenuItem, 
				this->toolStripMenuItem, this->x720ToolStripMenuItem, this->x768ToolStripMenuItem, this->x480ToolStripMenuItem, this->x320ToolStripMenuItem});
			this->resolutionToolStripMenuItem->Name = L"resolutionToolStripMenuItem";
			this->resolutionToolStripMenuItem->Size = System::Drawing::Size(80, 20);
			this->resolutionToolStripMenuItem->Text = L"Resolution";
			// 
			// x1080ToolStripMenuItem
			// 
			this->x1080ToolStripMenuItem->Name = L"x1080ToolStripMenuItem";
			this->x1080ToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->x1080ToolStripMenuItem->Text = L"1920X1080";
			this->x1080ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// toolStripMenuItem
			// 
			this->toolStripMenuItem->Name = L"toolStripMenuItem";
			this->toolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->toolStripMenuItem->Text = L"1280X1024";
			this->toolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// x720ToolStripMenuItem
			// 
			this->x720ToolStripMenuItem->Name = L"x720ToolStripMenuItem";
			this->x720ToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->x720ToolStripMenuItem->Text = L"1280X720";
			this->x720ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// x768ToolStripMenuItem
			// 
			this->x768ToolStripMenuItem->Name = L"x768ToolStripMenuItem";
			this->x768ToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->x768ToolStripMenuItem->Text = L"1024X768";
			this->x768ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// x480ToolStripMenuItem
			// 
			this->x480ToolStripMenuItem->Name = L"x480ToolStripMenuItem";
			this->x480ToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->x480ToolStripMenuItem->Text = L"640X480";
			this->x480ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// x320ToolStripMenuItem
			// 
			this->x320ToolStripMenuItem->Name = L"x320ToolStripMenuItem";
			this->x320ToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->x320ToolStripMenuItem->Text = L"480x320";
			this->x320ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::x1080ToolStripMenuItem_Click);
			// 
			// cameraTypeToolStripMenuItem
			// 
			this->cameraTypeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->Orthgnal_ToolStripMenuItem, 
				this->Perspective_ToolStripMenuItem});
			this->cameraTypeToolStripMenuItem->Name = L"cameraTypeToolStripMenuItem";
			this->cameraTypeToolStripMenuItem->Size = System::Drawing::Size(92, 20);
			this->cameraTypeToolStripMenuItem->Text = L"CameraType";
			// 
			// Orthgnal_ToolStripMenuItem
			// 
			this->Orthgnal_ToolStripMenuItem->Name = L"Orthgnal_ToolStripMenuItem";
			this->Orthgnal_ToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->Orthgnal_ToolStripMenuItem->Text = L"Orthgnal";
			this->Orthgnal_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::dToolStripMenuItem1_Click);
			// 
			// Perspective_ToolStripMenuItem
			// 
			this->Perspective_ToolStripMenuItem->Checked = true;
			this->Perspective_ToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->Perspective_ToolStripMenuItem->Name = L"Perspective_ToolStripMenuItem";
			this->Perspective_ToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->Perspective_ToolStripMenuItem->Text = L"Perspective";
			this->Perspective_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::dToolStripMenuItem1_Click);
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->ParticlePolicyPlane_ToolStripMenuItem, 
				this->ParticleDataPlane_ToolStripMenuItem, this->EmiterGroupPlane_ToolStripMenuItem});
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(47, 20);
			this->viewToolStripMenuItem->Text = L"View";
			// 
			// ParticlePolicyPlane_ToolStripMenuItem
			// 
			this->ParticlePolicyPlane_ToolStripMenuItem->Name = L"ParticlePolicyPlane_ToolStripMenuItem";
			this->ParticlePolicyPlane_ToolStripMenuItem->Size = System::Drawing::Size(187, 22);
			this->ParticlePolicyPlane_ToolStripMenuItem->Text = L"Particle Policy Plane";
			this->ParticlePolicyPlane_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::screenToolStripMenuItem_Click);
			// 
			// ParticleDataPlane_ToolStripMenuItem
			// 
			this->ParticleDataPlane_ToolStripMenuItem->Name = L"ParticleDataPlane_ToolStripMenuItem";
			this->ParticleDataPlane_ToolStripMenuItem->Size = System::Drawing::Size(187, 22);
			this->ParticleDataPlane_ToolStripMenuItem->Text = L"Particle Data Plane";
			this->ParticleDataPlane_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::screenToolStripMenuItem_Click);
			// 
			// EmiterGroupPlane_ToolStripMenuItem
			// 
			this->EmiterGroupPlane_ToolStripMenuItem->Name = L"EmiterGroupPlane_ToolStripMenuItem";
			this->EmiterGroupPlane_ToolStripMenuItem->Size = System::Drawing::Size(187, 22);
			this->EmiterGroupPlane_ToolStripMenuItem->Text = L"Emiter Group Plane";
			this->EmiterGroupPlane_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::screenToolStripMenuItem_Click);
			// 
			// assistanceToolStripMenuItem
			// 
			this->assistanceToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->showLineToolStripMenuItem, 
				this->showFPSToolStripMenuItem, this->showPlaneToolStripMenuItem, this->showEmitterToolStripMenuItem});
			this->assistanceToolStripMenuItem->Name = L"assistanceToolStripMenuItem";
			this->assistanceToolStripMenuItem->Size = System::Drawing::Size(77, 20);
			this->assistanceToolStripMenuItem->Text = L"Assistance";
			// 
			// showLineToolStripMenuItem
			// 
			this->showLineToolStripMenuItem->Checked = true;
			this->showLineToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showLineToolStripMenuItem->Name = L"showLineToolStripMenuItem";
			this->showLineToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->showLineToolStripMenuItem->Text = L"ShowLine";
			this->showLineToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::showLineToolStripMenuItem_Click);
			// 
			// showFPSToolStripMenuItem
			// 
			this->showFPSToolStripMenuItem->Checked = true;
			this->showFPSToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showFPSToolStripMenuItem->Name = L"showFPSToolStripMenuItem";
			this->showFPSToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->showFPSToolStripMenuItem->Text = L"ShowFPS";
			this->showFPSToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::showLineToolStripMenuItem_Click);
			// 
			// showPlaneToolStripMenuItem
			// 
			this->showPlaneToolStripMenuItem->Checked = true;
			this->showPlaneToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showPlaneToolStripMenuItem->Name = L"showPlaneToolStripMenuItem";
			this->showPlaneToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->showPlaneToolStripMenuItem->Text = L"ShowPlane";
			this->showPlaneToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::showLineToolStripMenuItem_Click);
			// 
			// showEmitterToolStripMenuItem
			// 
			this->showEmitterToolStripMenuItem->Checked = true;
			this->showEmitterToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showEmitterToolStripMenuItem->Name = L"showEmitterToolStripMenuItem";
			this->showEmitterToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->showEmitterToolStripMenuItem->Text = L"ShowEmitter";
			this->showEmitterToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::showLineToolStripMenuItem_Click);
			// 
			// ParticleControl_tabControl
			// 
			this->ParticleControl_tabControl->Appearance = System::Windows::Forms::TabAppearance::FlatButtons;
			this->ParticleControl_tabControl->Controls->Add(this->Particle);
			this->ParticleControl_tabControl->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ParticleControl_tabControl->Location = System::Drawing::Point(0, 39);
			this->ParticleControl_tabControl->Margin = System::Windows::Forms::Padding(2);
			this->ParticleControl_tabControl->Name = L"ParticleControl_tabControl";
			this->ParticleControl_tabControl->SelectedIndex = 0;
			this->ParticleControl_tabControl->Size = System::Drawing::Size(1618, 643);
			this->ParticleControl_tabControl->TabIndex = 2;
			this->ParticleControl_tabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::ParticleControl_tabControl_SelectedIndexChanged);
			// 
			// MakePanelFor19
			// 
			this->MakePanelFor19->Enabled = true;
			this->MakePanelFor19->Interval = 1000;
			this->MakePanelFor19->Tick += gcnew System::EventHandler(this, &Form1::MakePanelFor19_Tick);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->ClientSize = System::Drawing::Size(1618, 682);
			this->Controls->Add(this->ParticleControl_tabControl);
			this->Controls->Add(this->toolStrip1);
			this->KeyPreview = true;
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Form1";
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyUp);
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->Particle->ResumeLayout(false);
			this->Particle->PerformLayout();
			this->MainHorizontal_splitContainer->Panel2->ResumeLayout(false);
			this->MainHorizontal_splitContainer->ResumeLayout(false);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->ResumeLayout(false);
			this->EmiterListAndDrawPanel_splitContainer->Panel1->PerformLayout();
			this->EmiterListAndDrawPanel_splitContainer->Panel2->ResumeLayout(false);
			this->EmiterListAndDrawPanel_splitContainer->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MaxParticleCount_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->EmitCount_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ParticleCount_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->EmitTimeDis_numericUpDown))->EndInit();
			this->RenderingPlaneAndPolicyPlane_splitContainer->Panel2->ResumeLayout(false);
			this->RenderingPlaneAndPolicyPlane_splitContainer->ResumeLayout(false);
			this->InitAndAct_Policy_splitContainer->Panel1->ResumeLayout(false);
			this->InitAndAct_Policy_splitContainer->Panel2->ResumeLayout(false);
			this->InitAndAct_Policy_splitContainer->ResumeLayout(false);
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Panel1->ResumeLayout(false);
			this->InitPolicyWithInitEditFlowPanel_splitContainer->Panel2->ResumeLayout(false);
			this->InitPolicyWithInitEditFlowPanel_splitContainer->ResumeLayout(false);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->panel1->ResumeLayout(false);
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Panel1->ResumeLayout(false);
			this->ActtPolicyWithActEditFlowPanel_splitContainer->Panel2->ResumeLayout(false);
			this->ActtPolicyWithActEditFlowPanel_splitContainer->ResumeLayout(false);
			this->tableLayoutPanel2->ResumeLayout(false);
			this->panel2->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ParticleControl_tabControl->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void AddInitPolicy_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {			 
			 if( sender == DeleteInitPolicy_button )
			 {
				DelInitPolicy();
			 }
			 else
			 if( sender == AddInitPolicy_button )
			 {
				 AddInitPolicy();
			 }
			 else
			 if( sender == AddActPolicy_button )
			 {
				AddActPolicy();
			 }
			 else
			 if( sender == DelActPolicy_button )
			 {
				DelActPolicy();
			 }
		 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
		    if( !g_pPaticleManager )
		        return;
			DEFAULT_SHADER = L"MyPrtShader";
			NO_TEXTURE_SHADER =L"MyPrtShaderNoTexture";
			cTexture::m_suiLastUsingImageIndex = -1;
			FATMING_CORE::UseShaderProgram();
//			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
//			glClearColor( 0.1f,0.1f,0.2f,1 );
//			glClearDepth(1.0f);
//			POINT l_ViewportSize = { m_pRenderPanel->Width,m_pRenderPanel->Height };
//			glViewport(0,0,l_ViewportSize.x,l_ViewportSize.y);
//			glEnable(GL_DEPTH_TEST);
//			glDisable(GL_CULL_FACE);
//
//				FATMING_CORE::UseShaderProgram(L"MyPrtShaderNoTexture");
//				Setup2DShaderViewProjectionMatrix(m_pFrameCamera->GetWorldViewProjection());
//				SetupShaderColor(Vector4(1,1,1,1));
//					//glEnable(GL_DEPTH_TEST);
//					static float	l_f = 0.f;
//					l_f+=1.f;
//					//m_pFrameCamera->Render();
//					cMatrix44	l_mat = cMatrix44::XAxisRotationMatrix(D3DXToRadian(l_f));
//					RenderPlane(cMatrix44::Identity,100,100,1,1,3);
//					RenderPlane(l_mat,100,100,1,1,3);
//					GLRender::RenderCube(Vector3(10,10,10),cMatrix44::Identity,Vector4(1,1,1,1));
//					RenderCircle(Vector2(0,0),10);
//					if( m_pEmitterEditor )
//						m_pEmitterEditor->MyHintRender(g_sTimeAndFPS.fElpaseTime);
//				DisableShaderProgram(NO_TEXTURE_SHADER);
//			SwapBuffers (m_Hdc);
//			return;
			if(m_pEmitterEditor&& this->m_pEmitterEditor->m_bRfreshParticleEmiterData )
			{
				//refresh particle emitter data
				cPrtEmitter*l_pPrtEmitter = m_pEmitterEditor->m_pParticleEmiterWithShowPosition->GetPrtEmitter();
				EmiteListr_listBox->SelectedIndices->Clear();
				EmiteListr_listBox->SelectedIndex = EmiteListr_listBox->Items->IndexOf(DNCT::WcharToGcstring(l_pPrtEmitter->GetName()));
				//remove the original,assign to current global particle emitter,so we could watch the dynamic particle emitter data changed.
				SAFE_DELETE(l_pPrtEmitter);
				DataInvert_button_Click(sender,e);
				this->m_pEmitterEditor->m_bRfreshParticleEmiterData = false;
			}

			POINT l_ViewportSize = { m_pRenderPanel->Width,m_pRenderPanel->Height };
			glViewport(0,0,l_ViewportSize.x,l_ViewportSize.y);
			g_sTimeAndFPS.Update();

			if(showFPSToolStripMenuItem->Checked)
			{
				if( Perspective_ToolStripMenuItem == sender)
				{
					FPS_label->Text =
						String(g_sTimeAndFPS.GetFPS()).ToString()+",ParticleCount:"+
						g_pPrtEmitter->GetCurrentWorkingParticles().ToString()+DNCT::GetChanglineString()+
						",x:"+g_pOrthogonalCamera->GetMouseWorldPos().x.ToString()+"y:"+g_pOrthogonalCamera->GetMouseWorldPos().y.ToString();
				}
				else
				{
					FPS_label->Text = String(g_sTimeAndFPS.GetFPS()).ToString()+",ParticleCount:"+
						g_pPrtEmitter->GetCurrentWorkingParticles().ToString()+DNCT::GetChanglineString()+
						",x:"+m_pFrameCamera->GetWorldPosition().x.ToString()+
						",y:"+m_pFrameCamera->GetWorldPosition().y.ToString()+
						",z:"+m_pFrameCamera->GetWorldPosition().z.ToString();
				}
			}
			
			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
			glClearColor( 0.1f,0.1f,0.2f,1 );
			glClearDepth(1.0f);
			//The glDepthMask function enables or disables writing into the depth buffer.
			glPushAttrib(GL_ALL_ATTRIB_BITS );
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDisable(GL_TEXTURE_2D);
			glAlphaFunc(GL_GREATER, 0.0001f);
			if( showPlaneToolStripMenuItem->Checked )
			{
				FATMING_CORE::UseShaderProgram(DEFAULT_SHADER);
				if(Perspective_ToolStripMenuItem->Checked == false)
				{
					glDisable(GL_DEPTH_TEST);
					POINT	l_Pos = {0,0};
					g_pOrthogonalCamera->Render();
					g_pOrthogonalCamera->DrawGrid(0,0,Vector4(1.f,1.f,0.3f,1.f));
					GLRender::RenderRectangle(l_Pos,1920,1080,Vector4(0.3f,1.f,0.3f,1.f));
				}
				else
				{
					m_pFrameCamera->Render();
					cMatrix44	l_mat = cMatrix44::XAxisRotationMatrix(D3DXToRadian(90));
					RenderPlane(cMatrix44::TranslationMatrix(Vector3(0,0,-5))*l_mat,10,10,1,1,1,Vector4(0.5,1,0,1));
					//RenderPlane(l_mat,10,10,1,1,1)
					//RenderPlane(cMatrix44::TranslationMatrix(Vector3(0,0,5))*l_mat,10,10,1,1,1);
					if( m_pEmitterEditor )
					{
						glEnable(GL_ALPHA_TEST);
						glEnable(GL_BLEND);
						glDisable(GL_CULL_FACE);
						glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
						m_pEmitterEditor->MyHintRender(g_sTimeAndFPS.fElpaseTime);
					}
				}
				DisableShaderProgram(NO_TEXTURE_SHADER);
			}
			int l_i = glGetError();
			if( l_i != 0 )
			{
				int a=0;
				//assert(0&&"opengl error");
			}
			glDepthMask(GL_FALSE);
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			if( g_pBKImage )
			{
				//glEnable2D(DRAW_PANEL_RESOLUTION_WIDTH,DRAW_PANEL_RESOLUTION_HEIGHT);
				g_pBKImage->Render(0,0);
			}
			UseShaderProgram(L"MyPrtShader");
			RECT l_rc = {0,0,1920,1080};
			if(KillOutRangeParticle_checkBox->Checked)
				g_pPrtEmitter->KillParticleByOutRange(l_rc);
			if(Perspective_ToolStripMenuItem->Checked == true)
			{
				m_pFrameCamera->Render();
			}
			else
			{
				g_pOrthogonalCamera->Render();
			}
			l_i = glGetError();
			if( l_i != 0 )
			{
				int a=0;
				//assert(0&&"opengl error");
			}
			glEnable(GL_BLEND);
			//glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			if( m_pEmitterEditor )
				m_pEmitterEditor->MyRender(g_sTimeAndFPS.fElpaseTime);
			if( *m_pbParticleUpdate )
			{
				l_i = glGetError();
				if( l_i != 0 )
				{
					int a=0;
					//assert(0&&"opengl error");
				}
				glBlendFunc(g_sfactor,g_dfactor);
				g_pPrtEmitter->Update(g_sTimeAndFPS.fElpaseTime);
				g_pPrtEmitter->Render();
				l_i = glGetError();
				if( l_i != 0 )
				{
					int a=0;
					//assert(0&&"opengl error");
				}
			}
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			if( EmitterDirection_checkBox->Checked )
			{
				Vector3 l_vdirection = g_pPrtEmitter->GetEmitDirection();
				float	l_fLength = l_vdirection.Length()*10;
				//glEnable2D(1920,1080);
				//RenderArrow(*g_pPrtEmitter->GetPos(),l_vdirection.Normalize()*l_fLength,5,Vector4::One);
			}
			
			//m_pFrameCamera->DisableRender();
			//GLRender::glDisable2D();
			//glEnable2D(640,480);
			//RenderArrow(Vector3(10,470,0),Vector3(100,0,0),50,Vector4(1,1,0,1));
			//RenderArrow(Vector3(10,470,0),Vector3(0,-100,0),50,Vector4(1,1,0,1));
			//glDisable2D();
			glPopAttrib();
			SwapBuffers (m_Hdc);
			
		 }
private: System::Void EmiterListAndDrawPanel_splitContainer_Panel2_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 m_pRenderPanel->Focus();
		 }
private: System::Void EmiterListAndDrawPanel_splitContainer_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
		 {
			if(e->KeyCode == Keys::R)
				g_pOrthogonalCamera->Reset();
			else
			if( e->KeyCode == Keys::Add  )
			{
				g_pOrthogonalCamera->SetScale(g_pOrthogonalCamera->GetScale()-0.1f);
			}
			else
			if( e->KeyCode == Keys::Subtract )
			{
				g_pOrthogonalCamera->SetScale(g_pOrthogonalCamera->GetScale()+0.1f);
			}
		 }
private: System::Void MyMouseEvent(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 if( !this->timer1->Enabled )
				 return;
			 if( Perspective_ToolStripMenuItem->Checked )
			 {
				if( m_pCameraBehaveByMouseBehave )
				{
					GCFORM::MouseButtons l_MouseButton = e->Button;
					m_pCameraBehaveByMouseBehave->CameraUpDateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,g_sTimeAndFPS.fElpaseTime);
				}
			 }
			 else
			 {
				 if( sender == EmiterListAndDrawPanel_splitContainer )
				 {
					 if( e->Delta>0 )
					 {
						g_pOrthogonalCamera->SetScale(g_pOrthogonalCamera->GetScale()-0.1f);
					 }
					 else
					 if( e->Delta<0 )
					 {
						g_pOrthogonalCamera->SetScale(g_pOrthogonalCamera->GetScale()+0.1f);
					 }
				 }
				 else
				 {
					GCFORM::MouseButtons l_MouseButton = e->Button;
					g_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
						,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)m_pRenderPanel->Size.Width,
						(float)m_pRenderPanel->Size.Height));
				 }
			 }
			 if(g_eEditState == eES_TRANSLATTION)
			 {

				 if ( e->Button == System::Windows::Forms::MouseButtons::Left )
				 {
					 if( Perspective_ToolStripMenuItem->Checked )
					 {
						 g_pPrtEmitter->SetPos(Vector3::Zero);
					 }
					 else
					 {
						 Vector3 l_vMousePos = Vector3(g_pOrthogonalCamera->GetMouseWorldPos().x,
							 g_pOrthogonalCamera->GetMouseWorldPos().y,0.f);
						 g_vEmiterPos = l_vMousePos;
						 g_pPrtEmitter->SetPos(l_vMousePos);
					 }
				 }
			 }
			 else
			 {
				 if(g_eEditState != eES_MOUSE )
				 if ( e->Button == System::Windows::Forms::MouseButtons::Left )
				 {
					 float	l_fx = g_pOrthogonalCamera->GetMouseWorldPos().x-g_vEmiterPos.x;
					 float	l_fy = g_pOrthogonalCamera->GetMouseWorldPos().y-g_vEmiterPos.y;
					 Vector3	l_vDirection = Vector3(l_fx,l_fy,0);
					 g_pPrtEmitter->SetEmitDirection(l_vDirection.Normalize());
				 }
			 }
		 }
private: System::Void Emit_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pbParticleUpdate = true;
			 if(g_pPrtEmitter->GetVelocity())
			 {
				 if( Perspective_ToolStripMenuItem->Checked )
					 g_pPrtEmitter->Emit(Vector3::Zero);
				 else
					g_pPrtEmitter->Emit(g_vEmiterPos);
			 }
		 }
private: System::Void MaxParticleCount_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 g_pPrtEmitter->Stop();
			 g_pPrtEmitter->SetMaxParticleCount(int(MaxParticleCount_numericUpDown->Value));
		 }
private: System::Void Stop_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pbParticleUpdate = false;
			 g_pPrtEmitter->Stop();
		 }
private: System::Void EmitCount_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 g_pPrtEmitter->SetParticleEmitCount((int)EmitCount_numericUpDown->Value);
		 }
private: System::Void ParticleCount_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 g_pPrtEmitter->SetEmitParticleAmount((int)ParticleCount_numericUpDown->Value);
		 }
private: System::Void EmitTimeDis_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 g_pPrtEmitter->SetGapTimeToShot((float)EmitTimeDis_numericUpDown->Value);
		 }
private: System::Void AddTexture_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->timer1->Enabled = false;
			 g_pPrtEmitter->Stop();
			 cli::array<String^>^l_pFileNames = DNCT::OpenFileAndGetNames("Image Files(*.dds;*.png;*.pi)|*.dds;*.png;*.pi|All files (*.*)|*.*");
			 if( l_pFileNames )
			 for each( String^l_strFileName in l_pFileNames )
			 {
				std::wstring l_str = DNCT::GcStringToWchar(DNCT::GetFileNameWithoutFullPath(l_strFileName,true));
				if( g_pPaticleManager->GetImageParser()->GetObject(l_str) )
				{
					WARNING_MSG(l_strFileName+" exists!!");
					continue;
				}
				String^l_strExtensionName = System::IO::Path::GetExtension(l_strFileName);
				if( l_strExtensionName->Equals(".pi") )
				{
					int	l_iCount = g_pPaticleManager->GetImageParser()->Count();
					bool	l_b = g_pPaticleManager->GetImageParser()->Parse(DNCT::GcStringToChar(l_strFileName));
					if( !l_b || g_pPaticleManager->GetImageParser()->Count() == l_iCount )
					{
						WARNING_MSG(l_strFileName+" open failed!!?");
						continue;
					}
					cPuzzleImage*l_pPI = dynamic_cast<cPuzzleImage*>(g_pPaticleManager->GetImageParser()->GetObject(g_pPaticleManager->GetImageParser()->Count()-1));
					this->Texture_treeView->Nodes->Add(PIToTreeNode(l_pPI));
				}
				else
				{
					 cBaseImage*l_pBaseImage = new cBaseImage(DNCT::GcStringToChar(l_strFileName).c_str());
					 if(l_pBaseImage->GetImageIndex())
					 {
						g_pPaticleManager->GetImageParser()->AddObject(l_pBaseImage);
						Texture_treeView->Nodes->Add(String(l_pBaseImage->GetCharName().c_str()).ToString());
					 }
					 else
					 {
						SAFE_DELETE(l_pBaseImage);
						WARNING_MSG("do not support such format\n"+l_strFileName);
					 }
				}
			 }
			 if( m_pCurrentDynamicTexture )
				m_pCurrentDynamicTexture->RefreshPI();
			 this->timer1->Enabled = true;
		 }
private: System::Void assignDefaultDataToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 InitEmiter(true);
		 }
private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == saveToolStripMenuItem )
			 {
				 String^l_strFileName = DNCT::SaveFileAndGetName(ConvertExtensionToFilter(g_pPaticleManager->ExtensionName()).c_str());
				 if( l_strFileName )
				 {
					l_strFileName = ForceAddExtenName(l_strFileName,(char*)g_pPaticleManager->ExtensionName());
					g_pPaticleManager->Export(DNCT::GcStringToChar(l_strFileName).c_str());
				 }
			 }
			 else
			 if( sender == saveParticleEmitterGroupToolStripMenuItem )
			 {
				 if( m_pEmitterEditor )
					this->m_pEmitterEditor->SaveFile();
			 }
		 }
private: System::Void AddEmiter_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strFileName = EmiterName_textBox->Text->Trim();
			 if(l_strFileName->Length)
			 {
				 if(DNCT::CheckListContainStringAndAdd(EmiteListr_listBox,l_strFileName))
				 {
					 WARING_YES_NO_TO_NO("same name replace original one?")
						return;
					 EmiteListr_listBox->Items->Remove(l_strFileName);
					 EmiteListr_listBox->Items->Add(l_strFileName);
					 //now texture has delete if RemoveObje called,so delete it and keep texture alive
					 cPrtEmitter*l_pPrtEmitter = (cPrtEmitter*)g_pPaticleManager->GetObject(DNCT::GcStringToWchar(l_strFileName));
					 g_pPaticleManager->RemoveObjectWithoutDelete(l_pPrtEmitter->GetName());
					 delete l_pPrtEmitter;
				 }
				 cPrtEmitter*l_pPrtEmitter = new cPrtEmitter(g_pPrtEmitter,false);
				 if( !l_pPrtEmitter->GetBaseImage() )
				 {
					 //EmiteListr_listBox->Items->Remove(l_strFileName);
					 //WARNING_MSG("texture assign failed!!");
					 if(this->Texture_treeView->SelectedNode)
					 {
						l_pPrtEmitter->SetBaseImage(dynamic_cast<cBaseImage*>(g_pPaticleManager->GetImageParser()->GetObject(DNCT::GcStringToWchar(Texture_treeView->SelectedNode->Text))));
					 }
				 }
				 l_pPrtEmitter->SetName(DNCT::GcStringToWchar(l_strFileName));
				 g_pPrtEmitter->SetName(DNCT::GcStringToWchar(l_strFileName));
				 g_pPaticleManager->AddObject(l_pPrtEmitter);
				 //if( m_pEmitterEditor )
				//	this->m_pEmitterEditor->RefreshParticle(l_pPrtEmitter);
			 }
			 else
			 {
				WARNING_MSG("please input name");
			 }
		 }
private: System::Void DeleteEmiter_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 for each ( int	l_iSelectedIndex in EmiteListr_listBox->SelectedIndices )
			 {
				if( m_pEmitterEditor )
				{
					if(this->m_pEmitterEditor->IsBehaviorEmitterUsingPrtByName((*g_pPaticleManager)[l_iSelectedIndex]->GetName()))
					{
						WARNING_MSG(DNCT::WcharToGcstring((*g_pPaticleManager)[l_iSelectedIndex]->GetName())+String(" this emitter is used by behavior emitter.").ToString());
						continue;
					}
				}
				g_pPaticleManager->RemoveObject(l_iSelectedIndex);
				EmiteListr_listBox->Items->RemoveAt(l_iSelectedIndex);
			 }
			 EmiteListr_listBox->SelectedIndices->Clear();
		 }
private: System::Void DataInvert_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( EmiteListr_listBox->SelectedIndex != -1 )
			{
				this->timer1->Enabled = false;
				::Threading::Thread::Sleep(100);
				InitEmiter(false);
				cPrtEmitter*l_pPrtEmitter = (*g_pPaticleManager)[EmiteListr_listBox->SelectedIndex];
				g_pPrtEmitter->SetDataByDataString(l_pPrtEmitter->GetDataInfo());
				g_pPrtEmitter->SetName(l_pPrtEmitter->GetName());
				g_pPrtEmitter->SetBaseImage(l_pPrtEmitter->GetBaseImage());
				//velovcity clone
				cPrtVelocityInitSetVelocity*l_pPrtVelocityInitSetVelocity = (cPrtVelocityInitSetVelocity*)g_pPrtEmitter->GetVelocity();
				PrtVelocityInitSetVelocity^l_pGCPrtVelocityInitSetVelocity = gcnew PrtVelocityInitSetVelocity(&l_pPrtVelocityInitSetVelocity->GetVeolcity()->x,
						&l_pPrtVelocityInitSetVelocity->GetVeolcity()->y,
						&l_pPrtVelocityInitSetVelocity->GetVeolcity()->z,l_pPrtVelocityInitSetVelocity->IsRandom(),l_pPrtVelocityInitSetVelocity->GetRandomOffSet());

				InitPolicy_flowLayoutPanel->Controls->Add(l_pGCPrtVelocityInitSetVelocity);

				int	l_iCount = l_pPrtEmitter->GetInitPolicyParticleList()->Count();
				for( int i=0;i<l_iCount;++i )
				{
					cParticleBase*l_pPrtBase = dynamic_cast<cParticleBase*>((*l_pPrtEmitter->GetInitPolicyParticleList())[i]->Clone());
					l_pPrtBase->SetEmitterOwner(g_pPrtEmitter);
					AddPolicyIntoData(l_pPrtBase,true);
				}
				l_iCount = l_pPrtEmitter->GetActPolicyParticleList()->Count();
				for( int i=0;i<l_iCount;++i )
				{
					cParticleBase*l_pPrtBase = dynamic_cast<cParticleBase*>((*l_pPrtEmitter->GetActPolicyParticleList())[i]->Clone());
					l_pPrtBase->SetEmitterOwner(g_pPrtEmitter);
					AddPolicyIntoData(l_pPrtBase,false);
				}
				MaxParticleCount_numericUpDown->Value = l_pPrtEmitter->GetMaxParticleCount();
				g_pPrtEmitter->SetMaxParticleCount(l_pPrtEmitter->GetMaxParticleCount());
				EmitCount_numericUpDown->Value = l_pPrtEmitter->GetParticleEmitCount();
				g_pPrtEmitter->SetParticleEmitCount(l_pPrtEmitter->GetParticleEmitCount());
				ParticleCount_numericUpDown->Value = l_pPrtEmitter->GetEmitParticleAmount();
				g_pPrtEmitter->SetEmitParticleAmount(l_pPrtEmitter->GetEmitParticleAmount());
				EmitTimeDis_numericUpDown->Value = System::Decimal(l_pPrtEmitter->GetGapTimeToShot());
				g_pPrtEmitter->SetGapTimeToShot(l_pPrtEmitter->GetGapTimeToShot());
				int	l_iSrcIndex = BlendingIndexToIndex(l_pPrtEmitter->GetSrcBlendingMode());
				int	l_iDestIndex = BlendingIndexToIndex(l_pPrtEmitter->GetDestBlendingMode());
				SrcBlending_listBox->SelectedIndex = l_iSrcIndex;
				DestBlending_listBox->SelectedIndex = l_iDestIndex;
				if( l_pPrtEmitter->GetBaseImage())
				{
					if( l_pPrtEmitter->GetBaseImage()->Type() == cBaseImage::TypeID||
						l_pPrtEmitter->GetBaseImage()->Type() == cPuzzleImage::TypeID)
					{
						String^l_strTextureName = DNCT::WcharToGcstring(l_pPrtEmitter->GetBaseImage()->GetName());
						Texture_treeView->SelectedNode = Texture_treeView->Nodes[l_strTextureName];
					}
					else
					if(l_pPrtEmitter->GetBaseImage()->Type() == cPuzzleImageUnit::TypeID)
					{
						cPuzzleImageUnit*l_pPuzzleImageUnit = (cPuzzleImageUnit*)l_pPrtEmitter->GetBaseImage();
						String^l_strPIName = DNCT::WcharToGcstring(l_pPuzzleImageUnit->GetOwner()->GetName());
						String^l_strTextureName = DNCT::WcharToGcstring(l_pPrtEmitter->GetBaseImage()->GetName());
						Texture_treeView->SelectedNode = Texture_treeView->Nodes[l_strPIName]->Nodes[l_strTextureName];					
					}
					else
					{
						assert(0&&"unknow texture type!");
					}
				}
				else
				{
					Texture_treeView->SelectedNode = nullptr;
				}
				if( m_pEmitterEditor )
					this->m_pEmitterEditor->ReassignParticleEmiterWithShowPositionData(this->m_pEmitterEditor->m_bRfreshParticleEmiterData);
				this->timer1->Enabled = true;
			}
		 }
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
		     this->timer1->Enabled = false;
			 if( sender == openParticleEmitterGroupToolStripMenuItem )
			 {
				 if( m_pEmitterEditor )
				 {
					if(this->m_pEmitterEditor->OpenFile())
					{
						InitEmiter(true);
						TextureRefresh();
					}
				 }
			 }
			 else
			 if( sender == openToolStripMenuItem )
			 {
				 String^l_str = DNCT::OpenFileAndGetName(ConbineFileDescribtionWithExtensionToFilter("Particle Files",g_pPaticleManager->ExtensionName(),true).c_str());
				 if( l_str )
				 {
					 //SAFE_DELETE(g_pPaticleManager);
					 //g_pPaticleManager = new cPaticleManager();
					 //because path file
					 g_pPaticleManager->Destroy();
					 InitEmiter(false);
					 bool	l_b = g_pPaticleManager->Parse(DNCT::GcStringToChar(l_str));
					 if( l_b )
					 {
						TextureRefresh();
					 }
				 }
			 }
			 this->timer1->Enabled = true;
		 }
private: System::Void SrcBlending_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( SrcBlending_listBox->SelectedIndex != -1 )
			 {
				 g_sfactor = IndexToBlendingIndex(SrcBlending_listBox->SelectedIndex);
				 if( g_pPrtEmitter )
					g_pPrtEmitter->SetSrcBlendingMode(g_sfactor);
			 }
		 }
private: System::Void DestBlending_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( DestBlending_listBox->SelectedIndex != -1 )
			 {
				 g_dfactor = IndexToBlendingIndex(DestBlending_listBox->SelectedIndex);
				 if( g_pPrtEmitter )
					g_pPrtEmitter->SetDestBlendingMode(g_dfactor);
			 }
		 }
private: System::Void backGroundToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_str = DNCT::OpenFileAndGetName();
			 if( l_str )
			 {
				SAFE_DELETE(g_pBKImage);
				g_pBKImage = new cBaseImage(DNCT::GcStringToChar(l_str).c_str());
			 }
		 }
private: System::Void x1080ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if( sender == x1080ToolStripMenuItem )
			 {
				DRAW_PANEL_RESOLUTION_WIDTH = 1920.f;
				DRAW_PANEL_RESOLUTION_HEIGHT = 1080.f;
			 }
			 else
			 if( sender == x720ToolStripMenuItem )
			 {
				DRAW_PANEL_RESOLUTION_WIDTH = 1280.f;
				DRAW_PANEL_RESOLUTION_HEIGHT = 720.f;
			 }
			 else
			 if( sender == x768ToolStripMenuItem )
			 {
				DRAW_PANEL_RESOLUTION_WIDTH = 1024.f;
				DRAW_PANEL_RESOLUTION_HEIGHT = 768.f;
			 }
			 else
			 if( sender == x480ToolStripMenuItem )
			 {
				DRAW_PANEL_RESOLUTION_WIDTH = 640.f;
				DRAW_PANEL_RESOLUTION_HEIGHT = 480.f;
			 }
			 else
			 if( sender == toolStripMenuItem )
			 {
				DRAW_PANEL_RESOLUTION_WIDTH = 1280.f;
				DRAW_PANEL_RESOLUTION_HEIGHT = 1024.f;
			 }
			 else
			 if( sender == x320ToolStripMenuItem )
			 {
				DRAW_PANEL_RESOLUTION_WIDTH = 480.f;
				DRAW_PANEL_RESOLUTION_HEIGHT = 320.f;
			 }
			 Projection	l_ProjectionMV;
			 l_ProjectionMV.SetFovYAspect( XM_PIDIV4 ,DRAW_PANEL_RESOLUTION_WIDTH/DRAW_PANEL_RESOLUTION_HEIGHT, 1000.0f * 1e-4f,2000000.f);
			 m_pFrameCamera->SetProjection(l_ProjectionMV);
			 g_pOrthogonalCamera->SetResolution(Vector2(DRAW_PANEL_RESOLUTION_WIDTH,DRAW_PANEL_RESOLUTION_HEIGHT));
			 g_pOrthogonalCamera->Reset();
			 if( RenderingPlaneAndPolicyPlane_splitContainer->Width>DRAW_PANEL_RESOLUTION_WIDTH&&EmiterListAndDrawPanel_splitContainer->Height>DRAW_PANEL_RESOLUTION_WIDTH )
			 {
			    RenderingPlaneAndPolicyPlane_splitContainer->SplitterDistance = (int)DRAW_PANEL_RESOLUTION_WIDTH;
			    EmiterListAndDrawPanel_splitContainer->SplitterDistance = (int)(EmiterListAndDrawPanel_splitContainer->Height - DRAW_PANEL_RESOLUTION_HEIGHT);
			 }
             else
             {
                float l_fNewWidth = DRAW_PANEL_RESOLUTION_WIDTH/1.5f;
                float l_fNewHeight = DRAW_PANEL_RESOLUTION_HEIGHT/1.5f;
                try
                {
			        RenderingPlaneAndPolicyPlane_splitContainer->SplitterDistance = (int)l_fNewWidth;
			        EmiterListAndDrawPanel_splitContainer->SplitterDistance = (int)(EmiterListAndDrawPanel_splitContainer->Height - l_fNewHeight);
			    }
			    catch(System::Exception^l_pExp)
			    {
                    l_fNewWidth /= 1.5f;
                    l_fNewHeight /= 1.5f;
			        RenderingPlaneAndPolicyPlane_splitContainer->SplitterDistance = (int)l_fNewWidth;
			        EmiterListAndDrawPanel_splitContainer->SplitterDistance = (int)(EmiterListAndDrawPanel_splitContainer->Height - l_fNewHeight);			    
					OutputDebugString(DNCT::GcStringToWchar(l_pExp->ToString()).c_str());
			    }
             }

		 }
private: System::Void DeleteTexture_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( RemoveTextureFromSelectedNodesTreeView(Texture_treeView,g_pPaticleManager->GetImageParser()) )
			 {
			    Texture_treeView->SelectedNode->Remove();
			 }
		 }
private: System::Void DisableTexture_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 g_pPrtEmitter->SetBaseImage(nullptr);
		 }
private: System::Void ParticleType_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( !g_pPrtEmitter )
				 return;
			 if( ParticleType_listBox->SelectedIndex == 0 )
				 g_pPrtEmitter->SetPrimitiveType(GL_POINTS);
			 else
			 if( ParticleType_listBox->SelectedIndex == 1 )
				 g_pPrtEmitter->SetPrimitiveType(GL_QUADS);

		 }
private: System::Void ParticleControl_tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 timer1->Enabled = false;
			 if( ParticleControl_tabControl->SelectedIndex == 0 )
			 {
				if( m_Hdc && m_pHGLRC  )
				{
					wglMakeCurrent( m_Hdc,m_pHGLRC );
					timer1->Enabled = true;
				}
			 }
		 }
private: System::Void Mouse_toolStripButton_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			System::Windows::Forms::ToolStripButton^l_pButton  =(System::Windows::Forms::ToolStripButton^)sender;
			if(l_pButton->BackColor == System::Drawing::Color::LightBlue)
				return;
			l_pButton->BackColor = System::Drawing::Color::LightBlue;
			switch( g_eEditState )
			{
				case eES_MOUSE:
					Mouse_toolStripButton->BackColor = System::Drawing::Color::Transparent;
					break;
				case eES_TRANSLATTION:
					Translation_toolStripButton->BackColor = System::Drawing::Color::Transparent;
					break;
				case eES_ROTATIONZ:
					Rotation_toolStripButton->BackColor = System::Drawing::Color::Transparent;
					break;
			}
			if( Translation_toolStripButton == sender )
			{
				g_eEditState = eES_TRANSLATTION;
			}
			else
			if( Rotation_toolStripButton == sender )
			{
				g_eEditState = eES_ROTATIONZ;
			}
			else
			if( Mouse_toolStripButton == sender )
			{
				g_eEditState = eES_MOUSE;
			}
		 }

private: System::Void toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 g_bCameraResetClick = true;
			 if(ParticleControl_tabControl->SelectedTab == this->Particle )
			 {
				 g_pOrthogonalCamera->Reset();
				 m_pFrameCamera->SetWorldTransform(cMatrix44::Identity);
			 }
			 else
			 //if(ParticleControl_tabControl->SelectedIndex == 1 )
			 {
				VECTOR4	l_vPos = {0,1,6.5,1};
				if( m_pFrameCamera )
				{
					m_pFrameCamera->SetWorldTransform(cMatrix44::Identity);
					m_pFrameCamera->SetWorldPosition(l_vPos);
				}
			 }
		 }
private: System::Void Policy_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 GCFORM::ListBox^l_pListBox = (ListBox^)sender;
			 if( l_pListBox->SelectedIndex != -1 )
			 {
				 std::string	l_str = GetUsageExplanaing(DNCT::GcStringToWchar(l_pListBox->SelectedItem->ToString()).c_str());
				 PolictHin_toolTip->Show(String(l_str.c_str()).ToString(),l_pListBox);
			 }
		 }
private: System::Void dToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( Perspective_ToolStripMenuItem == sender)
			{
				Perspective_ToolStripMenuItem->Checked = true;
				Orthgnal_ToolStripMenuItem->Checked = false;
			}
			else
			{
				Perspective_ToolStripMenuItem->Checked = false;
				Orthgnal_ToolStripMenuItem->Checked = true;
			}
		 }
private: System::Void screenToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == ParticlePolicyPlane_ToolStripMenuItem )
			 {
				 EmiterListAndDrawPanel_splitContainer->Panel1Collapsed = !EmiterListAndDrawPanel_splitContainer->Panel1Collapsed;
			 }
			 else
			 if( sender == ParticleDataPlane_ToolStripMenuItem )
			 {
				 this->RenderingPlaneAndPolicyPlane_splitContainer->Panel2Collapsed = !this->RenderingPlaneAndPolicyPlane_splitContainer->Panel2Collapsed;
			 }
			 else
			 if( sender == EmiterGroupPlane_ToolStripMenuItem )
			 {
				 MainHorizontal_splitContainer->Panel1Collapsed = !MainHorizontal_splitContainer->Panel1Collapsed;
			 }
		 }
private: System::Void Form1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( e->Control )//s for 83,49 for 1
			 {
				System::EventArgs^  e2;
				System::Object^  sender2;
				if( e->KeyValue == 50 )//2
				{
					sender2 = ParticlePolicyPlane_ToolStripMenuItem;	 
				}
				else
				if( e->KeyValue == 51 )//3
				{
					sender2 = ParticleDataPlane_ToolStripMenuItem;
				}
				else
				if( e->KeyValue == 49 )//1
				{
					sender2 = EmiterGroupPlane_ToolStripMenuItem;
				}
				else
				if( e->KeyValue == 52 )//4
				{
					if( EmiterListAndDrawPanel_splitContainer->Panel1Collapsed&&
						RenderingPlaneAndPolicyPlane_splitContainer->Panel2Collapsed&&
						MainHorizontal_splitContainer->Panel1Collapsed)
					{
						EmiterListAndDrawPanel_splitContainer->Panel1Collapsed = false;
						this->RenderingPlaneAndPolicyPlane_splitContainer->Panel2Collapsed = false;
						MainHorizontal_splitContainer->Panel1Collapsed = false;
					}
					else
					{
						EmiterListAndDrawPanel_splitContainer->Panel1Collapsed = true;
						this->RenderingPlaneAndPolicyPlane_splitContainer->Panel2Collapsed = true;
						MainHorizontal_splitContainer->Panel1Collapsed = true;
					}
				}
				
				if( sender2 )
					screenToolStripMenuItem_Click(sender2,e2);
			 }
			 if(GCFORM::Keys::Add == e->KeyCode)
			 {
				 m_pCameraBehaveByMouseBehave->SetMouseMoveSpeed(m_pCameraBehaveByMouseBehave->GetMouseMoveSpeed()*2);
			 }
			 else
			 if(GCFORM::Keys::Subtract == e->KeyCode)
			 {
				m_pCameraBehaveByMouseBehave->SetMouseMoveSpeed(m_pCameraBehaveByMouseBehave->GetMouseMoveSpeed()/2);
			 }
		 }
private: System::Void EmiteListr_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			DataInvert_button_Click(sender,e);
		 }
private: System::Void showLineToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			System::Windows::Forms::ToolStripMenuItem^l_pToolStripMenuItem = (System::Windows::Forms::ToolStripMenuItem^)sender;
			l_pToolStripMenuItem->Checked = !l_pToolStripMenuItem->Checked;
			if( sender == showLineToolStripMenuItem )
			{
				if( m_pEmitterEditor )
					this->m_pEmitterEditor->m_bShowLine = l_pToolStripMenuItem->Checked;
			}
			else
			if( sender == showEmitterToolStripMenuItem )
			{
				if( m_pEmitterEditor )
					m_pEmitterEditor->m_bShowEmitter = l_pToolStripMenuItem->Checked;
			}
		 }
private: System::Void MakePanelFor19_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->MakePanelFor19->Enabled = false;
			 EmiterListAndDrawPanel_splitContainer->SplitterDistance = 194;
			 this->MainHorizontal_splitContainer->SplitterDistance = 256+5;
		 }
private: System::Void EmiteListr_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( e->KeyCode == GCFORM::Keys::Enter )
			 {
				DataInvert_button_Click(sender,e);
			 }
		 }
private: System::Void EmiteListr_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( EmiteListr_listBox->SelectedItem )
			 {
				 String^l_str = EmiteListr_listBox->SelectedItem->ToString();
				 EmiterName_textBox->Text = l_str;
			 }
		 }
private: System::Void Texture_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		 {
			 if( Texture_treeView->SelectedNode )
			 {
				 String^l_strName = Texture_treeView->SelectedNode->Text;
				 std::wstring l_strTextureName = DNCT::GcStringToWchar(l_strName);
 				 if( !Texture_treeView->SelectedNode->Parent )//simple gl texture
				 {
					 std::wstring l_strTextureName = DNCT::GcStringToWchar(l_strName);//it could be simple texture or pi texture!.
					 g_pPrtEmitter->SetBaseImage(dynamic_cast<cBaseImage*>(g_pPaticleManager->GetImageParser()->GetObject(l_strTextureName)));
				 }
				 else//pi
				 {
					String^l_strParentName = Texture_treeView->SelectedNode->Parent->Text;
					l_strTextureName = DNCT::GcStringToWchar(l_strParentName);
					cPuzzleImage*l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(g_pPaticleManager->GetImageParser()->GetObject(l_strTextureName));
					l_strTextureName = DNCT::GcStringToWchar(l_strName);
					g_pPrtEmitter->SetBaseImage(l_pPuzzleImage->GetObject(l_strTextureName));
				 }
			 }
		 }
};
}