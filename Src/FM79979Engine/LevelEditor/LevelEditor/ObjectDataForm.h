#pragma once

#include "LevelEditorApp.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
//need name to fetch relate xml node to export data,and get image to assign to the new object
namespace LevelEditor
{

	public ref class ObjectDataForm : public System::Windows::Forms::UserControl
	{
	public:
		ObjectDataForm(GCFORM::TabControl^e_ptabControl,String^e_pTabText,GCFORM::Form^e_pForm)
		{
			InitializeComponent();
			GCFORM::TabPage^l_pPage = gcnew GCFORM::TabPage;
			l_pPage->Text = "ObjectDataForm";
			l_pPage->Controls->Add(this);
			e_ptabControl->TabPages->Add(l_pPage);
			m_pForm = e_pForm;
		}

	protected:
		~ObjectDataForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::SplitContainer^  splitContainer1;
	public: System::Windows::Forms::SplitContainer^  splitContainer2;
	public: System::Windows::Forms::MenuStrip^  menuStrip1;
	public: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  imageToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  openTemplateToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  imageToolStripMenuItem1;
	public: System::Windows::Forms::ToolStripMenuItem^  assistanceToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  showGridToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  autoAllignGridToolStripMenuItem;
	public: System::Windows::Forms::TreeView^  XMLDataForQuickApply_treeView;
	private: System::ComponentModel::IContainer^  components;
	public: 

	public: System::Windows::Forms::ContextMenuStrip^  UserDefineData__contextMenuStrip;
	public: System::Windows::Forms::ToolStripMenuItem^  refreshToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  addNewNodeFromFileToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  deleteNodeToolStripMenuItem;
	public: System::Windows::Forms::Label^  label1;
	public: XMLDataEditor::XMLForm^  ObjectTemplate_xmlNodeEditComponent;
	public: System::Windows::Forms::ListBox^  AllStage_listBox;
	public: System::Windows::Forms::TextBox^  StageName_textBox;
	public: System::Windows::Forms::Button^  AddStage_button;
	public: System::Windows::Forms::Button^  DelStage_button;
	public: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	public: System::Windows::Forms::Button^  ReRditStagebutton;
	public: System::Windows::Forms::ToolStripMenuItem^  camerResetToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  showResolutionBorderToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  backgroundImageToolStripMenuItem;
	public: System::Windows::Forms::Label^  label10;
	public: System::Windows::Forms::Label^  label4;
	public: System::Windows::Forms::NumericUpDown^  StartY_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  StartX_numericUpDown;
	public: System::Windows::Forms::Button^  AddToLayer_button;
	public: System::Windows::Forms::Button^  LayerDown_button;
	public: System::Windows::Forms::Button^  LayerUp_button;
	public: System::Windows::Forms::ListBox^  Layer_listBox;
	public: System::Windows::Forms::Button^  DeleteLayer_button;
	public: System::Windows::Forms::ToolStripMenuItem^  autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  cameraResetToolStripMenuItem;
	public: System::Windows::Forms::ListBox^  LayerObjectData_listBox;
	public: System::Windows::Forms::Button^  DeleteObject_button;
	public: System::Windows::Forms::ToolStripMenuItem^  showWarningMsgToolStripMenuItem;
	public: System::Windows::Forms::GroupBox^  groupBox1;
	public: System::Windows::Forms::Label^  label3;
	public: System::Windows::Forms::CheckBox^  AllowSamepositionObject_checkBox;
	public: System::Windows::Forms::NumericUpDown^  GridHeight_numericUpDown;
	public: System::Windows::Forms::Label^  label6;
	public: System::Windows::Forms::NumericUpDown^  GridWidth_numericUpDown;
	public: System::Windows::Forms::Label^  label7;
	public: System::Windows::Forms::Label^  label2;
	public: System::Windows::Forms::NumericUpDown^  MapHeight_numericUpDown;
	public: System::Windows::Forms::NumericUpDown^  MapWidth_numericUpDown;
	public: System::Windows::Forms::Button^  AddObjectToStage_button;
	public: System::Windows::Forms::Label^  label5;
	public: System::Windows::Forms::Label^  label15;
	public: System::Windows::Forms::Button^  DelTemplate_button;
	public: System::Windows::Forms::Label^  label13;
	public: System::Windows::Forms::ListBox^  Template_listBox;
	public: System::Windows::Forms::Button^  AddTemplate_button;
	public: System::Windows::Forms::NumericUpDown^  AddObjectY_numericUpDown;
	public: System::Windows::Forms::Button^  DelImage_button;
	public: System::Windows::Forms::NumericUpDown^  AddObjectX_numericUpDown;
	public: System::Windows::Forms::Button^  AddImage_button;
	public: System::Windows::Forms::TextBox^  TemplateName_textBox;
	public: System::Windows::Forms::Button^  Clear_button;
	public: System::Windows::Forms::Button^  ReEditTemplate_button;
	public: System::Windows::Forms::Label^  label17;
	public: System::Windows::Forms::TextBox^  LayerName_textBox;
	public: System::Windows::Forms::TreeView^  ImageList_treeView;
	public: System::Windows::Forms::ContextMenuStrip^  UserDefineData_contextMenuStrip;
	public: System::Windows::Forms::ToolStripMenuItem^  addToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  deleteToolStripMenuItem;
	public: System::Windows::Forms::ContextMenuStrip^  LayerData_contextMenuStrip;
	public: System::Windows::Forms::ToolStripMenuItem^  selectAllThisTypeTemplateToolStripMenuItem;
	public: System::Windows::Forms::ToolStripMenuItem^  refreshUserDataFromToolStripMenuItem;
	public: System::Windows::Forms::Label^  label14;
	public: System::Windows::Forms::NumericUpDown^  AddObjectZ_numericUpDown;
	public: System::Windows::Forms::Timer^  timer1;
	public: System::Windows::Forms::SplitContainer^  Main_splitContainer;
	public: System::Windows::Forms::CheckBox^  PlaceObjectByImageCenter_checkBox;
	public: System::Windows::Forms::GroupBox^  StageData_groupBox;
	public: System::Windows::Forms::GroupBox^  TemplateList_groupBox;
	public: System::Windows::Forms::GroupBox^  XMLData_groupBox;
	public: System::Windows::Forms::TabControl^  tabControl1;
	public: System::Windows::Forms::TabPage^  tabPage1;
	public: System::Windows::Forms::TabPage^  tabPage2;
	public: System::Windows::Forms::FlowLayoutPanel^  EventData_flowLayoutPanel;
	public: System::Windows::Forms::CheckBox^  LayerVisible_checkBox;
	public: System::Windows::Forms::ComboBox^  MouseMode_comboBox;
	public: System::Windows::Forms::Label^  MouseMode_label;
	public: System::Windows::Forms::GroupBox^  LayerList_groupBox;
	public: System::Windows::Forms::GroupBox^  Object_groupBox;
	public: System::Windows::Forms::ToolStripMenuItem^  assignToObjectEventDataToolStripMenuItem;
	public: System::Windows::Forms::Button^  NoImage_button;
	public: XMLDataEditor::XMLForm^			LevelLayerGridEventDataNode_xmlNodeEditComponent;
	public: System::Windows::Forms::CheckBox^  InGameRender_checkBox;





		//my
	GCFORM::Form^m_pForm;
#pragma region Windows Form Designer generated code
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->TemplateList_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->NoImage_button = (gcnew System::Windows::Forms::Button());
			this->ImageList_treeView = (gcnew System::Windows::Forms::TreeView());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->TemplateName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->Template_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AddImage_button = (gcnew System::Windows::Forms::Button());
			this->ReEditTemplate_button = (gcnew System::Windows::Forms::Button());
			this->AddTemplate_button = (gcnew System::Windows::Forms::Button());
			this->DelImage_button = (gcnew System::Windows::Forms::Button());
			this->DelTemplate_button = (gcnew System::Windows::Forms::Button());
			this->XMLData_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->XMLDataForQuickApply_treeView = (gcnew System::Windows::Forms::TreeView());
			this->UserDefineData__contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->refreshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addNewNodeFromFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteNodeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->assignToObjectEventDataToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->LayerList_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->InGameRender_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->Layer_listBox = (gcnew System::Windows::Forms::ListBox());
			this->LayerName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->LayerUp_button = (gcnew System::Windows::Forms::Button());
			this->DeleteLayer_button = (gcnew System::Windows::Forms::Button());
			this->LayerDown_button = (gcnew System::Windows::Forms::Button());
			this->AddToLayer_button = (gcnew System::Windows::Forms::Button());
			this->LayerVisible_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->StageData_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->StageName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->StartY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->AllStage_listBox = (gcnew System::Windows::Forms::ListBox());
			this->MapWidth_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->DelStage_button = (gcnew System::Windows::Forms::Button());
			this->StartX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ReRditStagebutton = (gcnew System::Windows::Forms::Button());
			this->MapHeight_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->AddStage_button = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->GridWidth_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->GridHeight_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent = (gcnew XMLDataEditor::XMLForm());
			this->Object_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->AddObjectX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->AddObjectToStage_button = (gcnew System::Windows::Forms::Button());
			this->Clear_button = (gcnew System::Windows::Forms::Button());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->AddObjectZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->DeleteObject_button = (gcnew System::Windows::Forms::Button());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->AddObjectY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->MouseMode_label = (gcnew System::Windows::Forms::Label());
			this->PlaceObjectByImageCenter_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->MouseMode_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->LayerObjectData_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AllowSamepositionObject_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->EventData_flowLayoutPanel = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->ObjectTemplate_xmlNodeEditComponent = (gcnew XMLDataEditor::XMLForm());
			this->Main_splitContainer = (gcnew System::Windows::Forms::SplitContainer());
			this->assistanceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showGridToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->autoAllignGridToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->camerResetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showResolutionBorderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showWarningMsgToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openTemplateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imageToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->backgroundImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cameraResetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->UserDefineData_contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->addToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->LayerData_contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->selectAllThisTypeTemplateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->refreshUserDataFromToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->flowLayoutPanel1->SuspendLayout();
			this->TemplateList_groupBox->SuspendLayout();
			this->XMLData_groupBox->SuspendLayout();
			this->UserDefineData__contextMenuStrip->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer2))->BeginInit();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->LayerList_groupBox->SuspendLayout();
			this->StageData_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MapWidth_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MapHeight_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridWidth_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridHeight_numericUpDown))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->Object_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectZ_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectY_numericUpDown))->BeginInit();
			this->tabPage2->SuspendLayout();
			this->EventData_flowLayoutPanel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Main_splitContainer))->BeginInit();
			this->Main_splitContainer->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->UserDefineData_contextMenuStrip->SuspendLayout();
			this->LayerData_contextMenuStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 24);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->AllowDrop = true;
			this->splitContainer1->Panel1->AutoScroll = true;
			this->splitContainer1->Panel1->Controls->Add(this->flowLayoutPanel1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
			this->splitContainer1->Size = System::Drawing::Size(1900, 1017);
			this->splitContainer1->SplitterDistance = 234;
			this->splitContainer1->SplitterWidth = 3;
			this->splitContainer1->TabIndex = 0;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->AutoScroll = true;
			this->flowLayoutPanel1->Controls->Add(this->TemplateList_groupBox);
			this->flowLayoutPanel1->Controls->Add(this->XMLData_groupBox);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(2);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(232, 1015);
			this->flowLayoutPanel1->TabIndex = 0;
			// 
			// TemplateList_groupBox
			// 
			this->TemplateList_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->TemplateList_groupBox->Controls->Add(this->NoImage_button);
			this->TemplateList_groupBox->Controls->Add(this->ImageList_treeView);
			this->TemplateList_groupBox->Controls->Add(this->label5);
			this->TemplateList_groupBox->Controls->Add(this->TemplateName_textBox);
			this->TemplateList_groupBox->Controls->Add(this->label1);
			this->TemplateList_groupBox->Controls->Add(this->Template_listBox);
			this->TemplateList_groupBox->Controls->Add(this->AddImage_button);
			this->TemplateList_groupBox->Controls->Add(this->ReEditTemplate_button);
			this->TemplateList_groupBox->Controls->Add(this->AddTemplate_button);
			this->TemplateList_groupBox->Controls->Add(this->DelImage_button);
			this->TemplateList_groupBox->Controls->Add(this->DelTemplate_button);
			this->TemplateList_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->TemplateList_groupBox->Location = System::Drawing::Point(3, 3);
			this->TemplateList_groupBox->Name = L"TemplateList_groupBox";
			this->TemplateList_groupBox->Size = System::Drawing::Size(226, 518);
			this->TemplateList_groupBox->TabIndex = 2;
			this->TemplateList_groupBox->TabStop = false;
			this->TemplateList_groupBox->Text = L"ObjectTemplateList/範本列表";
			// 
			// NoImage_button
			// 
			this->NoImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->NoImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NoImage_button->Location = System::Drawing::Point(154, 262);
			this->NoImage_button->Name = L"NoImage_button";
			this->NoImage_button->Size = System::Drawing::Size(66, 29);
			this->NoImage_button->TabIndex = 61;
			this->NoImage_button->Text = L"NoImage";
			this->NoImage_button->UseVisualStyleBackColor = false;
			// 
			// ImageList_treeView
			// 
			this->ImageList_treeView->Location = System::Drawing::Point(13, 33);
			this->ImageList_treeView->Name = L"ImageList_treeView";
			this->ImageList_treeView->Size = System::Drawing::Size(194, 226);
			this->ImageList_treeView->TabIndex = 60;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Enabled = false;
			this->label5->Location = System::Drawing::Point(7, 300);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(79, 13);
			this->label5->TabIndex = 13;
			this->label5->Text = L"TemplateName";
			// 
			// TemplateName_textBox
			// 
			this->TemplateName_textBox->Location = System::Drawing::Point(139, 316);
			this->TemplateName_textBox->Margin = System::Windows::Forms::Padding(2);
			this->TemplateName_textBox->Name = L"TemplateName_textBox";
			this->TemplateName_textBox->Size = System::Drawing::Size(69, 20);
			this->TemplateName_textBox->TabIndex = 10;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 16);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(52, 13);
			this->label1->TabIndex = 24;
			this->label1->Text = L"ImageList";
			// 
			// Template_listBox
			// 
			this->Template_listBox->FormattingEnabled = true;
			this->Template_listBox->Location = System::Drawing::Point(5, 316);
			this->Template_listBox->Margin = System::Windows::Forms::Padding(2);
			this->Template_listBox->Name = L"Template_listBox";
			this->Template_listBox->Size = System::Drawing::Size(121, 186);
			this->Template_listBox->Sorted = true;
			this->Template_listBox->TabIndex = 14;
			// 
			// AddImage_button
			// 
			this->AddImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->AddImage_button->Location = System::Drawing::Point(5, 264);
			this->AddImage_button->Margin = System::Windows::Forms::Padding(2);
			this->AddImage_button->Name = L"AddImage_button";
			this->AddImage_button->Size = System::Drawing::Size(58, 24);
			this->AddImage_button->TabIndex = 23;
			this->AddImage_button->Text = L"Add/增加";
			this->AddImage_button->UseVisualStyleBackColor = false;
			// 
			// ReEditTemplate_button
			// 
			this->ReEditTemplate_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->ReEditTemplate_button->Location = System::Drawing::Point(139, 477);
			this->ReEditTemplate_button->Margin = System::Windows::Forms::Padding(2);
			this->ReEditTemplate_button->Name = L"ReEditTemplate_button";
			this->ReEditTemplate_button->Size = System::Drawing::Size(70, 24);
			this->ReEditTemplate_button->TabIndex = 39;
			this->ReEditTemplate_button->Text = L"ReEdit";
			this->ReEditTemplate_button->UseVisualStyleBackColor = false;
			// 
			// AddTemplate_button
			// 
			this->AddTemplate_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->AddTemplate_button->Location = System::Drawing::Point(140, 356);
			this->AddTemplate_button->Margin = System::Windows::Forms::Padding(2);
			this->AddTemplate_button->Name = L"AddTemplate_button";
			this->AddTemplate_button->Size = System::Drawing::Size(70, 24);
			this->AddTemplate_button->TabIndex = 17;
			this->AddTemplate_button->Text = L"Add";
			this->AddTemplate_button->UseVisualStyleBackColor = false;
			// 
			// DelImage_button
			// 
			this->DelImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->DelImage_button->Location = System::Drawing::Point(68, 264);
			this->DelImage_button->Margin = System::Windows::Forms::Padding(2);
			this->DelImage_button->Name = L"DelImage_button";
			this->DelImage_button->Size = System::Drawing::Size(58, 24);
			this->DelImage_button->TabIndex = 22;
			this->DelImage_button->Text = L"Del/移除";
			this->DelImage_button->UseVisualStyleBackColor = false;
			// 
			// DelTemplate_button
			// 
			this->DelTemplate_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->DelTemplate_button->Location = System::Drawing::Point(139, 423);
			this->DelTemplate_button->Margin = System::Windows::Forms::Padding(2);
			this->DelTemplate_button->Name = L"DelTemplate_button";
			this->DelTemplate_button->Size = System::Drawing::Size(70, 24);
			this->DelTemplate_button->TabIndex = 16;
			this->DelTemplate_button->Text = L"Del";
			this->DelTemplate_button->UseVisualStyleBackColor = false;
			// 
			// XMLData_groupBox
			// 
			this->XMLData_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->XMLData_groupBox->Controls->Add(this->XMLDataForQuickApply_treeView);
			this->XMLData_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->XMLData_groupBox->Location = System::Drawing::Point(3, 527);
			this->XMLData_groupBox->Name = L"XMLData_groupBox";
			this->XMLData_groupBox->Size = System::Drawing::Size(225, 343);
			this->XMLData_groupBox->TabIndex = 45;
			this->XMLData_groupBox->TabStop = false;
			this->XMLData_groupBox->Text = L"XMLDataForQuickApply";
			// 
			// XMLDataForQuickApply_treeView
			// 
			this->XMLDataForQuickApply_treeView->AllowDrop = true;
			this->XMLDataForQuickApply_treeView->ContextMenuStrip = this->UserDefineData__contextMenuStrip;
			this->XMLDataForQuickApply_treeView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->XMLDataForQuickApply_treeView->Location = System::Drawing::Point(3, 16);
			this->XMLDataForQuickApply_treeView->Margin = System::Windows::Forms::Padding(2);
			this->XMLDataForQuickApply_treeView->Name = L"XMLDataForQuickApply_treeView";
			this->XMLDataForQuickApply_treeView->Size = System::Drawing::Size(219, 324);
			this->XMLDataForQuickApply_treeView->TabIndex = 0;
			// 
			// UserDefineData__contextMenuStrip
			// 
			this->UserDefineData__contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->refreshToolStripMenuItem, 
				this->addNewNodeFromFileToolStripMenuItem, this->deleteNodeToolStripMenuItem, this->assignToObjectEventDataToolStripMenuItem});
			this->UserDefineData__contextMenuStrip->Name = L"contextMenuStrip1";
			this->UserDefineData__contextMenuStrip->Size = System::Drawing::Size(231, 92);
			// 
			// refreshToolStripMenuItem
			// 
			this->refreshToolStripMenuItem->Name = L"refreshToolStripMenuItem";
			this->refreshToolStripMenuItem->Size = System::Drawing::Size(230, 22);
			this->refreshToolStripMenuItem->Text = L"AssignToObjectTemplateData";
			// 
			// addNewNodeFromFileToolStripMenuItem
			// 
			this->addNewNodeFromFileToolStripMenuItem->Name = L"addNewNodeFromFileToolStripMenuItem";
			this->addNewNodeFromFileToolStripMenuItem->Size = System::Drawing::Size(230, 22);
			this->addNewNodeFromFileToolStripMenuItem->Text = L"AddNewNodeFromFile";
			// 
			// deleteNodeToolStripMenuItem
			// 
			this->deleteNodeToolStripMenuItem->Name = L"deleteNodeToolStripMenuItem";
			this->deleteNodeToolStripMenuItem->Size = System::Drawing::Size(230, 22);
			this->deleteNodeToolStripMenuItem->Text = L"DeleteNode";
			// 
			// assignToObjectEventDataToolStripMenuItem
			// 
			this->assignToObjectEventDataToolStripMenuItem->Name = L"assignToObjectEventDataToolStripMenuItem";
			this->assignToObjectEventDataToolStripMenuItem->Size = System::Drawing::Size(230, 22);
			this->assignToObjectEventDataToolStripMenuItem->Text = L"AssignToObjectEventData";
			// 
			// splitContainer2
			// 
			this->splitContainer2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->tabControl1);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->Main_splitContainer);
			this->splitContainer2->Size = System::Drawing::Size(1661, 1015);
			this->splitContainer2->SplitterDistance = 348;
			this->splitContainer2->SplitterWidth = 2;
			this->splitContainer2->TabIndex = 0;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(1657, 344);
			this->tabControl1->TabIndex = 43;
			// 
			// tabPage1
			// 
			this->tabPage1->AllowDrop = true;
			this->tabPage1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->tabPage1->Controls->Add(this->LayerList_groupBox);
			this->tabPage1->Controls->Add(this->StageData_groupBox);
			this->tabPage1->Controls->Add(this->groupBox1);
			this->tabPage1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(1649, 318);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"StageData";
			// 
			// LayerList_groupBox
			// 
			this->LayerList_groupBox->Controls->Add(this->InGameRender_checkBox);
			this->LayerList_groupBox->Controls->Add(this->label17);
			this->LayerList_groupBox->Controls->Add(this->Layer_listBox);
			this->LayerList_groupBox->Controls->Add(this->LayerName_textBox);
			this->LayerList_groupBox->Controls->Add(this->LayerUp_button);
			this->LayerList_groupBox->Controls->Add(this->DeleteLayer_button);
			this->LayerList_groupBox->Controls->Add(this->LayerDown_button);
			this->LayerList_groupBox->Controls->Add(this->AddToLayer_button);
			this->LayerList_groupBox->Controls->Add(this->LayerVisible_checkBox);
			this->LayerList_groupBox->Location = System::Drawing::Point(6, 3);
			this->LayerList_groupBox->Name = L"LayerList_groupBox";
			this->LayerList_groupBox->Size = System::Drawing::Size(293, 309);
			this->LayerList_groupBox->TabIndex = 39;
			this->LayerList_groupBox->TabStop = false;
			this->LayerList_groupBox->Text = L"Layer List";
			// 
			// InGameRender_checkBox
			// 
			this->InGameRender_checkBox->AutoSize = true;
			this->InGameRender_checkBox->Location = System::Drawing::Point(8, 170);
			this->InGameRender_checkBox->Name = L"InGameRender_checkBox";
			this->InGameRender_checkBox->Size = System::Drawing::Size(98, 17);
			this->InGameRender_checkBox->TabIndex = 69;
			this->InGameRender_checkBox->Text = L"InGameRender";
			this->InGameRender_checkBox->UseVisualStyleBackColor = true;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(5, 20);
			this->label17->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(35, 13);
			this->label17->TabIndex = 63;
			this->label17->Text = L"Name";
			// 
			// Layer_listBox
			// 
			this->Layer_listBox->FormattingEnabled = true;
			this->Layer_listBox->Location = System::Drawing::Point(114, 30);
			this->Layer_listBox->Margin = System::Windows::Forms::Padding(2);
			this->Layer_listBox->Name = L"Layer_listBox";
			this->Layer_listBox->Size = System::Drawing::Size(171, 212);
			this->Layer_listBox->TabIndex = 45;
			// 
			// LayerName_textBox
			// 
			this->LayerName_textBox->Location = System::Drawing::Point(6, 36);
			this->LayerName_textBox->Name = L"LayerName_textBox";
			this->LayerName_textBox->Size = System::Drawing::Size(100, 20);
			this->LayerName_textBox->TabIndex = 62;
			// 
			// LayerUp_button
			// 
			this->LayerUp_button->Location = System::Drawing::Point(90, 131);
			this->LayerUp_button->Margin = System::Windows::Forms::Padding(2);
			this->LayerUp_button->Name = L"LayerUp_button";
			this->LayerUp_button->Size = System::Drawing::Size(20, 42);
			this->LayerUp_button->TabIndex = 46;
			this->LayerUp_button->Text = L"^\r\n^";
			this->LayerUp_button->UseVisualStyleBackColor = true;
			// 
			// DeleteLayer_button
			// 
			this->DeleteLayer_button->Location = System::Drawing::Point(7, 103);
			this->DeleteLayer_button->Margin = System::Windows::Forms::Padding(2);
			this->DeleteLayer_button->Name = L"DeleteLayer_button";
			this->DeleteLayer_button->Size = System::Drawing::Size(82, 24);
			this->DeleteLayer_button->TabIndex = 49;
			this->DeleteLayer_button->Text = L"DeleteLayer";
			this->DeleteLayer_button->UseVisualStyleBackColor = true;
			// 
			// LayerDown_button
			// 
			this->LayerDown_button->Location = System::Drawing::Point(91, 189);
			this->LayerDown_button->Margin = System::Windows::Forms::Padding(2);
			this->LayerDown_button->Name = L"LayerDown_button";
			this->LayerDown_button->Size = System::Drawing::Size(20, 37);
			this->LayerDown_button->TabIndex = 47;
			this->LayerDown_button->Text = L"v\r\nv";
			this->LayerDown_button->UseVisualStyleBackColor = true;
			// 
			// AddToLayer_button
			// 
			this->AddToLayer_button->Location = System::Drawing::Point(6, 65);
			this->AddToLayer_button->Margin = System::Windows::Forms::Padding(2);
			this->AddToLayer_button->Name = L"AddToLayer_button";
			this->AddToLayer_button->Size = System::Drawing::Size(83, 24);
			this->AddToLayer_button->TabIndex = 48;
			this->AddToLayer_button->Text = L"AddToLayer";
			this->AddToLayer_button->UseVisualStyleBackColor = true;
			// 
			// LayerVisible_checkBox
			// 
			this->LayerVisible_checkBox->AutoSize = true;
			this->LayerVisible_checkBox->Location = System::Drawing::Point(6, 141);
			this->LayerVisible_checkBox->Name = L"LayerVisible_checkBox";
			this->LayerVisible_checkBox->Size = System::Drawing::Size(82, 17);
			this->LayerVisible_checkBox->TabIndex = 68;
			this->LayerVisible_checkBox->Text = L"LayerVisible";
			this->LayerVisible_checkBox->UseVisualStyleBackColor = true;
			// 
			// StageData_groupBox
			// 
			this->StageData_groupBox->Controls->Add(this->label4);
			this->StageData_groupBox->Controls->Add(this->label10);
			this->StageData_groupBox->Controls->Add(this->StageName_textBox);
			this->StageData_groupBox->Controls->Add(this->StartY_numericUpDown);
			this->StageData_groupBox->Controls->Add(this->AllStage_listBox);
			this->StageData_groupBox->Controls->Add(this->MapWidth_numericUpDown);
			this->StageData_groupBox->Controls->Add(this->DelStage_button);
			this->StageData_groupBox->Controls->Add(this->StartX_numericUpDown);
			this->StageData_groupBox->Controls->Add(this->ReRditStagebutton);
			this->StageData_groupBox->Controls->Add(this->MapHeight_numericUpDown);
			this->StageData_groupBox->Controls->Add(this->AddStage_button);
			this->StageData_groupBox->Controls->Add(this->label3);
			this->StageData_groupBox->Controls->Add(this->GridWidth_numericUpDown);
			this->StageData_groupBox->Controls->Add(this->label2);
			this->StageData_groupBox->Controls->Add(this->label6);
			this->StageData_groupBox->Controls->Add(this->GridHeight_numericUpDown);
			this->StageData_groupBox->Controls->Add(this->label7);
			this->StageData_groupBox->Location = System::Drawing::Point(962, 10);
			this->StageData_groupBox->Name = L"StageData_groupBox";
			this->StageData_groupBox->Size = System::Drawing::Size(408, 242);
			this->StageData_groupBox->TabIndex = 67;
			this->StageData_groupBox->TabStop = false;
			this->StageData_groupBox->Text = L"StageData";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Enabled = false;
			this->label4->Location = System::Drawing::Point(240, 35);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(36, 13);
			this->label4->TabIndex = 43;
			this->label4->Text = L"StartX";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Enabled = false;
			this->label10->Location = System::Drawing::Point(239, 61);
			this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(36, 13);
			this->label10->TabIndex = 44;
			this->label10->Text = L"StartY";
			// 
			// StageName_textBox
			// 
			this->StageName_textBox->Location = System::Drawing::Point(5, 18);
			this->StageName_textBox->Margin = System::Windows::Forms::Padding(2);
			this->StageName_textBox->Name = L"StageName_textBox";
			this->StageName_textBox->Size = System::Drawing::Size(226, 20);
			this->StageName_textBox->TabIndex = 36;
			// 
			// StartY_numericUpDown
			// 
			this->StartY_numericUpDown->Location = System::Drawing::Point(299, 53);
			this->StartY_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->StartY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->StartY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, System::Int32::MinValue});
			this->StartY_numericUpDown->Name = L"StartY_numericUpDown";
			this->StartY_numericUpDown->Size = System::Drawing::Size(65, 20);
			this->StartY_numericUpDown->TabIndex = 42;
			// 
			// AllStage_listBox
			// 
			this->AllStage_listBox->FormattingEnabled = true;
			this->AllStage_listBox->Location = System::Drawing::Point(5, 77);
			this->AllStage_listBox->Margin = System::Windows::Forms::Padding(2);
			this->AllStage_listBox->Name = L"AllStage_listBox";
			this->AllStage_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AllStage_listBox->Size = System::Drawing::Size(226, 147);
			this->AllStage_listBox->Sorted = true;
			this->AllStage_listBox->TabIndex = 35;
			// 
			// MapWidth_numericUpDown
			// 
			this->MapWidth_numericUpDown->Location = System::Drawing::Point(299, 146);
			this->MapWidth_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->MapWidth_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->MapWidth_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->MapWidth_numericUpDown->Name = L"MapWidth_numericUpDown";
			this->MapWidth_numericUpDown->Size = System::Drawing::Size(65, 20);
			this->MapWidth_numericUpDown->TabIndex = 29;
			this->MapWidth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {480, 0, 0, 0});
			// 
			// DelStage_button
			// 
			this->DelStage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->DelStage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelStage_button->Location = System::Drawing::Point(89, 47);
			this->DelStage_button->Margin = System::Windows::Forms::Padding(2);
			this->DelStage_button->Name = L"DelStage_button";
			this->DelStage_button->Size = System::Drawing::Size(58, 24);
			this->DelStage_button->TabIndex = 37;
			this->DelStage_button->Text = L"Del";
			this->DelStage_button->UseVisualStyleBackColor = false;
			// 
			// StartX_numericUpDown
			// 
			this->StartX_numericUpDown->Location = System::Drawing::Point(299, 24);
			this->StartX_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->StartX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->StartX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, System::Int32::MinValue});
			this->StartX_numericUpDown->Name = L"StartX_numericUpDown";
			this->StartX_numericUpDown->Size = System::Drawing::Size(65, 20);
			this->StartX_numericUpDown->TabIndex = 41;
			// 
			// ReRditStagebutton
			// 
			this->ReRditStagebutton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->ReRditStagebutton->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ReRditStagebutton->Location = System::Drawing::Point(163, 47);
			this->ReRditStagebutton->Margin = System::Windows::Forms::Padding(2);
			this->ReRditStagebutton->Name = L"ReRditStagebutton";
			this->ReRditStagebutton->Size = System::Drawing::Size(68, 24);
			this->ReRditStagebutton->TabIndex = 39;
			this->ReRditStagebutton->Text = L"Reedit";
			this->ReRditStagebutton->UseVisualStyleBackColor = false;
			// 
			// MapHeight_numericUpDown
			// 
			this->MapHeight_numericUpDown->Location = System::Drawing::Point(300, 181);
			this->MapHeight_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->MapHeight_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->MapHeight_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->MapHeight_numericUpDown->Name = L"MapHeight_numericUpDown";
			this->MapHeight_numericUpDown->Size = System::Drawing::Size(65, 20);
			this->MapHeight_numericUpDown->TabIndex = 30;
			this->MapHeight_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {320, 0, 0, 0});
			// 
			// AddStage_button
			// 
			this->AddStage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AddStage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddStage_button->Location = System::Drawing::Point(16, 47);
			this->AddStage_button->Margin = System::Windows::Forms::Padding(2);
			this->AddStage_button->Name = L"AddStage_button";
			this->AddStage_button->Size = System::Drawing::Size(58, 24);
			this->AddStage_button->TabIndex = 38;
			this->AddStage_button->Text = L"Add";
			this->AddStage_button->UseVisualStyleBackColor = false;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Enabled = false;
			this->label3->Location = System::Drawing::Point(239, 86);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(54, 13);
			this->label3->TabIndex = 28;
			this->label3->Text = L"GridWidth";
			// 
			// GridWidth_numericUpDown
			// 
			this->GridWidth_numericUpDown->Location = System::Drawing::Point(299, 83);
			this->GridWidth_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->GridWidth_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->GridWidth_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->GridWidth_numericUpDown->Name = L"GridWidth_numericUpDown";
			this->GridWidth_numericUpDown->Size = System::Drawing::Size(65, 20);
			this->GridWidth_numericUpDown->TabIndex = 26;
			this->GridWidth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Enabled = false;
			this->label2->Location = System::Drawing::Point(239, 120);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(57, 13);
			this->label2->TabIndex = 27;
			this->label2->Text = L"GridHeight";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Enabled = false;
			this->label6->Location = System::Drawing::Point(239, 151);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(56, 13);
			this->label6->TabIndex = 32;
			this->label6->Text = L"MapWidth";
			// 
			// GridHeight_numericUpDown
			// 
			this->GridHeight_numericUpDown->Location = System::Drawing::Point(299, 116);
			this->GridHeight_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->GridHeight_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->GridHeight_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->GridHeight_numericUpDown->Name = L"GridHeight_numericUpDown";
			this->GridHeight_numericUpDown->Size = System::Drawing::Size(65, 20);
			this->GridHeight_numericUpDown->TabIndex = 25;
			this->GridHeight_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Enabled = false;
			this->label7->Location = System::Drawing::Point(239, 181);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(59, 13);
			this->label7->TabIndex = 31;
			this->label7->Text = L"MapHeight";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->LevelLayerGridEventDataNode_xmlNodeEditComponent);
			this->groupBox1->Controls->Add(this->Object_groupBox);
			this->groupBox1->Controls->Add(this->MouseMode_label);
			this->groupBox1->Controls->Add(this->PlaceObjectByImageCenter_checkBox);
			this->groupBox1->Controls->Add(this->MouseMode_comboBox);
			this->groupBox1->Controls->Add(this->LayerObjectData_listBox);
			this->groupBox1->Controls->Add(this->AllowSamepositionObject_checkBox);
			this->groupBox1->Location = System::Drawing::Point(305, 3);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(657, 309);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"LayerObjectData";
			// 
			// LevelLayerGridEventDataNode_xmlNodeEditComponent
			// 
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->AllowDrop = true;
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->CollpaseButtonName = L"EventData";
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->CollpaseXMlElementValuePanel = true;
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->Location = System::Drawing::Point(249, -3);
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->Margin = System::Windows::Forms::Padding(0);
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->Name = L"LevelLayerGridEventDataNode_xmlNodeEditComponent";
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->Size = System::Drawing::Size(405, 308);
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->TabIndex = 26;
			// 
			// Object_groupBox
			// 
			this->Object_groupBox->Controls->Add(this->AddObjectX_numericUpDown);
			this->Object_groupBox->Controls->Add(this->AddObjectToStage_button);
			this->Object_groupBox->Controls->Add(this->Clear_button);
			this->Object_groupBox->Controls->Add(this->label14);
			this->Object_groupBox->Controls->Add(this->AddObjectZ_numericUpDown);
			this->Object_groupBox->Controls->Add(this->DeleteObject_button);
			this->Object_groupBox->Controls->Add(this->label13);
			this->Object_groupBox->Controls->Add(this->label15);
			this->Object_groupBox->Controls->Add(this->AddObjectY_numericUpDown);
			this->Object_groupBox->Location = System::Drawing::Point(6, 17);
			this->Object_groupBox->Name = L"Object_groupBox";
			this->Object_groupBox->Size = System::Drawing::Size(80, 218);
			this->Object_groupBox->TabIndex = 0;
			this->Object_groupBox->TabStop = false;
			// 
			// AddObjectX_numericUpDown
			// 
			this->AddObjectX_numericUpDown->Location = System::Drawing::Point(24, 22);
			this->AddObjectX_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->AddObjectX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->AddObjectX_numericUpDown->Name = L"AddObjectX_numericUpDown";
			this->AddObjectX_numericUpDown->Size = System::Drawing::Size(47, 20);
			this->AddObjectX_numericUpDown->TabIndex = 53;
			// 
			// AddObjectToStage_button
			// 
			this->AddObjectToStage_button->Location = System::Drawing::Point(7, 113);
			this->AddObjectToStage_button->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectToStage_button->Name = L"AddObjectToStage_button";
			this->AddObjectToStage_button->Size = System::Drawing::Size(64, 24);
			this->AddObjectToStage_button->TabIndex = 60;
			this->AddObjectToStage_button->Text = L"AddObject";
			this->AddObjectToStage_button->UseVisualStyleBackColor = true;
			// 
			// Clear_button
			// 
			this->Clear_button->Location = System::Drawing::Point(5, 169);
			this->Clear_button->Margin = System::Windows::Forms::Padding(2);
			this->Clear_button->Name = L"Clear_button";
			this->Clear_button->Size = System::Drawing::Size(70, 24);
			this->Clear_button->TabIndex = 38;
			this->Clear_button->Text = L"Clear";
			this->Clear_button->UseVisualStyleBackColor = true;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(8, 78);
			this->label14->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(12, 13);
			this->label14->TabIndex = 65;
			this->label14->Text = L"z";
			// 
			// AddObjectZ_numericUpDown
			// 
			this->AddObjectZ_numericUpDown->Location = System::Drawing::Point(23, 76);
			this->AddObjectZ_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->AddObjectZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->AddObjectZ_numericUpDown->Name = L"AddObjectZ_numericUpDown";
			this->AddObjectZ_numericUpDown->Size = System::Drawing::Size(47, 20);
			this->AddObjectZ_numericUpDown->TabIndex = 64;
			// 
			// DeleteObject_button
			// 
			this->DeleteObject_button->Location = System::Drawing::Point(6, 142);
			this->DeleteObject_button->Margin = System::Windows::Forms::Padding(2);
			this->DeleteObject_button->Name = L"DeleteObject_button";
			this->DeleteObject_button->Size = System::Drawing::Size(70, 23);
			this->DeleteObject_button->TabIndex = 1;
			this->DeleteObject_button->Text = L"Delete";
			this->DeleteObject_button->UseVisualStyleBackColor = true;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(9, 22);
			this->label13->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(12, 13);
			this->label13->TabIndex = 56;
			this->label13->Text = L"x";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(9, 51);
			this->label15->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(12, 13);
			this->label15->TabIndex = 58;
			this->label15->Text = L"y";
			// 
			// AddObjectY_numericUpDown
			// 
			this->AddObjectY_numericUpDown->Location = System::Drawing::Point(24, 49);
			this->AddObjectY_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->AddObjectY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->AddObjectY_numericUpDown->Name = L"AddObjectY_numericUpDown";
			this->AddObjectY_numericUpDown->Size = System::Drawing::Size(47, 20);
			this->AddObjectY_numericUpDown->TabIndex = 55;
			// 
			// MouseMode_label
			// 
			this->MouseMode_label->AutoSize = true;
			this->MouseMode_label->Location = System::Drawing::Point(93, 197);
			this->MouseMode_label->Name = L"MouseMode_label";
			this->MouseMode_label->Size = System::Drawing::Size(66, 13);
			this->MouseMode_label->TabIndex = 70;
			this->MouseMode_label->Text = L"MouseMode";
			// 
			// PlaceObjectByImageCenter_checkBox
			// 
			this->PlaceObjectByImageCenter_checkBox->AutoSize = true;
			this->PlaceObjectByImageCenter_checkBox->Location = System::Drawing::Point(17, 245);
			this->PlaceObjectByImageCenter_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->PlaceObjectByImageCenter_checkBox->Name = L"PlaceObjectByImageCenter_checkBox";
			this->PlaceObjectByImageCenter_checkBox->Size = System::Drawing::Size(156, 17);
			this->PlaceObjectByImageCenter_checkBox->TabIndex = 66;
			this->PlaceObjectByImageCenter_checkBox->Text = L"PlaceObjectByImageCenter";
			this->PlaceObjectByImageCenter_checkBox->UseVisualStyleBackColor = true;
			// 
			// MouseMode_comboBox
			// 
			this->MouseMode_comboBox->FormattingEnabled = true;
			this->MouseMode_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Add", L"Move", L"Erase"});
			this->MouseMode_comboBox->Location = System::Drawing::Point(95, 213);
			this->MouseMode_comboBox->Name = L"MouseMode_comboBox";
			this->MouseMode_comboBox->Size = System::Drawing::Size(89, 21);
			this->MouseMode_comboBox->TabIndex = 69;
			// 
			// LayerObjectData_listBox
			// 
			this->LayerObjectData_listBox->FormattingEnabled = true;
			this->LayerObjectData_listBox->HorizontalScrollbar = true;
			this->LayerObjectData_listBox->Location = System::Drawing::Point(92, 14);
			this->LayerObjectData_listBox->Margin = System::Windows::Forms::Padding(2);
			this->LayerObjectData_listBox->Name = L"LayerObjectData_listBox";
			this->LayerObjectData_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->LayerObjectData_listBox->Size = System::Drawing::Size(155, 173);
			this->LayerObjectData_listBox->TabIndex = 0;
			// 
			// AllowSamepositionObject_checkBox
			// 
			this->AllowSamepositionObject_checkBox->AutoSize = true;
			this->AllowSamepositionObject_checkBox->Location = System::Drawing::Point(17, 263);
			this->AllowSamepositionObject_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->AllowSamepositionObject_checkBox->Name = L"AllowSamepositionObject_checkBox";
			this->AllowSamepositionObject_checkBox->Size = System::Drawing::Size(145, 17);
			this->AllowSamepositionObject_checkBox->TabIndex = 36;
			this->AllowSamepositionObject_checkBox->Text = L"AllowSamepositionObject";
			this->AllowSamepositionObject_checkBox->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->AutoScroll = true;
			this->tabPage2->Controls->Add(this->EventData_flowLayoutPanel);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(1649, 318);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"EventData";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// EventData_flowLayoutPanel
			// 
			this->EventData_flowLayoutPanel->Controls->Add(this->ObjectTemplate_xmlNodeEditComponent);
			this->EventData_flowLayoutPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->EventData_flowLayoutPanel->Location = System::Drawing::Point(3, 3);
			this->EventData_flowLayoutPanel->Name = L"EventData_flowLayoutPanel";
			this->EventData_flowLayoutPanel->Size = System::Drawing::Size(1643, 312);
			this->EventData_flowLayoutPanel->TabIndex = 46;
			// 
			// ObjectTemplate_xmlNodeEditComponent
			// 
			this->ObjectTemplate_xmlNodeEditComponent->AllowDrop = true;
			this->ObjectTemplate_xmlNodeEditComponent->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ObjectTemplate_xmlNodeEditComponent->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->ObjectTemplate_xmlNodeEditComponent->CollpaseButtonName = L"ObjectTemplateData";
			this->ObjectTemplate_xmlNodeEditComponent->CollpaseXMlElementValuePanel = true;
			this->ObjectTemplate_xmlNodeEditComponent->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ObjectTemplate_xmlNodeEditComponent->Location = System::Drawing::Point(0, 0);
			this->ObjectTemplate_xmlNodeEditComponent->Margin = System::Windows::Forms::Padding(0);
			this->ObjectTemplate_xmlNodeEditComponent->Name = L"ObjectTemplate_xmlNodeEditComponent";
			this->ObjectTemplate_xmlNodeEditComponent->Size = System::Drawing::Size(292, 314);
			this->ObjectTemplate_xmlNodeEditComponent->TabIndex = 24;
			// 
			// Main_splitContainer
			// 
			this->Main_splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Main_splitContainer->Location = System::Drawing::Point(0, 0);
			this->Main_splitContainer->Name = L"Main_splitContainer";
			this->Main_splitContainer->Panel2Collapsed = true;
			this->Main_splitContainer->Size = System::Drawing::Size(1657, 661);
			this->Main_splitContainer->SplitterDistance = 560;
			this->Main_splitContainer->TabIndex = 0;
			// 
			// assistanceToolStripMenuItem
			// 
			this->assistanceToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->showGridToolStripMenuItem, 
				this->autoAllignGridToolStripMenuItem, this->camerResetToolStripMenuItem, this->showResolutionBorderToolStripMenuItem, this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem, 
				this->showWarningMsgToolStripMenuItem});
			this->assistanceToolStripMenuItem->Name = L"assistanceToolStripMenuItem";
			this->assistanceToolStripMenuItem->Size = System::Drawing::Size(74, 20);
			this->assistanceToolStripMenuItem->Text = L"Assistance";
			// 
			// showGridToolStripMenuItem
			// 
			this->showGridToolStripMenuItem->Checked = true;
			this->showGridToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showGridToolStripMenuItem->Name = L"showGridToolStripMenuItem";
			this->showGridToolStripMenuItem->Size = System::Drawing::Size(309, 22);
			this->showGridToolStripMenuItem->Text = L"ShowGrid";
			// 
			// autoAllignGridToolStripMenuItem
			// 
			this->autoAllignGridToolStripMenuItem->Checked = true;
			this->autoAllignGridToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->autoAllignGridToolStripMenuItem->Name = L"autoAllignGridToolStripMenuItem";
			this->autoAllignGridToolStripMenuItem->Size = System::Drawing::Size(309, 22);
			this->autoAllignGridToolStripMenuItem->Text = L"AutoAllignGrid";
			// 
			// camerResetToolStripMenuItem
			// 
			this->camerResetToolStripMenuItem->Name = L"camerResetToolStripMenuItem";
			this->camerResetToolStripMenuItem->Size = System::Drawing::Size(309, 22);
			this->camerResetToolStripMenuItem->Text = L"CamerReset";
			// 
			// showResolutionBorderToolStripMenuItem
			// 
			this->showResolutionBorderToolStripMenuItem->Checked = true;
			this->showResolutionBorderToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showResolutionBorderToolStripMenuItem->Name = L"showResolutionBorderToolStripMenuItem";
			this->showResolutionBorderToolStripMenuItem->Size = System::Drawing::Size(309, 22);
			this->showResolutionBorderToolStripMenuItem->Text = L"ShowResolutionBorder";
			// 
			// autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem
			// 
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Name = L"autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem";
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Size = System::Drawing::Size(309, 22);
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Text = L"AutoReadjustPositionWihleGridSizeIsChange";
			// 
			// showWarningMsgToolStripMenuItem
			// 
			this->showWarningMsgToolStripMenuItem->Name = L"showWarningMsgToolStripMenuItem";
			this->showWarningMsgToolStripMenuItem->Size = System::Drawing::Size(309, 22);
			this->showWarningMsgToolStripMenuItem->Text = L"ShowWarningMsg";
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->fileToolStripMenuItem, 
				this->imageToolStripMenuItem, this->imageToolStripMenuItem1, this->assistanceToolStripMenuItem, this->cameraResetToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(1900, 24);
			this->menuStrip1->TabIndex = 5;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
				this->saveToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->openToolStripMenuItem->Text = L"Open";
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			// 
			// imageToolStripMenuItem
			// 
			this->imageToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->openTemplateToolStripMenuItem});
			this->imageToolStripMenuItem->Name = L"imageToolStripMenuItem";
			this->imageToolStripMenuItem->Size = System::Drawing::Size(40, 20);
			this->imageToolStripMenuItem->Text = L"Xml";
			// 
			// openTemplateToolStripMenuItem
			// 
			this->openTemplateToolStripMenuItem->Name = L"openTemplateToolStripMenuItem";
			this->openTemplateToolStripMenuItem->Size = System::Drawing::Size(195, 22);
			this->openTemplateToolStripMenuItem->Text = L"AddNewNodeFromFile";
			// 
			// imageToolStripMenuItem1
			// 
			this->imageToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->backgroundImageToolStripMenuItem});
			this->imageToolStripMenuItem1->Name = L"imageToolStripMenuItem1";
			this->imageToolStripMenuItem1->Size = System::Drawing::Size(52, 20);
			this->imageToolStripMenuItem1->Text = L"Image";
			// 
			// backgroundImageToolStripMenuItem
			// 
			this->backgroundImageToolStripMenuItem->Name = L"backgroundImageToolStripMenuItem";
			this->backgroundImageToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->backgroundImageToolStripMenuItem->Text = L"BackgroundImage";
			// 
			// cameraResetToolStripMenuItem
			// 
			this->cameraResetToolStripMenuItem->Name = L"cameraResetToolStripMenuItem";
			this->cameraResetToolStripMenuItem->Size = System::Drawing::Size(88, 20);
			this->cameraResetToolStripMenuItem->Text = L"CameraReset";
			// 
			// UserDefineData_contextMenuStrip
			// 
			this->UserDefineData_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->addToolStripMenuItem, 
				this->editToolStripMenuItem, this->deleteToolStripMenuItem});
			this->UserDefineData_contextMenuStrip->Name = L"UserDefineData_contextMenuStrip";
			this->UserDefineData_contextMenuStrip->Size = System::Drawing::Size(108, 70);
			// 
			// addToolStripMenuItem
			// 
			this->addToolStripMenuItem->Name = L"addToolStripMenuItem";
			this->addToolStripMenuItem->Size = System::Drawing::Size(107, 22);
			this->addToolStripMenuItem->Text = L"Add";
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(107, 22);
			this->editToolStripMenuItem->Text = L"Edit";
			// 
			// deleteToolStripMenuItem
			// 
			this->deleteToolStripMenuItem->Name = L"deleteToolStripMenuItem";
			this->deleteToolStripMenuItem->Size = System::Drawing::Size(107, 22);
			this->deleteToolStripMenuItem->Text = L"Delete";
			// 
			// LayerData_contextMenuStrip
			// 
			this->LayerData_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->selectAllThisTypeTemplateToolStripMenuItem, 
				this->refreshUserDataFromToolStripMenuItem});
			this->LayerData_contextMenuStrip->Name = L"LayerData_contextMenuStrip";
			this->LayerData_contextMenuStrip->Size = System::Drawing::Size(237, 48);
			// 
			// selectAllThisTypeTemplateToolStripMenuItem
			// 
			this->selectAllThisTypeTemplateToolStripMenuItem->Name = L"selectAllThisTypeTemplateToolStripMenuItem";
			this->selectAllThisTypeTemplateToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->selectAllThisTypeTemplateToolStripMenuItem->Text = L"SelectAllThisTypeTemplate";
			// 
			// refreshUserDataFromToolStripMenuItem
			// 
			this->refreshUserDataFromToolStripMenuItem->Name = L"refreshUserDataFromToolStripMenuItem";
			this->refreshUserDataFromToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->refreshUserDataFromToolStripMenuItem->Text = L"RefreshUserDataFromXMLData";
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			// 
			// ObjectDataForm
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"ObjectDataForm";
			this->Size = System::Drawing::Size(1900, 1041);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->flowLayoutPanel1->ResumeLayout(false);
			this->TemplateList_groupBox->ResumeLayout(false);
			this->TemplateList_groupBox->PerformLayout();
			this->XMLData_groupBox->ResumeLayout(false);
			this->UserDefineData__contextMenuStrip->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer2))->EndInit();
			this->splitContainer2->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->LayerList_groupBox->ResumeLayout(false);
			this->LayerList_groupBox->PerformLayout();
			this->StageData_groupBox->ResumeLayout(false);
			this->StageData_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MapWidth_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MapHeight_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridWidth_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridHeight_numericUpDown))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->Object_groupBox->ResumeLayout(false);
			this->Object_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectZ_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectY_numericUpDown))->EndInit();
			this->tabPage2->ResumeLayout(false);
			this->EventData_flowLayoutPanel->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Main_splitContainer))->EndInit();
			this->Main_splitContainer->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->UserDefineData_contextMenuStrip->ResumeLayout(false);
			this->LayerData_contextMenuStrip->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void LayerVisible_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 			this->LayerVisible_checkBox->CheckedChanged += gcnew System::EventHandler(this, &ObjectDataForm::LayerVisible_checkBox_CheckedChanged);
//			 Layer_listBox;
		 }
};
}