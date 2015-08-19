#pragma once

#undef GetCurrentDirectory

namespace XMLDataEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class XMLForm : public System::Windows::Forms::UserControl
	{
	public:
		XMLForm()
		{
			InitializeComponent();
			this->Text= "XMLEditor";
			m_iCurrentSelectedListBoxIndex = -1;
			//
			//
			m_bDisableUpdateNodeInfo = false;
			m_pLabelAndNode = gcnew System::Collections::Hashtable();
			NodeValue_textBox->Visible = false;
			m_pNodeListForAppend = gcnew System::Collections::Hashtable();
			//NodeFile^l_pNodeFile = gcnew NodeFile();
			//l_pNodeFile->m_pHashTable = m_pNodeListForAppend;
			//m_pStringForExeFilePath = 	System::IO::Directory::GetCurrentDirectory()+"\\";
			//l_pNodeFile->LoadFile(TO_GCSTRING("default.fun"),m_pStringForExeFilePath);

			NodeName_textBox->Size = System::Drawing::Size(splitContainer1->Panel1->Size.Width-20,splitContainer1->Panel1->Size.Height);
			NodeValue_textBox->Size = System::Drawing::Size(splitContainer1->Panel2->Size.Width-20,splitContainer1->Panel1->Size.Height);

			m_AllLabelList = gcnew System::Collections::ArrayList;
			for( int i=0;i<100;++i )
			{
				GCFORM::Label^l_pLabel = gcnew GCFORM::Label();
				l_pLabel->Name = i.ToString();
				l_pLabel->Margin = label1->Margin;
				l_pLabel->Enabled = true;
				l_pLabel->Click += gcnew System::EventHandler(this, &XMLForm::label1_Click);
				l_pLabel->DoubleClick += gcnew System::EventHandler(this, &XMLForm::label1_DoubleClick);
				l_pLabel->Size = System::Drawing::Size(320,CurrentAttribute_treeView->ItemHeight);
				m_AllLabelList->Add(l_pLabel);
				this->splitContainer1->Panel2->Controls->Add(l_pLabel);
			}
			m_pbCut = new bool;
			*m_pbCut = false;
			m_RootName = "Root";
			System::Object^  sender; System::EventArgs^  e;
			XMLForm_Resize(sender,e);
			Adjust();
		}

	protected:
		/// <summary>
		/// </summary>
		~XMLForm()
		{
			delete m_pbCut;
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  NodeValue_textBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	public: System::Windows::Forms::TreeView^  CurrentAttribute_treeView;
	private: System::Windows::Forms::Timer^  timer1;			 
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::ContextMenuStrip^  TextTreeMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  AddGroupToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  AddChildToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  DeleteGroupToolStripMenuItem;
	private: System::Windows::Forms::TextBox^  NodeName_textBox;
	private: System::Windows::Forms::ToolStripMenuItem^  addCommentToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addTextToolStripMenuItem;
	private: System::Windows::Forms::Button^  AttributeUp_button;
	private: System::Windows::Forms::Button^  AttributeDown_button;
	private: System::Windows::Forms::ToolStripMenuItem^  addFilesIntoAttributeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addTextIntoAttributeToolStripMenuItem;
	private: System::Windows::Forms::Label^  label1;
	public: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  AddComment_button;
	private: System::Windows::Forms::Label^  label3;
	public: System::Windows::Forms::TextBox^  Value_textBox;
	public: System::Windows::Forms::TextBox^  Attribute_textBox;
	private: System::Windows::Forms::Button^  Cut_button;
	private: System::Windows::Forms::Button^  Paste_button;
	private: System::Windows::Forms::Button^  Copy_button;
	private: System::Windows::Forms::Button^  AddElement_button;
	private: System::Windows::Forms::Button^  Del_button;
	private: System::Windows::Forms::Button^  AddAttribute_button;
	private: System::Windows::Forms::Button^  Replace_button;
	private: System::Windows::Forms::ToolStripMenuItem^  cutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  copyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  pasteToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  expandToolStripMenuItem;
	public: 
	//my
	public:[TypeConverterAttribute(String::typeid),DescriptionAttribute("XMlElementValuePanelCollpase")]
		   [CategoryAttribute("My")]
		   property bool CollpaseXMlElementValuePanel
		   {
			   bool get(){ return this->splitContainer1->Panel2Collapsed; }
			   System::Void set(bool e_b){ splitContainer1->Panel2Collapsed = e_b; }
		   }
	public: String^m_RootName;
		//cut or paste if cut is true remove connection while paste
		bool	*m_pbCut;
	private: System::Windows::Forms::ToolStripMenuItem^  replaceNodeByXMLTemplateToolStripMenuItem;
private: System::Windows::Forms::Button^  Panel2CollpaseChange_button;
	public: 
		//this one for cut and Copy and paste
	System::Windows::Forms::TreeNode^m_pCurrentTempNode;
	//for buttons	
	void	DoNewRootNodeBySelectedNodeFromTreeNode(System::Windows::Forms::TreeView^e_pTreeView);
	void	DoNewRootNode(System::Windows::Forms::TreeNode^e_pNode);

	void	DoPasteButton();
	void	DoCutButton();
	void	DoCopyButton();
	void	DoReplaceButton();
	void	DoAddElementButton();
	void	DoAddCommentButton();
	void	DoDeleteButton();
	void	DoAddAttributeButton();
	public:	System::EventHandler^				m_TreeViewDeleteOrCreateFunction;
	//
	public:	 System::Void Adjust()
			 {
				 System::EventArgs^  e;
				 splitContainer1_Panel2_Resize(this,e);
			 }
	private: int			m_iCurrentSelectedListBoxIndex;	
	private: System::Void	ReNameTreeNode();
	private: System::Void	ModifyTreeNodeData();
	private: System::String^GetTextStringFrom(String^e_str);
	private: System::Void	SaveGameFile(System::String^e_pString);
	private: bool			SaveXmlFile(System::Object^  sender);
	private: System::Void	WriteInformation(ATG::XMLWriter*e_pWriter);
	private: System::Void	SearchNodes(GCFORM::TreeNode^e_pNode,System::String^e_pStringToCompare,bool* e_b );
	private: System::Void	AddAttributeIntoTreeNode();
	private: System::Void	AddElementIntoTreeNode();
	private: System::Void	AddTextIntoTreeNode();
	private: System::Void	AddComment();
	private: System::Void   DeleteTreeNode();
	private: System::String^m_pStringForFileName;
	//contain GCFORM::TreeNode,and use this to add to selected node
	private: System::Collections::Hashtable^	m_pNodeListForAppend;
	private: System::String	^m_pStringForExeFilePath;
	//arraylist to store whole labe
	private: System::Collections::ArrayList^m_AllLabelList;
	private: GCFORM::TreeNode^m_pTopNode;
	private: GCFORM::Label^m_pActivedLabel;
	//while click the labe focus on the correspond node
	private: System::Collections::Hashtable^m_pLabelAndNode;
    //
	private: bool	m_bDisableUpdateNodeInfo;
	public:[TypeConverterAttribute(String::typeid),DescriptionAttribute("XMLFormTextDescription")]
		   [CategoryAttribute("My")]
		   property String^ CollpaseButtonName
		   {
				String^ get(){ return groupBox1->Text; }
				System::Void set(String^ value){ groupBox1->Text = value; }
		   }
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
			this->NodeValue_textBox = (gcnew System::Windows::Forms::TextBox());
			this->TextTreeMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->AddGroupToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AddChildToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addCommentToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addTextToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->DeleteGroupToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addFilesIntoAttributeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addTextIntoAttributeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->copyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pasteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->expandToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->replaceNodeByXMLTemplateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->CurrentAttribute_treeView = (gcnew System::Windows::Forms::TreeView());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->NodeName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->AttributeUp_button = (gcnew System::Windows::Forms::Button());
			this->AttributeDown_button = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->Panel2CollpaseChange_button = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->AddComment_button = (gcnew System::Windows::Forms::Button());
			this->Cut_button = (gcnew System::Windows::Forms::Button());
			this->Paste_button = (gcnew System::Windows::Forms::Button());
			this->Copy_button = (gcnew System::Windows::Forms::Button());
			this->AddElement_button = (gcnew System::Windows::Forms::Button());
			this->Del_button = (gcnew System::Windows::Forms::Button());
			this->AddAttribute_button = (gcnew System::Windows::Forms::Button());
			this->Replace_button = (gcnew System::Windows::Forms::Button());
			this->Value_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Attribute_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->TextTreeMenuStrip->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->SuspendLayout();
			// 
			// NodeValue_textBox
			// 
			this->NodeValue_textBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->NodeValue_textBox->ContextMenuStrip = this->TextTreeMenuStrip;
			this->NodeValue_textBox->Location = System::Drawing::Point(2, 3);
			this->NodeValue_textBox->Name = L"NodeValue_textBox";
			this->NodeValue_textBox->Size = System::Drawing::Size(714, 15);
			this->NodeValue_textBox->TabIndex = 22;
			this->NodeValue_textBox->Visible = false;
			this->NodeValue_textBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &XMLForm::NodeValue_textBox_KeyUp);
			// 
			// TextTreeMenuStrip
			// 
			this->TextTreeMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(12) {this->AddGroupToolStripMenuItem, 
				this->AddChildToolStripMenuItem, this->addCommentToolStripMenuItem, this->addTextToolStripMenuItem, this->DeleteGroupToolStripMenuItem, 
				this->addFilesIntoAttributeToolStripMenuItem, this->addTextIntoAttributeToolStripMenuItem, this->cutToolStripMenuItem, this->copyToolStripMenuItem, 
				this->pasteToolStripMenuItem, this->expandToolStripMenuItem, this->replaceNodeByXMLTemplateToolStripMenuItem});
			this->TextTreeMenuStrip->Name = L"TextTreeMenuStrip";
			this->TextTreeMenuStrip->Size = System::Drawing::Size(247, 268);
			// 
			// AddGroupToolStripMenuItem
			// 
			this->AddGroupToolStripMenuItem->Name = L"AddGroupToolStripMenuItem";
			this->AddGroupToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->AddGroupToolStripMenuItem->Text = L"Add Element";
			this->AddGroupToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::addGroupToolStripMenuItem_Click_1);
			// 
			// AddChildToolStripMenuItem
			// 
			this->AddChildToolStripMenuItem->Name = L"AddChildToolStripMenuItem";
			this->AddChildToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->AddChildToolStripMenuItem->Text = L"AddAttribute";
			this->AddChildToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::addGroupToolStripMenuItem_Click_1);
			// 
			// addCommentToolStripMenuItem
			// 
			this->addCommentToolStripMenuItem->Name = L"addCommentToolStripMenuItem";
			this->addCommentToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->addCommentToolStripMenuItem->Text = L"AddComment";
			this->addCommentToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::addGroupToolStripMenuItem_Click_1);
			// 
			// addTextToolStripMenuItem
			// 
			this->addTextToolStripMenuItem->Name = L"addTextToolStripMenuItem";
			this->addTextToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->addTextToolStripMenuItem->Text = L"AddText";
			this->addTextToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::addGroupToolStripMenuItem_Click_1);
			// 
			// DeleteGroupToolStripMenuItem
			// 
			this->DeleteGroupToolStripMenuItem->Name = L"DeleteGroupToolStripMenuItem";
			this->DeleteGroupToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->DeleteGroupToolStripMenuItem->Text = L"Delete";
			this->DeleteGroupToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::addGroupToolStripMenuItem_Click_1);
			// 
			// addFilesIntoAttributeToolStripMenuItem
			// 
			this->addFilesIntoAttributeToolStripMenuItem->Name = L"addFilesIntoAttributeToolStripMenuItem";
			this->addFilesIntoAttributeToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->addFilesIntoAttributeToolStripMenuItem->Text = L"AddFilesIntoAttribute";
			this->addFilesIntoAttributeToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::addFilesIntoAttributeToolStripMenuItem_Click);
			// 
			// addTextIntoAttributeToolStripMenuItem
			// 
			this->addTextIntoAttributeToolStripMenuItem->Name = L"addTextIntoAttributeToolStripMenuItem";
			this->addTextIntoAttributeToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->addTextIntoAttributeToolStripMenuItem->Text = L"AddTextIntoAttribute";
			this->addTextIntoAttributeToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::addFilesIntoAttributeToolStripMenuItem_Click);
			// 
			// cutToolStripMenuItem
			// 
			this->cutToolStripMenuItem->Name = L"cutToolStripMenuItem";
			this->cutToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->cutToolStripMenuItem->Text = L"Cut";
			this->cutToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::AddElement_button_Click);
			// 
			// copyToolStripMenuItem
			// 
			this->copyToolStripMenuItem->Name = L"copyToolStripMenuItem";
			this->copyToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->copyToolStripMenuItem->Text = L"Copy";
			this->copyToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::AddElement_button_Click);
			// 
			// pasteToolStripMenuItem
			// 
			this->pasteToolStripMenuItem->Name = L"pasteToolStripMenuItem";
			this->pasteToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->pasteToolStripMenuItem->Text = L"Paste";
			this->pasteToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::AddElement_button_Click);
			// 
			// expandToolStripMenuItem
			// 
			this->expandToolStripMenuItem->Name = L"expandToolStripMenuItem";
			this->expandToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->expandToolStripMenuItem->Text = L"Expand";
			this->expandToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::expandToolStripMenuItem_Click);
			// 
			// replaceNodeByXMLTemplateToolStripMenuItem
			// 
			this->replaceNodeByXMLTemplateToolStripMenuItem->Name = L"replaceNodeByXMLTemplateToolStripMenuItem";
			this->replaceNodeByXMLTemplateToolStripMenuItem->Size = System::Drawing::Size(246, 22);
			this->replaceNodeByXMLTemplateToolStripMenuItem->Text = L"ReplaceNodeByXMLTemplate";
			this->replaceNodeByXMLTemplateToolStripMenuItem->Click += gcnew System::EventHandler(this, &XMLForm::replaceNodeByXMLTemplateToolStripMenuItem_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->CurrentAttribute_treeView);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->AllowDrop = true;
			this->splitContainer1->Panel2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->splitContainer1->Panel2->Controls->Add(this->NodeValue_textBox);
			this->splitContainer1->Panel2->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &XMLForm::CurrentAttribute_treeView_DragDrop);
			this->splitContainer1->Panel2->Resize += gcnew System::EventHandler(this, &XMLForm::splitContainer1_Panel2_Resize);
			this->splitContainer1->Panel2->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &XMLForm::CurrentAttribute_treeView_DragEnter);
			this->splitContainer1->Panel2->SizeChanged += gcnew System::EventHandler(this, &XMLForm::splitContainer1_Panel2_SizeChanged);
			this->splitContainer1->Panel2MinSize = 0;
			this->splitContainer1->Size = System::Drawing::Size(456, 271);
			this->splitContainer1->SplitterDistance = 139;
			this->splitContainer1->TabIndex = 23;
			// 
			// CurrentAttribute_treeView
			// 
			this->CurrentAttribute_treeView->AllowDrop = true;
			this->CurrentAttribute_treeView->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->CurrentAttribute_treeView->ContextMenuStrip = this->TextTreeMenuStrip;
			this->CurrentAttribute_treeView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->CurrentAttribute_treeView->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CurrentAttribute_treeView->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->CurrentAttribute_treeView->Location = System::Drawing::Point(0, 0);
			this->CurrentAttribute_treeView->Name = L"CurrentAttribute_treeView";
			this->CurrentAttribute_treeView->Size = System::Drawing::Size(139, 271);
			this->CurrentAttribute_treeView->TabIndex = 1;
			this->CurrentAttribute_treeView->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &XMLForm::CurrentAttribute_treeView_MouseWheel);
			this->CurrentAttribute_treeView->AfterCollapse += gcnew System::Windows::Forms::TreeViewEventHandler(this, &XMLForm::CurrentAttribute_treeView_AfterSelect);
			this->CurrentAttribute_treeView->Resize += gcnew System::EventHandler(this, &XMLForm::CurrentAttribute_treeView_Resize);
			this->CurrentAttribute_treeView->DoubleClick += gcnew System::EventHandler(this, &XMLForm::CurrentAttribute_treeView_DoubleClick);
			this->CurrentAttribute_treeView->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &XMLForm::CurrentAttribute_treeView_DragDrop);
			this->CurrentAttribute_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &XMLForm::CurrentAttribute_treeView_AfterSelect);
			this->CurrentAttribute_treeView->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &XMLForm::CurrentAttribute_treeView_DragEnter);
			this->CurrentAttribute_treeView->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &XMLForm::TextEDitor_treeView_KeyDown);
			this->CurrentAttribute_treeView->AfterExpand += gcnew System::Windows::Forms::TreeViewEventHandler(this, &XMLForm::CurrentAttribute_treeView_AfterSelect);
			this->CurrentAttribute_treeView->Click += gcnew System::EventHandler(this, &XMLForm::CurrentAttribute_treeView_Click);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &XMLForm::timer1_Tick);
			// 
			// NodeName_textBox
			// 
			this->NodeName_textBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->NodeName_textBox->Location = System::Drawing::Point(335, 52);
			this->NodeName_textBox->Name = L"NodeName_textBox";
			this->NodeName_textBox->Size = System::Drawing::Size(714, 15);
			this->NodeName_textBox->TabIndex = 24;
			this->NodeName_textBox->Visible = false;
			this->NodeName_textBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &XMLForm::NodeValue_textBox_KeyUp);
			// 
			// AttributeUp_button
			// 
			this->AttributeUp_button->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->AttributeUp_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AttributeUp_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AttributeUp_button->Location = System::Drawing::Point(78, 120);
			this->AttributeUp_button->Margin = System::Windows::Forms::Padding(2);
			this->AttributeUp_button->Name = L"AttributeUp_button";
			this->AttributeUp_button->Size = System::Drawing::Size(18, 43);
			this->AttributeUp_button->TabIndex = 28;
			this->AttributeUp_button->Text = L"^\r\n^";
			this->AttributeUp_button->UseVisualStyleBackColor = false;
			this->AttributeUp_button->Click += gcnew System::EventHandler(this, &XMLForm::AttributeUp_button_Click);
			// 
			// AttributeDown_button
			// 
			this->AttributeDown_button->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->AttributeDown_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AttributeDown_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AttributeDown_button->Location = System::Drawing::Point(78, 167);
			this->AttributeDown_button->Margin = System::Windows::Forms::Padding(2);
			this->AttributeDown_button->Name = L"AttributeDown_button";
			this->AttributeDown_button->Size = System::Drawing::Size(18, 48);
			this->AttributeDown_button->TabIndex = 29;
			this->AttributeDown_button->Text = L"v\r\nv";
			this->AttributeDown_button->UseVisualStyleBackColor = false;
			this->AttributeDown_button->Click += gcnew System::EventHandler(this, &XMLForm::AttributeUp_button_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(333, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(33, 12);
			this->label1->TabIndex = 30;
			this->label1->Text = L"label1";
			this->label1->Visible = false;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->splitContainer2);
			this->groupBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->groupBox1->Location = System::Drawing::Point(0, 0);
			this->groupBox1->Margin = System::Windows::Forms::Padding(0);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(0);
			this->groupBox1->Size = System::Drawing::Size(557, 286);
			this->groupBox1->TabIndex = 31;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"XMLData";
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
			this->splitContainer2->Location = System::Drawing::Point(0, 15);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(0);
			this->splitContainer2->Name = L"splitContainer2";
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->splitContainer2->Panel1->Controls->Add(this->Panel2CollpaseChange_button);
			this->splitContainer2->Panel1->Controls->Add(this->label3);
			this->splitContainer2->Panel1->Controls->Add(this->AttributeUp_button);
			this->splitContainer2->Panel1->Controls->Add(this->AttributeDown_button);
			this->splitContainer2->Panel1->Controls->Add(this->AddComment_button);
			this->splitContainer2->Panel1->Controls->Add(this->Cut_button);
			this->splitContainer2->Panel1->Controls->Add(this->Paste_button);
			this->splitContainer2->Panel1->Controls->Add(this->Copy_button);
			this->splitContainer2->Panel1->Controls->Add(this->AddElement_button);
			this->splitContainer2->Panel1->Controls->Add(this->Del_button);
			this->splitContainer2->Panel1->Controls->Add(this->AddAttribute_button);
			this->splitContainer2->Panel1->Controls->Add(this->Replace_button);
			this->splitContainer2->Panel1->Controls->Add(this->Value_textBox);
			this->splitContainer2->Panel1->Controls->Add(this->Attribute_textBox);
			this->splitContainer2->Panel1->Controls->Add(this->label2);
			this->splitContainer2->Panel1MinSize = 14;
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->splitContainer1);
			this->splitContainer2->Size = System::Drawing::Size(557, 271);
			this->splitContainer2->SplitterDistance = 97;
			this->splitContainer2->TabIndex = 0;
			// 
			// Panel2CollpaseChange_button
			// 
			this->Panel2CollpaseChange_button->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->Panel2CollpaseChange_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->Panel2CollpaseChange_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Panel2CollpaseChange_button->Location = System::Drawing::Point(78, 2);
			this->Panel2CollpaseChange_button->Margin = System::Windows::Forms::Padding(2);
			this->Panel2CollpaseChange_button->Name = L"Panel2CollpaseChange_button";
			this->Panel2CollpaseChange_button->Size = System::Drawing::Size(19, 16);
			this->Panel2CollpaseChange_button->TabIndex = 62;
			this->Panel2CollpaseChange_button->Text = L"...";
			this->Panel2CollpaseChange_button->UseVisualStyleBackColor = false;
			this->Panel2CollpaseChange_button->Click += gcnew System::EventHandler(this, &XMLForm::Panel2CollpaseChange_button_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label3->Location = System::Drawing::Point(3, 42);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(32, 12);
			this->label3->TabIndex = 61;
			this->label3->Text = L"Value";
			// 
			// AddComment_button
			// 
			this->AddComment_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AddComment_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddComment_button->Location = System::Drawing::Point(4, 125);
			this->AddComment_button->Margin = System::Windows::Forms::Padding(2);
			this->AddComment_button->Name = L"AddComment_button";
			this->AddComment_button->Size = System::Drawing::Size(79, 22);
			this->AddComment_button->TabIndex = 60;
			this->AddComment_button->Text = L"AddComment";
			this->AddComment_button->UseVisualStyleBackColor = false;
			this->AddComment_button->Click += gcnew System::EventHandler(this, &XMLForm::AddElement_button_Click);
			// 
			// Cut_button
			// 
			this->Cut_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->Cut_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Cut_button->Location = System::Drawing::Point(3, 220);
			this->Cut_button->Margin = System::Windows::Forms::Padding(2);
			this->Cut_button->Name = L"Cut_button";
			this->Cut_button->Size = System::Drawing::Size(79, 22);
			this->Cut_button->TabIndex = 59;
			this->Cut_button->Text = L"Cut";
			this->Cut_button->UseVisualStyleBackColor = false;
			this->Cut_button->Click += gcnew System::EventHandler(this, &XMLForm::AddElement_button_Click);
			// 
			// Paste_button
			// 
			this->Paste_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->Paste_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Paste_button->Location = System::Drawing::Point(3, 243);
			this->Paste_button->Margin = System::Windows::Forms::Padding(2);
			this->Paste_button->Name = L"Paste_button";
			this->Paste_button->Size = System::Drawing::Size(79, 22);
			this->Paste_button->TabIndex = 58;
			this->Paste_button->Text = L"Paste";
			this->Paste_button->UseVisualStyleBackColor = false;
			this->Paste_button->Click += gcnew System::EventHandler(this, &XMLForm::AddElement_button_Click);
			// 
			// Copy_button
			// 
			this->Copy_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->Copy_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Copy_button->Location = System::Drawing::Point(3, 195);
			this->Copy_button->Margin = System::Windows::Forms::Padding(2);
			this->Copy_button->Name = L"Copy_button";
			this->Copy_button->Size = System::Drawing::Size(79, 22);
			this->Copy_button->TabIndex = 57;
			this->Copy_button->Text = L"Copy";
			this->Copy_button->UseVisualStyleBackColor = false;
			this->Copy_button->Click += gcnew System::EventHandler(this, &XMLForm::AddElement_button_Click);
			// 
			// AddElement_button
			// 
			this->AddElement_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AddElement_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddElement_button->Location = System::Drawing::Point(4, 78);
			this->AddElement_button->Margin = System::Windows::Forms::Padding(2);
			this->AddElement_button->Name = L"AddElement_button";
			this->AddElement_button->Size = System::Drawing::Size(79, 22);
			this->AddElement_button->TabIndex = 56;
			this->AddElement_button->Text = L"AddElement";
			this->AddElement_button->UseVisualStyleBackColor = false;
			this->AddElement_button->Click += gcnew System::EventHandler(this, &XMLForm::AddElement_button_Click);
			// 
			// Del_button
			// 
			this->Del_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->Del_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Del_button->Location = System::Drawing::Point(5, 171);
			this->Del_button->Margin = System::Windows::Forms::Padding(2);
			this->Del_button->Name = L"Del_button";
			this->Del_button->Size = System::Drawing::Size(78, 22);
			this->Del_button->TabIndex = 55;
			this->Del_button->Text = L"Delete";
			this->Del_button->UseVisualStyleBackColor = false;
			this->Del_button->Click += gcnew System::EventHandler(this, &XMLForm::AddElement_button_Click);
			// 
			// AddAttribute_button
			// 
			this->AddAttribute_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AddAttribute_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddAttribute_button->Location = System::Drawing::Point(5, 101);
			this->AddAttribute_button->Margin = System::Windows::Forms::Padding(2);
			this->AddAttribute_button->Name = L"AddAttribute_button";
			this->AddAttribute_button->Size = System::Drawing::Size(80, 22);
			this->AddAttribute_button->TabIndex = 54;
			this->AddAttribute_button->Text = L"AddAttribute";
			this->AddAttribute_button->UseVisualStyleBackColor = false;
			this->AddAttribute_button->Click += gcnew System::EventHandler(this, &XMLForm::AddElement_button_Click);
			// 
			// Replace_button
			// 
			this->Replace_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->Replace_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Replace_button->Location = System::Drawing::Point(3, 148);
			this->Replace_button->Margin = System::Windows::Forms::Padding(2);
			this->Replace_button->Name = L"Replace_button";
			this->Replace_button->Size = System::Drawing::Size(80, 22);
			this->Replace_button->TabIndex = 53;
			this->Replace_button->Text = L"Replace";
			this->Replace_button->UseVisualStyleBackColor = false;
			this->Replace_button->Click += gcnew System::EventHandler(this, &XMLForm::AddElement_button_Click);
			// 
			// Value_textBox
			// 
			this->Value_textBox->Location = System::Drawing::Point(2, 55);
			this->Value_textBox->Margin = System::Windows::Forms::Padding(2);
			this->Value_textBox->Name = L"Value_textBox";
			this->Value_textBox->Size = System::Drawing::Size(80, 22);
			this->Value_textBox->TabIndex = 52;
			// 
			// Attribute_textBox
			// 
			this->Attribute_textBox->Location = System::Drawing::Point(2, 18);
			this->Attribute_textBox->Margin = System::Windows::Forms::Padding(2);
			this->Attribute_textBox->Name = L"Attribute_textBox";
			this->Attribute_textBox->Size = System::Drawing::Size(80, 22);
			this->Attribute_textBox->TabIndex = 51;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label2->Location = System::Drawing::Point(3, 4);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(32, 12);
			this->label2->TabIndex = 50;
			this->label2->Text = L"Name";
			// 
			// XMLForm
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->NodeName_textBox);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"XMLForm";
			this->Size = System::Drawing::Size(557, 286);
			this->Load += gcnew System::EventHandler(this, &XMLForm::XMLForm_Load);
			this->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &XMLForm::CurrentAttribute_treeView_DragDrop);
			this->Resize += gcnew System::EventHandler(this, &XMLForm::XMLForm_Resize);
			this->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &XMLForm::CurrentAttribute_treeView_DragEnter);
			this->TextTreeMenuStrip->ResumeLayout(false);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->Panel2->PerformLayout();
			this->splitContainer1->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel1->PerformLayout();
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void XMLForm_Resize(System::Object^  sender, System::EventArgs^  e)
			 {
				 splitContainer1->Size = System::Drawing::Size(this->Size.Width-splitContainer1->Location.X-30,this->Size.Height-splitContainer1->Location.Y-40);
			 }
