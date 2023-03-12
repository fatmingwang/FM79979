#pragma once
#include "NodeFile.h"
#include "ValueInputForm.h"
#include "NodeSearchForm.h"
#include "xmlNodeParseCodeGenerator.h"
#include <string>
#include "BinaryFile.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../XML/StringCompress.h"
#include "EncryptStringConvert.h"
#include <string>
#include <codecvt> // for std::codecvt_utf8
#include <locale> 
	struct	sTextDataFile
	{
		DWORD	iNumCategory;
		struct	sCategory
		{
			DWORD	iNameSize;
			std::wstring	strName;
			DWORD	iNumText;
			struct	sText
			{
				DWORD	iNameSize;
				DWORD	iTextSize;
				std::wstring	strName;
				std::wstring	strText;
			};
			std::vector<sText>	TextList;
		};
		std::vector<sCategory>	CategoryVector;
	};
namespace XMLDataEditor 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 的摘要
	///
	/// 警告: 如果您變更這個類別的名稱，就必須變更與這個類別所依據之所有 .resx 檔案關聯的
	///          Managed 資源編譯器工具的 'Resource File Name' 屬性。
	///          否則，這些設計工具
	///          將無法與這個表單關聯的當地語系化資源
	///          正確互動。
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(String^e_strFileName)
		{
			InitializeComponent();

			 //EncryptStringConvert^l_pEncryptStringConvert = gcnew EncryptStringConvert();
			 //l_pEncryptStringConvert->Show();

			m_iBinaryHeaderID = 79979;
			m_bFileChanged = false;
			this->Text= "XMLEditor";
			m_iCurrentSelectedListBoxIndex = -1;
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_bDisableUpdateNodeInfo = false;
			m_pLabelAndNode = gcnew System::Collections::Hashtable();
			NodeValue_textBox->Visible = false;
			m_pNodeListForAppend = gcnew System::Collections::Hashtable();
			NodeFile^l_pNodeFile = gcnew NodeFile();
			l_pNodeFile->m_pHashTable = m_pNodeListForAppend;
			WCHAR	l_str[MAX_PATH];
			::GetCurrentDirectory(MAX_PATH,l_str);
			m_pStringForExeFilePath = DNCT::WcharToGcstring(l_str)+"\\";
			//l_pNodeFile->LoadFile(TO_GCSTRING("default.fun"),m_pStringForExeFilePath);

			NodeName_textBox->Size = System::Drawing::Size(splitContainer1->Panel1->Size.Width-20,splitContainer1->Panel1->Size.Height);
			NodeValue_textBox->Size = System::Drawing::Size(splitContainer1->Panel2->Size.Width-20,splitContainer1->Panel1->Size.Height);

			for each( System::Object^l_p in m_pNodeListForAppend->Keys )
				this->NodeList_comboBox->Items->Add(l_p->ToString());
			m_AllLabelList = gcnew System::Collections::ArrayList;
			for( int i=0;i<100;++i )
			{
				GCFORM::Label^l_pLabel = gcnew GCFORM::Label();
				l_pLabel->Name = i.ToString();
				l_pLabel->Margin = label1->Margin;
				l_pLabel->Enabled = true;
				l_pLabel->Click += gcnew System::EventHandler(this, &Form1::label1_Click);
				l_pLabel->DoubleClick += gcnew System::EventHandler(this, &Form1::label1_DoubleClick);
				l_pLabel->Size = System::Drawing::Size(320,MainNode_treeView->ItemHeight);
				m_AllLabelList->Add(l_pLabel);
				this->splitContainer1->Panel2->Controls->Add(l_pLabel);
			}
			if( System::IO::File::Exists(e_strFileName) )
			{
				 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(e_strFileName);
				 if( l_pNode )
				 {
					 this->Text = e_strFileName;
					 MainNode_treeView->Nodes->Add(l_pNode);
				 }
			}

				System::Drawing::Font^l_pFont = gcnew System::Drawing::Font("Arial", 12, FontStyle::Bold, GraphicsUnit::Point);
				NodeName_textBox->Font = l_pFont;
				MainNode_treeView->Font = l_pFont;
				NodeValue_textBox->Font = l_pFont;
				for( int i=0;i<100;++i )
				{
					GCFORM::Label^l_pLabel = (GCFORM::Label^)m_AllLabelList[i];
					l_pLabel->Font = l_pFont;
					l_pLabel->Size = System::Drawing::Size(l_pLabel->Width,MainNode_treeView->ItemHeight);
				}
				System::Windows::Forms::TreeViewEventArgs^  e2;
				System::Object^sender;
				MainNode_treeView_AfterSelect(sender,e2);
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  NodeListAddChild_button;
	protected: 
	private: System::Windows::Forms::Button^  NodeListAddGroup_button;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  NodeList_comboBox;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  NewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  SaveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  SaveAsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ExitToolStripMenuItem;
	private: System::Windows::Forms::TextBox^  NodeValue_textBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::TreeView^  MainNode_treeView;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Timer^  timer1;			 
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::ContextMenuStrip^  TextTreeMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  AddGroupToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  AddChildToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  DeleteGroupToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toNodeListToolStripMenuItem;
	private: System::Windows::Forms::TextBox^  NodeName_textBox;
	private: System::Windows::Forms::CheckBox^  Topmost_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  addCommentToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  Collapse_checkBox;
	private: System::Windows::Forms::ToolTip^  toolTip;
	private: System::Windows::Forms::CheckBox^  ShowValue_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  addTextToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  fontToolStripMenuItem;
	private: System::Windows::Forms::Button^  AttributeUp_button;
	private: System::Windows::Forms::Button^  AttributeDown_button;
	private: System::Windows::Forms::CheckBox^  CollpaseElement_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  addFilesIntoAttributeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addTextIntoAttributeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  xmlParserToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toBinaryFileToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  Unicode_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  toUTF32ToolStripMenuItem;
	private: System::Windows::Forms::Label^  NodeChildrenCount_label;
	private: System::Windows::Forms::ToolStripMenuItem^  Encode_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  Decode_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToBinaryToolStripMenuItem;
	private: System::Windows::Forms::ToolStripButton^  Save_toolStripButton;
	private: System::Windows::Forms::ToolStripMenuItem^  EncodeFolderToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  DecodeFolderToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  replaceNodeByTemplateFileToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  textConvertToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  forceIfFileIsInlegalToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^ toUTF32ByFolderToolStripMenuItem;
	   //my
	private: int			m_iCurrentSelectedListBoxIndex;	
	private: System::Void	ReNameTreeNode();
	private: System::Void	ModifyTreeNodeData();
	private: System::String^GetTextStringFrom(String^e_str);
	private: bool			SaveXmlFile(System::Object^  sender);
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
    //for sync
	private: bool	m_bDisableUpdateNodeInfo;
			 int	m_iBinaryHeaderID;
			bool	m_bFileChanged;
	GCFORM::TreeNode^OpenFileGetTreeNode(String^e_strFileName);
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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->NodeListAddChild_button = (gcnew System::Windows::Forms::Button());
			this->NodeListAddGroup_button = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->NodeList_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->NewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SaveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SaveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ExitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toUTF32ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToBinaryToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->fontToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->xmlParserToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toBinaryFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Encode_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Decode_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->EncodeFolderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->DecodeFolderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->textConvertToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->forceIfFileIsInlegalToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->NodeValue_textBox = (gcnew System::Windows::Forms::TextBox());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->MainNode_treeView = (gcnew System::Windows::Forms::TreeView());
			this->TextTreeMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->AddGroupToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AddChildToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addCommentToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addTextToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->DeleteGroupToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toNodeListToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addFilesIntoAttributeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addTextIntoAttributeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->replaceNodeByTemplateFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->NodeName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Topmost_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Collapse_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->toolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->ShowValue_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->AttributeUp_button = (gcnew System::Windows::Forms::Button());
			this->AttributeDown_button = (gcnew System::Windows::Forms::Button());
			this->CollpaseElement_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Unicode_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->NodeChildrenCount_label = (gcnew System::Windows::Forms::Label());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->Save_toolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->toUTF32ByFolderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->TextTreeMenuStrip->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// NodeListAddChild_button
			// 
			this->NodeListAddChild_button->Enabled = false;
			this->NodeListAddChild_button->Location = System::Drawing::Point(228, 104);
			this->NodeListAddChild_button->Margin = System::Windows::Forms::Padding(6, 6, 6, 6);
			this->NodeListAddChild_button->Name = L"NodeListAddChild_button";
			this->NodeListAddChild_button->Size = System::Drawing::Size(192, 38);
			this->NodeListAddChild_button->TabIndex = 20;
			this->NodeListAddChild_button->Text = L"AddToChild";
			this->NodeListAddChild_button->UseVisualStyleBackColor = true;
			this->NodeListAddChild_button->Click += gcnew System::EventHandler(this, &Form1::NodeListAddGroup_button_Click);
			// 
			// NodeListAddGroup_button
			// 
			this->NodeListAddGroup_button->Enabled = false;
			this->NodeListAddGroup_button->Location = System::Drawing::Point(28, 104);
			this->NodeListAddGroup_button->Margin = System::Windows::Forms::Padding(6, 6, 6, 6);
			this->NodeListAddGroup_button->Name = L"NodeListAddGroup_button";
			this->NodeListAddGroup_button->Size = System::Drawing::Size(188, 38);
			this->NodeListAddGroup_button->TabIndex = 19;
			this->NodeListAddGroup_button->Text = L"AddToGroup";
			this->NodeListAddGroup_button->UseVisualStyleBackColor = true;
			this->NodeListAddGroup_button->Click += gcnew System::EventHandler(this, &Form1::NodeListAddGroup_button_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(24, 56);
			this->label2->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(97, 25);
			this->label2->TabIndex = 18;
			this->label2->Text = L"NodeList";
			// 
			// NodeList_comboBox
			// 
			this->NodeList_comboBox->FormattingEnabled = true;
			this->NodeList_comboBox->Location = System::Drawing::Point(456, 104);
			this->NodeList_comboBox->Margin = System::Windows::Forms::Padding(6, 6, 6, 6);
			this->NodeList_comboBox->Name = L"NodeList_comboBox";
			this->NodeList_comboBox->Size = System::Drawing::Size(286, 33);
			this->NodeList_comboBox->TabIndex = 17;
			this->NodeList_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::NodeList_comboBox_SelectedIndexChanged);
			// 
			// menuStrip1
			// 
			this->menuStrip1->GripMargin = System::Windows::Forms::Padding(2, 2, 0, 2);
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(32, 32);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->fileToolStripMenuItem,
					this->fontToolStripMenuItem, this->xmlParserToolStripMenuItem, this->toBinaryFileToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(2104, 46);
			this->menuStrip1->TabIndex = 12;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {
				this->NewToolStripMenuItem,
					this->openToolStripMenuItem, this->SaveToolStripMenuItem, this->SaveAsToolStripMenuItem, this->ExitToolStripMenuItem, this->toUTF32ToolStripMenuItem,
					this->saveToBinaryToolStripMenuItem, this->toUTF32ByFolderToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(71, 38);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// NewToolStripMenuItem
			// 
			this->NewToolStripMenuItem->Name = L"NewToolStripMenuItem";
			this->NewToolStripMenuItem->Size = System::Drawing::Size(359, 44);
			this->NewToolStripMenuItem->Text = L"New";
			this->NewToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(359, 44);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
			// 
			// SaveToolStripMenuItem
			// 
			this->SaveToolStripMenuItem->Name = L"SaveToolStripMenuItem";
			this->SaveToolStripMenuItem->Size = System::Drawing::Size(359, 44);
			this->SaveToolStripMenuItem->Text = L"Save";
			this->SaveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
			// 
			// SaveAsToolStripMenuItem
			// 
			this->SaveAsToolStripMenuItem->Name = L"SaveAsToolStripMenuItem";
			this->SaveAsToolStripMenuItem->Size = System::Drawing::Size(359, 44);
			this->SaveAsToolStripMenuItem->Text = L"Save as";
			this->SaveAsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
			// 
			// ExitToolStripMenuItem
			// 
			this->ExitToolStripMenuItem->Name = L"ExitToolStripMenuItem";
			this->ExitToolStripMenuItem->Size = System::Drawing::Size(359, 44);
			this->ExitToolStripMenuItem->Text = L"Exit";
			this->ExitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
			// 
			// toUTF32ToolStripMenuItem
			// 
			this->toUTF32ToolStripMenuItem->Name = L"toUTF32ToolStripMenuItem";
			this->toUTF32ToolStripMenuItem->Size = System::Drawing::Size(359, 44);
			this->toUTF32ToolStripMenuItem->Text = L"ToUTF32";
			this->toUTF32ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::toUTF32ToolStripMenuItem_Click);
			// 
			// saveToBinaryToolStripMenuItem
			// 
			this->saveToBinaryToolStripMenuItem->Name = L"saveToBinaryToolStripMenuItem";
			this->saveToBinaryToolStripMenuItem->Size = System::Drawing::Size(359, 44);
			this->saveToBinaryToolStripMenuItem->Text = L"SaveToBinary";
			this->saveToBinaryToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToBinaryToolStripMenuItem_Click);
			// 
			// fontToolStripMenuItem
			// 
			this->fontToolStripMenuItem->Name = L"fontToolStripMenuItem";
			this->fontToolStripMenuItem->Size = System::Drawing::Size(82, 38);
			this->fontToolStripMenuItem->Text = L"Font";
			this->fontToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::fontToolStripMenuItem_Click);
			// 
			// xmlParserToolStripMenuItem
			// 
			this->xmlParserToolStripMenuItem->Name = L"xmlParserToolStripMenuItem";
			this->xmlParserToolStripMenuItem->Size = System::Drawing::Size(135, 38);
			this->xmlParserToolStripMenuItem->Text = L"xmlParser";
			this->xmlParserToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::xmlParserToolStripMenuItem_Click);
			// 
			// toBinaryFileToolStripMenuItem
			// 
			this->toBinaryFileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->Encode_ToolStripMenuItem,
					this->Decode_ToolStripMenuItem, this->EncodeFolderToolStripMenuItem, this->DecodeFolderToolStripMenuItem, this->textConvertToolStripMenuItem,
					this->forceIfFileIsInlegalToolStripMenuItem
			});
			this->toBinaryFileToolStripMenuItem->Name = L"toBinaryFileToolStripMenuItem";
			this->toBinaryFileToolStripMenuItem->Size = System::Drawing::Size(137, 38);
			this->toBinaryFileToolStripMenuItem->Text = L"BinaryFile";
			this->toBinaryFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::toBinaryFileToolStripMenuItem_Click);
			// 
			// Encode_ToolStripMenuItem
			// 
			this->Encode_ToolStripMenuItem->Name = L"Encode_ToolStripMenuItem";
			this->Encode_ToolStripMenuItem->Size = System::Drawing::Size(343, 44);
			this->Encode_ToolStripMenuItem->Text = L"Encode";
			this->Encode_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem1_Click);
			// 
			// Decode_ToolStripMenuItem
			// 
			this->Decode_ToolStripMenuItem->Name = L"Decode_ToolStripMenuItem";
			this->Decode_ToolStripMenuItem->Size = System::Drawing::Size(343, 44);
			this->Decode_ToolStripMenuItem->Text = L"Decode";
			this->Decode_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem1_Click);
			// 
			// EncodeFolderToolStripMenuItem
			// 
			this->EncodeFolderToolStripMenuItem->Name = L"EncodeFolderToolStripMenuItem";
			this->EncodeFolderToolStripMenuItem->Size = System::Drawing::Size(343, 44);
			this->EncodeFolderToolStripMenuItem->Text = L"EncodeFolder";
			this->EncodeFolderToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem1_Click);
			// 
			// DecodeFolderToolStripMenuItem
			// 
			this->DecodeFolderToolStripMenuItem->Name = L"DecodeFolderToolStripMenuItem";
			this->DecodeFolderToolStripMenuItem->Size = System::Drawing::Size(343, 44);
			this->DecodeFolderToolStripMenuItem->Text = L"DecodeFolder";
			this->DecodeFolderToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem1_Click);
			// 
			// textConvertToolStripMenuItem
			// 
			this->textConvertToolStripMenuItem->Name = L"textConvertToolStripMenuItem";
			this->textConvertToolStripMenuItem->Size = System::Drawing::Size(343, 44);
			this->textConvertToolStripMenuItem->Text = L"TextConvert";
			this->textConvertToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::textConvertToolStripMenuItem_Click);
			// 
			// forceIfFileIsInlegalToolStripMenuItem
			// 
			this->forceIfFileIsInlegalToolStripMenuItem->Checked = true;
			this->forceIfFileIsInlegalToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->forceIfFileIsInlegalToolStripMenuItem->Name = L"forceIfFileIsInlegalToolStripMenuItem";
			this->forceIfFileIsInlegalToolStripMenuItem->Size = System::Drawing::Size(343, 44);
			this->forceIfFileIsInlegalToolStripMenuItem->Text = L"ForceIfFileIsInlegal";
			this->forceIfFileIsInlegalToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::forceIfFileIsInlegalToolStripMenuItem_Click);
			// 
			// NodeValue_textBox
			// 
			this->NodeValue_textBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->NodeValue_textBox->Location = System::Drawing::Point(72, 167);
			this->NodeValue_textBox->Margin = System::Windows::Forms::Padding(6, 6, 6, 6);
			this->NodeValue_textBox->Name = L"NodeValue_textBox";
			this->NodeValue_textBox->Size = System::Drawing::Size(1428, 24);
			this->NodeValue_textBox->TabIndex = 22;
			this->NodeValue_textBox->Visible = false;
			this->NodeValue_textBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::NodeValue_textBox_KeyUp);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Location = System::Drawing::Point(36, 223);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(6, 6, 6, 6);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->MainNode_treeView);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->AllowDrop = true;
			this->splitContainer1->Panel2->BackColor = System::Drawing::Color::White;
			this->splitContainer1->Panel2->Controls->Add(this->NodeValue_textBox);
			this->splitContainer1->Panel2->SizeChanged += gcnew System::EventHandler(this, &Form1::splitContainer1_Panel2_SizeChanged);
			this->splitContainer1->Panel2->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::MainNode_treeView_DragDrop);
			this->splitContainer1->Panel2->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::MainNode_treeView_DragEnter);
			this->splitContainer1->Panel2->Resize += gcnew System::EventHandler(this, &Form1::splitContainer1_Panel2_Resize);
			this->splitContainer1->Panel2MinSize = 0;
			this->splitContainer1->Size = System::Drawing::Size(2018, 923);
			this->splitContainer1->SplitterDistance = 386;
			this->splitContainer1->SplitterWidth = 8;
			this->splitContainer1->TabIndex = 23;
			// 
			// MainNode_treeView
			// 
			this->MainNode_treeView->AllowDrop = true;
			this->MainNode_treeView->ContextMenuStrip = this->TextTreeMenuStrip;
			this->MainNode_treeView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->MainNode_treeView->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->MainNode_treeView->Location = System::Drawing::Point(0, 0);
			this->MainNode_treeView->Margin = System::Windows::Forms::Padding(6, 6, 6, 6);
			this->MainNode_treeView->Name = L"MainNode_treeView";
			this->MainNode_treeView->Size = System::Drawing::Size(386, 923);
			this->MainNode_treeView->TabIndex = 1;
			this->MainNode_treeView->AfterCollapse += gcnew System::Windows::Forms::TreeViewEventHandler(this, &Form1::MainNode_treeView_AfterSelect);
			this->MainNode_treeView->AfterExpand += gcnew System::Windows::Forms::TreeViewEventHandler(this, &Form1::MainNode_treeView_AfterSelect);
			this->MainNode_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &Form1::MainNode_treeView_AfterSelect);
			this->MainNode_treeView->Click += gcnew System::EventHandler(this, &Form1::MainNode_treeView_Click);
			this->MainNode_treeView->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::MainNode_treeView_DragDrop);
			this->MainNode_treeView->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::MainNode_treeView_DragEnter);
			this->MainNode_treeView->DoubleClick += gcnew System::EventHandler(this, &Form1::MainNode_treeView_DoubleClick);
			this->MainNode_treeView->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::TextEDitor_treeView_KeyDown);
			this->MainNode_treeView->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MainNode_treeView_MouseWheel);
			this->MainNode_treeView->Resize += gcnew System::EventHandler(this, &Form1::MainNode_treeView_Resize);
			// 
			// TextTreeMenuStrip
			// 
			this->TextTreeMenuStrip->ImageScalingSize = System::Drawing::Size(32, 32);
			this->TextTreeMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {
				this->AddGroupToolStripMenuItem,
					this->AddChildToolStripMenuItem, this->addCommentToolStripMenuItem, this->addTextToolStripMenuItem, this->DeleteGroupToolStripMenuItem,
					this->toNodeListToolStripMenuItem, this->addFilesIntoAttributeToolStripMenuItem, this->addTextIntoAttributeToolStripMenuItem,
					this->replaceNodeByTemplateFileToolStripMenuItem
			});
			this->TextTreeMenuStrip->Name = L"TextTreeMenuStrip";
			this->TextTreeMenuStrip->Size = System::Drawing::Size(391, 346);
			// 
			// AddGroupToolStripMenuItem
			// 
			this->AddGroupToolStripMenuItem->Name = L"AddGroupToolStripMenuItem";
			this->AddGroupToolStripMenuItem->Size = System::Drawing::Size(390, 38);
			this->AddGroupToolStripMenuItem->Text = L"Add Element";
			this->AddGroupToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::addGroupToolStripMenuItem_Click_1);
			// 
			// AddChildToolStripMenuItem
			// 
			this->AddChildToolStripMenuItem->Name = L"AddChildToolStripMenuItem";
			this->AddChildToolStripMenuItem->Size = System::Drawing::Size(390, 38);
			this->AddChildToolStripMenuItem->Text = L"AddAttribute";
			this->AddChildToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::addGroupToolStripMenuItem_Click_1);
			// 
			// addCommentToolStripMenuItem
			// 
			this->addCommentToolStripMenuItem->Name = L"addCommentToolStripMenuItem";
			this->addCommentToolStripMenuItem->Size = System::Drawing::Size(390, 38);
			this->addCommentToolStripMenuItem->Text = L"AddComment";
			this->addCommentToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::addGroupToolStripMenuItem_Click_1);
			// 
			// addTextToolStripMenuItem
			// 
			this->addTextToolStripMenuItem->Name = L"addTextToolStripMenuItem";
			this->addTextToolStripMenuItem->Size = System::Drawing::Size(390, 38);
			this->addTextToolStripMenuItem->Text = L"AddText";
			this->addTextToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::addGroupToolStripMenuItem_Click_1);
			// 
			// DeleteGroupToolStripMenuItem
			// 
			this->DeleteGroupToolStripMenuItem->Name = L"DeleteGroupToolStripMenuItem";
			this->DeleteGroupToolStripMenuItem->Size = System::Drawing::Size(390, 38);
			this->DeleteGroupToolStripMenuItem->Text = L"Delete";
			this->DeleteGroupToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::addGroupToolStripMenuItem_Click_1);
			// 
			// toNodeListToolStripMenuItem
			// 
			this->toNodeListToolStripMenuItem->Name = L"toNodeListToolStripMenuItem";
			this->toNodeListToolStripMenuItem->Size = System::Drawing::Size(390, 38);
			this->toNodeListToolStripMenuItem->Text = L"ToNodeList";
			this->toNodeListToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::toNodeListToolStripMenuItem_Click);
			// 
			// addFilesIntoAttributeToolStripMenuItem
			// 
			this->addFilesIntoAttributeToolStripMenuItem->Name = L"addFilesIntoAttributeToolStripMenuItem";
			this->addFilesIntoAttributeToolStripMenuItem->Size = System::Drawing::Size(390, 38);
			this->addFilesIntoAttributeToolStripMenuItem->Text = L"AddFilesIntoAttribute";
			this->addFilesIntoAttributeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::addFilesIntoAttributeToolStripMenuItem_Click);
			// 
			// addTextIntoAttributeToolStripMenuItem
			// 
			this->addTextIntoAttributeToolStripMenuItem->Name = L"addTextIntoAttributeToolStripMenuItem";
			this->addTextIntoAttributeToolStripMenuItem->Size = System::Drawing::Size(390, 38);
			this->addTextIntoAttributeToolStripMenuItem->Text = L"AddTextIntoAttribute";
			this->addTextIntoAttributeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::addFilesIntoAttributeToolStripMenuItem_Click);
			// 
			// replaceNodeByTemplateFileToolStripMenuItem
			// 
			this->replaceNodeByTemplateFileToolStripMenuItem->Name = L"replaceNodeByTemplateFileToolStripMenuItem";
			this->replaceNodeByTemplateFileToolStripMenuItem->Size = System::Drawing::Size(390, 38);
			this->replaceNodeByTemplateFileToolStripMenuItem->Text = L"ReplaceNodeByTemplateFile";
			this->replaceNodeByTemplateFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::replaceNodeByTemplateFileToolStripMenuItem_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(1922, 73);
			this->label1->Margin = System::Windows::Forms::Padding(0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(70, 25);
			this->label1->TabIndex = 0;
			this->label1->Text = L"label1";
			this->label1->Visible = false;
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click);
			this->label1->DoubleClick += gcnew System::EventHandler(this, &Form1::label1_DoubleClick);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// NodeName_textBox
			// 
			this->NodeName_textBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->NodeName_textBox->Location = System::Drawing::Point(508, 152);
			this->NodeName_textBox->Margin = System::Windows::Forms::Padding(6, 6, 6, 6);
			this->NodeName_textBox->Name = L"NodeName_textBox";
			this->NodeName_textBox->Size = System::Drawing::Size(1428, 24);
			this->NodeName_textBox->TabIndex = 24;
			this->NodeName_textBox->Visible = false;
			this->NodeName_textBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::NodeValue_textBox_KeyUp);
			// 
			// Topmost_checkBox
			// 
			this->Topmost_checkBox->AutoSize = true;
			this->Topmost_checkBox->Location = System::Drawing::Point(428, 65);
			this->Topmost_checkBox->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->Topmost_checkBox->Name = L"Topmost_checkBox";
			this->Topmost_checkBox->Size = System::Drawing::Size(127, 29);
			this->Topmost_checkBox->TabIndex = 25;
			this->Topmost_checkBox->Text = L"Topmost";
			this->Topmost_checkBox->UseVisualStyleBackColor = true;
			this->Topmost_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::Topmost_checkBox_CheckedChanged);
			// 
			// Collapse_checkBox
			// 
			this->Collapse_checkBox->AutoSize = true;
			this->Collapse_checkBox->Location = System::Drawing::Point(568, 67);
			this->Collapse_checkBox->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->Collapse_checkBox->Name = L"Collapse_checkBox";
			this->Collapse_checkBox->Size = System::Drawing::Size(152, 29);
			this->Collapse_checkBox->TabIndex = 26;
			this->Collapse_checkBox->Text = L"CollapseAll";
			this->Collapse_checkBox->UseVisualStyleBackColor = true;
			this->Collapse_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged);
			// 
			// ShowValue_checkBox
			// 
			this->ShowValue_checkBox->AutoSize = true;
			this->ShowValue_checkBox->Location = System::Drawing::Point(738, 67);
			this->ShowValue_checkBox->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->ShowValue_checkBox->Name = L"ShowValue_checkBox";
			this->ShowValue_checkBox->Size = System::Drawing::Size(152, 29);
			this->ShowValue_checkBox->TabIndex = 27;
			this->ShowValue_checkBox->Text = L"ShowValue";
			this->ShowValue_checkBox->UseVisualStyleBackColor = true;
			// 
			// AttributeUp_button
			// 
			this->AttributeUp_button->Location = System::Drawing::Point(4, 473);
			this->AttributeUp_button->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->AttributeUp_button->Name = L"AttributeUp_button";
			this->AttributeUp_button->Size = System::Drawing::Size(28, 77);
			this->AttributeUp_button->TabIndex = 28;
			this->AttributeUp_button->Text = L"^\r\n^";
			this->AttributeUp_button->UseVisualStyleBackColor = true;
			this->AttributeUp_button->Click += gcnew System::EventHandler(this, &Form1::AttributeUp_button_Click);
			// 
			// AttributeDown_button
			// 
			this->AttributeDown_button->Location = System::Drawing::Point(4, 604);
			this->AttributeDown_button->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->AttributeDown_button->Name = L"AttributeDown_button";
			this->AttributeDown_button->Size = System::Drawing::Size(28, 85);
			this->AttributeDown_button->TabIndex = 29;
			this->AttributeDown_button->Text = L"v\r\nv";
			this->AttributeDown_button->UseVisualStyleBackColor = true;
			this->AttributeDown_button->Click += gcnew System::EventHandler(this, &Form1::AttributeUp_button_Click);
			// 
			// CollpaseElement_checkBox
			// 
			this->CollpaseElement_checkBox->AutoSize = true;
			this->CollpaseElement_checkBox->Location = System::Drawing::Point(36, 183);
			this->CollpaseElement_checkBox->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->CollpaseElement_checkBox->Name = L"CollpaseElement_checkBox";
			this->CollpaseElement_checkBox->Size = System::Drawing::Size(206, 29);
			this->CollpaseElement_checkBox->TabIndex = 30;
			this->CollpaseElement_checkBox->Text = L"CollapseElement";
			this->CollpaseElement_checkBox->UseVisualStyleBackColor = true;
			this->CollpaseElement_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::CollpaseElement_checkBox_CheckedChanged);
			// 
			// Unicode_checkBox
			// 
			this->Unicode_checkBox->AutoSize = true;
			this->Unicode_checkBox->Checked = true;
			this->Unicode_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->Unicode_checkBox->Location = System::Drawing::Point(296, 152);
			this->Unicode_checkBox->Margin = System::Windows::Forms::Padding(6, 6, 6, 6);
			this->Unicode_checkBox->Name = L"Unicode_checkBox";
			this->Unicode_checkBox->Size = System::Drawing::Size(123, 29);
			this->Unicode_checkBox->TabIndex = 31;
			this->Unicode_checkBox->Text = L"Unicode";
			this->Unicode_checkBox->UseVisualStyleBackColor = true;
			// 
			// NodeChildrenCount_label
			// 
			this->NodeChildrenCount_label->AutoSize = true;
			this->NodeChildrenCount_label->Location = System::Drawing::Point(32, 154);
			this->NodeChildrenCount_label->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->NodeChildrenCount_label->Name = L"NodeChildrenCount_label";
			this->NodeChildrenCount_label->Size = System::Drawing::Size(206, 25);
			this->NodeChildrenCount_label->TabIndex = 32;
			this->NodeChildrenCount_label->Text = L"NodeChildren Count";
			// 
			// toolStrip1
			// 
			this->toolStrip1->ImageScalingSize = System::Drawing::Size(32, 32);
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->Save_toolStripButton });
			this->toolStrip1->Location = System::Drawing::Point(0, 46);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Padding = System::Windows::Forms::Padding(0, 0, 4, 0);
			this->toolStrip1->Size = System::Drawing::Size(2104, 42);
			this->toolStrip1->TabIndex = 33;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// Save_toolStripButton
			// 
			this->Save_toolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->Save_toolStripButton->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Save_toolStripButton.Image")));
			this->Save_toolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->Save_toolStripButton->Name = L"Save_toolStripButton";
			this->Save_toolStripButton->Size = System::Drawing::Size(46, 36);
			this->Save_toolStripButton->Text = L"toolStripButton1";
			this->Save_toolStripButton->Click += gcnew System::EventHandler(this, &Form1::Save_toolStripButton_Click);
			// 
			// toUTF32ByFolderToolStripMenuItem
			// 
			this->toUTF32ByFolderToolStripMenuItem->Name = L"toUTF32ByFolderToolStripMenuItem";
			this->toUTF32ByFolderToolStripMenuItem->Size = System::Drawing::Size(359, 44);
			this->toUTF32ByFolderToolStripMenuItem->Text = L"ToUTF32ByFolder";
			this->toUTF32ByFolderToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::toUTF32ByFolderToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(12, 25);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(2104, 1194);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->NodeChildrenCount_label);
			this->Controls->Add(this->Unicode_checkBox);
			this->Controls->Add(this->CollpaseElement_checkBox);
			this->Controls->Add(this->AttributeDown_button);
			this->Controls->Add(this->AttributeUp_button);
			this->Controls->Add(this->ShowValue_checkBox);
			this->Controls->Add(this->Collapse_checkBox);
			this->Controls->Add(this->Topmost_checkBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->NodeName_textBox);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->NodeListAddChild_button);
			this->Controls->Add(this->NodeListAddGroup_button);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->NodeList_comboBox);
			this->KeyPreview = true;
			this->Margin = System::Windows::Forms::Padding(6, 6, 6, 6);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::MainNode_treeView_DragDrop);
			this->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::MainNode_treeView_DragEnter);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyUp);
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->Panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->TextTreeMenuStrip->ResumeLayout(false);
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e)
		 {
			 splitContainer1->Size = System::Drawing::Size(this->Size.Width-splitContainer1->Location.X-30,this->Size.Height-splitContainer1->Location.Y-40);
		 }
