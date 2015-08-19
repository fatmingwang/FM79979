#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


#include "EventInstanceEditor.h"

namespace FMBookEditor
{
	public ref class EventInstanceList_Editor
	{
	public:
		EventInstanceList_Editor(FMBookEditorUI::FMBookUI^e_pUserControl1,cEventInstance_Editor^e_pEventSelector);
		~EventInstanceList_Editor();
    //my
		FMBookEditorUI::EventObjectInstanceList^m_pEventObjectInstanceList;
		public:FMBookEditorUI::EventInstanceList^		m_pEventInstanceList;
		cEventInstance_Editor^					m_pEventInstanceEditor;
		private: System::Void Add(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Del(System::Object^  sender, System::EventArgs^  e);

		private: System::Void DelListBoxItem(System::Object^  sender, System::EventArgs^  e);

		private: System::Void Revert(System::Object^  sender, System::EventArgs^  e);
		public: System::Void Revert();
		//set EventObjectInstance text name
		private: System::Void SelectionChanged(System::Object^  sender, System::Windows::Controls::SelectionChangedEventArgs^  e);
		public:	 void	 Refresh();
//end
	};
//end
}



