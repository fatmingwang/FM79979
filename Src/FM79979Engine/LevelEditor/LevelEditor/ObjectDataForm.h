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
			MouseMode_comboBox->SelectedIndex = 0;
			GCFORM::TabPage^l_pPage = gcnew GCFORM::TabPage;
			l_pPage->Text = "ObjectDataForm";
			l_pPage->Controls->Add(this);
			e_ptabControl->TabPages->Add(l_pPage);
			m_pForm = e_pForm;
			m_pLevelEditorApp = new cLevelEditorApp((HWND)Main_splitContainer->Panel1->Handle.ToPointer());
			m_pLevelEditorApp->Init();
			

			LevelLayerGridEventDataNode_xmlNodeEditComponent->m_TreeViewDeleteOrCreateFunction += gcnew System::EventHandler(this,&ObjectDataForm::LevelLayerGridEventDataNode_xmlNodeEditComponentTreeViewDeleteOrCreateFunction);
			Main_splitContainer->Panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseMove);
			Main_splitContainer->Panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseDown);
			Main_splitContainer->Panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseUp);
			Main_splitContainer->Panel1->DoubleClick += gcnew System::EventHandler(this, &ObjectDataForm::MyMouseDoubleClick);
			//make sure u hvae call focus while the panel is mouse down
			Main_splitContainer->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseUp);
			e_pForm->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ObjectDataForm::MyKeyPress);


			m_UniqueUserDefineDataForMap_Cell_ObjectData = gcnew System::Collections::Hashtable;
			m_TemplateNameAndAttributeNode = gcnew System::Collections::Hashtable;

			ToolTip^l_pToolTip = gcnew ToolTip();
			l_pToolTip->SetToolTip( TemplateList_groupBox, "select images and data template to add new data" );
			this->timer1->Enabled = true;
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
			if( m_UniqueUserDefineDataForMap_Cell_ObjectData )
				delete m_UniqueUserDefineDataForMap_Cell_ObjectData;
			if(m_TemplateNameAndAttributeNode)
				delete m_TemplateNameAndAttributeNode;
			SAFE_DELETE(m_pLevelEditorApp);
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
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
	private: System::Windows::Forms::TreeView^  XMLDataForQuickApply_treeView;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::ContextMenuStrip^  UserDefineData__contextMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  refreshToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addNewNodeFromFileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  deleteNodeToolStripMenuItem;
	private: System::Windows::Forms::Label^  label1;
	private: XMLDataEditor::XMLForm^  ObjectTemplate_xmlNodeEditComponent;
	private: System::Windows::Forms::ListBox^  AllStage_listBox;
	private: System::Windows::Forms::TextBox^  StageName_textBox;
	private: System::Windows::Forms::Button^  AddStage_button;
	private: System::Windows::Forms::Button^  DelStage_button;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::Button^  ReRditStagebutton;
	private: System::Windows::Forms::ToolStripMenuItem^  camerResetToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showResolutionBorderToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  backgroundImageToolStripMenuItem;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  StartY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  StartX_numericUpDown;
	private: System::Windows::Forms::Button^  AddToLayer_button;
	private: System::Windows::Forms::Button^  LayerDown_button;
	private: System::Windows::Forms::Button^  LayerUp_button;
	private: System::Windows::Forms::ListBox^  Layer_listBox;
	private: System::Windows::Forms::Button^  DeleteLayer_button;
	private: System::Windows::Forms::ToolStripMenuItem^  autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  cameraResetToolStripMenuItem;
	private: System::Windows::Forms::ListBox^  LayerObjectData_listBox;
	private: System::Windows::Forms::Button^  DeleteObject_button;
	private: System::Windows::Forms::ToolStripMenuItem^  showWarningMsgToolStripMenuItem;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::CheckBox^  AllowSamepositionObject_checkBox;
	private: System::Windows::Forms::NumericUpDown^  GridHeight_numericUpDown;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::NumericUpDown^  GridWidth_numericUpDown;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  MapHeight_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  MapWidth_numericUpDown;
	private: System::Windows::Forms::Button^  AddObjectToStage_button;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Button^  DelTemplate_button;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::ListBox^  Template_listBox;
	private: System::Windows::Forms::Button^  AddTemplate_button;
	private: System::Windows::Forms::NumericUpDown^  AddObjectY_numericUpDown;
	private: System::Windows::Forms::Button^  DelImage_button;
	private: System::Windows::Forms::NumericUpDown^  AddObjectX_numericUpDown;
	private: System::Windows::Forms::Button^  AddImage_button;
	private: System::Windows::Forms::TextBox^  TemplateName_textBox;
	private: System::Windows::Forms::Button^  Clear_button;
	private: System::Windows::Forms::Button^  ReEditTemplate_button;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::TextBox^  LayerName_textBox;
	private: System::Windows::Forms::TreeView^  ImageList_treeView;

	private: System::Windows::Forms::ContextMenuStrip^  UserDefineData_contextMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  addToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  deleteToolStripMenuItem;
	private: System::Windows::Forms::ContextMenuStrip^  LayerData_contextMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  selectAllThisTypeTemplateToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  refreshUserDataFromToolStripMenuItem;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::NumericUpDown^  AddObjectZ_numericUpDown;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::SplitContainer^  Main_splitContainer;
	private: System::Windows::Forms::CheckBox^  PlaceObjectByImageCenter_checkBox;
	private: System::Windows::Forms::GroupBox^  StageData_groupBox;
	private: System::Windows::Forms::GroupBox^  TemplateList_groupBox;
	private: System::Windows::Forms::GroupBox^  XMLData_groupBox;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::FlowLayoutPanel^  EventData_flowLayoutPanel;
	private: System::Windows::Forms::CheckBox^  LayerVisible_checkBox;
	private: System::Windows::Forms::ComboBox^  MouseMode_comboBox;
	private: System::Windows::Forms::Label^  MouseMode_label;
	private: System::Windows::Forms::GroupBox^  LayerList_groupBox;
	private: System::Windows::Forms::GroupBox^  Object_groupBox;
	private: System::Windows::Forms::ToolStripMenuItem^  assignToObjectEventDataToolStripMenuItem;
	private: System::Windows::Forms::Button^  NoImage_button;
	private: XMLDataEditor::XMLForm^			LevelLayerGridEventDataNode_xmlNodeEditComponent;
	protected://my
			GCFORM::Form^m_pForm;
			//String,TreeNodes
			System::Collections::Hashtable^m_TemplateNameAndAttributeNode;
			cLevelEditorApp*m_pLevelEditorApp;
			 void				AddEventDataNode(cLevelLayerGridData*e_pLevelLayerGridData,GCFORM::TreeNode^e_pNode);
			 void				RemoveEventDataNode(cLevelLayerGridData*e_pLevelLayerGridData);
			//object's unique ID with a treenode
			System::Collections::Hashtable^m_UniqueUserDefineDataForMap_Cell_ObjectData;
			void	SaveTreeNodeFile(String^e_strFileName,System::Collections::Hashtable^e_pHashTable,bool e_bUserDefineData);
			//if e_bEraseContiously is true,we won't add object if blank is empty
	private:System::Void MyMouseDoubleClick(System::Object^  sender, System::EventArgs^  e);
	private:System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private:System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private:System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
			System::Void Render();
			//template data click
			System::Void TemplateDataInvertorReEdit();
			//for user data
			//System::Void RemoveUserDefineNode(cLayer*e_pMapLayer);
			//System::Void RemoveUserDefineNode(cLevelLayerList*e_pMap_CellData);
			//System::Void RemoveUserDefineNode(cLevelLayerGridData*e_pMap_Cell_ObjectData);
			//while stage data reedited is called,to check the user data is re generate
			//System::Void CopyUserDataToNewData(cLevelLayerList*e_pSrc,cLevelLayerList*e_pDest);
			void	OpenUserDefineData(String^e_strFileName);
			void	OpenQuickApplyXmlFile(String^e_strFileName);
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
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->flowLayoutPanel1->SuspendLayout();
			this->TemplateList_groupBox->SuspendLayout();
			this->XMLData_groupBox->SuspendLayout();
			this->UserDefineData__contextMenuStrip->SuspendLayout();
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
			this->splitContainer1->Size = System::Drawing::Size(1900, 937);
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
			this->flowLayoutPanel1->Size = System::Drawing::Size(232, 935);
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
			this->TemplateList_groupBox->Size = System::Drawing::Size(226, 478);
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
			this->NoImage_button->Location = System::Drawing::Point(154, 242);
			this->NoImage_button->Name = L"NoImage_button";
			this->NoImage_button->Size = System::Drawing::Size(66, 27);
			this->NoImage_button->TabIndex = 61;
			this->NoImage_button->Text = L"NoImage";
			this->NoImage_button->UseVisualStyleBackColor = false;
			this->NoImage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::NoImage_button_Click);
			// 
			// ImageList_treeView
			// 
			this->ImageList_treeView->Location = System::Drawing::Point(13, 30);
			this->ImageList_treeView->Name = L"ImageList_treeView";
			this->ImageList_treeView->Size = System::Drawing::Size(194, 209);
			this->ImageList_treeView->TabIndex = 60;
			this->ImageList_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &ObjectDataForm::ImageList_treeView_AfterSelect);
			this->ImageList_treeView->BeforeSelect += gcnew System::Windows::Forms::TreeViewCancelEventHandler(this, &ObjectDataForm::ImageList_treeView_BeforeSelect);
			this->ImageList_treeView->MouseLeave += gcnew System::EventHandler(this, &ObjectDataForm::ImageList_treeView_MouseLeave);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Enabled = false;
			this->label5->Location = System::Drawing::Point(7, 277);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(75, 12);
			this->label5->TabIndex = 13;
			this->label5->Text = L"TemplateName";
			// 
			// TemplateName_textBox
			// 
			this->TemplateName_textBox->Location = System::Drawing::Point(139, 292);
			this->TemplateName_textBox->Margin = System::Windows::Forms::Padding(2);
			this->TemplateName_textBox->Name = L"TemplateName_textBox";
			this->TemplateName_textBox->Size = System::Drawing::Size(69, 22);
			this->TemplateName_textBox->TabIndex = 10;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 15);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(51, 12);
			this->label1->TabIndex = 24;
			this->label1->Text = L"ImageList";
			// 
			// Template_listBox
			// 
			this->Template_listBox->FormattingEnabled = true;
			this->Template_listBox->ItemHeight = 12;
			this->Template_listBox->Location = System::Drawing::Point(5, 292);
			this->Template_listBox->Margin = System::Windows::Forms::Padding(2);
			this->Template_listBox->Name = L"Template_listBox";
			this->Template_listBox->Size = System::Drawing::Size(121, 172);
			this->Template_listBox->Sorted = true;
			this->Template_listBox->TabIndex = 14;
			this->Template_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ObjectDataForm::Template_listBox_SelectedIndexChanged);
			// 
			// AddImage_button
			// 
			this->AddImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->AddImage_button->Location = System::Drawing::Point(5, 244);
			this->AddImage_button->Margin = System::Windows::Forms::Padding(2);
			this->AddImage_button->Name = L"AddImage_button";
			this->AddImage_button->Size = System::Drawing::Size(58, 22);
			this->AddImage_button->TabIndex = 23;
			this->AddImage_button->Text = L"Add/增加";
			this->AddImage_button->UseVisualStyleBackColor = false;
			this->AddImage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddImage_button_Click);
			// 
			// ReEditTemplate_button
			// 
			this->ReEditTemplate_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->ReEditTemplate_button->Location = System::Drawing::Point(139, 440);
			this->ReEditTemplate_button->Margin = System::Windows::Forms::Padding(2);
			this->ReEditTemplate_button->Name = L"ReEditTemplate_button";
			this->ReEditTemplate_button->Size = System::Drawing::Size(70, 22);
			this->ReEditTemplate_button->TabIndex = 39;
			this->ReEditTemplate_button->Text = L"ReEdit";
			this->ReEditTemplate_button->UseVisualStyleBackColor = false;
			this->ReEditTemplate_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::ReEditTemplate_button_Click);
			// 
			// AddTemplate_button
			// 
			this->AddTemplate_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->AddTemplate_button->Location = System::Drawing::Point(140, 329);
			this->AddTemplate_button->Margin = System::Windows::Forms::Padding(2);
			this->AddTemplate_button->Name = L"AddTemplate_button";
			this->AddTemplate_button->Size = System::Drawing::Size(70, 22);
			this->AddTemplate_button->TabIndex = 17;
			this->AddTemplate_button->Text = L"Add";
			this->AddTemplate_button->UseVisualStyleBackColor = false;
			this->AddTemplate_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddTemplate_button_Click);
			// 
			// DelImage_button
			// 
			this->DelImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->DelImage_button->Location = System::Drawing::Point(68, 244);
			this->DelImage_button->Margin = System::Windows::Forms::Padding(2);
			this->DelImage_button->Name = L"DelImage_button";
			this->DelImage_button->Size = System::Drawing::Size(58, 22);
			this->DelImage_button->TabIndex = 22;
			this->DelImage_button->Text = L"Del/移除";
			this->DelImage_button->UseVisualStyleBackColor = false;
			this->DelImage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddImage_button_Click);
			// 
			// DelTemplate_button
			// 
			this->DelTemplate_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->DelTemplate_button->Location = System::Drawing::Point(139, 390);
			this->DelTemplate_button->Margin = System::Windows::Forms::Padding(2);
			this->DelTemplate_button->Name = L"DelTemplate_button";
			this->DelTemplate_button->Size = System::Drawing::Size(70, 22);
			this->DelTemplate_button->TabIndex = 16;
			this->DelTemplate_button->Text = L"Del";
			this->DelTemplate_button->UseVisualStyleBackColor = false;
			this->DelTemplate_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddTemplate_button_Click);
			// 
			// XMLData_groupBox
			// 
			this->XMLData_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->XMLData_groupBox->Controls->Add(this->XMLDataForQuickApply_treeView);
			this->XMLData_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->XMLData_groupBox->Location = System::Drawing::Point(3, 487);
			this->XMLData_groupBox->Name = L"XMLData_groupBox";
			this->XMLData_groupBox->Size = System::Drawing::Size(225, 317);
			this->XMLData_groupBox->TabIndex = 45;
			this->XMLData_groupBox->TabStop = false;
			this->XMLData_groupBox->Text = L"XMLDataForQuickApply";
			// 
			// XMLDataForQuickApply_treeView
			// 
			this->XMLDataForQuickApply_treeView->AllowDrop = true;
			this->XMLDataForQuickApply_treeView->ContextMenuStrip = this->UserDefineData__contextMenuStrip;
			this->XMLDataForQuickApply_treeView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->XMLDataForQuickApply_treeView->Location = System::Drawing::Point(3, 18);
			this->XMLDataForQuickApply_treeView->Margin = System::Windows::Forms::Padding(2);
			this->XMLDataForQuickApply_treeView->Name = L"XMLDataForQuickApply_treeView";
			this->XMLDataForQuickApply_treeView->Size = System::Drawing::Size(219, 296);
			this->XMLDataForQuickApply_treeView->TabIndex = 0;
			this->XMLDataForQuickApply_treeView->DoubleClick += gcnew System::EventHandler(this, &ObjectDataForm::AssignTemplateData_Click);
			this->XMLDataForQuickApply_treeView->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &ObjectDataForm::treeView1_DragDrop);
			this->XMLDataForQuickApply_treeView->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &ObjectDataForm::treeView1_DragEnter);
			this->XMLDataForQuickApply_treeView->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &ObjectDataForm::treeView1_KeyUp);
			this->XMLDataForQuickApply_treeView->ItemDrag += gcnew System::Windows::Forms::ItemDragEventHandler(this, &ObjectDataForm::XMLDataForQuickApply_treeView_ItemDrag);
			// 
			// UserDefineData__contextMenuStrip
			// 
			this->UserDefineData__contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->refreshToolStripMenuItem, 
				this->addNewNodeFromFileToolStripMenuItem, this->deleteNodeToolStripMenuItem, this->assignToObjectEventDataToolStripMenuItem});
			this->UserDefineData__contextMenuStrip->Name = L"contextMenuStrip1";
			this->UserDefineData__contextMenuStrip->Size = System::Drawing::Size(245, 92);
			// 
			// refreshToolStripMenuItem
			// 
			this->refreshToolStripMenuItem->Name = L"refreshToolStripMenuItem";
			this->refreshToolStripMenuItem->Size = System::Drawing::Size(244, 22);
			this->refreshToolStripMenuItem->Text = L"AssignToObjectTemplateData";
			this->refreshToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::AssignTemplateData_Click);
			// 
			// addNewNodeFromFileToolStripMenuItem
			// 
			this->addNewNodeFromFileToolStripMenuItem->Name = L"addNewNodeFromFileToolStripMenuItem";
			this->addNewNodeFromFileToolStripMenuItem->Size = System::Drawing::Size(244, 22);
			this->addNewNodeFromFileToolStripMenuItem->Text = L"AddNewNodeFromFile";
			this->addNewNodeFromFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::addNewNodeFromFileToolStripMenuItem_Click);
			// 
			// deleteNodeToolStripMenuItem
			// 
			this->deleteNodeToolStripMenuItem->Name = L"deleteNodeToolStripMenuItem";
			this->deleteNodeToolStripMenuItem->Size = System::Drawing::Size(244, 22);
			this->deleteNodeToolStripMenuItem->Text = L"DeleteNode";
			this->deleteNodeToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::deleteNodeToolStripMenuItem_Click);
			// 
			// assignToObjectEventDataToolStripMenuItem
			// 
			this->assignToObjectEventDataToolStripMenuItem->Name = L"assignToObjectEventDataToolStripMenuItem";
			this->assignToObjectEventDataToolStripMenuItem->Size = System::Drawing::Size(244, 22);
			this->assignToObjectEventDataToolStripMenuItem->Text = L"AssignToObjectEventData";
			this->assignToObjectEventDataToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::assignToObjectEventDataToolStripMenuItem_Click);
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
			this->splitContainer2->Size = System::Drawing::Size(1661, 935);
			this->splitContainer2->SplitterDistance = 321;
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
			this->tabControl1->Size = System::Drawing::Size(1657, 317);
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
			this->tabPage1->Size = System::Drawing::Size(1649, 291);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"StageData";
			// 
			// LayerList_groupBox
			// 
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
			this->LayerList_groupBox->Size = System::Drawing::Size(293, 227);
			this->LayerList_groupBox->TabIndex = 39;
			this->LayerList_groupBox->TabStop = false;
			this->LayerList_groupBox->Text = L"Layer List";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(5, 18);
			this->label17->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(32, 12);
			this->label17->TabIndex = 63;
			this->label17->Text = L"Name";
			// 
			// Layer_listBox
			// 
			this->Layer_listBox->FormattingEnabled = true;
			this->Layer_listBox->ItemHeight = 12;
			this->Layer_listBox->Location = System::Drawing::Point(114, 28);
			this->Layer_listBox->Margin = System::Windows::Forms::Padding(2);
			this->Layer_listBox->Name = L"Layer_listBox";
			this->Layer_listBox->Size = System::Drawing::Size(171, 196);
			this->Layer_listBox->TabIndex = 45;
			this->Layer_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ObjectDataForm::Layer_listBox_SelectedIndexChanged);
			// 
			// LayerName_textBox
			// 
			this->LayerName_textBox->Location = System::Drawing::Point(6, 33);
			this->LayerName_textBox->Name = L"LayerName_textBox";
			this->LayerName_textBox->Size = System::Drawing::Size(100, 22);
			this->LayerName_textBox->TabIndex = 62;
			// 
			// LayerUp_button
			// 
			this->LayerUp_button->Location = System::Drawing::Point(90, 121);
			this->LayerUp_button->Margin = System::Windows::Forms::Padding(2);
			this->LayerUp_button->Name = L"LayerUp_button";
			this->LayerUp_button->Size = System::Drawing::Size(20, 39);
			this->LayerUp_button->TabIndex = 46;
			this->LayerUp_button->Text = L"^\r\n^";
			this->LayerUp_button->UseVisualStyleBackColor = true;
			this->LayerUp_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::LayerDown_button_Click);
			// 
			// DeleteLayer_button
			// 
			this->DeleteLayer_button->Location = System::Drawing::Point(7, 95);
			this->DeleteLayer_button->Margin = System::Windows::Forms::Padding(2);
			this->DeleteLayer_button->Name = L"DeleteLayer_button";
			this->DeleteLayer_button->Size = System::Drawing::Size(82, 22);
			this->DeleteLayer_button->TabIndex = 49;
			this->DeleteLayer_button->Text = L"DeleteLayer";
			this->DeleteLayer_button->UseVisualStyleBackColor = true;
			this->DeleteLayer_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddToLayer_button_Click);
			// 
			// LayerDown_button
			// 
			this->LayerDown_button->Location = System::Drawing::Point(91, 174);
			this->LayerDown_button->Margin = System::Windows::Forms::Padding(2);
			this->LayerDown_button->Name = L"LayerDown_button";
			this->LayerDown_button->Size = System::Drawing::Size(20, 34);
			this->LayerDown_button->TabIndex = 47;
			this->LayerDown_button->Text = L"v\r\nv";
			this->LayerDown_button->UseVisualStyleBackColor = true;
			this->LayerDown_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::LayerDown_button_Click);
			// 
			// AddToLayer_button
			// 
			this->AddToLayer_button->Location = System::Drawing::Point(6, 60);
			this->AddToLayer_button->Margin = System::Windows::Forms::Padding(2);
			this->AddToLayer_button->Name = L"AddToLayer_button";
			this->AddToLayer_button->Size = System::Drawing::Size(83, 22);
			this->AddToLayer_button->TabIndex = 48;
			this->AddToLayer_button->Text = L"AddToLayer";
			this->AddToLayer_button->UseVisualStyleBackColor = true;
			this->AddToLayer_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddToLayer_button_Click);
			// 
			// LayerVisible_checkBox
			// 
			this->LayerVisible_checkBox->AutoSize = true;
			this->LayerVisible_checkBox->Location = System::Drawing::Point(6, 130);
			this->LayerVisible_checkBox->Name = L"LayerVisible_checkBox";
			this->LayerVisible_checkBox->Size = System::Drawing::Size(83, 16);
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
			this->StageData_groupBox->Location = System::Drawing::Point(962, 9);
			this->StageData_groupBox->Name = L"StageData_groupBox";
			this->StageData_groupBox->Size = System::Drawing::Size(408, 223);
			this->StageData_groupBox->TabIndex = 67;
			this->StageData_groupBox->TabStop = false;
			this->StageData_groupBox->Text = L"StageData";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Enabled = false;
			this->label4->Location = System::Drawing::Point(240, 32);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(34, 12);
			this->label4->TabIndex = 43;
			this->label4->Text = L"StartX";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Enabled = false;
			this->label10->Location = System::Drawing::Point(239, 56);
			this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(34, 12);
			this->label10->TabIndex = 44;
			this->label10->Text = L"StartY";
			// 
			// StageName_textBox
			// 
			this->StageName_textBox->Location = System::Drawing::Point(5, 17);
			this->StageName_textBox->Margin = System::Windows::Forms::Padding(2);
			this->StageName_textBox->Name = L"StageName_textBox";
			this->StageName_textBox->Size = System::Drawing::Size(226, 22);
			this->StageName_textBox->TabIndex = 36;
			// 
			// StartY_numericUpDown
			// 
			this->StartY_numericUpDown->Location = System::Drawing::Point(299, 49);
			this->StartY_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->StartY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->StartY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, System::Int32::MinValue});
			this->StartY_numericUpDown->Name = L"StartY_numericUpDown";
			this->StartY_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->StartY_numericUpDown->TabIndex = 42;
			this->StartY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::StartX_numericUpDown_ValueChanged);
			// 
			// AllStage_listBox
			// 
			this->AllStage_listBox->FormattingEnabled = true;
			this->AllStage_listBox->ItemHeight = 12;
			this->AllStage_listBox->Location = System::Drawing::Point(5, 71);
			this->AllStage_listBox->Margin = System::Windows::Forms::Padding(2);
			this->AllStage_listBox->Name = L"AllStage_listBox";
			this->AllStage_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AllStage_listBox->Size = System::Drawing::Size(226, 136);
			this->AllStage_listBox->Sorted = true;
			this->AllStage_listBox->TabIndex = 35;
			this->AllStage_listBox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::AllStage_listBox_MouseDoubleClick);
			this->AllStage_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ObjectDataForm::AllStage_listBox_SelectedIndexChanged);
			this->AllStage_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &ObjectDataForm::AllStage_listBox_KeyUp);
			// 
			// MapWidth_numericUpDown
			// 
			this->MapWidth_numericUpDown->Location = System::Drawing::Point(299, 135);
			this->MapWidth_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->MapWidth_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->MapWidth_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->MapWidth_numericUpDown->Name = L"MapWidth_numericUpDown";
			this->MapWidth_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->MapWidth_numericUpDown->TabIndex = 29;
			this->MapWidth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {480, 0, 0, 0});
			this->MapWidth_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::Row_numericUpDown_ValueChanged);
			// 
			// DelStage_button
			// 
			this->DelStage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->DelStage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelStage_button->Location = System::Drawing::Point(89, 43);
			this->DelStage_button->Margin = System::Windows::Forms::Padding(2);
			this->DelStage_button->Name = L"DelStage_button";
			this->DelStage_button->Size = System::Drawing::Size(58, 22);
			this->DelStage_button->TabIndex = 37;
			this->DelStage_button->Text = L"Del";
			this->DelStage_button->UseVisualStyleBackColor = false;
			this->DelStage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddStage_button_Click);
			// 
			// StartX_numericUpDown
			// 
			this->StartX_numericUpDown->Location = System::Drawing::Point(299, 22);
			this->StartX_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->StartX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->StartX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, System::Int32::MinValue});
			this->StartX_numericUpDown->Name = L"StartX_numericUpDown";
			this->StartX_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->StartX_numericUpDown->TabIndex = 41;
			this->StartX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::StartX_numericUpDown_ValueChanged);
			// 
			// ReRditStagebutton
			// 
			this->ReRditStagebutton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->ReRditStagebutton->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ReRditStagebutton->Location = System::Drawing::Point(163, 43);
			this->ReRditStagebutton->Margin = System::Windows::Forms::Padding(2);
			this->ReRditStagebutton->Name = L"ReRditStagebutton";
			this->ReRditStagebutton->Size = System::Drawing::Size(68, 22);
			this->ReRditStagebutton->TabIndex = 39;
			this->ReRditStagebutton->Text = L"Reedit";
			this->ReRditStagebutton->UseVisualStyleBackColor = false;
			this->ReRditStagebutton->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddStage_button_Click);
			// 
			// MapHeight_numericUpDown
			// 
			this->MapHeight_numericUpDown->Location = System::Drawing::Point(300, 167);
			this->MapHeight_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->MapHeight_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->MapHeight_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->MapHeight_numericUpDown->Name = L"MapHeight_numericUpDown";
			this->MapHeight_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->MapHeight_numericUpDown->TabIndex = 30;
			this->MapHeight_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {320, 0, 0, 0});
			this->MapHeight_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::Row_numericUpDown_ValueChanged);
			// 
			// AddStage_button
			// 
			this->AddStage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AddStage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddStage_button->Location = System::Drawing::Point(16, 43);
			this->AddStage_button->Margin = System::Windows::Forms::Padding(2);
			this->AddStage_button->Name = L"AddStage_button";
			this->AddStage_button->Size = System::Drawing::Size(58, 22);
			this->AddStage_button->TabIndex = 38;
			this->AddStage_button->Text = L"Add";
			this->AddStage_button->UseVisualStyleBackColor = false;
			this->AddStage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddStage_button_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Enabled = false;
			this->label3->Location = System::Drawing::Point(239, 79);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(55, 12);
			this->label3->TabIndex = 28;
			this->label3->Text = L"GridWidth";
			// 
			// GridWidth_numericUpDown
			// 
			this->GridWidth_numericUpDown->Location = System::Drawing::Point(299, 77);
			this->GridWidth_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->GridWidth_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->GridWidth_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->GridWidth_numericUpDown->Name = L"GridWidth_numericUpDown";
			this->GridWidth_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->GridWidth_numericUpDown->TabIndex = 26;
			this->GridWidth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->GridWidth_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::GridHeight_numericUpDown_ValueChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Enabled = false;
			this->label2->Location = System::Drawing::Point(239, 111);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(57, 12);
			this->label2->TabIndex = 27;
			this->label2->Text = L"GridHeight";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Enabled = false;
			this->label6->Location = System::Drawing::Point(239, 139);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(55, 12);
			this->label6->TabIndex = 32;
			this->label6->Text = L"MapWidth";
			// 
			// GridHeight_numericUpDown
			// 
			this->GridHeight_numericUpDown->Location = System::Drawing::Point(299, 107);
			this->GridHeight_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->GridHeight_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->GridHeight_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->GridHeight_numericUpDown->Name = L"GridHeight_numericUpDown";
			this->GridHeight_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->GridHeight_numericUpDown->TabIndex = 25;
			this->GridHeight_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->GridHeight_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::GridHeight_numericUpDown_ValueChanged);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Enabled = false;
			this->label7->Location = System::Drawing::Point(239, 167);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(57, 12);
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
			this->groupBox1->Size = System::Drawing::Size(657, 285);
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
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->Size = System::Drawing::Size(405, 285);
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->TabIndex = 26;
			this->LevelLayerGridEventDataNode_xmlNodeEditComponent->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &ObjectDataForm::ObjectData_treeView_DragEnter);
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
			this->Object_groupBox->Location = System::Drawing::Point(6, 16);
			this->Object_groupBox->Name = L"Object_groupBox";
			this->Object_groupBox->Size = System::Drawing::Size(80, 201);
			this->Object_groupBox->TabIndex = 0;
			this->Object_groupBox->TabStop = false;
			// 
			// AddObjectX_numericUpDown
			// 
			this->AddObjectX_numericUpDown->Location = System::Drawing::Point(24, 20);
			this->AddObjectX_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->AddObjectX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->AddObjectX_numericUpDown->Name = L"AddObjectX_numericUpDown";
			this->AddObjectX_numericUpDown->Size = System::Drawing::Size(47, 22);
			this->AddObjectX_numericUpDown->TabIndex = 53;
			// 
			// AddObjectToStage_button
			// 
			this->AddObjectToStage_button->Location = System::Drawing::Point(7, 104);
			this->AddObjectToStage_button->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectToStage_button->Name = L"AddObjectToStage_button";
			this->AddObjectToStage_button->Size = System::Drawing::Size(64, 22);
			this->AddObjectToStage_button->TabIndex = 60;
			this->AddObjectToStage_button->Text = L"AddObject";
			this->AddObjectToStage_button->UseVisualStyleBackColor = true;
			this->AddObjectToStage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddObjectToStage_button_Click);
			// 
			// Clear_button
			// 
			this->Clear_button->Location = System::Drawing::Point(5, 156);
			this->Clear_button->Margin = System::Windows::Forms::Padding(2);
			this->Clear_button->Name = L"Clear_button";
			this->Clear_button->Size = System::Drawing::Size(70, 22);
			this->Clear_button->TabIndex = 38;
			this->Clear_button->Text = L"Clear";
			this->Clear_button->UseVisualStyleBackColor = true;
			this->Clear_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::Clear_button_Click);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(8, 72);
			this->label14->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(10, 12);
			this->label14->TabIndex = 65;
			this->label14->Text = L"z";
			// 
			// AddObjectZ_numericUpDown
			// 
			this->AddObjectZ_numericUpDown->Location = System::Drawing::Point(23, 70);
			this->AddObjectZ_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->AddObjectZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->AddObjectZ_numericUpDown->Name = L"AddObjectZ_numericUpDown";
			this->AddObjectZ_numericUpDown->Size = System::Drawing::Size(47, 22);
			this->AddObjectZ_numericUpDown->TabIndex = 64;
			// 
			// DeleteObject_button
			// 
			this->DeleteObject_button->Location = System::Drawing::Point(6, 131);
			this->DeleteObject_button->Margin = System::Windows::Forms::Padding(2);
			this->DeleteObject_button->Name = L"DeleteObject_button";
			this->DeleteObject_button->Size = System::Drawing::Size(70, 21);
			this->DeleteObject_button->TabIndex = 1;
			this->DeleteObject_button->Text = L"Delete";
			this->DeleteObject_button->UseVisualStyleBackColor = true;
			this->DeleteObject_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::DeleteObject_button_Click);
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(9, 20);
			this->label13->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(11, 12);
			this->label13->TabIndex = 56;
			this->label13->Text = L"x";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(9, 47);
			this->label15->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(11, 12);
			this->label15->TabIndex = 58;
			this->label15->Text = L"y";
			// 
			// AddObjectY_numericUpDown
			// 
			this->AddObjectY_numericUpDown->Location = System::Drawing::Point(24, 45);
			this->AddObjectY_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->AddObjectY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->AddObjectY_numericUpDown->Name = L"AddObjectY_numericUpDown";
			this->AddObjectY_numericUpDown->Size = System::Drawing::Size(47, 22);
			this->AddObjectY_numericUpDown->TabIndex = 55;
			// 
			// MouseMode_label
			// 
			this->MouseMode_label->AutoSize = true;
			this->MouseMode_label->Location = System::Drawing::Point(93, 182);
			this->MouseMode_label->Name = L"MouseMode_label";
			this->MouseMode_label->Size = System::Drawing::Size(63, 12);
			this->MouseMode_label->TabIndex = 70;
			this->MouseMode_label->Text = L"MouseMode";
			// 
			// PlaceObjectByImageCenter_checkBox
			// 
			this->PlaceObjectByImageCenter_checkBox->AutoSize = true;
			this->PlaceObjectByImageCenter_checkBox->Location = System::Drawing::Point(17, 226);
			this->PlaceObjectByImageCenter_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->PlaceObjectByImageCenter_checkBox->Name = L"PlaceObjectByImageCenter_checkBox";
			this->PlaceObjectByImageCenter_checkBox->Size = System::Drawing::Size(152, 16);
			this->PlaceObjectByImageCenter_checkBox->TabIndex = 66;
			this->PlaceObjectByImageCenter_checkBox->Text = L"PlaceObjectByImageCenter";
			this->PlaceObjectByImageCenter_checkBox->UseVisualStyleBackColor = true;
			// 
			// MouseMode_comboBox
			// 
			this->MouseMode_comboBox->FormattingEnabled = true;
			this->MouseMode_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Add", L"Move", L"Erase"});
			this->MouseMode_comboBox->Location = System::Drawing::Point(95, 197);
			this->MouseMode_comboBox->Name = L"MouseMode_comboBox";
			this->MouseMode_comboBox->Size = System::Drawing::Size(89, 20);
			this->MouseMode_comboBox->TabIndex = 69;
			// 
			// LayerObjectData_listBox
			// 
			this->LayerObjectData_listBox->FormattingEnabled = true;
			this->LayerObjectData_listBox->HorizontalScrollbar = true;
			this->LayerObjectData_listBox->ItemHeight = 12;
			this->LayerObjectData_listBox->Location = System::Drawing::Point(92, 13);
			this->LayerObjectData_listBox->Margin = System::Windows::Forms::Padding(2);
			this->LayerObjectData_listBox->Name = L"LayerObjectData_listBox";
			this->LayerObjectData_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->LayerObjectData_listBox->Size = System::Drawing::Size(155, 160);
			this->LayerObjectData_listBox->TabIndex = 0;
			this->LayerObjectData_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ObjectDataForm::LayerObjectData_listBox_SelectedIndexChanged);
			// 
			// AllowSamepositionObject_checkBox
			// 
			this->AllowSamepositionObject_checkBox->AutoSize = true;
			this->AllowSamepositionObject_checkBox->Location = System::Drawing::Point(17, 243);
			this->AllowSamepositionObject_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->AllowSamepositionObject_checkBox->Name = L"AllowSamepositionObject_checkBox";
			this->AllowSamepositionObject_checkBox->Size = System::Drawing::Size(144, 16);
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
			this->tabPage2->Size = System::Drawing::Size(1649, 291);
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
			this->EventData_flowLayoutPanel->Size = System::Drawing::Size(1643, 285);
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
			this->ObjectTemplate_xmlNodeEditComponent->Size = System::Drawing::Size(292, 290);
			this->ObjectTemplate_xmlNodeEditComponent->TabIndex = 24;
			// 
			// Main_splitContainer
			// 
			this->Main_splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Main_splitContainer->Location = System::Drawing::Point(0, 0);
			this->Main_splitContainer->Name = L"Main_splitContainer";
			this->Main_splitContainer->Panel2Collapsed = true;
			this->Main_splitContainer->Size = System::Drawing::Size(1657, 608);
			this->Main_splitContainer->SplitterDistance = 560;
			this->Main_splitContainer->TabIndex = 0;
			// 
			// assistanceToolStripMenuItem
			// 
			this->assistanceToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->showGridToolStripMenuItem, 
				this->autoAllignGridToolStripMenuItem, this->camerResetToolStripMenuItem, this->showResolutionBorderToolStripMenuItem, this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem, 
				this->showWarningMsgToolStripMenuItem});
			this->assistanceToolStripMenuItem->Name = L"assistanceToolStripMenuItem";
			this->assistanceToolStripMenuItem->Size = System::Drawing::Size(76, 20);
			this->assistanceToolStripMenuItem->Text = L"Assistance";
			// 
			// showGridToolStripMenuItem
			// 
			this->showGridToolStripMenuItem->Checked = true;
			this->showGridToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showGridToolStripMenuItem->Name = L"showGridToolStripMenuItem";
			this->showGridToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->showGridToolStripMenuItem->Text = L"ShowGrid";
			this->showGridToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// autoAllignGridToolStripMenuItem
			// 
			this->autoAllignGridToolStripMenuItem->Checked = true;
			this->autoAllignGridToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->autoAllignGridToolStripMenuItem->Name = L"autoAllignGridToolStripMenuItem";
			this->autoAllignGridToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->autoAllignGridToolStripMenuItem->Text = L"AutoAllignGrid";
			this->autoAllignGridToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// camerResetToolStripMenuItem
			// 
			this->camerResetToolStripMenuItem->Name = L"camerResetToolStripMenuItem";
			this->camerResetToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->camerResetToolStripMenuItem->Text = L"CamerReset";
			this->camerResetToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// showResolutionBorderToolStripMenuItem
			// 
			this->showResolutionBorderToolStripMenuItem->Checked = true;
			this->showResolutionBorderToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showResolutionBorderToolStripMenuItem->Name = L"showResolutionBorderToolStripMenuItem";
			this->showResolutionBorderToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->showResolutionBorderToolStripMenuItem->Text = L"ShowResolutionBorder";
			this->showResolutionBorderToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem
			// 
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Name = L"autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem";
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Text = L"AutoReadjustPositionWihleGridSizeIsChange";
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// showWarningMsgToolStripMenuItem
			// 
			this->showWarningMsgToolStripMenuItem->Name = L"showWarningMsgToolStripMenuItem";
			this->showWarningMsgToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->showWarningMsgToolStripMenuItem->Text = L"ShowWarningMsg";
			this->showWarningMsgToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
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
			this->fileToolStripMenuItem->Size = System::Drawing::Size(38, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(106, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(106, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::saveToolStripMenuItem_Click);
			// 
			// imageToolStripMenuItem
			// 
			this->imageToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->openTemplateToolStripMenuItem});
			this->imageToolStripMenuItem->Name = L"imageToolStripMenuItem";
			this->imageToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->imageToolStripMenuItem->Text = L"Xml";
			// 
			// openTemplateToolStripMenuItem
			// 
			this->openTemplateToolStripMenuItem->Name = L"openTemplateToolStripMenuItem";
			this->openTemplateToolStripMenuItem->Size = System::Drawing::Size(205, 22);
			this->openTemplateToolStripMenuItem->Text = L"AddNewNodeFromFile";
			this->openTemplateToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::addNewNodeFromFileToolStripMenuItem_Click);
			// 
			// imageToolStripMenuItem1
			// 
			this->imageToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->backgroundImageToolStripMenuItem});
			this->imageToolStripMenuItem1->Name = L"imageToolStripMenuItem1";
			this->imageToolStripMenuItem1->Size = System::Drawing::Size(55, 20);
			this->imageToolStripMenuItem1->Text = L"Image";
			// 
			// backgroundImageToolStripMenuItem
			// 
			this->backgroundImageToolStripMenuItem->Name = L"backgroundImageToolStripMenuItem";
			this->backgroundImageToolStripMenuItem->Size = System::Drawing::Size(178, 22);
			this->backgroundImageToolStripMenuItem->Text = L"BackgroundImage";
			this->backgroundImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::backgroundImageToolStripMenuItem_Click);
			// 
			// cameraResetToolStripMenuItem
			// 
			this->cameraResetToolStripMenuItem->Name = L"cameraResetToolStripMenuItem";
			this->cameraResetToolStripMenuItem->Size = System::Drawing::Size(94, 20);
			this->cameraResetToolStripMenuItem->Text = L"CameraReset";
			this->cameraResetToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::cameraResetToolStripMenuItem_Click);
			// 
			// UserDefineData_contextMenuStrip
			// 
			this->UserDefineData_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->addToolStripMenuItem, 
				this->editToolStripMenuItem, this->deleteToolStripMenuItem});
			this->UserDefineData_contextMenuStrip->Name = L"UserDefineData_contextMenuStrip";
			this->UserDefineData_contextMenuStrip->Size = System::Drawing::Size(112, 70);
			// 
			// addToolStripMenuItem
			// 
			this->addToolStripMenuItem->Name = L"addToolStripMenuItem";
			this->addToolStripMenuItem->Size = System::Drawing::Size(111, 22);
			this->addToolStripMenuItem->Text = L"Add";
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(111, 22);
			this->editToolStripMenuItem->Text = L"Edit";
			// 
			// deleteToolStripMenuItem
			// 
			this->deleteToolStripMenuItem->Name = L"deleteToolStripMenuItem";
			this->deleteToolStripMenuItem->Size = System::Drawing::Size(111, 22);
			this->deleteToolStripMenuItem->Text = L"Delete";
			// 
			// LayerData_contextMenuStrip
			// 
			this->LayerData_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->selectAllThisTypeTemplateToolStripMenuItem, 
				this->refreshUserDataFromToolStripMenuItem});
			this->LayerData_contextMenuStrip->Name = L"LayerData_contextMenuStrip";
			this->LayerData_contextMenuStrip->Size = System::Drawing::Size(251, 48);
			// 
			// selectAllThisTypeTemplateToolStripMenuItem
			// 
			this->selectAllThisTypeTemplateToolStripMenuItem->Name = L"selectAllThisTypeTemplateToolStripMenuItem";
			this->selectAllThisTypeTemplateToolStripMenuItem->Size = System::Drawing::Size(250, 22);
			this->selectAllThisTypeTemplateToolStripMenuItem->Text = L"SelectAllThisTypeTemplate";
			// 
			// refreshUserDataFromToolStripMenuItem
			// 
			this->refreshUserDataFromToolStripMenuItem->Name = L"refreshUserDataFromToolStripMenuItem";
			this->refreshUserDataFromToolStripMenuItem->Size = System::Drawing::Size(250, 22);
			this->refreshUserDataFromToolStripMenuItem->Text = L"RefreshUserDataFromXMLData";
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &ObjectDataForm::timer1_Tick);
			// 
			// ObjectDataForm
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"ObjectDataForm";
			this->Size = System::Drawing::Size(1900, 961);
			this->Load += gcnew System::EventHandler(this, &ObjectDataForm::ObjectDataForm_Load);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->flowLayoutPanel1->ResumeLayout(false);
			this->TemplateList_groupBox->ResumeLayout(false);
			this->TemplateList_groupBox->PerformLayout();
			this->XMLData_groupBox->ResumeLayout(false);
			this->UserDefineData__contextMenuStrip->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel2->ResumeLayout(false);
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
			this->Main_splitContainer->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->UserDefineData_contextMenuStrip->ResumeLayout(false);
			this->LayerData_contextMenuStrip->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(this->Visible)
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
					 XMLDataForQuickApply_treeView->Nodes->Add(l_pNode);
				 }
			}
		 }

