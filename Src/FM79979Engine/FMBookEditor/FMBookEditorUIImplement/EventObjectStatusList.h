#pragma once


#include "EventObjectStatus.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMBookEditor
{

	public ref class EventObjectStatusList_Editor
	{
	public:
		EventObjectStatusList_Editor(FMBookEditorUI::FMBookUI^e_pUserControl1,EventObjectStatus^e_pEditorEventObjectStatus)
		{
			m_pEventObjectStatus = e_pUserControl1->m_EventObjectStatus;
			m_pEditorEventObjectStatus = e_pEditorEventObjectStatus;
			m_pEventObjectInstanceList = e_pUserControl1->m_EventObjectInstanceList;
			m_pEventObjectStatusList = e_pUserControl1->m_EventObjectStatusList;
			m_pEventObjectStatusList->f_Add += gcnew EventHandler(this,&EventObjectStatusList_Editor::Add);
			m_pEventObjectStatusList->f_Del += gcnew EventHandler(this,&EventObjectStatusList_Editor::Del);
			m_pEventObjectStatusList->f_Revert  += gcnew EventHandler(this,&EventObjectStatusList_Editor::Revert);
			m_pEventObjectStatusList->m_EventObjectStatusList_listBox->SelectionChanged += gcnew System::Windows::Controls::SelectionChangedEventHandler(this,&EventObjectStatusList_Editor::SelectionChanged);
		}
	protected:
		~EventObjectStatusList_Editor()
		{
		}

    //my
		EventObjectStatus^						m_pEditorEventObjectStatus;
		FMBookEditorUI::EventObjectStatusList^	m_pEventObjectStatusList;
		FMBookEditorUI::EventObjectStatus^		m_pEventObjectStatus;
		FMBookEditorUI::EventObjectInstanceList^m_pEventObjectInstanceList;
		private: System::Void Add(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Del(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Revert(System::Object^  sender, System::EventArgs^  e);
		public:	 void	 Refresh();
		private: System::Void SelectionChanged(System::Object^  sender, System::Windows::Controls::SelectionChangedEventArgs^  e);
//end
	};
//end
}
