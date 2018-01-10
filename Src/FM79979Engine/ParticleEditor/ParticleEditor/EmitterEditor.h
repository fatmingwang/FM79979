#pragma once

//#include "FCDocument\FCDSceneNodeIterator.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

extern cPrtEmitter*g_pPrtEmitter;
extern cPaticleManager*g_pPaticleManager;

namespace ParticalEditor 
{

	/// <summary>
	/// EmitterEditor 的摘要
	/// </summary>
	public ref class EmitterEditor : public System::Windows::Forms::UserControl
	{
	public:
		EmitterEditor(GCFORM::Panel^e_pPanel,GCFORM::Timer^e_pTimer,bool	*e_pbParticleUpdate)
		{
			m_bRfreshParticleEmiterData = false;
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			this->Dock = GCFORM::DockStyle::Fill;
			m_pTimer = e_pTimer;
			m_bShowLine = true;
			m_bShowEmitter = true;
			e_pPanel->Controls->Add(this);
			m_fMaximaCubeSize = 0.00001f;
			m_pBehaviorPaticleManager = new cBehaviorPaticleManager();
			//use external one,so ensure set it to NULL while Destroy
			SAFE_DELETE(m_pBehaviorPaticleManager->m_pPaticleManager);
			m_pParticleEmiterWithShowPosition = new cParticleEmiterWithShowPosition(L"My79979Test");
			m_pParticleEmitterGroup = new cParticleEmitterGroup();
			ReassignParticleEmiterWithShowPositionData(false);
			//collpaseForm1->DoCollpease();
			//collpaseForm2->DoCollpease();
			//collpaseForm3->DoCollpease();
			RotateType_comboBox->SelectedIndex = 0;
			m_pbParticleUpdate = e_pbParticleUpdate;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		virtual	~EmitterEditor()
		{
			if (components)
			{
				delete components;
			}				 
			//because it come from outside
			m_pParticleEmiterWithShowPosition->SetPrtEmitter(0);
			SAFE_DELETE(m_pParticleEmiterWithShowPosition);
			//because it come from external
			m_pBehaviorPaticleManager->m_pPaticleManager = 0;
			SAFE_DELETE(m_pBehaviorPaticleManager);
			SAFE_DELETE(m_pParticleEmitterGroup);
		}


	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  modelToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  OnlyOneModelToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addToolStripMenuItem;
	private: DotNetComponent::XYZNumeriaclControl^  ParticlePathPos_xyzNumeriaclControl;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ToolStripMenuItem^  pathToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ListBox^  Path_listBox;
	private: System::Windows::Forms::Label^  label3;
	private: DotNetComponent::NumericalControl^  StartTime_numericalControl;
	private: DotNetComponent::NumericalControl^  EndTime_numericalControl;
	private: DotNetComponent::NumericalControl^  TotalTimeReflash_numericalControl;
	private: System::Windows::Forms::ToolStripMenuItem^  showPathToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem;
	private: System::Windows::Forms::ListBox^  ParticleBehavior_listBox;
	private: System::Windows::Forms::TreeView^  SceneNodes_treeView;
	private: System::Windows::Forms::ComboBox^  PathType_comboBox;


	private: System::Windows::Forms::Button^  DeleteParticleEmitter_button;
	private: System::Windows::Forms::Button^  AddParticleEmitter_button;
	private: System::Windows::Forms::TextBox^  EmitterName_textBox;
	private: System::Windows::Forms::Label^  label1;
	private: DotNetComponent::CheckBoxControl^  ImmediateDired_checkBox;


	private: System::Windows::Forms::CheckBox^  Loop_checkBox;
	private: System::Windows::Forms::CheckBox^  AllPlay_checkBox;
	private: System::Windows::Forms::CheckBox^  AllCurveLoop_checkBox;
	private: System::Windows::Forms::CheckBox^  Play_checkBox;

	private: System::Windows::Forms::Button^  MergeMPDIData_button;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Button^  AnimationInvert_button;
	private: System::Windows::Forms::TextBox^  AnimationName_textBox;
	private: System::Windows::Forms::ListBox^  WholeAnimationList_listBox;
	private: System::Windows::Forms::Button^  DeleteListObject_button;
	private: System::Windows::Forms::Button^  AddAnimationToList_button;
	private: System::Windows::Forms::Label^  Progress_label;

	private: System::Windows::Forms::Label^  label4;






	private: System::Windows::Forms::Button^  ParticleBehaviorInvert_button;
	private: System::Windows::Forms::Label^  label6;
	private: DotNetComponent::XYZNumeriaclControl^  ParticlePathAngle_xyzNumeriaclControl;
	private: System::Windows::Forms::ComboBox^  RotateType_comboBox;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: DotNetComponent::XYZNumeriaclControl^  EmitDirection_xyzNumeriaclControl;
	private: System::Windows::Forms::TrackBar^  Progress_trackBar;
private: System::Windows::Forms::TabControl^  tabControl1;
private: System::Windows::Forms::TabPage^  tabPage1;
private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel2;
private: System::Windows::Forms::TabPage^  tabPage2;
private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel3;
private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::GroupBox^  groupBox2;
private: System::Windows::Forms::GroupBox^  groupBox3;
private: System::Windows::Forms::GroupBox^  groupBox4;
		 //my
	public:
			//the particle emit pointer,if we want to emit behavior emit set this as false
			 bool*	m_pbParticleUpdate;
			 //for show line and show emitter
			 void	RenderAssistance(float e_fElpaseTime);
			 bool	OpenFile();
			 void	SaveFile();
			 //get UI status and set it to current particle emitter behavior
			 void	AssignUIToParticleBehaviorData(bool e_bRefresh);
			 //
			 void	RefreshPathData();
			 public:cParticleEmiterWithShowPosition*m_pParticleEmiterWithShowPosition;
			 cParticleEmitterGroup*m_pParticleEmitterGroup;
			 GCFORM::Timer^m_pTimer;//from from1 to stop render if need
			 cBehaviorPaticleManager*m_pBehaviorPaticleManager;
			 float	m_fMaximaCubeSize;
			 //while particle data is changed,all emitter behavior should refresh the target emitter is as same as changed
			 void	RefreshParticle(cPrtEmitter*e_pPrtEmitter);
			 //because we wanna change particle emitter.
			 void	ReassignParticleEmiterWithShowPositionData(bool e_bAssignEmitterAngle);
			 //if behavior emitter is using the particle emitter return true to aviod the user dekete the par emitter still referenced.
			 bool	IsBehaviorEmitterUsingPrtByName(const WCHAR*e_strName);
			 //particle emitter data show in the form.h
			 bool	m_bRfreshParticleEmiterData;
			 //
			 bool	m_bShowLine;
			 bool	m_bShowEmitter;
	public: System::Void	MyHintRender(float e_fElpaseTime)
			{
				static float	l_fColor = 0.f;
				l_fColor+=e_fElpaseTime;
				if(  l_fColor>1.f )
					l_fColor = 0.f;
				if( SceneNodes_treeView->SelectedNode )
				{
					//if( m_pSelectedFCDSceneNode )
					{
						//cMatrix44	l_mat = *(cMatrix44*)&m_pSelectedFCDSceneNode->CalculateWorldTransform();
						//RenderCube(Vector3(m_fMaximaCubeSize,m_fMaximaCubeSize,m_fMaximaCubeSize),cMatrix44::TranslationMatrix(l_mat.r[3]),Vector4(1,l_fColor,l_fColor,1));
					}
				}
				RenderAssistance(e_fElpaseTime);
			}
	public: System::Void	MyRender(float e_fElpaseTime)
			{
				if( Play_checkBox->Checked )
				{
					m_pParticleEmiterWithShowPosition->Update(e_fElpaseTime);
					m_pParticleEmiterWithShowPosition->Render();
					if( m_pParticleEmiterWithShowPosition->IsAnimationDone() )
						Play_checkBox->Checked = false;
					else
					{
						float	l_fTime = m_pParticleEmiterWithShowPosition->GetPastTime();
						if(  l_fTime >= m_pParticleEmiterWithShowPosition->GetEndTime() )
						{
							l_fTime -= m_pParticleEmiterWithShowPosition->GetEndTime();
						}
						//Progress_trackBar->Value = (int)l_fTime*1000;
					}
				}
				else
				if( AllPlay_checkBox->Checked )
				{
					m_pParticleEmitterGroup->Update(e_fElpaseTime);
					m_pParticleEmitterGroup->Render();
				}
			}
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
			this->modelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->OnlyOneModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showPathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->EmitDirection_xyzNumeriaclControl = (gcnew DotNetComponent::XYZNumeriaclControl());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->SceneNodes_treeView = (gcnew System::Windows::Forms::TreeView());
			this->ParticlePathPos_xyzNumeriaclControl = (gcnew DotNetComponent::XYZNumeriaclControl());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->Path_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->ParticlePathAngle_xyzNumeriaclControl = (gcnew DotNetComponent::XYZNumeriaclControl());
			this->RotateType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->PathType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->Progress_label = (gcnew System::Windows::Forms::Label());
			this->Loop_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ParticleBehaviorInvert_button = (gcnew System::Windows::Forms::Button());
			this->AllPlay_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->AllCurveLoop_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Play_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ImmediateDired_checkBox = (gcnew DotNetComponent::CheckBoxControl());
			this->EndTime_numericalControl = (gcnew DotNetComponent::NumericalControl());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->ParticleBehavior_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->DeleteParticleEmitter_button = (gcnew System::Windows::Forms::Button());
			this->TotalTimeReflash_numericalControl = (gcnew DotNetComponent::NumericalControl());
			this->StartTime_numericalControl = (gcnew DotNetComponent::NumericalControl());
			this->AddParticleEmitter_button = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->EmitterName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Progress_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->AnimationInvert_button = (gcnew System::Windows::Forms::Button());
			this->WholeAnimationList_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AddAnimationToList_button = (gcnew System::Windows::Forms::Button());
			this->MergeMPDIData_button = (gcnew System::Windows::Forms::Button());
			this->DeleteListObject_button = (gcnew System::Windows::Forms::Button());
			this->AnimationName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->flowLayoutPanel3 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->flowLayoutPanel2 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Progress_trackBar))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->flowLayoutPanel3->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->flowLayoutPanel2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->modelToolStripMenuItem, 
				this->pathToolStripMenuItem, this->viewToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(347, 24);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// modelToolStripMenuItem
			// 
			this->modelToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->OnlyOneModelToolStripMenuItem, 
				this->addToolStripMenuItem});
			this->modelToolStripMenuItem->Name = L"modelToolStripMenuItem";
			this->modelToolStripMenuItem->Size = System::Drawing::Size(56, 20);
			this->modelToolStripMenuItem->Text = L"Model";
			// 
			// OnlyOneModelToolStripMenuItem
			// 
			this->OnlyOneModelToolStripMenuItem->Name = L"OnlyOneModelToolStripMenuItem";
			this->OnlyOneModelToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->OnlyOneModelToolStripMenuItem->Text = L"OnlyOneModel";
			this->OnlyOneModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &EmitterEditor::AddModel_button_Click);
			// 
			// addToolStripMenuItem
			// 
			this->addToolStripMenuItem->Name = L"addToolStripMenuItem";
			this->addToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->addToolStripMenuItem->Text = L"Add";
			this->addToolStripMenuItem->Click += gcnew System::EventHandler(this, &EmitterEditor::AddModel_button_Click);
			// 
			// pathToolStripMenuItem
			// 
			this->pathToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
				this->showPathToolStripMenuItem});
			this->pathToolStripMenuItem->Name = L"pathToolStripMenuItem";
			this->pathToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->pathToolStripMenuItem->Text = L"Path";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(131, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &EmitterEditor::openToolStripMenuItem_Click);
			// 
			// showPathToolStripMenuItem
			// 
			this->showPathToolStripMenuItem->Name = L"showPathToolStripMenuItem";
			this->showPathToolStripMenuItem->Size = System::Drawing::Size(131, 22);
			this->showPathToolStripMenuItem->Text = L"ShowPath";
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(49, 20);
			this->viewToolStripMenuItem->Text = L"View";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->ForeColor = System::Drawing::Color::Chartreuse;
			this->label8->Location = System::Drawing::Point(96, 21);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(152, 15);
			this->label8->TabIndex = 159;
			this->label8->Text = L"EmitDirection/路徑角度";
			// 
			// EmitDirection_xyzNumeriaclControl
			// 
			this->EmitDirection_xyzNumeriaclControl->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->EmitDirection_xyzNumeriaclControl->Location = System::Drawing::Point(153, 40);
			this->EmitDirection_xyzNumeriaclControl->Margin = System::Windows::Forms::Padding(0);
			this->EmitDirection_xyzNumeriaclControl->Name = L"EmitDirection_xyzNumeriaclControl";
			this->EmitDirection_xyzNumeriaclControl->Size = System::Drawing::Size(85, 114);
			this->EmitDirection_xyzNumeriaclControl->TabIndex = 158;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::Chartreuse;
			this->label4->Location = System::Drawing::Point(6, 21);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(87, 15);
			this->label4->TabIndex = 155;
			this->label4->Text = L"Position/位置";
			// 
			// SceneNodes_treeView
			// 
			this->SceneNodes_treeView->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->SceneNodes_treeView->ForeColor = System::Drawing::SystemColors::Info;
			this->SceneNodes_treeView->Location = System::Drawing::Point(6, 161);
			this->SceneNodes_treeView->Name = L"SceneNodes_treeView";
			this->SceneNodes_treeView->Size = System::Drawing::Size(252, 327);
			this->SceneNodes_treeView->TabIndex = 154;
			this->SceneNodes_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &EmitterEditor::SceneNodes_treeView_AfterSelect);
			// 
			// ParticlePathPos_xyzNumeriaclControl
			// 
			this->ParticlePathPos_xyzNumeriaclControl->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->ParticlePathPos_xyzNumeriaclControl->Location = System::Drawing::Point(12, 40);
			this->ParticlePathPos_xyzNumeriaclControl->Margin = System::Windows::Forms::Padding(4);
			this->ParticlePathPos_xyzNumeriaclControl->Name = L"ParticlePathPos_xyzNumeriaclControl";
			this->ParticlePathPos_xyzNumeriaclControl->Size = System::Drawing::Size(83, 114);
			this->ParticlePathPos_xyzNumeriaclControl->TabIndex = 3;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->ForeColor = System::Drawing::Color::Chartreuse;
			this->label7->Location = System::Drawing::Point(18, 17);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(148, 15);
			this->label7->TabIndex = 159;
			this->label7->Text = L"RotationType/旋轉型態";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->ForeColor = System::Drawing::Color::Chartreuse;
			this->label6->Location = System::Drawing::Point(150, 16);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(130, 15);
			this->label6->TabIndex = 157;
			this->label6->Text = L"PathAngle/路徑角度";
			// 
			// Path_listBox
			// 
			this->Path_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Path_listBox->Enabled = false;
			this->Path_listBox->ForeColor = System::Drawing::SystemColors::Info;
			this->Path_listBox->FormattingEnabled = true;
			this->Path_listBox->ItemHeight = 15;
			this->Path_listBox->Location = System::Drawing::Point(6, 140);
			this->Path_listBox->MaximumSize = System::Drawing::Size(256, 1080);
			this->Path_listBox->Name = L"Path_listBox";
			this->Path_listBox->Size = System::Drawing::Size(256, 304);
			this->Path_listBox->TabIndex = 145;
			this->Path_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EmitterEditor::Path_listBox_SelectedIndexChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::Color::Chartreuse;
			this->label3->Location = System::Drawing::Point(19, 67);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(125, 15);
			this->label3->TabIndex = 147;
			this->label3->Text = L"PathType/路徑型態";
			// 
			// ParticlePathAngle_xyzNumeriaclControl
			// 
			this->ParticlePathAngle_xyzNumeriaclControl->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->ParticlePathAngle_xyzNumeriaclControl->Location = System::Drawing::Point(137, 30);
			this->ParticlePathAngle_xyzNumeriaclControl->Margin = System::Windows::Forms::Padding(0);
			this->ParticlePathAngle_xyzNumeriaclControl->Name = L"ParticlePathAngle_xyzNumeriaclControl";
			this->ParticlePathAngle_xyzNumeriaclControl->Size = System::Drawing::Size(85, 111);
			this->ParticlePathAngle_xyzNumeriaclControl->TabIndex = 156;
			// 
			// RotateType_comboBox
			// 
			this->RotateType_comboBox->FormattingEnabled = true;
			this->RotateType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"None", L"Self", L"World"});
			this->RotateType_comboBox->Location = System::Drawing::Point(17, 37);
			this->RotateType_comboBox->Margin = System::Windows::Forms::Padding(2);
			this->RotateType_comboBox->Name = L"RotateType_comboBox";
			this->RotateType_comboBox->Size = System::Drawing::Size(92, 23);
			this->RotateType_comboBox->TabIndex = 158;
			this->RotateType_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EmitterEditor::RotateType_comboBox_SelectedIndexChanged);
			// 
			// PathType_comboBox
			// 
			this->PathType_comboBox->FormattingEnabled = true;
			this->PathType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"No Path", L"Follow Path", L"All PathPoint"});
			this->PathType_comboBox->Location = System::Drawing::Point(18, 87);
			this->PathType_comboBox->Name = L"PathType_comboBox";
			this->PathType_comboBox->Size = System::Drawing::Size(91, 23);
			this->PathType_comboBox->TabIndex = 146;
			this->PathType_comboBox->Text = L"No Path";
			this->PathType_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &EmitterEditor::PathType_comboBox_SelectedIndexChanged);
			// 
			// Progress_label
			// 
			this->Progress_label->AutoSize = true;
			this->Progress_label->ForeColor = System::Drawing::Color::Chartreuse;
			this->Progress_label->Location = System::Drawing::Point(9, 23);
			this->Progress_label->Name = L"Progress_label";
			this->Progress_label->Size = System::Drawing::Size(55, 15);
			this->Progress_label->TabIndex = 166;
			this->Progress_label->Text = L"Progress";
			// 
			// Loop_checkBox
			// 
			this->Loop_checkBox->AutoSize = true;
			this->Loop_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->Loop_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Loop_checkBox->Location = System::Drawing::Point(11, 205);
			this->Loop_checkBox->Name = L"Loop_checkBox";
			this->Loop_checkBox->Size = System::Drawing::Size(208, 18);
			this->Loop_checkBox->TabIndex = 161;
			this->Loop_checkBox->Text = L"SingleCurveLoop/單一路徑loop";
			this->Loop_checkBox->UseVisualStyleBackColor = true;
			this->Loop_checkBox->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &EmitterEditor::Loop_checkBox_MouseClick);
			// 
			// ParticleBehaviorInvert_button
			// 
			this->ParticleBehaviorInvert_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->ParticleBehaviorInvert_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ParticleBehaviorInvert_button->Location = System::Drawing::Point(217, 337);
			this->ParticleBehaviorInvert_button->Name = L"ParticleBehaviorInvert_button";
			this->ParticleBehaviorInvert_button->Size = System::Drawing::Size(61, 23);
			this->ParticleBehaviorInvert_button->TabIndex = 167;
			this->ParticleBehaviorInvert_button->Text = L"Reedit/重新編輯";
			this->ParticleBehaviorInvert_button->UseVisualStyleBackColor = false;
			this->ParticleBehaviorInvert_button->Click += gcnew System::EventHandler(this, &EmitterEditor::AnimationInvert_button_Click);
			// 
			// AllPlay_checkBox
			// 
			this->AllPlay_checkBox->AutoSize = true;
			this->AllPlay_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->AllPlay_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllPlay_checkBox->Location = System::Drawing::Point(10, 225);
			this->AllPlay_checkBox->Name = L"AllPlay_checkBox";
			this->AllPlay_checkBox->Size = System::Drawing::Size(127, 18);
			this->AllPlay_checkBox->TabIndex = 162;
			this->AllPlay_checkBox->Text = L"AllPlay/全部撥放";
			this->AllPlay_checkBox->UseVisualStyleBackColor = true;
			this->AllPlay_checkBox->CheckedChanged += gcnew System::EventHandler(this, &EmitterEditor::EmitterStop_button_Click);
			// 
			// AllCurveLoop_checkBox
			// 
			this->AllCurveLoop_checkBox->AutoSize = true;
			this->AllCurveLoop_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->AllCurveLoop_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllCurveLoop_checkBox->Location = System::Drawing::Point(10, 243);
			this->AllCurveLoop_checkBox->Name = L"AllCurveLoop_checkBox";
			this->AllCurveLoop_checkBox->Size = System::Drawing::Size(190, 18);
			this->AllCurveLoop_checkBox->TabIndex = 164;
			this->AllCurveLoop_checkBox->Text = L"AllCurveLoop/全部路徑loop";
			this->AllCurveLoop_checkBox->UseVisualStyleBackColor = true;
			this->AllCurveLoop_checkBox->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &EmitterEditor::Loop_checkBox_MouseClick);
			// 
			// Play_checkBox
			// 
			this->Play_checkBox->AutoSize = true;
			this->Play_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->Play_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Play_checkBox->Location = System::Drawing::Point(11, 184);
			this->Play_checkBox->Name = L"Play_checkBox";
			this->Play_checkBox->Size = System::Drawing::Size(145, 18);
			this->Play_checkBox->TabIndex = 163;
			this->Play_checkBox->Text = L"SinglePlay/單一撥放";
			this->Play_checkBox->UseVisualStyleBackColor = true;
			this->Play_checkBox->CheckedChanged += gcnew System::EventHandler(this, &EmitterEditor::EmitterStop_button_Click);
			// 
			// ImmediateDired_checkBox
			// 
			this->ImmediateDired_checkBox->CheckboxName = L"ImmediateDired/立即死亡";
			this->ImmediateDired_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImmediateDired_checkBox->Location = System::Drawing::Point(9, 261);
			this->ImmediateDired_checkBox->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->ImmediateDired_checkBox->Name = L"ImmediateDired_checkBox";
			this->ImmediateDired_checkBox->Size = System::Drawing::Size(164, 24);
			this->ImmediateDired_checkBox->TabIndex = 157;
			this->ImmediateDired_checkBox->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &EmitterEditor::Loop_checkBox_MouseClick);
			// 
			// EndTime_numericalControl
			// 
			this->EndTime_numericalControl->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->EndTime_numericalControl->Location = System::Drawing::Point(11, 117);
			this->EndTime_numericalControl->Margin = System::Windows::Forms::Padding(0);
			this->EndTime_numericalControl->Name = L"EndTime_numericalControl";
			this->EndTime_numericalControl->Size = System::Drawing::Size(162, 24);
			this->EndTime_numericalControl->TabIndex = 150;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->ForeColor = System::Drawing::Color::Chartreuse;
			this->label14->Location = System::Drawing::Point(9, 61);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(153, 15);
			this->label14->TabIndex = 142;
			this->label14->Text = L"PathStartTime/開始時間";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::Chartreuse;
			this->label2->Location = System::Drawing::Point(9, 100);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(124, 15);
			this->label2->TabIndex = 138;
			this->label2->Text = L"EndTime/結束時間";
			// 
			// ParticleBehavior_listBox
			// 
			this->ParticleBehavior_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ParticleBehavior_listBox->ForeColor = System::Drawing::SystemColors::Info;
			this->ParticleBehavior_listBox->FormattingEnabled = true;
			this->ParticleBehavior_listBox->ItemHeight = 15;
			this->ParticleBehavior_listBox->Location = System::Drawing::Point(6, 375);
			this->ParticleBehavior_listBox->MaximumSize = System::Drawing::Size(256, 1080);
			this->ParticleBehavior_listBox->Name = L"ParticleBehavior_listBox";
			this->ParticleBehavior_listBox->ScrollAlwaysVisible = true;
			this->ParticleBehavior_listBox->Size = System::Drawing::Size(252, 154);
			this->ParticleBehavior_listBox->TabIndex = 153;
			this->ParticleBehavior_listBox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &EmitterEditor::WholeAnimationList_listBox_MouseDoubleClick);
			this->ParticleBehavior_listBox->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &EmitterEditor::WholeAnimationList_listBox_MouseClick);
			this->ParticleBehavior_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &EmitterEditor::ParticleBehavior_listBox_KeyUp);
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->ForeColor = System::Drawing::Color::Chartreuse;
			this->label20->Location = System::Drawing::Point(9, 141);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(206, 15);
			this->label20->TabIndex = 144;
			this->label20->Text = L"ReflashAllTime/設定整體新時間";
			// 
			// DeleteParticleEmitter_button
			// 
			this->DeleteParticleEmitter_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->DeleteParticleEmitter_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DeleteParticleEmitter_button->Location = System::Drawing::Point(123, 337);
			this->DeleteParticleEmitter_button->Name = L"DeleteParticleEmitter_button";
			this->DeleteParticleEmitter_button->Size = System::Drawing::Size(88, 23);
			this->DeleteParticleEmitter_button->TabIndex = 156;
			this->DeleteParticleEmitter_button->Text = L"DeleteParticleEmitter/刪除粒子發射器";
			this->DeleteParticleEmitter_button->UseVisualStyleBackColor = false;
			this->DeleteParticleEmitter_button->Click += gcnew System::EventHandler(this, &EmitterEditor::AddParticleEmitter_button_Click);
			// 
			// TotalTimeReflash_numericalControl
			// 
			this->TotalTimeReflash_numericalControl->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->TotalTimeReflash_numericalControl->Location = System::Drawing::Point(11, 158);
			this->TotalTimeReflash_numericalControl->Margin = System::Windows::Forms::Padding(0);
			this->TotalTimeReflash_numericalControl->Name = L"TotalTimeReflash_numericalControl";
			this->TotalTimeReflash_numericalControl->Size = System::Drawing::Size(162, 23);
			this->TotalTimeReflash_numericalControl->TabIndex = 151;
			// 
			// StartTime_numericalControl
			// 
			this->StartTime_numericalControl->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->StartTime_numericalControl->Location = System::Drawing::Point(11, 73);
			this->StartTime_numericalControl->Margin = System::Windows::Forms::Padding(0);
			this->StartTime_numericalControl->Name = L"StartTime_numericalControl";
			this->StartTime_numericalControl->Size = System::Drawing::Size(162, 27);
			this->StartTime_numericalControl->TabIndex = 149;
			// 
			// AddParticleEmitter_button
			// 
			this->AddParticleEmitter_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AddParticleEmitter_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddParticleEmitter_button->Location = System::Drawing::Point(11, 337);
			this->AddParticleEmitter_button->Name = L"AddParticleEmitter_button";
			this->AddParticleEmitter_button->Size = System::Drawing::Size(107, 23);
			this->AddParticleEmitter_button->TabIndex = 155;
			this->AddParticleEmitter_button->Text = L"AddParticleEmitter/增加粒子發射器粒子";
			this->AddParticleEmitter_button->UseVisualStyleBackColor = false;
			this->AddParticleEmitter_button->Click += gcnew System::EventHandler(this, &EmitterEditor::AddParticleEmitter_button_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::Chartreuse;
			this->label1->Location = System::Drawing::Point(6, 287);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(162, 15);
			this->label1->TabIndex = 158;
			this->label1->Text = L"EmitterName/發射器名字";
			// 
			// EmitterName_textBox
			// 
			this->EmitterName_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->EmitterName_textBox->Location = System::Drawing::Point(10, 305);
			this->EmitterName_textBox->Name = L"EmitterName_textBox";
			this->EmitterName_textBox->Size = System::Drawing::Size(166, 25);
			this->EmitterName_textBox->TabIndex = 157;
			// 
			// Progress_trackBar
			// 
			this->Progress_trackBar->Location = System::Drawing::Point(67, 23);
			this->Progress_trackBar->Name = L"Progress_trackBar";
			this->Progress_trackBar->Size = System::Drawing::Size(226, 53);
			this->Progress_trackBar->TabIndex = 165;
			this->Progress_trackBar->ValueChanged += gcnew System::EventHandler(this, &EmitterEditor::Progress_trackBar_ValueChanged);
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->ForeColor = System::Drawing::Color::Chartreuse;
			this->label15->Location = System::Drawing::Point(9, 22);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(153, 15);
			this->label15->TabIndex = 123;
			this->label15->Text = L"Animation Name/動畫名";
			// 
			// AnimationInvert_button
			// 
			this->AnimationInvert_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AnimationInvert_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationInvert_button->Location = System::Drawing::Point(154, 62);
			this->AnimationInvert_button->Name = L"AnimationInvert_button";
			this->AnimationInvert_button->Size = System::Drawing::Size(52, 32);
			this->AnimationInvert_button->TabIndex = 124;
			this->AnimationInvert_button->Text = L"Reedit/重新編輯";
			this->AnimationInvert_button->UseVisualStyleBackColor = false;
			this->AnimationInvert_button->Click += gcnew System::EventHandler(this, &EmitterEditor::AnimationInvert_button_Click);
			// 
			// WholeAnimationList_listBox
			// 
			this->WholeAnimationList_listBox->AllowDrop = true;
			this->WholeAnimationList_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->WholeAnimationList_listBox->ForeColor = System::Drawing::SystemColors::Info;
			this->WholeAnimationList_listBox->FormattingEnabled = true;
			this->WholeAnimationList_listBox->ItemHeight = 15;
			this->WholeAnimationList_listBox->Location = System::Drawing::Point(6, 102);
			this->WholeAnimationList_listBox->MinimumSize = System::Drawing::Size(4, 69);
			this->WholeAnimationList_listBox->Name = L"WholeAnimationList_listBox";
			this->WholeAnimationList_listBox->ScrollAlwaysVisible = true;
			this->WholeAnimationList_listBox->Size = System::Drawing::Size(252, 439);
			this->WholeAnimationList_listBox->TabIndex = 121;
			this->WholeAnimationList_listBox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &EmitterEditor::WholeAnimationList_listBox_MouseDoubleClick);
			this->WholeAnimationList_listBox->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &EmitterEditor::WholeAnimationList_listBox_MouseClick);
			this->WholeAnimationList_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &EmitterEditor::ParticleBehavior_listBox_KeyUp);
			// 
			// AddAnimationToList_button
			// 
			this->AddAnimationToList_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AddAnimationToList_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddAnimationToList_button->Location = System::Drawing::Point(6, 64);
			this->AddAnimationToList_button->Name = L"AddAnimationToList_button";
			this->AddAnimationToList_button->Size = System::Drawing::Size(56, 30);
			this->AddAnimationToList_button->TabIndex = 119;
			this->AddAnimationToList_button->Text = L"Add/增加";
			this->AddAnimationToList_button->UseVisualStyleBackColor = false;
			this->AddAnimationToList_button->Click += gcnew System::EventHandler(this, &EmitterEditor::AddAnimationToList_button_Click);
			// 
			// MergeMPDIData_button
			// 
			this->MergeMPDIData_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->MergeMPDIData_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->MergeMPDIData_button->Location = System::Drawing::Point(211, 62);
			this->MergeMPDIData_button->Name = L"MergeMPDIData_button";
			this->MergeMPDIData_button->Size = System::Drawing::Size(52, 32);
			this->MergeMPDIData_button->TabIndex = 125;
			this->MergeMPDIData_button->Text = L"Merge/混合";
			this->MergeMPDIData_button->UseVisualStyleBackColor = false;
			// 
			// DeleteListObject_button
			// 
			this->DeleteListObject_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->DeleteListObject_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DeleteListObject_button->Location = System::Drawing::Point(67, 63);
			this->DeleteListObject_button->Name = L"DeleteListObject_button";
			this->DeleteListObject_button->Size = System::Drawing::Size(82, 31);
			this->DeleteListObject_button->TabIndex = 120;
			this->DeleteListObject_button->Text = L"Delete/刪除";
			this->DeleteListObject_button->UseVisualStyleBackColor = false;
			// 
			// AnimationName_textBox
			// 
			this->AnimationName_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AnimationName_textBox->Location = System::Drawing::Point(12, 36);
			this->AnimationName_textBox->Name = L"AnimationName_textBox";
			this->AnimationName_textBox->Size = System::Drawing::Size(142, 25);
			this->AnimationName_textBox->TabIndex = 122;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 24);
			this->tabControl1->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(347, 1254);
			this->tabControl1->TabIndex = 159;
			// 
			// tabPage2
			// 
			this->tabPage2->AutoScroll = true;
			this->tabPage2->Controls->Add(this->flowLayoutPanel3);
			this->tabPage2->Location = System::Drawing::Point(4, 25);
			this->tabPage2->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->tabPage2->Size = System::Drawing::Size(339, 1225);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"EmitterGroupData";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// flowLayoutPanel3
			// 
			this->flowLayoutPanel3->AutoScroll = true;
			this->flowLayoutPanel3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->flowLayoutPanel3->Controls->Add(this->groupBox1);
			this->flowLayoutPanel3->Controls->Add(this->groupBox2);
			this->flowLayoutPanel3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel3->Location = System::Drawing::Point(4, 4);
			this->flowLayoutPanel3->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->flowLayoutPanel3->Name = L"flowLayoutPanel3";
			this->flowLayoutPanel3->Size = System::Drawing::Size(331, 1217);
			this->flowLayoutPanel3->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->AutoScroll = true;
			this->tabPage1->Controls->Add(this->flowLayoutPanel2);
			this->tabPage1->Location = System::Drawing::Point(4, 25);
			this->tabPage1->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->tabPage1->Size = System::Drawing::Size(339, 1225);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"EmitterData";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// flowLayoutPanel2
			// 
			this->flowLayoutPanel2->AutoScroll = true;
			this->flowLayoutPanel2->Controls->Add(this->groupBox3);
			this->flowLayoutPanel2->Controls->Add(this->groupBox4);
			this->flowLayoutPanel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel2->Location = System::Drawing::Point(4, 4);
			this->flowLayoutPanel2->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->flowLayoutPanel2->Name = L"flowLayoutPanel2";
			this->flowLayoutPanel2->Size = System::Drawing::Size(331, 1217);
			this->flowLayoutPanel2->TabIndex = 0;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->Progress_trackBar);
			this->groupBox1->Controls->Add(this->EmitterName_textBox);
			this->groupBox1->Controls->Add(this->ParticleBehavior_listBox);
			this->groupBox1->Controls->Add(this->Progress_label);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->Loop_checkBox);
			this->groupBox1->Controls->Add(this->AddParticleEmitter_button);
			this->groupBox1->Controls->Add(this->AllPlay_checkBox);
			this->groupBox1->Controls->Add(this->StartTime_numericalControl);
			this->groupBox1->Controls->Add(this->AllCurveLoop_checkBox);
			this->groupBox1->Controls->Add(this->TotalTimeReflash_numericalControl);
			this->groupBox1->Controls->Add(this->ParticleBehaviorInvert_button);
			this->groupBox1->Controls->Add(this->DeleteParticleEmitter_button);
			this->groupBox1->Controls->Add(this->Play_checkBox);
			this->groupBox1->Controls->Add(this->label20);
			this->groupBox1->Controls->Add(this->ImmediateDired_checkBox);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->EndTime_numericalControl);
			this->groupBox1->Controls->Add(this->label14);
			this->groupBox1->Location = System::Drawing::Point(3, 3);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(302, 533);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"groupBox1";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->WholeAnimationList_listBox);
			this->groupBox2->Controls->Add(this->MergeMPDIData_button);
			this->groupBox2->Controls->Add(this->AddAnimationToList_button);
			this->groupBox2->Controls->Add(this->AnimationInvert_button);
			this->groupBox2->Controls->Add(this->DeleteListObject_button);
			this->groupBox2->Controls->Add(this->label15);
			this->groupBox2->Controls->Add(this->AnimationName_textBox);
			this->groupBox2->Location = System::Drawing::Point(3, 542);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(274, 544);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"groupBox2";
			// 
			// groupBox3
			// 
			this->groupBox3->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->groupBox3->Controls->Add(this->label4);
			this->groupBox3->Controls->Add(this->label8);
			this->groupBox3->Controls->Add(this->ParticlePathPos_xyzNumeriaclControl);
			this->groupBox3->Controls->Add(this->EmitDirection_xyzNumeriaclControl);
			this->groupBox3->Controls->Add(this->SceneNodes_treeView);
			this->groupBox3->Location = System::Drawing::Point(3, 3);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(272, 494);
			this->groupBox3->TabIndex = 158;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"EmitterStartPosition/發射器初始位置";
			// 
			// groupBox4
			// 
			this->groupBox4->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->groupBox4->Controls->Add(this->Path_listBox);
			this->groupBox4->Controls->Add(this->ParticlePathAngle_xyzNumeriaclControl);
			this->groupBox4->Controls->Add(this->label7);
			this->groupBox4->Controls->Add(this->label3);
			this->groupBox4->Controls->Add(this->PathType_comboBox);
			this->groupBox4->Controls->Add(this->RotateType_comboBox);
			this->groupBox4->Controls->Add(this->label6);
			this->groupBox4->Location = System::Drawing::Point(3, 503);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(287, 450);
			this->groupBox4->TabIndex = 159;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"PathData/路徑資料";
			// 
			// EmitterEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->menuStrip1);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"EmitterEditor";
			this->Size = System::Drawing::Size(347, 1278);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Progress_trackBar))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->flowLayoutPanel3->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->flowLayoutPanel2->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void AddModel_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				m_pTimer->Enabled = false;
				if( 0 )
				{					
				}
				m_pTimer->Enabled = true;
			 }
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_str = DNCT::OpenFileAndGetName("XML Files(*.xml;*.fat;*.dae)|*.xml;*.fat;*.dae|All files (*.*)|*.*");
			 if( l_str )
			 {
				 Path_listBox->Items->Clear();
				 m_pBehaviorPaticleManager->m_CurveManager.Destroy();
				 if(System::IO::Path::GetExtension(l_str)->ToLower()->Equals(".dae"))
				 {
					 WARNING_MSG("call fatming to fix this!");
//					 cCurveManager*l_pCurveManager = DaeDocumentToCurveManager(DNCT::GcStringToWchar(l_str));
//					 if( l_pCurveManager )
//						 l_pCurveManager->DumpListPointer(&m_pBehaviorPaticleManager->m_CurveManager);
//					 SAFE_DELETE(l_pCurveManager);
				 }
				 else
				 {
					m_pBehaviorPaticleManager->m_CurveManager.Parse(DNCT::GcStringToChar(l_str));
				 }
				 bool	l_bPathTimeIsZero = false;
				 for(int i=0;i<m_pBehaviorPaticleManager->m_CurveManager.Count();++i)
				 {
					 if( m_pBehaviorPaticleManager->m_CurveManager[i]->GetEndTime() <= 0.f )
					 {
						 if( !l_bPathTimeIsZero )
						 {
							 WARNING_MSG(DNCT::WcharToGcstring(m_pBehaviorPaticleManager->m_CurveManager[i]->GetName())+String("path time is zero!!assign time to 1 second").ToString());
						 }
						l_bPathTimeIsZero = true;
						m_pBehaviorPaticleManager->m_CurveManager[i]->AverageTimeAssign(1.f);
					 }
				 }
				 for(int i=0;i<m_pBehaviorPaticleManager->m_CurveManager.Count();++i)
				 {
					 Path_listBox->Items->Add(String(UT::WcharToChar(m_pBehaviorPaticleManager->m_CurveManager[i]->GetName()).c_str()).ToString());
				 }
			 }
		 }