private: System::Void AllNode_listBox_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 NodeValue_textBox->Visible = false;
		 }
private: System::Void CurrentAttribute_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		 {
			 if( m_bDisableUpdateNodeInfo )
				 return;
			 if( CurrentAttribute_treeView->SelectedNode )
			 {
				 Attribute_textBox->Text = CurrentAttribute_treeView->SelectedNode->Text;
				 Value_textBox->Text = CurrentAttribute_treeView->SelectedNode->Name;
			 }
			 NodeValue_textBox->Visible = false;
			 NodeName_textBox->Visible = false;
			 int	l_iPosX = 10;
			 int	l_iPosY = 0;
			 GCFORM::TreeNode^l_pNode = CurrentAttribute_treeView->TopNode;
			 GCFORM::Label^l_pLabel;
			 int	l_iIndex = 0;
			 int	l_iHeight = CurrentAttribute_treeView->ItemHeight;
			 for( int i=0;i<100;++i )
			 {
				 l_pLabel = (GCFORM::Label^)m_AllLabelList[i];
				 l_pLabel->Visible = false;
			 }
			 m_pLabelAndNode->Clear();
			 while(l_pNode)
			 {
				 l_pLabel = (GCFORM::Label^)m_AllLabelList[l_iIndex];
				 m_pLabelAndNode[l_pLabel] = l_pNode;
				 l_pLabel->Text = l_pNode->Name;
				 //current selected node
				 if( l_pNode == CurrentAttribute_treeView->SelectedNode )
				 {
					 l_pLabel->BackColor = System::Drawing::Color::AntiqueWhite;
				 }
				 else
				 {
					 if( l_pLabel->Text->Length )
					 {
						 int	l_iR = 255;
						 int	l_iG = 255;
						 int	l_iB = 255;
						 if( l_pNode->Level%3 == 0 )
						 {
							l_iR -= l_pNode->Level*20;
							while( l_iR<0 )
								l_iR = 255+l_iR;
						 }
						 else
						 if( l_pNode->Level%3 == 1 )
						 {
							l_iG -= l_pNode->Level*20;
							while( l_iG<0 )
								l_iG = 255+l_iG;
						 }
						 else
						 if( l_pNode->Level%3 == 2 )
						 {
							l_iB -= l_pNode->Level*20;
							while( l_iB<0 )
								l_iB = 255+l_iB;
						 }
						//attribute
						l_pLabel->BackColor = System::Drawing::Color::FromArgb(l_iR,l_iG,l_iB);
					 }
					 else//element
					 {
						 l_pLabel->BackColor = System::Drawing::Color::White;
					 }
				 }
				 l_pLabel->Location = System::Drawing::Point( l_iPosX,l_iPosY+(l_iIndex*l_iHeight) );
				 l_pLabel->Visible = true;
				 if( !l_pNode->NextVisibleNode)
				 {
					return;
				 }
				 else
				 {
					l_pNode = l_pNode->NextVisibleNode;
				 }
				 l_iIndex++;
				 if( l_iIndex>=100 )
					 return;
			 }
		 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pTopNode != CurrentAttribute_treeView->TopNode )
			 {
				System::Windows::Forms::TreeViewEventArgs^  e2;
				CurrentAttribute_treeView_AfterSelect(sender,e2);
				m_pTopNode = CurrentAttribute_treeView->TopNode;
			 }
		 }
