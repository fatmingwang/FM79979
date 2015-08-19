#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
extern float	DRAW_PANEL_RESOLUTION_WIDTH;
extern float	DRAW_PANEL_RESOLUTION_HEIGHT;
//need name to fetch relate xml node to export data,and get image to assign to the new object
extern cMap_CellData*g_pForListBoxTemplateCellData;
extern cMap_CellData*g_pCurrentStageEditData;			//for current stage
extern cBaseImage*g_pCurrentSelectedImage;
extern cBaseImage*g_pBackgroundImage;
namespace LevelEditor {

	/// <summary>
	/// ObjectDataForm 的摘要
	/// </summary>
	public ref class ObjectDataForm : public System::Windows::Forms::UserControl
	{
	public:
		ObjectDataForm(GCFORM::TabControl^e_ptabControl,String^e_pTabText,GCFORM::Form^e_pForm)
		{
			InitializeComponent();
			m_pOrthogonalCamera = new OrthogonalCamera;
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_pOpenglFormControl = gcnew OpenglForm::OpenglFormControl(e_ptabControl,e_pTabText,e_pForm,this);
			this->splitContainer2->Panel2->Controls->Add(m_pOpenglFormControl);
			m_pOpenglFormControl->timer1->Tick += gcnew System::EventHandler(this, &ObjectDataForm::timer1_Tick);
			m_pOpenglFormControl->splitContainer1->Panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseMove);
			m_pOpenglFormControl->splitContainer1->Panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseDown);
			m_pOpenglFormControl->splitContainer1->Panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseUp);
			//make sure u hvae call focus while the panel is mouse down
			m_pOpenglFormControl->splitContainer1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseUp);

			m_pMapDataManager = new cMapDataManager;
			g_pCurrentStageEditData = new cMap_CellData;
			g_pForListBoxTemplateCellData = &m_pMapDataManager->m_ForListBoxTemplateCellData;
			m_TemplateNameAndAttributeNode = gcnew System::Collections::Hashtable;

			DotNetComponent::CollpaseForm^l_pCollpaseForm1 = gcnew DotNetComponent::CollpaseForm();
			l_pCollpaseForm1->SetupChild(treeView1,"ReferenceNode",flowLayoutPanel1->Controls,treeView1->Size);
			DotNetComponent::CollpaseForm^l_pCollpaseForm2 = gcnew DotNetComponent::CollpaseForm();
			l_pCollpaseForm2->SetupChild(xmlNodeEditComponent1,"Attribute",flowLayoutPanel1->Controls,xmlNodeEditComponent1->Size);
			DotNetComponent::CollpaseForm^l_pCollpaseForm3 = gcnew DotNetComponent::CollpaseForm();
			l_pCollpaseForm3->SetupChild(StageData_groupBox,StageData_groupBox->Text,flowLayoutPanel1->Controls,StageData_groupBox->Size);
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~ObjectDataForm()
		{
			if (components)
			{
				delete components;
			}
			SAFE_DELETE(m_pOrthogonalCamera);
			SAFE_DELETE(m_pMapDataManager);
			SAFE_DELETE(g_pCurrentStageEditData);
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::ListBox^  ImageList_listBox;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  imageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openTemplateToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  imageToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  assistanceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showGridToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  autoAllignGridToolStripMenuItem;
	private: System::Windows::Forms::TreeView^  treeView1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  refreshToolStripMenuItem;
	private: System::Windows::Forms::ListBox^  Template_listBox;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  Image_textBox;
	private: System::Windows::Forms::TextBox^  TemplateName_textBox;
	private: System::Windows::Forms::Button^  AddTemplate_button;
	private: System::Windows::Forms::Button^  DelTemplate_button;
	private: System::Windows::Forms::ToolStripMenuItem^  addNewNodeFromFileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  deleteNodeToolStripMenuItem;
	private: System::Windows::Forms::Button^  AddImage_button;
	private: System::Windows::Forms::Button^  DelImage_button;
	private: System::Windows::Forms::Label^  label1;
	private: DotNetComponent::XmlNodeEditComponent^  xmlNodeEditComponent1;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::NumericUpDown^  Column_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  Row_numericUpDown;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  GridWidth_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  GridHeight_numericUpDown;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::TextBox^  OutputData_textBox;
	private: System::Windows::Forms::ListBox^  AllStage_listBox;
	private: System::Windows::Forms::TextBox^  StageName_textBox;
	private: System::Windows::Forms::Button^  AddStage_button;
	private: System::Windows::Forms::Button^  DelStage_button;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::Button^  ReRditStagebutton;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::GroupBox^  StageData_groupBox;
	private: System::Windows::Forms::CheckBox^  AllowSamepositionObject_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  camerResetToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showResolutionBorderToolStripMenuItem;
	private: System::Windows::Forms::Label^  NumObjectInCurrentStagelabel_label;
	private: System::Windows::Forms::Button^  Clear_button;
	private: System::Windows::Forms::ToolStripMenuItem^  backgroundImageToolStripMenuItem;
	private: System::Windows::Forms::Button^  ReEditTemplate_button;
	private: System::Windows::Forms::CheckBox^  Erase_checkBox;
	protected://my
			OrthogonalCamera	*m_pOrthogonalCamera;
			OpenglForm::OpenglFormControl^m_pOpenglFormControl;
			cMapDataManager*m_pMapDataManager;
	private: System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
			 System::Void Render();
			 System::Void TemplateDataInvertorReEdit();
			 //String,TreeNodes
			 System::Collections::Hashtable^m_TemplateNameAndAttributeNode;
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
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->refreshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addNewNodeFromFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteNodeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->StageData_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->StageName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->ReRditStagebutton = (gcnew System::Windows::Forms::Button());
			this->DelStage_button = (gcnew System::Windows::Forms::Button());
			this->AddStage_button = (gcnew System::Windows::Forms::Button());
			this->AllStage_listBox = (gcnew System::Windows::Forms::ListBox());
			this->xmlNodeEditComponent1 = (gcnew DotNetComponent::XmlNodeEditComponent());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->Erase_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ReEditTemplate_button = (gcnew System::Windows::Forms::Button());
			this->Clear_button = (gcnew System::Windows::Forms::Button());
			this->NumObjectInCurrentStagelabel_label = (gcnew System::Windows::Forms::Label());
			this->AllowSamepositionObject_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->OutputData_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->Column_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Row_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->GridWidth_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GridHeight_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->Image_textBox = (gcnew System::Windows::Forms::TextBox());
			this->ImageList_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AddImage_button = (gcnew System::Windows::Forms::Button());
			this->TemplateName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->DelImage_button = (gcnew System::Windows::Forms::Button());
			this->AddTemplate_button = (gcnew System::Windows::Forms::Button());
			this->DelTemplate_button = (gcnew System::Windows::Forms::Button());
			this->Template_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->assistanceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showGridToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->autoAllignGridToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->camerResetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showResolutionBorderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openTemplateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imageToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->backgroundImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->flowLayoutPanel1->SuspendLayout();
			this->contextMenuStrip1->SuspendLayout();
			this->StageData_groupBox->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Column_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Row_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridWidth_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridHeight_numericUpDown))->BeginInit();
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
			this->splitContainer1->Panel1->Controls->Add(this->flowLayoutPanel1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
			this->splitContainer1->Size = System::Drawing::Size(1280, 998);
			this->splitContainer1->SplitterDistance = 293;
			this->splitContainer1->TabIndex = 0;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Controls->Add(this->treeView1);
			this->flowLayoutPanel1->Controls->Add(this->StageData_groupBox);
			this->flowLayoutPanel1->Controls->Add(this->xmlNodeEditComponent1);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(289, 994);
			this->flowLayoutPanel1->TabIndex = 0;
			// 
			// treeView1
			// 
			this->treeView1->AllowDrop = true;
			this->treeView1->ContextMenuStrip = this->contextMenuStrip1;
			this->treeView1->Location = System::Drawing::Point(3, 3);
			this->treeView1->Name = L"treeView1";
			this->treeView1->Size = System::Drawing::Size(280, 337);
			this->treeView1->TabIndex = 0;
			this->treeView1->DoubleClick += gcnew System::EventHandler(this, &ObjectDataForm::treeView1_DoubleClick);
			this->treeView1->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &ObjectDataForm::treeView1_DragDrop);
			this->treeView1->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &ObjectDataForm::treeView1_DragEnter);
			this->treeView1->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &ObjectDataForm::treeView1_KeyUp);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->refreshToolStripMenuItem, 
				this->addNewNodeFromFileToolStripMenuItem, this->deleteNodeToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(218, 70);
			// 
			// refreshToolStripMenuItem
			// 
			this->refreshToolStripMenuItem->Name = L"refreshToolStripMenuItem";
			this->refreshToolStripMenuItem->Size = System::Drawing::Size(217, 22);
			this->refreshToolStripMenuItem->Text = L"Assign";
			this->refreshToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::refreshToolStripMenuItem_Click);
			// 
			// addNewNodeFromFileToolStripMenuItem
			// 
			this->addNewNodeFromFileToolStripMenuItem->Name = L"addNewNodeFromFileToolStripMenuItem";
			this->addNewNodeFromFileToolStripMenuItem->Size = System::Drawing::Size(217, 22);
			this->addNewNodeFromFileToolStripMenuItem->Text = L"AddNewNodeFromFile";
			this->addNewNodeFromFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::addNewNodeFromFileToolStripMenuItem_Click);
			// 
			// deleteNodeToolStripMenuItem
			// 
			this->deleteNodeToolStripMenuItem->Name = L"deleteNodeToolStripMenuItem";
			this->deleteNodeToolStripMenuItem->Size = System::Drawing::Size(217, 22);
			this->deleteNodeToolStripMenuItem->Text = L"DeleteNode";
			this->deleteNodeToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::deleteNodeToolStripMenuItem_Click);
			// 
			// StageData_groupBox
			// 
			this->StageData_groupBox->Controls->Add(this->StageName_textBox);
			this->StageData_groupBox->Controls->Add(this->ReRditStagebutton);
			this->StageData_groupBox->Controls->Add(this->DelStage_button);
			this->StageData_groupBox->Controls->Add(this->AddStage_button);
			this->StageData_groupBox->Controls->Add(this->AllStage_listBox);
			this->StageData_groupBox->Location = System::Drawing::Point(3, 346);
			this->StageData_groupBox->Name = L"StageData_groupBox";
			this->StageData_groupBox->Size = System::Drawing::Size(278, 213);
			this->StageData_groupBox->TabIndex = 40;
			this->StageData_groupBox->TabStop = false;
			this->StageData_groupBox->Text = L"StageData";
			// 
			// StageName_textBox
			// 
			this->StageName_textBox->Location = System::Drawing::Point(6, 15);
			this->StageName_textBox->Name = L"StageName_textBox";
			this->StageName_textBox->Size = System::Drawing::Size(260, 25);
			this->StageName_textBox->TabIndex = 36;
			// 
			// ReRditStagebutton
			// 
			this->ReRditStagebutton->Location = System::Drawing::Point(174, 46);
			this->ReRditStagebutton->Name = L"ReRditStagebutton";
			this->ReRditStagebutton->Size = System::Drawing::Size(91, 26);
			this->ReRditStagebutton->TabIndex = 39;
			this->ReRditStagebutton->Text = L"ReRditStage";
			this->ReRditStagebutton->UseVisualStyleBackColor = true;
			this->ReRditStagebutton->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddStage_button_Click);
			// 
			// DelStage_button
			// 
			this->DelStage_button->Location = System::Drawing::Point(89, 46);
			this->DelStage_button->Name = L"DelStage_button";
			this->DelStage_button->Size = System::Drawing::Size(79, 26);
			this->DelStage_button->TabIndex = 37;
			this->DelStage_button->Text = L"Del";
			this->DelStage_button->UseVisualStyleBackColor = true;
			this->DelStage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddStage_button_Click);
			// 
			// AddStage_button
			// 
			this->AddStage_button->Location = System::Drawing::Point(6, 46);
			this->AddStage_button->Name = L"AddStage_button";
			this->AddStage_button->Size = System::Drawing::Size(79, 26);
			this->AddStage_button->TabIndex = 38;
			this->AddStage_button->Text = L"Add";
			this->AddStage_button->UseVisualStyleBackColor = true;
			this->AddStage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddStage_button_Click);
			// 
			// AllStage_listBox
			// 
			this->AllStage_listBox->FormattingEnabled = true;
			this->AllStage_listBox->ItemHeight = 15;
			this->AllStage_listBox->Location = System::Drawing::Point(6, 78);
			this->AllStage_listBox->Name = L"AllStage_listBox";
			this->AllStage_listBox->Size = System::Drawing::Size(260, 124);
			this->AllStage_listBox->TabIndex = 35;
			// 
			// xmlNodeEditComponent1
			// 
			this->xmlNodeEditComponent1->Location = System::Drawing::Point(0, 562);
			this->xmlNodeEditComponent1->Margin = System::Windows::Forms::Padding(0);
			this->xmlNodeEditComponent1->Name = L"xmlNodeEditComponent1";
			this->xmlNodeEditComponent1->Size = System::Drawing::Size(281, 223);
			this->xmlNodeEditComponent1->TabIndex = 24;
			// 
			// splitContainer2
			// 
			this->splitContainer2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->Erase_checkBox);
			this->splitContainer2->Panel1->Controls->Add(this->ReEditTemplate_button);
			this->splitContainer2->Panel1->Controls->Add(this->Clear_button);
			this->splitContainer2->Panel1->Controls->Add(this->NumObjectInCurrentStagelabel_label);
			this->splitContainer2->Panel1->Controls->Add(this->AllowSamepositionObject_checkBox);
			this->splitContainer2->Panel1->Controls->Add(this->label9);
			this->splitContainer2->Panel1->Controls->Add(this->OutputData_textBox);
			this->splitContainer2->Panel1->Controls->Add(this->label8);
			this->splitContainer2->Panel1->Controls->Add(this->label6);
			this->splitContainer2->Panel1->Controls->Add(this->label7);
			this->splitContainer2->Panel1->Controls->Add(this->Column_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->Row_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->label3);
			this->splitContainer2->Panel1->Controls->Add(this->label2);
			this->splitContainer2->Panel1->Controls->Add(this->GridWidth_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->GridHeight_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->label1);
			this->splitContainer2->Panel1->Controls->Add(this->Image_textBox);
			this->splitContainer2->Panel1->Controls->Add(this->ImageList_listBox);
			this->splitContainer2->Panel1->Controls->Add(this->AddImage_button);
			this->splitContainer2->Panel1->Controls->Add(this->TemplateName_textBox);
			this->splitContainer2->Panel1->Controls->Add(this->DelImage_button);
			this->splitContainer2->Panel1->Controls->Add(this->AddTemplate_button);
			this->splitContainer2->Panel1->Controls->Add(this->DelTemplate_button);
			this->splitContainer2->Panel1->Controls->Add(this->Template_listBox);
			this->splitContainer2->Panel1->Controls->Add(this->label5);
			this->splitContainer2->Size = System::Drawing::Size(983, 998);
			this->splitContainer2->SplitterDistance = 293;
			this->splitContainer2->TabIndex = 0;
			// 
			// Erase_checkBox
			// 
			this->Erase_checkBox->AutoSize = true;
			this->Erase_checkBox->Location = System::Drawing::Point(455, 1);
			this->Erase_checkBox->Name = L"Erase_checkBox";
			this->Erase_checkBox->Size = System::Drawing::Size(94, 19);
			this->Erase_checkBox->TabIndex = 40;
			this->Erase_checkBox->Text = L"EraseObject";
			this->Erase_checkBox->UseVisualStyleBackColor = true;
			// 
			// ReEditTemplate_button
			// 
			this->ReEditTemplate_button->Location = System::Drawing::Point(384, 260);
			this->ReEditTemplate_button->Name = L"ReEditTemplate_button";
			this->ReEditTemplate_button->Size = System::Drawing::Size(117, 26);
			this->ReEditTemplate_button->TabIndex = 39;
			this->ReEditTemplate_button->Text = L"ReEditTemplate";
			this->ReEditTemplate_button->UseVisualStyleBackColor = true;
			this->ReEditTemplate_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::ReEditTemplate_button_Click);
			// 
			// Clear_button
			// 
			this->Clear_button->Location = System::Drawing::Point(262, 255);
			this->Clear_button->Name = L"Clear_button";
			this->Clear_button->Size = System::Drawing::Size(94, 27);
			this->Clear_button->TabIndex = 38;
			this->Clear_button->Text = L"Clear";
			this->Clear_button->UseVisualStyleBackColor = true;
			this->Clear_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::Clear_button_Click);
			// 
			// NumObjectInCurrentStagelabel_label
			// 
			this->NumObjectInCurrentStagelabel_label->AutoSize = true;
			this->NumObjectInCurrentStagelabel_label->Location = System::Drawing::Point(566, 267);
			this->NumObjectInCurrentStagelabel_label->Name = L"NumObjectInCurrentStagelabel_label";
			this->NumObjectInCurrentStagelabel_label->Size = System::Drawing::Size(0, 15);
			this->NumObjectInCurrentStagelabel_label->TabIndex = 37;
			// 
			// AllowSamepositionObject_checkBox
			// 
			this->AllowSamepositionObject_checkBox->AutoSize = true;
			this->AllowSamepositionObject_checkBox->Location = System::Drawing::Point(10, 266);
			this->AllowSamepositionObject_checkBox->Name = L"AllowSamepositionObject_checkBox";
			this->AllowSamepositionObject_checkBox->Size = System::Drawing::Size(174, 19);
			this->AllowSamepositionObject_checkBox->TabIndex = 36;
			this->AllowSamepositionObject_checkBox->Text = L"AllowSamepositionObject";
			this->AllowSamepositionObject_checkBox->UseVisualStyleBackColor = true;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(196, 6);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(93, 15);
			this->label9->TabIndex = 35;
			this->label9->Text = L"TemplateName";
			// 
			// OutputData_textBox
			// 
			this->OutputData_textBox->Enabled = false;
			this->OutputData_textBox->Location = System::Drawing::Point(555, 6);
			this->OutputData_textBox->Multiline = true;
			this->OutputData_textBox->Name = L"OutputData_textBox";
			this->OutputData_textBox->Size = System::Drawing::Size(421, 245);
			this->OutputData_textBox->TabIndex = 34;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(367, 3);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(82, 15);
			this->label8->TabIndex = 33;
			this->label8->Text = L"TemplateList";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Enabled = false;
			this->label6->Location = System::Drawing::Point(194, 186);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(33, 15);
			this->label6->TabIndex = 32;
			this->label6->Text = L"Row";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Enabled = false;
			this->label7->Location = System::Drawing::Point(194, 224);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(52, 15);
			this->label7->TabIndex = 31;
			this->label7->Text = L"Column";
			// 
			// Column_numericUpDown
			// 
			this->Column_numericUpDown->Location = System::Drawing::Point(271, 224);
			this->Column_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->Column_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->Column_numericUpDown->Name = L"Column_numericUpDown";
			this->Column_numericUpDown->Size = System::Drawing::Size(85, 25);
			this->Column_numericUpDown->TabIndex = 30;
			this->Column_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->Column_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::Row_numericUpDown_ValueChanged);
			// 
			// Row_numericUpDown
			// 
			this->Row_numericUpDown->Location = System::Drawing::Point(271, 184);
			this->Row_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->Row_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->Row_numericUpDown->Name = L"Row_numericUpDown";
			this->Row_numericUpDown->Size = System::Drawing::Size(85, 25);
			this->Row_numericUpDown->TabIndex = 29;
			this->Row_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->Row_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::Row_numericUpDown_ValueChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Enabled = false;
			this->label3->Location = System::Drawing::Point(194, 113);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(68, 15);
			this->label3->TabIndex = 28;
			this->label3->Text = L"GridWidth";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Enabled = false;
			this->label2->Location = System::Drawing::Point(194, 154);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(71, 15);
			this->label2->TabIndex = 27;
			this->label2->Text = L"GridHeight";
			// 
			// GridWidth_numericUpDown
			// 
			this->GridWidth_numericUpDown->Location = System::Drawing::Point(271, 110);
			this->GridWidth_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->GridWidth_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->GridWidth_numericUpDown->Name = L"GridWidth_numericUpDown";
			this->GridWidth_numericUpDown->Size = System::Drawing::Size(85, 25);
			this->GridWidth_numericUpDown->TabIndex = 26;
			this->GridWidth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->GridWidth_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::GridHeight_numericUpDown_ValueChanged);
			// 
			// GridHeight_numericUpDown
			// 
			this->GridHeight_numericUpDown->Location = System::Drawing::Point(271, 148);
			this->GridHeight_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->GridHeight_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->GridHeight_numericUpDown->Name = L"GridHeight_numericUpDown";
			this->GridHeight_numericUpDown->Size = System::Drawing::Size(85, 25);
			this->GridHeight_numericUpDown->TabIndex = 25;
			this->GridHeight_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->GridHeight_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::GridHeight_numericUpDown_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(64, 15);
			this->label1->TabIndex = 24;
			this->label1->Text = L"ImageList";
			// 
			// Image_textBox
			// 
			this->Image_textBox->Enabled = false;
			this->Image_textBox->Location = System::Drawing::Point(197, 70);
			this->Image_textBox->Name = L"Image_textBox";
			this->Image_textBox->Size = System::Drawing::Size(159, 25);
			this->Image_textBox->TabIndex = 12;
			// 
			// ImageList_listBox
			// 
			this->ImageList_listBox->FormattingEnabled = true;
			this->ImageList_listBox->ItemHeight = 15;
			this->ImageList_listBox->Location = System::Drawing::Point(9, 24);
			this->ImageList_listBox->Name = L"ImageList_listBox";
			this->ImageList_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->ImageList_listBox->Size = System::Drawing::Size(165, 184);
			this->ImageList_listBox->TabIndex = 0;
			this->ImageList_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ObjectDataForm::ImageList_listBox_SelectedIndexChanged);
			// 
			// AddImage_button
			// 
			this->AddImage_button->Location = System::Drawing::Point(10, 223);
			this->AddImage_button->Name = L"AddImage_button";
			this->AddImage_button->Size = System::Drawing::Size(79, 26);
			this->AddImage_button->TabIndex = 23;
			this->AddImage_button->Text = L"Add/增加";
			this->AddImage_button->UseVisualStyleBackColor = true;
			this->AddImage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddImage_button_Click);
			// 
			// TemplateName_textBox
			// 
			this->TemplateName_textBox->Location = System::Drawing::Point(195, 24);
			this->TemplateName_textBox->Name = L"TemplateName_textBox";
			this->TemplateName_textBox->Size = System::Drawing::Size(161, 25);
			this->TemplateName_textBox->TabIndex = 10;
			// 
			// DelImage_button
			// 
			this->DelImage_button->Location = System::Drawing::Point(95, 223);
			this->DelImage_button->Name = L"DelImage_button";
			this->DelImage_button->Size = System::Drawing::Size(79, 26);
			this->DelImage_button->TabIndex = 22;
			this->DelImage_button->Text = L"Del/移除";
			this->DelImage_button->UseVisualStyleBackColor = true;
			this->DelImage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddImage_button_Click);
			// 
			// AddTemplate_button
			// 
			this->AddTemplate_button->Location = System::Drawing::Point(384, 225);
			this->AddTemplate_button->Name = L"AddTemplate_button";
			this->AddTemplate_button->Size = System::Drawing::Size(79, 26);
			this->AddTemplate_button->TabIndex = 17;
			this->AddTemplate_button->Text = L"Add";
			this->AddTemplate_button->UseVisualStyleBackColor = true;
			this->AddTemplate_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddTemplate_button_Click);
			// 
			// DelTemplate_button
			// 
			this->DelTemplate_button->Location = System::Drawing::Point(470, 225);
			this->DelTemplate_button->Name = L"DelTemplate_button";
			this->DelTemplate_button->Size = System::Drawing::Size(79, 26);
			this->DelTemplate_button->TabIndex = 16;
			this->DelTemplate_button->Text = L"Del";
			this->DelTemplate_button->UseVisualStyleBackColor = true;
			this->DelTemplate_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddTemplate_button_Click);
			// 
			// Template_listBox
			// 
			this->Template_listBox->FormattingEnabled = true;
			this->Template_listBox->ItemHeight = 15;
			this->Template_listBox->Location = System::Drawing::Point(384, 24);
			this->Template_listBox->Name = L"Template_listBox";
			this->Template_listBox->Size = System::Drawing::Size(165, 184);
			this->Template_listBox->TabIndex = 14;
			this->Template_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ObjectDataForm::Template_listBox_SelectedIndexChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Enabled = false;
			this->label5->Location = System::Drawing::Point(194, 52);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(42, 15);
			this->label5->TabIndex = 13;
			this->label5->Text = L"Image";
			// 
			// assistanceToolStripMenuItem
			// 
			this->assistanceToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->showGridToolStripMenuItem, 
				this->autoAllignGridToolStripMenuItem, this->camerResetToolStripMenuItem, this->showResolutionBorderToolStripMenuItem});
			this->assistanceToolStripMenuItem->Name = L"assistanceToolStripMenuItem";
			this->assistanceToolStripMenuItem->Size = System::Drawing::Size(88, 22);
			this->assistanceToolStripMenuItem->Text = L"Assistance";
			this->assistanceToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::assistanceToolStripMenuItem_Click);
			// 
			// showGridToolStripMenuItem
			// 
			this->showGridToolStripMenuItem->Checked = true;
			this->showGridToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showGridToolStripMenuItem->Name = L"showGridToolStripMenuItem";
			this->showGridToolStripMenuItem->Size = System::Drawing::Size(218, 22);
			this->showGridToolStripMenuItem->Text = L"ShowGrid";
			this->showGridToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// autoAllignGridToolStripMenuItem
			// 
			this->autoAllignGridToolStripMenuItem->Checked = true;
			this->autoAllignGridToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->autoAllignGridToolStripMenuItem->Name = L"autoAllignGridToolStripMenuItem";
			this->autoAllignGridToolStripMenuItem->Size = System::Drawing::Size(218, 22);
			this->autoAllignGridToolStripMenuItem->Text = L"AutoAllignGrid";
			this->autoAllignGridToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// camerResetToolStripMenuItem
			// 
			this->camerResetToolStripMenuItem->Name = L"camerResetToolStripMenuItem";
			this->camerResetToolStripMenuItem->Size = System::Drawing::Size(218, 22);
			this->camerResetToolStripMenuItem->Text = L"CamerReset";
			this->camerResetToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// showResolutionBorderToolStripMenuItem
			// 
			this->showResolutionBorderToolStripMenuItem->Checked = true;
			this->showResolutionBorderToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showResolutionBorderToolStripMenuItem->Name = L"showResolutionBorderToolStripMenuItem";
			this->showResolutionBorderToolStripMenuItem->Size = System::Drawing::Size(218, 22);
			this->showResolutionBorderToolStripMenuItem->Text = L"ShowResolutionBorder";
			this->showResolutionBorderToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->fileToolStripMenuItem, 
				this->imageToolStripMenuItem, this->imageToolStripMenuItem1, this->assistanceToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1280, 26);
			this->menuStrip1->TabIndex = 5;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
				this->saveToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(40, 22);
			this->fileToolStripMenuItem->Text = L"File";
			this->fileToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::fileToolStripMenuItem_Click);
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(111, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(111, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::saveToolStripMenuItem_Click);
			// 
			// imageToolStripMenuItem
			// 
			this->imageToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->openTemplateToolStripMenuItem});
			this->imageToolStripMenuItem->Name = L"imageToolStripMenuItem";
			this->imageToolStripMenuItem->Size = System::Drawing::Size(44, 22);
			this->imageToolStripMenuItem->Text = L"Xml";
			this->imageToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::imageToolStripMenuItem_Click);
			// 
			// openTemplateToolStripMenuItem
			// 
			this->openTemplateToolStripMenuItem->Name = L"openTemplateToolStripMenuItem";
			this->openTemplateToolStripMenuItem->Size = System::Drawing::Size(217, 22);
			this->openTemplateToolStripMenuItem->Text = L"AddNewNodeFromFile";
			this->openTemplateToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::addNewNodeFromFileToolStripMenuItem_Click);
			// 
			// imageToolStripMenuItem1
			// 
			this->imageToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->backgroundImageToolStripMenuItem});
			this->imageToolStripMenuItem1->Name = L"imageToolStripMenuItem1";
			this->imageToolStripMenuItem1->Size = System::Drawing::Size(63, 22);
			this->imageToolStripMenuItem1->Text = L"Image";
			this->imageToolStripMenuItem1->Click += gcnew System::EventHandler(this, &ObjectDataForm::imageToolStripMenuItem1_Click);
			// 
			// backgroundImageToolStripMenuItem
			// 
			this->backgroundImageToolStripMenuItem->Name = L"backgroundImageToolStripMenuItem";
			this->backgroundImageToolStripMenuItem->Size = System::Drawing::Size(195, 22);
			this->backgroundImageToolStripMenuItem->Text = L"BackgroundImage";
			this->backgroundImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::backgroundImageToolStripMenuItem_Click);
			// 
			// ObjectDataForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->Name = L"ObjectDataForm";
			this->Size = System::Drawing::Size(1280, 1024);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->flowLayoutPanel1->ResumeLayout(false);
			this->contextMenuStrip1->ResumeLayout(false);
			this->StageData_groupBox->ResumeLayout(false);
			this->StageData_groupBox->PerformLayout();
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel1->PerformLayout();
			this->splitContainer2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Column_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Row_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridWidth_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridHeight_numericUpDown))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void assistanceToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
		 }