private: System::Void AllNode_listBox_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 NodeValue_textBox->Visible = false;
		 }
private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == NewToolStripMenuItem )
			 {
				this->MainNode_treeView->Nodes->Clear();
				m_pStringForFileName = "";
			 }
			 else
			 if(sender == openToolStripMenuItem)
			 {
				 if(m_bFileChanged)
				 {
					 WARING_YES_NO_TO_NO("file not save,still open new file!?")
					 {
						 return;
					 }
				 }
				 MainNode_treeView->Nodes->Clear();
				 String^l_strFileName = DNCT::OpenFileAndGetName("xml Files(*.dae;*.xml;*.fat)|*.dae;*.xml;*.fat|All files (*.*)|*.*");
				 GCFORM::TreeNode^l_pNode = OpenFileGetTreeNode(l_strFileName);
				 this->Text = l_strFileName;
				 MainNode_treeView->Nodes->Add(l_pNode);
			 }
			 else
			 if( sender == SaveToolStripMenuItem ||  sender == SaveAsToolStripMenuItem )
			 {
				 if(SaveXmlFile(sender))
				 {
					 //NodeFile^ l_pFile = gcnew NodeFile();
					 //l_pFile->m_pHashTable = this->m_pNodeListForAppend;
					 //l_pFile->WriteFile(m_pStringForExeFilePath);
				 }
			 }
			 else
			 if(sender == ExitToolStripMenuItem)
			 {
				this->Close();
			 }
		 }
