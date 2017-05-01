#pragma once
#include <string>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../XML/StringCompress.h"

#include "StageParser.h"
#include "GameData.h"
#include "GemPriceToProbability.h"
extern cOrthogonalCamera*	g_pOrthogonalCamera;
namespace XMLDataEditor 
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
		Form1(String^e_strFileName)
		{
			InitializeComponent();

			m_pTargetControl = OpemnGL_panel;
			m_pGameApp = new cGameApp((HWND)m_pTargetControl->Handle.ToPointer());
			m_pGameApp->Init();
			m_HdcMV = GetDC((HWND)m_pTargetControl->Handle.ToPointer());
			m_HGLRCMV = m_pGameApp->m_sHGLRC;
			m_pOrthogonalCamera = new cOrthogonalCamera();
			g_pOrthogonalCamera = m_pOrthogonalCamera;
			//for mouse event
			m_pTargetControl->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseMove);
			m_pTargetControl->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseDown);
			m_pTargetControl->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
			m_pTargetControl->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
			m_pGameData = nullptr;
			m_pStageParser = new cStageParser();
			m_pChartWithName = new cChartWithName();
			m_pLinerData = nullptr;
			this->timer1->Enabled = true;
			m_bWaitParseFile = false;
			m_iFirstStageMonsterCount = 0;
			m_iFirstStageHurdleAndBreakableCount = 0;
			m_iFirstStagePickUpCount = 0;
			m_iFirstStageCoinCount = 0;
			m_fFirstSgateTime = 0.f;
			Main_tabControl->SelectedIndex = 3;
			m_pGemPriceToProbability = new cGemPriceToProbability[Probability_tabControl->TabPages->Count];
			array<GCFORM::ListBox^>^l_pListBox = { BadItemProbability_listBox,GoodItemProbability_listBox };
			array<GCFORM::TextBox^>^l_pTextBox = { BadItemProbability_textBox,GoodItemProbability_textBox };
			array<GCFORM::NumericUpDown^>^l_pNumericUpDown = { BadItemScore_numericUpDown,GoodItemScore_numericUpDown};
			m_pDrawListBoxArray = l_pListBox;
			m_pDrawTextBoxArray = l_pTextBox;
			m_pDrawNumericUpDownArray = l_pNumericUpDown;
		}

	protected:
		~Form1()
		{
			m_bWaitParseFile = true;
			this->timer1->Enabled = false;
			if (components)
			{
				delete components;
			}
			SAFE_DELETE(m_pOrthogonalCamera);
			SAFE_DELETE(m_pStageParser);
			SAFE_DELETE(m_pGameData);
			SAFE_DELETE(m_pChartWithName);
			SAFE_DELETE(m_pLinerData);
			SAFE_DELETE(m_pGameApp);
			SAFE_DELETE_ARRAY(m_pGemPriceToProbability);
		}
//my
		cGameData*			m_pGameData;
		cStageParser*		m_pStageParser;
		cChartWithName*		m_pChartWithName;
		cGameApp*			m_pGameApp;
		HDC					m_HdcMV;
		HGLRC				m_HGLRCMV;
		Control^			m_pTargetControl;
		cOrthogonalCamera*	m_pOrthogonalCamera;
		cLinerDataProcessor<Vector3>*m_pLinerData;
		cGemPriceToProbability *m_pGemPriceToProbability;
		array<GCFORM::ListBox^>^m_pDrawListBoxArray;
		array<GCFORM::TextBox^>^m_pDrawTextBoxArray;
		array<GCFORM::NumericUpDown^>^m_pDrawNumericUpDownArray;
		bool				m_bWaitParseFile;
		int					m_iFirstStageMonsterCount;
		int					m_iFirstStageHurdleAndBreakableCount;
		int					m_iFirstStagePickUpCount;
		int					m_iFirstStageCoinCount;
		float				m_fFirstSgateTime;

	private: System::Windows::Forms::TextBox^  FileName_textBox;
	private: System::Windows::Forms::Button^  OpenFile_button;
	private: System::Windows::Forms::ListBox^  Node_listBox;
	private: System::Windows::Forms::Label^  Node_label;
	private: System::Windows::Forms::ListBox^  NodeGroupAndCount_listBox;
	private: System::Windows::Forms::Label^  NodeGroupAndCount_label;
	private: System::Windows::Forms::Panel^  OpemnGL_panel;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::ListBox^  RenderMode_listBox;
	private: System::Windows::Forms::Label^  RenderMode_label;
	private: System::Windows::Forms::ListBox^  RenderParameterIndex_listBox;
	private: System::Windows::Forms::CheckBox^  ShowParameterValue_checkBox;
	private: System::Windows::Forms::TabControl^  Main_tabControl;
	private: System::Windows::Forms::TabPage^  Gameplay_tabPage;
	private: System::Windows::Forms::TabPage^  GameData_tabPage;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TabControl^  GameData_tabControl;
	private: System::Windows::Forms::TabPage^  Enemy_tabPage;
	private: System::Windows::Forms::TabPage^  Shop;
	private: System::Windows::Forms::ListBox^  EnemyData_listBox;
	private: System::Windows::Forms::ListBox^  ShopData_listBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::TabPage^  Monster_tabPage;
	private: System::Windows::Forms::ListBox^  MonsterStatusShowCurve_listBox;
	private: System::Windows::Forms::ListBox^  MonsterData_listBox;
	private: System::Windows::Forms::ListBox^  EnemyStatus_listBox;
	private: System::Windows::Forms::ListBox^  ShopStatus_listBox;
	private: System::Windows::Forms::CheckBox^  MonsterShowValue_checkBox;
	private: System::Windows::Forms::CheckBox^  EnemyShowValue_checkBox;
	private: System::Windows::Forms::CheckBox^  ShopShowValue_checkBox;
	private: System::Windows::Forms::ListBox^  Item_listBox;
	private: System::Windows::Forms::Button^  CaemraReset_button;
	private: System::Windows::Forms::TabPage^  NewCurve_tabPage;
	private: System::Windows::Forms::Label^  Steep_label;
	private: System::Windows::Forms::Label^  Grident_label;
	private: System::Windows::Forms::Label^  StepValue_label;
	private: System::Windows::Forms::Label^  StartPos_label;
	private: System::Windows::Forms::NumericUpDown^  StepY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  StepX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  StartPosY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  StartPosX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  Grident_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  Steep_numericUpDown;
private: System::Windows::Forms::Label^  Number_label;
private: System::Windows::Forms::NumericUpDown^  Number_numericUpDown;
private: System::Windows::Forms::TextBox^  Result_textBox;

private: System::Windows::Forms::CheckBox^  MonsterRender_checkBox;
private: System::Windows::Forms::CheckBox^  EnemyRender_checkBox;
private: System::Windows::Forms::TextBox^  TimeToGems_textBox;
private: System::Windows::Forms::Label^  TargetSeconds_label;
private: System::Windows::Forms::Label^  OneRMBCostGems_label;
private: System::Windows::Forms::NumericUpDown^  TargetSceonds_numericUpDown;

private: System::Windows::Forms::NumericUpDown^  OneRMBCostGems_numericUpDown;
private: System::Windows::Forms::Label^  AHourSalary_label;
private: System::Windows::Forms::NumericUpDown^  AHourSalary_numericUpDown;
private: System::Windows::Forms::Label^  DailySalary_label;
private: System::Windows::Forms::Label^  GemPrice_label;
private: System::Windows::Forms::NumericUpDown^  GemPrice_numericUpDown;
private: System::Windows::Forms::NumericUpDown^  Round_numericUpDown;


private: System::Windows::Forms::Label^  XMLPrefix_label;
private: System::Windows::Forms::TextBox^  XMLPrefix_textBox;
private: System::Windows::Forms::GroupBox^  TimeMoneyExchange_groupBox;
private: System::Windows::Forms::CheckBox^  MoneyRound_checkBox;

private: System::Windows::Forms::NumericUpDown^  MoneyRound_numericUpDown;

private: System::Windows::Forms::CheckBox^  Round_checkBox;
private: System::Windows::Forms::TextBox^  TimeToMoneyList__textBox;

private: System::Windows::Forms::TextBox^  MoneyXMLPrefix_textBox;

private: System::Windows::Forms::Label^  MoneyXMLPrefix_label;
private: System::Windows::Forms::Button^  DoTimeToMoney_button;
private: System::Windows::Forms::ListBox^  StageClearBonus_listBox;
private: System::Windows::Forms::CheckBox^  StageRender_checkBox;
private: System::Windows::Forms::CheckBox^  StageBonusRender_checkBox;
private: System::Windows::Forms::CheckBox^  GridentWithPrevious_checkBox;
private: System::Windows::Forms::TabPage^  ItemProbability_tabPage;
private: System::Windows::Forms::TextBox^  GoodItemProbability_textBox;

private: System::Windows::Forms::ListBox^  GoodItemProbability_listBox;

private: System::Windows::Forms::Button^  GemPriceToProbabilityGenerate_button;
private: System::Windows::Forms::Label^  CoinToGem_label;
private: System::Windows::Forms::Label^  HeartToGems_label;
private: System::Windows::Forms::NumericUpDown^  CoinToGems_numericUpDown;
private: System::Windows::Forms::NumericUpDown^  HeartToGems_numericUpDown;
private: System::Windows::Forms::TabControl^  Probability_tabControl;

private: System::Windows::Forms::TabPage^  GoodPool_tabPage;
private: System::Windows::Forms::NumericUpDown^  GoodItemScore_numericUpDown;

