#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

//#using "../../Media/XMLComponent.dll"


namespace FMBookEditor
{

	public ref class XMLText:public cFatmingUtilitiesBase
	{
	public:
		XMLText(CoreWPF::FMC::XMLText^e_pUserControl1)
		{
			m_pstrCurrentText = new std::wstring;
			m_pstrCurrentFileName = new std::wstring;
			m_pstrCurrentNode = new std::wstring;

			m_pTreeViewList = gcnew System::Collections::ArrayList();
			m_pTreeView = gcnew System::Windows::Forms::TreeView;
			m_pTreeView->Dock = System::Windows::Forms::DockStyle::Fill;

			m_TextBox = e_pUserControl1->m_TextBox;
			m_pFileList = e_pUserControl1->m_TextFile_listBox;			
			e_pUserControl1->WindowlsFomrControl->Child = m_pTreeView;

			e_pUserControl1->f_AddTextFile += gcnew EventHandler(this,&XMLText::AddText_button_Click);
			e_pUserControl1->f_DelTextFile += gcnew EventHandler(this,&XMLText::DelText_button_Click);
			e_pUserControl1->f_TextChanged += gcnew EventHandler(this,&XMLText::Text_ListBox_SelectedChanged);
			m_pTreeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &XMLText::TreeView_AfterSelect);
		}
protected:
		~XMLText()
		{
			SAFE_DELETE(m_pstrCurrentText);
			SAFE_DELETE(m_pstrCurrentFileName);
			SAFE_DELETE(m_pstrCurrentNode);
		}
		//my
		private: System::Windows::Controls::TextBox^	m_TextBox;
		private: System::Windows::Forms::TreeView^		m_pTreeView;
		private: System::Windows::Controls::ListBox^	m_pFileList;
		private: System::Collections::ArrayList^		m_pTreeViewList;
		public:std::wstring*							m_pstrCurrentText;
		public:std::wstring*							m_pstrCurrentFileName;
		public:std::wstring*							m_pstrCurrentNode;
		public: EventHandler^ TextChanged;
		private: System::Void AddText_button_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void DelText_button_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Text_ListBox_SelectedChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void TreeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e);
		public:	 void	Refresh();
	};
}
