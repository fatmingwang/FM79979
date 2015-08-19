#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "EventBase_Editor.h"

namespace FMBookEditor
{
	public ref class EventButtonSelectedActiver_Editor:public EventBase_Editor
	{
	public:
		EventButtonSelectedActiver_Editor(FMBookEditorUI::Event::Activer::EventButtonSelectedActiver^e_pEventButtonSelectedActiver);
		EventButtonSelectedActiver_Editor(FMBookEditorUI::Event::Activer::EventButtonSelectedActiver^e_pEventButtonSelectedActiver,cEventButtonSelectedActiver*e_pEventButtonSelectedActiver2);
	protected:
		~EventButtonSelectedActiver_Editor();
    //my
		cEventButtonSelectedActiver*m_pEventButtonSelectedActiver;
		FMBookEditorUI::Event::Activer::EventButtonSelectedActiver^m_pEventButtonSelectedActiverUI;
		private:	System::Void Valdiate(System::Object^  sender, System::EventArgs^  e);
		private: System::Void AddResult(System::Object^  sender, System::EventArgs^  e);
		private: System::Void DelResult(System::Object^  sender, System::EventArgs^  e);
		public:		virtual	System::String^ IsLegal() override;
	//end
	};
//end
}