private: System::Void addGroupToolStripMenuItem_Click_1(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == addCommentToolStripMenuItem )
				AddComment();
			 if( sender == AddGroupToolStripMenuItem )
				AddElementIntoTreeNode();
			 else
			 if( sender == AddChildToolStripMenuItem )
				AddAttributeIntoTreeNode();
			 else
			 if( sender == DeleteGroupToolStripMenuItem )
				DeleteTreeNode();
			 else
			 if(  sender == addTextToolStripMenuItem)
				AddTextIntoTreeNode();
			 System::Windows::Forms::TreeViewEventArgs^  e2;
			 CurrentAttribute_treeView_AfterSelect(sender,e2);
		 }
private: System::Void TextEDitor_treeView_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 bool	l_bClick = false;
			 if( sender == this->CurrentAttribute_treeView )
			 {
				 if(e->Control)
				 {
					 if(e->KeyCode == GCFORM::Keys::A )
					 {
						AddAttributeIntoTreeNode();
						l_bClick = true;
					 }
					 else
					 if(e->KeyCode ==  GCFORM::Keys::E )
					 {
						AddElementIntoTreeNode();
						l_bClick = true;
					 }
				 }
				 else
				 if( e->KeyCode == GCFORM::Keys::F1 )
				 {
					ModifyTreeNodeData();
					l_bClick = true;
				}
				 else
				 if( e->KeyCode == GCFORM::Keys::F2 )
				 {
					 l_bClick = true;
					ReNameTreeNode();
				 }
				 else
				 if( e->KeyCode == GCFORM::Keys::Space )
				 {
					l_bClick = true;
					if(CurrentAttribute_treeView->SelectedNode)
					{
						if(CurrentAttribute_treeView->SelectedNode->Nodes->Count>0)
						{
							if(!CurrentAttribute_treeView->SelectedNode->Nodes[0]->IsVisible)
								CurrentAttribute_treeView->SelectedNode->Expand();
							else
								CurrentAttribute_treeView->SelectedNode->Collapse();
						}
					}
				 }
				 else
				 if(e->KeyCode == GCFORM::Keys::Delete )
				 {
					DeleteTreeNode();
					l_bClick = true;
				 }
			 }
			 if(e->Control)
			 {
				 if( e->KeyCode == GCFORM::Keys::F )//f
				 {
					NodeSearchForm ^l_p = gcnew NodeSearchForm(CurrentAttribute_treeView);
					l_p->TopMost  =true;
					l_p->Show();//
				 }
			 }
			 if( l_bClick == true )
			 {
				 if(m_TreeViewDeleteOrCreateFunction)
					 m_TreeViewDeleteOrCreateFunction(sender,e);
				 System::Windows::Forms::TreeViewEventArgs^  e2;
				 CurrentAttribute_treeView_AfterSelect(sender,e2);
			 }
		 }