private: System::Void AssignData()
		 {
			 if( XMLDataForQuickApply_treeView->SelectedNode )
			 {
				 //if( XMLDataForQuickApply_treeView->SelectedNode->Nodes->Count != 0 )
				 {
						ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Add((GCFORM::TreeNode^)XMLDataForQuickApply_treeView->SelectedNode->Clone());
				 }
				 //else
				 {
					//WARNING_MSG("there is no attributes could be edit");
				 }
			 }		 
		 }

private: System::Void AssignTemplateData_Click(System::Object^  sender, System::EventArgs^  e)
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
					 XMLDataForQuickApply_treeView->Nodes->Add(l_pNode);
				 }
			}
		 }
		 void	RemoveSelectedNode()
		 {
			 if( XMLDataForQuickApply_treeView->SelectedNode )
			 {
				 XMLDataForQuickApply_treeView->Nodes->Remove(XMLDataForQuickApply_treeView->SelectedNode);
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
				array<String^>^l_strFileNames = DNCT::OpenFileAndGetNames("Image Files(*.pi;*.mpdi)|*.pi;*.mpdi|All files (*.*)|*.*");
				if(l_strFileNames)
				for each(String^l_strName in l_strFileNames)
				{
					if( System::String::Compare(System::IO::Path::GetExtension(l_strName),".pi") == 0 )
					{
						if(cGameApp::m_spImageParser->GetObject(DNCT::GcStringToWchar(System::IO::Path::GetFileNameWithoutExtension(l_strName))))
						{
							WARNING_MSG(l_strName+" exists!");
							continue;
						}
						
						if(cGameApp::m_spImageParser->Parse(DNCT::GcStringToChar(l_strName)))
						{
							ImageList_treeView->Nodes->Add(DNCT::PIToTreeNode(dynamic_cast<cPuzzleImage*>(cGameApp::m_spImageParser->GetLastObject())));
						}
						else
						{
							WARNING_MSG(l_strName+" open failed");
						}
					}
					else
					if( System::String::Compare(System::IO::Path::GetExtension(l_strName),".mpdi") == 0 )
					{
						if(cGameApp::m_spImageParser->GetObject(DNCT::GcStringToWchar(System::IO::Path::GetFileNameWithoutExtension(l_strName))))
						{
							WARNING_MSG(l_strName+" exists!");
							continue;
						}
						
						if(cGameApp::m_spAnimationParser->Parse(DNCT::GcStringToChar(l_strName)))
						{
							ImageList_treeView->Nodes->Add(DNCT::MPDIListToTreeNode(dynamic_cast<cMPDIList*>(cGameApp::m_spAnimationParser->GetLastObject())));
						}
						else
						{
							WARNING_MSG(l_strName+" open failed");
						}
					}
				}
			}
			else
			if( sender == DelImage_button )
			{
				if(RemoveTextureFromSelectedNodesTreeView(ImageList_treeView,cGameApp::m_spImageParser))
					ImageList_treeView->SelectedNode->Remove();
			}
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
					 cLevelLayerGridData*l_pMap_Cell_ObjectData = this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->GetObject(DNCT::GcStringToWchar(l_strName));
					 assert(l_pMap_Cell_ObjectData&&"call fatming!! if( sender == DelTemplate_button )");
					 for(int i=0;i<this->m_pLevelEditorApp->m_pLevelData->Count();++i)
					 {
						 cLevelLayerList*l_pMap_CellData = this->m_pLevelEditorApp->m_pLevelData->GetObject(i);
						 for(int j=0;j<l_pMap_CellData->Count();++j)
						 {//this template has been assigned to the orther data,check for delete or reserved
							 cLayer*l_pMapLayer = (*l_pMap_CellData)[j];
							 for( int k=0;k<l_pMapLayer->Count();++k )
							 {
								 if(l_pMapLayer->GetObject(k)->GetImage() == l_pMap_Cell_ObjectData->GetImage())
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
										 l_pMapLayer->RemoveObject(k);
										 --k;
									 }
								 }
							 }
						 }
					 }
					 Template_listBox->Items->RemoveAt(Template_listBox->SelectedIndices[0]);
					 m_TemplateNameAndAttributeNode->Remove(l_strName);
					 this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->RemoveObject(DNCT::GcStringToWchar(l_strName));
				 }
			 }
			 else
			 if(sender == AddTemplate_button)
			 {
				 //if( !ImageList_treeView->SelectedNode )
				 //{
					// WARNING_MSG("please select Image");
					// return;
				 //}
				 //if(!ImageList_treeView->SelectedNode->Parent)
				 //{
					// WARNING_MSG("please pi's child");
					// return;				 
				 //}
				 String^l_strTemplateName = TemplateName_textBox->Text->Trim();
				 if( l_strTemplateName->Length >0 )
				 {
					 if( l_strTemplateName[0]>=L'0'&&l_strTemplateName[0]<=L'9' )
					 {
						WARNING_MSG("first character can't be numerial");
						return;
					 }
					 GCFORM::TreeNode^l_pNode;
					 cLevelLayerGridData*l_pLevelLayerGridData = this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->GetObject(DNCT::GcStringToWchar(l_strTemplateName));
					 if( l_pLevelLayerGridData || m_TemplateNameAndAttributeNode->ContainsKey(l_strTemplateName) )
					 {
						WARING_YES_NO_TO_NO("replace old data!?")
						{
							return;
						}
						Template_listBox->Items->Remove(l_strTemplateName);
						if( l_pLevelLayerGridData->GetTemplateDataNode() )
						{
							std::wstring l_strTemplateDataNodeName = l_pLevelLayerGridData->GetTemplateDataNode()->GetName();
							m_TemplateNameAndAttributeNode->Remove(DNCT::WcharToGcstring( l_strTemplateDataNodeName.c_str() ));
							m_pLevelEditorApp->m_pLevelData->m_TemplateDataNodeList.RemoveObject(l_strTemplateDataNodeName.c_str());
						}
						m_pLevelEditorApp->m_pLevelData->m_pTemplateList->RemoveObject(DNCT::GcStringToWchar(l_strTemplateName));
					 }
					 cRenderObject*l_pRenderObjectBehavior = 0;
					 //if(!this->m_pLevelEditorApp->m_pLevelData->GetObject(DNCT::GcStringToWchar(TemplateName_textBox->Text->Trim()))
					 {
						 if( ImageList_treeView->SelectedNode )
						 {
							 cPuzzleImage*l_pPi = cGameApp::m_spImageParser->GetPuzzleImage(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Parent->Text));
							 if( !l_pPi )
							 {
								 cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIList(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Parent->Text));
								 l_pRenderObjectBehavior = l_pMPDIList->GetObject(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Text));
							 }
							 else
							 {
								l_pRenderObjectBehavior = l_pPi->GetObject(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Text));
							 }
							if( !l_pRenderObjectBehavior )
							{
								WARNING_MSG("image is not exists");
								return;
							}
						}
						 else
						 {//create a empty image
							if(cGameApp::m_spImageParser->GetObject(DNCT::GcStringToWchar(TemplateName_textBox->Text)))
							{
								WARNING_MSG("empty image name is exists!!");
								return ;
							}
							cBaseImage*l_pBaseImage = new cBaseImage(0,false);
							cGameApp::m_spImageParser->AddObject(l_pBaseImage);
							l_pBaseImage->SetName(DNCT::GcStringToWchar(TemplateName_textBox->Text));
							l_pBaseImage->SetWidth((int)GridWidth_numericUpDown->Value);
							l_pBaseImage->SetHeight((int)GridHeight_numericUpDown->Value);
							l_pRenderObjectBehavior = l_pBaseImage;
						 }
						cEventDataNode*l_pEventDataNode = 0;
						 if(ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Count!=0)
						 {
							 //set node name as same as l_strTemplateName
							 //l_pNode = ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0];
							 //if(l_pNode->Text->CompareTo(l_strTemplateName))
							 //{
								//l_pNode = gcnew GCFORM::TreeNode();
								//l_pNode->Text = l_strTemplateName;
								//l_pNode->Nodes->Add((GCFORM::TreeNode^)ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0]->Clone());
							 //}
							 //else
								l_pNode = (GCFORM::TreeNode^)ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0]->Clone();

							 String^l_strText = l_pNode->Text;
							 //l_pNode->Text = l_strTemplateName;
							 l_pEventDataNode = new cEventDataNode();
							 l_pEventDataNode->SetName(DNCT::GcStringToWchar(l_strText));
							 TiXmlElement*l_pTiXmlElement = DNCT::TreeNodeToTinyXMLNode(l_pNode,0);
							 l_pEventDataNode->SetDataNode(l_pTiXmlElement);
							 SAFE_DELETE(l_pTiXmlElement);
							 m_pLevelEditorApp->m_pLevelData->m_TemplateDataNodeList.AddObject(l_pEventDataNode);
							m_TemplateNameAndAttributeNode[l_strText] = l_pNode;
						 }
						 Template_listBox->Items->Add(l_strTemplateName);
						 cLevelLayerGridData*l_pLevelLayerGridData = new cLevelLayerGridData();
						 l_pLevelLayerGridData->SetName(DNCT::GcStringToWchar(l_strTemplateName));
						 l_pLevelLayerGridData->SetImage(l_pRenderObjectBehavior);
						 l_pLevelLayerGridData->SetTemplateDataNode(l_pEventDataNode);
						 bool	l_b = this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->AddObject(l_pLevelLayerGridData);
						 assert(l_b&&"this template exists!!");
						 ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
						 Template_listBox->SelectedIndex = Template_listBox->Items->Count-1;
					 }
				 }
			 }
		 }