private: System::Void imageToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
		 }
private: System::Void imageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
		 }
private: System::Void fileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
		 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			Render();
		 }
private: System::Void treeView1_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			 if ( e->Data->GetDataPresent( DataFormats::FileDrop ) )
			 {
				e->Effect = DragDropEffects::Copy;
			 }
			 else
				 e->Effect = DragDropEffects::None;
		 }
private: System::Void treeView1_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			array<String^>^l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
			for each(String^l_str in l_strFileNames)
			{
				 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_str);
				 if( l_pNode )
				 {
					 //this->Text += System::IO::Path::GetFileName(l_str);
					 treeView1->Nodes->Add(l_pNode);
				 }
			}
		 }
		 void	AssignData()
		 {
			 if( treeView1->SelectedNode )
			 {
				 if( treeView1->SelectedNode->Nodes->Count != 0 )
				 {
					 xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes->Clear();
					 xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes->Add((GCFORM::TreeNode^)treeView1->SelectedNode->Clone());
				 }
				 else
				 {
					WARNING_MSG("there is no attributes could be edit");
				 }
			 }		 
		 }
private: System::Void treeView1_DoubleClick(System::Object^  sender, System::EventArgs^  e)
		 {
			 AssignData();
		 }
private: System::Void refreshToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 AssignData();
		 }
