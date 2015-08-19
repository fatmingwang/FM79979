#include "StdAfx.h"
#include "EventMouseTrigger.h"
namespace FMBookEditor
{

	EventMouseTrigger_Editor::EventMouseTrigger_Editor(FMBookEditorUI::Event::Trigger::EventMouseTrigger^e_pEventMouseTriggerUI)
	{
		m_pEventMouseTriggerUI = e_pEventMouseTriggerUI;
		m_pEventMouseTrigger = new cEventMouseTrigger((TiXmlElement*)0);
	}

	EventMouseTrigger_Editor::EventMouseTrigger_Editor(FMBookEditorUI::Event::Trigger::EventMouseTrigger^e_pEventMouseTriggerUI,cEventMouseTrigger*e_pEventMouseTrigger)
	{

	}


	EventMouseTrigger_Editor::~EventMouseTrigger_Editor()
	{

	}


	System::Void EventMouseTrigger_Editor::Valdiate(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::String^ EventMouseTrigger_Editor::IsLegal()
	{
		return L"no implement";
	}
}