private: System::Void CurrentAttribute_treeView_DoubleClick(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( !CurrentAttribute_treeView->SelectedNode )
				 return;
			 GCFORM::TreeNode^l_pNode = CurrentAttribute_treeView->TopNode;
			 int	l_iIndex = 0;
			 int	l_iHeight = CurrentAttribute_treeView->ItemHeight;
			 while(l_pNode)
			 {
				 if( l_pNode == CurrentAttribute_treeView->SelectedNode )
				 {
					 break;
				 }
				 l_pNode = l_pNode->NextVisibleNode;
				 l_iIndex++;
			 }
			 int	l_iIndentDis = 0;
			 GCFORM::TreeNode^l_pNodeParent = l_pNode;
			 while( l_pNodeParent )
			 {
				l_pNodeParent = l_pNodeParent->Parent;
				l_iIndentDis+=CurrentAttribute_treeView->Indent;
			 }
			 NodeName_textBox->BackColor = System::Drawing::Color::AntiqueWhite;
			 NodeName_textBox->Size = System::Drawing::Size(CurrentAttribute_treeView->Size.Width-l_iIndentDis-25,NodeName_textBox->Size.Height);
			 NodeName_textBox->Text = l_pNode->Text;
			 NodeName_textBox->Visible = true;
			 NodeName_textBox->Location = System::Drawing::Point(l_iIndentDis+splitContainer1->Location.X+3,splitContainer1->Location.Y+l_iHeight*l_iIndex);
			 NodeName_textBox->Focus();
		 }