private: System::Void AddStage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( sender == AddStage_button )
			{
				WCHAR*	l_strName = DNCT::GcStringToWchar(StageName_textBox->Text);
				if( m_pLevelEditorApp->m_pLevelData->GetObject(l_strName) )
				{
					WARING_YES_NO_TO_NO("replace data?")
					{
						return;
					}
					//this->RemoveUserDefineNode(m_pLevelEditorApp->m_pLevelData->GetObject(l_strName));
					m_pLevelEditorApp->m_pLevelData->RemoveObject(l_strName);
					AllStage_listBox->Items->Remove(StageName_textBox->Text);
					m_pLevelEditorApp->m_bStageDataChanged = false;
				}
				m_pLevelEditorApp->m_pCurrentStageEditData->SetName(l_strName);
				Row_numericUpDown_ValueChanged(sender,e);
				GridHeight_numericUpDown_ValueChanged(sender,e);
				StartX_numericUpDown_ValueChanged(sender,e);
				cLevelLayerList*l_pMap_CellData = new cLevelLayerList(m_pLevelEditorApp->m_pCurrentStageEditData);
				//this->RemoveUserDefineNode(m_pLevelEditorApp->m_pCurrentStageEditData);
				m_pLevelEditorApp->m_pLevelData->AddObject(l_pMap_CellData);
				m_pLevelEditorApp->m_pCurrentStageEditData->Destroy();
				m_pLevelEditorApp->m_pCurrentStageEditData->SetName(L"");
				AllStage_listBox->Items->Add(StageName_textBox->Text);
				LayerObjectData_listBox->Items->Clear();
				Layer_listBox->Items->Clear();
				LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
			}
			else
			{
				if( AllStage_listBox->SelectedIndex == -1 )
					return;
				//if( m_pLevelEditorApp->m_bStageDataChanged )
				//{
				//	WARING_YES_NO_TO_NO("ignore current edit data?")
				//	{
				//		return;
				//	}
				//}
				if( sender == ReRditStagebutton )
				{//fuck......it will lost unique user define data..go check m_UniqueUserDefineDataForMap_Cell_ObjectData
					Layer_listBox->Items->Clear();
					SAFE_DELETE(m_pLevelEditorApp->m_pCurrentStageEditData);
					m_pLevelEditorApp->m_pCurrentStageEditData = new cLevelLayerList(this->m_pLevelEditorApp->m_pLevelData->GetObject(DNCT::GcStringToWchar(AllStage_listBox->SelectedItem->ToString())));
					//CopyUserDataToNewData(this->m_pLevelEditorApp->m_pLevelData->GetObject(DNCT::GcStringToWchar(AllStage_listBox->SelectedItem->ToString())),m_pLevelEditorApp->m_pCurrentStageEditData);
					Vector2	l_vSize = m_pLevelEditorApp->m_pCurrentStageEditData->GetSize();
					POINT	l_GridSize = m_pLevelEditorApp->m_pCurrentStageEditData->GetGridSize();
					Vector3	l_vPos = m_pLevelEditorApp->m_pCurrentStageEditData->GetPos();
					this->StartX_numericUpDown->Value = (int)l_vPos.x;
					this->StartY_numericUpDown->Value = (int)l_vPos.y;
					GridWidth_numericUpDown->Value = (int)l_GridSize.x;
					GridHeight_numericUpDown->Value = (int)l_GridSize.y;
					MapWidth_numericUpDown->Value = (int)l_vSize.x;
					MapHeight_numericUpDown->Value = (int)l_vSize.y;
					LayerObjectData_listBox->Items->Clear();
					for(int i=0;i<m_pLevelEditorApp->m_pCurrentStageEditData->Count();++i)
					{
						Layer_listBox->Items->Add(DNCT::WcharToGcstring(m_pLevelEditorApp->m_pCurrentStageEditData->GetObject(i)->GetName()));
					}
					if( Layer_listBox->Items->Count )
					{
						Layer_listBox->SelectedIndex = 0;
					}
					LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
				}
				else
				if( sender == DelStage_button )
				{
					//this->RemoveUserDefineNode(m_pLevelEditorApp->m_pLevelData->GetObject(AllStage_listBox->SelectedIndex));
					m_pLevelEditorApp->m_pLevelData->RemoveObject(AllStage_listBox->SelectedIndex);
					AllStage_listBox->Items->RemoveAt(AllStage_listBox->SelectedIndex);
				}
			}
		 }