private: System::Void MainNode_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		 {
			 if( MainNode_treeView->Nodes->Count >0 )
				m_bFileChanged = true;
			 NodeChildrenCount_label->Text = "";
			 if( MainNode_treeView->SelectedNode )
			 {
				 if( ShowValue_checkBox->Checked )
					toolTip->SetToolTip(MainNode_treeView,this->MainNode_treeView->SelectedNode->Name);
				 int	l_iNumAttributes = 0;
				 int	l_iNumElement = 0;
				 for each ( GCFORM::TreeNode^l_pNode in MainNode_treeView->SelectedNode->Nodes )
				 {
					 if( l_pNode->Name->Length == 0 )
						 ++l_iNumElement;
					 else
					 {
						 if( !l_pNode->Name->Equals(XML_COMMENT) && !l_pNode->Name->Equals(XML_TEXT)  )
							++l_iNumAttributes;
					 }
				 }
				 NodeChildrenCount_label->Text = MainNode_treeView->SelectedNode->Text+DNCT::GetChanglineString()
					 +"ElementCount:"+l_iNumElement.ToString()+DNCT::GetChanglineString()
					 +"Attribute:"+l_iNumAttributes.ToString();
			 }
			 if( m_bDisableUpdateNodeInfo )
				 return;
			 NodeValue_textBox->Visible = false;
			 NodeName_textBox->Visible = false;
			 int	l_iPosX = 10;
			 int	l_iPosY = 0;
			 GCFORM::TreeNode^l_pNode = MainNode_treeView->TopNode;
			 GCFORM::Label^l_pLabel;
			 int	l_iIndex = 0;
			 int	l_iHeight = MainNode_treeView->ItemHeight;
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
				 if( l_pNode == MainNode_treeView->SelectedNode )
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
			 if( m_pTopNode != MainNode_treeView->TopNode )
			 {
				System::Windows::Forms::TreeViewEventArgs^  e2;
				MainNode_treeView_AfterSelect(sender,e2);
				m_pTopNode = MainNode_treeView->TopNode;
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
			 MainNode_treeView_AfterSelect(sender,e2);
		 }
private: System::Void toNodeListToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(this->MainNode_treeView->SelectedNode)
			 {
				 bool	l_bSame = false;
				 System::String^l_pSelectNode = this->MainNode_treeView->SelectedNode->Text->ToString();
				 for(int i=0;i<this->NodeList_comboBox->Items->Count;i++)
				 {
					 System::String^l_pStringForItem = NodeList_comboBox->Items[i]->ToString();
					 if( l_pStringForItem->Equals(l_pSelectNode) )
					 {
						WARING_YES_NO_TO_YES("Would you like to replace?")
						{
							DNCT::ReplaceValuInHashTable(l_pStringForItem,MainNode_treeView->SelectedNode->Clone(),m_pNodeListForAppend);							
							return;
						}
						l_bSame = true;
					 }
				 }
				 if(!l_bSame)
				 {
					this->m_pNodeListForAppend->Add(l_pSelectNode,this->MainNode_treeView->SelectedNode->Clone());
					NodeList_comboBox->Items->Add(l_pSelectNode);
				 }
			 }
			 else
				 WARNING_MSG("please select one node");
		 }
