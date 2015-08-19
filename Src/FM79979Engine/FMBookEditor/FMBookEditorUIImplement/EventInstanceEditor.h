#pragma once


//#include "ObjectStatusTrigger.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace System::Speech::Synthesis;


namespace FMBookEditor
{

	public ref class cEventInstance_Editor
	{
	public:
		cEventInstance_Editor(FMBookEditorUI::FMBookUI^e_pUserControl1)
		{
			m_pFMBookUI = e_pUserControl1;
			m_pEventListBox = m_pFMBookUI->m_EventInstance->m_EventList_Listbox;
			m_pWrapTabControl = m_pFMBookUI->m_EventInstance->WrapPaneTabControl;
			m_pTriggerEventWrapPanel = m_pFMBookUI->m_EventInstance->m_EventTriggerListWrapPanel;
			m_pActiverEventWrapPanel = m_pFMBookUI->m_EventInstance->m_EventActiverListWrapPanel;

			m_pFMBookUI->m_EventInstance->f_ActiverEventListboxDoubleClick  += gcnew EventHandler(this,&cEventInstance_Editor::ActiverEventListboxDoubleClick);
			m_pFMBookUI->m_EventInstance->f_TriggerEventListboxDoubleClick  += gcnew EventHandler(this,&cEventInstance_Editor::TriggerEventListboxDoubleClick);
			m_pFMBookUI->m_EventInstance->f_DelEvent += gcnew EventHandler(this,&cEventInstance_Editor::DelEventFromEventList);
			m_pFMBookUI->m_EventInstance->f_EventInit += gcnew EventHandler(this,&cEventInstance_Editor::EventInit);
			m_pEventListHashtable = gcnew System::Collections::Hashtable();
			m_pEventWithImplementHashtable = gcnew System::Collections::Hashtable();
		}

	protected:
		~cEventInstance_Editor();
    //my
		//activer,trigger and lisxboxitem
		public:System::Collections::Hashtable^m_pEventListHashtable;
		//see above,this one will store UIImplement class
		public:System::Collections::Hashtable^m_pEventWithImplementHashtable;

		public:System::Windows::Controls::ListBox^m_pEventListBox;
		public:System::Windows::Controls::WrapPanel^m_pTriggerEventWrapPanel;
		public:System::Windows::Controls::TabControl^m_pWrapTabControl;
		public:System::Windows::Controls::WrapPanel^m_pActiverEventWrapPanel;
		FMBookEditorUI::FMBookUI^m_pFMBookUI;

		private: System::Void DelEventFromEventList(System::Object^  sender, System::EventArgs^  e);
		private: System::Void ActiverEventListboxDoubleClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void TriggerEventListboxDoubleClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void EventInit(System::Object^  sender, System::EventArgs^  e);

		public:	 void		Clear();
		public:	 bool		IsLegal();
	//end Event
	};
}