private: System::Void SceneNodes_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		 {
			 //if( m_pFeelingScene&&SceneNodes_treeView->SelectedNode )
			 //{
				//FCDSceneNode*l_pSceneNode = m_pFeelingScene->GetObject(0)->GetVisualSceneNode();
				//String^l_strSelectedName = SceneNodes_treeView->SelectedNode->Text;
				//m_pSelectedFCDSceneNode = (FCDSceneNode*)l_pSceneNode->FindDaeId(DNCT::GcStringToChar(l_strSelectedName));
				//if( m_pSelectedFCDSceneNode )
				//if( m_pSelectedFCDSceneNode->GetTransformCount() )
				//{
				//	cMatrix44	l_mat = *(cMatrix44*)&m_pSelectedFCDSceneNode->CalculateWorldTransform();
				//	ParticlePathPos_xyzNumeriaclControl->X_numericUpDown->Value = System::Decimal(l_mat.GetTranslation().x);
				//	ParticlePathPos_xyzNumeriaclControl->Y_numericUpDown->Value = System::Decimal(l_mat.GetTranslation().y);
				//	ParticlePathPos_xyzNumeriaclControl->Z_numericUpDown->Value = System::Decimal(l_mat.GetTranslation().z);
				//	return;
				//}
			 //}
			 //m_pSelectedFCDSceneNode = 0;
		 }