private: System::Void NodeList_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(NodeList_comboBox->SelectedIndex!=-1)
			 {
				 NodeListAddGroup_button->Enabled = true;
				 NodeListAddChild_button->Enabled = true;
			 }
			 else
			 {
				NodeListAddGroup_button->Enabled = false;
				NodeListAddChild_button->Enabled = false;
			 }
		 }
private: System::Void NodeListAddGroup_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 GCFORM::TreeNodeCollection^ l_pNodeCollectionForAddNode;
			 if( sender == NodeListAddChild_button )
			 {
				 if(MainNode_treeView->SelectedNode->Name->Length)
				 {
					 WARNING_MSG("attribute can't add child");
					 return;
				 }
				 l_pNodeCollectionForAddNode = this->MainNode_treeView->SelectedNode->Nodes;
			 }
			 else
				 l_pNodeCollectionForAddNode = this->MainNode_treeView->Nodes;

			 GCFORM::TreeNode^ l_p = (GCFORM::TreeNode^)((GCFORM::TreeNode^)(this->m_pNodeListForAppend[NodeList_comboBox->SelectedItem->ToString()]))->Clone();
			 //old node from file
			 if( l_p->Text->ToString()->Equals("root") )
			 {
				 for each(GCFORM::TreeNode^l_p2 in l_p->Nodes)
					l_pNodeCollectionForAddNode->Add( (GCFORM::TreeNode^)l_p2->Clone() );
			 }
			 else//new node from treeview
				 l_pNodeCollectionForAddNode->Add( (GCFORM::TreeNode^)l_p->Clone() );
		 }
