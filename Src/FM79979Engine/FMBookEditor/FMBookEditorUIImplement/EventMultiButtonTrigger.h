#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "EventButtonTrigger.h"
namespace FMBookEditor
{
	public ref class EventMultiButtonTrigger_Editor:public EventBase_Editor
	{
	public:
		EventMultiButtonTrigger_Editor(FMBookEditorUI::Event::Trigger::EventMultiButtonTrigger^e_pEventMultiButtonTrigger);
		EventMultiButtonTrigger_Editor(FMBookEditorUI::Event::Trigger::EventMultiButtonTrigger^e_pEventMultiButtonTrigger,cEventMultiButton*e_pEventMultiButton);
	protected:
		~EventMultiButtonTrigger_Editor();

    //my
		cEventMultiButton*										m_pEventMultiButton;
		FMBookEditorUI::Event::Trigger::EventMultiButtonTrigger^m_pEventMultiButtonTrigger;
		EventButtonTrigger_Editor^								m_pCurrentEventButtonTrigger_Editor;
		private: System::Void Add(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Del(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Revert(System::Object^  sender, System::EventArgs^  e);
//end
	};
//end
}