private: System::Void GridHeight_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 Vector2	l_vSize = m_pLevelEditorApp->m_pCurrentStageEditData->GetSize();
			 l_vSize.x = (float)GridWidth_numericUpDown->Value;
			 l_vSize.y = (float)GridHeight_numericUpDown->Value;
			 POINT	l_GridSize = {(int)GridWidth_numericUpDown->Value,(int)GridHeight_numericUpDown->Value};
			 m_pLevelEditorApp->m_pCurrentStageEditData->SetGridSize(l_GridSize);
			 if( autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Checked )
			 {
				 float	l_fWidth = m_pLevelEditorApp->m_pOrthogonalCamera->GetGridWidth()*(int)MapWidth_numericUpDown->Value;
				 float	l_fHeight = m_pLevelEditorApp->m_pOrthogonalCamera->GetGridHeight()*(int)MapHeight_numericUpDown->Value;
				 for( int i=0;i<m_pLevelEditorApp->m_pCurrentStageEditData->Count();++i )
				 {
					cLayer*l_pMapLayer = (*m_pLevelEditorApp->m_pCurrentStageEditData)[i];
					for( int j=0;j<l_pMapLayer->Count();++j )
					{
						cLevelLayerGridData*l_pMap_Cell_ObjectData = (*l_pMapLayer)[i];
						Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
						if( autoAllignGridToolStripMenuItem->Checked )//by grid position
						{
							//if( l_vPos.x >= 0.f&&l_fWidth >= l_vPos.x&&l_vPos.y >= 0.f&&l_fHeight >= l_vPos.y )
							{
								int	l_iRow = (int)(l_vPos.x/m_pLevelEditorApp->m_pOrthogonalCamera->GetGridWidth());
								int	l_iColumn = (int)(l_vPos.y/m_pLevelEditorApp->m_pOrthogonalCamera->GetGridHeight());
								l_vPos.x = l_iRow*l_vSize.x;
								l_vPos.y = l_iColumn*l_vSize.y;
							}
						}
						else//by scale rate
						{
							float	l_fScaleRateX = l_vSize.x/m_pLevelEditorApp->m_pOrthogonalCamera->GetGridWidth();
							float	l_fScaleRateY = l_vSize.y/m_pLevelEditorApp->m_pOrthogonalCamera->GetGridHeight();
							l_vPos.x *= l_fScaleRateX;
							l_vPos.y *= l_fScaleRateY;
						}
						l_pMap_Cell_ObjectData->SetPos(l_vPos);
					}
				 }
			 }
			 m_pLevelEditorApp->m_pOrthogonalCamera->SetGridHeight((float)GridHeight_numericUpDown->Value);
			 m_pLevelEditorApp->m_pOrthogonalCamera->SetGridWidth((float)GridWidth_numericUpDown->Value);
		 }