private: System::Void AddParticleEmitter_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == DeleteParticleEmitter_button )
			 {
				int	l_iSelectedIndex = ParticleBehavior_listBox->SelectedIndex;
				if( ParticleBehavior_listBox->SelectedIndex == -1 )
				{
					WARNING_MSG("please select");
					return;
				}
				ParticleBehavior_listBox->Items->RemoveAt(l_iSelectedIndex);
				m_pParticleEmitterGroup->RemoveObject(l_iSelectedIndex);
			 }
			 else
			 if( sender == AddParticleEmitter_button )
			 {
				 if( EmitterName_textBox->Text->Length == 0 )
					 return;
				 std::wstring l_strName = DNCT::GcStringToWchar(EmitterName_textBox->Text);
				 if( !g_pPaticleManager->GetObject(m_pParticleEmiterWithShowPosition->GetPrtEmitter()->GetName()) )
				 {
					 WARNING_MSG("particle must add in to list before save this behavior emitter");
					return;
				 }
				 if(ParticleBehavior_listBox->Items->Contains(EmitterName_textBox->Text))
				 //if( m_pBehaviorPaticleManager->GetObject(l_strName) )
				 {
					WARING_YES_NO_TO_NO("this emiter is exists,replace original one?")
						return;
					ParticleBehavior_listBox->Items->Remove(EmitterName_textBox->Text);
					m_pParticleEmitterGroup->RemoveObject(l_strName);
				 }
				 AssignUIToParticleBehaviorData(true);
				 //to save emit direction into the data store,if this is ignore it emit direction will lost.
				 m_pParticleEmiterWithShowPosition->SetEmitDirection(m_pParticleEmiterWithShowPosition->GetPrtEmitter()->GetEmitDirection());
				 cParticleEmiterWithShowPosition*l_pParticleEmiterWithShowPosition = new cParticleEmiterWithShowPosition(m_pParticleEmiterWithShowPosition);
				 l_pParticleEmiterWithShowPosition->SetName(l_strName);
				 bool	l_b = m_pParticleEmitterGroup->AddObject(l_pParticleEmiterWithShowPosition);
				 assert(l_b);
				 m_pParticleEmitterGroup->RefreshTotalPlayTime();
				 ParticleBehavior_listBox->Items->Add(EmitterName_textBox->Text);
			 }
		 }