private: System::Void CurrentAttribute_treeView_Resize(System::Object^  sender, System::EventArgs^  e)
		 {
			NodeName_textBox->Size = System::Drawing::Size(splitContainer1->Panel1->Size.Width-20,splitContainer1->Panel1->Size.Height);
		 }
private: System::Void splitContainer1_Panel2_Resize(System::Object^  sender, System::EventArgs^  e)
		 {
			 NodeValue_textBox->Size = System::Drawing::Size(splitContainer1->Panel2->Size.Width-20,splitContainer1->Panel1->Size.Height);
			 if( m_AllLabelList )
			 for( int i=0;i<m_AllLabelList->Count;++i )
			 {
				GCFORM::Label^l_pLabel = (GCFORM::Label^)m_AllLabelList[i];
				l_pLabel->Size = System::Drawing::Size(splitContainer1->Panel2->Size.Width,CurrentAttribute_treeView->ItemHeight);
			 }
		 }
private: System::Void CurrentAttribute_treeView_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			NodeName_textBox->Visible = false;
			NodeValue_textBox->Visible = false;
		 }
private: System::Void CurrentAttribute_treeView_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			System::Windows::Forms::MouseEventArgs^  e2;
			CurrentAttribute_treeView_MouseWheel(sender,e2);
		 }
