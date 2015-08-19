#pragma once

#include "NodeElementAndAttribute.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace XMLDataEditor
{
	/// <summary>
	/// Summary for xmlNodeParseCodeGenerator
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class xmlNodeParseCodeGenerator : public System::Windows::Forms::Form
	{
	public:
		xmlNodeParseCodeGenerator(String^e_strFileName)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			 Main_treeView->Nodes->Clear();

			 if( System::IO::File::Exists(e_strFileName) )
			 {
				 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(e_strFileName);
				 if( l_pNode )
				 {
					 this->Text = e_strFileName;
					 Main_treeView->Nodes->Add(l_pNode);
				 }
			 }
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~xmlNodeParseCodeGenerator()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::TreeView^  Main_treeView;
	private: System::Windows::Forms::TextBox^  Header_textBox;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  GenerateSourceFunctionData_checkBox;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  ParserFunctionInputData_textBox;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  EnumName_textBox;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  ClassName_textBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::CheckBox^  CompareTextByEnum_checkBox;
	private: System::Windows::Forms::TextBox^  ExtractClassInformation_textBox;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::CheckBox^  StringIsUnicode_checkBox;


	private: System::Windows::Forms::TextBox^  Source_textBox;



	void AddAttributesAndChildren(XmlNode^ inXmlNode, cNodeElementAndAttribute^ e_pNodeElementAndAttribute)
	{
		//find out element full name
		String^l_strElementName;
		XmlNode^l_pParentNode = inXmlNode->ParentNode;
		//find whole parent's name
		System::Collections::ArrayList^l_pList = gcnew System::Collections::ArrayList;
		while( l_pParentNode )
		{
			String^l_str = l_pParentNode->Name;
			if(l_pParentNode->ChildNodes->Count)
				l_str+="_";
			l_pParentNode = l_pParentNode->ParentNode;
			l_pList->Add(l_str);
		}
		//add whole parent's name
		for( int i=l_pList->Count-1;i!=-1;--i )
		{
			String^l_str = (String^)l_pList[i];
			l_strElementName+=l_str;
		}
		l_strElementName += inXmlNode->Name;
		//check if name exists do not add it again
		bool	l_bContain = false;
		for each(String^l_str in e_pNodeElementAndAttribute->m_pWholeElement)
		{
			if(l_str->Equals(l_strElementName))
			{
				l_bContain = true;
				l_strElementName = l_str;
				break;
			}
		}
		if( !l_bContain )
		{
			e_pNodeElementAndAttribute->m_pWholeElement->Add(l_strElementName);
		}
		//add element name and whole parent's name into hashtable,if not exist
		if(!e_pNodeElementAndAttribute->m_pElementNameAndElementWholeName->ContainsKey(l_strElementName))
		{
			e_pNodeElementAndAttribute->m_pElementNameAndElementWholeName[l_strElementName] = inXmlNode->Name;
		}
		//find all children's name
		System::Collections::ArrayList^l_pChildrenList;
		if(!e_pNodeElementAndAttribute->m_pElementAndElementChidrenList->ContainsKey(l_strElementName))
		{
			l_pChildrenList = gcnew System::Collections::ArrayList;
		}
		else
		{
			l_pChildrenList = (System::Collections::ArrayList^)e_pNodeElementAndAttribute->m_pElementAndElementChidrenList[l_strElementName];
		}
		for each (System::Xml::XmlNode^l_pNode in inXmlNode->ChildNodes)
		{
			if(!l_pChildrenList->Contains(l_pNode->Name))
			{
				if( !l_pNode->Name->Equals("#text") )
				{
					l_pChildrenList->Add(l_pNode);
					String^l_strElementWholeName = l_strElementName+"_"+l_pNode->Name;
					e_pNodeElementAndAttribute->m_pElementAndElementChidrenListWholeName[l_pNode] = l_strElementWholeName;
				}
			}
		}
		if( l_pChildrenList->Count )
		{
			e_pNodeElementAndAttribute->m_pElementAndElementChidrenList[l_strElementName] = l_pChildrenList;
		}
		//find whole attribute
		XmlAttributeCollection^ atts = inXmlNode->Attributes;
		//find out attribute
		if( atts )
        if (atts->Count != 0)
		{
			//firs to store all attributes
			System::Collections::Hashtable^l_pAttributeOfElement = gcnew System::Collections::Hashtable;
            for each (XmlAttribute ^att in atts)
			{
				l_pAttributeOfElement[att->Name] = att->Value;
            }
			//2nd find out the attribute list if we have already add it,or add it
			if(e_pNodeElementAndAttribute->m_pElementAndAttributes->ContainsKey(l_strElementName))
			{
				//3rd if the attribute is contain,compare whole attribute,if the attribute is not in the hashtable then add it
				System::Collections::Hashtable^l_pOldAttributeOfElement = (System::Collections::Hashtable^)e_pNodeElementAndAttribute->m_pElementAndAttributes[l_strElementName];
				for each (String^ l_strKey in l_pAttributeOfElement->Keys)
				{//4th test attribute is exists,if not add new attribute
					if( !l_pOldAttributeOfElement->ContainsKey(l_strKey) )
					{
						l_pOldAttributeOfElement[l_strKey] = l_pAttributeOfElement[l_strKey];
					}
				}
			}
			else
			{
				e_pNodeElementAndAttribute->m_pElementAndAttributes[l_strElementName] = l_pAttributeOfElement;
			}
        }
		//
        for each (XmlNode^ xNode in inXmlNode->ChildNodes)
		{
            switch (xNode->NodeType)
			{
			case XmlNodeType::Element :
                AddAttributesAndChildren(xNode, e_pNodeElementAndAttribute);
                break;
			case XmlNodeType::Text:
				if(!e_pNodeElementAndAttribute->m_pElementAndText->ContainsKey(l_strElementName))
				{
					e_pNodeElementAndAttribute->m_pElementAndText[l_strElementName] = l_strElementName+XML_TEXT;
				}
                break;
            case XmlNodeType::Comment:
				if(!e_pNodeElementAndAttribute->m_pElementAndComment->ContainsKey(l_strElementName))
				{
					e_pNodeElementAndAttribute->m_pElementAndComment[l_strElementName] = l_strElementName+xNode->Name;
				}
                break;
            case XmlNodeType::ProcessingInstruction:
                //tNode = gcnew TreeNode(xNode->Value);
                break;
            case XmlNodeType::CDATA:
                //tNode = gcnew TreeNode(xNode->Value);
                break;				
            default :
				assert(0);
//                throw gcnew Exception("Unexpected NodeType: " + xNode->NodeType->ToString());
            }
        }
    }
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->Main_treeView = (gcnew System::Windows::Forms::TreeView());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->Header_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Source_textBox = (gcnew System::Windows::Forms::TextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->GenerateSourceFunctionData_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->ParserFunctionInputData_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->EnumName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->ClassName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->CompareTextByEnum_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ExtractClassInformation_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->StringIsUnicode_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Location = System::Drawing::Point(80, 144);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(4);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->Main_treeView);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
			this->splitContainer1->Size = System::Drawing::Size(1477, 642);
			this->splitContainer1->SplitterDistance = 488;
			this->splitContainer1->SplitterWidth = 5;
			this->splitContainer1->TabIndex = 1;
			// 
			// Main_treeView
			// 
			this->Main_treeView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Main_treeView->Location = System::Drawing::Point(0, 0);
			this->Main_treeView->Margin = System::Windows::Forms::Padding(4);
			this->Main_treeView->Name = L"Main_treeView";
			this->Main_treeView->Size = System::Drawing::Size(488, 642);
			this->Main_treeView->TabIndex = 0;
			this->Main_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &xmlNodeParseCodeGenerator::treeView1_AfterSelect);
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(4);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->Header_textBox);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->Source_textBox);
			this->splitContainer2->Size = System::Drawing::Size(984, 642);
			this->splitContainer2->SplitterDistance = 301;
			this->splitContainer2->SplitterWidth = 5;
			this->splitContainer2->TabIndex = 4;
			// 
			// Header_textBox
			// 
			this->Header_textBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Header_textBox->Location = System::Drawing::Point(0, 0);
			this->Header_textBox->Margin = System::Windows::Forms::Padding(4);
			this->Header_textBox->Multiline = true;
			this->Header_textBox->Name = L"Header_textBox";
			this->Header_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->Header_textBox->Size = System::Drawing::Size(984, 301);
			this->Header_textBox->TabIndex = 1;
			// 
			// Source_textBox
			// 
			this->Source_textBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Source_textBox->Location = System::Drawing::Point(0, 0);
			this->Source_textBox->Margin = System::Windows::Forms::Padding(4);
			this->Source_textBox->Multiline = true;
			this->Source_textBox->Name = L"Source_textBox";
			this->Source_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->Source_textBox->Size = System::Drawing::Size(984, 336);
			this->Source_textBox->TabIndex = 3;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(1671, 28);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->openToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(44, 24);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(152, 24);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &xmlNodeParseCodeGenerator::openToolStripMenuItem_Click);
			// 
			// GenerateSourceFunctionData_checkBox
			// 
			this->GenerateSourceFunctionData_checkBox->AutoSize = true;
			this->GenerateSourceFunctionData_checkBox->Checked = true;
			this->GenerateSourceFunctionData_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->GenerateSourceFunctionData_checkBox->Location = System::Drawing::Point(340, 41);
			this->GenerateSourceFunctionData_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->GenerateSourceFunctionData_checkBox->Name = L"GenerateSourceFunctionData_checkBox";
			this->GenerateSourceFunctionData_checkBox->Size = System::Drawing::Size(194, 19);
			this->GenerateSourceFunctionData_checkBox->TabIndex = 24;
			this->GenerateSourceFunctionData_checkBox->Text = L"GenerateSourceFunctionData";
			this->GenerateSourceFunctionData_checkBox->UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(340, 76);
			this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(148, 15);
			this->label3->TabIndex = 23;
			this->label3->Text = L"ParserFunctionInputData";
			// 
			// ParserFunctionInputData_textBox
			// 
			this->ParserFunctionInputData_textBox->Location = System::Drawing::Point(508, 72);
			this->ParserFunctionInputData_textBox->Margin = System::Windows::Forms::Padding(4);
			this->ParserFunctionInputData_textBox->Name = L"ParserFunctionInputData_textBox";
			this->ParserFunctionInputData_textBox->Size = System::Drawing::Size(188, 25);
			this->ParserFunctionInputData_textBox->TabIndex = 22;
			this->ParserFunctionInputData_textBox->Text = L"TiXmlElement*e_pTiXmlElement";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(13, 72);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(74, 15);
			this->label2->TabIndex = 21;
			this->label2->Text = L"EnumName";
			// 
			// EnumName_textBox
			// 
			this->EnumName_textBox->Location = System::Drawing::Point(101, 69);
			this->EnumName_textBox->Margin = System::Windows::Forms::Padding(4);
			this->EnumName_textBox->Name = L"EnumName_textBox";
			this->EnumName_textBox->Size = System::Drawing::Size(188, 25);
			this->EnumName_textBox->TabIndex = 20;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(19, 38);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(69, 15);
			this->label1->TabIndex = 19;
			this->label1->Text = L"ClassName";
			// 
			// ClassName_textBox
			// 
			this->ClassName_textBox->Location = System::Drawing::Point(101, 34);
			this->ClassName_textBox->Margin = System::Windows::Forms::Padding(4);
			this->ClassName_textBox->Name = L"ClassName_textBox";
			this->ClassName_textBox->Size = System::Drawing::Size(188, 25);
			this->ClassName_textBox->TabIndex = 18;
			// 
			// CompareTextByEnum_checkBox
			// 
			this->CompareTextByEnum_checkBox->AutoSize = true;
			this->CompareTextByEnum_checkBox->Checked = true;
			this->CompareTextByEnum_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CompareTextByEnum_checkBox->Location = System::Drawing::Point(539, 41);
			this->CompareTextByEnum_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->CompareTextByEnum_checkBox->Name = L"CompareTextByEnum_checkBox";
			this->CompareTextByEnum_checkBox->Size = System::Drawing::Size(156, 19);
			this->CompareTextByEnum_checkBox->TabIndex = 25;
			this->CompareTextByEnum_checkBox->Text = L"CompareTextByEnum";
			this->CompareTextByEnum_checkBox->UseVisualStyleBackColor = true;
			// 
			// ExtractClassInformation_textBox
			// 
			this->ExtractClassInformation_textBox->Location = System::Drawing::Point(969, 39);
			this->ExtractClassInformation_textBox->Margin = System::Windows::Forms::Padding(4);
			this->ExtractClassInformation_textBox->Multiline = true;
			this->ExtractClassInformation_textBox->Name = L"ExtractClassInformation_textBox";
			this->ExtractClassInformation_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->ExtractClassInformation_textBox->Size = System::Drawing::Size(468, 98);
			this->ExtractClassInformation_textBox->TabIndex = 26;
			this->ExtractClassInformation_textBox->Text = L"public:\r\n        DEFINE_TYPE_INFO();\r\n        void\tParseData(TiXmlElement*e_pTiXm" 
				L"lElement);";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(785, 46);
			this->label4->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(149, 15);
			this->label4->TabIndex = 27;
			this->label4->Text = L"Extract class information";
			// 
			// StringIsUnicode_checkBox
			// 
			this->StringIsUnicode_checkBox->AutoSize = true;
			this->StringIsUnicode_checkBox->Checked = true;
			this->StringIsUnicode_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->StringIsUnicode_checkBox->Location = System::Drawing::Point(340, 9);
			this->StringIsUnicode_checkBox->Name = L"StringIsUnicode_checkBox";
			this->StringIsUnicode_checkBox->Size = System::Drawing::Size(121, 19);
			this->StringIsUnicode_checkBox->TabIndex = 28;
			this->StringIsUnicode_checkBox->Text = L"StringIsUnicode";
			this->StringIsUnicode_checkBox->UseVisualStyleBackColor = true;
			// 
			// xmlNodeParseCodeGenerator
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1671, 801);
			this->Controls->Add(this->StringIsUnicode_checkBox);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->ExtractClassInformation_textBox);
			this->Controls->Add(this->CompareTextByEnum_checkBox);
			this->Controls->Add(this->GenerateSourceFunctionData_checkBox);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->ParserFunctionInputData_textBox);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->EnumName_textBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->ClassName_textBox);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"xmlNodeParseCodeGenerator";
			this->Text = L"xmlNodeParseCodeGenerator";
			this->Resize += gcnew System::EventHandler(this, &xmlNodeParseCodeGenerator::xmlNodeParseCodeGenerator_Resize);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel1->PerformLayout();
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->Panel2->PerformLayout();
			this->splitContainer2->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
			 { 
				//check for legal
				GCFORM::TreeNode^l_pSelectedNode = Main_treeView->SelectedNode;
				if( !l_pSelectedNode )
					return;
				if( l_pSelectedNode->Text->Equals(XML_COMMENT)||l_pSelectedNode->Text->Equals(XML_TEXT) )
				{
					WARNING_MSG("comment and text can't be apply");
					return;
				}
				//make tree node to xml node
				XmlDocument^ d = gcnew XmlDocument();
				XmlNode^ n = d->CreateNode(XmlNodeType::Element,"root","");

				System::Xml::XmlNode^l_pXmlNode = DNCT::getXmlNode(l_pSelectedNode, d);
				if( l_pXmlNode )
					n->AppendChild(l_pXmlNode);

				//for each (TreeNode^ t in l_pSelectedNode->Nodes)
				//{
				//	l_pXmlNode = DNCT::getXmlNode(t, d);
				//	if(  l_pXmlNode )
				//	{
				//		n->AppendChild(l_pXmlNode);
				//	}
				//}
				//input data to store info we needed
				cNodeElementAndAttribute^ e_pNodeElementAndAttribute = gcnew cNodeElementAndAttribute();
				System::Collections::ArrayList^l_pXmlNodeList = gcnew System::Collections::ArrayList;
				for each(XmlNode^l_pDestNode in n->ChildNodes  )
				{
					l_pXmlNodeList->Add(l_pDestNode);
				}
				//we don't wanna root's name so do remove here.
				n->RemoveAll();
				for each(XmlNode^l_pDestNode in l_pXmlNodeList  )
					AddAttributesAndChildren(l_pDestNode,e_pNodeElementAndAttribute);
				//assign default value we want to assign
				char	l_cDoubleQuote[2] = {'"','\0'};
				String^l_strDoubleQuote = String(l_cDoubleQuote).ToString();
				if( !ClassName_textBox->Text->Length )
					ClassName_textBox->Text = l_pSelectedNode->Text;
				if( !EnumName_textBox->Text->Length )
					EnumName_textBox->Text = l_pSelectedNode->Text;		
				//,cpp
				String^l_strFunctionInputData = ParserFunctionInputData_textBox->Text;
				//class name
				String^l_strForClassName = ClassName_textBox->Text;
				//enum type name
				String^l_strForEnumName = EnumName_textBox->Text;
				//WholeElement_listBox->Items->Clear();
				//for enum member
				String^l_strForHeaderEnum = "enum e"+l_strForEnumName+"List"+DNCT::GetChanglineString();
				l_strForHeaderEnum +="{"+DNCT::GetChanglineString();
				//
				String^l_strForHeaderClass = "class c"+l_strForClassName+" :public NamedTypedObject"+DNCT::GetChanglineString();
				l_strForHeaderClass+="{"+DNCT::GetChanglineString();
				//l_strForHeaderClass+="void	"+l_strForClassName+"::HandleElementData("+l_strFunctionInputData+")"+DNCT::GetChanglineString();
				//
				String^l_strWholeElementStaticName;
				if( this->StringIsUnicode_checkBox->Checked )
					l_strWholeElementStaticName = "static const WCHAR*g_s"+l_strForClassName+"Element[e"+l_strForEnumName+"_MAX"+"] ="+DNCT::GetChanglineString();
				else
					l_strWholeElementStaticName = "static const char*g_s"+l_strForClassName+"Element[e"+l_strForEnumName+"_MAX"+"] ="+DNCT::GetChanglineString();
				l_strWholeElementStaticName+="{"+DNCT::GetChanglineString();
				//
				String^l_strForFunction;
				bool	l_bFirst  = true;
				for each (String^l_str in e_pNodeElementAndAttribute->m_pWholeElement  )
				{
					//
					l_strForHeaderEnum += "	e"+l_strForEnumName+"_"+l_str;
					if( l_bFirst )
						l_strForHeaderEnum+=" = 0,"+DNCT::GetChanglineString();
					else
						l_strForHeaderEnum+=","+DNCT::GetChanglineString();
					//
					l_bFirst = false;
					l_strForHeaderClass += "//"+DNCT::GetChanglineString();
					l_strForHeaderClass += "	void	Parse"+l_str+"("+l_strFunctionInputData+");"+DNCT::GetChanglineString();
					//
					String^l_strElementNameFromHashTable = (String^)e_pNodeElementAndAttribute->m_pElementNameAndElementWholeName[l_str];
					l_strWholeElementStaticName+="	"+l_strDoubleQuote+l_strElementNameFromHashTable+l_strDoubleQuote+","+DNCT::GetChanglineString();
					//
					//
					l_strForFunction +="//"+DNCT::GetChanglineString()
						+"//"+DNCT::GetChanglineString()
						+"//"+DNCT::GetChanglineString();
					l_strForFunction += "void	c"+l_strForClassName+"::Parse"+l_str+"("+l_strFunctionInputData+")"+DNCT::GetChanglineString();
					l_strForFunction += "{"+DNCT::GetChanglineString();
					if( GenerateSourceFunctionData_checkBox->Checked )
					{
						if( this->StringIsUnicode_checkBox->Checked )
						{
							l_strForFunction += "	const WCHAR*l_strName = 0;"+DNCT::GetChanglineString();
							l_strForFunction += "	const WCHAR*l_strValue = 0;"+DNCT::GetChanglineString();
						}
						else
						{
							l_strForFunction += "	const char*l_strName = 0;"+DNCT::GetChanglineString();
							l_strForFunction += "	const char*l_strValue = 0;"+DNCT::GetChanglineString();						
						}
						if(e_pNodeElementAndAttribute->m_pElementAndAttributes->ContainsKey(l_str))
						{
							l_strForFunction += "	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();"+DNCT::GetChanglineString();
							l_strForFunction += "	while( l_pTiXmlAttribute )"+DNCT::GetChanglineString();
							l_strForFunction += "	{"+DNCT::GetChanglineString();
							l_strForFunction += "		l_strName = l_pTiXmlAttribute->Name();"+DNCT::GetChanglineString();
							l_strForFunction += "		l_strValue = l_pTiXmlAttribute->Value();"+DNCT::GetChanglineString();
						   System::Collections::Hashtable^l_pAttributesTable = (System::Collections::Hashtable^)e_pNodeElementAndAttribute->m_pElementAndAttributes[l_str];
						   IEnumerator^ myEnum = l_pAttributesTable->GetEnumerator();
						   while ( myEnum->MoveNext() )
						   {
							  DictionaryEntry de =  *safe_cast<DictionaryEntry ^>(myEnum->Current);
							  //WholeElement_listBox->Items->Add("Attributes:"+de.Key+","+de.Value);
							  if( this->StringIsUnicode_checkBox->Checked )
								l_strForFunction += "		if(!wcscmp(l_strName,L"+l_strDoubleQuote;
							  else
								l_strForFunction += "		if(!strcmp(l_strName,"+l_strDoubleQuote;
							  l_strForFunction += de.Key;
							  l_strForFunction += l_strDoubleQuote;
							  l_strForFunction += "))"+DNCT::GetChanglineString()+"		{"+DNCT::GetChanglineString()+"			l_strValue;"+DNCT::GetChanglineString()+"		}"+DNCT::GetChanglineString()+"		else"+DNCT::GetChanglineString();
						   }
						   l_strForFunction += "		{"+DNCT::GetChanglineString();
						   l_strForFunction += "			assert(0);"+DNCT::GetChanglineString();
						   l_strForFunction += "		}"+DNCT::GetChanglineString();
						   l_strForFunction += "		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();"+DNCT::GetChanglineString();
						   l_strForFunction += "	}"+DNCT::GetChanglineString();
						}
						if(e_pNodeElementAndAttribute->m_pElementAndText->ContainsKey(l_str))
						{
							if( this->StringIsUnicode_checkBox->Checked )
								l_strForFunction += "	const WCHAR*l_strText = e_pTiXmlElement->GetText();"+DNCT::GetChanglineString();
							else
								l_strForFunction += "	const char*l_strText = e_pTiXmlElement->GetText();"+DNCT::GetChanglineString();
							l_strForFunction += "	assert(l_strText);"+DNCT::GetChanglineString();
						}
						System::Collections::ArrayList^l_pList = (System::Collections::ArrayList^)e_pNodeElementAndAttribute->m_pElementAndElementChidrenList[l_str];
						if( l_pList&&l_pList->Count )
						{
							l_strForFunction += "	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();"+DNCT::GetChanglineString();
							l_strForFunction += "	while(l_pChildTiXmlElement)"+DNCT::GetChanglineString();
							l_strForFunction += "	{"+DNCT::GetChanglineString();
							l_strForFunction += "		l_strName = l_pChildTiXmlElement->Value();"+DNCT::GetChanglineString();
							//prevent same function has called so here we store the function has used
							System::Collections::ArrayList^l_pUsedFunctionList = gcnew System::Collections::ArrayList;
							for each(System::Xml::XmlNode^l_strChildName in l_pList)
							{
								//find out child's whole name
								String^l_strChildWholeName = (String^)e_pNodeElementAndAttribute->m_pElementAndElementChidrenListWholeName[l_strChildName];
								bool	l_bUsed = false;
								for each ( String^l_strUsedFunctionName in l_pUsedFunctionList )
								{
									if(l_strUsedFunctionName->Equals(l_strChildWholeName))
										l_bUsed = true;
								}
								if( l_bUsed )
									continue;
								l_pUsedFunctionList->Add(l_strChildWholeName);
								if( this->StringIsUnicode_checkBox->Checked )
									l_strForFunction += "		if(!wcscmp(l_strName,";
								else
									l_strForFunction += "		if(!strcmp(l_strName,";
								if( CompareTextByEnum_checkBox->Checked )
								{
									l_strForFunction += "g_s"+l_strForClassName+"Element[e"+l_strForEnumName+"_"+l_strChildWholeName+"]";
								}
								else
								{
									if( this->StringIsUnicode_checkBox->Checked )
										l_strForFunction += "L";
									l_strForFunction += l_strDoubleQuote;
									l_strForFunction += l_strChildName->Name;
									l_strForFunction += l_strDoubleQuote;
								}
								l_strForFunction += "))"+DNCT::GetChanglineString();
								l_strForFunction += "		{"+DNCT::GetChanglineString();
								//call function name
								l_strForFunction += "			Parse"+l_strChildWholeName+"( "+ "l_pChildTiXmlElement" +" );"+DNCT::GetChanglineString();
								l_strForFunction += "		}"+DNCT::GetChanglineString();
								l_strForFunction += "		else"+DNCT::GetChanglineString();
							}
							l_strForFunction += "		{"+DNCT::GetChanglineString();
							l_strForFunction += "			assert(0);"+DNCT::GetChanglineString();
							l_strForFunction += "		}"+DNCT::GetChanglineString();
							l_strForFunction += "		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();"+DNCT::GetChanglineString();
							l_strForFunction +=	"	}"+DNCT::GetChanglineString();
						}
					}
					l_strForFunction += DNCT::GetChanglineString()+"}"+DNCT::GetChanglineString();
					//
					//WholeElement_listBox->Items->Add(l_str);
					//if(e_pNodeElementAndAttribute->m_pElementAndAttributes->ContainsKey(l_str))
					//{
					//   System::Collections::Hashtable^l_pAttributesTable = (System::Collections::Hashtable^)e_pNodeElementAndAttribute->m_pElementAndAttributes[l_str];
					//   IEnumerator^ myEnum = l_pAttributesTable->GetEnumerator();
					//   while ( myEnum->MoveNext() )
					//   {
					//	  DictionaryEntry de =  *safe_cast<DictionaryEntry ^>(myEnum->Current);
					//	  WholeElement_listBox->Items->Add("Attributes:"+de.Key+","+de.Value);
					//   }
					//}
					//if(e_pNodeElementAndAttribute->m_pElementAndComment->ContainsKey(l_str))
					//{
					//	String^l_strComment = (String^)e_pNodeElementAndAttribute->m_pElementAndComment[l_str];
					//	WholeElement_listBox->Items->Add("Comment:"+l_strComment);
					//}
					//if(e_pNodeElementAndAttribute->m_pElementAndText->ContainsKey(l_str))
					//{
					//	String^l_strText = (String^)e_pNodeElementAndAttribute->m_pElementAndText[l_str];
					//	WholeElement_listBox->Items->Add("Text:"+l_strText);
					//}
				}
				//
				l_strForHeaderEnum += "	e"+l_strForEnumName+"_MAX"+DNCT::GetChanglineString();
				l_strForHeaderEnum += "};"+DNCT::GetChanglineString();
				//
				l_strWholeElementStaticName += "};"+DNCT::GetChanglineString();
				//
				l_strForHeaderClass += ExtractClassInformation_textBox->Text+DNCT::GetChanglineString();
				l_strForHeaderClass += "};"+DNCT::GetChanglineString();
				//
				this->Header_textBox->Text = l_strForHeaderEnum;
				this->Header_textBox->Text += l_strWholeElementStaticName;
				this->Header_textBox->Text += l_strForHeaderClass;
				this->Source_textBox->Text = l_strForFunction;
			 }
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 Main_treeView->Nodes->Clear();
			 String^l_strFileName = DNCT::OpenFileAndGetName("xml Files(*.dae;*.xml;*.fat)|*.dae;*.xml;*.fat|All files (*.*)|*.*");
			 if( !l_strFileName )
				 return;
			 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_strFileName);
			 if( l_pNode )
			 {
				 this->Text = l_strFileName;
				 Main_treeView->Nodes->Add(l_pNode);
			 }
		 }
private: System::Void xmlNodeParseCodeGenerator_Resize(System::Object^  sender, System::EventArgs^  e)
		 {
			 splitContainer1->Size = System::Drawing::Size(this->Size.Width-splitContainer1->Location.X,this->Size.Height-splitContainer1->Location.Y);
		 }
};
}