private: System::Void PathType_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( PathType_comboBox->SelectedIndex != -1 )
			 {
				 if( PathType_comboBox->SelectedIndex == 0 )
					Path_listBox->Enabled = false;
				 else
				 {
					Path_listBox->Enabled = true;
				 }
				 m_pParticleEmiterWithShowPosition->SetPathType((cParticleEmiterWithShowPosition::ePathType)PathType_comboBox->SelectedIndex);
				if( m_pBehaviorPaticleManager->m_CurveManager.Count() && PathType_comboBox->SelectedIndex != 0 )
				{
					if(!m_pParticleEmiterWithShowPosition->GetCurveWithTime())
						m_pParticleEmiterWithShowPosition->SetCurveWithTime(m_pBehaviorPaticleManager->m_CurveManager[0]);
				}
			 }
		 }
private: System::Void EmitterStop_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 *m_pbParticleUpdate = false;
			 if( sender == Play_checkBox )
			 {
				 AllPlay_checkBox->Checked = false;
				 if( !Play_checkBox->Checked )
				 {
					 m_pParticleEmiterWithShowPosition->SetAnimationDone(true);
					 if(m_pParticleEmiterWithShowPosition->GetPrtEmitter())
					    m_pParticleEmiterWithShowPosition->GetPrtEmitter()->Stop();
				 }
				 else
				 {
					 //set 1 second for default value
					 if(*EndTime_numericalControl->m_pValue <= 0.f )
					 {
						*EndTime_numericalControl->m_pValue = 1.f;
						EndTime_numericalControl->AssignValueToUI();
					 }
					 AssignUIToParticleBehaviorData(true);
					 m_pParticleEmiterWithShowPosition->Init();
					 Progress_trackBar->Maximum = (int)(m_pParticleEmiterWithShowPosition->GetEndTime()*1000.f);
				 }
			 }
			 else
			 if( sender == AllPlay_checkBox )
			 {
				if( AllPlay_checkBox->Checked )
				{
					Play_checkBox->Checked = false;
					m_pParticleEmitterGroup->RefreshTotalPlayTime();
					m_pParticleEmitterGroup->SetAnimationLoop(AllCurveLoop_checkBox->Checked);
					m_pParticleEmitterGroup->Init();
					//m_pParticleEmitterGroup->SetPlayCount(1);
					ParticleBehavior_listBox->SelectedIndices->Clear();
				}
				else
				{
				    m_pParticleEmitterGroup->Stop();
				}
			 }
		 }