private: System::Void TextEDitor_treeView_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 bool	l_bClick = false;
			 if( sender == this->MainNode_treeView )
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
					if(MainNode_treeView->SelectedNode)
					{
						if(MainNode_treeView->SelectedNode->Nodes->Count>0)
						{
							if(!MainNode_treeView->SelectedNode->Nodes[0]->IsVisible)
								MainNode_treeView->SelectedNode->Expand();
							else
								MainNode_treeView->SelectedNode->Collapse();
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
					NodeSearchForm ^l_p = gcnew NodeSearchForm(MainNode_treeView);
					l_p->TopMost  =true;
					l_p->Show();//
				 }
			 }
			 if( l_bClick == true )
			 {
				 System::Windows::Forms::TreeViewEventArgs^  e2;
				 MainNode_treeView_AfterSelect(sender,e2);
			 }
		 }

private: System::Void MainNode_treeView_DoubleClick(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( !MainNode_treeView->SelectedNode )
				 return;
			 GCFORM::TreeNode^l_pNode = MainNode_treeView->TopNode;
			 int	l_iIndex = 0;
			 int	l_iHeight = MainNode_treeView->ItemHeight;
			 while(l_pNode)
			 {
				 if( l_pNode == MainNode_treeView->SelectedNode )
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
				l_iIndentDis+=MainNode_treeView->Indent;
			 }
			 NodeName_textBox->BackColor = System::Drawing::Color::AntiqueWhite;
			 NodeName_textBox->Size = System::Drawing::Size(MainNode_treeView->Size.Width-l_iIndentDis-25,NodeName_textBox->Size.Height);
			 NodeName_textBox->Text = l_pNode->Text;
			 NodeName_textBox->Visible = true;
			 NodeName_textBox->Location = System::Drawing::Point(l_iIndentDis+splitContainer1->Location.X+3,splitContainer1->Location.Y+l_iHeight*l_iIndex);
			 NodeName_textBox->Focus();
		 }