private: System::Void showGridToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 //if( sender == showGridToolStripMenuItem )
			 {
				 System::Windows::Forms::ToolStripMenuItem^l_pSender = (System::Windows::Forms::ToolStripMenuItem^)sender;
				 l_pSender->Checked = !l_pSender->Checked;
			 }
		 }
private: System::Void Row_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 Vector2	l_Size = m_pLevelEditorApp->m_pCurrentStageEditData->GetSize();
			 l_Size.x = (float)MapWidth_numericUpDown->Value;
			 l_Size.y = (float)MapHeight_numericUpDown->Value;
			 m_pLevelEditorApp->m_pCurrentStageEditData->SetSize(l_Size);
		 }
private: System::Void Template_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( Template_listBox->SelectedIndex != -1 )
			 {
				 WCHAR*l_strName = DNCT::GcStringToWchar(Template_listBox->SelectedItem->ToString());
				 m_pLevelEditorApp->m_pCurrentSelectedImage = this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->GetObject(l_strName)->GetImage();
				 TemplateDataInvertorReEdit();
			 }
		 }
private: System::Void Clear_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList() )
				m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList()->Destroy();
			LayerObjectData_listBox->Items->Clear();
		 }

private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strFileName = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
			 if( l_strFileName )
			 {
				this->m_pLevelEditorApp->m_pLevelData->Export(DNCT::GcStringToChar(l_strFileName));
				//for quick apply
				String^l_strTempDirectory = System::IO::Path::GetDirectoryName(l_strFileName)+"/XmlTemp";
				System::IO::Directory::CreateDirectory(l_strTempDirectory);
				l_strTempDirectory +="/temp.xml";
				DNCT::SaveTreeViewToFile(XMLDataForQuickApply_treeView,l_strTempDirectory);
				//for template file
				//ObjectDataForm::SaveTreeNodeFile( System::IO::Path::ChangeExtension(l_strFileName,"tmp"),m_TemplateNameAndAttributeNode,false);
				//for unique user define data
				//ObjectDataForm::SaveTreeNodeFile( System::IO::Path::ChangeExtension(l_strFileName,"userdefine"),m_UniqueUserDefineDataForMap_Cell_ObjectData,true);
			 }
		 }