private: System::Void NodeValue_textBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( sender == NodeName_textBox )
			 {
				 if( CurrentAttribute_treeView->SelectedNode )
				 {
					static bool	l_bWarning = false;
					for each( wchar_t l_wchar in NodeName_textBox->Text )
					{
						if( l_wchar>256 )
						{
							if( !l_bWarning )
							{
								l_bWarning = true;
								WARNING_MSG("only could using English  here");
							}
						}
					}
					CurrentAttribute_treeView->SelectedNode->Text = NodeName_textBox->Text;
					 if(CurrentAttribute_treeView->SelectedNode->Text->Equals(XML_COMMENT))
					 {
						 CurrentAttribute_treeView->SelectedNode->ForeColor = System::Drawing::Color::Green;
					 }
					 if(CurrentAttribute_treeView->SelectedNode->Text->Equals(XML_TEXT))
					 {
						CurrentAttribute_treeView->SelectedNode->ForeColor = System::Drawing::Color::RoyalBlue;
					 }
				 }
			 }
			 else
			 {
				 if( e->KeyCode == GCFORM::Keys::F2 )
				 {
					 System::String^l_pString = GetTextStringFrom(NodeValue_textBox->Text);
					 if( l_pString->Length )
					 {
						 m_pActivedLabel->Text = l_pString;
						 this->CurrentAttribute_treeView->SelectedNode->Name = l_pString;
					 }
				 }
				 else
				 {
					 m_pActivedLabel->Text = NodeValue_textBox->Text;
					 this->CurrentAttribute_treeView->SelectedNode->Name = NodeValue_textBox->Text;
				 }
			 }
		 }