private: System::Windows::Forms::TabPage^  BadPool_tabPage;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::Label^  BadItemScore_label;
private: System::Windows::Forms::NumericUpDown^  BadItemScore_numericUpDown;
private: System::Windows::Forms::ListBox^  BadItemProbability_listBox;
private: System::Windows::Forms::TextBox^  BadItemProbability_textBox;




	private: System::ComponentModel::IContainer^  components;
			 /// <summary>
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->FileName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->OpenFile_button = (gcnew System::Windows::Forms::Button());
			this->Node_listBox = (gcnew System::Windows::Forms::ListBox());
			this->Node_label = (gcnew System::Windows::Forms::Label());
			this->NodeGroupAndCount_listBox = (gcnew System::Windows::Forms::ListBox());
			this->NodeGroupAndCount_label = (gcnew System::Windows::Forms::Label());
			this->OpemnGL_panel = (gcnew System::Windows::Forms::Panel());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->RenderMode_listBox = (gcnew System::Windows::Forms::ListBox());
			this->RenderMode_label = (gcnew System::Windows::Forms::Label());
			this->RenderParameterIndex_listBox = (gcnew System::Windows::Forms::ListBox());
			this->ShowParameterValue_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Main_tabControl = (gcnew System::Windows::Forms::TabControl());
			this->Gameplay_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->StageBonusRender_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->StageRender_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->StageClearBonus_listBox = (gcnew System::Windows::Forms::ListBox());
			this->GameData_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->GameData_tabControl = (gcnew System::Windows::Forms::TabControl());
			this->Monster_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->TimeMoneyExchange_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->DoTimeToMoney_button = (gcnew System::Windows::Forms::Button());
			this->TimeToMoneyList__textBox = (gcnew System::Windows::Forms::TextBox());
			this->MoneyXMLPrefix_textBox = (gcnew System::Windows::Forms::TextBox());
			this->MoneyXMLPrefix_label = (gcnew System::Windows::Forms::Label());
			this->MoneyRound_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->DailySalary_label = (gcnew System::Windows::Forms::Label());
			this->MoneyRound_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->OneRMBCostGems_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GemPrice_label = (gcnew System::Windows::Forms::Label());
			this->TargetSceonds_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GemPrice_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->OneRMBCostGems_label = (gcnew System::Windows::Forms::Label());
			this->TargetSeconds_label = (gcnew System::Windows::Forms::Label());
			this->AHourSalary_label = (gcnew System::Windows::Forms::Label());
			this->TimeToGems_textBox = (gcnew System::Windows::Forms::TextBox());
			this->AHourSalary_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->MonsterRender_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->MonsterShowValue_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->MonsterStatusShowCurve_listBox = (gcnew System::Windows::Forms::ListBox());
			this->MonsterData_listBox = (gcnew System::Windows::Forms::ListBox());
			this->Enemy_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->EnemyRender_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->EnemyShowValue_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->EnemyStatus_listBox = (gcnew System::Windows::Forms::ListBox());
			this->EnemyData_listBox = (gcnew System::Windows::Forms::ListBox());
			this->Shop = (gcnew System::Windows::Forms::TabPage());
			this->Item_listBox = (gcnew System::Windows::Forms::ListBox());
			this->ShopShowValue_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ShopStatus_listBox = (gcnew System::Windows::Forms::ListBox());
			this->ShopData_listBox = (gcnew System::Windows::Forms::ListBox());
			this->NewCurve_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->GridentWithPrevious_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Round_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Result_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Number_label = (gcnew System::Windows::Forms::Label());
			this->Round_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Number_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->XMLPrefix_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Steep_label = (gcnew System::Windows::Forms::Label());
			this->XMLPrefix_label = (gcnew System::Windows::Forms::Label());
			this->Grident_label = (gcnew System::Windows::Forms::Label());
			this->StepValue_label = (gcnew System::Windows::Forms::Label());
			this->StartPos_label = (gcnew System::Windows::Forms::Label());
			this->StepY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->StepX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->StartPosY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->StartPosX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Grident_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Steep_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ItemProbability_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->Probability_tabControl = (gcnew System::Windows::Forms::TabControl());
			this->BadPool_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->BadItemScore_label = (gcnew System::Windows::Forms::Label());
			this->BadItemScore_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->BadItemProbability_listBox = (gcnew System::Windows::Forms::ListBox());
			this->BadItemProbability_textBox = (gcnew System::Windows::Forms::TextBox());
			this->GoodPool_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->GoodItemScore_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GoodItemProbability_listBox = (gcnew System::Windows::Forms::ListBox());
			this->GoodItemProbability_textBox = (gcnew System::Windows::Forms::TextBox());
			this->CoinToGem_label = (gcnew System::Windows::Forms::Label());
			this->HeartToGems_label = (gcnew System::Windows::Forms::Label());
			this->CoinToGems_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GemPriceToProbabilityGenerate_button = (gcnew System::Windows::Forms::Button());
			this->HeartToGems_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->CaemraReset_button = (gcnew System::Windows::Forms::Button());
			this->Main_tabControl->SuspendLayout();
			this->Gameplay_tabPage->SuspendLayout();
			this->GameData_tabPage->SuspendLayout();
			this->GameData_tabControl->SuspendLayout();
			this->Monster_tabPage->SuspendLayout();
			this->TimeMoneyExchange_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MoneyRound_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->OneRMBCostGems_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TargetSceonds_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GemPrice_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AHourSalary_numericUpDown))->BeginInit();
			this->Enemy_tabPage->SuspendLayout();
			this->Shop->SuspendLayout();
			this->NewCurve_tabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Round_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Number_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StepY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StepX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartPosY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartPosX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Grident_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Steep_numericUpDown))->BeginInit();
			this->ItemProbability_tabPage->SuspendLayout();
			this->Probability_tabControl->SuspendLayout();
			this->BadPool_tabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->BadItemScore_numericUpDown))->BeginInit();
			this->GoodPool_tabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GoodItemScore_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CoinToGems_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->HeartToGems_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// FileName_textBox
			// 
			this->FileName_textBox->Location = System::Drawing::Point(8, 29);
			this->FileName_textBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->FileName_textBox->Name = L"FileName_textBox";
			this->FileName_textBox->Size = System::Drawing::Size(729, 38);
			this->FileName_textBox->TabIndex = 0;
			this->FileName_textBox->Text = L"gameplay.xml";
			// 
			// OpenFile_button
			// 
			this->OpenFile_button->Location = System::Drawing::Point(8, 91);
			this->OpenFile_button->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->OpenFile_button->Name = L"OpenFile_button";
			this->OpenFile_button->Size = System::Drawing::Size(312, 69);
			this->OpenFile_button->TabIndex = 1;
			this->OpenFile_button->Text = L"Open Gameplay File";
			this->OpenFile_button->UseVisualStyleBackColor = true;
			this->OpenFile_button->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Node_listBox
			// 
			this->Node_listBox->FormattingEnabled = true;
			this->Node_listBox->ItemHeight = 31;
			this->Node_listBox->Location = System::Drawing::Point(16, 38);
			this->Node_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Node_listBox->Name = L"Node_listBox";
			this->Node_listBox->Size = System::Drawing::Size(713, 1089);
			this->Node_listBox->TabIndex = 2;
			this->Node_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::Node_listBox_SelectedIndexChanged);
			// 
			// Node_label
			// 
			this->Node_label->AutoSize = true;
			this->Node_label->Location = System::Drawing::Point(8, 0);
			this->Node_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->Node_label->Name = L"Node_label";
			this->Node_label->Size = System::Drawing::Size(83, 32);
			this->Node_label->TabIndex = 3;
			this->Node_label->Text = L"Node";
			// 
			// NodeGroupAndCount_listBox
			// 
			this->NodeGroupAndCount_listBox->FormattingEnabled = true;
			this->NodeGroupAndCount_listBox->ItemHeight = 31;
			this->NodeGroupAndCount_listBox->Location = System::Drawing::Point(16, 1192);
			this->NodeGroupAndCount_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->NodeGroupAndCount_listBox->Name = L"NodeGroupAndCount_listBox";
			this->NodeGroupAndCount_listBox->Size = System::Drawing::Size(444, 438);
			this->NodeGroupAndCount_listBox->TabIndex = 4;
			// 
			// NodeGroupAndCount_label
			// 
			this->NodeGroupAndCount_label->AutoSize = true;
			this->NodeGroupAndCount_label->Location = System::Drawing::Point(8, 1154);
			this->NodeGroupAndCount_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->NodeGroupAndCount_label->Name = L"NodeGroupAndCount_label";
			this->NodeGroupAndCount_label->Size = System::Drawing::Size(251, 32);
			this->NodeGroupAndCount_label->TabIndex = 5;
			this->NodeGroupAndCount_label->Text = L"Triggers and count";
			// 
			// OpemnGL_panel
			// 
			this->OpemnGL_panel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->OpemnGL_panel->Location = System::Drawing::Point(0, 0);
			this->OpemnGL_panel->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->OpemnGL_panel->Name = L"OpemnGL_panel";
			this->OpemnGL_panel->Size = System::Drawing::Size(1670, 2088);
			this->OpemnGL_panel->TabIndex = 7;
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// RenderMode_listBox
			// 
			this->RenderMode_listBox->FormattingEnabled = true;
			this->RenderMode_listBox->ItemHeight = 31;
			this->RenderMode_listBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Map", L"ChartCompare"});
			this->RenderMode_listBox->Location = System::Drawing::Point(483, 1221);
			this->RenderMode_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->RenderMode_listBox->Name = L"RenderMode_listBox";
			this->RenderMode_listBox->Size = System::Drawing::Size(308, 97);
			this->RenderMode_listBox->TabIndex = 10;
			// 
			// RenderMode_label
			// 
			this->RenderMode_label->AutoSize = true;
			this->RenderMode_label->Location = System::Drawing::Point(475, 1161);
			this->RenderMode_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->RenderMode_label->Name = L"RenderMode_label";
			this->RenderMode_label->Size = System::Drawing::Size(179, 32);
			this->RenderMode_label->TabIndex = 11;
			this->RenderMode_label->Text = L"RenderMode";
			// 
			// RenderParameterIndex_listBox
			// 
			this->RenderParameterIndex_listBox->FormattingEnabled = true;
			this->RenderParameterIndex_listBox->ItemHeight = 31;
			this->RenderParameterIndex_listBox->Location = System::Drawing::Point(8, 1679);
			this->RenderParameterIndex_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->RenderParameterIndex_listBox->Name = L"RenderParameterIndex_listBox";
			this->RenderParameterIndex_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->RenderParameterIndex_listBox->Size = System::Drawing::Size(444, 376);
			this->RenderParameterIndex_listBox->TabIndex = 12;
			this->RenderParameterIndex_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::RenderParameterIndex_listBox_SelectedIndexChanged);
			// 
			// ShowParameterValue_checkBox
			// 
			this->ShowParameterValue_checkBox->AutoSize = true;
			this->ShowParameterValue_checkBox->Location = System::Drawing::Point(491, 1650);
			this->ShowParameterValue_checkBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->ShowParameterValue_checkBox->Name = L"ShowParameterValue_checkBox";
			this->ShowParameterValue_checkBox->Size = System::Drawing::Size(198, 36);
			this->ShowParameterValue_checkBox->TabIndex = 13;
			this->ShowParameterValue_checkBox->Text = L"ShowValue";
			this->ShowParameterValue_checkBox->UseVisualStyleBackColor = true;
			this->ShowParameterValue_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::ShowParameterValue_checkBox_CheckedChanged);
			// 
			// Main_tabControl
			// 
			this->Main_tabControl->Controls->Add(this->Gameplay_tabPage);
			this->Main_tabControl->Controls->Add(this->GameData_tabPage);
			this->Main_tabControl->Controls->Add(this->NewCurve_tabPage);
			this->Main_tabControl->Controls->Add(this->ItemProbability_tabPage);
			this->Main_tabControl->Location = System::Drawing::Point(32, 174);
			this->Main_tabControl->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Main_tabControl->Name = L"Main_tabControl";
			this->Main_tabControl->SelectedIndex = 0;
			this->Main_tabControl->Size = System::Drawing::Size(2685, 2160);
			this->Main_tabControl->TabIndex = 0;
			// 
			// Gameplay_tabPage
			// 
			this->Gameplay_tabPage->Controls->Add(this->StageBonusRender_checkBox);
			this->Gameplay_tabPage->Controls->Add(this->StageRender_checkBox);
			this->Gameplay_tabPage->Controls->Add(this->StageClearBonus_listBox);
			this->Gameplay_tabPage->Controls->Add(this->ShowParameterValue_checkBox);
			this->Gameplay_tabPage->Controls->Add(this->Node_listBox);
			this->Gameplay_tabPage->Controls->Add(this->RenderParameterIndex_listBox);
			this->Gameplay_tabPage->Controls->Add(this->Node_label);
			this->Gameplay_tabPage->Controls->Add(this->RenderMode_label);
			this->Gameplay_tabPage->Controls->Add(this->NodeGroupAndCount_listBox);
			this->Gameplay_tabPage->Controls->Add(this->RenderMode_listBox);
			this->Gameplay_tabPage->Controls->Add(this->NodeGroupAndCount_label);
			this->Gameplay_tabPage->Location = System::Drawing::Point(10, 48);
			this->Gameplay_tabPage->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Gameplay_tabPage->Name = L"Gameplay_tabPage";
			this->Gameplay_tabPage->Padding = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Gameplay_tabPage->Size = System::Drawing::Size(2665, 2102);
			this->Gameplay_tabPage->TabIndex = 0;
			this->Gameplay_tabPage->Text = L"Gameplay";
			this->Gameplay_tabPage->UseVisualStyleBackColor = true;
			// 
			// StageBonusRender_checkBox
			// 
			this->StageBonusRender_checkBox->AutoSize = true;
			this->StageBonusRender_checkBox->Checked = true;
			this->StageBonusRender_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->StageBonusRender_checkBox->Location = System::Drawing::Point(483, 1748);
			this->StageBonusRender_checkBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->StageBonusRender_checkBox->Name = L"StageBonusRender_checkBox";
			this->StageBonusRender_checkBox->Size = System::Drawing::Size(227, 36);
			this->StageBonusRender_checkBox->TabIndex = 16;
			this->StageBonusRender_checkBox->Text = L"BonusRender";
			this->StageBonusRender_checkBox->UseVisualStyleBackColor = true;
			// 
			// StageRender_checkBox
			// 
			this->StageRender_checkBox->AutoSize = true;
			this->StageRender_checkBox->Checked = true;
			this->StageRender_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->StageRender_checkBox->Location = System::Drawing::Point(496, 1595);
			this->StageRender_checkBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->StageRender_checkBox->Name = L"StageRender_checkBox";
			this->StageRender_checkBox->Size = System::Drawing::Size(221, 36);
			this->StageRender_checkBox->TabIndex = 15;
			this->StageRender_checkBox->Text = L"StageRender";
			this->StageRender_checkBox->UseVisualStyleBackColor = true;
			// 
			// StageClearBonus_listBox
			// 
			this->StageClearBonus_listBox->FormattingEnabled = true;
			this->StageClearBonus_listBox->ItemHeight = 31;
			this->StageClearBonus_listBox->Location = System::Drawing::Point(483, 1824);
			this->StageClearBonus_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->StageClearBonus_listBox->Name = L"StageClearBonus_listBox";
			this->StageClearBonus_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->StageClearBonus_listBox->Size = System::Drawing::Size(295, 190);
			this->StageClearBonus_listBox->TabIndex = 14;
			this->StageClearBonus_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::StageClearBonus_listBox_SelectedIndexChanged);
			// 
			// GameData_tabPage
			// 
			this->GameData_tabPage->Controls->Add(this->GameData_tabControl);
			this->GameData_tabPage->Location = System::Drawing::Point(10, 48);
			this->GameData_tabPage->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->GameData_tabPage->Name = L"GameData_tabPage";
			this->GameData_tabPage->Padding = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->GameData_tabPage->Size = System::Drawing::Size(2665, 2102);
			this->GameData_tabPage->TabIndex = 1;
			this->GameData_tabPage->Text = L"GameData";
			this->GameData_tabPage->UseVisualStyleBackColor = true;
			// 
			// GameData_tabControl
			// 
			this->GameData_tabControl->Controls->Add(this->Monster_tabPage);
			this->GameData_tabControl->Controls->Add(this->Enemy_tabPage);
			this->GameData_tabControl->Controls->Add(this->Shop);
			this->GameData_tabControl->Dock = System::Windows::Forms::DockStyle::Fill;
			this->GameData_tabControl->Location = System::Drawing::Point(8, 7);
			this->GameData_tabControl->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->GameData_tabControl->Name = L"GameData_tabControl";
			this->GameData_tabControl->SelectedIndex = 0;
			this->GameData_tabControl->Size = System::Drawing::Size(2649, 2088);
			this->GameData_tabControl->TabIndex = 0;
			// 
			// Monster_tabPage
			// 
			this->Monster_tabPage->Controls->Add(this->TimeMoneyExchange_groupBox);
			this->Monster_tabPage->Controls->Add(this->MonsterRender_checkBox);
			this->Monster_tabPage->Controls->Add(this->MonsterShowValue_checkBox);
			this->Monster_tabPage->Controls->Add(this->MonsterStatusShowCurve_listBox);
			this->Monster_tabPage->Controls->Add(this->MonsterData_listBox);
			this->Monster_tabPage->Location = System::Drawing::Point(10, 48);
			this->Monster_tabPage->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Monster_tabPage->Name = L"Monster_tabPage";
			this->Monster_tabPage->Padding = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Monster_tabPage->Size = System::Drawing::Size(2629, 2030);
			this->Monster_tabPage->TabIndex = 0;
			this->Monster_tabPage->Text = L"Monster";
			this->Monster_tabPage->UseVisualStyleBackColor = true;
			// 
			// TimeMoneyExchange_groupBox
			// 
			this->TimeMoneyExchange_groupBox->Controls->Add(this->DoTimeToMoney_button);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->TimeToMoneyList__textBox);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->MoneyXMLPrefix_textBox);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->MoneyXMLPrefix_label);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->MoneyRound_checkBox);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->DailySalary_label);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->MoneyRound_numericUpDown);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->OneRMBCostGems_numericUpDown);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->GemPrice_label);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->TargetSceonds_numericUpDown);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->GemPrice_numericUpDown);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->OneRMBCostGems_label);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->TargetSeconds_label);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->AHourSalary_label);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->TimeToGems_textBox);
			this->TimeMoneyExchange_groupBox->Controls->Add(this->AHourSalary_numericUpDown);
			this->TimeMoneyExchange_groupBox->Location = System::Drawing::Point(16, 1188);
			this->TimeMoneyExchange_groupBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->TimeMoneyExchange_groupBox->Name = L"TimeMoneyExchange_groupBox";
			this->TimeMoneyExchange_groupBox->Padding = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->TimeMoneyExchange_groupBox->Size = System::Drawing::Size(747, 818);
			this->TimeMoneyExchange_groupBox->TabIndex = 0;
			this->TimeMoneyExchange_groupBox->TabStop = false;
			this->TimeMoneyExchange_groupBox->Text = L"TimeMoneyExchange";
			// 
			// DoTimeToMoney_button
			// 
			this->DoTimeToMoney_button->Location = System::Drawing::Point(392, 749);
			this->DoTimeToMoney_button->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->DoTimeToMoney_button->Name = L"DoTimeToMoney_button";
			this->DoTimeToMoney_button->Size = System::Drawing::Size(320, 55);
			this->DoTimeToMoney_button->TabIndex = 27;
			this->DoTimeToMoney_button->Text = L"DoTimeToMoney";
			this->DoTimeToMoney_button->UseVisualStyleBackColor = true;
			this->DoTimeToMoney_button->Click += gcnew System::EventHandler(this, &Form1::DoTimeToMoney_button_Click);
			// 
			// TimeToMoneyList__textBox
			// 
			this->TimeToMoneyList__textBox->Location = System::Drawing::Point(392, 289);
			this->TimeToMoneyList__textBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->TimeToMoneyList__textBox->Multiline = true;
			this->TimeToMoneyList__textBox->Name = L"TimeToMoneyList__textBox";
			this->TimeToMoneyList__textBox->Size = System::Drawing::Size(313, 440);
			this->TimeToMoneyList__textBox->TabIndex = 24;
			// 
			// MoneyXMLPrefix_textBox
			// 
			this->MoneyXMLPrefix_textBox->Location = System::Drawing::Point(397, 222);
			this->MoneyXMLPrefix_textBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->MoneyXMLPrefix_textBox->Name = L"MoneyXMLPrefix_textBox";
			this->MoneyXMLPrefix_textBox->Size = System::Drawing::Size(260, 38);
			this->MoneyXMLPrefix_textBox->TabIndex = 25;
			// 
			// MoneyXMLPrefix_label
			// 
			this->MoneyXMLPrefix_label->AutoSize = true;
			this->MoneyXMLPrefix_label->Location = System::Drawing::Point(400, 181);
			this->MoneyXMLPrefix_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->MoneyXMLPrefix_label->Name = L"MoneyXMLPrefix_label";
			this->MoneyXMLPrefix_label->Size = System::Drawing::Size(146, 32);
			this->MoneyXMLPrefix_label->TabIndex = 26;
			this->MoneyXMLPrefix_label->Text = L"XMLPrefix";
			// 
			// MoneyRound_checkBox
			// 
			this->MoneyRound_checkBox->AutoSize = true;
			this->MoneyRound_checkBox->Checked = true;
			this->MoneyRound_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->MoneyRound_checkBox->Location = System::Drawing::Point(405, 52);
			this->MoneyRound_checkBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->MoneyRound_checkBox->Name = L"MoneyRound_checkBox";
			this->MoneyRound_checkBox->Size = System::Drawing::Size(137, 36);
			this->MoneyRound_checkBox->TabIndex = 23;
			this->MoneyRound_checkBox->Text = L"Round";
			this->MoneyRound_checkBox->UseVisualStyleBackColor = true;
			// 
			// DailySalary_label
			// 
			this->DailySalary_label->AutoSize = true;
			this->DailySalary_label->Location = System::Drawing::Point(16, 38);
			this->DailySalary_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->DailySalary_label->Name = L"DailySalary_label";
			this->DailySalary_label->Size = System::Drawing::Size(63, 32);
			this->DailySalary_label->TabIndex = 13;
			this->DailySalary_label->Text = L"100";
			// 
			// MoneyRound_numericUpDown
			// 
			this->MoneyRound_numericUpDown->Location = System::Drawing::Point(400, 119);
			this->MoneyRound_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->MoneyRound_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->MoneyRound_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->MoneyRound_numericUpDown->Name = L"MoneyRound_numericUpDown";
			this->MoneyRound_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->MoneyRound_numericUpDown->TabIndex = 22;
			this->MoneyRound_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {60, 0, 0, 0});
			// 
			// OneRMBCostGems_numericUpDown
			// 
			this->OneRMBCostGems_numericUpDown->Location = System::Drawing::Point(16, 341);
			this->OneRMBCostGems_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->OneRMBCostGems_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->OneRMBCostGems_numericUpDown->Name = L"OneRMBCostGems_numericUpDown";
			this->OneRMBCostGems_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->OneRMBCostGems_numericUpDown->TabIndex = 5;
			this->OneRMBCostGems_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {80, 0, 0, 0});
			this->OneRMBCostGems_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown1_ValueChanged);
			// 
			// GemPrice_label
			// 
			this->GemPrice_label->AutoSize = true;
			this->GemPrice_label->Location = System::Drawing::Point(13, 181);
			this->GemPrice_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->GemPrice_label->Name = L"GemPrice_label";
			this->GemPrice_label->Size = System::Drawing::Size(141, 32);
			this->GemPrice_label->TabIndex = 15;
			this->GemPrice_label->Text = L"GemPrice";
			// 
			// TargetSceonds_numericUpDown
			// 
			this->TargetSceonds_numericUpDown->Location = System::Drawing::Point(16, 444);
			this->TargetSceonds_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->TargetSceonds_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->TargetSceonds_numericUpDown->Name = L"TargetSceonds_numericUpDown";
			this->TargetSceonds_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->TargetSceonds_numericUpDown->TabIndex = 6;
			this->TargetSceonds_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {60, 0, 0, 0});
			this->TargetSceonds_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown1_ValueChanged);
			// 
			// GemPrice_numericUpDown
			// 
			this->GemPrice_numericUpDown->Location = System::Drawing::Point(19, 231);
			this->GemPrice_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->GemPrice_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->GemPrice_numericUpDown->Name = L"GemPrice_numericUpDown";
			this->GemPrice_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->GemPrice_numericUpDown->TabIndex = 14;
			this->GemPrice_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {6, 0, 0, 0});
			this->GemPrice_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown1_ValueChanged);
			// 
			// OneRMBCostGems_label
			// 
			this->OneRMBCostGems_label->AutoSize = true;
			this->OneRMBCostGems_label->Location = System::Drawing::Point(16, 298);
			this->OneRMBCostGems_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->OneRMBCostGems_label->Name = L"OneRMBCostGems_label";
			this->OneRMBCostGems_label->Size = System::Drawing::Size(264, 32);
			this->OneRMBCostGems_label->TabIndex = 8;
			this->OneRMBCostGems_label->Text = L"OneRMBCostGems";
			// 
			// TargetSeconds_label
			// 
			this->TargetSeconds_label->AutoSize = true;
			this->TargetSeconds_label->Location = System::Drawing::Point(16, 405);
			this->TargetSeconds_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->TargetSeconds_label->Name = L"TargetSeconds_label";
			this->TargetSeconds_label->Size = System::Drawing::Size(208, 32);
			this->TargetSeconds_label->TabIndex = 9;
			this->TargetSeconds_label->Text = L"TargetSeconds";
			// 
			// AHourSalary_label
			// 
			this->AHourSalary_label->AutoSize = true;
			this->AHourSalary_label->Location = System::Drawing::Point(16, 83);
			this->AHourSalary_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->AHourSalary_label->Name = L"AHourSalary_label";
			this->AHourSalary_label->Size = System::Drawing::Size(225, 32);
			this->AHourSalary_label->TabIndex = 12;
			this->AHourSalary_label->Text = L"One Hour Salary";
			// 
			// TimeToGems_textBox
			// 
			this->TimeToGems_textBox->Location = System::Drawing::Point(19, 506);
			this->TimeToGems_textBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->TimeToGems_textBox->Multiline = true;
			this->TimeToGems_textBox->Name = L"TimeToGems_textBox";
			this->TimeToGems_textBox->Size = System::Drawing::Size(305, 266);
			this->TimeToGems_textBox->TabIndex = 10;
			// 
			// AHourSalary_numericUpDown
			// 
			this->AHourSalary_numericUpDown->DecimalPlaces = 1;
			this->AHourSalary_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->AHourSalary_numericUpDown->Location = System::Drawing::Point(16, 122);
			this->AHourSalary_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->AHourSalary_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, 0});
			this->AHourSalary_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->AHourSalary_numericUpDown->Name = L"AHourSalary_numericUpDown";
			this->AHourSalary_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->AHourSalary_numericUpDown->TabIndex = 11;
			this->AHourSalary_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {125, 0, 0, 65536});
			this->AHourSalary_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown1_ValueChanged);
			// 
			// MonsterRender_checkBox
			// 
			this->MonsterRender_checkBox->AutoSize = true;
			this->MonsterRender_checkBox->Checked = true;
			this->MonsterRender_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->MonsterRender_checkBox->Location = System::Drawing::Point(288, 1102);
			this->MonsterRender_checkBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->MonsterRender_checkBox->Name = L"MonsterRender_checkBox";
			this->MonsterRender_checkBox->Size = System::Drawing::Size(146, 36);
			this->MonsterRender_checkBox->TabIndex = 4;
			this->MonsterRender_checkBox->Text = L"Render";
			this->MonsterRender_checkBox->UseVisualStyleBackColor = true;
			// 
			// MonsterShowValue_checkBox
			// 
			this->MonsterShowValue_checkBox->AutoSize = true;
			this->MonsterShowValue_checkBox->Location = System::Drawing::Point(59, 1102);
			this->MonsterShowValue_checkBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->MonsterShowValue_checkBox->Name = L"MonsterShowValue_checkBox";
			this->MonsterShowValue_checkBox->Size = System::Drawing::Size(198, 36);
			this->MonsterShowValue_checkBox->TabIndex = 2;
			this->MonsterShowValue_checkBox->Text = L"ShowValue";
			this->MonsterShowValue_checkBox->UseVisualStyleBackColor = true;
			this->MonsterShowValue_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::MonsterShowValue_checkBox_CheckedChanged);
			// 
			// MonsterStatusShowCurve_listBox
			// 
			this->MonsterStatusShowCurve_listBox->FormattingEnabled = true;
			this->MonsterStatusShowCurve_listBox->ItemHeight = 31;
			this->MonsterStatusShowCurve_listBox->Items->AddRange(gcnew cli::array< System::Object^  >(8) {L"HP", L"STR", L"Stamina", 
				L"Exp", L"CoinCost", L"TimeCost", L"CrystalCost", L"Skill CD"});
			this->MonsterStatusShowCurve_listBox->Location = System::Drawing::Point(16, 734);
			this->MonsterStatusShowCurve_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->MonsterStatusShowCurve_listBox->Name = L"MonsterStatusShowCurve_listBox";
			this->MonsterStatusShowCurve_listBox->Size = System::Drawing::Size(705, 345);
			this->MonsterStatusShowCurve_listBox->TabIndex = 1;
			this->MonsterStatusShowCurve_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::MonsterStatusShowCurve_listBox_SelectedIndexChanged);
			// 
			// MonsterData_listBox
			// 
			this->MonsterData_listBox->FormattingEnabled = true;
			this->MonsterData_listBox->ItemHeight = 31;
			this->MonsterData_listBox->Location = System::Drawing::Point(8, 7);
			this->MonsterData_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->MonsterData_listBox->Name = L"MonsterData_listBox";
			this->MonsterData_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->MonsterData_listBox->Size = System::Drawing::Size(748, 686);
			this->MonsterData_listBox->TabIndex = 0;
			this->MonsterData_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::MonsterData_listBox_SelectedIndexChanged);
			// 
			// Enemy_tabPage
			// 
			this->Enemy_tabPage->Controls->Add(this->EnemyRender_checkBox);
			this->Enemy_tabPage->Controls->Add(this->EnemyShowValue_checkBox);
			this->Enemy_tabPage->Controls->Add(this->EnemyStatus_listBox);
			this->Enemy_tabPage->Controls->Add(this->EnemyData_listBox);
			this->Enemy_tabPage->Location = System::Drawing::Point(10, 48);
			this->Enemy_tabPage->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Enemy_tabPage->Name = L"Enemy_tabPage";
			this->Enemy_tabPage->Padding = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Enemy_tabPage->Size = System::Drawing::Size(2596, 1950);
			this->Enemy_tabPage->TabIndex = 1;
			this->Enemy_tabPage->Text = L"Enemy";
			this->Enemy_tabPage->UseVisualStyleBackColor = true;
			// 
			// EnemyRender_checkBox
			// 
			this->EnemyRender_checkBox->AutoSize = true;
			this->EnemyRender_checkBox->Checked = true;
			this->EnemyRender_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->EnemyRender_checkBox->Location = System::Drawing::Point(432, 1521);
			this->EnemyRender_checkBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->EnemyRender_checkBox->Name = L"EnemyRender_checkBox";
			this->EnemyRender_checkBox->Size = System::Drawing::Size(146, 36);
			this->EnemyRender_checkBox->TabIndex = 5;
			this->EnemyRender_checkBox->Text = L"Render";
			this->EnemyRender_checkBox->UseVisualStyleBackColor = true;
			// 
			// EnemyShowValue_checkBox
			// 
			this->EnemyShowValue_checkBox->AutoSize = true;
			this->EnemyShowValue_checkBox->Location = System::Drawing::Point(133, 1521);
			this->EnemyShowValue_checkBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->EnemyShowValue_checkBox->Name = L"EnemyShowValue_checkBox";
			this->EnemyShowValue_checkBox->Size = System::Drawing::Size(198, 36);
			this->EnemyShowValue_checkBox->TabIndex = 3;
			this->EnemyShowValue_checkBox->Text = L"ShowValue";
			this->EnemyShowValue_checkBox->UseVisualStyleBackColor = true;
			this->EnemyShowValue_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::EnemyShowValue_checkBox_CheckedChanged);
			// 
			// EnemyStatus_listBox
			// 
			this->EnemyStatus_listBox->FormattingEnabled = true;
			this->EnemyStatus_listBox->ItemHeight = 31;
			this->EnemyStatus_listBox->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"HP", L"STR", L"TIME_ADD", L"TIME_MINUS", 
				L"SPEED", L"CLOSE_SPEED", L"SKILL_TIME_REDUCE"});
			this->EnemyStatus_listBox->Location = System::Drawing::Point(8, 937);
			this->EnemyStatus_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->EnemyStatus_listBox->Name = L"EnemyStatus_listBox";
			this->EnemyStatus_listBox->Size = System::Drawing::Size(732, 531);
			this->EnemyStatus_listBox->TabIndex = 2;
			this->EnemyStatus_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::EnemyStatus_listBox_SelectedIndexChanged);
			// 
			// EnemyData_listBox
			// 
			this->EnemyData_listBox->FormattingEnabled = true;
			this->EnemyData_listBox->ItemHeight = 31;
			this->EnemyData_listBox->Location = System::Drawing::Point(8, 14);
			this->EnemyData_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->EnemyData_listBox->Name = L"EnemyData_listBox";
			this->EnemyData_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->EnemyData_listBox->Size = System::Drawing::Size(748, 903);
			this->EnemyData_listBox->TabIndex = 1;
			this->EnemyData_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::EnemyData_listBox_SelectedIndexChanged);
			// 
			// Shop
			// 
			this->Shop->Controls->Add(this->Item_listBox);
			this->Shop->Controls->Add(this->ShopShowValue_checkBox);
			this->Shop->Controls->Add(this->ShopStatus_listBox);
			this->Shop->Controls->Add(this->ShopData_listBox);
			this->Shop->Location = System::Drawing::Point(10, 48);
			this->Shop->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Shop->Name = L"Shop";
			this->Shop->Size = System::Drawing::Size(2596, 1950);
			this->Shop->TabIndex = 2;
			this->Shop->Text = L"Shop";
			this->Shop->UseVisualStyleBackColor = true;
			// 
			// Item_listBox
			// 
			this->Item_listBox->FormattingEnabled = true;
			this->Item_listBox->ItemHeight = 31;
			this->Item_listBox->Location = System::Drawing::Point(8, 610);
			this->Item_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Item_listBox->Name = L"Item_listBox";
			this->Item_listBox->Size = System::Drawing::Size(713, 903);
			this->Item_listBox->TabIndex = 15;
			// 
			// ShopShowValue_checkBox
			// 
			this->ShopShowValue_checkBox->AutoSize = true;
			this->ShopShowValue_checkBox->Location = System::Drawing::Point(112, 1943);
			this->ShopShowValue_checkBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->ShopShowValue_checkBox->Name = L"ShopShowValue_checkBox";
			this->ShopShowValue_checkBox->Size = System::Drawing::Size(198, 36);
			this->ShopShowValue_checkBox->TabIndex = 2;
			this->ShopShowValue_checkBox->Text = L"ShowValue";
			this->ShopShowValue_checkBox->UseVisualStyleBackColor = true;
			this->ShopShowValue_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::ShopShowValue_checkBox_CheckedChanged);
			// 
			// ShopStatus_listBox
			// 
			this->ShopStatus_listBox->FormattingEnabled = true;
			this->ShopStatus_listBox->ItemHeight = 31;
			this->ShopStatus_listBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Price", L"Attribute"});
			this->ShopStatus_listBox->Location = System::Drawing::Point(8, 1576);
			this->ShopStatus_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->ShopStatus_listBox->Name = L"ShopStatus_listBox";
			this->ShopStatus_listBox->Size = System::Drawing::Size(764, 345);
			this->ShopStatus_listBox->TabIndex = 1;
			// 
			// ShopData_listBox
			// 
			this->ShopData_listBox->FormattingEnabled = true;
			this->ShopData_listBox->ItemHeight = 31;
			this->ShopData_listBox->Location = System::Drawing::Point(8, 7);
			this->ShopData_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->ShopData_listBox->Name = L"ShopData_listBox";
			this->ShopData_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiSimple;
			this->ShopData_listBox->Size = System::Drawing::Size(764, 562);
			this->ShopData_listBox->TabIndex = 0;
			// 
			// NewCurve_tabPage
			// 
			this->NewCurve_tabPage->Controls->Add(this->GridentWithPrevious_checkBox);
			this->NewCurve_tabPage->Controls->Add(this->Round_checkBox);
			this->NewCurve_tabPage->Controls->Add(this->Result_textBox);
			this->NewCurve_tabPage->Controls->Add(this->Number_label);
			this->NewCurve_tabPage->Controls->Add(this->Round_numericUpDown);
			this->NewCurve_tabPage->Controls->Add(this->Number_numericUpDown);
			this->NewCurve_tabPage->Controls->Add(this->XMLPrefix_textBox);
			this->NewCurve_tabPage->Controls->Add(this->Steep_label);
			this->NewCurve_tabPage->Controls->Add(this->XMLPrefix_label);
			this->NewCurve_tabPage->Controls->Add(this->Grident_label);
			this->NewCurve_tabPage->Controls->Add(this->StepValue_label);
			this->NewCurve_tabPage->Controls->Add(this->StartPos_label);
			this->NewCurve_tabPage->Controls->Add(this->StepY_numericUpDown);
			this->NewCurve_tabPage->Controls->Add(this->StepX_numericUpDown);
			this->NewCurve_tabPage->Controls->Add(this->StartPosY_numericUpDown);
			this->NewCurve_tabPage->Controls->Add(this->StartPosX_numericUpDown);
			this->NewCurve_tabPage->Controls->Add(this->Grident_numericUpDown);
			this->NewCurve_tabPage->Controls->Add(this->Steep_numericUpDown);
			this->NewCurve_tabPage->Location = System::Drawing::Point(10, 48);
			this->NewCurve_tabPage->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->NewCurve_tabPage->Name = L"NewCurve_tabPage";
			this->NewCurve_tabPage->Size = System::Drawing::Size(2665, 2102);
			this->NewCurve_tabPage->TabIndex = 2;
			this->NewCurve_tabPage->Text = L"NewCurve";
			this->NewCurve_tabPage->UseVisualStyleBackColor = true;
			// 
			// GridentWithPrevious_checkBox
			// 
			this->GridentWithPrevious_checkBox->AutoSize = true;
			this->GridentWithPrevious_checkBox->Checked = true;
			this->GridentWithPrevious_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->GridentWithPrevious_checkBox->Location = System::Drawing::Point(480, 405);
			this->GridentWithPrevious_checkBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->GridentWithPrevious_checkBox->Name = L"GridentWithPrevious_checkBox";
			this->GridentWithPrevious_checkBox->Size = System::Drawing::Size(315, 36);
			this->GridentWithPrevious_checkBox->TabIndex = 22;
			this->GridentWithPrevious_checkBox->Text = L"GridentWithPrevious";
			this->GridentWithPrevious_checkBox->UseVisualStyleBackColor = true;
			this->GridentWithPrevious_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::GridentWithPrevious_checkBox_CheckedChanged);
			// 
			// Round_checkBox
			// 
			this->Round_checkBox->AutoSize = true;
			this->Round_checkBox->Location = System::Drawing::Point(488, 577);
			this->Round_checkBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Round_checkBox->Name = L"Round_checkBox";
			this->Round_checkBox->Size = System::Drawing::Size(137, 36);
			this->Round_checkBox->TabIndex = 21;
			this->Round_checkBox->Text = L"Round";
			this->Round_checkBox->UseVisualStyleBackColor = true;
			this->Round_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::GridentWithPrevious_checkBox_CheckedChanged);
			// 
			// Result_textBox
			// 
			this->Result_textBox->Location = System::Drawing::Point(72, 951);
			this->Result_textBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Result_textBox->Multiline = true;
			this->Result_textBox->Name = L"Result_textBox";
			this->Result_textBox->Size = System::Drawing::Size(313, 698);
			this->Result_textBox->TabIndex = 12;
			// 
			// Number_label
			// 
			this->Number_label->AutoSize = true;
			this->Number_label->Location = System::Drawing::Point(64, 69);
			this->Number_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->Number_label->Name = L"Number_label";
			this->Number_label->Size = System::Drawing::Size(115, 32);
			this->Number_label->TabIndex = 11;
			this->Number_label->Text = L"Number";
			// 
			// Round_numericUpDown
			// 
			this->Round_numericUpDown->Location = System::Drawing::Point(483, 644);
			this->Round_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Round_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->Round_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->Round_numericUpDown->Name = L"Round_numericUpDown";
			this->Round_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->Round_numericUpDown->TabIndex = 19;
			this->Round_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {60, 0, 0, 0});
			this->Round_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::Round_numericUpDown_ValueChanged);
			// 
			// Number_numericUpDown
			// 
			this->Number_numericUpDown->Location = System::Drawing::Point(72, 129);
			this->Number_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Number_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999, 0, 0, 0});
			this->Number_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->Number_numericUpDown->Name = L"Number_numericUpDown";
			this->Number_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->Number_numericUpDown->TabIndex = 10;
			this->Number_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->Number_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::StartPosX_numericUpDown_ValueChanged);
			// 
			// XMLPrefix_textBox
			// 
			this->XMLPrefix_textBox->Location = System::Drawing::Point(485, 754);
			this->XMLPrefix_textBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->XMLPrefix_textBox->Name = L"XMLPrefix_textBox";
			this->XMLPrefix_textBox->Size = System::Drawing::Size(260, 38);
			this->XMLPrefix_textBox->TabIndex = 17;
			// 
			// Steep_label
			// 
			this->Steep_label->AutoSize = true;
			this->Steep_label->Location = System::Drawing::Point(64, 849);
			this->Steep_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->Steep_label->Name = L"Steep_label";
			this->Steep_label->Size = System::Drawing::Size(90, 32);
			this->Steep_label->TabIndex = 9;
			this->Steep_label->Text = L"Steep";
			// 
			// XMLPrefix_label
			// 
			this->XMLPrefix_label->AutoSize = true;
			this->XMLPrefix_label->Location = System::Drawing::Point(488, 699);
			this->XMLPrefix_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->XMLPrefix_label->Name = L"XMLPrefix_label";
			this->XMLPrefix_label->Size = System::Drawing::Size(146, 32);
			this->XMLPrefix_label->TabIndex = 18;
			this->XMLPrefix_label->Text = L"XMLPrefix";
			// 
			// Grident_label
			// 
			this->Grident_label->AutoSize = true;
			this->Grident_label->Location = System::Drawing::Point(64, 756);
			this->Grident_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->Grident_label->Name = L"Grident_label";
			this->Grident_label->Size = System::Drawing::Size(109, 32);
			this->Grident_label->TabIndex = 8;
			this->Grident_label->Text = L"Grident";
			// 
			// StepValue_label
			// 
			this->StepValue_label->AutoSize = true;
			this->StepValue_label->Location = System::Drawing::Point(64, 484);
			this->StepValue_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->StepValue_label->Name = L"StepValue_label";
			this->StepValue_label->Size = System::Drawing::Size(148, 32);
			this->StepValue_label->TabIndex = 7;
			this->StepValue_label->Text = L"StepValue";
			// 
			// StartPos_label
			// 
			this->StartPos_label->AutoSize = true;
			this->StartPos_label->Location = System::Drawing::Point(64, 284);
			this->StartPos_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->StartPos_label->Name = L"StartPos_label";
			this->StartPos_label->Size = System::Drawing::Size(124, 32);
			this->StartPos_label->TabIndex = 6;
			this->StartPos_label->Text = L"StartPos";
			// 
			// StepY_numericUpDown
			// 
			this->StepY_numericUpDown->DecimalPlaces = 2;
			this->StepY_numericUpDown->Location = System::Drawing::Point(72, 634);
			this->StepY_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->StepY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->StepY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->StepY_numericUpDown->Name = L"StepY_numericUpDown";
			this->StepY_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->StepY_numericUpDown->TabIndex = 5;
			this->StepY_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->StepY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::StartPosX_numericUpDown_ValueChanged);
			// 
			// StepX_numericUpDown
			// 
			this->StepX_numericUpDown->DecimalPlaces = 2;
			this->StepX_numericUpDown->Location = System::Drawing::Point(72, 551);
			this->StepX_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->StepX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->StepX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->StepX_numericUpDown->Name = L"StepX_numericUpDown";
			this->StepX_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->StepX_numericUpDown->TabIndex = 4;
			this->StepX_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->StepX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::StartPosX_numericUpDown_ValueChanged);
			// 
			// StartPosY_numericUpDown
			// 
			this->StartPosY_numericUpDown->DecimalPlaces = 2;
			this->StartPosY_numericUpDown->Location = System::Drawing::Point(72, 405);
			this->StartPosY_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->StartPosY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->StartPosY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->StartPosY_numericUpDown->Name = L"StartPosY_numericUpDown";
			this->StartPosY_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->StartPosY_numericUpDown->TabIndex = 3;
			this->StartPosY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::StartPosX_numericUpDown_ValueChanged);
			// 
			// StartPosX_numericUpDown
			// 
			this->StartPosX_numericUpDown->DecimalPlaces = 2;
			this->StartPosX_numericUpDown->Location = System::Drawing::Point(72, 322);
			this->StartPosX_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->StartPosX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, 0});
			this->StartPosX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999, 0, 0, System::Int32::MinValue});
			this->StartPosX_numericUpDown->Name = L"StartPosX_numericUpDown";
			this->StartPosX_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->StartPosX_numericUpDown->TabIndex = 2;
			this->StartPosX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::StartPosX_numericUpDown_ValueChanged);
			// 
			// Grident_numericUpDown
			// 
			this->Grident_numericUpDown->DecimalPlaces = 4;
			this->Grident_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->Grident_numericUpDown->Location = System::Drawing::Point(72, 794);
			this->Grident_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Grident_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->Grident_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->Grident_numericUpDown->Name = L"Grident_numericUpDown";
			this->Grident_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->Grident_numericUpDown->TabIndex = 1;
			this->Grident_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->Grident_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::StartPosX_numericUpDown_ValueChanged);
			// 
			// Steep_numericUpDown
			// 
			this->Steep_numericUpDown->DecimalPlaces = 4;
			this->Steep_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 196608});
			this->Steep_numericUpDown->Location = System::Drawing::Point(72, 889);
			this->Steep_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Steep_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->Steep_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, System::Int32::MinValue});
			this->Steep_numericUpDown->Name = L"Steep_numericUpDown";
			this->Steep_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->Steep_numericUpDown->TabIndex = 0;
			this->Steep_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::StartPosX_numericUpDown_ValueChanged);
			// 
			// ItemProbability_tabPage
			// 
			this->ItemProbability_tabPage->Controls->Add(this->Probability_tabControl);
			this->ItemProbability_tabPage->Controls->Add(this->CoinToGem_label);
			this->ItemProbability_tabPage->Controls->Add(this->HeartToGems_label);
			this->ItemProbability_tabPage->Controls->Add(this->CoinToGems_numericUpDown);
			this->ItemProbability_tabPage->Controls->Add(this->GemPriceToProbabilityGenerate_button);
			this->ItemProbability_tabPage->Controls->Add(this->HeartToGems_numericUpDown);
			this->ItemProbability_tabPage->Location = System::Drawing::Point(10, 48);
			this->ItemProbability_tabPage->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->ItemProbability_tabPage->Name = L"ItemProbability_tabPage";
			this->ItemProbability_tabPage->Size = System::Drawing::Size(2665, 2102);
			this->ItemProbability_tabPage->TabIndex = 3;
			this->ItemProbability_tabPage->Text = L"ItemProbability";
			this->ItemProbability_tabPage->UseVisualStyleBackColor = true;
			// 
			// Probability_tabControl
			// 
			this->Probability_tabControl->Controls->Add(this->BadPool_tabPage);
			this->Probability_tabControl->Controls->Add(this->GoodPool_tabPage);
			this->Probability_tabControl->Location = System::Drawing::Point(27, 17);
			this->Probability_tabControl->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Probability_tabControl->Name = L"Probability_tabControl";
			this->Probability_tabControl->SelectedIndex = 0;
			this->Probability_tabControl->Size = System::Drawing::Size(2549, 1352);
			this->Probability_tabControl->TabIndex = 0;
			// 
			// BadPool_tabPage
			// 
			this->BadPool_tabPage->Controls->Add(this->BadItemScore_label);
			this->BadPool_tabPage->Controls->Add(this->BadItemScore_numericUpDown);
			this->BadPool_tabPage->Controls->Add(this->BadItemProbability_listBox);
			this->BadPool_tabPage->Controls->Add(this->BadItemProbability_textBox);
			this->BadPool_tabPage->Location = System::Drawing::Point(10, 48);
			this->BadPool_tabPage->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->BadPool_tabPage->Name = L"BadPool_tabPage";
			this->BadPool_tabPage->Padding = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->BadPool_tabPage->Size = System::Drawing::Size(2529, 1294);
			this->BadPool_tabPage->TabIndex = 1;
			this->BadPool_tabPage->Text = L"BadPool";
			this->BadPool_tabPage->UseVisualStyleBackColor = true;
			// 
			// BadItemScore_label
			// 
			this->BadItemScore_label->AutoSize = true;
			this->BadItemScore_label->Location = System::Drawing::Point(2285, 284);
			this->BadItemScore_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->BadItemScore_label->Name = L"BadItemScore_label";
			this->BadItemScore_label->Size = System::Drawing::Size(169, 32);
			this->BadItemScore_label->TabIndex = 7;
			this->BadItemScore_label->Text = L"SelectScore";
			// 
			// BadItemScore_numericUpDown
			// 
			this->BadItemScore_numericUpDown->Location = System::Drawing::Point(2272, 322);
			this->BadItemScore_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->BadItemScore_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000000, 0, 0, 0});
			this->BadItemScore_numericUpDown->Name = L"BadItemScore_numericUpDown";
			this->BadItemScore_numericUpDown->Size = System::Drawing::Size(227, 38);
			this->BadItemScore_numericUpDown->TabIndex = 6;
			this->BadItemScore_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::BadItemScore_numericUpDown_ValueChanged);
			// 
			// BadItemProbability_listBox
			// 
			this->BadItemProbability_listBox->FormattingEnabled = true;
			this->BadItemProbability_listBox->ItemHeight = 31;
			this->BadItemProbability_listBox->Location = System::Drawing::Point(0, 12);
			this->BadItemProbability_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->BadItemProbability_listBox->Name = L"BadItemProbability_listBox";
			this->BadItemProbability_listBox->ScrollAlwaysVisible = true;
			this->BadItemProbability_listBox->Size = System::Drawing::Size(2249, 1058);
			this->BadItemProbability_listBox->TabIndex = 4;
			this->BadItemProbability_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::BadItemProbability_listBox_SelectedIndexChanged);
			// 
			// BadItemProbability_textBox
			// 
			this->BadItemProbability_textBox->Location = System::Drawing::Point(11, 1090);
			this->BadItemProbability_textBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->BadItemProbability_textBox->Multiline = true;
			this->BadItemProbability_textBox->Name = L"BadItemProbability_textBox";
			this->BadItemProbability_textBox->Size = System::Drawing::Size(2239, 245);
			this->BadItemProbability_textBox->TabIndex = 5;
			// 
			// GoodPool_tabPage
			// 
			this->GoodPool_tabPage->Controls->Add(this->label1);
			this->GoodPool_tabPage->Controls->Add(this->GoodItemScore_numericUpDown);
			this->GoodPool_tabPage->Controls->Add(this->GoodItemProbability_listBox);
			this->GoodPool_tabPage->Controls->Add(this->GoodItemProbability_textBox);
			this->GoodPool_tabPage->Location = System::Drawing::Point(10, 48);
			this->GoodPool_tabPage->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->GoodPool_tabPage->Name = L"GoodPool_tabPage";
			this->GoodPool_tabPage->Padding = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->GoodPool_tabPage->Size = System::Drawing::Size(2529, 1294);
			this->GoodPool_tabPage->TabIndex = 0;
			this->GoodPool_tabPage->Text = L"GoodPool";
			this->GoodPool_tabPage->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(2288, 410);
			this->label1->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(169, 32);
			this->label1->TabIndex = 3;
			this->label1->Text = L"SelectScore";
			// 
			// GoodItemScore_numericUpDown
			// 
			this->GoodItemScore_numericUpDown->Location = System::Drawing::Point(2275, 448);
			this->GoodItemScore_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->GoodItemScore_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000000, 0, 0, 0});
			this->GoodItemScore_numericUpDown->Name = L"GoodItemScore_numericUpDown";
			this->GoodItemScore_numericUpDown->Size = System::Drawing::Size(227, 38);
			this->GoodItemScore_numericUpDown->TabIndex = 2;
			this->GoodItemScore_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::BadItemScore_numericUpDown_ValueChanged);
			// 
			// GoodItemProbability_listBox
			// 
			this->GoodItemProbability_listBox->FormattingEnabled = true;
			this->GoodItemProbability_listBox->ItemHeight = 31;
			this->GoodItemProbability_listBox->Location = System::Drawing::Point(16, 0);
			this->GoodItemProbability_listBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->GoodItemProbability_listBox->Name = L"GoodItemProbability_listBox";
			this->GoodItemProbability_listBox->Size = System::Drawing::Size(2249, 1058);
			this->GoodItemProbability_listBox->TabIndex = 0;
			this->GoodItemProbability_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::BadItemProbability_listBox_SelectedIndexChanged);
			// 
			// GoodItemProbability_textBox
			// 
			this->GoodItemProbability_textBox->Location = System::Drawing::Point(16, 1078);
			this->GoodItemProbability_textBox->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->GoodItemProbability_textBox->Multiline = true;
			this->GoodItemProbability_textBox->Name = L"GoodItemProbability_textBox";
			this->GoodItemProbability_textBox->Size = System::Drawing::Size(2249, 190);
			this->GoodItemProbability_textBox->TabIndex = 1;
			// 
			// CoinToGem_label
			// 
			this->CoinToGem_label->AutoSize = true;
			this->CoinToGem_label->Location = System::Drawing::Point(35, 1591);
			this->CoinToGem_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->CoinToGem_label->Name = L"CoinToGem_label";
			this->CoinToGem_label->Size = System::Drawing::Size(168, 32);
			this->CoinToGem_label->TabIndex = 0;
			this->CoinToGem_label->Text = L"CoinToGem";
			// 
			// HeartToGems_label
			// 
			this->HeartToGems_label->AutoSize = true;
			this->HeartToGems_label->Location = System::Drawing::Point(27, 1514);
			this->HeartToGems_label->Margin = System::Windows::Forms::Padding(8, 0, 8, 0);
			this->HeartToGems_label->Name = L"HeartToGems_label";
			this->HeartToGems_label->Size = System::Drawing::Size(178, 32);
			this->HeartToGems_label->TabIndex = 0;
			this->HeartToGems_label->Text = L"HeartToGem";
			// 
			// CoinToGems_numericUpDown
			// 
			this->CoinToGems_numericUpDown->Location = System::Drawing::Point(248, 1586);
			this->CoinToGems_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->CoinToGems_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000000, 0, 0, 0});
			this->CoinToGems_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->CoinToGems_numericUpDown->Name = L"CoinToGems_numericUpDown";
			this->CoinToGems_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->CoinToGems_numericUpDown->TabIndex = 1;
			this->CoinToGems_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
			// 
			// GemPriceToProbabilityGenerate_button
			// 
			this->GemPriceToProbabilityGenerate_button->Location = System::Drawing::Point(37, 1383);
			this->GemPriceToProbabilityGenerate_button->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->GemPriceToProbabilityGenerate_button->Name = L"GemPriceToProbabilityGenerate_button";
			this->GemPriceToProbabilityGenerate_button->Size = System::Drawing::Size(557, 107);
			this->GemPriceToProbabilityGenerate_button->TabIndex = 2;
			this->GemPriceToProbabilityGenerate_button->Text = L"GemPriceToProbabilityGenerate";
			this->GemPriceToProbabilityGenerate_button->UseVisualStyleBackColor = true;
			this->GemPriceToProbabilityGenerate_button->Click += gcnew System::EventHandler(this, &Form1::GemPriceToProbabilityGenerate_button_Click);
			// 
			// HeartToGems_numericUpDown
			// 
			this->HeartToGems_numericUpDown->Location = System::Drawing::Point(248, 1505);
			this->HeartToGems_numericUpDown->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->HeartToGems_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000000, 0, 0, 0});
			this->HeartToGems_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->HeartToGems_numericUpDown->Name = L"HeartToGems_numericUpDown";
			this->HeartToGems_numericUpDown->Size = System::Drawing::Size(320, 38);
			this->HeartToGems_numericUpDown->TabIndex = 0;
			this->HeartToGems_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(336, 91);
			this->button1->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(312, 69);
			this->button1->TabIndex = 8;
			this->button1->Text = L"OpenData Directory";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click_1);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->CaemraReset_button);
			this->splitContainer1->Panel1->Controls->Add(this->button1);
			this->splitContainer1->Panel1->Controls->Add(this->FileName_textBox);
			this->splitContainer1->Panel1->Controls->Add(this->OpenFile_button);
			this->splitContainer1->Panel1->Controls->Add(this->Main_tabControl);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->OpemnGL_panel);
			this->splitContainer1->Size = System::Drawing::Size(3824, 2088);
			this->splitContainer1->SplitterDistance = 2143;
			this->splitContainer1->SplitterWidth = 11;
			this->splitContainer1->TabIndex = 8;
			// 
			// CaemraReset_button
			// 
			this->CaemraReset_button->Location = System::Drawing::Point(667, 103);
			this->CaemraReset_button->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->CaemraReset_button->Name = L"CaemraReset_button";
			this->CaemraReset_button->Size = System::Drawing::Size(200, 55);
			this->CaemraReset_button->TabIndex = 9;
			this->CaemraReset_button->Text = L"CaemraReset";
			this->CaemraReset_button->UseVisualStyleBackColor = true;
			this->CaemraReset_button->Click += gcnew System::EventHandler(this, &Form1::CaemraReset_button_Click);
			// 
			// Form1
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(16, 31);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(3824, 2088);
			this->Controls->Add(this->splitContainer1);
			this->KeyPreview = true;
			this->Margin = System::Windows::Forms::Padding(8, 7, 8, 7);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			this->Main_tabControl->ResumeLayout(false);
			this->Gameplay_tabPage->ResumeLayout(false);
			this->Gameplay_tabPage->PerformLayout();
			this->GameData_tabPage->ResumeLayout(false);
			this->GameData_tabControl->ResumeLayout(false);
			this->Monster_tabPage->ResumeLayout(false);
			this->Monster_tabPage->PerformLayout();
			this->TimeMoneyExchange_groupBox->ResumeLayout(false);
			this->TimeMoneyExchange_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MoneyRound_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->OneRMBCostGems_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TargetSceonds_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GemPrice_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AHourSalary_numericUpDown))->EndInit();
			this->Enemy_tabPage->ResumeLayout(false);
			this->Enemy_tabPage->PerformLayout();
			this->Shop->ResumeLayout(false);
			this->Shop->PerformLayout();
			this->NewCurve_tabPage->ResumeLayout(false);
			this->NewCurve_tabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Round_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Number_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StepY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StepX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartPosY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartPosX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Grident_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Steep_numericUpDown))->EndInit();
			this->ItemProbability_tabPage->ResumeLayout(false);
			this->ItemProbability_tabPage->PerformLayout();
			this->Probability_tabControl->ResumeLayout(false);
			this->BadPool_tabPage->ResumeLayout(false);
			this->BadPool_tabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->BadItemScore_numericUpDown))->EndInit();
			this->GoodPool_tabPage->ResumeLayout(false);
			this->GoodPool_tabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GoodItemScore_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CoinToGems_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->HeartToGems_numericUpDown))->EndInit();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e)
		 {
		 }
