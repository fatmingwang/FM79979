#pragma once

#include "../../Core/Terrain/ShockBoobsPainter.h"
#include "../../Core/FatmingCollada/AllColladaInclude.h"
cMesh*g_pMesh = 0;
cColladaParser*g_pColladaParser = 0;
extern cShockBoobsShader*g_pShockBoobsShader;
namespace EditorSample 
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
			NamedTypedObject*l_pNamedTypedObject = new NamedTypedObject();
			delete l_pNamedTypedObject;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pTargetControl = Main_splitContainer->Panel2;
			m_pOrthogonalCamera = 0;
			m_pFrameCamera = 0;
			m_pCameraBehaveByMouseBehave = 0;
			ColorType_comboBox->SelectedIndex = 0;
			TerianViewType_comboBox->SelectedIndex = 0;
			m_pMyBoobsPainter = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			this->timer1->Enabled = false;
			if (components)
			{
				delete components;
			}
			if( m_pMyBoobsPainter )
				delete m_pMyBoobsPainter;
			SAFE_DELETE(m_pOrthogonalCamera);
			SAFE_DELETE(m_pFrameCamera);
			SAFE_DELETE(m_pCameraBehaveByMouseBehave);
		}
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Timer^  timer1;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		//my
		cGameApp*						m_pGameApp;
		HDC								m_HdcMV;
		HGLRC							m_HGLRCMV;
		Control^						m_pTargetControl;
		cOrthogonalCamera*				m_pOrthogonalCamera;
		cFrameCamera*					m_pFrameCamera;
		cCameraBehaveByMouseBehave*		m_pCameraBehaveByMouseBehave;
		cShockBoobsPainter*				m_pMyBoobsPainter;

	private: System::Windows::Forms::SplitContainer^  Main_splitContainer;
	private: System::Windows::Forms::Label^  CircleRadius_label;
	private: System::Windows::Forms::NumericUpDown^  CircleRadius_numericUpDown;
	private: System::Windows::Forms::Button^  Clean_button;
	private: System::Windows::Forms::Button^  CameraRest_button;

	private: System::Windows::Forms::NumericUpDown^  TerrianHeight_numericUpDown;
	private: System::Windows::Forms::Label^  TerrianHeight_label;
	private: System::Windows::Forms::GroupBox^  Terrian_groupBox;
	private: System::Windows::Forms::GroupBox^  Shocking_groupBox;
	private: System::Windows::Forms::Label^  ShockSpeedValue_label;
	private: System::Windows::Forms::NumericUpDown^  ShockSpeed_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ShockingSpeedMinus_numericUpDown;



	private: System::Windows::Forms::Label^  ShockSpeedMinus_label;

	private: System::Windows::Forms::Button^  Shock_button;
	private: System::Windows::Forms::CheckBox^  CameraSwitch_checkBox;
	private: System::Windows::Forms::CheckBox^  ShockLoop_checkBox;
	private: System::Windows::Forms::Label^  ShockStrgethValue_label;
	private: System::Windows::Forms::NumericUpDown^  ShockStregthValue_numericUpDown;
	private: System::Windows::Forms::Label^  ColorType_label;
	private: System::Windows::Forms::ComboBox^  ColorType_comboBox;
	private: System::Windows::Forms::ComboBox^  TerianViewType_comboBox;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::Label^  MaximumShockStregth_label;
	private: System::Windows::Forms::NumericUpDown^  MaximumShockStregth_numericUpDown;
	private: System::Windows::Forms::CheckBox^  MyPainterMode_checkBox;
	private: System::Windows::Forms::Label^  ViewMode_label;
	private: System::Windows::Forms::ToolStripMenuItem^  patingImageToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;

