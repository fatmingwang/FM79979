#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

extern UT::sTimeAndFPS g_sTimeAndFPS;
extern GLenum g_sfactor;
extern GLenum g_dfactor;
extern float	DRAW_PANEL_RESOLUTION_WIDTH;
extern float	DRAW_PANEL_RESOLUTION_HEIGHT;
extern bool	g_bInvertTextureAdress_V;//bmp texture adress V is invert
extern bool	g_bCameraResetClick;
void	DrawAxisArrow(cFrameCamera*	e_pFrameCamera);
namespace FATMING_CORE
{
	extern char*g_strMyMeshVS;
	extern char*g_strMyMeshFS;
	extern char*g_strMySkinningMeshVS;
	extern char*g_strMySkinningMeshFS;
}

namespace ParticalEditor {

	/// <summary>
	/// MeshViewer 的摘要
	/// </summary>
	public ref class MeshViewer : public System::Windows::Forms::UserControl
	{
	public:
		MeshViewer(System::Windows::Forms::TabControl^e_pTabControl )
		{
			InitializeComponent();
			m_pTabControl = e_pTabControl;
			this->Dock = GCFORM::DockStyle::Fill;
			m_pTabPage = gcnew GCFORM::TabPage;
			m_pTabPage->Text = "MeshViewer";
			m_pTabPage->Controls->Add(this);
			e_pTabControl->TabPages->Add(m_pTabPage);
			e_pTabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &MeshViewer::tabControl_SelectedIndexChanged);
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_HdcMV = GetDC((HWND)this->splitContainer1->Panel2->Handle.ToPointer());
			m_pHGLRCMV = UT::InitOpenGL((HWND)this->splitContainer1->Panel2->Handle.ToPointer(),true,m_HdcMV);
			
			m_pAllMesh = new cNamedTypedObjectVector<cMesh>;
			Projection	l_ProjectionMV;
			l_ProjectionMV.SetFovYAspect( XM_PIDIV4 ,1920/1080.f, 1000.0f * 1e-4f,2000000.f);
			VECTOR4	l_vPos = {0,1,6.5,1};
			m_pFrameCamera = new cFrameCamera();
			m_pFrameCamera->SetWorldTransform(cMatrix44::Identity);
			m_pFrameCamera->SetWorldPosition(l_vPos);
			m_pFrameCamera->SetProjection(l_ProjectionMV);
			m_pCameraBehaveByMouseBehave = new cCameraBehaveByMouseBehave(m_pFrameCamera);
			this->splitContainer1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MeshViewer::MyMouseEvent);
			this->splitContainer1->Panel2->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MeshViewer::MyMouseEvent);
			this->splitContainer1->Panel2->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MeshViewer::MyMouseEvent);
			this->splitContainer1->Panel2->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MeshViewer::MyMouseEvent);
			//wglMakeCurrent( m_HdcMV,m_pHGLRCMV );

			STATIC_MESH_SHADER = L"StaticMeshShader";
			SKINNING_MESH_SHADER = L"SkinningMeshShader";
			DEFAULT_SHADER = L"MeshShader";
			NO_TEXTURE_SHADER = L"MeshShader_NoTexture";

			FATMING_CORE::CreateShader(g_bMyMeshVSClientState,g_strMyMeshVS,g_strMyMeshFS,STATIC_MESH_SHADER);
			FATMING_CORE::CreateShader(g_bMySkinningMeshVSClientState,g_strMySkinningMeshVS,g_strMySkinningMeshFS,SKINNING_MESH_SHADER);
			FATMING_CORE::CreateShader(g_bCommonVSNoTextureClientState,NO_TEXTURE_SHADER);
			FATMING_CORE::CreateShader(g_bCommonVSClientState,DEFAULT_SHADER);
			cGameApp::m_spGlyphFontRender = new cGlyphFontRender("Font");
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MeshViewer()
		{
			if (components)
			{
				delete components;
			}
			STATIC_MESH_SHADER = L"StaticMeshShader";
			SKINNING_MESH_SHADER = L"SkinningMeshShader";
			DEFAULT_SHADER = L"MeshShader";
			NO_TEXTURE_SHADER = L"MeshShader_NoTexture";
			FATMING_CORE::DeleteShader(STATIC_MESH_SHADER);
			FATMING_CORE::DeleteShader(SKINNING_MESH_SHADER);
			FATMING_CORE::DeleteShader(NO_TEXTURE_SHADER);
			FATMING_CORE::DeleteShader(DEFAULT_SHADER);
			SAFE_DELETE(m_pAllMesh);
			SAFE_DELETE(m_pColladaParser);
			SAFE_DELETE(m_pFrameCamera);
			SAFE_DELETE(m_pCameraBehaveByMouseBehave);
			SAFE_DELETE(cGameApp::m_spGlyphFontRender);
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::TextBox^  CameraInfo_textBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer3;
	private: System::Windows::Forms::TextBox^  ColladaParsingData_textBox;
	private: System::Windows::Forms::ToolStripMenuItem^  invertTextureAdressToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  trueToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  Animation_checkBox;
	private: System::Windows::Forms::CheckBox^  RenderSkeleton_checkBox;
	private: System::Windows::Forms::TrackBar^  AnimationProgress_trackBar;
	private: System::Windows::Forms::CheckBox^  ShowAnimationMesh_checkBox;
	private: System::Windows::Forms::NumericUpDown^  SpecificJointAnimation_numericUpDown;
	private: System::Windows::Forms::CheckBox^  SpecificJointAnimation_checkBox;
	private: System::Windows::Forms::Label^  JointName_label;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  Speed_numericUpDown;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  StartJoint_numericUpDown;
	private: System::Windows::Forms::CheckBox^  Mesh_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  savePathFileToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  Update_WithSpecificJoint_checkBox;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::ToolStripMenuItem^  cameraToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  Camera2d_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  Camera3d_ToolStripMenuItem;
	//my
			GCFORM::TabPage^m_pTabPage;
			GCFORM::TabControl^m_pTabControl;
			HGLRC	m_pHGLRCMV;
			HDC		m_HdcMV;
			cNamedTypedObjectVector<cMesh>*	m_pAllMesh;
			cColladaParser*	m_pColladaParser;
			cFrameCamera*m_pFrameCamera;


		 cCameraBehaveByMouseBehave *m_pCameraBehaveByMouseBehave;
	public:void MakeCurrentTabPage()
		   {
				m_pTabControl->SelectedIndex = m_pTabControl->TabPages->IndexOf(this->m_pTabPage);
				wglMakeCurrent( m_HdcMV,m_pHGLRCMV );
				timer1->Enabled = true;		   
		   }
	private:
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
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->Update_WithSpecificJoint_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->StartJoint_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->Speed_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->JointName_label = (gcnew System::Windows::Forms::Label());
			this->SpecificJointAnimation_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->SpecificJointAnimation_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ShowAnimationMesh_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Mesh_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->AnimationProgress_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->RenderSkeleton_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Animation_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->savePathFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->invertTextureAdressToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->trueToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cameraToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Camera2d_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Camera3d_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainer3 = (gcnew System::Windows::Forms::SplitContainer());
			this->CameraInfo_textBox = (gcnew System::Windows::Forms::TextBox());
			this->ColladaParsingData_textBox = (gcnew System::Windows::Forms::TextBox());
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartJoint_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Speed_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SpecificJointAnimation_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AnimationProgress_trackBar))->BeginInit();
			this->menuStrip1->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->splitContainer3->Panel1->SuspendLayout();
			this->splitContainer3->Panel2->SuspendLayout();
			this->splitContainer3->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->label3);
			this->splitContainer1->Panel1->Controls->Add(this->Update_WithSpecificJoint_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->label2);
			this->splitContainer1->Panel1->Controls->Add(this->StartJoint_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->Speed_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->JointName_label);
			this->splitContainer1->Panel1->Controls->Add(this->SpecificJointAnimation_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->SpecificJointAnimation_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->ShowAnimationMesh_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->Mesh_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->AnimationProgress_trackBar);
			this->splitContainer1->Panel1->Controls->Add(this->RenderSkeleton_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->Animation_checkBox);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MeshViewer::splitContainer1_Panel2_MouseClick);
			this->splitContainer1->Size = System::Drawing::Size(771, 412);
			this->splitContainer1->SplitterDistance = 178;
			this->splitContainer1->SplitterWidth = 3;
			this->splitContainer1->TabIndex = 0;
			this->splitContainer1->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MeshViewer::splitContainer1_KeyUp);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(69, 138);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(11, 12);
			this->label3->TabIndex = 15;
			this->label3->Text = L"0";
			// 
			// Update_WithSpecificJoint_checkBox
			// 
			this->Update_WithSpecificJoint_checkBox->AutoSize = true;
			this->Update_WithSpecificJoint_checkBox->Location = System::Drawing::Point(14, 280);
			this->Update_WithSpecificJoint_checkBox->Name = L"Update_WithSpecificJoint_checkBox";
			this->Update_WithSpecificJoint_checkBox->Size = System::Drawing::Size(71, 16);
			this->Update_WithSpecificJoint_checkBox->TabIndex = 14;
			this->Update_WithSpecificJoint_checkBox->Text = L"AllUpdate";
			this->Update_WithSpecificJoint_checkBox->UseVisualStyleBackColor = true;
			this->Update_WithSpecificJoint_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MeshViewer::Update_WithSpecificJoint_checkBox_CheckedChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(11, 362);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(48, 12);
			this->label2->TabIndex = 13;
			this->label2->Text = L"StartJoint";
			// 
			// StartJoint_numericUpDown
			// 
			this->StartJoint_numericUpDown->Location = System::Drawing::Point(14, 376);
			this->StartJoint_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->StartJoint_numericUpDown->Name = L"StartJoint_numericUpDown";
			this->StartJoint_numericUpDown->Size = System::Drawing::Size(90, 22);
			this->StartJoint_numericUpDown->TabIndex = 12;
			this->StartJoint_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MeshViewer::StartJoint_numericUpDown_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(11, 309);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(33, 12);
			this->label1->TabIndex = 11;
			this->label1->Text = L"Speed";
			// 
			// Speed_numericUpDown
			// 
			this->Speed_numericUpDown->Location = System::Drawing::Point(14, 323);
			this->Speed_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->Speed_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, System::Int32::MinValue});
			this->Speed_numericUpDown->Name = L"Speed_numericUpDown";
			this->Speed_numericUpDown->Size = System::Drawing::Size(97, 22);
			this->Speed_numericUpDown->TabIndex = 10;
			// 
			// JointName_label
			// 
			this->JointName_label->AutoSize = true;
			this->JointName_label->Location = System::Drawing::Point(9, 227);
			this->JointName_label->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->JointName_label->Name = L"JointName_label";
			this->JointName_label->Size = System::Drawing::Size(54, 12);
			this->JointName_label->TabIndex = 9;
			this->JointName_label->Text = L"JointName";
			// 
			// SpecificJointAnimation_numericUpDown
			// 
			this->SpecificJointAnimation_numericUpDown->Location = System::Drawing::Point(12, 204);
			this->SpecificJointAnimation_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->SpecificJointAnimation_numericUpDown->Name = L"SpecificJointAnimation_numericUpDown";
			this->SpecificJointAnimation_numericUpDown->Size = System::Drawing::Size(90, 22);
			this->SpecificJointAnimation_numericUpDown->TabIndex = 8;
			this->SpecificJointAnimation_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MeshViewer::SpecificJointAnimation_numericUpDown_ValueChanged);
			// 
			// SpecificJointAnimation_checkBox
			// 
			this->SpecificJointAnimation_checkBox->AutoSize = true;
			this->SpecificJointAnimation_checkBox->Location = System::Drawing::Point(12, 184);
			this->SpecificJointAnimation_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->SpecificJointAnimation_checkBox->Name = L"SpecificJointAnimation_checkBox";
			this->SpecificJointAnimation_checkBox->Size = System::Drawing::Size(132, 16);
			this->SpecificJointAnimation_checkBox->TabIndex = 7;
			this->SpecificJointAnimation_checkBox->Text = L"SpecificJointAnimation";
			this->SpecificJointAnimation_checkBox->UseVisualStyleBackColor = true;
			this->SpecificJointAnimation_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MeshViewer::SpecificJointAnimation_checkBox_CheckedChanged);
			// 
			// ShowAnimationMesh_checkBox
			// 
			this->ShowAnimationMesh_checkBox->AutoSize = true;
			this->ShowAnimationMesh_checkBox->Location = System::Drawing::Point(20, 22);
			this->ShowAnimationMesh_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->ShowAnimationMesh_checkBox->Name = L"ShowAnimationMesh_checkBox";
			this->ShowAnimationMesh_checkBox->Size = System::Drawing::Size(124, 16);
			this->ShowAnimationMesh_checkBox->TabIndex = 6;
			this->ShowAnimationMesh_checkBox->Text = L"ShowAnimationMesh";
			this->ShowAnimationMesh_checkBox->UseVisualStyleBackColor = true;
			// 
			// Mesh_checkBox
			// 
			this->Mesh_checkBox->AutoSize = true;
			this->Mesh_checkBox->Checked = true;
			this->Mesh_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->Mesh_checkBox->Location = System::Drawing::Point(18, 2);
			this->Mesh_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->Mesh_checkBox->Name = L"Mesh_checkBox";
			this->Mesh_checkBox->Size = System::Drawing::Size(75, 16);
			this->Mesh_checkBox->TabIndex = 5;
			this->Mesh_checkBox->Text = L"ShowMesh";
			this->Mesh_checkBox->UseVisualStyleBackColor = true;
			// 
			// AnimationProgress_trackBar
			// 
			this->AnimationProgress_trackBar->Location = System::Drawing::Point(14, 105);
			this->AnimationProgress_trackBar->Margin = System::Windows::Forms::Padding(2);
			this->AnimationProgress_trackBar->Name = L"AnimationProgress_trackBar";
			this->AnimationProgress_trackBar->Size = System::Drawing::Size(163, 45);
			this->AnimationProgress_trackBar->TabIndex = 4;
			this->AnimationProgress_trackBar->Scroll += gcnew System::EventHandler(this, &MeshViewer::AnimationProgress_trackBar_Scroll);
			this->AnimationProgress_trackBar->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MeshViewer::AnimationProgress_trackBar_MouseDown);
			// 
			// RenderSkeleton_checkBox
			// 
			this->RenderSkeleton_checkBox->AutoSize = true;
			this->RenderSkeleton_checkBox->Location = System::Drawing::Point(20, 71);
			this->RenderSkeleton_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->RenderSkeleton_checkBox->Name = L"RenderSkeleton_checkBox";
			this->RenderSkeleton_checkBox->Size = System::Drawing::Size(98, 16);
			this->RenderSkeleton_checkBox->TabIndex = 3;
			this->RenderSkeleton_checkBox->Text = L"RenderSkeleton";
			this->RenderSkeleton_checkBox->UseVisualStyleBackColor = true;
			// 
			// Animation_checkBox
			// 
			this->Animation_checkBox->AutoSize = true;
			this->Animation_checkBox->Location = System::Drawing::Point(20, 51);
			this->Animation_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->Animation_checkBox->Name = L"Animation_checkBox";
			this->Animation_checkBox->Size = System::Drawing::Size(73, 16);
			this->Animation_checkBox->TabIndex = 0;
			this->Animation_checkBox->Text = L"Animation";
			this->Animation_checkBox->UseVisualStyleBackColor = true;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->fileToolStripMenuItem, 
				this->invertTextureAdressToolStripMenuItem, this->cameraToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(771, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
				this->savePathFileToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(39, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MeshViewer::openToolStripMenuItem_Click);
			// 
			// savePathFileToolStripMenuItem
			// 
			this->savePathFileToolStripMenuItem->Name = L"savePathFileToolStripMenuItem";
			this->savePathFileToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->savePathFileToolStripMenuItem->Text = L"SavePathFile";
			this->savePathFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MeshViewer::savePathFileToolStripMenuItem_Click);
			// 
			// invertTextureAdressToolStripMenuItem
			// 
			this->invertTextureAdressToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->trueToolStripMenuItem});
			this->invertTextureAdressToolStripMenuItem->Name = L"invertTextureAdressToolStripMenuItem";
			this->invertTextureAdressToolStripMenuItem->Size = System::Drawing::Size(130, 20);
			this->invertTextureAdressToolStripMenuItem->Text = L"InvertTextureAdress";
			// 
			// trueToolStripMenuItem
			// 
			this->trueToolStripMenuItem->Name = L"trueToolStripMenuItem";
			this->trueToolStripMenuItem->Size = System::Drawing::Size(101, 22);
			this->trueToolStripMenuItem->Text = L"True";
			this->trueToolStripMenuItem->Click += gcnew System::EventHandler(this, &MeshViewer::invertTextureAdressToolStripMenuItem_Click);
			// 
			// cameraToolStripMenuItem
			// 
			this->cameraToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->Camera2d_ToolStripMenuItem, 
				this->Camera3d_ToolStripMenuItem});
			this->cameraToolStripMenuItem->Name = L"cameraToolStripMenuItem";
			this->cameraToolStripMenuItem->Size = System::Drawing::Size(64, 20);
			this->cameraToolStripMenuItem->Text = L"Camera";
			// 
			// Camera2d_ToolStripMenuItem
			// 
			this->Camera2d_ToolStripMenuItem->Name = L"Camera2d_ToolStripMenuItem";
			this->Camera2d_ToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->Camera2d_ToolStripMenuItem->Text = L"2D";
			this->Camera2d_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MeshViewer::dToolStripMenuItem1_Click);
			// 
			// Camera3d_ToolStripMenuItem
			// 
			this->Camera3d_ToolStripMenuItem->Checked = true;
			this->Camera3d_ToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->Camera3d_ToolStripMenuItem->Name = L"Camera3d_ToolStripMenuItem";
			this->Camera3d_ToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->Camera3d_ToolStripMenuItem->Text = L"3D";
			this->Camera3d_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MeshViewer::dToolStripMenuItem1_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &MeshViewer::timer1_Tick);
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 24);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->splitContainer3);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->splitContainer1);
			this->splitContainer2->Size = System::Drawing::Size(771, 490);
			this->splitContainer2->SplitterDistance = 75;
			this->splitContainer2->SplitterWidth = 3;
			this->splitContainer2->TabIndex = 2;
			// 
			// splitContainer3
			// 
			this->splitContainer3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer3->Location = System::Drawing::Point(0, 0);
			this->splitContainer3->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer3->Name = L"splitContainer3";
			// 
			// splitContainer3.Panel1
			// 
			this->splitContainer3->Panel1->Controls->Add(this->CameraInfo_textBox);
			// 
			// splitContainer3.Panel2
			// 
			this->splitContainer3->Panel2->Controls->Add(this->ColladaParsingData_textBox);
			this->splitContainer3->Size = System::Drawing::Size(771, 75);
			this->splitContainer3->SplitterDistance = 256;
			this->splitContainer3->SplitterWidth = 3;
			this->splitContainer3->TabIndex = 0;
			// 
			// CameraInfo_textBox
			// 
			this->CameraInfo_textBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->CameraInfo_textBox->Location = System::Drawing::Point(0, 0);
			this->CameraInfo_textBox->Margin = System::Windows::Forms::Padding(2);
			this->CameraInfo_textBox->Multiline = true;
			this->CameraInfo_textBox->Name = L"CameraInfo_textBox";
			this->CameraInfo_textBox->Size = System::Drawing::Size(256, 75);
			this->CameraInfo_textBox->TabIndex = 0;
			// 
			// ColladaParsingData_textBox
			// 
			this->ColladaParsingData_textBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ColladaParsingData_textBox->Location = System::Drawing::Point(0, 0);
			this->ColladaParsingData_textBox->Margin = System::Windows::Forms::Padding(2);
			this->ColladaParsingData_textBox->Multiline = true;
			this->ColladaParsingData_textBox->Name = L"ColladaParsingData_textBox";
			this->ColladaParsingData_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->ColladaParsingData_textBox->Size = System::Drawing::Size(512, 75);
			this->ColladaParsingData_textBox->TabIndex = 1;
			this->ColladaParsingData_textBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MeshViewer::ColladaParsingData_textBox_KeyUp);
			// 
			// MeshViewer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer2);
			this->Controls->Add(this->menuStrip1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"MeshViewer";
			this->Size = System::Drawing::Size(771, 514);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartJoint_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Speed_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SpecificJointAnimation_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AnimationProgress_trackBar))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->ResumeLayout(false);
			this->splitContainer3->Panel1->ResumeLayout(false);
			this->splitContainer3->Panel1->PerformLayout();
			this->splitContainer3->Panel2->ResumeLayout(false);
			this->splitContainer3->Panel2->PerformLayout();
			this->splitContainer3->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
				static bool	l_bsEnd = true;
				if( !l_bsEnd )
					return;
				l_bsEnd = false;
				int l_i = glGetError();
				if( l_i != 0 )
				{
					int a=0;
					//assert(0&&"opengl error");
				}
				cTexture::m_suiLastUsingImageIndex = -1;
				STATIC_MESH_SHADER = L"StaticMeshShader";
				SKINNING_MESH_SHADER = L"SkinningMeshShader";
				DEFAULT_SHADER = L"MeshShader";
				NO_TEXTURE_SHADER = L"MeshShader_NoTexture";
				FATMING_CORE::UseShaderProgram(DEFAULT_SHADER);

				if( g_bCameraResetClick )
				{
					g_bCameraResetClick = false;
					m_pFrameCamera->SetWorldTransform(cMatrix44::Identity);
					VECTOR4	l_vPos = {0,1,6.5,1};
					m_pFrameCamera->SetWorldPosition(l_vPos);
				}
				glViewport(0,0,splitContainer1->Panel2->Width,splitContainer1->Panel2->Height);
				g_sTimeAndFPS.Update();
				glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
				glClearColor( 0,0.5,0.5,1 );
				glClearDepth(1.0f);