private: System::Void Progress_trackBar_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 Progress_label->Text = Progress_trackBar->Value.ToString();
		 }
private: System::Void Path_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( Path_listBox->SelectedIndex != -1 )
			 {
				EndTime_numericalControl->numericUpDown->Value = System::Decimal(m_pBehaviorPaticleManager->m_CurveManager[Path_listBox->SelectedIndex]->GetEndTime()); 
				m_pParticleEmiterWithShowPosition->SetCurveWithTime(m_pBehaviorPaticleManager->m_CurveManager[Path_listBox->SelectedIndex]);
			 }			 
		 }

private: System::Void AddAnimationToList_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(AnimationName_textBox->Text->Length)
			 {
				 if(!WholeAnimationList_listBox->Items->Contains(AnimationName_textBox->Text))
				 {
					 if( m_pParticleEmitterGroup->Count() == 0 )
					 {
						 WARNING_MSG("no data!!");
						 return;
					 }
					 cParticleEmitterGroup*l_pParticleEmitterGroup = new cParticleEmitterGroup(m_pParticleEmitterGroup);
					 l_pParticleEmitterGroup->SetName(DNCT::GcStringToWchar(AnimationName_textBox->Text));
					 this->m_pBehaviorPaticleManager->AddObject(l_pParticleEmitterGroup);
					 WholeAnimationList_listBox->Items->Add(AnimationName_textBox->Text);
				 }
				 else
				 {
					WARNING_MSG("name exists!!");
				 }
			 }
		 }
