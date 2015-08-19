#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMBookEditor
{
	public ref class EventPackages_Editor
	{
	public:
		EventPackages_Editor(FMBookEditorUI::FMBookUI^e_pUserControl1)
		{
			m_pEventPackages = e_pUserControl1->m_EventPackages;
			m_pFMBookUI = e_pUserControl1;

			m_pEventPackages->f_Add += gcnew EventHandler(this,&EventPackages_Editor::Add);
			m_pEventPackages->f_Del += gcnew EventHandler(this,&EventPackages_Editor::Del);
			m_pEventPackages->f_Revert += gcnew EventHandler(this,&EventPackages_Editor::Revert);			
		}
	protected:
		~EventPackages_Editor()
		{
		}

    //my
		FMBookEditorUI::EventPackages^	m_pEventPackages;
		FMBookEditorUI::FMBookUI^		m_pFMBookUI;
		private: System::Void Add(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Del(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Revert(System::Object^  sender, System::EventArgs^  e);
		public:	 void	 Refresh();
//end
	};
//end
}
