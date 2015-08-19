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
	public ref class EventButtonTrigger_Editor:public EventBase_Editor
	{
	public:
		//this one for multibutton trigger UI,so won't add eventinstance to sFMBookEditorData::m_spEventInstance
		EventButtonTrigger_Editor(FMBookEditorUI::Event::Trigger::EventButton^e_pEventButton);
		EventButtonTrigger_Editor(FMBookEditorUI::Event::Trigger::EventButtonTrigger^e_pEventButtonTrigger);
		EventButtonTrigger_Editor(FMBookEditorUI::Event::Trigger::EventButtonTrigger^e_pEventButtonTrigger,cEventButton*e_pEventButton);
	protected:
		~EventButtonTrigger_Editor();
    //my
		Vector3*m_pvButtonPos;
		//FMBookEditorUI::Event::Trigger::EventButtonTrigger^	m_pEventButtonTrigger;
		FMBookEditorUI::Event::Trigger::EventButton^		m_pEventButton;
		public:cEventButton*m_pButton;
		private: System::Void OpenColorDialog(System::Object^  sender, System::EventArgs^  e);
		//while data is changed call this
		private:	System::Void Valdiate(System::Object^  sender, System::EventArgs^  e);
		private:	System::Void PosChange(System::Object^  sender, System::EventArgs^  e);
		public:		virtual	System::String^ IsLegal() override;
		//it's possible for multi button so it is not a new event instance
		void		SetData(cEventButton*e_pEventButton,bool e_bNewInstance);
	//end
	};
//end
}