private:System::Void DumpStatic()
		{
			SAFE_DELETE(m_pChartWithName);
			m_pChartWithName = new cChartWithName();
			RenderParameterIndex_listBox->Items->Clear();
//			this->Static_textBox->Text = "";
			cChartWithName::sNameAndData*	l_pTime = new cChartWithName::sNameAndData();
			cChartWithName::sNameAndData*	l_pPickup = new cChartWithName::sNameAndData();
			cChartWithName::sNameAndData*	l_pMonster = new cChartWithName::sNameAndData();
			cChartWithName::sNameAndData*	l_pHurdlwAndBreakable = new cChartWithName::sNameAndData();
			cChartWithName::sNameAndData*	l_pCoin = new cChartWithName::sNameAndData();
			cChartWithName::sNameAndData*	l_pMonsterAndDamageObject = new cChartWithName::sNameAndData();
			l_pTime->m_Name = L"Time";
			l_pPickup->m_Name = L"Pickup";
			l_pMonster->m_Name = L"Monster";
			l_pHurdlwAndBreakable->m_Name = L"HurdlwAndBreakable";
			l_pCoin->m_Name = L"Coin";
			l_pMonsterAndDamageObject->m_Name = L"MonsterAndDamageObject";
			const int	l_Width = m_pChartWithName->m_ObjectIndexDistance;
			const float	l_fHeightScale = m_pChartWithName->m_fHeightScale;
			//m_pChartWithName;
			for(size_t i=0;i<m_pStageParser->m_GamePlayVector.size();++i)
			{
				sGameNode*l_pGameNode = &m_pStageParser->m_GamePlayVector[i];
				std::wstring l_strGamePlayName = l_pGameNode->getValue(L"gameplayName");
				//this->Static_textBox->Text += DNCT::WcharToGcstring(l_strGamePlayName.c_str());
				//this->Static_textBox->Text += DNCT::GetChanglineString();
				//l_str = "Time:"+l_pGameNode->GetFinishTime()+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "Pickup:"+l_pGameNode->GetPickupCount()+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "Monster:"+l_pGameNode->GetMonsterCount()+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "HurdlAndBreakable:"+l_pGameNode->GetHurdleAndBreakableCount()+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "Coin:"+l_pGameNode->m_iCoinCount+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				l_pTime->m_pLineData->AddData(Vector3((float)i*l_Width,l_pGameNode->GetFinishTime()*l_fHeightScale,0),0);
				l_pPickup->m_pLineData->AddData(Vector3((float)i*l_Width,l_pGameNode->GetPickupCount()*l_fHeightScale,0),0);
				l_pMonster->m_pLineData->AddData(Vector3((float)i*l_Width,l_pGameNode->GetMonsterCount()*l_fHeightScale,0),0);
				l_pHurdlwAndBreakable->m_pLineData->AddData(Vector3((float)i*l_Width,l_pGameNode->GetHurdleAndBreakableCount()*l_fHeightScale,0),0);
				l_pCoin->m_pLineData->AddData(Vector3((float)i*l_Width,l_pGameNode->m_iCoinCount*l_fHeightScale,0),0);

				l_pMonsterAndDamageObject->m_pLineData->AddData(Vector3((float)i*l_Width,l_pGameNode->GetHurdleAndBreakableCount()*l_fHeightScale+l_pGameNode->GetMonsterCount()*l_fHeightScale,0),0);


				//l_str = "TimeScale:"+(l_pGameNode->GetFinishTime()/m_fFirstSgateTime)+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "MonsterScale:"+((float)l_pGameNode->GetMonsterCount()/m_iFirstStageMonsterCount)+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "HurdleAndBreakScale:"+((float)l_pGameNode->GetHurdleAndBreakableCount()/m_iFirstStageHurdleAndBreakableCount)+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "PickScale:"+((float)l_pGameNode->GetPickupCount()/m_iFirstStagePickUpCount)+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "CoinScale:"+((float)l_pGameNode->m_iCoinCount/this->m_iFirstStageCoinCount)+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//this->Static_textBox->Text += "=========================================";
				//this->Static_textBox->Text += DNCT::GetChanglineString();
			}
			m_pChartWithName->AddData(l_pTime);
			m_pChartWithName->AddData(l_pPickup);
			m_pChartWithName->AddData(l_pMonster);
			m_pChartWithName->AddData(l_pHurdlwAndBreakable);
			m_pChartWithName->AddData(l_pCoin);
			m_pChartWithName->AddData(l_pMonsterAndDamageObject);
			for(size_t i=0;i<m_pChartWithName->m_LineDataVector.size();++i)
			{
				RenderParameterIndex_listBox->Items->Add(DNCT::WcharToGcstring(m_pChartWithName->m_LineDataVector[i]->m_Name.c_str()));
			}
		}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 m_bWaitParseFile = true;
				 String^l_strFileName = nullptr;
				 if( FileName_textBox->Text->Length != 0 )
					 l_strFileName = FileName_textBox->Text;
				 Node_listBox->Items->Clear();
				 if( !System::IO::File::Exists(l_strFileName) )
				 {
					 String^l_strFileName2 =  System::IO::Directory::GetCurrentDirectory()+"/"+l_strFileName;
					 if( !System::IO::File::Exists(l_strFileName2) )
					 {
						l_strFileName = DNCT::OpenFileAndGetName();
					 }
					 else
					 {
						l_strFileName = l_strFileName2;
					 }
				 }
				 if( l_strFileName )
				 {
					 String^l_strTempFileName = System::IO::Directory::GetCurrentDirectory()+"/mytemp.xml";
					 if(!DNCT::FileToUnicode(l_strFileName,nullptr,l_strTempFileName))
					 {
						 WARNING_MSG("file not exists!");
						return;
					 }
					 if(m_pStageParser->ParseWithMyParse(DNCT::GcStringToChar(l_strTempFileName)))
					 {
						 for(size_t i=0;i<m_pStageParser->m_GamePlayVector.size();++i)
						 {
							sGameNode*l_pGameNode = &m_pStageParser->m_GamePlayVector[i];
							std::wstring l_strValue = m_pStageParser->m_GamePlayVector[i].getValue(L"gameplayName");
							String^l_strStageName = i+":"+DNCT::WcharToGcstring(l_strValue.c_str());
							float l_fTimeToCloseStage = l_pGameNode->GetFinishTime();
							l_strStageName += "         Time:";
							l_strStageName += l_fTimeToCloseStage;
							Node_listBox->Items->Add(l_strStageName);

							if( i == 1)
							{
								m_iFirstStageMonsterCount = l_pGameNode->GetMonsterCount();
								m_iFirstStageHurdleAndBreakableCount = l_pGameNode->GetHurdleAndBreakableCount();
								m_iFirstStagePickUpCount = l_pGameNode->GetPickupCount();
								m_iFirstStageCoinCount = l_pGameNode->m_iCoinCount;
								m_fFirstSgateTime = l_fTimeToCloseStage;
								//TutorialStatic_label->Text = "";
								//TutorialStatic_label->Text += "Time:";
								//TutorialStatic_label->Text += m_fFirstSgateTime+DNCT::GetChanglineString();
								//TutorialStatic_label->Text += "Monster:";
								//TutorialStatic_label->Text += m_iFirstStageMonsterCount+DNCT::GetChanglineString();
								//TutorialStatic_label->Text += "HurdleAndBreakable:";
								//TutorialStatic_label->Text += m_iFirstStageHurdleAndBreakableCount+DNCT::GetChanglineString();
								//TutorialStatic_label->Text += "Pickup:";
								//TutorialStatic_label->Text += m_iFirstStagePickUpCount+DNCT::GetChanglineString();
							}

						 }
						 System::IO::File::Delete(l_strTempFileName);
						 DumpStatic();
					 }

				 }
				 m_bWaitParseFile = false;
			 }
	private: System::Void Node_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 int	l_iSelectedIndex = Node_listBox->SelectedIndex;
				 if( l_iSelectedIndex == -1 )
					 return;
				 if((int)this->m_pStageParser->m_GamePlayVector.size() > l_iSelectedIndex)
				 {
					sGameNode *l_pGameNode = &this->m_pStageParser->m_GamePlayVector[l_iSelectedIndex];
					std::vector<std::wstring>l_NameVector;
					std::vector<int>l_CountVector;
					l_pGameNode->DumpyGroupTypeInfo(&l_NameVector,&l_CountVector);
					l_pGameNode->PlayerGo();
					NodeGroupAndCount_listBox->Items->Clear();
					for(size_t i=0;i<l_CountVector.size();++i)
					{
						String^l_str = DNCT::WcharToGcstring(l_NameVector[i].c_str());
						l_str += ":"+l_CountVector[i];
						//NodeGroupAndCount_listBox->Items->Add(l_str);
					}
					String^l_str;
					l_str = "Time:"+l_pGameNode->GetFinishTime();
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "Pickup:"+l_pGameNode->GetPickupCount();
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "Monster:"+l_pGameNode->GetMonsterCount();
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "HurdlAndBreakable:"+l_pGameNode->GetHurdleAndBreakableCount();
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "Coin:"+l_pGameNode->m_iCoinCount;
					NodeGroupAndCount_listBox->Items->Add(l_str);

					l_str = "TimeScale:"+(l_pGameNode->GetFinishTime()/m_fFirstSgateTime);
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "MonsterScale:"+((float)l_pGameNode->GetMonsterCount()/m_iFirstStageMonsterCount);
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "HurdleAndBreakScale:"+((float)l_pGameNode->GetHurdleAndBreakableCount()/m_iFirstStageHurdleAndBreakableCount);
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "PickScale:"+((float)l_pGameNode->GetPickupCount()/m_iFirstStagePickUpCount);
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "CoinScale:"+((float)l_pGameNode->m_iCoinCount/m_iFirstStagePickUpCount);
					NodeGroupAndCount_listBox->Items->Add(l_str);
				 }
			 }

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			int	l_iSelectedIndex = Node_listBox->SelectedIndex;
			if( m_bWaitParseFile )
				return;
			//cGameApp::m_svGameResolution.x = (float)m_pTargetControl->Width;
			//cGameApp::m_svGameResolution.y = (float)m_pTargetControl->Height;
			wglMakeCurrent( m_HdcMV,m_HGLRCMV );
			RECT rcClient;
			GetClientRect((HWND)m_pTargetControl->Handle.ToPointer(), &rcClient);
			UseShaderProgram();
			cGameApp::m_svViewPortSize.x = 0.f;
			cGameApp::m_svViewPortSize.y = 0.f;
			cGameApp::m_svViewPortSize.z = (float)m_pTargetControl->Width;
			cGameApp::m_svViewPortSize.w = (float)rcClient.bottom;
			this->m_pGameApp->Run();
			this->m_pGameApp->m_svBGColor = Vector4(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
			glClearDepth(1.0f);
			glAlphaFunc(GL_GREATER,0.0001f);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			Vector2	l_vViewPort(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height());
			if(m_pOrthogonalCamera)
			{
				//m_pOrthogonalCamera->CameraUpdateByMouse(cGameApp::m_sbMouseClickStatus[0],cGameApp::m_sbMouseClickStatus[1],cGameApp::m_sMouseWhellDelta,cGameApp::m_sScreenMousePosition.x,cGameApp::m_sScreenMousePosition.y,l_vViewPort);
				m_pOrthogonalCamera->Render();
				m_pOrthogonalCamera->DrawGrid(100,100,Vector4(0.05f,0.1f,0.1f,1));
			}
			if( RenderMode_listBox->SelectedIndex != 1 )
			{
				if( l_iSelectedIndex != -1 && StageRender_checkBox->Checked )
				{
					sGameNode* l_pGameNode = &this->m_pStageParser->m_GamePlayVector[l_iSelectedIndex];
					l_pGameNode->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
					if(l_pGameNode->MouseMove((int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y))
					{
					}
					l_pGameNode->RenderScene();
				}
			}
			else
			{
				m_pChartWithName->Render();
			}
			if( StageClearBonus_listBox->SelectedIndex != -1 )
			{
				/*for(int i=0;i<StageClearBonus_listBox->SelectedIndices->Count;++i)
				{
					int l_iIndex = StageClearBonus_listBox->SelectedIndices[i];
				}*/
			}
			if( m_pGameData && this->m_pGameData->m_pGameData )
			{
				switch(GameData_tabControl->SelectedIndex)
				{
					case 2:
						this->m_pGameData->m_pGameData->ShopRender();
						break;
					default:
					if( MonsterRender_checkBox->Checked )
						this->m_pGameData->m_pGameData->MonsterRender();
					if( EnemyRender_checkBox->Checked )
						this->m_pGameData->m_pGameData->EnemyRender();
					if( StageBonusRender_checkBox->Checked )
						this->m_pGameData->m_pGameData->CampaginRender();
						break;
				}
			}
			if( m_pLinerData )
			{
				int l_iNumPoints = (int)Number_numericUpDown->Value*100;
				l_iNumPoints -= 100;
				m_pLinerData->GetMaxValueDis()->z = 1;
				m_pLinerData->DebugRenderWithMaximumValue(Vector3((float)l_iNumPoints,1000,1000),false,true);
			}
			//cGameApp::ShowInfo();
			SwapBuffers(m_HdcMV);
		 }
		System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			//this->Focus();
			if( !timer1->Enabled || !this->Visible )
				return;
			//make mouse wheel work
			m_pTargetControl->Focus();
			if( m_pGameApp )
				m_pGameApp->MouseDown(e->X,e->Y);
			POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
			GCFORM::MouseButtons l_MouseButton = e->Button;
			m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
				,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)this->m_pTargetControl->Width,(float)this->m_pTargetControl->Height));
		}
		System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			if( !timer1->Enabled || !this->Visible )
				return;
			if( m_pGameApp )
				m_pGameApp->MouseMove(e->X,e->Y);
			int l_iDelta = 1;
			if (GCFORM::Control::ModifierKeys.HasFlag(GCFORM::Keys::Control))
			{
				 l_iDelta *= 10;
			}
			POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
			GCFORM::MouseButtons l_MouseButton = e->Button;
			m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
				,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,0,e->X*l_iDelta,e->Y*l_iDelta,Vector2((float)this->m_pTargetControl->Width,(float)this->m_pTargetControl->Height));
		}

		System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			if( !timer1->Enabled || !this->Visible )
				return;
			if( m_pGameApp )
				m_pGameApp->MouseUp(e->X,e->Y);
			POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
			GCFORM::MouseButtons l_MouseButton = e->Button;
			int l_iDelta = e->Delta;
			if (GCFORM::Control::ModifierKeys.HasFlag(GCFORM::Keys::Control))
			{
				 l_iDelta *= 10;
			}

			m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
				,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,l_iDelta,e->X,e->Y,Vector2((float)this->m_pTargetControl->Width,(float)this->m_pTargetControl->Height));
		}