private: System::Void MainNode_treeView_Resize(System::Object^  sender, System::EventArgs^  e)
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
				l_pLabel->Size = System::Drawing::Size(splitContainer1->Panel2->Size.Width,MainNode_treeView->ItemHeight);
			 }
		 }

private: System::Void MainNode_treeView_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			NodeName_textBox->Visible = false;
			NodeValue_textBox->Visible = false;
		 }

private: System::Void MainNode_treeView_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			System::Windows::Forms::MouseEventArgs^  e2;
			MainNode_treeView_MouseWheel(sender,e2);
		 }

private: System::Void NodeValue_textBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( sender == NodeName_textBox )
			 {
				 if( MainNode_treeView->SelectedNode )
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
					MainNode_treeView->SelectedNode->Text = NodeName_textBox->Text;
					 if(MainNode_treeView->SelectedNode->Text->Equals(XML_COMMENT))
					 {
						 MainNode_treeView->SelectedNode->ForeColor = System::Drawing::Color::Green;
					 }
					 if(MainNode_treeView->SelectedNode->Text->Equals(XML_TEXT))
					 {
						MainNode_treeView->SelectedNode->ForeColor = System::Drawing::Color::RoyalBlue;
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
						 this->MainNode_treeView->SelectedNode->Name = l_pString;
					 }
				 }
				 else
				 {
					 m_pActivedLabel->Text = NodeValue_textBox->Text;
					 this->MainNode_treeView->SelectedNode->Name = NodeValue_textBox->Text;
				 }
			 }
		 }

private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 NodeValue_textBox->Visible = false;
			 GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)m_pLabelAndNode[(GCFORM::Label^)sender];
			 this->MainNode_treeView->SelectedNode = l_pNode;
		 }

private: System::Void label1_DoubleClick(System::Object^  sender, System::EventArgs^  e)
		 {
			 GCFORM::Label^l_pLabel = (GCFORM::Label^)sender;
			 GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)m_pLabelAndNode[(GCFORM::Label^)sender];
			 if( l_pNode->Nodes->Count != 0 )
				 return;
			 this->MainNode_treeView->SelectedNode = l_pNode;
			 m_pActivedLabel = l_pLabel;
			 NodeValue_textBox->Text = l_pLabel->Text;
			 NodeValue_textBox->Location = l_pLabel->Location;
			 NodeValue_textBox->Visible = true;
			 NodeValue_textBox->Focus();
		 }

private: System::Void Topmost_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->TopMost = this->Topmost_checkBox->Checked;
		 }

private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_bDisableUpdateNodeInfo = true;
			 Collapse_checkBox->Checked?Collapse_checkBox->Text = "CollapseAll":Collapse_checkBox->Text = "ExpandAll";
			 if(Collapse_checkBox->Checked)
				this->MainNode_treeView->CollapseAll();
			 else
				 MainNode_treeView->ExpandAll();
			 m_bDisableUpdateNodeInfo = false;
			 System::Windows::Forms::TreeViewEventArgs^  e2;
			 MainNode_treeView_AfterSelect(sender,e2);
		 }

private: System::Void splitContainer1_Panel2_SizeChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			System::Windows::Forms::MouseEventArgs^  e2;
			MainNode_treeView_MouseWheel(sender,e2);
		 }

private: System::Void MainNode_treeView_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			this->Text = "";
			cli::array<String^>^l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
			if( l_strFileNames->Length == 1 )
			{
				 GCFORM::TreeNode^l_pNode = OpenFileGetTreeNode(l_strFileNames[0]);
				 if( l_pNode )
				 {
					 this->Text = l_strFileNames[0];
					 MainNode_treeView->Nodes->Add(l_pNode);
				 }
			}
			else
			{
				for each(String^l_str in l_strFileNames)
				{
					 GCFORM::TreeNode^l_pNode = OpenFileGetTreeNode(l_str);
					 if( l_pNode )
					 {
						 this->Text += System::IO::Path::GetFileName(l_str);
						 MainNode_treeView->Nodes->Add(l_pNode);
					 }
					 this->Text += "  ;  ";
				}
			}
		 }
private: System::Void MainNode_treeView_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
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
				MainNode_treeView->Font = l_pFont;
				NodeValue_textBox->Font = l_pFont;
				for( int i=0;i<100;++i )
				{
					GCFORM::Label^l_pLabel = (GCFORM::Label^)m_AllLabelList[i];
					l_pLabel->Font = l_pFont;
					l_pLabel->Size = System::Drawing::Size(l_pLabel->Width,MainNode_treeView->ItemHeight);
				}
				System::Windows::Forms::TreeViewEventArgs^  e2;
				MainNode_treeView_AfterSelect(sender,e2);
			 }
		 }

private: System::Void AttributeUp_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			GCFORM::TreeNode^l_pSelectedNode = MainNode_treeView->SelectedNode;
			if( !MainNode_treeView->SelectedNode || !l_pSelectedNode->Parent )
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
			MainNode_treeView_AfterSelect(sender,e2);
			MainNode_treeView->SelectedNode = l_pSelectedNode;
		 }
private: System::Void CollpaseElement_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			GCFORM::TreeNode^l_pSelectedNode = MainNode_treeView->SelectedNode;
			if( !MainNode_treeView->SelectedNode )
				return;
			if( CollpaseElement_checkBox->Checked )
			{
				l_pSelectedNode->ExpandAll();
			}
			else
				l_pSelectedNode->Collapse();
		 }
private: System::Void addFilesIntoAttributeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			GCFORM::TreeNode^l_pSelectedNode = MainNode_treeView->SelectedNode;
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
					cli::array<String^>^l_FilesName = DNCT::OpenFileAndGetNames();
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
				cli::array<Char>^l_Chars = {'\n'};
				cli::array<String^>^l_strExtensionNameSplit = l_pFilesName->Split( l_Chars );
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
			MainNode_treeView_AfterSelect(sender,e2);
		 }
private: System::Void xmlParserToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			xmlNodeParseCodeGenerator^l_pxmlNodeParseCodeGenerator = gcnew xmlNodeParseCodeGenerator("");
			l_pxmlNodeParseCodeGenerator->Show();
		 }