private: System::Void addNewNodeFromFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			array<String^>^l_strFileNames = DNCT::OpenFileAndGetNames();
			if(!l_strFileNames)
				return;
			for each(String^l_str in l_strFileNames)
			{
				 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_str);
				 if( l_pNode )
				 {
					 //this->Text += System::IO::Path::GetFileName(l_str);
					 treeView1->Nodes->Add(l_pNode);
				 }
			}
		 }
		 void	RemoveSelectedNode()
		 {
			 if( treeView1->SelectedNode )
			 {
				 treeView1->Nodes->Remove(treeView1->SelectedNode);
			 }			 
		 }
private: System::Void deleteNodeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			RemoveSelectedNode();
		 }
private: System::Void treeView1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( e->KeyCode == GCFORM::Keys::Delete )
			 {
				RemoveSelectedNode();
			 }
		 }

private: System::Void AddImage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( sender == AddImage_button )
			{
				array<String^>^l_strFileNames = DNCT::OpenFileAndGetNames("(*.png)|*.png");
				if(l_strFileNames)
				for each(String^l_strName in l_strFileNames)
				{
					String^l_strImageName = System::IO::Path::GetFileNameWithoutExtension(l_strName);
					String^l_strExtensName = System::IO::Path::GetExtension(l_strName);
					if(l_strExtensName->Equals(".png")||
						l_strExtensName->Equals(".PNG"))
					{
						if(!DNCT::CheckListContainStringAndAdd(ImageList_listBox,l_strImageName))
						{
							char*	l_strImageName = DNCT::GcStringToChar(l_strName);
							cBaseImage*l_pImage = new cBaseImage(l_strImageName);
							if( l_pImage->GetImageIndex() != 0 )
							{
								if( m_pMapDataManager->GetNum() == 0 )
								{
									GridWidth_numericUpDown->Value = *l_pImage->GetImageShowWidth();
									GridHeight_numericUpDown->Value = *l_pImage->GetImageShowHeight();
								}
								m_pMapDataManager->m_AllImageList.AddObject(l_pImage);

							}
							else
							{
								SAFE_DELETE(l_strImageName);
								ImageList_listBox->Items->RemoveAt(ImageList_listBox->Items->Count-1);
							}
						}
					}
				}
			}
			else
			if( sender == DelImage_button )
			{
				while(ImageList_listBox->SelectedIndices->Count>0)
				{
					int	l_iSelectedIndex = ImageList_listBox->SelectedIndices[0];
					WCHAR*	l_strImageName = DNCT::GcStringToWchar(ImageList_listBox->Items[l_iSelectedIndex]->ToString());
					m_pMapDataManager->m_AllImageList.RemoveObject(l_strImageName);
					ImageList_listBox->Items->RemoveAt(l_iSelectedIndex);
				}
			}
		 }