private: System::Void RenderParameterIndex_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->m_pChartWithName->m_RenderIndex.clear();
			 for(int i=0;i<RenderParameterIndex_listBox->SelectedIndices->Count;++i)
			 {
				 this->m_pChartWithName->m_RenderIndex.push_back(RenderParameterIndex_listBox->SelectedIndices[i]);
			 }
		 }
private: System::Void ShowParameterValue_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->m_pChartWithName->m_bShowValue = ShowParameterValue_checkBox->Checked;
			 this->m_pGameData->m_pGameData->m_CampaignRenderFlagControl.m_bShowValue = ShowParameterValue_checkBox->Checked;
		 }
private: System::Void button1_Click_1(System::Object^  sender, System::EventArgs^  e) 
		 {
			 m_bWaitParseFile = true;
			 std::string l_strLastDirectory;
			 const char*l_strDirectory = "mr_xml_directory.txt";
			 //const char*l_strDirectory = "";
			 cBinaryFile l_cBinaryFile;
			 if(l_cBinaryFile.Openfile(l_strDirectory))
			 {
				 l_strLastDirectory = (char*)l_cBinaryFile.GetDataFile(0);
				 l_strLastDirectory.resize(l_cBinaryFile.GetDataFileSize());
			 }
			 String^l_strDirectory3;
			 if(l_strLastDirectory.length() == 0 || !System::IO::Directory::Exists(gcnew String(l_strLastDirectory.c_str())) )
				l_strDirectory3 = DNCT::SelectDirectory(l_strLastDirectory.c_str());
			else
				l_strDirectory3 = gcnew String(l_strLastDirectory.c_str());
			 if(l_strDirectory3)
			 {
				std::string l_strDirectory4 = DNCT::GcStringToChar(l_strDirectory3);
				SAFE_DELETE(m_pGameData);
				m_pGameData = new cGameData();
				this->MonsterData_listBox->Items->Clear();
				this->EnemyData_listBox->Items->Clear();
				this->ShopData_listBox->Items->Clear();
				this->StageClearBonus_listBox->Items->Clear();
				if(m_pGameData->Parse(l_strDirectory4.c_str()))
				{
					l_cBinaryFile.CloseFile();
					if(l_cBinaryFile.Writefile(l_strDirectory,false,true))
					{
						l_cBinaryFile.WriteToFile(l_strDirectory4.c_str());
						l_cBinaryFile.CloseFile();
					}
					for(size_t i=0;i<m_pGameData->m_pGameData->m_EnemyStatusVector.size();++i)
					{
						auto l_pEnemyStatus = m_pGameData->m_pGameData->m_EnemyStatusVector[i];
						this->EnemyData_listBox->Items->Add(gcnew String(l_pEnemyStatus->GetMonsterName().c_str()));
					}
					for(size_t i=0;i<m_pGameData->m_pGameData->m_MonsterInfoVector.size();++i)
					{
						auto l_pMonsterInfo = m_pGameData->m_pGameData->m_MonsterInfoVector[i];
						//String^l_strMonsterInfo = gcnew String(l_pMonsterInfo->GetMonsterName().c_str());
						//l_strMonsterInfo += gcnew String(l_pMonsterInfo->GetInfo().c_str());
						//this->MonsterData_listBox->Items->Add(l_strMonsterInfo);
						this->MonsterData_listBox->Items->Add(gcnew String(l_pMonsterInfo->GetMonsterName().c_str()));
					}
					for(auto l_Iterator : m_pGameData->m_pGameData->m_MonsterShopDataMap)
					{
						l_Iterator.first;
						auto l_pVector = l_Iterator.second;
						if( l_pVector->size() > 0 )
						//for(size_t j=0;j<l_pVector->size();++j)
						{
							auto l_pData  = (*l_pVector)[0];
							std::wstring l_strMonsterName = m_pGameData->m_pGameData->GetMonsterName(l_pData->GetMonsterID());
							this->ShopData_listBox->Items->Add(DNCT::WcharToGcstring(l_strMonsterName.c_str()));
						}
						for(auto l_pData: (*l_pVector))
						{
							String^l_strItemName = l_pData->GetMonsterID().ToString();
							l_strItemName += ":";
							l_strItemName += gcnew String(l_pData->GetDescription().c_str());
							Item_listBox->Items->Add(l_strItemName);
						}
					}
					for(auto l_pData : m_pGameData->m_pGameData->m_pCampaign->m_AllRenderData.m_LineDataVector)
					{
						this->StageClearBonus_listBox->Items->Add(gcnew String(l_pData->m_Name.c_str()));
					}
					m_pGameData->m_pGameData->m_EnemyRenderFlagControl.m_bShowValue = EnemyShowValue_checkBox->Checked;
					m_pGameData->m_pGameData->m_MonsterRenderFlagControl.m_bShowValue = MonsterShowValue_checkBox->Checked;
				}
			 }
			 l_cBinaryFile.CloseFile();
			 m_bWaitParseFile = false;
		 }