//=================================================
//=================================================
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
				FATMING_CORE::UseShaderProgram(NO_TEXTURE_SHADER);
				m_pFrameCamera->Render();
				cMatrix44	l_mat = cMatrix44::XAxisRotationMatrix(D3DXToRadian(90));
				DrawPlane(cMatrix44::TranslationMatrix(Vector3(0,0,-5))*l_mat,10,10,1,1,1,Vector4(0.5,1,0,1));
				//DisableShaderProgram(L"MeshShader_NoTexture");
				glEnable(GL_ALPHA_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
				//glEnable(GL_CULL_FACE);
				//glCullFace(GL_NONE);
				glEnable(GL_TEXTURE_2D);
				if( m_pColladaParser )
				{
					for(int i=0;i<(int)m_pColladaParser->m_CurveList.size();++i)
					{
						m_pColladaParser->m_CurveList[i].RenderCurve();
					}
				}
				if( m_pAllMesh->Count() )
				{
					FATMING_CORE::UseShaderProgram(STATIC_MESH_SHADER);
					FATMING_CORE::SetupShaderColor(Vector4::One);
					m_pFrameCamera->Render();
					if( Mesh_checkBox->Checked )
					{
						for(int i=0;i<m_pAllMesh->Count();++i)
							(*m_pAllMesh)[i]->Render();
					}
					if( m_pColladaParser )
					if(m_pColladaParser->m_pAllAnimationMesh)
					for(int i=0;i<m_pColladaParser->m_pAllAnimationMesh->Count();++i)
					{
						FATMING_CORE::UseShaderProgram(SKINNING_MESH_SHADER);
						m_pFrameCamera->Render();
						float	l_fElpaseTime = g_sTimeAndFPS.fElpaseTime;
						if( Animation_checkBox->Checked )
						{
							int	l_iValue = (int)Speed_numericUpDown->Value;
							if(l_iValue>=0)
							{
								for( int i=0;i<l_iValue;++i )
								{
									l_fElpaseTime *= 2;
								}
							}
							else
							{
								l_iValue*=-1;
								for( int i=0;i<l_iValue;++i )
								{
									l_fElpaseTime /= 2;
								}							
							}
							if( !Update_WithSpecificJoint_checkBox->Checked )
							{
								(*m_pColladaParser->m_pAllAnimationMesh)[i]->Update(l_fElpaseTime);
							}
							else
							{
								(*m_pColladaParser->m_pAllAnimationMesh)[i]->JointUpdate(l_fElpaseTime);
							}
							AnimationProgress_trackBar->Value = (int)((*m_pColladaParser->m_pAllAnimationMesh)[i]->GetCurrentTime()*1000);
						}
						if( ShowAnimationMesh_checkBox->Checked )
						{
							for(int j=0;j<m_pColladaParser->m_pAllAnimationMesh->Count();++j)
							{
								(*m_pColladaParser->m_pAllAnimationMesh)[i]->Update(l_fElpaseTime);
								(*m_pColladaParser->m_pAllAnimationMesh)[i]->SetWorldPosition(Vector3(0.f,0.f,j*30.f));
								(*m_pColladaParser->m_pAllAnimationMesh)[i]->Render();
							}
						}
						if( RenderSkeleton_checkBox->Checked )
						{
							glDisable(GL_DEPTH_TEST);
							(*m_pColladaParser->m_pAllAnimationMesh)[i]->RenderSkeleton();
							int	l_iResolution[4] = {0,0,1920,1080};
							glEnable2D((float)l_iResolution[2],(float)l_iResolution[3]);
							(*m_pColladaParser->m_pAllAnimationMesh)[i]->RenderSkeletonName(m_pFrameCamera->GetProjection().GetMatrix(),m_pFrameCamera->GetWorldView(),l_iResolution);
							glEnable(GL_DEPTH_TEST);
						}
					}
				}
//=======================================================				
//=======================================================*/
				glDisable(GL_TEXTURE_2D);
				//draw coordinate hint
				FATMING_CORE::UseShaderProgram(NO_TEXTURE_SHADER);
				glEnable2D(1920,1080);
				DrawAxisArrow(m_pFrameCamera);
				glDisable2D();
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_BLEND);
				SwapBuffers (m_HdcMV);
				l_bsEnd = true;
				label3->Text = AnimationProgress_trackBar->Value.ToString();
			 }
	private: System::Void tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 timer1->Enabled = false;
			 if( m_pTabControl->SelectedTab == m_pTabPage )
			 {
				wglMakeCurrent( m_HdcMV,m_pHGLRCMV );
				timer1->Enabled = true;
			 }
		 }
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strFileName = DNCT::OpenFileAndGetName("Model Files(*.dae;*.xml;*.fat)|*.dae;*.xml;*.fat|All files (*.*)|*.*");
			 if( l_strFileName )
			 {
				 this->timer1->Enabled = false;
				 ShowAnimationMesh_checkBox->Checked = false;
				 Animation_checkBox->Checked = false;
				 SAFE_DELETE(m_pColladaParser);
				 m_pColladaParser = new cColladaParser();
				 m_pColladaParser->ParseDataIntoXMLNode(DNCT::GcStringToChar(l_strFileName).c_str(),true);
				 m_pAllMesh->Destroy();
				 m_pColladaParser->m_pAllMesh->DumpListPointer(m_pAllMesh);
				 m_pColladaParser->m_pAllMesh->Destroy();
				 m_pColladaParser->ClearData();
				 if( m_pAllMesh->Count() )
				 {
					 ColladaParsingData_textBox->Text = "";
					 if( m_pColladaParser->m_pAllAnimationMesh->Count() )
					 {
						 AnimationProgress_trackBar->Maximum = (int)(m_pColladaParser->m_pAllAnimationMesh->GetObject(0)->GetEndTime()*1000);
						 StartJoint_numericUpDown->Maximum = m_pColladaParser->m_pAllAnimationMesh->GetObject(0)->GetSkinningBoneVector()->Count()-1;
					 }
					 for(UINT i=0;i<m_pColladaParser->m_strParsingDataList.size();++i)
					 {
						//ColladaParsingData_textBox->Text += String(m_pColladaParser->m_strParsingDataList[i].c_str()).ToString();
						//ColladaParsingData_textBox->Text += DNCT::GetChanglineString();
					 }
					 if( m_pColladaParser->m_pAllAnimationMesh&&m_pColladaParser->m_pAllAnimationMesh->Count() )
						 SpecificJointAnimation_numericUpDown->Maximum = m_pColladaParser->m_pAllAnimationMesh->GetObject(0)->GetSkinningBoneVector()->Count()-1;
				 }
				 this->timer1->Enabled  = true;
			 }
		 }