private: System::Void ImageList_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			if( ImageList_listBox->SelectedIndex!=-1 )
			{
				cBaseImage*l_pNewImage = m_pMapDataManager->m_AllImageList.GetObject(DNCT::GcStringToWchar(ImageList_listBox->Items[ImageList_listBox->SelectedIndex]->ToString()));
				if( Template_listBox->SelectedIndex != -1 )
				{//change old data image as new
					String^l_strName = Template_listBox->Items[Template_listBox->SelectedIndex]->ToString();
					cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = g_pForListBoxTemplateCellData->GetObject(DNCT::GcStringToWchar(l_strName));
					assert(l_pMap_Cell_ObjectData&&"call fatming!! if( sender == DelTemplate_button )");
					for(int i=0;i<m_pMapDataManager->GetNum();++i)
					{
						cMap_CellData*l_pMap_CellData = m_pMapDataManager->GetObject(i);
						for(int j=0;j<l_pMap_CellData->GetNum();++j)
						{
							if((*l_pMap_CellData)[j]->GetImage() == l_pMap_Cell_ObjectData->GetImage())
							{
								(*l_pMap_CellData)[j]->SetImage(l_pNewImage);
							}
						}
					}
				}
				Image_textBox->Text = ImageList_listBox->Items[ImageList_listBox->SelectedIndex]->ToString();
				//g_pCurrentSelectedImage = l_pNewImage;
			}
			//else
			//	g_pCurrentSelectedImage = 0;
		 }