#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Required method for Designer support - do not modify
			 /// the contents of this method with the code editor.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->components = (gcnew System::ComponentModel::Container());
				 this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
				 this->Main_splitContainer = (gcnew System::Windows::Forms::SplitContainer());
				 this->MyPainterMode_checkBox = (gcnew System::Windows::Forms::CheckBox());
				 this->ViewMode_label = (gcnew System::Windows::Forms::Label());
				 this->TerianViewType_comboBox = (gcnew System::Windows::Forms::ComboBox());
				 this->ColorType_label = (gcnew System::Windows::Forms::Label());
				 this->ColorType_comboBox = (gcnew System::Windows::Forms::ComboBox());
				 this->ShockLoop_checkBox = (gcnew System::Windows::Forms::CheckBox());
				 this->CameraSwitch_checkBox = (gcnew System::Windows::Forms::CheckBox());
				 this->Shock_button = (gcnew System::Windows::Forms::Button());
				 this->Shocking_groupBox = (gcnew System::Windows::Forms::GroupBox());
				 this->MaximumShockStregth_label = (gcnew System::Windows::Forms::Label());
				 this->ShockStrgethValue_label = (gcnew System::Windows::Forms::Label());
				 this->ShockSpeedMinus_label = (gcnew System::Windows::Forms::Label());
				 this->MaximumShockStregth_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
				 this->ShockSpeedValue_label = (gcnew System::Windows::Forms::Label());
				 this->ShockStregthValue_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
				 this->ShockSpeed_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
				 this->ShockingSpeedMinus_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
				 this->Terrian_groupBox = (gcnew System::Windows::Forms::GroupBox());
				 this->TerrianHeight_label = (gcnew System::Windows::Forms::Label());
				 this->TerrianHeight_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
				 this->CircleRadius_label = (gcnew System::Windows::Forms::Label());
				 this->CircleRadius_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
				 this->CameraRest_button = (gcnew System::Windows::Forms::Button());
				 this->Clean_button = (gcnew System::Windows::Forms::Button());
				 this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
				 this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->patingImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->Main_splitContainer->Panel1->SuspendLayout();
				 this->Main_splitContainer->SuspendLayout();
				 this->Shocking_groupBox->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MaximumShockStregth_numericUpDown))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShockStregthValue_numericUpDown))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShockSpeed_numericUpDown))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShockingSpeedMinus_numericUpDown))->BeginInit();
				 this->Terrian_groupBox->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TerrianHeight_numericUpDown))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CircleRadius_numericUpDown))->BeginInit();
				 this->menuStrip1->SuspendLayout();
				 this->SuspendLayout();
				 // 
				 // timer1
				 // 
				 this->timer1->Interval = 1;
				 this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
				 // 
				 // Main_splitContainer
				 // 
				 this->Main_splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->Main_splitContainer->Location = System::Drawing::Point(0, 24);
				 this->Main_splitContainer->Name = L"Main_splitContainer";
				 // 
				 // Main_splitContainer.Panel1
				 // 
				 this->Main_splitContainer->Panel1->Controls->Add(this->MyPainterMode_checkBox);
				 this->Main_splitContainer->Panel1->Controls->Add(this->ViewMode_label);
				 this->Main_splitContainer->Panel1->Controls->Add(this->TerianViewType_comboBox);
				 this->Main_splitContainer->Panel1->Controls->Add(this->ColorType_label);
				 this->Main_splitContainer->Panel1->Controls->Add(this->ColorType_comboBox);
				 this->Main_splitContainer->Panel1->Controls->Add(this->ShockLoop_checkBox);
				 this->Main_splitContainer->Panel1->Controls->Add(this->CameraSwitch_checkBox);
				 this->Main_splitContainer->Panel1->Controls->Add(this->Shock_button);
				 this->Main_splitContainer->Panel1->Controls->Add(this->Shocking_groupBox);
				 this->Main_splitContainer->Panel1->Controls->Add(this->Terrian_groupBox);
				 this->Main_splitContainer->Panel1->Controls->Add(this->CameraRest_button);
				 this->Main_splitContainer->Panel1->Controls->Add(this->Clean_button);
				 this->Main_splitContainer->Size = System::Drawing::Size(916, 528);
				 this->Main_splitContainer->SplitterDistance = 305;
				 this->Main_splitContainer->TabIndex = 0;
				 // 
				 // MyPainterMode_checkBox
				 // 
				 this->MyPainterMode_checkBox->AutoSize = true;
				 this->MyPainterMode_checkBox->Location = System::Drawing::Point(93, 457);
				 this->MyPainterMode_checkBox->Name = L"MyPainterMode_checkBox";
				 this->MyPainterMode_checkBox->Size = System::Drawing::Size(77, 16);
				 this->MyPainterMode_checkBox->TabIndex = 17;
				 this->MyPainterMode_checkBox->Text = L"Paint Mode";
				 this->MyPainterMode_checkBox->UseVisualStyleBackColor = true;
				 this->MyPainterMode_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::MyPainterMode_checkBox_CheckedChanged);
				 // 
				 // ViewMode_label
				 // 
				 this->ViewMode_label->AutoSize = true;
				 this->ViewMode_label->Location = System::Drawing::Point(12, 481);
				 this->ViewMode_label->Name = L"ViewMode_label";
				 this->ViewMode_label->Size = System::Drawing::Size(56, 12);
				 this->ViewMode_label->TabIndex = 16;
				 this->ViewMode_label->Text = L"ViewMode";
				 // 
				 // TerianViewType_comboBox
				 // 
				 this->TerianViewType_comboBox->FormattingEnabled = true;
				 this->TerianViewType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Normal", L"Debug", L"Both"});
				 this->TerianViewType_comboBox->Location = System::Drawing::Point(14, 496);
				 this->TerianViewType_comboBox->Name = L"TerianViewType_comboBox";
				 this->TerianViewType_comboBox->Size = System::Drawing::Size(121, 20);
				 this->TerianViewType_comboBox->TabIndex = 15;
				 // 
				 // ColorType_label
				 // 
				 this->ColorType_label->AutoSize = true;
				 this->ColorType_label->Location = System::Drawing::Point(11, 293);
				 this->ColorType_label->Name = L"ColorType_label";
				 this->ColorType_label->Size = System::Drawing::Size(56, 12);
				 this->ColorType_label->TabIndex = 14;
				 this->ColorType_label->Text = L"ColorType";
				 // 
				 // ColorType_comboBox
				 // 
				 this->ColorType_comboBox->FormattingEnabled = true;
				 this->ColorType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Add", L"Del", L"Remove"});
				 this->ColorType_comboBox->Location = System::Drawing::Point(73, 290);
				 this->ColorType_comboBox->Name = L"ColorType_comboBox";
				 this->ColorType_comboBox->Size = System::Drawing::Size(91, 20);
				 this->ColorType_comboBox->TabIndex = 13;
				 this->ColorType_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::ColorType_comboBox_SelectedIndexChanged);
				 // 
				 // ShockLoop_checkBox
				 // 
				 this->ShockLoop_checkBox->AutoSize = true;
				 this->ShockLoop_checkBox->Location = System::Drawing::Point(4, 316);
				 this->ShockLoop_checkBox->Name = L"ShockLoop_checkBox";
				 this->ShockLoop_checkBox->Size = System::Drawing::Size(78, 16);
				 this->ShockLoop_checkBox->TabIndex = 12;
				 this->ShockLoop_checkBox->Text = L"ShockLoop";
				 this->ShockLoop_checkBox->UseVisualStyleBackColor = true;
				 // 
				 // CameraSwitch_checkBox
				 // 
				 this->CameraSwitch_checkBox->AutoSize = true;
				 this->CameraSwitch_checkBox->Location = System::Drawing::Point(12, 457);
				 this->CameraSwitch_checkBox->Name = L"CameraSwitch_checkBox";
				 this->CameraSwitch_checkBox->Size = System::Drawing::Size(38, 16);
				 this->CameraSwitch_checkBox->TabIndex = 11;
				 this->CameraSwitch_checkBox->Text = L"2D";
				 this->CameraSwitch_checkBox->UseVisualStyleBackColor = true;
				 this->CameraSwitch_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::CameraSwitch_checkBox_CheckedChanged);
				 // 
				 // Shock_button
				 // 
				 this->Shock_button->Location = System::Drawing::Point(3, 340);
				 this->Shock_button->Name = L"Shock_button";
				 this->Shock_button->Size = System::Drawing::Size(113, 35);
				 this->Shock_button->TabIndex = 10;
				 this->Shock_button->Text = L"Shock";
				 this->Shock_button->UseVisualStyleBackColor = true;
				 this->Shock_button->Click += gcnew System::EventHandler(this, &Form1::Shock_button_Click);
				 // 
				 // Shocking_groupBox
				 // 
				 this->Shocking_groupBox->Controls->Add(this->MaximumShockStregth_label);
				 this->Shocking_groupBox->Controls->Add(this->ShockStrgethValue_label);
				 this->Shocking_groupBox->Controls->Add(this->ShockSpeedMinus_label);
				 this->Shocking_groupBox->Controls->Add(this->MaximumShockStregth_numericUpDown);
				 this->Shocking_groupBox->Controls->Add(this->ShockSpeedValue_label);
				 this->Shocking_groupBox->Controls->Add(this->ShockStregthValue_numericUpDown);
				 this->Shocking_groupBox->Controls->Add(this->ShockSpeed_numericUpDown);
				 this->Shocking_groupBox->Controls->Add(this->ShockingSpeedMinus_numericUpDown);
				 this->Shocking_groupBox->Location = System::Drawing::Point(14, 98);
				 this->Shocking_groupBox->Name = L"Shocking_groupBox";
				 this->Shocking_groupBox->Size = System::Drawing::Size(164, 186);
				 this->Shocking_groupBox->TabIndex = 9;
				 this->Shocking_groupBox->TabStop = false;
				 this->Shocking_groupBox->Text = L"                    ShockParameter";
				 // 
				 // MaximumShockStregth_label
				 // 
				 this->MaximumShockStregth_label->AutoSize = true;
				 this->MaximumShockStregth_label->Location = System::Drawing::Point(2, 137);
				 this->MaximumShockStregth_label->Name = L"MaximumShockStregth_label";
				 this->MaximumShockStregth_label->Size = System::Drawing::Size(115, 12);
				 this->MaximumShockStregth_label->TabIndex = 10;
				 this->MaximumShockStregth_label->Text = L"MaximumShockStregth";
				 // 
				 // ShockStrgethValue_label
				 // 
				 this->ShockStrgethValue_label->AutoSize = true;
				 this->ShockStrgethValue_label->Location = System::Drawing::Point(3, 94);
				 this->ShockStrgethValue_label->Name = L"ShockStrgethValue_label";
				 this->ShockStrgethValue_label->Size = System::Drawing::Size(94, 12);
				 this->ShockStrgethValue_label->TabIndex = 8;
				 this->ShockStrgethValue_label->Text = L"ShockStrgethValue";
				 // 
				 // ShockSpeedMinus_label
				 // 
				 this->ShockSpeedMinus_label->AutoSize = true;
				 this->ShockSpeedMinus_label->Location = System::Drawing::Point(4, 49);
				 this->ShockSpeedMinus_label->Name = L"ShockSpeedMinus_label";
				 this->ShockSpeedMinus_label->Size = System::Drawing::Size(91, 12);
				 this->ShockSpeedMinus_label->TabIndex = 1;
				 this->ShockSpeedMinus_label->Text = L"ShockSpeedMinus";
				 // 
				 // MaximumShockStregth_numericUpDown
				 // 
				 this->MaximumShockStregth_numericUpDown->DecimalPlaces = 5;
				 this->MaximumShockStregth_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
				 this->MaximumShockStregth_numericUpDown->Location = System::Drawing::Point(3, 153);
				 this->MaximumShockStregth_numericUpDown->Name = L"MaximumShockStregth_numericUpDown";
				 this->MaximumShockStregth_numericUpDown->Size = System::Drawing::Size(99, 22);
				 this->MaximumShockStregth_numericUpDown->TabIndex = 9;
				 this->MaximumShockStregth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
				 this->MaximumShockStregth_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::Shock_button_Click);
				 // 
				 // ShockSpeedValue_label
				 // 
				 this->ShockSpeedValue_label->AutoSize = true;
				 this->ShockSpeedValue_label->Location = System::Drawing::Point(4, 13);
				 this->ShockSpeedValue_label->Name = L"ShockSpeedValue_label";
				 this->ShockSpeedValue_label->Size = System::Drawing::Size(62, 12);
				 this->ShockSpeedValue_label->TabIndex = 8;
				 this->ShockSpeedValue_label->Text = L"ShockSpeed";
				 // 
				 // ShockStregthValue_numericUpDown
				 // 
				 this->ShockStregthValue_numericUpDown->DecimalPlaces = 5;
				 this->ShockStregthValue_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
				 this->ShockStregthValue_numericUpDown->Location = System::Drawing::Point(3, 113);
				 this->ShockStregthValue_numericUpDown->Name = L"ShockStregthValue_numericUpDown";
				 this->ShockStregthValue_numericUpDown->Size = System::Drawing::Size(99, 22);
				 this->ShockStregthValue_numericUpDown->TabIndex = 7;
				 this->ShockStregthValue_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->ShockStregthValue_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::Shock_button_Click);
				 // 
				 // ShockSpeed_numericUpDown
				 // 
				 this->ShockSpeed_numericUpDown->DecimalPlaces = 5;
				 this->ShockSpeed_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
				 this->ShockSpeed_numericUpDown->Location = System::Drawing::Point(3, 26);
				 this->ShockSpeed_numericUpDown->Name = L"ShockSpeed_numericUpDown";
				 this->ShockSpeed_numericUpDown->Size = System::Drawing::Size(99, 22);
				 this->ShockSpeed_numericUpDown->TabIndex = 7;
				 this->ShockSpeed_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->ShockSpeed_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::Shock_button_Click);
				 // 
				 // ShockingSpeedMinus_numericUpDown
				 // 
				 this->ShockingSpeedMinus_numericUpDown->DecimalPlaces = 5;
				 this->ShockingSpeedMinus_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
				 this->ShockingSpeedMinus_numericUpDown->Location = System::Drawing::Point(4, 67);
				 this->ShockingSpeedMinus_numericUpDown->Name = L"ShockingSpeedMinus_numericUpDown";
				 this->ShockingSpeedMinus_numericUpDown->Size = System::Drawing::Size(99, 22);
				 this->ShockingSpeedMinus_numericUpDown->TabIndex = 0;
				 this->ShockingSpeedMinus_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
				 this->ShockingSpeedMinus_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::Shock_button_Click);
				 // 
				 // Terrian_groupBox
				 // 
				 this->Terrian_groupBox->Controls->Add(this->TerrianHeight_label);
				 this->Terrian_groupBox->Controls->Add(this->TerrianHeight_numericUpDown);
				 this->Terrian_groupBox->Controls->Add(this->CircleRadius_label);
				 this->Terrian_groupBox->Controls->Add(this->CircleRadius_numericUpDown);
				 this->Terrian_groupBox->Location = System::Drawing::Point(12, 12);
				 this->Terrian_groupBox->Name = L"Terrian_groupBox";
				 this->Terrian_groupBox->Size = System::Drawing::Size(217, 80);
				 this->Terrian_groupBox->TabIndex = 0;
				 this->Terrian_groupBox->TabStop = false;
				 this->Terrian_groupBox->Text = L"Terrian";
				 // 
				 // TerrianHeight_label
				 // 
				 this->TerrianHeight_label->AutoSize = true;
				 this->TerrianHeight_label->Location = System::Drawing::Point(3, 18);
				 this->TerrianHeight_label->Name = L"TerrianHeight_label";
				 this->TerrianHeight_label->Size = System::Drawing::Size(70, 12);
				 this->TerrianHeight_label->TabIndex = 8;
				 this->TerrianHeight_label->Text = L"TerrianHeight";
				 // 
				 // TerrianHeight_numericUpDown
				 // 
				 this->TerrianHeight_numericUpDown->Location = System::Drawing::Point(74, 14);
				 this->TerrianHeight_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {51, 0, 0, 0});
				 this->TerrianHeight_numericUpDown->Name = L"TerrianHeight_numericUpDown";
				 this->TerrianHeight_numericUpDown->Size = System::Drawing::Size(92, 22);
				 this->TerrianHeight_numericUpDown->TabIndex = 7;
				 this->TerrianHeight_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::Alpha_numericUpDown_ValueChanged);
				 // 
				 // CircleRadius_label
				 // 
				 this->CircleRadius_label->AutoSize = true;
				 this->CircleRadius_label->Location = System::Drawing::Point(4, 49);
				 this->CircleRadius_label->Name = L"CircleRadius_label";
				 this->CircleRadius_label->Size = System::Drawing::Size(65, 12);
				 this->CircleRadius_label->TabIndex = 3;
				 this->CircleRadius_label->Text = L"EffectRadius";
				 // 
				 // CircleRadius_numericUpDown
				 // 
				 this->CircleRadius_numericUpDown->Location = System::Drawing::Point(76, 43);
				 this->CircleRadius_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2048, 0, 0, 0});
				 this->CircleRadius_numericUpDown->Name = L"CircleRadius_numericUpDown";
				 this->CircleRadius_numericUpDown->Size = System::Drawing::Size(90, 22);
				 this->CircleRadius_numericUpDown->TabIndex = 2;
				 this->CircleRadius_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
				 this->CircleRadius_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::CircleRadius_numericUpDown_ValueChanged);
				 // 
				 // CameraRest_button
				 // 
				 this->CameraRest_button->Location = System::Drawing::Point(3, 416);
				 this->CameraRest_button->Name = L"CameraRest_button";
				 this->CameraRest_button->Size = System::Drawing::Size(76, 35);
				 this->CameraRest_button->TabIndex = 6;
				 this->CameraRest_button->Text = L"CameraRest";
				 this->CameraRest_button->UseVisualStyleBackColor = true;
				 this->CameraRest_button->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
				 // 
				 // Clean_button
				 // 
				 this->Clean_button->Location = System::Drawing::Point(3, 381);
				 this->Clean_button->Name = L"Clean_button";
				 this->Clean_button->Size = System::Drawing::Size(113, 29);
				 this->Clean_button->TabIndex = 5;
				 this->Clean_button->Text = L"Clean";
				 this->Clean_button->UseVisualStyleBackColor = true;
				 this->Clean_button->Click += gcnew System::EventHandler(this, &Form1::Clean_button_Click);
				 // 
				 // menuStrip1
				 // 
				 this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
					 this->patingImageToolStripMenuItem});
				 this->menuStrip1->Location = System::Drawing::Point(0, 0);
				 this->menuStrip1->Name = L"menuStrip1";
				 this->menuStrip1->Size = System::Drawing::Size(916, 24);
				 this->menuStrip1->TabIndex = 1;
				 this->menuStrip1->Text = L"menuStrip1";
				 // 
				 // fileToolStripMenuItem
				 // 
				 this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
					 this->saveToolStripMenuItem});
				 this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
				 this->fileToolStripMenuItem->Size = System::Drawing::Size(39, 20);
				 this->fileToolStripMenuItem->Text = L"File";
				 // 
				 // openToolStripMenuItem
				 // 
				 this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
				 this->openToolStripMenuItem->Size = System::Drawing::Size(108, 22);
				 this->openToolStripMenuItem->Text = L"Open";
				 this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
				 // 
				 // saveToolStripMenuItem
				 // 
				 this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
				 this->saveToolStripMenuItem->Size = System::Drawing::Size(108, 22);
				 this->saveToolStripMenuItem->Text = L"Save";
				 this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
				 // 
				 // patingImageToolStripMenuItem
				 // 
				 this->patingImageToolStripMenuItem->Name = L"patingImageToolStripMenuItem";
				 this->patingImageToolStripMenuItem->Size = System::Drawing::Size(92, 20);
				 this->patingImageToolStripMenuItem->Text = L"PatingImage";
				 this->patingImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::patingImageToolStripMenuItem_Click);
				 // 
				 // Form1
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(916, 552);
				 this->Controls->Add(this->Main_splitContainer);
				 this->Controls->Add(this->menuStrip1);
				 this->Name = L"Form1";
				 this->Text = L"Form1";
				 this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
				 this->Shown += gcnew System::EventHandler(this, &Form1::Form1_Shown);
				 this->Main_splitContainer->Panel1->ResumeLayout(false);
				 this->Main_splitContainer->Panel1->PerformLayout();
				 this->Main_splitContainer->ResumeLayout(false);
				 this->Shocking_groupBox->ResumeLayout(false);
				 this->Shocking_groupBox->PerformLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MaximumShockStregth_numericUpDown))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShockStregthValue_numericUpDown))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShockSpeed_numericUpDown))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShockingSpeedMinus_numericUpDown))->EndInit();
				 this->Terrian_groupBox->ResumeLayout(false);
				 this->Terrian_groupBox->PerformLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TerrianHeight_numericUpDown))->EndInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CircleRadius_numericUpDown))->EndInit();
				 this->menuStrip1->ResumeLayout(false);
				 this->menuStrip1->PerformLayout();
				 this->ResumeLayout(false);
				 this->PerformLayout();

			 }