private: System::Void splitContainer1_Panel2_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 splitContainer1->Panel2->Focus();
		 }
private: System::Void MyMouseEvent(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			GCFORM::MouseButtons l_MouseButton = e->Button;
			m_pCameraBehaveByMouseBehave->CameraUpDateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,g_sTimeAndFPS.fElpaseTime);
			CameraInfo_textBox->Text = "x:"+m_pFrameCamera->GetWorldPosition().x.ToString()+","
							 "y:"+m_pFrameCamera->GetWorldPosition().y.ToString()+","
							 "z:"+m_pFrameCamera->GetWorldPosition().z.ToString()+DNCT::GetChanglineString();
			CameraInfo_textBox->Text += "Right:"+m_pFrameCamera->GetWorldRight().x.ToString()+","
							 +m_pFrameCamera->GetWorldRight().y.ToString()+","
							 +m_pFrameCamera->GetWorldRight().z.ToString()+DNCT::GetChanglineString();
			CameraInfo_textBox->Text += "Up"+m_pFrameCamera->GetWorldUp().x.ToString()+","
							 ""+m_pFrameCamera->GetWorldUp().y.ToString()+","
							 ""+m_pFrameCamera->GetWorldUp().z.ToString()+DNCT::GetChanglineString();
			CameraInfo_textBox->Text += "Direction"+m_pFrameCamera->GetWorldDirection().x.ToString()+","
							 ""+m_pFrameCamera->GetWorldDirection().y.ToString()+","
							 ""+m_pFrameCamera->GetWorldDirection().z.ToString();
		 }