private: System::Void AddTemplate_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == DelTemplate_button )
			 {
				 bool	l_bFirstTime = true;
				 bool	l_bEraseOldData = false;//if yes and template is conflict,erase the data
				 while(Template_listBox->SelectedIndices->Count)
				 {
					 String^l_strName = Template_listBox->Items[Template_listBox->SelectedIndices[0]]->ToString();
					 //check for old data using this image
					 cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = g_pForListBoxTemplateCellData->GetObject(DNCT::GcStringToWchar(l_strName));
					 assert(l_pMap_Cell_ObjectData&&"call fatming!! if( sender == DelTemplate_button )");
					 for(int i=0;i<m_pMapDataManager->GetNum();++i)
					 {
						 cMap_CellData*l_pMap_CellData = m_pMapDataManager->GetObject(i);
						 for(int j=0;j<l_pMap_CellData->GetNum();++j)
						 {//this template has been assigned to the orther data,check for delete or reserved
							 if((*l_pMap_CellData)[j]->GetImage() == l_pMap_Cell_ObjectData->GetImage())
							 {
								 if( !l_bFirstTime )
								 {
									l_bFirstTime = true;
									WARING_YES_NO_TO_YES("would u like to erase the data has been assigned by this data?")
									{
										l_bEraseOldData = true;
									}
									else
									{
										WARNING_MSG("the data has been referenced can't be delete");
										return;
									}
								 }
								 if( l_bEraseOldData )
								 {
									 l_pMap_CellData->RemoveObject(j);
									 --j;
								 }
							 }
						 }
					 }
					 m_TemplateNameAndAttributeNode->Remove(l_strName);
					 g_pForListBoxTemplateCellData->RemoveObject(DNCT::GcStringToWchar(l_strName));
				 }
			 }
			 else
			 if(sender == AddTemplate_button)
			 {
				 String^l_strTemplateName = TemplateName_textBox->Text->Trim();
				 if( l_strTemplateName->Length >0 )
				 {
					 if( l_strTemplateName[0]>=L'0'&&l_strTemplateName[0]<=L'9' )
					 {
						WARNING_MSG("first character can't be numerial");
						return;
					 }
					 GCFORM::TreeNode^l_pNode;
					 if(!m_TemplateNameAndAttributeNode->ContainsKey(l_strTemplateName))
					 //if(!m_pMapDataManager->GetObject(DNCT::GcStringToWchar(TemplateName_textBox->Text->Trim()))
					 {
						cBaseImage*l_pImage = m_pMapDataManager->m_AllImageList.GetObject(DNCT::GcStringToWchar(Image_textBox->Text));
						if( !l_pImage )
						{
							WARNING_MSG("image is not exists");
							return;
						}
						 if(xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes->Count!=0)
						 {
							 l_pNode = (GCFORM::TreeNode^)xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes[0]->Clone();
							 l_pNode->Text = l_strTemplateName;
						 }
						 m_TemplateNameAndAttributeNode[l_strTemplateName] = l_pNode;
						 Template_listBox->Items->Add(l_strTemplateName);
						 cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = new cMap_Cell_ObjectData();
						 l_pMap_Cell_ObjectData->SetName(DNCT::GcStringToWchar(l_strTemplateName));
						 l_pMap_Cell_ObjectData->SetImage(l_pImage);
						 g_pForListBoxTemplateCellData->AddObject(l_pMap_Cell_ObjectData);
					 }
				 }
			 }
		 }