private:System::Void	MyRenderFlagChange(GCFORM::ListBox^e_pListBox,sGameData::sRenderFlagControl*e_pRenderFlagControl)
		{
			 int l_iIndxe = e_pListBox->SelectedIndex;
			 if( l_iIndxe != -1 )
			 {
				 e_pRenderFlagControl->m_iRenderFlag = 1<<l_iIndxe;
			 }
			 else
			 {
				e_pRenderFlagControl->m_iRenderFlag = 0;
			 }		
		}
private:System::Void	MyRenderIndexChange(GCFORM::ListBox^e_pListBox,sGameData::sRenderFlagControl*e_pRenderFlagControl)
		{
			 e_pRenderFlagControl->m_RenderIndexVector.clear();
			 for(int i = 0 ;i<e_pListBox->SelectedIndices->Count;++i)
			 {
				 int l_iIndex = e_pListBox->SelectedIndices[i];
				 e_pRenderFlagControl->m_RenderIndexVector.push_back(l_iIndex);
			 }	
		}
private: System::Void MonsterStatusShowCurve_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 MyRenderFlagChange(MonsterStatusShowCurve_listBox,&this->m_pGameData->m_pGameData->m_MonsterRenderFlagControl);
		 }
private: System::Void MonsterData_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 MyRenderIndexChange(MonsterData_listBox,&this->m_pGameData->m_pGameData->m_MonsterRenderFlagControl);
		 }