private: System::Void backgroundImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			String^l_strFileName = DNCT::OpenFileAndGetName();
			if( l_strFileName )
			{
				SAFE_DELETE(m_pLevelEditorApp->m_pBGImage);
				m_pLevelEditorApp->m_pBGImage = new cBaseImage(DNCT::GcStringToChar(l_strFileName));
			}
		 }

private: System::Void ReEditTemplate_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 TemplateDataInvertorReEdit();
		 }
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			timer1->Enabled = false;
			String^l_strFileName = DNCT::OpenFileAndGetName();
			if( l_strFileName )
			{
				Layer_listBox->Items->Clear();
				AllStage_listBox->Items->Clear();
				this->ImageList_treeView->Nodes->Clear();
				LayerObjectData_listBox->Items->Clear();
				this->m_TemplateNameAndAttributeNode->Clear();
				this->Template_listBox->Items->Clear();
				m_UniqueUserDefineDataForMap_Cell_ObjectData->Clear();
				cGameApp::m_spAnimationParser->Destroy();
				cGameApp::m_spImageParser->Destroy();
				m_pLevelEditorApp->DestoryOldData();
				bool	l_b = this->m_pLevelEditorApp->m_pLevelData->ParseWithMyParse(DNCT::GcStringToChar(l_strFileName));
				if( l_b )
				{
					//return;
					//for(int i=0;i<cGameApp::m_spImageParser->Count();++i)
					{
						ImagerParserAddIntiTreeView(cGameApp::m_spImageParser,ImageList_treeView,true);
					}
					//for(int i=0;i<cGameApp::m_spImageParser->Count();++i)
					{
						ImagerParserAddIntiTreeView(cGameApp::m_spAnimationParser,ImageList_treeView,false);
					}
					//l_strFileName = System::IO::Path::ChangeExtension(l_strFileName,"tmp");
					//treeView1->Nodes->Clear();
					//GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_strFileName);
					//if(l_pNode)
					//{
					//	for each(GCFORM::TreeNode^l_pNode in l_pNode->Nodes)
					//	{
					//		treeView1->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
					//		m_TemplateNameAndAttributeNode->Add(l_pNode->Text,l_pNode->Clone());
					//	}
					//}
					for(int i=0;i<this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->Count();++i)
					{
						this->Template_listBox->Items->Add(DNCT::WcharToGcstring((*this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList)[i]->GetName()));
					}
					for(int i=0;i<this->m_pLevelEditorApp->m_pLevelData->Count();++i)
					{
						AllStage_listBox->Items->Add(DNCT::WcharToGcstring(this->m_pLevelEditorApp->m_pLevelData->GetObject(i)->GetName()));
					}
					//open quick apply xml data
					OpenQuickApplyXmlFile(l_strFileName);
					//OpenUserDefineData(l_strFileName);
					for(int i=0;i<m_pLevelEditorApp->m_pLevelData->m_TemplateDataNodeList.Count();++i)
					{
						cEventDataNode*l_pEventDataNode = m_pLevelEditorApp->m_pLevelData->m_TemplateDataNodeList[i];
						TiXmlElement*l_pTiXmlElement = l_pEventDataNode->GetDataNode();
						GCFORM::TreeNode^l_pTreeNode = TinyXMLNodeToTreeNode(l_pTiXmlElement);
						m_TemplateNameAndAttributeNode[gcnew String(l_pEventDataNode->GetCharName().c_str())] = l_pTreeNode;
					}
					for( int i=0;i<m_pLevelEditorApp->m_pLevelData->m_EventDataNodeList.Count();++i )
					{
						cEventDataNode*l_pEventDataNode = m_pLevelEditorApp->m_pLevelData->m_EventDataNodeList[i];
						GCFORM::TreeNode^l_pTreeNode = TinyXMLNodeToTreeNode(l_pEventDataNode->GetDataNode());
						m_UniqueUserDefineDataForMap_Cell_ObjectData[l_pEventDataNode->GetUniqueID()] = l_pTreeNode;
					}
				}
			}
			timer1->Enabled = true;
		 }