private: System::Void AddStage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( sender == AddStage_button )
			{
				WCHAR*	l_strName = DNCT::GcStringToWchar(StageName_textBox->Text);
				if( !this->m_pMapDataManager->GetObject(l_strName) )
				{
					g_pCurrentStageEditData->SetName(l_strName);
					cMap_CellData*l_pMap_CellData = new cMap_CellData(g_pCurrentStageEditData);
					this->m_pMapDataManager->AddObject(l_pMap_CellData);
					g_pCurrentStageEditData->Clear();
					AllStage_listBox->Items->Add(StageName_textBox->Text);
				}
				else
				{
					WARNING_MSG("name exists!!");
					return;
				}
			}
			else
			{
				if( AllStage_listBox->SelectedIndex == -1 )
					return;
				if( sender == ReRditStagebutton )
				{
					SAFE_DELETE(g_pCurrentStageEditData);
					g_pCurrentStageEditData = new cMap_CellData(m_pMapDataManager->GetObject(AllStage_listBox->SelectedIndex));
					Vector2	l_vSize = g_pCurrentStageEditData->GetSize();
					POINT	l_GridSize = g_pCurrentStageEditData->GetGridSize();
					GridWidth_numericUpDown->Value = (int)l_vSize.x;
					GridHeight_numericUpDown->Value = (int)l_vSize.y;
					Row_numericUpDown->Value = (int)l_GridSize.x;
					Column_numericUpDown->Value = (int)l_GridSize.y;

				}
				else
				if( sender == DelStage_button )
				{
					this->m_pMapDataManager->RemoveObject(AllStage_listBox->SelectedIndex);
					AllStage_listBox->Items->RemoveAt(AllStage_listBox->SelectedIndex);
				}
			}
		 }