private: System::Void WholeAnimationList_listBox_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 if( sender == ParticleBehavior_listBox )
			 {
				 if( ParticleBehavior_listBox->SelectedIndex != -1 )
				 {
					 EmitterName_textBox->Text = ParticleBehavior_listBox->SelectedItem->ToString();
				 }
			 }
			 else
			 if( sender == WholeAnimationList_listBox )
			 {
				if( WholeAnimationList_listBox->SelectedIndex != -1 )
				{
					AnimationName_textBox->Text = WholeAnimationList_listBox->SelectedItem->ToString();
				}
			 }
		 }
private: System::Void WholeAnimationList_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 if( sender == ParticleBehavior_listBox )
			 {
				 if( ParticleBehavior_listBox->SelectedIndex != -1 )
				 {
					//to avoid kill the target one
					m_pParticleEmiterWithShowPosition->SetPrtEmitter(0);
					SAFE_DELETE(m_pParticleEmiterWithShowPosition);
					m_pParticleEmiterWithShowPosition = new cParticleEmiterWithShowPosition(m_pParticleEmitterGroup->GetObject(ParticleBehavior_listBox->SelectedIndex));
					int	l_ii = (int)m_pParticleEmiterWithShowPosition->GetPathType();
					PathType_comboBox->SelectedIndex = (int)m_pParticleEmiterWithShowPosition->GetPathType();
					int	l_iIndex = -1;
					if( m_pParticleEmiterWithShowPosition->GetCurveWithTime() )
					{
						l_iIndex = Path_listBox->Items->IndexOf(DNCT::WcharToGcstring(m_pParticleEmiterWithShowPosition->GetCurveWithTime()->GetName()));
					}
					
					Path_listBox->SelectedIndex = l_iIndex;
					this->m_bRfreshParticleEmiterData = true;
					Loop_checkBox->Checked = m_pParticleEmiterWithShowPosition->IsAnimationLoop();
					
				 }
			 }
			 else
			 if( sender == WholeAnimationList_listBox )
			 {
				if( WholeAnimationList_listBox->SelectedIndex != -1 )
				{
					cParticleEmitterGroup*l_pParticleEmitterGroup = dynamic_cast<cParticleEmitterGroup*>(this->m_pBehaviorPaticleManager->GetObject(WholeAnimationList_listBox->SelectedIndex));
					ParticleBehavior_listBox->Items->Clear();
					for(int i=0;i<l_pParticleEmitterGroup->Count();++i)
					{
						ParticleBehavior_listBox->Items->Add(DNCT::WcharToGcstring(l_pParticleEmitterGroup->GetObject(i)->GetName()));
					}
					//set data to target
					SAFE_DELETE(m_pParticleEmitterGroup);
					m_pParticleEmitterGroup = new cParticleEmitterGroup(l_pParticleEmitterGroup);
					ParticleBehavior_listBox->SelectedIndex = 0;
					WholeAnimationList_listBox_MouseDoubleClick(ParticleBehavior_listBox,e);
					AllCurveLoop_checkBox->Checked = l_pParticleEmitterGroup->IsAnimationLoop();
				}
			 }
		 }
private: System::Void ParticleBehavior_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			if( e->KeyCode == GCFORM::Keys::Enter )
			{
				System::Windows::Forms::MouseEventArgs^  e2;
				WholeAnimationList_listBox_MouseDoubleClick(sender,e2);
			}
		 }
private: System::Void AnimationInvert_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 System::Windows::Forms::MouseEventArgs^  e2;
			 if( sender == AnimationInvert_button )
			 {
				WholeAnimationList_listBox_MouseDoubleClick(WholeAnimationList_listBox,e2);
			 }
			 else
			 if( sender == ParticleBehaviorInvert_button )
			 {
				 WholeAnimationList_listBox_MouseDoubleClick(ParticleBehavior_listBox,e2);
			 }
		 }
private: System::Void RotateType_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			RefreshPathData();
		 }
private: System::Void Loop_checkBox_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 AssignUIToParticleBehaviorData(true);
		 }
};
}