#pragma endregion
			 System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pGameApp )
					 this->m_pGameApp->Run();
				 //update
				 if( m_pMyBoobsPainter )
					 m_pMyBoobsPainter->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
				 //render
				 wglMakeCurrent( m_HdcMV,m_HGLRCMV );
				 RECT rcClient;
				 GetClientRect((HWND)m_pTargetControl->Handle.ToPointer(), &rcClient);
				 UseShaderProgram();
				 cGameApp::m_svViewPortSize.x = 0.f;
				 cGameApp::m_svViewPortSize.y = 0.f;
				 cGameApp::m_svViewPortSize.z = (float)Width;
				 cGameApp::m_svViewPortSize.w = (float)rcClient.bottom;//(float)Height-(Height-rcClient.bottom);
				 //RECT	l_rc;
				 //GetWindowRect((HWND)this->Handle.ToPointer(),&l_rc);
				 //cGameApp::m_svViewPortSize.y = l_rc.top;
				 glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
				 glClearColor(0.5,0.5,0.5,0);
				 glClearDepth(1.0f);											// Depth Buffer Setup
				 glAlphaFunc(GL_GREATER,0.0001f);
				 glShadeModel(GL_SMOOTH);									// Select Smooth Shading
				 glEnable(GL_TEXTURE_2D);
				 glEnable(GL_BLEND);
				 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				 glEnable(GL_ALPHA_TEST);
				 FATMING_CORE::UseShaderProgram(DEFAULT_SHADER);
				 if( m_pOrthogonalCamera )
					 m_pOrthogonalCamera->Render();
				 if(!CameraSwitch_checkBox->Checked)
					 m_pOrthogonalCamera->DrawGrid(0,0,Vector4(0.3f,0.7f,0.3f,0.7f));
				 if( m_pMyBoobsPainter )
				 {
					 if( ShockLoop_checkBox->Checked )
					 {
						 if(m_pMyBoobsPainter->IsShockDone())
						 {
							 Shock_button_Click(this,e);
						 }
					 }
					 m_pMyBoobsPainter->Render();
					 switch( TerianViewType_comboBox->SelectedIndex)
					 {
					 case 0:
						 m_pMyBoobsPainter->RenderTerrian(CameraSwitch_checkBox->Checked?m_pFrameCamera:0);
						 break;
					 case 1:
						 m_pMyBoobsPainter->DebugRenderTerrian(CameraSwitch_checkBox->Checked?m_pFrameCamera:0);
						 break;
					 case 2:
						 m_pMyBoobsPainter->RenderTerrian(CameraSwitch_checkBox->Checked?m_pFrameCamera:0);
						 m_pMyBoobsPainter->DebugRenderTerrian(CameraSwitch_checkBox->Checked?m_pFrameCamera:0);
						 break;
					 }
				 }
				 if( m_pOrthogonalCamera )
					 m_pOrthogonalCamera->Render();
				 Vector2	l_vMousePos = m_pOrthogonalCamera->GetMouseWorldPos();
				 cGameApp::m_sMousePosition.x = (int)l_vMousePos.x;
				 cGameApp::m_sMousePosition.y = (int)l_vMousePos.y;
				 glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
				 Vector3	l_vPos = m_pFrameCamera->GetWorldPosition();
				 cGameApp::RenderFont(0,70,L"CameraPos");
				 cGameApp::RenderFont(0,85,ValueToStringW(l_vPos).c_str());
				 cGameApp::ShowInfo();
				 SwapBuffers(m_HdcMV);
			 }

			 System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 //this->Focus();
				 if( !timer1->Enabled || !this->Visible )
					 return;
				 m_pTargetControl->Focus();
				 m_pGameApp->MouseDown(e->X,e->Y);
				 GCFORM::MouseButtons l_MouseButton = e->Button;
				 if(CameraSwitch_checkBox->Checked)
				 {
					 if(m_pMyBoobsPainter->m_ePatinerMode == ePM_PAINT)
					 {
						m_pCameraBehaveByMouseBehave->CameraUpDateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta*10,e->X,e->Y,cGameApp::m_sTimeAndFPS.fElpaseTime);
					 }
					 else
					 if( l_MouseButton == System::Windows::Forms::MouseButtons::Left )
						m_pMyBoobsPainter->MouseDown(e->X,e->Y);
				 }
				 else
				 {
					 m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
						 ,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
					 Vector2	l_vMousePos = m_pOrthogonalCamera->GetMouseWorldPos();
					 if( m_pMyBoobsPainter )
					 {
						 if( l_MouseButton == System::Windows::Forms::MouseButtons::Left )
							 m_pMyBoobsPainter->MouseDown((int)l_vMousePos.x,(int)l_vMousePos.y);
					 }
				 }
			 }
			 System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 if( !timer1->Enabled || !this->Visible )
					 return;
				 m_pGameApp->MouseMove(e->X,e->Y);
				 GCFORM::MouseButtons l_MouseButton = e->Button;
				 if(CameraSwitch_checkBox->Checked)
				 {
					 if(m_pMyBoobsPainter->m_ePatinerMode == ePM_PAINT)
					 {
						m_pCameraBehaveByMouseBehave->CameraUpDateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta*10,e->X,e->Y,cGameApp::m_sTimeAndFPS.fElpaseTime);
					 }
					 else
						 m_pMyBoobsPainter->MouseMove(e->X,e->Y);

				 }
				 else
				 {
					 POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
					 m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
						 ,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
					 Vector2	l_vMousePos = m_pOrthogonalCamera->GetMouseWorldPos();
					 //if( l_MouseButton == System::Windows::Forms::MouseButtons::Left )
					 if( m_pMyBoobsPainter )
						 m_pMyBoobsPainter->MouseMove((int)l_vMousePos.x,(int)l_vMousePos.y);
				 }
			 }

			 System::Void MyMouseHover(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( !timer1->Enabled || !this->Visible )
					 return;
				 //System::Windows::Forms::MouseEventArgs^l_pMouseEventArgs = (System::Windows::Forms::MouseEventArgs^)e;
				 //Vector2	l_vMousePos = m_pOrthogonalCamera->GetMouseWorldPos();
				 //if( l_pMouseEventArgs->Button == System::Windows::Forms::MouseButtons::Left )
				 //	m_pMyBoobsPainter->MouseUp((int)l_vMousePos.x,(int)l_vMousePos.y);
			 }

			 System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 if( !timer1->Enabled || !this->Visible )
					 return;
				 m_pGameApp->MouseUp(e->X,e->Y);
				 GCFORM::MouseButtons l_MouseButton = e->Button;
				 if(CameraSwitch_checkBox->Checked)
				 {
					 if(m_pMyBoobsPainter->m_ePatinerMode == ePM_PAINT)
					 {
						m_pCameraBehaveByMouseBehave->CameraUpDateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta*10,e->X,e->Y,cGameApp::m_sTimeAndFPS.fElpaseTime);
					 }
					 else
					 if( l_MouseButton == System::Windows::Forms::MouseButtons::Left )
					 {
						 m_pMyBoobsPainter->MouseUp(e->X,e->Y);
					 }
				 }
				 else
				 {
					 m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
						 ,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
					 Vector2	l_vMousePos = m_pOrthogonalCamera->GetMouseWorldPos();
					 if( m_pMyBoobsPainter )
					 {
						 if( l_MouseButton == System::Windows::Forms::MouseButtons::Left )
							 m_pMyBoobsPainter->MouseUp((int)l_vMousePos.x,(int)l_vMousePos.y);
					 }
				 }
			 }

	private: System::Void Alpha_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 int	l_iHeight = (int)TerrianHeight_numericUpDown->Value;
				 //l_iHeight += 8355711;
				 int	l_iMultiply = 3;
				 int64	l_iA = 255;
				 int64	l_iR = l_iHeight*l_iMultiply;
				 int64	l_iG = l_iHeight*l_iMultiply;
				 int64	l_iB = l_iHeight*l_iMultiply;
				 //l_iG = BitMoveRight(l_iG,8);
				 //l_iR = BitMoveRight(l_iR,16);
				 if( m_pMyBoobsPainter )
				 {
					 m_pMyBoobsPainter->SetColor(Vector4((float)l_iR,(float)l_iG,(float)l_iB,(float)l_iA));
				 }
				 //011111110111111101111111 = 8355711
				 //int	l_iHeight = (int)TerrianHeight_numericUpDown->Value;
				 ////l_iHeight += 8355711;
				 //int64	l_iA = (int)Alpha_numericUpDown->Value;
				 //int64	l_iR = l_iHeight&0x00ff0000;
				 //int64	l_iG = l_iHeight&0x0000ff00;
				 //int64	l_iB = l_iHeight&0x000000ff;
				 //l_iG = BitMoveRight(l_iG,8);
				 //l_iR = BitMoveRight(l_iR,16);
				 //if( m_pMyBoobsPainter )
				 //{
				 // m_pMyBoobsPainter->SetColor(Vector4((float)l_iR,(float)l_iG,(float)l_iB,(float)l_iA));
				 //}
			 }
	private: System::Void CircleRadius_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 if( m_pMyBoobsPainter )
					 m_pMyBoobsPainter->SetSphereRadius((int)CircleRadius_numericUpDown->Value);
			 }
	private: System::Void Clean_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pMyBoobsPainter )
					 m_pMyBoobsPainter->Clean();
			 }
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e)
			 {

			 }
	private: System::Void Form1_Shown(System::Object^  sender, System::EventArgs^  e) 
			 {
				 if( m_pGameApp )
					 return ;
				 m_pGameApp = new cGameApp((HWND)m_pTargetControl->Handle.ToPointer());
				 m_pGameApp->Init();

				 m_HdcMV = GetDC((HWND)m_pTargetControl->Handle.ToPointer());
				 m_HGLRCMV = m_pGameApp->m_sHGLRC;
				 //for mouse event
				 m_pTargetControl->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseMove);
				 m_pTargetControl->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseDown);
				 m_pTargetControl->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
				 m_pTargetControl->MouseHover += gcnew System::EventHandler(this, &Form1::MyMouseHover);
				 m_pTargetControl->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
				 //
				 m_pOrthogonalCamera = new cOrthogonalCamera();
				 //m_pMyBoobsPainter = new cShockBoobsPainter("123.png",ePM_GAME_MODE);
				 m_pMyBoobsPainter = new cShockBoobsPainter("123.png");
				 Projection	l_ProjectionMV;
				 l_ProjectionMV.SetFovYAspect( XM_PIDIV4 ,1920/1080.f, 1000.0f * 1e-4f,2000000.f);
				 m_pFrameCamera = new cFrameCamera();
				 float	l_fMat[] = {0.98139834f,0.00000000f,-0.19198620f,0.00000000f,0.022139410f,-0.99332988f,0.11317252f,0.00000000f,-0.19070573f,-0.11531734f,-0.97485310f
					 ,0.00000000f,65.08221f,56.62949f,-702.8380f,1.000000f};
				 cMatrix44	l_Mat = cMatrix44(l_fMat);
				 m_pFrameCamera->SetWorldTransform(l_Mat);
				 m_pFrameCamera->SetProjection(l_ProjectionMV);
				 m_pCameraBehaveByMouseBehave = new cCameraBehaveByMouseBehave(m_pFrameCamera);
				 m_pCameraBehaveByMouseBehave->SetMouseMoveSpeed(10);
				 TerrianHeight_numericUpDown->Value = 1;
				 //
				 this->timer1->Enabled = true;
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 float	l_fMat[] = {0.98139834f,0.00000000f,-0.19198620f,0.00000000f,0.022139410f,-0.99332988f,0.11317252f,0.00000000f,-0.19070573f,-0.11531734f,-0.97485310f
					 ,0.00000000f,65.08221f,56.62949f,-702.8380f,1.000000f};
				 cMatrix44	l_Mat = cMatrix44(l_fMat);
				 m_pFrameCamera->SetWorldTransform(l_Mat);
				 //String^l_strFileName = DNCT::OpenFileAndGetName();
				 //if( l_strFileName )
				 //{
				 //	g_pColladaParser = new cColladaParser();
				 //	g_pColladaParser->ParseDataIntoXMLNode(DNCT::GcStringToChar(l_strFileName));
				 //	g_pMesh = g_pColladaParser->m_AllMesh[0];
				 //}
			 }
	
	private: System::Void Shock_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pMyBoobsPainter && this->timer1->Enabled )
				 {
					 m_pMyBoobsPainter->SetParmater((float)ShockSpeed_numericUpDown->Value,(float)ShockingSpeedMinus_numericUpDown->Value,(float)ShockStregthValue_numericUpDown->Value,(float)MaximumShockStregth_numericUpDown->Value);
				 }
			 }
	private: System::Void CameraSwitch_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(CameraSwitch_checkBox->Checked)
					 CameraSwitch_checkBox->Text = "3D";
				 else
					 CameraSwitch_checkBox->Text = "2D";
			 }
	private: System::Void ColorType_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(ColorType_comboBox->SelectedIndex != -1 )
				 {
					 if( m_pMyBoobsPainter )
						 m_pMyBoobsPainter->m_eColorType = (ePaintType)ColorType_comboBox->SelectedIndex;
				 }
			 }
	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 String^l_str = DNCT::OpenFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
				 if( l_str )
				 {
					 if(!m_pMyBoobsPainter->Parse(DNCT::GcStringToChar(l_str)))
					 {
						WARNING_MSG("open file failed");
					 }
					 else
					 {
						m_pMyBoobsPainter->m_ePatinerMode = ePM_SHOCK;
						cShockBoobsShader*l_pShader = m_pMyBoobsPainter->GetShader();
						ShockSpeed_numericUpDown->Value = System::Decimal(l_pShader->GetShockSpeed());
						ShockingSpeedMinus_numericUpDown->Value = System::Decimal(l_pShader->GetShockSpeedMinus());
						ShockStregthValue_numericUpDown->Value = System::Decimal(l_pShader->GetShockStrgethValue());
						MaximumShockStregth_numericUpDown->Value = System::Decimal(l_pShader->GetMaximumShockStregth());
					 }
				 }
				 this->timer1->Enabled = true;
			 }
	private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;	 
				 String^l_strFileName = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
				 if( l_strFileName )
				 {
					 if( m_pMyBoobsPainter )
					 {
						if(!m_pMyBoobsPainter->SaveFile(DNCT::GcStringToChar(l_strFileName)))
						{
							WARNING_MSG("save file failed");
						}
					 }
				 }
				 this->timer1->Enabled = true;
			 }
	private: System::Void MyPainterMode_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( MyPainterMode_checkBox->Checked )
				 {
					 MyPainterMode_checkBox->Text = "Shock Mode";
					 m_pMyBoobsPainter->m_ePatinerMode = ePM_SHOCK;
				 }
				 else
				 {
					MyPainterMode_checkBox->Text = "Paint Mode";
					m_pMyBoobsPainter->m_ePatinerMode = ePM_PAINT;
				 }
			 }
private: System::Void patingImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strFileName = DNCT::OpenFileAndGetName("png files (*.png)|*.png|All files (*.*)|*.*");
			 if( l_strFileName )
			 {
				m_pMyBoobsPainter->ChangePaintingImage(DNCT::GcStringToChar(l_strFileName));
			 }
		 }
};
}