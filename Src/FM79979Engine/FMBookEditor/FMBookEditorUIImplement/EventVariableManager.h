#pragma once


#include "ObjectStatusTrigger.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace System::Speech::Synthesis;


namespace FMBookEditor
{

	public ref class cEventVariableManager_Editor
	{
	public:
		cEventVariableManager_Editor(FMBookEditorUI::FMBookUI^e_pUserControl1)
		{
			m_pFMBookUI = e_pUserControl1;
			m_pEventVariableManager = m_pFMBookUI->m_EventVariableManager;

			m_pEventVariableManager->f_Add += gcnew EventHandler(this,&cEventVariableManager_Editor::Add);
			m_pEventVariableManager->f_Del += gcnew EventHandler(this,&cEventVariableManager_Editor::Del);
			m_pEventVariableManager->f_Revert  += gcnew EventHandler(this,&cEventVariableManager_Editor::Revert);
		}

	protected:
		~cEventVariableManager_Editor()
		{

		}
    //my

		FMBookEditorUI::FMBookUI^	m_pFMBookUI;
		FMBookEditorUI::EventVariableManager^	m_pEventVariableManager;
		private: System::Void Add(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Del(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Revert(System::Object^  sender, System::EventArgs^  e);
		public:	 void	Refresh();
	//end Event
	};
}
