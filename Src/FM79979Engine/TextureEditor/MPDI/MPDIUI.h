#pragma once

#include "../../Core/Image/MPDIForEditor.h"
#include "MPDIThread.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Windows::Forms::Integration;

namespace MPDI 
{
	String^CueToStartCurveWithTimeDataCheck(cCueToStartCurveWithTime*e_pCueToStartCurveWithTime);
	String^MultiPathDynamicImageDataCheck(cMultiPathDynamicImage*e_pMultiPathDynamicImage);
	String^MPDIListDataCheck(cMPDIList*e_pMPDIList);
	eAnimationEvent	IntToAnimationEvent(int e_iIndex);
	char*	eAnimationEventToAnimationEventString(eAnimationEvent e_eAnimationEvent);
	void	RenderHintPoint(cCueToStartCurveWithTime*e_pObject,bool e_bShowBorder);
	public ref class MPDIUI : public System::Windows::Forms::UserControl
	{
	public:
		MPDIUI(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			RotationPosOffset_xyzNumeriaclContro->MyNumericUpDown1->Increment = 1;
			RotationPosOffset_xyzNumeriaclContro->MyNumericUpDown2->Increment = 1;
			RotationPosOffset_xyzNumeriaclContro->MyNumericUpDown3->Increment = 1;
			ImageAnchor_comboBox->SelectedIndex = 0;
			GCFORM::ToolTip^l_pToolTip1 = gcnew GCFORM::ToolTip();
			l_pToolTip1->SetToolTip( this->WholePuzzleImageUnit_listBox, "select images and click right mouse button to add path" );
			l_pToolTip1->SetToolTip( this->PathNode_listBox, "click right mouse button to Fade in Fade out " );
			l_pToolTip1->SetToolTip( this->WholePath_listBox, "click right mouse button to add game UI" );

			LanguageSwitch(Controls,"/",this->Handle);
			LanguageSwitch(groupBox1->Controls,"/",this->Handle);
			for(int i=0;i<tabControl->TabPages->Count;++i)
			{
				LanguageSwitch(tabControl->TabPages[i]->Controls,"/",this->Handle);
			}
			//LanguageSwitch(Color_groupBox->Controls,"/",this->Handle);
			//LanguageSwitch(Position_groupBox->Controls,"/",this->Handle);
			//LanguageSwitch(ColorBlending_groupBox->Controls,"/",this->Handle);
			//LanguageSwitch(Rotation_groupBox->Controls,"/",this->Handle);
			//if crash occur
			{// http://stackoverflow.com/questions/13315940/apps-exe-file-missing-net-targetframework-but-only-on-clean-builds
				//if here occur crash clear temp file
				//C:\Users\UserName\AppData\Local\Temp
				ElementHost^l_pHost = GetMyElementHost(l_pHost);
				l_pHost->Dock = DockStyle::Fill;
				m_pColorPicker = gcnew CoreWPF::Common::ColorPicker;
				m_pColorPicker->SetUsingAlphaChannel(false);
				l_pHost->Child = m_pColorPicker;
				ColorPicker_panel->Controls->Add(l_pHost);

				m_pColorPicker->f_SelectedColorChanged += gcnew System::EventHandler(this, &MPDIUI::ColorChanged);
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MPDIUI()
		{
			delete m_pColorPicker;
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::Button^  SelectEvenPoints_button;
	public: System::Windows::Forms::GroupBox^  groupBox1;
	public: System::Windows::Forms::NumericUpDown^  TotalPlayTime_numericUpDown;
	public: System::Windows::Forms::Label^  label8;
	public: System::Windows::Forms::Label^  label4;
	public: System::Windows::Forms::Label^  label20;
	public: System::Windows::Forms::NumericUpDown^  RearrangeWholePathTime_numericUpDown;
	public: System::Windows::Forms::Label^  label1;
	public: System::Windows::Forms::NumericUpDown^  PathNodeTime_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  StartTime_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  PathLOD_numericUpDown;
	public: System::Windows::Forms::Label^  label14;
	public: System::Windows::Forms::ListBox^  WholePath_listBox;
	public: System::Windows::Forms::GroupBox^  Rotation_groupBox;
	public: System::Windows::Forms::Label^  label3;
	public: System::Windows::Forms::NumericUpDown^  AngleY_numericUpDown;
	public: System::Windows::Forms::Label^  label2;
	public: System::Windows::Forms::NumericUpDown^  AngleX_numericUpDown;
	public: System::Windows::Forms::Label^  label5;
	public: System::Windows::Forms::NumericUpDown^  AngleZ_numericUpDown;
	public: System::Windows::Forms::ListBox^  WholePuzzleImageUnit_listBox;
	public: System::Windows::Forms::CheckBox^  Loop_checkBox;
	public: System::Windows::Forms::NumericUpDown^  NumPointsToInsert_numericUpDown;
	public: System::Windows::Forms::Button^  ApplyLODToPoints_button;
	public: System::Windows::Forms::Button^  AssignImage_button;
	public: System::Windows::Forms::GroupBox^  ColorBlending_groupBox;
	public: System::Windows::Forms::CheckBox^  EnableColorBlending_checkBox;
	public: System::Windows::Forms::ComboBox^  SrcBlending_comboBox;
	public: System::Windows::Forms::ComboBox^  DestBlending_comboBox;
	public: System::Windows::Forms::Label^  label18;
	public: System::Windows::Forms::Label^  label19;
	public: System::Windows::Forms::Button^  InsertPoint_button;
	public: System::Windows::Forms::CheckBox^  BehaviorUpdateActive_checkBox;
	public: System::Windows::Forms::Label^  label25;
	public: System::Windows::Forms::Label^  ErrorMsg_label;
	public: System::Windows::Forms::Button^  PathPiorityDown_button;
	public: System::Windows::Forms::Button^  InvertAllPath_button;
	public: System::Windows::Forms::CheckBox^  ShowSelectedImageBorder_checkBox;
	public: System::Windows::Forms::Button^  PathPiorityUp_button;
	public: System::Windows::Forms::Button^  SelectAllPoints_button;
	public: System::Windows::Forms::Button^  DelPI_button;
	public: System::Windows::Forms::Button^  DelPoint_button;
	public: System::Windows::Forms::CheckBox^  ShowAllPath_checkBox;
	public: System::Windows::Forms::ComboBox^  ImageAnchor_comboBox;
	public: System::Windows::Forms::Button^  AddPath_button;
	public: System::Windows::Forms::CheckBox^  AverageAssign_checkBox;
	public: System::Windows::Forms::CheckBox^  UseViewPort_checkBox;
	public: System::Windows::Forms::Button^  DelPath_button;
	public: System::Windows::Forms::Button^  SelectFirstAndLast_button;
	public: System::Windows::Forms::ListBox^  AllPI_listBox;
	public: System::Windows::Forms::CheckBox^  Play_checkBox;
	public: System::Windows::Forms::ListBox^  PathNode_listBox;
	public: System::Windows::Forms::GroupBox^  Size_groupBox;
	public: System::Windows::Forms::Label^  label6;
	public: System::Windows::Forms::Label^  label21;
	public: System::Windows::Forms::NumericUpDown^  Width_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  Height_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  SizeScale_numericUpDown;
	public: System::Windows::Forms::Label^  label7;
	public: System::Windows::Forms::CheckBox^  Mirror_checkBox;
	public: DotNetComponent::XYZWNumericalControl^  ViewPort_xyzwNumericalControl;
	public: System::Windows::Forms::Label^  label22;
	public: System::Windows::Forms::Button^  ClonePath_button;
	public: System::Windows::Forms::GroupBox^  Color_groupBox;
	public: System::Windows::Forms::NumericUpDown^  RedColor_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  GreenColor_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  AlphaColor_numericUpDown;
	public: System::Windows::Forms::Label^  label12;
	public: System::Windows::Forms::NumericUpDown^  BlueColor_numericUpDown;
	public: System::Windows::Forms::Label^  label11;
	public: System::Windows::Forms::Label^  label13;
	public: System::Windows::Forms::Label^  label10;
	public: DotNetComponent::XYZNumeriaclControl^  RotationPosOffset_xyzNumeriaclContro;
	public: System::Windows::Forms::CheckBox^  SubMPDI_StayAtLastFrame_checkBox;

	public: System::Windows::Forms::Button^  InvertPath_button;
	public: System::Windows::Forms::GroupBox^  Position_groupBox;
	public: System::Windows::Forms::NumericUpDown^  PathNodePosX_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  PathNodePosY_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  PathNodePosZ_numericUpDown;
	public: System::Windows::Forms::ContextMenuStrip^  PointDataUtility_contextMenuStrip;
	public: System::Windows::Forms::ToolStripMenuItem^  FadeIn_toolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  FadeOut_toolStripMenuItem;
	public: System::Windows::Forms::ContextMenuStrip^  PathGroup_contextMenuStrip;
	public: System::Windows::Forms::ContextMenuStrip^  GameUIData_contextMenuStrip;
	public: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;
	public: System::Windows::Forms::ToolStripMenuItem^  addPointDataWithImageByPathNameToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  AddSliderBarUI_toolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  AddMultiPageUI_toolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  addSliderBarWithTabPagesUIToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  addMultipageWithTabPagesUIToolStripMenuItem;
	private: System::Windows::Forms::TabControl^  tabControl;
	private: System::Windows::Forms::TabPage^  ViewPort_tabPage;
	private: System::Windows::Forms::TabPage^  ColorBlending;
	private: System::Windows::Forms::TabPage^  RotationOffset;
	private: System::Windows::Forms::Panel^  ColorPicker_panel;
	public: System::Windows::Forms::ToolStripMenuItem^  yesNoDialogToolStripMenuItem;
public:
	//my
	CoreWPF::Common::ColorPicker^m_pColorPicker;
	private: System::Void ColorChanged(System::Object^  sender, System::EventArgs^  e);
	public: void	AssignLatestPI()
	{
		if(AllPI_listBox->Items->Count)
		{
			AllPI_listBox->SelectedItems->Clear();
			AllPI_listBox->SelectedIndex = AllPI_listBox->Items->Count-1;
		}
		else
		{
			AllPI_listBox->SelectedIndex = -1;
		}
	}
private: System::ComponentModel::IContainer^  components;
	public:
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
			this->SelectEvenPoints_button = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->TotalPlayTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->RearrangeWholePathTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->PathNodeTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->StartTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->PathLOD_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->WholePath_listBox = (gcnew System::Windows::Forms::ListBox());
			this->Rotation_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->AngleY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->AngleX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->AngleZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->WholePuzzleImageUnit_listBox = (gcnew System::Windows::Forms::ListBox());
			this->Loop_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->NumPointsToInsert_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ApplyLODToPoints_button = (gcnew System::Windows::Forms::Button());
			this->AssignImage_button = (gcnew System::Windows::Forms::Button());
			this->ColorBlending_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->EnableColorBlending_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->SrcBlending_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->DestBlending_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->InsertPoint_button = (gcnew System::Windows::Forms::Button());
			this->BehaviorUpdateActive_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->ErrorMsg_label = (gcnew System::Windows::Forms::Label());
			this->PathPiorityDown_button = (gcnew System::Windows::Forms::Button());
			this->InvertAllPath_button = (gcnew System::Windows::Forms::Button());
			this->ShowSelectedImageBorder_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->PathPiorityUp_button = (gcnew System::Windows::Forms::Button());
			this->SelectAllPoints_button = (gcnew System::Windows::Forms::Button());
			this->DelPI_button = (gcnew System::Windows::Forms::Button());
			this->DelPoint_button = (gcnew System::Windows::Forms::Button());
			this->ShowAllPath_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ImageAnchor_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->AddPath_button = (gcnew System::Windows::Forms::Button());
			this->AverageAssign_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->UseViewPort_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->DelPath_button = (gcnew System::Windows::Forms::Button());
			this->SelectFirstAndLast_button = (gcnew System::Windows::Forms::Button());
			this->AllPI_listBox = (gcnew System::Windows::Forms::ListBox());
			this->Play_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->PathNode_listBox = (gcnew System::Windows::Forms::ListBox());
			this->Size_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->Width_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Height_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->SizeScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->Mirror_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ViewPort_xyzwNumericalControl = (gcnew DotNetComponent::XYZWNumericalControl());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->ClonePath_button = (gcnew System::Windows::Forms::Button());
			this->Color_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->RedColor_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GreenColor_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->AlphaColor_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ColorPicker_panel = (gcnew System::Windows::Forms::Panel());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->BlueColor_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->RotationPosOffset_xyzNumeriaclContro = (gcnew DotNetComponent::XYZNumeriaclControl());
			this->SubMPDI_StayAtLastFrame_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->InvertPath_button = (gcnew System::Windows::Forms::Button());
			this->Position_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->PathNodePosX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->PathNodePosY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->PathNodePosZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->PointDataUtility_contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->FadeIn_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->FadeOut_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->PathGroup_contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addPointDataWithImageByPathNameToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->GameUIData_contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->AddSliderBarUI_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AddMultiPageUI_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addSliderBarWithTabPagesUIToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addMultipageWithTabPagesUIToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->yesNoDialogToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tabControl = (gcnew System::Windows::Forms::TabControl());
			this->ViewPort_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->ColorBlending = (gcnew System::Windows::Forms::TabPage());
			this->RotationOffset = (gcnew System::Windows::Forms::TabPage());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TotalPlayTime_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->RearrangeWholePathTime_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PathNodeTime_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->StartTime_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PathLOD_numericUpDown))->BeginInit();
			this->Rotation_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->AngleY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->AngleX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->AngleZ_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->NumPointsToInsert_numericUpDown))->BeginInit();
			this->ColorBlending_groupBox->SuspendLayout();
			this->Size_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Width_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Height_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SizeScale_numericUpDown))->BeginInit();
			this->Color_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->RedColor_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GreenColor_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->AlphaColor_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BlueColor_numericUpDown))->BeginInit();
			this->Position_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PathNodePosX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PathNodePosY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PathNodePosZ_numericUpDown))->BeginInit();
			this->PointDataUtility_contextMenuStrip->SuspendLayout();
			this->PathGroup_contextMenuStrip->SuspendLayout();
			this->GameUIData_contextMenuStrip->SuspendLayout();
			this->tabControl->SuspendLayout();
			this->ViewPort_tabPage->SuspendLayout();
			this->ColorBlending->SuspendLayout();
			this->RotationOffset->SuspendLayout();
			this->SuspendLayout();
			// 
			// SelectEvenPoints_button
			// 
			this->SelectEvenPoints_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->SelectEvenPoints_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->SelectEvenPoints_button->Location = System::Drawing::Point(943, 155);
			this->SelectEvenPoints_button->Name = L"SelectEvenPoints_button";
			this->SelectEvenPoints_button->Size = System::Drawing::Size(102, 20);
			this->SelectEvenPoints_button->TabIndex = 199;
			this->SelectEvenPoints_button->Text = L"SelectEvenPoints/選偶數";
			this->SelectEvenPoints_button->UseVisualStyleBackColor = false;
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->groupBox1->Controls->Add(this->TotalPlayTime_numericUpDown);
			this->groupBox1->Controls->Add(this->label8);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label20);
			this->groupBox1->Controls->Add(this->RearrangeWholePathTime_numericUpDown);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->PathNodeTime_numericUpDown);
			this->groupBox1->Controls->Add(this->StartTime_numericUpDown);
			this->groupBox1->Controls->Add(this->PathLOD_numericUpDown);
			this->groupBox1->Controls->Add(this->label14);
			this->groupBox1->Location = System::Drawing::Point(378, 3);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(184, 154);
			this->groupBox1->TabIndex = 160;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Path";
			// 
			// TotalPlayTime_numericUpDown
			// 
			this->TotalPlayTime_numericUpDown->Location = System::Drawing::Point(6, 21);
			this->TotalPlayTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 999999999, 0, 0, 0 });
			this->TotalPlayTime_numericUpDown->Name = L"TotalPlayTime_numericUpDown";
			this->TotalPlayTime_numericUpDown->Size = System::Drawing::Size(70, 22);
			this->TotalPlayTime_numericUpDown->TabIndex = 75;
			// 
			// label8
			// 
			this->label8->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label8->AutoSize = true;
			this->label8->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label8->Location = System::Drawing::Point(83, 23);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(112, 12);
			this->label8->TabIndex = 76;
			this->label8->Text = L"TotalPlayTime/總時間";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(82, 127);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(87, 12);
			this->label4->TabIndex = 71;
			this->label4->Text = L"PathLOD/精細度";
			// 
			// label20
			// 
			this->label20->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label20->AutoSize = true;
			this->label20->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label20->Location = System::Drawing::Point(82, 97);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(199, 12);
			this->label20->TabIndex = 136;
			this->label20->Text = L"NewTimeForWholPath/設定整體新時間";
			this->label20->TextAlign = System::Drawing::ContentAlignment::BottomRight;
			// 
			// RearrangeWholePathTime_numericUpDown
			// 
			this->RearrangeWholePathTime_numericUpDown->Location = System::Drawing::Point(6, 95);
			this->RearrangeWholePathTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {
				999999999, 0,
					0, 0
			});
			this->RearrangeWholePathTime_numericUpDown->Name = L"RearrangeWholePathTime_numericUpDown";
			this->RearrangeWholePathTime_numericUpDown->Size = System::Drawing::Size(70, 22);
			this->RearrangeWholePathTime_numericUpDown->TabIndex = 135;
			// 
			// label1
			// 
			this->label1->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label1->Location = System::Drawing::Point(83, 71);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(107, 12);
			this->label1->TabIndex = 67;
			this->label1->Text = L"perTime/單獨點時間";
			// 
			// PathNodeTime_numericUpDown
			// 
			this->PathNodeTime_numericUpDown->Location = System::Drawing::Point(6, 71);
			this->PathNodeTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000000, 0, 0, 0 });
			this->PathNodeTime_numericUpDown->Name = L"PathNodeTime_numericUpDown";
			this->PathNodeTime_numericUpDown->Size = System::Drawing::Size(68, 22);
			this->PathNodeTime_numericUpDown->TabIndex = 64;
			// 
			// StartTime_numericUpDown
			// 
			this->StartTime_numericUpDown->Location = System::Drawing::Point(6, 46);
			this->StartTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 999999999, 0, 0, 0 });
			this->StartTime_numericUpDown->Name = L"StartTime_numericUpDown";
			this->StartTime_numericUpDown->Size = System::Drawing::Size(70, 22);
			this->StartTime_numericUpDown->TabIndex = 109;
			// 
			// PathLOD_numericUpDown
			// 
			this->PathLOD_numericUpDown->Location = System::Drawing::Point(6, 120);
			this->PathLOD_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 7, 0, 0, 0 });
			this->PathLOD_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->PathLOD_numericUpDown->Name = L"PathLOD_numericUpDown";
			this->PathLOD_numericUpDown->Size = System::Drawing::Size(69, 22);
			this->PathLOD_numericUpDown->TabIndex = 70;
			this->PathLOD_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// label14
			// 
			this->label14->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label14->AutoSize = true;
			this->label14->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label14->Location = System::Drawing::Point(84, 49);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(121, 12);
			this->label14->TabIndex = 110;
			this->label14->Text = L"PathStartTime/開始時間";
			// 
			// WholePath_listBox
			// 
			this->WholePath_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->WholePath_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholePath_listBox->FormattingEnabled = true;
			this->WholePath_listBox->HorizontalScrollbar = true;
			this->WholePath_listBox->ItemHeight = 12;
			this->WholePath_listBox->Location = System::Drawing::Point(3, 10);
			this->WholePath_listBox->MinimumSize = System::Drawing::Size(4, 73);
			this->WholePath_listBox->Name = L"WholePath_listBox";
			this->WholePath_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->WholePath_listBox->Size = System::Drawing::Size(156, 244);
			this->WholePath_listBox->TabIndex = 163;
			// 
			// Rotation_groupBox
			// 
			this->Rotation_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Rotation_groupBox->Controls->Add(this->label3);
			this->Rotation_groupBox->Controls->Add(this->AngleY_numericUpDown);
			this->Rotation_groupBox->Controls->Add(this->label2);
			this->Rotation_groupBox->Controls->Add(this->AngleX_numericUpDown);
			this->Rotation_groupBox->Controls->Add(this->label5);
			this->Rotation_groupBox->Controls->Add(this->AngleZ_numericUpDown);
			this->Rotation_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Rotation_groupBox->Location = System::Drawing::Point(1183, 5);
			this->Rotation_groupBox->Name = L"Rotation_groupBox";
			this->Rotation_groupBox->Size = System::Drawing::Size(86, 96);
			this->Rotation_groupBox->TabIndex = 156;
			this->Rotation_groupBox->TabStop = false;
			this->Rotation_groupBox->Text = L"Rotation";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label3->Location = System::Drawing::Point(3, 44);
			this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(13, 12);
			this->label3->TabIndex = 93;
			this->label3->Text = L"Y";
			// 
			// AngleY_numericUpDown
			// 
			this->AngleY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AngleY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AngleY_numericUpDown->Location = System::Drawing::Point(25, 42);
			this->AngleY_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->AngleY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->AngleY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->AngleY_numericUpDown->Name = L"AngleY_numericUpDown";
			this->AngleY_numericUpDown->Size = System::Drawing::Size(48, 22);
			this->AngleY_numericUpDown->TabIndex = 4;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label2->Location = System::Drawing::Point(3, 19);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(13, 12);
			this->label2->TabIndex = 91;
			this->label2->Text = L"X";
			// 
			// AngleX_numericUpDown
			// 
			this->AngleX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AngleX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AngleX_numericUpDown->Location = System::Drawing::Point(24, 13);
			this->AngleX_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->AngleX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->AngleX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->AngleX_numericUpDown->Name = L"AngleX_numericUpDown";
			this->AngleX_numericUpDown->Size = System::Drawing::Size(48, 22);
			this->AngleX_numericUpDown->TabIndex = 3;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label5->Location = System::Drawing::Point(3, 74);
			this->label5->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(12, 12);
			this->label5->TabIndex = 89;
			this->label5->Text = L"Z";
			// 
			// AngleZ_numericUpDown
			// 
			this->AngleZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AngleZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AngleZ_numericUpDown->Location = System::Drawing::Point(24, 69);
			this->AngleZ_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->AngleZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->AngleZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->AngleZ_numericUpDown->Name = L"AngleZ_numericUpDown";
			this->AngleZ_numericUpDown->Size = System::Drawing::Size(48, 22);
			this->AngleZ_numericUpDown->TabIndex = 5;
			// 
			// WholePuzzleImageUnit_listBox
			// 
			this->WholePuzzleImageUnit_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->WholePuzzleImageUnit_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholePuzzleImageUnit_listBox->FormattingEnabled = true;
			this->WholePuzzleImageUnit_listBox->HorizontalScrollbar = true;
			this->WholePuzzleImageUnit_listBox->ItemHeight = 12;
			this->WholePuzzleImageUnit_listBox->Location = System::Drawing::Point(580, 12);
			this->WholePuzzleImageUnit_listBox->MinimumSize = System::Drawing::Size(4, 74);
			this->WholePuzzleImageUnit_listBox->Name = L"WholePuzzleImageUnit_listBox";
			this->WholePuzzleImageUnit_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->WholePuzzleImageUnit_listBox->Size = System::Drawing::Size(173, 220);
			this->WholePuzzleImageUnit_listBox->TabIndex = 169;
			// 
			// Loop_checkBox
			// 
			this->Loop_checkBox->AutoSize = true;
			this->Loop_checkBox->Enabled = false;
			this->Loop_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8));
			this->Loop_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Loop_checkBox->Location = System::Drawing::Point(186, 197);
			this->Loop_checkBox->Name = L"Loop_checkBox";
			this->Loop_checkBox->Size = System::Drawing::Size(162, 15);
			this->Loop_checkBox->TabIndex = 167;
			this->Loop_checkBox->Text = L"SingleCurveLoop/單一路徑loop";
			this->Loop_checkBox->UseVisualStyleBackColor = true;
			// 
			// NumPointsToInsert_numericUpDown
			// 
			this->NumPointsToInsert_numericUpDown->Location = System::Drawing::Point(834, 205);
			this->NumPointsToInsert_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->NumPointsToInsert_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->NumPointsToInsert_numericUpDown->Name = L"NumPointsToInsert_numericUpDown";
			this->NumPointsToInsert_numericUpDown->Size = System::Drawing::Size(58, 22);
			this->NumPointsToInsert_numericUpDown->TabIndex = 178;
			this->NumPointsToInsert_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// ApplyLODToPoints_button
			// 
			this->ApplyLODToPoints_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ApplyLODToPoints_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ApplyLODToPoints_button->Location = System::Drawing::Point(186, 156);
			this->ApplyLODToPoints_button->Name = L"ApplyLODToPoints_button";
			this->ApplyLODToPoints_button->Size = System::Drawing::Size(92, 19);
			this->ApplyLODToPoints_button->TabIndex = 180;
			this->ApplyLODToPoints_button->Text = L"LODToPoints/LOD點數轉換";
			this->ApplyLODToPoints_button->UseVisualStyleBackColor = false;
			// 
			// AssignImage_button
			// 
			this->AssignImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AssignImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AssignImage_button->Location = System::Drawing::Point(602, 238);
			this->AssignImage_button->Name = L"AssignImage_button";
			this->AssignImage_button->Size = System::Drawing::Size(156, 22);
			this->AssignImage_button->TabIndex = 170;
			this->AssignImage_button->Text = L"ApplyImageToPoint/將所選圖片應用到點上";
			this->AssignImage_button->UseVisualStyleBackColor = false;
			// 
			// ColorBlending_groupBox
			// 
			this->ColorBlending_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ColorBlending_groupBox->Controls->Add(this->EnableColorBlending_checkBox);
			this->ColorBlending_groupBox->Controls->Add(this->SrcBlending_comboBox);
			this->ColorBlending_groupBox->Controls->Add(this->DestBlending_comboBox);
			this->ColorBlending_groupBox->Controls->Add(this->label18);
			this->ColorBlending_groupBox->Controls->Add(this->label19);
			this->ColorBlending_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ColorBlending_groupBox->Location = System::Drawing::Point(6, 17);
			this->ColorBlending_groupBox->Name = L"ColorBlending_groupBox";
			this->ColorBlending_groupBox->Size = System::Drawing::Size(140, 113);
			this->ColorBlending_groupBox->TabIndex = 158;
			this->ColorBlending_groupBox->TabStop = false;
			this->ColorBlending_groupBox->Text = L"Color Blending";
			// 
			// EnableColorBlending_checkBox
			// 
			this->EnableColorBlending_checkBox->AutoSize = true;
			this->EnableColorBlending_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->EnableColorBlending_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->EnableColorBlending_checkBox->Location = System::Drawing::Point(6, 14);
			this->EnableColorBlending_checkBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->EnableColorBlending_checkBox->Name = L"EnableColorBlending_checkBox";
			this->EnableColorBlending_checkBox->Size = System::Drawing::Size(177, 16);
			this->EnableColorBlending_checkBox->TabIndex = 141;
			this->EnableColorBlending_checkBox->Text = L"EnableColorBlending/顏色混合";
			this->EnableColorBlending_checkBox->UseVisualStyleBackColor = false;
			// 
			// SrcBlending_comboBox
			// 
			this->SrcBlending_comboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->SrcBlending_comboBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->SrcBlending_comboBox->FormattingEnabled = true;
			this->SrcBlending_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(9) {
				L"GL_ZERO", L"GL_ONE", L"GL_ONE_MINUS_SRC_COLOR",
					L"GL_DST_COLOR", L"GL_ONE_MINUS_DST_COLOR", L"GL_SRC_ALPHA", L"GL_ONE_MINUS_SRC_ALPHA", L"GL_DST_ALPHA", L"GL_ONE_MINUS_DST_ALPHA"
			});
			this->SrcBlending_comboBox->Location = System::Drawing::Point(6, 46);
			this->SrcBlending_comboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->SrcBlending_comboBox->Name = L"SrcBlending_comboBox";
			this->SrcBlending_comboBox->Size = System::Drawing::Size(128, 20);
			this->SrcBlending_comboBox->TabIndex = 14;
			// 
			// DestBlending_comboBox
			// 
			this->DestBlending_comboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DestBlending_comboBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DestBlending_comboBox->FormattingEnabled = true;
			this->DestBlending_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(9) {
				L"GL_ZERO", L"GL_ONE", L"GL_ONE_MINUS_SRC_COLOR",
					L"GL_DST_COLOR", L"GL_ONE_MINUS_DST_COLOR", L"GL_SRC_ALPHA", L"GL_ONE_MINUS_SRC_ALPHA", L"GL_DST_ALPHA", L"GL_ONE_MINUS_DST_ALPHA"
			});
			this->DestBlending_comboBox->Location = System::Drawing::Point(7, 85);
			this->DestBlending_comboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->DestBlending_comboBox->Name = L"DestBlending_comboBox";
			this->DestBlending_comboBox->Size = System::Drawing::Size(127, 20);
			this->DestBlending_comboBox->TabIndex = 15;
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label18->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label18->Location = System::Drawing::Point(7, 33);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(47, 12);
			this->label18->TabIndex = 144;
			this->label18->Text = L"Src/圖檔";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label19->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label19->Location = System::Drawing::Point(7, 71);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(52, 12);
			this->label19->TabIndex = 145;
			this->label19->Text = L"Dest/目的";
			// 
			// InsertPoint_button
			// 
			this->InsertPoint_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->InsertPoint_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->InsertPoint_button->Location = System::Drawing::Point(759, 206);
			this->InsertPoint_button->Name = L"InsertPoint_button";
			this->InsertPoint_button->Size = System::Drawing::Size(69, 20);
			this->InsertPoint_button->TabIndex = 176;
			this->InsertPoint_button->Text = L"InsertPoint/插入";
			this->InsertPoint_button->UseVisualStyleBackColor = false;
			// 
			// BehaviorUpdateActive_checkBox
			// 
			this->BehaviorUpdateActive_checkBox->AutoSize = true;
			this->BehaviorUpdateActive_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->BehaviorUpdateActive_checkBox->ForeColor = System::Drawing::Color::White;
			this->BehaviorUpdateActive_checkBox->Location = System::Drawing::Point(833, 252);
			this->BehaviorUpdateActive_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->BehaviorUpdateActive_checkBox->Name = L"BehaviorUpdateActive_checkBox";
			this->BehaviorUpdateActive_checkBox->Size = System::Drawing::Size(175, 16);
			this->BehaviorUpdateActive_checkBox->TabIndex = 181;
			this->BehaviorUpdateActive_checkBox->Text = L"BehaviorUpdate/動態更新資料";
			this->BehaviorUpdateActive_checkBox->UseVisualStyleBackColor = false;
			// 
			// label25
			// 
			this->label25->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label25->AutoSize = true;
			this->label25->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label25->Location = System::Drawing::Point(279, 14);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(69, 12);
			this->label25->TabIndex = 198;
			this->label25->Text = L"ImageAnchor";
			this->label25->TextAlign = System::Drawing::ContentAlignment::BottomRight;
			// 
			// ErrorMsg_label
			// 
			this->ErrorMsg_label->AutoSize = true;
			this->ErrorMsg_label->Location = System::Drawing::Point(759, 234);
			this->ErrorMsg_label->Name = L"ErrorMsg_label";
			this->ErrorMsg_label->Size = System::Drawing::Size(25, 12);
			this->ErrorMsg_label->TabIndex = 166;
			this->ErrorMsg_label->Text = L"Msg";
			// 
			// PathPiorityDown_button
			// 
			this->PathPiorityDown_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathPiorityDown_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathPiorityDown_button->Location = System::Drawing::Point(161, 75);
			this->PathPiorityDown_button->Name = L"PathPiorityDown_button";
			this->PathPiorityDown_button->Size = System::Drawing::Size(22, 32);
			this->PathPiorityDown_button->TabIndex = 183;
			this->PathPiorityDown_button->Text = L"v\r\nv";
			this->PathPiorityDown_button->UseVisualStyleBackColor = false;
			// 
			// InvertAllPath_button
			// 
			this->InvertAllPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->InvertAllPath_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->InvertAllPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->InvertAllPath_button->Location = System::Drawing::Point(186, 128);
			this->InvertAllPath_button->Name = L"InvertAllPath_button";
			this->InvertAllPath_button->Size = System::Drawing::Size(84, 20);
			this->InvertAllPath_button->TabIndex = 191;
			this->InvertAllPath_button->Text = L"InvertAllPath/整體路徑反轉";
			this->InvertAllPath_button->UseVisualStyleBackColor = false;
			// 
			// ShowSelectedImageBorder_checkBox
			// 
			this->ShowSelectedImageBorder_checkBox->AutoSize = true;
			this->ShowSelectedImageBorder_checkBox->Location = System::Drawing::Point(446, 244);
			this->ShowSelectedImageBorder_checkBox->Name = L"ShowSelectedImageBorder_checkBox";
			this->ShowSelectedImageBorder_checkBox->Size = System::Drawing::Size(150, 16);
			this->ShowSelectedImageBorder_checkBox->TabIndex = 189;
			this->ShowSelectedImageBorder_checkBox->Text = L"ShowSelectedImageBorder";
			this->ShowSelectedImageBorder_checkBox->UseVisualStyleBackColor = true;
			// 
			// PathPiorityUp_button
			// 
			this->PathPiorityUp_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathPiorityUp_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathPiorityUp_button->Location = System::Drawing::Point(161, 10);
			this->PathPiorityUp_button->Name = L"PathPiorityUp_button";
			this->PathPiorityUp_button->Size = System::Drawing::Size(22, 39);
			this->PathPiorityUp_button->TabIndex = 184;
			this->PathPiorityUp_button->Text = L"^\r\n^";
			this->PathPiorityUp_button->UseVisualStyleBackColor = false;
			// 
			// SelectAllPoints_button
			// 
			this->SelectAllPoints_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->SelectAllPoints_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->SelectAllPoints_button->Location = System::Drawing::Point(759, 178);
			this->SelectAllPoints_button->Name = L"SelectAllPoints_button";
			this->SelectAllPoints_button->Size = System::Drawing::Size(102, 20);
			this->SelectAllPoints_button->TabIndex = 190;
			this->SelectAllPoints_button->Text = L"SelectAllPoints/全選";
			this->SelectAllPoints_button->UseVisualStyleBackColor = false;
			// 
			// DelPI_button
			// 
			this->DelPI_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelPI_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelPI_button->Location = System::Drawing::Point(378, 242);
			this->DelPI_button->Margin = System::Windows::Forms::Padding(2);
			this->DelPI_button->Name = L"DelPI_button";
			this->DelPI_button->Size = System::Drawing::Size(63, 22);
			this->DelPI_button->TabIndex = 187;
			this->DelPI_button->Text = L"DelPI/砍PI";
			this->DelPI_button->UseVisualStyleBackColor = false;
			// 
			// DelPoint_button
			// 
			this->DelPoint_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelPoint_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelPoint_button->Location = System::Drawing::Point(867, 155);
			this->DelPoint_button->Name = L"DelPoint_button";
			this->DelPoint_button->Size = System::Drawing::Size(61, 20);
			this->DelPoint_button->TabIndex = 168;
			this->DelPoint_button->Text = L"DelPoint/刪除";
			this->DelPoint_button->UseVisualStyleBackColor = false;
			// 
			// ShowAllPath_checkBox
			// 
			this->ShowAllPath_checkBox->AutoSize = true;
			this->ShowAllPath_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8));
			this->ShowAllPath_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ShowAllPath_checkBox->Location = System::Drawing::Point(186, 215);
			this->ShowAllPath_checkBox->Name = L"ShowAllPath_checkBox";
			this->ShowAllPath_checkBox->Size = System::Drawing::Size(148, 15);
			this->ShowAllPath_checkBox->TabIndex = 174;
			this->ShowAllPath_checkBox->Text = L"ShowAllPath/顯示所有路徑";
			this->ShowAllPath_checkBox->UseVisualStyleBackColor = true;
			// 
			// ImageAnchor_comboBox
			// 
			this->ImageAnchor_comboBox->FormattingEnabled = true;
			this->ImageAnchor_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(9) {
				L"LeftUp", L"CenterUp", L"RightUp",
					L"LeftCenter", L"CenterCenter", L"RightCenter", L"LeftDown", L"CenterDown", L"RightDown"
			});
			this->ImageAnchor_comboBox->Location = System::Drawing::Point(281, 29);
			this->ImageAnchor_comboBox->Margin = System::Windows::Forms::Padding(2);
			this->ImageAnchor_comboBox->Name = L"ImageAnchor_comboBox";
			this->ImageAnchor_comboBox->Size = System::Drawing::Size(92, 20);
			this->ImageAnchor_comboBox->TabIndex = 197;
			// 
			// AddPath_button
			// 
			this->AddPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddPath_button->Enabled = false;
			this->AddPath_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->AddPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddPath_button->Location = System::Drawing::Point(186, 13);
			this->AddPath_button->Name = L"AddPath_button";
			this->AddPath_button->Size = System::Drawing::Size(56, 20);
			this->AddPath_button->TabIndex = 164;
			this->AddPath_button->Text = L"AddPath/增加路徑";
			this->AddPath_button->UseVisualStyleBackColor = false;
			// 
			// AverageAssign_checkBox
			// 
			this->AverageAssign_checkBox->AutoSize = true;
			this->AverageAssign_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AverageAssign_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AverageAssign_checkBox->Location = System::Drawing::Point(834, 231);
			this->AverageAssign_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->AverageAssign_checkBox->Name = L"AverageAssign_checkBox";
			this->AverageAssign_checkBox->Size = System::Drawing::Size(169, 16);
			this->AverageAssign_checkBox->TabIndex = 179;
			this->AverageAssign_checkBox->Text = L"AverageAssign/自動平均給值";
			this->AverageAssign_checkBox->UseVisualStyleBackColor = false;
			// 
			// UseViewPort_checkBox
			// 
			this->UseViewPort_checkBox->AutoSize = true;
			this->UseViewPort_checkBox->Location = System::Drawing::Point(13, 14);
			this->UseViewPort_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->UseViewPort_checkBox->Name = L"UseViewPort_checkBox";
			this->UseViewPort_checkBox->Size = System::Drawing::Size(159, 16);
			this->UseViewPort_checkBox->TabIndex = 196;
			this->UseViewPort_checkBox->Text = L"UseViewPort/使用可視範圍";
			this->UseViewPort_checkBox->UseVisualStyleBackColor = true;
			// 
			// DelPath_button
			// 
			this->DelPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelPath_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->DelPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelPath_button->Location = System::Drawing::Point(186, 46);
			this->DelPath_button->Name = L"DelPath_button";
			this->DelPath_button->Size = System::Drawing::Size(75, 19);
			this->DelPath_button->TabIndex = 165;
			this->DelPath_button->Text = L"DelPath/刪除路徑";
			this->DelPath_button->UseVisualStyleBackColor = false;
			// 
			// SelectFirstAndLast_button
			// 
			this->SelectFirstAndLast_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->SelectFirstAndLast_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->SelectFirstAndLast_button->Location = System::Drawing::Point(761, 155);
			this->SelectFirstAndLast_button->Name = L"SelectFirstAndLast_button";
			this->SelectFirstAndLast_button->Size = System::Drawing::Size(102, 20);
			this->SelectFirstAndLast_button->TabIndex = 188;
			this->SelectFirstAndLast_button->Text = L"SelectFirstAndLast/選頭尾";
			this->SelectFirstAndLast_button->UseVisualStyleBackColor = false;
			// 
			// AllPI_listBox
			// 
			this->AllPI_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AllPI_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllPI_listBox->FormattingEnabled = true;
			this->AllPI_listBox->HorizontalScrollbar = true;
			this->AllPI_listBox->ItemHeight = 12;
			this->AllPI_listBox->Location = System::Drawing::Point(378, 164);
			this->AllPI_listBox->MinimumSize = System::Drawing::Size(4, 17);
			this->AllPI_listBox->Name = L"AllPI_listBox";
			this->AllPI_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AllPI_listBox->Size = System::Drawing::Size(184, 76);
			this->AllPI_listBox->TabIndex = 186;
			// 
			// Play_checkBox
			// 
			this->Play_checkBox->AutoSize = true;
			this->Play_checkBox->Enabled = false;
			this->Play_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8));
			this->Play_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Play_checkBox->Location = System::Drawing::Point(186, 177);
			this->Play_checkBox->Name = L"Play_checkBox";
			this->Play_checkBox->Size = System::Drawing::Size(117, 15);
			this->Play_checkBox->TabIndex = 177;
			this->Play_checkBox->Text = L"SinglePlay/單一撥放";
			this->Play_checkBox->UseVisualStyleBackColor = true;
			// 
			// PathNode_listBox
			// 
			this->PathNode_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNode_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNode_listBox->FormattingEnabled = true;
			this->PathNode_listBox->HorizontalScrollbar = true;
			this->PathNode_listBox->ItemHeight = 12;
			this->PathNode_listBox->Location = System::Drawing::Point(763, 18);
			this->PathNode_listBox->MinimumSize = System::Drawing::Size(4, 93);
			this->PathNode_listBox->Name = L"PathNode_listBox";
			this->PathNode_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->PathNode_listBox->Size = System::Drawing::Size(315, 136);
			this->PathNode_listBox->TabIndex = 162;
			// 
			// Size_groupBox
			// 
			this->Size_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Size_groupBox->Controls->Add(this->label6);
			this->Size_groupBox->Controls->Add(this->label21);
			this->Size_groupBox->Controls->Add(this->Width_numericUpDown);
			this->Size_groupBox->Controls->Add(this->Height_numericUpDown);
			this->Size_groupBox->Controls->Add(this->SizeScale_numericUpDown);
			this->Size_groupBox->Controls->Add(this->label7);
			this->Size_groupBox->Controls->Add(this->Mirror_checkBox);
			this->Size_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Size_groupBox->Location = System::Drawing::Point(1084, 101);
			this->Size_groupBox->Name = L"Size_groupBox";
			this->Size_groupBox->Size = System::Drawing::Size(93, 157);
			this->Size_groupBox->TabIndex = 192;
			this->Size_groupBox->TabStop = false;
			this->Size_groupBox->Text = L"Size";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label6->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label6->Location = System::Drawing::Point(7, 13);
			this->label6->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(49, 12);
			this->label6->TabIndex = 88;
			this->label6->Text = L"Width/寬";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label21->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label21->Location = System::Drawing::Point(6, 89);
			this->label21->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(136, 12);
			this->label21->TabIndex = 147;
			this->label21->Text = L"ScaleSyncly/尺寸等比縮放";
			// 
			// Width_numericUpDown
			// 
			this->Width_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Width_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Width_numericUpDown->Location = System::Drawing::Point(7, 26);
			this->Width_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->Width_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000000, 0, 0, 0 });
			this->Width_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000000, 0, 0, System::Int32::MinValue });
			this->Width_numericUpDown->Name = L"Width_numericUpDown";
			this->Width_numericUpDown->Size = System::Drawing::Size(69, 22);
			this->Width_numericUpDown->TabIndex = 6;
			// 
			// Height_numericUpDown
			// 
			this->Height_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Height_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Height_numericUpDown->Location = System::Drawing::Point(9, 64);
			this->Height_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->Height_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000000, 0, 0, 0 });
			this->Height_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1410065408, 2, 0, System::Int32::MinValue });
			this->Height_numericUpDown->Name = L"Height_numericUpDown";
			this->Height_numericUpDown->Size = System::Drawing::Size(67, 22);
			this->Height_numericUpDown->TabIndex = 7;
			// 
			// SizeScale_numericUpDown
			// 
			this->SizeScale_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->SizeScale_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->SizeScale_numericUpDown->Location = System::Drawing::Point(5, 105);
			this->SizeScale_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->SizeScale_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->SizeScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->SizeScale_numericUpDown->Name = L"SizeScale_numericUpDown";
			this->SizeScale_numericUpDown->Size = System::Drawing::Size(71, 22);
			this->SizeScale_numericUpDown->TabIndex = 8;
			this->SizeScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100, 0, 0, 0 });
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label7->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label7->Location = System::Drawing::Point(7, 50);
			this->label7->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(51, 12);
			this->label7->TabIndex = 91;
			this->label7->Text = L"Height/高";
			// 
			// Mirror_checkBox
			// 
			this->Mirror_checkBox->AutoSize = true;
			this->Mirror_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Mirror_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Mirror_checkBox->Location = System::Drawing::Point(5, 135);
			this->Mirror_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->Mirror_checkBox->Name = L"Mirror_checkBox";
			this->Mirror_checkBox->Size = System::Drawing::Size(55, 16);
			this->Mirror_checkBox->TabIndex = 9;
			this->Mirror_checkBox->Text = L"Mirror";
			this->Mirror_checkBox->UseVisualStyleBackColor = false;
			// 
			// ViewPort_xyzwNumericalControl
			// 
			this->ViewPort_xyzwNumericalControl->Location = System::Drawing::Point(7, 33);
			this->ViewPort_xyzwNumericalControl->Margin = System::Windows::Forms::Padding(4);
			this->ViewPort_xyzwNumericalControl->Name = L"ViewPort_xyzwNumericalControl";
			this->ViewPort_xyzwNumericalControl->Size = System::Drawing::Size(112, 101);
			this->ViewPort_xyzwNumericalControl->TabIndex = 157;
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(3, 16);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(164, 12);
			this->label22->TabIndex = 195;
			this->label22->Text = L"RotationPosOffset/旋轉中心調整";
			// 
			// ClonePath_button
			// 
			this->ClonePath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ClonePath_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->ClonePath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ClonePath_button->Location = System::Drawing::Point(186, 73);
			this->ClonePath_button->Name = L"ClonePath_button";
			this->ClonePath_button->Size = System::Drawing::Size(56, 20);
			this->ClonePath_button->TabIndex = 175;
			this->ClonePath_button->Text = L"ClonePath/拷貝路徑";
			this->ClonePath_button->UseVisualStyleBackColor = false;
			// 
			// Color_groupBox
			// 
			this->Color_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(74)), static_cast<System::Int32>(static_cast<System::Byte>(74)),
				static_cast<System::Int32>(static_cast<System::Byte>(74)));
			this->Color_groupBox->Controls->Add(this->RedColor_numericUpDown);
			this->Color_groupBox->Controls->Add(this->GreenColor_numericUpDown);
			this->Color_groupBox->Controls->Add(this->AlphaColor_numericUpDown);
			this->Color_groupBox->Controls->Add(this->ColorPicker_panel);
			this->Color_groupBox->Controls->Add(this->label12);
			this->Color_groupBox->Controls->Add(this->BlueColor_numericUpDown);
			this->Color_groupBox->Controls->Add(this->label11);
			this->Color_groupBox->Controls->Add(this->label13);
			this->Color_groupBox->Controls->Add(this->label10);
			this->Color_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Color_groupBox->Location = System::Drawing::Point(1188, 107);
			this->Color_groupBox->Name = L"Color_groupBox";
			this->Color_groupBox->Size = System::Drawing::Size(78, 142);
			this->Color_groupBox->TabIndex = 161;
			this->Color_groupBox->TabStop = false;
			this->Color_groupBox->Text = L"Color";
			// 
			// RedColor_numericUpDown
			// 
			this->RedColor_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->RedColor_numericUpDown->DecimalPlaces = 2;
			this->RedColor_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->RedColor_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->RedColor_numericUpDown->Location = System::Drawing::Point(21, 14);
			this->RedColor_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->RedColor_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 0 });
			this->RedColor_numericUpDown->Name = L"RedColor_numericUpDown";
			this->RedColor_numericUpDown->Size = System::Drawing::Size(43, 22);
			this->RedColor_numericUpDown->TabIndex = 10;
			this->RedColor_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// GreenColor_numericUpDown
			// 
			this->GreenColor_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GreenColor_numericUpDown->DecimalPlaces = 2;
			this->GreenColor_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GreenColor_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->GreenColor_numericUpDown->Location = System::Drawing::Point(21, 41);
			this->GreenColor_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->GreenColor_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 0 });
			this->GreenColor_numericUpDown->Name = L"GreenColor_numericUpDown";
			this->GreenColor_numericUpDown->Size = System::Drawing::Size(43, 22);
			this->GreenColor_numericUpDown->TabIndex = 11;
			this->GreenColor_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// AlphaColor_numericUpDown
			// 
			this->AlphaColor_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AlphaColor_numericUpDown->DecimalPlaces = 2;
			this->AlphaColor_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AlphaColor_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->AlphaColor_numericUpDown->Location = System::Drawing::Point(21, 89);
			this->AlphaColor_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->AlphaColor_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 0 });
			this->AlphaColor_numericUpDown->Name = L"AlphaColor_numericUpDown";
			this->AlphaColor_numericUpDown->Size = System::Drawing::Size(43, 22);
			this->AlphaColor_numericUpDown->TabIndex = 13;
			this->AlphaColor_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// ColorPicker_panel
			// 
			this->ColorPicker_panel->Location = System::Drawing::Point(2, 113);
			this->ColorPicker_panel->Name = L"ColorPicker_panel";
			this->ColorPicker_panel->Size = System::Drawing::Size(70, 22);
			this->ColorPicker_panel->TabIndex = 201;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label12->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label12->Location = System::Drawing::Point(3, 93);
			this->label12->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(13, 12);
			this->label12->TabIndex = 101;
			this->label12->Text = L"A";
			// 
			// BlueColor_numericUpDown
			// 
			this->BlueColor_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->BlueColor_numericUpDown->DecimalPlaces = 2;
			this->BlueColor_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->BlueColor_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->BlueColor_numericUpDown->Location = System::Drawing::Point(21, 67);
			this->BlueColor_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->BlueColor_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 0 });
			this->BlueColor_numericUpDown->Name = L"BlueColor_numericUpDown";
			this->BlueColor_numericUpDown->Size = System::Drawing::Size(43, 22);
			this->BlueColor_numericUpDown->TabIndex = 12;
			this->BlueColor_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label11->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label11->Location = System::Drawing::Point(2, 19);
			this->label11->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(13, 12);
			this->label11->TabIndex = 103;
			this->label11->Text = L"R";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label13->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label13->Location = System::Drawing::Point(2, 70);
			this->label13->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(13, 12);
			this->label13->TabIndex = 99;
			this->label13->Text = L"B";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label10->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label10->Location = System::Drawing::Point(2, 45);
			this->label10->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(13, 12);
			this->label10->TabIndex = 105;
			this->label10->Text = L"G";
			// 
			// RotationPosOffset_xyzNumeriaclContro
			// 
			this->RotationPosOffset_xyzNumeriaclContro->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->RotationPosOffset_xyzNumeriaclContro->Location = System::Drawing::Point(5, 31);
			this->RotationPosOffset_xyzNumeriaclContro->Margin = System::Windows::Forms::Padding(0);
			this->RotationPosOffset_xyzNumeriaclContro->Name = L"RotationPosOffset_xyzNumeriaclContro";
			this->RotationPosOffset_xyzNumeriaclContro->Size = System::Drawing::Size(60, 87);
			this->RotationPosOffset_xyzNumeriaclContro->TabIndex = 194;
			// 
			// SubMPDI_StayAtLastFrame_checkBox
			// 
			this->SubMPDI_StayAtLastFrame_checkBox->AutoSize = true;
			this->SubMPDI_StayAtLastFrame_checkBox->Enabled = false;
			this->SubMPDI_StayAtLastFrame_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8));
			this->SubMPDI_StayAtLastFrame_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->SubMPDI_StayAtLastFrame_checkBox->Location = System::Drawing::Point(186, 235);
			this->SubMPDI_StayAtLastFrame_checkBox->Name = L"SubMPDI_StayAtLastFrame_checkBox";
			this->SubMPDI_StayAtLastFrame_checkBox->Size = System::Drawing::Size(167, 15);
			this->SubMPDI_StayAtLastFrame_checkBox->TabIndex = 193;
			this->SubMPDI_StayAtLastFrame_checkBox->Text = L"StayAtLastFrame/停在最後一格";
			this->SubMPDI_StayAtLastFrame_checkBox->UseVisualStyleBackColor = true;
			// 
			// InvertPath_button
			// 
			this->InvertPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->InvertPath_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->InvertPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->InvertPath_button->Location = System::Drawing::Point(186, 101);
			this->InvertPath_button->Name = L"InvertPath_button";
			this->InvertPath_button->Size = System::Drawing::Size(75, 19);
			this->InvertPath_button->TabIndex = 185;
			this->InvertPath_button->Text = L"InvertPath/顛倒路勁";
			this->InvertPath_button->UseVisualStyleBackColor = false;
			// 
			// Position_groupBox
			// 
			this->Position_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Position_groupBox->Controls->Add(this->PathNodePosX_numericUpDown);
			this->Position_groupBox->Controls->Add(this->PathNodePosY_numericUpDown);
			this->Position_groupBox->Controls->Add(this->PathNodePosZ_numericUpDown);
			this->Position_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Position_groupBox->Location = System::Drawing::Point(1084, 3);
			this->Position_groupBox->Name = L"Position_groupBox";
			this->Position_groupBox->Size = System::Drawing::Size(86, 97);
			this->Position_groupBox->TabIndex = 159;
			this->Position_groupBox->TabStop = false;
			this->Position_groupBox->Text = L"Position";
			// 
			// PathNodePosX_numericUpDown
			// 
			this->PathNodePosX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNodePosX_numericUpDown->DecimalPlaces = 1;
			this->PathNodePosX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNodePosX_numericUpDown->Location = System::Drawing::Point(7, 15);
			this->PathNodePosX_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->PathNodePosX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->PathNodePosX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->PathNodePosX_numericUpDown->Name = L"PathNodePosX_numericUpDown";
			this->PathNodePosX_numericUpDown->Size = System::Drawing::Size(71, 22);
			this->PathNodePosX_numericUpDown->TabIndex = 0;
			// 
			// PathNodePosY_numericUpDown
			// 
			this->PathNodePosY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNodePosY_numericUpDown->DecimalPlaces = 1;
			this->PathNodePosY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNodePosY_numericUpDown->Location = System::Drawing::Point(7, 43);
			this->PathNodePosY_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->PathNodePosY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->PathNodePosY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->PathNodePosY_numericUpDown->Name = L"PathNodePosY_numericUpDown";
			this->PathNodePosY_numericUpDown->Size = System::Drawing::Size(71, 22);
			this->PathNodePosY_numericUpDown->TabIndex = 1;
			// 
			// PathNodePosZ_numericUpDown
			// 
			this->PathNodePosZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNodePosZ_numericUpDown->DecimalPlaces = 1;
			this->PathNodePosZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNodePosZ_numericUpDown->Location = System::Drawing::Point(6, 69);
			this->PathNodePosZ_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->PathNodePosZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->PathNodePosZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->PathNodePosZ_numericUpDown->Name = L"PathNodePosZ_numericUpDown";
			this->PathNodePosZ_numericUpDown->Size = System::Drawing::Size(71, 22);
			this->PathNodePosZ_numericUpDown->TabIndex = 2;
			// 
			// PointDataUtility_contextMenuStrip
			// 
			this->PointDataUtility_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->FadeIn_toolStripMenuItem,
					this->FadeOut_toolStripMenuItem
			});
			this->PointDataUtility_contextMenuStrip->Name = L"PointDataUtility_contextMenuStrip";
			this->PointDataUtility_contextMenuStrip->Size = System::Drawing::Size(110, 48);
			// 
			// FadeIn_toolStripMenuItem
			// 
			this->FadeIn_toolStripMenuItem->Name = L"FadeIn_toolStripMenuItem";
			this->FadeIn_toolStripMenuItem->Size = System::Drawing::Size(109, 22);
			this->FadeIn_toolStripMenuItem->Text = L"FadeIn";
			// 
			// FadeOut_toolStripMenuItem
			// 
			this->FadeOut_toolStripMenuItem->Name = L"FadeOut_toolStripMenuItem";
			this->FadeOut_toolStripMenuItem->Size = System::Drawing::Size(109, 22);
			this->FadeOut_toolStripMenuItem->Text = L"FadeOut";
			// 
			// PathGroup_contextMenuStrip
			// 
			this->PathGroup_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->toolStripMenuItem1,
					this->addPointDataWithImageByPathNameToolStripMenuItem
			});
			this->PathGroup_contextMenuStrip->Name = L"PathGroup_contextMenuStrip";
			this->PathGroup_contextMenuStrip->Size = System::Drawing::Size(249, 48);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(248, 22);
			this->toolStripMenuItem1->Text = L"Add Group Path";
			// 
			// addPointDataWithImageByPathNameToolStripMenuItem
			// 
			this->addPointDataWithImageByPathNameToolStripMenuItem->Name = L"addPointDataWithImageByPathNameToolStripMenuItem";
			this->addPointDataWithImageByPathNameToolStripMenuItem->Size = System::Drawing::Size(248, 22);
			this->addPointDataWithImageByPathNameToolStripMenuItem->Text = L"AddPointDataWithImageByPathName";
			// 
			// GameUIData_contextMenuStrip
			// 
			this->GameUIData_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->AddSliderBarUI_toolStripMenuItem,
					this->AddMultiPageUI_toolStripMenuItem, this->addSliderBarWithTabPagesUIToolStripMenuItem, this->addMultipageWithTabPagesUIToolStripMenuItem,
					this->yesNoDialogToolStripMenuItem
			});
			this->GameUIData_contextMenuStrip->Name = L"GameUIData_contextMenuStrip";
			this->GameUIData_contextMenuStrip->Size = System::Drawing::Size(229, 114);
			// 
			// AddSliderBarUI_toolStripMenuItem
			// 
			this->AddSliderBarUI_toolStripMenuItem->Name = L"AddSliderBarUI_toolStripMenuItem";
			this->AddSliderBarUI_toolStripMenuItem->Size = System::Drawing::Size(228, 22);
			this->AddSliderBarUI_toolStripMenuItem->Text = L"Add Slider Bar UI";
			// 
			// AddMultiPageUI_toolStripMenuItem
			// 
			this->AddMultiPageUI_toolStripMenuItem->Name = L"AddMultiPageUI_toolStripMenuItem";
			this->AddMultiPageUI_toolStripMenuItem->Size = System::Drawing::Size(228, 22);
			this->AddMultiPageUI_toolStripMenuItem->Text = L"Add Multi Page UI";
			// 
			// addSliderBarWithTabPagesUIToolStripMenuItem
			// 
			this->addSliderBarWithTabPagesUIToolStripMenuItem->Name = L"addSliderBarWithTabPagesUIToolStripMenuItem";
			this->addSliderBarWithTabPagesUIToolStripMenuItem->Size = System::Drawing::Size(228, 22);
			this->addSliderBarWithTabPagesUIToolStripMenuItem->Text = L"Add Slider Bar With TabPages UI";
			// 
			// addMultipageWithTabPagesUIToolStripMenuItem
			// 
			this->addMultipageWithTabPagesUIToolStripMenuItem->Name = L"addMultipageWithTabPagesUIToolStripMenuItem";
			this->addMultipageWithTabPagesUIToolStripMenuItem->Size = System::Drawing::Size(228, 22);
			this->addMultipageWithTabPagesUIToolStripMenuItem->Text = L"Add Multipage With TabPages UI";
			// 
			// yesNoDialogToolStripMenuItem
			// 
			this->yesNoDialogToolStripMenuItem->Name = L"yesNoDialogToolStripMenuItem";
			this->yesNoDialogToolStripMenuItem->Size = System::Drawing::Size(228, 22);
			this->yesNoDialogToolStripMenuItem->Text = L"YesNoDialog";
			// 
			// tabControl
			// 
			this->tabControl->Controls->Add(this->ViewPort_tabPage);
			this->tabControl->Controls->Add(this->ColorBlending);
			this->tabControl->Controls->Add(this->RotationOffset);
			this->tabControl->Location = System::Drawing::Point(1275, 5);
			this->tabControl->Multiline = true;
			this->tabControl->Name = L"tabControl";
			this->tabControl->SelectedIndex = 0;
			this->tabControl->Size = System::Drawing::Size(179, 261);
			this->tabControl->TabIndex = 200;
			// 
			// ViewPort_tabPage
			// 
			this->ViewPort_tabPage->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ViewPort_tabPage->Controls->Add(this->UseViewPort_checkBox);
			this->ViewPort_tabPage->Controls->Add(this->ViewPort_xyzwNumericalControl);
			this->ViewPort_tabPage->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ViewPort_tabPage->Location = System::Drawing::Point(4, 40);
			this->ViewPort_tabPage->Name = L"ViewPort_tabPage";
			this->ViewPort_tabPage->Padding = System::Windows::Forms::Padding(3);
			this->ViewPort_tabPage->Size = System::Drawing::Size(171, 217);
			this->ViewPort_tabPage->TabIndex = 0;
			this->ViewPort_tabPage->Text = L"ViewPort";
			this->ViewPort_tabPage->UseVisualStyleBackColor = true;
			// 
			// ColorBlending
			// 
			this->ColorBlending->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ColorBlending->Controls->Add(this->ColorBlending_groupBox);
			this->ColorBlending->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ColorBlending->Location = System::Drawing::Point(4, 40);
			this->ColorBlending->Name = L"ColorBlending";
			this->ColorBlending->Padding = System::Windows::Forms::Padding(3);
			this->ColorBlending->Size = System::Drawing::Size(171, 217);
			this->ColorBlending->TabIndex = 1;
			this->ColorBlending->Text = L"ColorBlending";
			this->ColorBlending->UseVisualStyleBackColor = true;
			// 
			// RotationOffset
			// 
			this->RotationOffset->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->RotationOffset->Controls->Add(this->label22);
			this->RotationOffset->Controls->Add(this->RotationPosOffset_xyzNumeriaclContro);
			this->RotationOffset->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->RotationOffset->Location = System::Drawing::Point(4, 40);
			this->RotationOffset->Name = L"RotationOffset";
			this->RotationOffset->Size = System::Drawing::Size(171, 217);
			this->RotationOffset->TabIndex = 2;
			this->RotationOffset->Text = L"RotationOffset";
			this->RotationOffset->UseVisualStyleBackColor = true;
			// 
			// MPDIUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Controls->Add(this->WholePath_listBox);
			this->Controls->Add(this->PathPiorityDown_button);
			this->Controls->Add(this->ApplyLODToPoints_button);
			this->Controls->Add(this->SelectEvenPoints_button);
			this->Controls->Add(this->AssignImage_button);
			this->Controls->Add(this->PathPiorityUp_button);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->WholePuzzleImageUnit_listBox);
			this->Controls->Add(this->NumPointsToInsert_numericUpDown);
			this->Controls->Add(this->InvertAllPath_button);
			this->Controls->Add(this->InsertPoint_button);
			this->Controls->Add(this->AddPath_button);
			this->Controls->Add(this->Loop_checkBox);
			this->Controls->Add(this->DelPath_button);
			this->Controls->Add(this->ClonePath_button);
			this->Controls->Add(this->Rotation_groupBox);
			this->Controls->Add(this->SelectAllPoints_button);
			this->Controls->Add(this->tabControl);
			this->Controls->Add(this->DelPoint_button);
			this->Controls->Add(this->ErrorMsg_label);
			this->Controls->Add(this->SelectFirstAndLast_button);
			this->Controls->Add(this->InvertPath_button);
			this->Controls->Add(this->PathNode_listBox);
			this->Controls->Add(this->AllPI_listBox);
			this->Controls->Add(this->ShowAllPath_checkBox);
			this->Controls->Add(this->SubMPDI_StayAtLastFrame_checkBox);
			this->Controls->Add(this->label25);
			this->Controls->Add(this->Position_groupBox);
			this->Controls->Add(this->Size_groupBox);
			this->Controls->Add(this->Color_groupBox);
			this->Controls->Add(this->Play_checkBox);
			this->Controls->Add(this->AverageAssign_checkBox);
			this->Controls->Add(this->BehaviorUpdateActive_checkBox);
			this->Controls->Add(this->DelPI_button);
			this->Controls->Add(this->ImageAnchor_comboBox);
			this->Controls->Add(this->ShowSelectedImageBorder_checkBox);
			this->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Name = L"MPDIUI";
			this->Size = System::Drawing::Size(1484, 267);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TotalPlayTime_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->RearrangeWholePathTime_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PathNodeTime_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->StartTime_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PathLOD_numericUpDown))->EndInit();
			this->Rotation_groupBox->ResumeLayout(false);
			this->Rotation_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->AngleY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->AngleX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->AngleZ_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->NumPointsToInsert_numericUpDown))->EndInit();
			this->ColorBlending_groupBox->ResumeLayout(false);
			this->ColorBlending_groupBox->PerformLayout();
			this->Size_groupBox->ResumeLayout(false);
			this->Size_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Width_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Height_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SizeScale_numericUpDown))->EndInit();
			this->Color_groupBox->ResumeLayout(false);
			this->Color_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->RedColor_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GreenColor_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->AlphaColor_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BlueColor_numericUpDown))->EndInit();
			this->Position_groupBox->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PathNodePosX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PathNodePosY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PathNodePosZ_numericUpDown))->EndInit();
			this->PointDataUtility_contextMenuStrip->ResumeLayout(false);
			this->PathGroup_contextMenuStrip->ResumeLayout(false);
			this->GameUIData_contextMenuStrip->ResumeLayout(false);
			this->tabControl->ResumeLayout(false);
			this->ViewPort_tabPage->ResumeLayout(false);
			this->ViewPort_tabPage->PerformLayout();
			this->ColorBlending->ResumeLayout(false);
			this->RotationOffset->ResumeLayout(false);
			this->RotationOffset->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
public: System::Void PathNode_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
		{
			 if(e->Control)
			 {
				 if( e->KeyCode == GCFORM::Keys::P )
				 {
					 PathNodePosX_numericUpDown->Focus();
				 }
				 else
				 if( e->KeyCode == GCFORM::Keys::S )
				 {
					 Width_numericUpDown->Focus();
				 }
				 else
				 if( e->KeyCode == GCFORM::Keys::C )
				 {
					 RedColor_numericUpDown->Focus();
				 }
				 else
				 if( e->KeyCode == GCFORM::Keys::R )
				 {
					AngleZ_numericUpDown->Focus();
				 }
				 else
				 if( e->KeyCode == GCFORM::Keys::D1 )
				 {
					 WholePath_listBox->Focus();
				 }
				 else
				 if( e->KeyCode == GCFORM::Keys::D2 )
					WholePuzzleImageUnit_listBox->Focus();
				 else
				 if( e->KeyCode == GCFORM::Keys::D3 )
				 {
					 PathNode_listBox->Focus();
				 }
			 }
		}
};
}