private: System::Void invertTextureAdressToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 g_bInvertTextureAdress_V = !g_bInvertTextureAdress_V;
			 invertTextureAdressToolStripMenuItem->Checked = g_bInvertTextureAdress_V;
		 }
private: System::Void splitContainer1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
		 }
private: System::Void ColladaParsingData_textBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if(e->KeyCode >= GCFORM::Keys::NumPad0&&e->KeyCode <= GCFORM::Keys::NumPad9)
			 {
				//if((int)(e->KeyCode-GCFORM::Keys::NumPad0)<m_pAllMesh->Count())
				//	m_pbMeshShow[(int)e->KeyCode-(int)GCFORM::Keys::NumPad0] = !m_pbMeshShow[(int)e->KeyCode-(int)GCFORM::Keys::NumPad0];
			 }
		 }
private: System::Void AnimationProgress_trackBar_Scroll(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( !Animation_checkBox->Checked )
			 {
				 if(m_pColladaParser->m_pAllAnimationMesh)
				 {
					 float l_fCurrentAnimaitonTime = AnimationProgress_trackBar->Value/1000.f;
					 (*m_pColladaParser->m_pAllAnimationMesh)[0]->SetCurrentAnimationTime(l_fCurrentAnimaitonTime);
				 }
			 }
		 }