private: System::Void GridHeight_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 Vector2	l_vSize = g_pCurrentStageEditData->GetSize();
			 l_vSize.x = (float)GridWidth_numericUpDown->Value;
			 l_vSize.y = (float)GridHeight_numericUpDown->Value;
			 float	l_fWidth = m_pOrthogonalCamera->GetGridWidth()*(int)Row_numericUpDown->Value;
			 float	l_fHeight = m_pOrthogonalCamera->GetGridHeight()*(int)Column_numericUpDown->Value;
			 for( int i=0;i<g_pCurrentStageEditData->GetNum();++i )
			 {
				cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = (*g_pCurrentStageEditData)[i];
				Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
				if( autoAllignGridToolStripMenuItem->Checked )//by grid position
				{
					//if( l_vPos.x >= 0.f&&l_fWidth >= l_vPos.x&&l_vPos.y >= 0.f&&l_fHeight >= l_vPos.y )
					{
						int	l_iRow = (int)(l_vPos.x/m_pOrthogonalCamera->GetGridWidth());
						int	l_iColumn = (int)(l_vPos.y/m_pOrthogonalCamera->GetGridHeight());
						l_vPos.x = l_iRow*l_vSize.x;
						l_vPos.y = l_iColumn*l_vSize.y;
					}
				}
				else//by scale rate
				{
					float	l_fScaleRateX = l_vSize.x/m_pOrthogonalCamera->GetGridWidth();
					float	l_fScaleRateY = l_vSize.y/m_pOrthogonalCamera->GetGridHeight();
					l_vPos.x *= l_fScaleRateX;
					l_vPos.y *= l_fScaleRateY;
				}
				l_pMap_Cell_ObjectData->SetPos(l_vPos);
			 }
			 m_pOrthogonalCamera->SetGridHeight((float)GridHeight_numericUpDown->Value);
			 m_pOrthogonalCamera->SetGridWidth((float)GridWidth_numericUpDown->Value);
			 g_pCurrentStageEditData->SetSize(l_vSize);
		 }