private: System::Void EnemyStatus_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 MyRenderFlagChange(EnemyStatus_listBox,&this->m_pGameData->m_pGameData->m_EnemyRenderFlagControl);
		 }
private: System::Void EnemyData_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 MyRenderIndexChange(EnemyData_listBox,&this->m_pGameData->m_pGameData->m_EnemyRenderFlagControl);
		 }
private: System::Void MonsterShowValue_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->m_pGameData->m_pGameData->m_MonsterRenderFlagControl.m_bShowValue = MonsterShowValue_checkBox->Checked;
		 }
private: System::Void EnemyShowValue_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->m_pGameData->m_pGameData->m_EnemyRenderFlagControl.m_bShowValue = EnemyShowValue_checkBox->Checked;
		 }
private: System::Void ShopShowValue_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->m_pGameData->m_pGameData->m_ShopRenderFlagControl.m_bShowValue = ShopShowValue_checkBox->Checked;
		 }
private: System::Void StageClearBonus_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 MyRenderIndexChange(StageClearBonus_listBox,&this->m_pGameData->m_pGameData->m_CampaignRenderFlagControl);
		 }
private: System::Void CaemraReset_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_pOrthogonalCamera->SetViewRect(Vector4(-1920/2.f,-1080/2.f,1920/2.f,1080/2.f));
			 m_pOrthogonalCamera->SetCameraPos(Vector2(0,0));
		 }