private: System::Void AnimationProgress_trackBar_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 Animation_checkBox->Checked = false;
		 }
private: System::Void SpecificJointAnimation_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pColladaParser&&m_pColladaParser->m_pAllAnimationMesh&&m_pColladaParser->m_pAllAnimationMesh->Count())
			 {
				if( this->SpecificJointAnimation_checkBox->Checked )
					m_pColladaParser->m_pAllAnimationMesh->GetObject(0)->m_iOnlyInflunceJoint = (int)SpecificJointAnimation_numericUpDown->Value;
				 this->JointName_label->Text = DNCT::WcharToGcstring(m_pColladaParser->m_pAllAnimationMesh->GetObject(0)->GetSkinningBoneVector()->GetObject((int)SpecificJointAnimation_numericUpDown->Value)->GetName())
#ifdef _DEBUG
					 +DNCT::GetChanglineString()+DNCT::WcharToGcstring((WCHAR*)m_pColladaParser->m_pAllAnimationMesh->GetObject(0)->GetSkinningBoneVector()->GetObject((int)SpecificJointAnimation_numericUpDown->Value)->m_strSID.c_str())
#endif
					;

				 cBone*l_pBone = (*m_pColladaParser->m_pAllAnimationMesh->GetObject(0)->GetSkinningBoneVector())[m_pColladaParser->m_pAllAnimationMesh->GetObject(0)->m_iOnlyInflunceJoint];


				cMatrix44	l_matW = l_pBone->GetWorldTransform();
				cMatrix44	l_matInvBindPose = 	l_pBone->GetInvBindPose();
				//cMatrix44  l_WorldTransform =  l_matW*l_matInvBindPose*this->m_matMeshBindShapePose;
				cMatrix44  l_WorldTransform =  l_matW*l_matInvBindPose;
				this->JointName_label->Text += 	DNCT::GetChanglineString()+
				//String(MatrixToString(l_matInvBindPose)).ToString()+DNCT::GetChanglineString()+
				String(MatrixToStringWithIndex(l_matW).c_str()).ToString()+DNCT::GetChanglineString()+
				String(MatrixToStringWithIndex(l_WorldTransform).c_str()).ToString();
			 }
		 }