private: System::Void showGridToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( camerResetToolStripMenuItem == sender )
			 {
				 m_pOrthogonalCamera->SetResolution(Vector2(DRAW_PANEL_RESOLUTION_WIDTH,DRAW_PANEL_RESOLUTION_HEIGHT));
				 m_pOrthogonalCamera->Reset();
			 }
			 else
			 //if( sender == showGridToolStripMenuItem )
			 {
				 System::Windows::Forms::ToolStripMenuItem^l_pSender = (System::Windows::Forms::ToolStripMenuItem^)sender;
				 l_pSender->Checked = !l_pSender->Checked;
			 }
		 }
private: System::Void Row_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 POINT	l_GriSize = g_pCurrentStageEditData->GetGridSize();
			 l_GriSize.x = (int)Row_numericUpDown->Value;
			 l_GriSize.y = (int)Column_numericUpDown->Value;
			 g_pCurrentStageEditData->SetGridSize(l_GriSize);
		 }
private: System::Void Template_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( Template_listBox->SelectedIndex != -1 )
			 {
				 g_pCurrentSelectedImage = g_pForListBoxTemplateCellData->GetObject(DNCT::GcStringToWchar(Template_listBox->SelectedItem->ToString()))->GetImage();
				 TemplateDataInvertorReEdit();
			 }
		 }
private: System::Void Clear_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			g_pCurrentStageEditData->Clear();
		 }
private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strFileName = DNCT::SaveFileAndGetName();
			 if( l_strFileName )
			 {
				 m_pMapDataManager->Export(DNCT::GcStringToChar(l_strFileName));
				 l_strFileName = System::IO::Path::ChangeExtension(l_strFileName,"tmp");
				 GCFORM::TreeNode^ l_pTemplateNode = gcnew GCFORM::TreeNode();
				 l_pTemplateNode->Text = "Root";
				 for each (TreeNode^ t in m_TemplateNameAndAttributeNode->Values)
				 {
					 if( t )
						l_pTemplateNode->Nodes->Add((TreeNode^)t->Clone());
				 }
				//SaveTreeNodeToFile(l_pTemplateNode,l_strFileName);
				 XmlDocument^ d = gcnew XmlDocument();
				 XmlNode^ n = d->CreateNode(XmlNodeType::Element,"root1","");
				 //for each (TreeNode^ t in m_TemplateNameAndAttributeNode->Values)
				 {
					 n->AppendChild(DNCT::getXmlNode(l_pTemplateNode, d));
				 }
				 for each(XmlNode^ e_ChildNode in n->ChildNodes)
					d->AppendChild(e_ChildNode);
				  d->Save(l_strFileName);
			 }
		 }
private: System::Void backgroundImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			String^l_strFileName = DNCT::OpenFileAndGetName();
			if( l_strFileName )
			{
				SAFE_DELETE(g_pBackgroundImage);
				g_pBackgroundImage = new cBaseImage(DNCT::GcStringToChar(l_strFileName));
			}
		 }

private: System::Void ReEditTemplate_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 TemplateDataInvertorReEdit();
		 }
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			String^l_strFileName = DNCT::OpenFileAndGetName();
			if( l_strFileName )
			{
				AllStage_listBox->Items->Clear();
				g_pCurrentSelectedImage = 0;
				g_pCurrentStageEditData->Clear();
				g_pForListBoxTemplateCellData->Clear();
				m_pMapDataManager->m_AllImageList.Clear();
				m_pMapDataManager->Clear();
				ImageList_listBox->Items->Clear();
				Image_textBox->Text = "";
				this->m_TemplateNameAndAttributeNode->Clear();
				this->Template_listBox->Items->Clear();
				bool	l_b = m_pMapDataManager->Parser(DNCT::GcStringToChar(l_strFileName));
				if( l_b )
				{
					for(int i=0;i<m_pMapDataManager->m_AllImageList.GetNum();++i)
						ImageList_listBox->Items->Add(DNCT::WcharToGcstring(m_pMapDataManager->m_AllImageList[i]->GetName()));
					l_strFileName = System::IO::Path::ChangeExtension(l_strFileName,"tmp");
					treeView1->Nodes->Clear();
					GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_strFileName);
					if(l_pNode)
					{
						for each(GCFORM::TreeNode^l_pNode in l_pNode->Nodes)
						{
							treeView1->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
							m_TemplateNameAndAttributeNode->Add(l_pNode->Text,l_pNode->Clone());
						}
					}
					for(int i=0;i<m_pMapDataManager->m_ForListBoxTemplateCellData.GetNum();++i)
					{
						this->Template_listBox->Items->Add(DNCT::WcharToGcstring(m_pMapDataManager->m_ForListBoxTemplateCellData[i]->GetName()));
					}
					for(int i=0;i<m_pMapDataManager->GetNum();++i)
					{
						AllStage_listBox->Items->Add(DNCT::WcharToGcstring(m_pMapDataManager->GetObject(i)->GetName()));
					}
				}
			}
		 }
};
}