private: System::Void StartPosX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			int l_iNumPoints = (int)Number_numericUpDown->Value;
			float l_fStartPosX = (float)StartPosX_numericUpDown->Value;
			float l_fStartPosY = (float)StartPosY_numericUpDown->Value;
			float l_fStepX = (float)StepX_numericUpDown->Value;
			float l_fStepY = (float)StepY_numericUpDown->Value;
			float l_fGrident = (float)Grident_numericUpDown->Value;
			float l_fSteep = (float)Steep_numericUpDown->Value;
			Result_textBox->Text = "";
			SAFE_DELETE(m_pLinerData);
			String^l_strPrefix = XMLPrefix_textBox->Text;
			TimeToMoneyList__textBox->Text = "";
			if( GridentWithPrevious_checkBox->Checked )
				m_pLinerData = cLinerDataProcessor<Vector3>::GererateDataWithPreviousData(l_iNumPoints,Vector3(l_fStartPosX,l_fStartPosY,0),Vector3(l_fStepX,l_fStepY,0),l_fGrident,l_fSteep);
			else
				m_pLinerData = cLinerDataProcessor<Vector3>::GererateData(l_iNumPoints,Vector3(l_fStartPosX,l_fStartPosY,0),Vector3(l_fStepX,l_fStepY,0),l_fGrident,l_fSteep);

			for(size_t i=0;i<m_pLinerData->Count();++i)
			{
				Vector3 l_vPos = *m_pLinerData->GetData(i);
				l_vPos.x = 100.f*i;
				if( this->Round_checkBox->Checked )
				{
					int l_iRound = (int)(l_vPos.y/(int)this->Round_numericUpDown->Value);
					l_vPos.y = (float)(l_iRound*(int)this->Round_numericUpDown->Value);
				}
				if( l_strPrefix && l_strPrefix->Length )
				{
					Result_textBox->Text += l_strPrefix;
					Result_textBox->Text += "=\"";
				}
				Result_textBox->Text += l_vPos.y.ToString();
				TimeToMoneyList__textBox->Text += l_vPos.y.ToString();
				if( l_strPrefix && l_strPrefix->Length )
				{
					Result_textBox->Text += "\"";
				}
				Result_textBox->Text += DNCT::GetChanglineString();
				TimeToMoneyList__textBox->Text += DNCT::GetChanglineString();
				l_vPos.y = -l_vPos.y;
				l_vPos.z = 0;
				m_pLinerData->ChangeData(i,l_vPos);
			}
			DoTimeToMoney_button_Click(sender,e);
		 }