private: System::Void SpecificJointAnimation_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
				 if(m_pColladaParser->m_pAllAnimationMesh&&m_pColladaParser->m_pAllAnimationMesh->Count())
				 {
					 m_pColladaParser->m_pAllAnimationMesh->GetObject(0)->m_bOnlyInflunceJoint = SpecificJointAnimation_checkBox->Checked;
				 }
		 }
private: System::Void StartJoint_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pColladaParser->m_pAllAnimationMesh&&m_pColladaParser->m_pAllAnimationMesh->Count())
			 {
				 m_pColladaParser->m_pAllAnimationMesh->GetObject(0)->m_iStartJoint = (int)StartJoint_numericUpDown->Value;
			 }
			 
		 }
private: System::Void savePathFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(  m_pColladaParser&&this->m_pColladaParser->m_CurveList.size() )
			 {
				 String^l_strfileName = DNCT::SaveFileAndGetName("XML	Files(*.xml)|*.xml|All files (*.*)|*.*");
				 if( l_strfileName )
				 {
					cCurveManager	l_vCurveManager;
					for( int i=0;i<(int)m_pColladaParser->m_CurveList.size();++i )
					{
						cCurveWithTime*l_p = new cCurveWithTime;
						for( int j=0;j<(int)m_pColladaParser->m_CurveList[i].GetOriginalPointList().size();++j )
						{
							std::vector<Vector3>	l_PosVector = m_pColladaParser->m_CurveList[i].GetOriginalPointList();
							l_p->AddPoint(l_PosVector[j],0.f);
						}
						l_vCurveManager.AddObjectNeglectExist(l_p);
					}
					l_vCurveManager.ExportData(DNCT::GcStringToChar(l_strfileName).c_str());
				 }
			 }
		 }