private: System::Void toBinaryFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			return;
			//sTextDataFile	l_TextDataFile;
			//bool	l_bFormatIsRight = false;
			//if(MainNode_treeView->Nodes->Count)
			//{
			//	if(MainNode_treeView->Nodes[0]->Nodes->Count)
			//	{
			//		l_bFormatIsRight = true;
			//	}
			//}
			//if( !l_bFormatIsRight )
			//{
			//	WARNING_MSG("format is not correct");
			//	return;
			//}
			//l_TextDataFile.iNumCategory = MainNode_treeView->Nodes[0]->Nodes->Count;
			//if( MainNode_treeView->Nodes->Count != 1 )
			//{
			//	WARNING_MSG("format is not correct");
			//	return;			
			//}
			////category
			//for each(GCFORM::TreeNode^l_pNode in MainNode_treeView->Nodes[0]->Nodes)
			//{
			//	if( l_pNode->Nodes->Count <1 )
			//	{
			//		WARNING_MSG("format is not correct");
			//		return;
			//	}
			//	//get category Name
			//	GCFORM::TreeNode^l_pCategoryNameNode = l_pNode->Nodes[0];
			//	sTextDataFile::sCategory	l_Category;
			//	l_Category.iNameSize = l_pCategoryNameNode->Name->Length;
			//	l_Category.iNumText = l_pNode->Nodes->Count-1;//this one is name
			//	l_Category.strName = DNCT::GcStringToWchar(l_pCategoryNameNode->Name);
			//	//text,skip first child becaue it's child
			//	for( int i=1;i<l_pNode->Nodes->Count;++i )
			//	{
			//		GCFORM::TreeNode^l_pTextNode = l_pNode->Nodes[i];
			//		if( l_pTextNode->Nodes->Count != 2 )
			//		{
			//			WARNING_MSG("format is not correct");
			//			return;
			//		}
			//		//text's attribute,name and text
			//		GCFORM::TreeNode^l_pTextName;
			//		GCFORM::TreeNode^l_pText;
			//		if(l_pTextNode->Nodes[0]->Text->Equals("Name"))
			//		{
			//			l_pTextName = l_pTextNode->Nodes[0];
			//			l_pText = l_pTextNode->Nodes[1];
			//		}
			//		if(l_pTextNode->Nodes[0]->Text->Equals("Text"))
			//		{
			//			l_pTextName = l_pTextNode->Nodes[1];
			//			l_pText = l_pTextNode->Nodes[0];
			//		}
			//		if( !l_pTextName )
			//		{
			//			WARNING_MSG("format is not correct");
			//			WARNING_MSG(l_pTextNode->Nodes[0]->Text);
			//			return;						
			//		}
			//		sTextDataFile::sCategory::sText	l_Text;
			//		l_Text.iTextSize = l_pText->Name->Length;
			//		l_Text.iNameSize = l_pTextName->Name->Length;
			//		l_Text.strText = DNCT::GcStringToWchar(l_pText->Name);
			//		l_Text.strName = DNCT::GcStringToWchar(l_pTextName->Name);
			//		l_Category.TextList.push_back(l_Text);
			//	}
			//	l_TextDataFile.CategoryVector.push_back(l_Category);
			//}

			//String^l_strFileName = DNCT::SaveFileAndGetName();
			//if( l_strFileName )
			//{
			//	cBinaryFile	l_BinaryFile;
			//	if(l_BinaryFile.Writefile(DNCT::GcStringToChar(l_strFileName)))
			//	{
			//		WCHAR	*l_strMagicIDDat = {L"幹你娘老機掰"};
			//		l_BinaryFile.WriteToFile(l_strMagicIDDat);
			//		l_BinaryFile.WriteToFile(l_TextDataFile.iNumCategory);
			//		for(DWORD i=0;i<l_TextDataFile.iNumCategory;++i)
			//		{
			//			sTextDataFile::sCategory*l_pCategory = &l_TextDataFile.CategoryVector[i];
			//			l_BinaryFile.WriteToFile(l_pCategory->iNameSize);
			//			l_BinaryFile.WriteToFile(l_pCategory->strName.c_str());
			//			l_BinaryFile.WriteToFile(l_pCategory->iNumText);
			//			for(DWORD j=0;j<l_pCategory->iNumText;++j)
			//			{
			//				sTextDataFile::sCategory::sText*l_pText = &l_pCategory->TextList[j];
			//				l_BinaryFile.WriteToFile(l_pText->iNameSize);
			//				l_BinaryFile.WriteToFile(l_pText->iTextSize);
			//				l_BinaryFile.WriteToFile(l_pText->strName.c_str());
			//				l_BinaryFile.WriteToFile(l_pText->strText.c_str());
			//			}
			//		}
			//		l_BinaryFile.CloseFile();
			//	}
			//	else
			//	{
			//		WARNING_MSG("save file error");
			//	}
			//}
		 }
private: System::Void Form1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if(e->Control)
			 {
				 //107 for Add
				 //109 for minus
				int	l_iValue  = e->KeyValue;
				if( l_iValue == 109 || l_iValue == 107 )
				{
					int	l_iNewSize = l_iValue==109?-2:2;
					if( NodeName_textBox->Font->Size < 8 )
						l_iNewSize = 0;
					if( NodeName_textBox->Font->Size >= 64 )
						l_iNewSize = 0;

					System::Drawing::Font^l_pFont = gcnew System::Drawing::Font(NodeName_textBox->Font->Name,NodeName_textBox->Font->Size+l_iNewSize, NodeName_textBox->Font->Style,GraphicsUnit::Point);
					NodeName_textBox->Font = l_pFont;
					MainNode_treeView->Font = l_pFont;
					NodeValue_textBox->Font = l_pFont;
					for( int i=0;i<100;++i )
					{
						GCFORM::Label^l_pLabel = (GCFORM::Label^)m_AllLabelList[i];
						l_pLabel->Font = l_pFont;
						l_pLabel->Size = System::Drawing::Size(l_pLabel->Width,MainNode_treeView->ItemHeight);
					}
					System::Windows::Forms::TreeViewEventArgs^  e2;
					System::Object^sender;
					MainNode_treeView_AfterSelect(sender,e2);				
				}
			 }
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e)
		 {
			for( int i=0;i<100;++i )
			{
				GCFORM::Label^l_pLabel = (GCFORM::Label^)m_AllLabelList[i];
				l_pLabel->Size = System::Drawing::Size(l_pLabel->Width,MainNode_treeView->ItemHeight);
			}
		 }

private: System::Void toUTF32ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 cli::array<String^>^l_str = OpenFileAndGetNames();
			 if (l_str)
			 {
				 for each (String ^ l_strFileName in l_str)
				 {
					 FileToUnicode(l_strFileName, "big5");
				 }
			 }
		 }

private: System::Void openToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == Encode_ToolStripMenuItem )
			 {
				 cli::array<String^>^ l_strNames = DNCT::OpenFileAndGetNames();
				 if( l_strNames )
				 {
					 String^l_strDirectory = DNCT::GetDirectoryWithoutFileName(l_strNames[0]);
					 l_strDirectory += "Compress";
					 DNCT::CreateDirectory(l_strDirectory);
					 for each (String^l_strName in l_strNames )
					 {
						std::string	l_strFileNmae = DNCT::GcStringToChar(l_strName);
						System::String^l_strOutputFileName = l_strDirectory+"\\"+DNCT::GetFileNameWithoutFullPath(l_strName);
						std::string	l_strOutputFullFileName = DNCT::GcStringToChar(l_strOutputFileName);
						bool	l_b = forceIfFileIsInlegalToolStripMenuItem->Checked;
						FileCompressHuffman(l_strFileNmae.c_str(),l_strOutputFullFileName.c_str(),m_iBinaryHeaderID,l_b);
					 }
				 }			 
			 }
			 else
			 {
				String^l_strDirectoryName = DNCT::SelectDirectory();
				String^l_strErrorMsg = gcnew String("Compress failed:");
				bool	l_bFailed = false;
				if( l_strDirectoryName )
				{
					System::Collections::ArrayList^l_pArrayList = gcnew System::Collections::ArrayList;
					System::Collections::ArrayList^l_pArrayList2 = gcnew System::Collections::ArrayList;
					GetFilesNameByRecursivelyDirectoryAndAssignNewFolderToIt(l_strDirectoryName,l_pArrayList,"Compress",l_pArrayList2);
					for(int i=0;i<l_pArrayList->Count;++i )
					{
						String^l_strOriginalName = (String^)l_pArrayList[i];
						String^l_strNewName = (String^)l_pArrayList2[i];
						System::Diagnostics::Debug::WriteLine(l_strOriginalName);
						//System::Diagnostics::Debug::(L"\n");
						DNCT::CreateDirectory(System::IO::Path::GetDirectoryName(l_strNewName));
						std::string	l_strFileName = DNCT::GcStringToChar(l_strOriginalName);
						std::string	l_strOutputFileName = DNCT::GcStringToChar(l_strNewName);
						//try
						{
							bool	l_b = forceIfFileIsInlegalToolStripMenuItem->Checked;
							bool	l_bResult = FileCompressHuffman(l_strFileName.c_str(),l_strOutputFileName.c_str(),m_iBinaryHeaderID,l_b);
							if( !l_bResult )
							{
								l_strErrorMsg = "Compress failed:";
								l_strErrorMsg += l_strOriginalName;
								l_strErrorMsg += "\n";
								WARNING_MSG(l_strErrorMsg);							
							}
						}
						//catch(System::Exception^e)
						//{
						//	l_strErrorMsg = "Compress failed:";
						//	l_strErrorMsg += l_strOriginalName;
						//	l_strErrorMsg += "__";
						//	l_strErrorMsg += e->ToString();
						//	l_strErrorMsg += "\n";
						//	WARNING_MSG(l_strErrorMsg);
						//	//l_bFailed = true;
						//}
					}
				}
				if( l_bFailed )
				{
					WARNING_MSG(l_strErrorMsg);
				}
			 }
		 }