private: System::Void StartX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_pLevelEditorApp->m_pCurrentStageEditData->SetPos(Vector3((float)this->StartX_numericUpDown->Value,(float)this->StartY_numericUpDown->Value,0.f));
		 }
private: System::Void AddToLayer_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( DeleteLayer_button == sender )
			 {
				 timer1->Enabled = false;
				 Sleep(10);
				 LayerObjectData_listBox->Items->Clear();
				 while(Layer_listBox->SelectedIndices->Count>0)
				 {
					int	l_iSelectedIndex = Layer_listBox->SelectedIndices[0];
					//RemoveUserDefineNode(m_pLevelEditorApp->m_pCurrentStageEditData->GetObject(l_iSelectedIndex));
					m_pLevelEditorApp->m_pCurrentStageEditData->RemoveObject(l_iSelectedIndex);
					Layer_listBox->Items->RemoveAt(l_iSelectedIndex);
					m_pLevelEditorApp->m_pCurrentStageEditData->SetCurrentList((cLayer*)0);
				 }
				 timer1->Enabled = true;
			 }
			 else
			 if( sender == AddToLayer_button )
			 {
				 if(LayerName_textBox->Text->Length)
				 {
					 if(!DNCT::CheckListContainStringAndAdd(Layer_listBox,LayerName_textBox->Text,true))
					 {
						 cLayer*l_pMapLayer = new cLayer();
						 l_pMapLayer->SetName(DNCT::GcStringToWchar(LayerName_textBox->Text));
						 m_pLevelEditorApp->m_pCurrentStageEditData->AddObject(l_pMapLayer);
						 Layer_listBox->SelectedIndices->Clear();
						 for( int i=0;i<Layer_listBox->Items->Count;++i )
							Layer_listBox->SelectedIndices->Add(i);
						 m_pLevelEditorApp->m_pCurrentStageEditData->SetCurrentList(Layer_listBox->Items->Count-1);
					 }
				 }
			 }
		 }

private: System::Void AllStage_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			if( m_pLevelEditorApp->m_pCurrentStageEditData->Count() )
			{
				if( showWarningMsgToolStripMenuItem->Checked  )
				{
					WARING_YES_NO_TO_NO("it will clear exist data!would u like to do it?")
						return;
				}
			}
			AddStage_button_Click(ReRditStagebutton,e);
		 }
private: System::Void LayerDown_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( LayerUp_button == sender )
			 {
				 if( Layer_listBox->SelectedIndex != 0 && Layer_listBox->Items->Count>=2 )
				 {
					 int	l_iSelectedIndex = Layer_listBox->SelectedIndex;
					 String^l_strName1 = Layer_listBox->Items[l_iSelectedIndex]->ToString();
					 String^l_strName2 = Layer_listBox->Items[l_iSelectedIndex-1]->ToString();
					 Layer_listBox->Items[l_iSelectedIndex] = l_strName2;
					 Layer_listBox->Items[l_iSelectedIndex-1] = l_strName1;
					 m_pLevelEditorApp->m_pCurrentStageEditData->SwapIndex(l_iSelectedIndex,l_iSelectedIndex-1);
				 }
			 }
			 else
			 if( sender == LayerDown_button )
			 {
				 if( Layer_listBox->SelectedIndex != Layer_listBox->Items->Count-1&& Layer_listBox->Items->Count>=2 )
				 {
					 int	l_iSelectedIndex = Layer_listBox->SelectedIndex;
					 String^l_strName1 = Layer_listBox->Items[l_iSelectedIndex]->ToString();
					 String^l_strName2 = Layer_listBox->Items[l_iSelectedIndex+1]->ToString();
					 Layer_listBox->Items[l_iSelectedIndex] = l_strName2;
					 Layer_listBox->Items[l_iSelectedIndex+1] = l_strName1;				 
					 m_pLevelEditorApp->m_pCurrentStageEditData->SwapIndex(l_iSelectedIndex,l_iSelectedIndex+1);
				 }				
			 }
		 }
private: System::Void AllStage_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(AllStage_listBox->SelectedItem)
			 {
				 StageName_textBox->Text = AllStage_listBox->SelectedItem->ToString();
			 }
		 }
private: System::Void cameraResetToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			m_pLevelEditorApp->m_pOrthogonalCamera->SetResolution(Vector2(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y));
			m_pLevelEditorApp->m_pOrthogonalCamera->Reset();
		 }
private: System::Void AddObjectToStage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 Vector3	l_vPos = Vector3((float)AddObjectX_numericUpDown->Value,(float)AddObjectY_numericUpDown->Value,(float)AddObjectZ_numericUpDown->Value);
			 m_pLevelEditorApp->AddObjectIntoCurrentStage(l_vPos,LayerObjectData_listBox,Template_listBox);
		 }
private: System::Void DeleteObject_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 while(LayerObjectData_listBox->SelectedIndices->Count>0)
			 {
				int	l_iSelectedIndex = LayerObjectData_listBox->SelectedIndices[0];
				//RemoveUserDefineNode(m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList()->GetObject(l_iSelectedIndex));
				m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList()->RemoveObject(l_iSelectedIndex);
				LayerObjectData_listBox->Items->RemoveAt(l_iSelectedIndex);
			 }
		 }