private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 NodeValue_textBox->Visible = false;
			 GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)m_pLabelAndNode[(GCFORM::Label^)sender];
			 this->CurrentAttribute_treeView->SelectedNode = l_pNode;
		 }

private: System::Void label1_DoubleClick(System::Object^  sender, System::EventArgs^  e)
		 {
			 GCFORM::Label^l_pLabel = (GCFORM::Label^)sender;
			 GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)m_pLabelAndNode[(GCFORM::Label^)sender];
			 if( l_pNode->Nodes->Count != 0 )
				 return;
			 this->CurrentAttribute_treeView->SelectedNode = l_pNode;
			 m_pActivedLabel = l_pLabel;
			 NodeValue_textBox->Text = l_pLabel->Text;
			 NodeValue_textBox->Location = l_pLabel->Location;
			 NodeValue_textBox->Visible = true;
			 NodeValue_textBox->Focus();
		 }

private: System::Void splitContainer1_Panel2_SizeChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			System::Windows::Forms::MouseEventArgs^  e2;
			CurrentAttribute_treeView_MouseWheel(sender,e2);
		 }
private: System::Void CurrentAttribute_treeView_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			array<String^>^l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
			for each(String^l_str in l_strFileNames)
			{
				 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_str);
				 if( l_pNode )
				 {
					 this->Text += System::IO::Path::GetFileName(l_str);
					 CurrentAttribute_treeView->Nodes->Add(l_pNode);
				 }
			}
			 if( m_TreeViewDeleteOrCreateFunction )
				m_TreeViewDeleteOrCreateFunction(sender,e);
		 }
private: System::Void CurrentAttribute_treeView_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			 if ( e->Data->GetDataPresent( DataFormats::FileDrop ) )
			 {
				e->Effect = DragDropEffects::Copy;
			 }
			 else
				 e->Effect = DragDropEffects::None;
		 }

private: System::Void fontToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 System::Drawing::Font^l_pFont = DNCT::GetFontFromFontDialog();
			 if( l_pFont )
			 {
				NodeName_textBox->Font = l_pFont;
				CurrentAttribute_treeView->Font = l_pFont;
				NodeValue_textBox->Font = l_pFont;
				for( int i=0;i<100;++i )
				{
					GCFORM::Label^l_pLabel = (GCFORM::Label^)m_AllLabelList[i];
					l_pLabel->Font = l_pFont;
					l_pLabel->Size = System::Drawing::Size(l_pLabel->Width,CurrentAttribute_treeView->ItemHeight);
				}
				System::Windows::Forms::TreeViewEventArgs^  e2;
				CurrentAttribute_treeView_AfterSelect(sender,e2);
			 }
		 }

