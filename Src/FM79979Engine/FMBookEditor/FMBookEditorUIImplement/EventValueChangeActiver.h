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
	public ref class EventValueChangeActiver_Editor:public EventBase_Editor
	{
	public:
		EventValueChangeActiver_Editor(FMBookEditorUI::Event::Activer::EventValueChangeActiver^e_pEventValueChangeActiver);
		EventValueChangeActiver_Editor(FMBookEditorUI::Event::Activer::EventValueChangeActiver^e_pEventValueChangeActiver,cEventValueChangeActiver*e_pEventValueChangeActiver2);
	protected:
		~EventValueChangeActiver_Editor();
    //my
		cEventValueChangeActiver*m_pEventValueChangeActiver;
		FMBookEditorUI::Event::Activer::EventValueChangeActiver^m_pEventValueChangeActiverUI;
		//private: System::Void OpenColorDialog(System::Object^  sender, System::EventArgs^  e);
		private:	System::Void Valdiate(System::Object^  sender, System::EventArgs^  e);
		//private:	System::Void PosChange(System::Object^  sender, System::EventArgs^  e);
		public:		virtual	System::String^ IsLegal() override;
	//end
	};
//end
}