private: System::Void saveToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == Decode_ToolStripMenuItem )
			 {
				 cli::array<String^>^ l_strNames = DNCT::OpenFileAndGetNames();
				 if( l_strNames )
				 {
					 String^l_strDirectory = DNCT::GetDirectoryWithoutFileName(l_strNames[0]);
					 l_strDirectory += "Decode";
					 DNCT::CreateDirectory(l_strDirectory);				
					 for each (String^l_strName in l_strNames )
					 {
						std::string	l_strFileNmae = DNCT::GcStringToChar(l_strName);
						String^l_strOutputFileName = l_strDirectory+"/"+DNCT::GetFileNameWithoutFullPath(l_strName);
						std::string	l_strOutputFullFileName = DNCT::GcStringToChar(l_strOutputFileName);
						bool	l_b = forceIfFileIsInlegalToolStripMenuItem->Checked;
						FileDempressHuffman(l_strFileNmae.c_str(),l_strOutputFullFileName.c_str(),m_iBinaryHeaderID,l_b);
					 }
				 }
			 }
			 else
			 {
				String^l_strErrorMsg = gcnew String("decompress failed:");
				bool	l_bFailed = false;
				String^l_strDirectoryName = DNCT::SelectDirectory();
				if( l_strDirectoryName )
				{
					System::Collections::ArrayList^l_pArrayList = gcnew System::Collections::ArrayList;
					System::Collections::ArrayList^l_pArrayList2 = gcnew System::Collections::ArrayList;
					GetFilesNameByRecursivelyDirectoryAndAssignNewFolderToIt(l_strDirectoryName,l_pArrayList,"Decompress",l_pArrayList2);
					for(int i=0;i<l_pArrayList->Count;++i )
					{
						String^l_strOriginalName = (String^)l_pArrayList[i];
						String^l_strNewName = (String^)l_pArrayList2[i];
						DNCT::CreateDirectory(System::IO::Path::GetDirectoryName(l_strNewName));
						std::string	l_strFileName = DNCT::GcStringToChar(l_strOriginalName);
						std::string	l_strOutputFileName = DNCT::GcStringToChar(l_strNewName);
						try
						{
							bool	l_b = forceIfFileIsInlegalToolStripMenuItem->Checked;
							FileDempressHuffman(l_strFileName.c_str(),l_strOutputFileName.c_str(),m_iBinaryHeaderID,l_b);
						}
						catch(System::Exception^e)
						{
							l_strErrorMsg += e->ToString();
							l_strErrorMsg += "\n";
							l_bFailed = true;
						}
					}
				}
				if( l_bFailed )
				{
					WARNING_MSG(l_strErrorMsg);
				}
			 }
		 }

private: System::Void saveToBinaryToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			String^l_strFileName = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
			if( l_strFileName )
			{
				DNCT::SaveTreeViewToFile(MainNode_treeView,l_strFileName);
				if( System::IO::File::Exists(l_strFileName) )
				{
					auto l_strTargetFile = DNCT::GcStringToChar(l_strFileName);
					bool	l_b = forceIfFileIsInlegalToolStripMenuItem->Checked;
					if(!FileCompressHuffman(l_strTargetFile.c_str(),l_strTargetFile.c_str(),this->m_iBinaryHeaderID,l_b))
					{
						WARNING_MSG("save to binary failed");
					}
				}
			}
		 }
	private: System::Void Save_toolStripButton_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				SaveXmlFile(SaveToolStripMenuItem);
			 }
	private: System::Void replaceNodeByTemplateFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 bool	l_bOkToRewrite = false;
				 String^l_strTempFileName = "temp.bb";
				 String^l_strTemplateFileName = DNCT::OpenFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
				 String^l_strOutputFileName = "temp.bb3";
				 if( l_strTemplateFileName )
				 {
					std::string	l_strTempFileName1 = DNCT::GcStringToChar(l_strTempFileName);
					std::string	l_strTemplateFileName1 = DNCT::GcStringToChar(l_strTemplateFileName);
					std::string	l_strOutputFileNmae1 = DNCT::GcStringToChar(l_strOutputFileName);
					SaveTreeViewToFile(this->MainNode_treeView,l_strTempFileName);
					if( System::IO::File::Exists(l_strTempFileName) )
					{
						if(cNodeISAX::ReplaceNodeDataByTemplateNode(l_strTempFileName1.c_str(),l_strTemplateFileName1.c_str(),l_strOutputFileNmae1.c_str()))
						{
							if( System::IO::File::Exists(l_strOutputFileName) )
							{
								 MainNode_treeView->Nodes->Clear();
								 GCFORM::TreeNode^l_pNode = OpenFileGetTreeNode(l_strOutputFileName);
								 MainNode_treeView->Nodes->Add(l_pNode);
								l_bOkToRewrite = true;
								System::IO::File::Delete(l_strTempFileName);
								System::IO::File::Delete(l_strOutputFileName);
							}
						}
					}
				 }
				 if( !l_bOkToRewrite )
				 {
					WARNING_MSG("template file failed");
				 }
			 }
	private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
			 {
				 if(m_bFileChanged)
				 {
					 WARING_YES_NO_TO_NO("file not save,still leave!?")
					 {
						 e->Cancel = true;
					 }
				 }
			 }
	private: System::Void textConvertToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 EncryptStringConvert^l_pEncryptStringConvert = gcnew EncryptStringConvert();
				 l_pEncryptStringConvert->Show();
			 }
	private: System::Void forceIfFileIsInlegalToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 forceIfFileIsInlegalToolStripMenuItem->Checked = !forceIfFileIsInlegalToolStripMenuItem->Checked;
				 bool	l_bResult = forceIfFileIsInlegalToolStripMenuItem->Checked;
				 int a=0;
			 }
	private: System::Void toUTF32ByFolderToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		String^ l_strErrorMsg = gcnew String("to utf32 failed:");
		bool	l_bFailed = false;
		String^ l_strDirectoryName = DNCT::SelectDirectory();
		if (l_strDirectoryName)
		{
			System::Collections::ArrayList^ l_pArrayList = gcnew System::Collections::ArrayList;
			System::Collections::ArrayList^ l_pArrayList2 = gcnew System::Collections::ArrayList;
			GetFilesNameByRecursivelyDirectoryAndAssignNewFolderToIt(l_strDirectoryName, l_pArrayList, "UTF32XML", l_pArrayList2);
			for (int i = 0; i < l_pArrayList->Count; ++i)
			{
				String^ l_strOriginalName = (String^)l_pArrayList[i];
				if (l_strOriginalName->Contains(".xml") || l_strOriginalName->Contains(".fai")
					|| l_strOriginalName->Contains(".mpdi")
					|| l_strOriginalName->Contains(".pi")
					|| l_strOriginalName->Contains(".collision")
					|| l_strOriginalName->Contains(".prt")
					|| l_strOriginalName->Contains(".path"))
				{
					String^ l_strNewName = (String^)l_pArrayList2[i];
					DNCT::CreateDirectory(System::IO::Path::GetDirectoryName(l_strNewName));
					try
					{
						FileToUnicode(l_strOriginalName, "big5", l_strNewName);
						//System::IO::StreamReader^ l_pReader = gcnew System::IO::StreamReader(l_strOriginalName);
						//auto l_strContent = l_pReader->ReadToEnd();
						//l_pReader->Close();
						//System::IO::StreamWriter^ l_pWriter = gcnew System::IO::StreamWriter(l_strNewName, false, System::Text::Encoding::UTF32);
						//l_pWriter->Write(l_strContent);
						//l_pWriter->Close();
					}
					catch (System::Exception^ e)
					{
						l_strErrorMsg += e->ToString();
						l_strErrorMsg += "\n";
						l_bFailed = true;
					}
				}
			}
			WARNING_MSG("finish!");
		}
		if (l_bFailed)
		{
			WARNING_MSG(l_strErrorMsg);
		}
	}
};
}