private: System::Void MyKeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
		 {
		    if( this->splitContainer2->Focused )
			{
                if( e->KeyChar == L'D'||
                    e->KeyChar == L'd' )
				{
					int	l_iColldedIndex = m_pLevelEditorApp->m_pCurrentStageEditData->CollideForSamePosition(m_pLevelEditorApp->m_vCurrentPlacePos);
			        if( l_iColldedIndex != -1 )
			        {
				        m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList()->RemoveObject(l_iColldedIndex);
				        LayerObjectData_listBox->Items->RemoveAt(l_iColldedIndex);
			        }
				}
				else
				if( e->KeyChar == L'a'||
				    e->KeyChar == L'A' )
				{
				    if( !AllowSamepositionObject_checkBox->Checked )
					{
						this->m_pLevelEditorApp->AddObjectOnCell(LayerObjectData_listBox,Template_listBox);
					}
				}
            }
		 }
	private: System::Void MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				if( e->KeyCode == Keys::Add  )
				{

				}
			 }

private: System::Void ObjectDataForm_Load(System::Object^  sender, System::EventArgs^  e)
        {
            splitContainer1->SplitterDistance = 213;
        }
private: System::Void ImageList_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		 {
            if( ImageList_treeView->SelectedNode && ImageList_treeView->SelectedNode->Parent )
            {
				 TemplateName_textBox->Text = ImageList_treeView->SelectedNode->Text;
				 WCHAR*l_strSelectedName = DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Parent->Text);
                 cPuzzleImage*l_pPI = dynamic_cast<cPuzzleImage*>(cGameApp::m_spImageParser->GetObject(l_strSelectedName));
				 if( l_pPI )
				 {
					cPuzzleImageUnit*l_pPuzzleImageUnit = dynamic_cast<cPuzzleImageUnit*>(l_pPI->GetObject(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Text)));
					//label9->Text = DNCT::WcharToGcstring(l_pPuzzleImageUnit->GetTexture()->DescriptionInfo());
					m_pLevelEditorApp->m_pHintImage = l_pPuzzleImageUnit;
				 }
				 else
				 {
					cMPDIList*l_pMPDIList = dynamic_cast<cMPDIList*>(cGameApp::m_spAnimationParser->GetObject(l_strSelectedName));
					if( l_pMPDIList )
					{
						cMPDI*l_pMPDI = l_pMPDIList->GetObject(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Text));
						//label9->Text = DNCT::WcharToGcstring(l_pMPDI->DescriptionInfo());
						l_pMPDI->Init();
						m_pLevelEditorApp->m_pHintImage = l_pMPDI;
					}
				 }
			}
			else
			{
			    //label9->Text = "";
			}
			//if( ImageList_listBox->SelectedIndex!=-1 )
			//{
			//	cBaseImage*l_pNewImage = cGameApp::m_spImageParser->GetObject(DNCT::GcStringToWchar(ImageList_listBox->Items[ImageList_listBox->SelectedIndex]->ToString()));
			//	if( Template_listBox->SelectedIndex != -1 )
			//	{//change old data image as new
			//		String^l_strName = Template_listBox->Items[Template_listBox->SelectedIndex]->ToString();
			//		cLevelLayerGridData*l_pMap_Cell_ObjectData = this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->GetObject(DNCT::GcStringToWchar(l_strName));
			//		assert(l_pMap_Cell_ObjectData&&"call fatming!! if( sender == DelTemplate_button )");
			//		for(int i=0;i<this->m_pLevelEditorApp->m_pLevelData->Count();++i)
			//		{
			//			cLevelLayerList*l_pMap_CellData = this->m_pLevelEditorApp->m_pLevelData->GetObject(i);
			//			for(int j=0;j<l_pMap_CellData->Count();++j)
			//			{
			//				if((*l_pMap_CellData)[j]->GetImage() == l_pMap_Cell_ObjectData->GetImage())
			//				{
			//					(*l_pMap_CellData)[j]->SetImage(l_pNewImage);
			//				}
			//			}
			//		}
			//	}
			//	Image_textBox->Text = ImageList_listBox->Items[ImageList_listBox->SelectedIndex]->ToString();
			//	//m_pLevelEditorApp->m_pCurrentSelectedImage = l_pNewImage;
			//}
			////else
			////	m_pLevelEditorApp->m_pCurrentSelectedImage = 0;
		 }
private: System::Void Layer_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 LayerObjectData_listBox->Items->Clear();
			 if( Layer_listBox->SelectedIndex == -1 )
			 {
				 m_pLevelEditorApp->m_pCurrentStageEditData->SetCurrentList((cLayer*)0);
			 }
			 else
			 {
				m_pLevelEditorApp->m_pCurrentStageEditData->SetCurrentList(Layer_listBox->SelectedIndex);
				DumpObjectListIntoListBox(LayerObjectData_listBox,m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList(),true);
			 }
		 }
private: System::Void ImageList_treeView_BeforeSelect(System::Object^  sender, System::Windows::Forms::TreeViewCancelEventArgs^  e)
         {
            if(this->ImageList_treeView->SelectedNode)
            {
                this->ImageList_treeView->SelectedNode->BackColor = System::Drawing::Color::White;
                this->ImageList_treeView->SelectedNode->ForeColor = System::Drawing::Color::Black;
            }
         }
private: System::Void AllStage_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( e->KeyCode == GCFORM::Keys::Enter )
			 {
				System::Windows::Forms::MouseEventArgs^  e2;
				AllStage_listBox_MouseDoubleClick(sender,e2);
			 }
		 }
private: System::Void LayerObjectData_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
        {
            if( LayerObjectData_listBox->SelectedIndex != -1)
            {
                LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
                m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData = m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList()->GetObject(LayerObjectData_listBox->SelectedIndex);
                if( m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData->GetEventDataNode() && m_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey(m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData->GetEventDataNode()->GetUniqueID()) )
                {
                    GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)m_UniqueUserDefineDataForMap_Cell_ObjectData[m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData->GetEventDataNode()->GetUniqueID()];
                    if( l_pNode )
                    {
                        LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
                    }
                }
            }
			else
			{
				LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
				ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
				m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData = 0;
			}
        }

private: System::Void SyncUserDefineDataByFile_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
            WARNING_MSG("select event data to sync userdefine data,it's possible edit event data at another file,and want to sync current user define data");
            String^l_strFileName = DNCT::OpenFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
            if( l_strFileName )
            {
                GCFORM::TreeNode^l_pEventDefinedEventNode = DNCT::OpenXmlFileToNode(nullptr);
                for each(String^l_strKey in this->m_TemplateNameAndAttributeNode->Keys)
                {
                    //if the userdefine data is define by event editor it will contain a name has named in the editor
                    //now we find it out
                    //<UserDefineData Stage="cc" Layer="a1" Index="9">
                    GCFORM::TreeNode^l_pUserDefineNode = (GCFORM::TreeNode^)m_TemplateNameAndAttributeNode[l_strKey];
                    if( l_pUserDefineNode->Nodes->Count == 1 )//UserDefineData
                    {
                        assert(l_pUserDefineNode->Nodes[0]->Nodes->Count>=4&&"this user define data is empty!?");
                        //get event node?(because its possible not)
                        GCFORM::TreeNode^l_pUserdefineNodeFromEventNode = l_pUserDefineNode->Nodes[0]->Nodes[3];
                        if( l_pUserdefineNodeFromEventNode->Name->Length )
                        {//it is a attribute,it's not from event editor
                            continue;
                        }
                        //ok it's possible defined by event editor,now check the name if it exist set the node
                        for each( GCFORM::TreeNode^l_pNode in l_pEventDefinedEventNode->Nodes )
                        {
                            //ensure it's edit in the event the first node is a element without attrbute
                            if( l_pNode->Name->Length == 0 )
                            {
                                //ok the name is matched, it might  edited by event editor
                                if(l_pUserdefineNodeFromEventNode->Text->Equals(l_pNode->Text))
                                {
                                    //remove old data sync event file to this
                                    l_pUserDefineNode->Nodes[0]->Nodes[3]->Remove();
                                    l_pUserDefineNode->Nodes[0]->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
                                }
                            }
                        }
                    }
                }
            }
        }
private: System::Void XMLDataForQuickApply_treeView_ItemDrag(System::Object^  sender, System::Windows::Forms::ItemDragEventArgs^  e) 
		 {
			 if(XMLDataForQuickApply_treeView->SelectedNode)
			 {
				 this->XMLDataForQuickApply_treeView->DoDragDrop(XMLDataForQuickApply_treeView->SelectedNode,DragDropEffects::Move);
			 }
		 }
private: System::Void assignToObjectEventDataToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if( LayerObjectData_listBox->SelectedIndex != -1 )
			 {
				 AddEventDataNode(m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData,XMLDataForQuickApply_treeView->SelectedNode);
			 }
			 else
			 {
				 WARNING_MSG("please select object!");
			 }
		 }

private: System::Void ObjectData_treeView_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			 GCFORM::TreeNode^l_pTreeNode;
			 int l_iIndex = LayerObjectData_listBox->SelectedIndex;
			 if( l_iIndex != -1 )
			 {
				l_pTreeNode = DragDropEventWhileTreeNodeDrop(e,true);
				if( l_pTreeNode )
				{
					AddEventDataNode(m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData,l_pTreeNode);
				}			 
			 }
		 }


private: System::Void	LevelLayerGridEventDataNode_xmlNodeEditComponentTreeViewDeleteOrCreateFunction(System::Object^sender,System::EventArgs^e)
		 {
			 if( m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData )
			 {
				if( LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Count == 0 )
				{
					RemoveEventDataNode(m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData);
					return;
				}
				else
				//if(!m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData->GetEventDataNode())
				{
					AddEventDataNode(m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData,LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0]);
				}
			 }
			 m_UniqueUserDefineDataForMap_Cell_ObjectData[m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData->GetUniqueID()] = LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0];
		 }
private: System::Void ImageList_treeView_MouseLeave(System::Object^  sender, System::EventArgs^  e) 
		 {
			m_pLevelEditorApp->m_pHintImage = 0;
		 }
private: System::Void NoImage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 ImageList_treeView->SelectedNode = nullptr;
		 }
};
}