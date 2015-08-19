#pragma once


#include "ObjectStatusTrigger.h"

//#include "EventInstanceEditor.h"
#include "EventInstanceList.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace System::Speech::Synthesis;


namespace FMBookEditor
{

	public ref class cEventObjectInstance_Editor
	{
	public:
		cEventObjectInstance_Editor(FMBookEditorUI::FMBookUI^e_pUserControl1,EventInstanceList_Editor^e_pEventInstanceList_Editor)
		{
			m_pFMBookUI = e_pUserControl1;
			m_pEventInstanceList_Editor = e_pEventInstanceList_Editor;
			m_pEventObjectStatus = m_pFMBookUI->m_EventObjectStatus;
			m_pEventObjectInstanceList = m_pFMBookUI->m_EventObjectInstanceList;
			m_EventObjectInstance_listBox = m_pEventObjectInstanceList->m_EventObjectInstance_listBox;
			m_pEventObjectStatusName = m_pEventObjectInstanceList->m_EventStatus_textBox;
			m_pEventObjectInstanceName = m_pEventObjectInstanceList->m_EventObjectInstanceName_textBox;
			m_pEventInstanceName = m_pEventObjectInstanceList->m_EventInstance_textBox;
			m_pEventObjectStatusWorkingIndexByEventVariableName = m_pEventObjectInstanceList->m_StatusEventVariable;
			m_pAllStatusWorkingAtSameTime_CheckBox = m_pEventObjectInstanceList->m_AllStatusWorkingAtSameTime_CheckBox;

			m_pEventObjectInstanceList->f_Add += gcnew EventHandler(this,&cEventObjectInstance_Editor::Add);
			m_pEventObjectInstanceList->f_Del += gcnew EventHandler(this,&cEventObjectInstance_Editor::Del);
			//m_pEventObjectInstanceList->f_Revert += gcnew EventHandler(this,&cEventObjectInstance_Editor::Revert);
		}

	protected:
		~cEventObjectInstance_Editor()
		{

		}
    //my
		EventInstanceList_Editor^				m_pEventInstanceList_Editor;
		//CoreWPF::Common::EditableListbox^		m_EventObjectInstance_listBox;
		System::Windows::Controls::ListBox^		m_EventObjectInstance_listBox;
		System::Windows::Controls::TextBox^		m_pEventObjectStatusName;
		System::Windows::Controls::TextBox^		m_pEventObjectInstanceName;
		System::Windows::Controls::TextBox^		m_pEventInstanceName;
		System::Windows::Controls::TextBox^		m_pEventObjectStatusWorkingIndexByEventVariableName;
		System::Windows::Controls::CheckBox^	m_pAllStatusWorkingAtSameTime_CheckBox;
		FMBookEditorUI::EventObjectStatus^		m_pEventObjectStatus;
		FMBookEditorUI::EventObjectInstanceList^m_pEventObjectInstanceList;
		cEventObjectStatus*									m_pEditorEventObjectStatu;
		FMBookEditorUI::FMBookUI^m_pFMBookUI;

		//private: System::Void Revert(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Add(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Del(System::Object^  sender, System::EventArgs^  e);
		private: System::Void SelectedChanged(System::Object^  sender, System::EventArgs^  e);
		public:	 void	 Refresh();
	//end Event
	};
}
