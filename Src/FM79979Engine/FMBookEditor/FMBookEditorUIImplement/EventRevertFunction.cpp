#include "StdAfx.h"
#include "EventRevertFunction.h"
#include "EventButtonTrigger.h"
#include "EventMultiButtonTrigger.h"
#include "EventValueChangeActiver.h"
#include "EventButtonSelectedActiver.h"


#define	FMBOOKUI_ACTIVER_UI(TargetType)	l_pUserControl = gcnew FMBookEditorUI::Event::Activer::TargetType(); l_strUIElementName = DNCT::WcharToGcstring(e_pBaseEvent->Type());
#define	FMBOOKUI_TRIGGER_UI(TargetType)	l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::TargetType();  l_strUIElementName = DNCT::WcharToGcstring(e_pBaseEvent->Type()); l_bTrigger = true;


void	SetEventBase_EditorByEventType(cEventBase*e_pBaseEvent,cEventInstance_Editor^e_pEventInstance_Editor)
{
	System::Windows::Controls::UserControl^l_pUserControl;
	System::Object^l_pUIImplementClass;
	bool	l_bTrigger = false;
	String^l_strUIElementName;
	if( e_pBaseEvent->Type() == cEventAddEventObjectInstaceActiver::TypeID)
	{
		FMBOOKUI_ACTIVER_UI(EventAddEventObjectInstaceActiver);
	}
	else
	if(e_pBaseEvent->Type() == cEventButtonSelectedActiver::TypeID)
	{
		FMBOOKUI_ACTIVER_UI(EventButtonSelectedActiver);
		l_pUIImplementClass = gcnew EventButtonSelectedActiver_Editor((FMBookEditorUI::Event::Activer::EventButtonSelectedActiver^)l_pUserControl,(cEventButtonSelectedActiver*)e_pBaseEvent);
	}
	//else
	//if(e_pBaseEvent->Type() == cEventListValueSelectionActiver::TypeID)
	//{
	//	l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventListValueSelectionActiver();
	//}
	//else
	//if(e_pBaseEvent->Type() == cEventExternalFunctionCallActiver::TypeID)
	//{
	//	l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventExternalFunctionCallActiver();
	//}
	//else
	//if(e_pBaseEvent->Type() == cEventListValueSelectionActiver::TypeID)
	//{
	//	l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventListValueSelectionActiver();
	//}
	////else
	////if(e_pBaseEvent->Type() == cEventMultiButtonSelectedActiver::TypeID)
	////{
	////	l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventMultiButtonSelectedActiver();
	////}
	//else
	//if(e_pBaseEvent->Type() == cEventObjecInstanceStatusChangeActiver::TypeID)
	//{
	//	l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventObjecInstanceStatusChangeActiver();
	//}
	else
	if(e_pBaseEvent->Type() == cEventValueChangeActiver::TypeID)
	{
		FMBOOKUI_ACTIVER_UI(EventValueChangeActiver);
		l_pUIImplementClass = gcnew EventValueChangeActiver_Editor((FMBookEditorUI::Event::Activer::EventValueChangeActiver^)l_pUserControl,(cEventValueChangeActiver*)e_pBaseEvent);
	}
	//else
	//if(e_pBaseEvent->Type() == cEventVariableCheckYesAndNoActiver::TypeID)
	//{
	//	l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventVariableCheckYesAndNoActiver();
	//}
	else//Trigger
	if(e_pBaseEvent->Type() == cEventButton::TypeID)
	{
		FMBOOKUI_TRIGGER_UI(EventButtonTrigger);
		l_pUIImplementClass = gcnew EventButtonTrigger_Editor((FMBookEditorUI::Event::Trigger::EventButtonTrigger^)l_pUserControl,(cEventButton*)e_pBaseEvent);
	}
	//else
	//if(e_pBaseEvent->Type() == cEventKeyUpDectectTrigger::TypeID)
	//{
	//	l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventKeyUpDectectTrigger();
	//}
	else
	if(e_pBaseEvent->Type() == cEventMultiButton::TypeID)
	{
		FMBOOKUI_TRIGGER_UI(EventMultiButtonTrigger);
		l_pUIImplementClass = gcnew EventMultiButtonTrigger_Editor((FMBookEditorUI::Event::Trigger::EventMultiButtonTrigger^)l_pUserControl,(cEventMultiButton*)e_pBaseEvent);
	}
	//else
	//if(e_pBaseEvent->Type() == cEventObjectStatusCheckerTrigger::TypeID)
	//{
	//	l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventObjectStatusCheckerTrigger();
	//}
	//else
	//if(e_pBaseEvent->Type() == cEventSelectionCheckerTrigger::TypeID)
	//{
	//	l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventSelectionCheckerTrigger();
	//}
	//else
	//if(e_pBaseEvent->Type() == cEventValueCheckerTrigger::TypeID)
	//{
	//	l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventValueCheckerTrigger();
	//}
	//else
	//if(e_pBaseEvent->Type() == cEventMouseTrigger::TypeID)
	//{
	//	l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventMouseTrigger();
	//}
	else
	{
		WARNING_MSG(DNCT::WcharToGcstring(e_pBaseEvent->Type())+" doest not supprt for now");
	}

	if( l_pUIImplementClass && l_pUserControl )
	{
		if(l_bTrigger)
			e_pEventInstance_Editor->m_pTriggerEventWrapPanel->Children->Add(l_pUserControl);
		else
			e_pEventInstance_Editor->m_pActiverEventWrapPanel->Children->Add(l_pUserControl);
		System::Windows::Controls::ListBoxItem^l_pSelectedItem = gcnew System::Windows::Controls::ListBoxItem();
		l_pSelectedItem->Content = l_strUIElementName;
		//m_pEventListBox->SelectionMode = System::Windows::Controls::SelectionMode::Single;
		e_pEventInstance_Editor->m_pEventListBox->Items->Add(l_pSelectedItem);
		e_pEventInstance_Editor->m_pEventListHashtable[l_pSelectedItem] = l_pUserControl;
		if( l_pUIImplementClass )
			e_pEventInstance_Editor->m_pEventWithImplementHashtable[l_pSelectedItem] = l_pUIImplementClass;	
	}
}

void	EventInstanceRevert(cEventInstance*e_pEventInstance,FMBookEditor::cEventInstance_Editor^e_pEventInstance_Editor)
{
	cDoAllMouseBehaviorList<cEventBase>*l_pTriggerEventList = &e_pEventInstance->m_TriggerEventList;
	cDoAllMouseBehaviorList<cEventBase>*l_pActiveEventList = &e_pEventInstance->m_ActiveEventList;
	cEventBase*l_pEventBase = 0;
	e_pEventInstance_Editor->Clear();
	for(int i=0;i<l_pTriggerEventList->Count();++i)
	{
		l_pEventBase = l_pTriggerEventList->GetObject(i);
		SetEventBase_EditorByEventType(l_pEventBase,e_pEventInstance_Editor);
	}
	for(int i=0;i<l_pActiveEventList->Count();++i)
	{
		l_pEventBase = l_pActiveEventList->GetObject(i);
		SetEventBase_EditorByEventType(l_pEventBase,e_pEventInstance_Editor);
	}
}