private: float		GetProperTimeToMoneyGem(int e_iTime)
{
			 //first get 1 gem cost money
			 float l_fGemPrice = (float)GemPrice_numericUpDown->Value;
			 int l_iGems = (int)this->OneRMBCostGems_numericUpDown->Value;
			 float l_fOneGemCost = l_fGemPrice/l_iGems;
			 //second find one second salary and respone time to money
			 float l_fOneHourSecondSalary = (float)AHourSalary_numericUpDown->Value;
			 float l_fOneSecondSalary = (float)AHourSalary_numericUpDown->Value/60.f/60.f;
			 int l_iWaitSconds = e_iTime;
			 float l_fCostSaraly = l_fOneSecondSalary*l_iWaitSconds;
			 float l_fResult = l_fCostSaraly/l_fOneGemCost;
			 return l_fResult;
}
private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 int l_iWaitSconds = (int)TargetSceonds_numericUpDown->Value;
			 float l_fResult = GetProperTimeToMoneyGem(l_iWaitSconds);
			 TimeToGems_textBox->Text = l_fResult.ToString();
			 this->DailySalary_label->Text = ((float)AHourSalary_numericUpDown->Value*60*8).ToString();
		 }
private: System::Void DoTimeToMoney_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 TimeToGems_textBox->Text = "";
			 String^l_strSource = TimeToMoneyList__textBox->Text;
			 //String^l_strKey = DNCT::GetChanglineString();
			 array<String^>^l_strArray = l_strSource->Split(L'\n');
			 String^l_strPrefix = MoneyXMLPrefix_textBox->Text;
			 for each(String^l_str in l_strArray  )
			 {
				int l_iValue = 0;
				if(System::Int32::TryParse(l_str,l_iValue))
				{
					int l_iValue = System::Int32::Parse(l_str);
					float l_fValue = GetProperTimeToMoneyGem(l_iValue);
					if( this->MoneyRound_checkBox->Checked )
					{
						int l_Rount = (int)(l_fValue /(int)this->MoneyRound_numericUpDown->Value);
						l_fValue = l_Rount * (float)this->MoneyRound_numericUpDown->Value;
					}
					if( l_strPrefix && l_strPrefix->Length )
					{
						TimeToGems_textBox->Text += l_strPrefix;
						TimeToGems_textBox->Text += "=\"";
					}
					TimeToGems_textBox->Text += l_fValue.ToString();
					if( l_strPrefix && l_strPrefix->Length )
					{
						TimeToGems_textBox->Text += "\"";
					}
					TimeToGems_textBox->Text += DNCT::GetChanglineString();
				}
				else
				{
					TimeToGems_textBox->Text += l_str+"data is wrong";
				}
			 }
			 
		 }
private: System::Void GridentWithPrevious_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 StartPosX_numericUpDown_ValueChanged(sender,e);
		 }
private: System::Void Round_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 StartPosX_numericUpDown_ValueChanged(sender,e);
		 }
private: System::Void GemPriceToProbabilityGenerate_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( !m_pGameData || !m_pGameData->m_pGameData )
			 {
				button1_Click_1(sender,e);
			 }
			 if( m_pGameData && m_pGameData->m_pGameData )
			 {
				std::wstring l_strErrorMessage;
				GoodItemProbability_listBox->Items->Clear();
				GoodItemProbability_textBox->Text = "";
				BadItemProbability_listBox->Items->Clear();
				BadItemProbability_textBox->Text = "";
				int l_iCurrentItemTypeForDraw = 0;
				for(int l_iCurrentPoolIndex = 0;l_iCurrentPoolIndex<Probability_tabControl->TabPages->Count;++l_iCurrentPoolIndex)
				{
					l_iCurrentItemTypeForDraw = l_iCurrentPoolIndex+1;
					 cGemPriceToProbability*l_pGemPriceToProbability = &m_pGemPriceToProbability[l_iCurrentPoolIndex];
					 l_pGemPriceToProbability->ClearData();
					 for(auto l_Iterator = m_pGameData->m_pGameData->m_MonsterShopDataMap.begin();
						 l_Iterator != m_pGameData->m_pGameData->m_MonsterShopDataMap.end();
						 ++l_Iterator)
					 {
						 std::vector<sGameData::sMonsterShop*>*l_pVector = l_Iterator->second;
						 for(size_t i=0;i<l_pVector->size();++i)
						 {
							//1 heart for 60 coins
							//10 gems for 300 coins
							//1 heart for 2 gems,now just make 1 heart for 1 gem
							//1 gems for 30 coins
							sGameData::sMonsterShop*l_pData = (*l_pVector)[i];
							std::wstring l_strName = ValueToStringW(l_pData->GetID());
							l_strName += L"_";
							l_strName += l_pData->m_strItemName;
							if( l_pData->GetPoolIndex() == 0 ||
								l_pData->GetPoolIndex() == l_iCurrentItemTypeForDraw || 
								l_pData->GetPoolIndex() == -1)
							{
								int l_iType = l_pData->GetPriceType();
								int l_iPrice = l_pData->GetPrice();
								switch(l_iType)
								{//1 for coins 2 for gems 3 for hearts
								case 1:
									l_iPrice = ValueDivideAddOneIfModulusIsNotZero(l_iPrice,(int)CoinToGems_numericUpDown->Value);
									break;
								case 3:
									l_iPrice = ValueDivideAddOneIfModulusIsNotZero(l_iPrice,(int)HeartToGems_numericUpDown->Value);
									break;
								default:
									break;
								}
								if( l_iPrice == 0 )
								{
									l_iPrice = 1;
									l_strErrorMessage += l_pData->m_strItemName;
									l_strErrorMessage += L"\t price is 0 force make it to 1 gem\n";
									//continue;
								}
								l_pGemPriceToProbability->AddGemsData(l_strName,l_iPrice);
							}
							else
							{
								l_strErrorMessage += l_strName;
								l_strErrorMessage += L"\t pool index wrong or not in this pool\n";
							}
						 }
					 }
					 for(size_t i=0;i<m_pGameData->m_pGameData->m_MonsterInfoVector.size();++i)
					 {
						auto l_MonsterInfo = m_pGameData->m_pGameData->m_MonsterInfoVector[i];
						if( l_MonsterInfo->GetPoolIndex() == 0 || 
							l_MonsterInfo->GetPoolIndex() == l_iCurrentItemTypeForDraw||
							l_MonsterInfo->GetPoolIndex() == -1)
						{
							int l_iPrice = l_MonsterInfo->GetUnlockValue();
							if(l_MonsterInfo->GetUnlockType() == 0)//coins
							{
								l_iPrice = ValueDivideAddOneIfModulusIsNotZero(l_iPrice,(int)CoinToGems_numericUpDown->Value);;
							}
							else
							if(l_MonsterInfo->GetUnlockType() == 1)//gems
							{
					
							}
							else//by stage clear dont add this!
							if(l_MonsterInfo->GetUnlockType() == 2)
							{
								l_strErrorMessage += l_MonsterInfo->GetMonsterName();
								l_strErrorMessage += L"\t this monster unlock by clear stage!\n";
								//continue;
							}
							if( l_iPrice == 0 )
							{
								l_iPrice = 1;
								l_strErrorMessage += l_MonsterInfo->GetMonsterName();
								l_strErrorMessage += L"\t price is 0 force make it to 1 gem\n";
								//continue;
							}
							l_pGemPriceToProbability->AddGemsData(l_MonsterInfo->GetMonsterName(),l_iPrice);
						}
						else
						{
							l_strErrorMessage += l_MonsterInfo->GetMonsterName();
							l_strErrorMessage += L"\t pool index wrong or not in this pool\n";
						}
					 }
					 DumpProbability(l_pGemPriceToProbability->DumpProbabilityResult(true),m_pDrawListBoxArray[l_iCurrentPoolIndex],m_pDrawTextBoxArray[l_iCurrentPoolIndex],&l_strErrorMessage);
				}
			 }
		 }
		 void	DumpProbability(std::vector<std::wstring>e_strResultVector,GCFORM::ListBox^e_pListBox,GCFORM::TextBox^e_pTextBox,std::wstring*e_pErrorMessage)
		{
			e_pListBox->Items->Clear();
			e_pTextBox->Text = "";
			String^l_strText = gcnew String("");
			std::vector<std::wstring> l_str = e_strResultVector;
			bool	l_bSkipFirst = true;
			for(auto l_strInfo : l_str)
			{
				String^l_strGCStringInfo = DNCT::WcharToGcstring(l_strInfo.c_str());
				l_strText += l_strGCStringInfo;
				l_strText += DNCT::GetChanglineString();
				if( l_bSkipFirst == true )
				{
				l_bSkipFirst = false;
				continue;
				}
				e_pListBox->Items->Add(l_strGCStringInfo);
			}
			if(e_pErrorMessage && e_pErrorMessage->length() != 0)
			{
				l_strText += "\n Error!!!!!!!!!!\t";
				l_strText += DNCT::WcharToGcstring(e_pErrorMessage->c_str());
			}
			e_pTextBox->Text = l_strText;
		}
private: System::Void BadItemProbability_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			int l_iPoolSelectedIndex = Probability_tabControl->SelectedIndex;
			if( l_iPoolSelectedIndex != -1 )
			{
				int l_iListboxSelectedIndex = m_pDrawListBoxArray[l_iPoolSelectedIndex]->SelectedIndex;
				if( l_iListboxSelectedIndex != -1 )
				{
					cGemPriceToProbability*l_pGemPriceToProbability = &m_pGemPriceToProbability[l_iPoolSelectedIndex];
					int l_iScore = l_pGemPriceToProbability->GetScoreByAddGemsDataOrder(l_iListboxSelectedIndex);
					m_pDrawNumericUpDownArray[l_iPoolSelectedIndex]->Value = l_iScore;
				}
			}
		 }
private: System::Void BadItemScore_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			int l_iPoolSelectedIndex = Probability_tabControl->SelectedIndex;
			if( l_iPoolSelectedIndex != -1 )
			{
				int l_iListboxSelectedIndex = m_pDrawListBoxArray[l_iPoolSelectedIndex]->SelectedIndex;
				if( l_iListboxSelectedIndex != -1 )
				{
					cGemPriceToProbability*l_pGemPriceToProbability = &m_pGemPriceToProbability[l_iPoolSelectedIndex];
					if(l_pGemPriceToProbability->GetScoreByAddGemsDataOrder(l_iListboxSelectedIndex)!= (int)m_pDrawNumericUpDownArray[l_iPoolSelectedIndex]->Value)
					{
						l_pGemPriceToProbability->SetScoreByAddGemsDataOrder(l_iListboxSelectedIndex,(int)m_pDrawNumericUpDownArray[l_iPoolSelectedIndex]->Value);
						std::wstring l_strInfo = l_pGemPriceToProbability->GetScoreInfoByAddGemsDataOrder(l_iListboxSelectedIndex);
						DNCT::ReplaceListBoxStringByIndex(m_pDrawListBoxArray[l_iPoolSelectedIndex],l_iListboxSelectedIndex,DNCT::WcharToGcstring(l_strInfo.c_str()));
						l_pGemPriceToProbability->CalculateTotalScore();
						l_pGemPriceToProbability->CalculateProbability();
						DumpProbability(l_pGemPriceToProbability->DumpInfo(true),m_pDrawListBoxArray[l_iPoolSelectedIndex],m_pDrawTextBoxArray[l_iPoolSelectedIndex],nullptr);

					}
				}
			}
		 }
};
}