private: System::Void AttributeUp_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			GCFORM::TreeNode^l_pSelectedNode = CurrentAttribute_treeView->SelectedNode;
			if( !CurrentAttribute_treeView->SelectedNode || !l_pSelectedNode->Parent )
				return;
			int	l_iTargetIndex = -1;
			int	l_iIndex = l_pSelectedNode->Index;
			GCFORM::TreeNode^l_pTragetNode;
			if( AttributeUp_button == sender  )
			{
				if( l_pSelectedNode->PrevNode )
				{
					l_pTragetNode = l_pSelectedNode->PrevNode;
					l_iTargetIndex = l_pTragetNode->Index;
				}
				else
				{
					WARNING_MSG("Top");
					return;
				}
			}
			else
			if( AttributeDown_button == sender )
			{
				if( l_pSelectedNode->NextNode )
				{
					l_pTragetNode = l_pSelectedNode->NextNode;
					l_iTargetIndex = l_pTragetNode->Index;
				}	
				else
				{
					WARNING_MSG("buttom");
					return;
				}
			}
			GCFORM::TreeNode^l_pSelectedNodeParent = l_pSelectedNode->Parent;
			//first add dummy
			GCFORM::TreeNode^l_pEmtpyNode1 = gcnew GCFORM::TreeNode();
			GCFORM::TreeNode^l_pEmtpyNode2 = gcnew GCFORM::TreeNode();
			l_pSelectedNodeParent->Nodes->Insert(l_iIndex,l_pEmtpyNode1);
			l_pSelectedNodeParent->Nodes->Insert(l_iTargetIndex,l_pEmtpyNode2);
			//second remove original
			l_pSelectedNodeParent->Nodes->Remove(l_pTragetNode);
			l_pSelectedNodeParent->Nodes->Remove(l_pSelectedNode);
			//third insert to destination index
			l_pSelectedNodeParent->Nodes->Insert(l_iIndex,l_pTragetNode);
			l_pSelectedNodeParent->Nodes->Insert(l_iTargetIndex,l_pSelectedNode);
			//final remove dummy
			l_pSelectedNodeParent->Nodes->Remove(l_pEmtpyNode1);
			l_pSelectedNodeParent->Nodes->Remove(l_pEmtpyNode2);
			System::Windows::Forms::TreeViewEventArgs^  e2;
			CurrentAttribute_treeView_AfterSelect(sender,e2);
			CurrentAttribute_treeView->SelectedNode = l_pSelectedNode;
		 }
private: System::Void addFilesIntoAttributeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			GCFORM::TreeNode^l_pSelectedNode = CurrentAttribute_treeView->SelectedNode;
			if( !l_pSelectedNode )
				return;
			//is it element?
			if( l_pSelectedNode->Text->Length == 0 )
			{
				WARNING_MSG("please select element not attribute!!");
				return;
			}
			if( sender == addFilesIntoAttributeToolStripMenuItem )
			{
				WARNING_MSG("input attrubute name");
				System::String^l_pString = GetTextStringFrom("FileName");
				if( l_pString )
				{
					WARNING_MSG("select file ");
					array<String^>^l_FilesName = DNCT::OpenFileAndGetNames();
					if( !l_FilesName )
						return;
					int	l_iIndex = 0;
					for each(String^l_FileName in l_FilesName)
					{
						GCFORM::TreeNode^l_pNode = gcnew GCFORM::TreeNode();
						l_pNode->Text = l_pString+l_iIndex.ToString();
						l_pNode->Name = System::IO::Path::GetFileName(l_FileName);
						l_pSelectedNode->Nodes->Add(l_pNode);
						l_iIndex++;
					}
				}
			}
			else
			if( sender == addTextIntoAttributeToolStripMenuItem )
			{
				WARNING_MSG("input attrubute name");
				System::String^l_pAttrubute = GetTextStringFrom("FileName");
				if( !l_pAttrubute )
					return;
				WARNING_MSG("input File name,seprate by line change");
				System::String^l_pFilesName = GetTextStringFrom("");
				if( !l_pFilesName )
					return;
				array<Char>^l_Chars = {'\n'};
				array<String^>^l_strExtensionNameSplit = l_pFilesName->Split( l_Chars );
				int	l_iIndex = 1;
				for each(String^l_FileName in l_strExtensionNameSplit)
				{
					if( l_FileName->Length )
					{
						l_FileName = l_FileName->Substring(0,l_FileName->Length-DNCT::GetChanglineString()->Length+1);
						GCFORM::TreeNode^l_pNode = gcnew GCFORM::TreeNode();
						l_pNode->Text = l_pAttrubute+l_iIndex.ToString();
						l_pNode->Name = l_FileName;
						l_pSelectedNode->Nodes->Add(l_pNode);
						l_iIndex++;
					}
				}		
			}
			System::Windows::Forms::TreeViewEventArgs^  e2;
			CurrentAttribute_treeView_AfterSelect(sender,e2);
		 }
private: System::Void AddElement_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == AddAttribute_button )
			 {
				DoAddAttributeButton(); 
			 }
			 else
			 if( sender == Del_button )
			 {
				DoDeleteButton();
			 }
			 else
			 if( sender == Replace_button )
			 {
				DoReplaceButton();
			 }
			 else
			 if( sender == AddElement_button )
			 {
				DoAddElementButton(); 
			 }
			 else
			 if( sender == Copy_button || sender == copyToolStripMenuItem )
			 {
				DoCopyButton();
			 }
			 else
			 if( sender == Paste_button || sender == pasteToolStripMenuItem )
			 {
				DoPasteButton();
			 }
			 else
			 if( sender == Cut_button || sender == cutToolStripMenuItem )
			 {
				DoCutButton();
			 }
			 else
			 if( sender == AddComment_button )
			 {
				DoAddCommentButton();
			 }
			 if( m_TreeViewDeleteOrCreateFunction )
			 {
				m_TreeViewDeleteOrCreateFunction(sender,e);
			 }
		 }
private: System::Void XMLForm_Load(System::Object^  sender, System::EventArgs^  e)
		 {
			 Adjust();
		 }
private: System::Void expandToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(this->CurrentAttribute_treeView->SelectedNode)
				 this->CurrentAttribute_treeView->SelectedNode->ExpandAll();
		 }
		//void	ReplaceNodeData()
private: System::Void replaceNodeByXMLTemplateToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strFileName = DNCT::OpenFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
			 if( l_strFileName )
			 {
				 System::Xml::XmlDocument^l_pDocument = OpenXmlFile(l_strFileName);
				 if(l_pDocument->DocumentElement)
				 {
					 //l_pDocument->DocumentElement->ChildNodes
					 //l_pDocument->DocumentElement->Attributes->Count
				 }
			 }
		 }
private: System::Void Panel2CollpaseChange_button_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->splitContainer1->Panel2Collapsed = !this->splitContainer1->Panel2Collapsed;
		 }
};
}