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
	public ref class EventMouseTrigger_Editor:public EventBase_Editor
	{
	public:
		//this one for multibutton trigger UI,so won't add eventinstance to sFMBookEditorData::m_spEventInstance
		EventMouseTrigger_Editor(FMBookEditorUI::Event::Trigger::EventMouseTrigger^e_pEventMouseTriggerUI);
		EventMouseTrigger_Editor(FMBookEditorUI::Event::Trigger::EventMouseTrigger^e_pEventMouseTriggerUI,cEventMouseTrigger*e_pEventMouseTrigger);
	protected:
		~EventMouseTrigger_Editor();
    //my
		cEventMouseTrigger*										m_pEventMouseTrigger;
		FMBookEditorUI::Event::Trigger::EventMouseTrigger^		m_pEventMouseTriggerUI;
		//while data is changed call this
		private:	System::Void Valdiate(System::Object^  sender, System::EventArgs^  e);
		public:		virtual	System::String^ IsLegal() override;
	//end
	};
}