private: System::Void Update_WithSpecificJoint_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( Update_WithSpecificJoint_checkBox->Checked )
			 {
				Update_WithSpecificJoint_checkBox->Text = "WithSpecificJoint";
			 }
			 else
			 {
				Update_WithSpecificJoint_checkBox->Text = "AllUpdate";
			 }
		 }
private: System::Void dToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 System::Windows::Forms::ToolStripMenuItem^l_pObject = (System::Windows::Forms::ToolStripMenuItem^)sender;
			 l_pObject->Checked = !l_pObject->Checked;
			 if(Camera2d_ToolStripMenuItem == sender)
			 {
				Camera3d_ToolStripMenuItem->Checked = !l_pObject->Checked;
			 }
			 else
			 {
				Camera2d_ToolStripMenuItem->Checked = !l_pObject->Checked;
			 }
			 Projection	l_ProjectionMV;
			 if( Camera2d_ToolStripMenuItem->Checked )
			 {
				 l_ProjectionMV.SetOrthographic(1920,1080,-10000,10000);
				 m_pFrameCamera->SetLocalPosition(Vector3(960,540,0));
			 }
			 else
			 {
				 l_ProjectionMV.SetFovYAspect( XM_PIDIV4 ,1920/1080.f, 1000.0f * 1e-4f,2000000.f);
			 }
			 m_pFrameCamera->SetProjection(l_ProjectionMV);
		 }
};
}
