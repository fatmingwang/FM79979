#include "StdAfx.h"
#include "EventInstanceEditor.h"
#include "EventButtonTrigger.h"
#include "EventValueChangeActiver.h"
#include "EventMultiButtonTrigger.h"
#include "EventButtonSelectedActiver.h"
namespace FMBookEditor
{
	cEventInstance_Editor::~cEventInstance_Editor()
	{
		Clear();
		delete m_pEventListHashtable;
		delete m_pEventWithImplementHashtable;
	}

	System::Void cEventInstance_Editor::DelEventFromEventList(System::Object^  sender, System::EventArgs^  e)
	{
		System::Windows::Controls::ListBox^l_pListBox = m_pFMBookUI->m_EventInstance->m_EventList_Listbox;
		if(l_pListBox->SelectedItem)
		{
			int	l_iIndex = l_pListBox->Items->IndexOf(l_pListBox->SelectedItem);
			System::Windows::Controls::UserControl^l_pUserControl = (System::Windows::Controls::UserControl^)m_pEventListHashtable[l_pListBox->SelectedItem];
			System::Object^l_pData = (System::Object^)m_pEventWithImplementHashtable[l_pListBox->SelectedItem];
			if( l_pData )
			{
				delete l_pData;
				m_pEventWithImplementHashtable->Remove(l_pListBox->SelectedItem);
			}
			if( l_pUserControl )
			{
				m_pEventListHashtable->Remove(l_pListBox->SelectedItem);
				if(m_pActiverEventWrapPanel->Children->Contains(l_pUserControl))
					m_pActiverEventWrapPanel->Children->Remove(l_pUserControl);
				if(m_pTriggerEventWrapPanel->Children->Contains(l_pUserControl))
					m_pTriggerEventWrapPanel->Children->Remove(l_pUserControl);
				l_pListBox->Items->Remove(l_pListBox->SelectedItem);
			}
		}
	}
	System::Void cEventInstance_Editor::ActiverEventListboxDoubleClick(System::Object^  sender, System::EventArgs^  e)
	{
		sFMBookEditorData::DataLegalCheck();
		System::Windows::Controls::ListBox^l_pListBox = (System::Windows::Controls::ListBox^)sender;
		System::Windows::Controls::ListBoxItem^l_pItem = (System::Windows::Controls::ListBoxItem^)l_pListBox->SelectedItem;
		if( !l_pItem )
			return;
		String^l_str = l_pItem->Content->ToString();
		System::Windows::Controls::UserControl^l_pUserControl;
		System::Object^l_pUIImplementClass;
		//if(l_str->Equals("CameraActiver"))
		//{
		//	m_pActiverEventWrapPanel->Children->Add(gcnew FMBookEditorUI::Event::Activer::CameraActiver());
		//}
		//else
		if(l_str->Equals("EventAddEventObjectInstaceActiver"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventAddEventObjectInstaceActiver();
		}
		else
		if(l_str->Equals("EventButtonSelectedActiver"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventButtonSelectedActiver();
			l_pUIImplementClass = gcnew EventButtonSelectedActiver_Editor((FMBookEditorUI::Event::Activer::EventButtonSelectedActiver^)l_pUserControl);
		}
		else
		if(l_str->Equals("EventListValueSelectionActiver"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventListValueSelectionActiver();
		}
		else
		if(l_str->Equals("EventExternalFunctionCallActiver"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventExternalFunctionCallActiver();
		}
		else
		if(l_str->Equals("EventListValueSelectionActiver"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventListValueSelectionActiver();
		}
		//else
		//if(l_str->Equals("EventMultiButtonSelectedActiver"))
		//{
		//	l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventMultiButtonSelectedActiver();
		//}
		else
		if(l_str->Equals("EventObjecInstanceStatusChangeActiver"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventObjecInstanceStatusChangeActiver();
		}
		else
		if(l_str->Equals("EventValueChangeActiver"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventValueChangeActiver();
			l_pUIImplementClass = gcnew EventValueChangeActiver_Editor((FMBookEditorUI::Event::Activer::EventValueChangeActiver^)l_pUserControl);
		}
		else
		if(l_str->Equals("EventVariableCheckYesAndNoActiver"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Activer::EventVariableCheckYesAndNoActiver();
		}
		else
		{
			WARNING_MSG(l_str+" doest not supprt for now");
			return;
		}
		if( l_pUserControl )
		{
			m_pActiverEventWrapPanel->Children->Add(l_pUserControl);
			System::Windows::Controls::ListBoxItem^l_pSelectedItem = gcnew System::Windows::Controls::ListBoxItem();
			l_pSelectedItem->Content = l_str;
			//m_pEventListBox->SelectionMode = System::Windows::Controls::SelectionMode::Single;
			m_pEventListBox->Items->Add(l_pSelectedItem);
			m_pEventListHashtable[l_pSelectedItem] = l_pUserControl;
			if( l_pUIImplementClass )
				m_pEventWithImplementHashtable[l_pSelectedItem] = l_pUIImplementClass;
			m_pWrapTabControl->SelectedIndex = 1;
		}
	}

	System::Void cEventInstance_Editor::EventInit(System::Object^  sender, System::EventArgs^  e)
	{
		if(sFMBookEditorData::m_spEventInstance)
			sFMBookEditorData::m_spEventInstance->Init();
	}

	System::Void cEventInstance_Editor::TriggerEventListboxDoubleClick(System::Object^  sender, System::EventArgs^  e)
	{
		sFMBookEditorData::DataLegalCheck();
		System::Windows::Controls::ListBox^l_pListBox = (System::Windows::Controls::ListBox^)sender;
		System::Windows::Controls::ListBoxItem^l_pItem = (System::Windows::Controls::ListBoxItem^)l_pListBox->SelectedItem;
		if( !l_pItem )
			return;
		String^l_str = l_pItem->Content->ToString();
		System::Windows::Controls::UserControl^l_pUserControl;
		System::Object^l_pUIImplementClass;
		if(l_str->Equals("Mouse"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventMouseTrigger();
			//CultureInfo::GetCultureInfo("zh-CN");
			SpeechSynthesizer^ synthesizer = gcnew SpeechSynthesizer();
			System::Collections::ObjectModel::ReadOnlyCollection<InstalledVoice^>^ l_pp = synthesizer->GetInstalledVoices(System::Globalization::CultureInfo::GetCultureInfo("zh-CN"));
			synthesizer->Volume = 100;  // 0...100
			synthesizer->Rate = 0;     // -10...10
 
			// Synchronous
			//synthesizer->Speak("Mouse Trigger Event added");
 
			// Asynchronous
			//synthesizer->SpeakAsync("Welcome to the WPF Tutorial.");
			if( l_pp->Count )
			{
				synthesizer->SelectVoice(l_pp[0]->VoiceInfo->Name);
				synthesizer->SpeakAsync("滑鼠事件增加");
			}
			else
			{
				synthesizer->SpeakAsync("Welcome to the WPF Tutorial.");
			}
		}
		else
		if(l_str->Equals("ObjectStatusTrigger"))
		{
			//FMBookEditorUI::Event::Trigger::ObjectStatusTrigger^l_pObjectStatusTrigger = gcnew FMBookEditorUI::Event::Trigger::ObjectStatusTrigger();
			//l_pUserControl = l_pObjectStatusTrigger);
			//cObjectStatusTrigger^l_p = gcnew cObjectStatusTrigger(m_pFMBookUI);
			//l_p->SetObjectStatusTrigger(l_pObjectStatusTrigger);
		}
		else
		if(l_str->Equals("EventButtonTrigger"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventButtonTrigger();
			l_pUIImplementClass = gcnew EventButtonTrigger_Editor((FMBookEditorUI::Event::Trigger::EventButtonTrigger^)l_pUserControl);
		}
		else
		if(l_str->Equals("EventKeyUpDectectTrigger"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventKeyUpDectectTrigger();
		}
		else
		if(l_str->Equals("EventMultiButtonTrigger"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventMultiButtonTrigger();
			l_pUIImplementClass = gcnew EventMultiButtonTrigger_Editor((FMBookEditorUI::Event::Trigger::EventMultiButtonTrigger^)l_pUserControl);
		}
		else
		if(l_str->Equals("EventObjectStatusCheckerTrigger"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventObjectStatusCheckerTrigger();
		}
		else
		if(l_str->Equals("EventSelectionCheckerTrigger"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventSelectionCheckerTrigger();
		}
		else
		if(l_str->Equals("EventValueCheckerTrigger"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventValueCheckerTrigger();
		}
		else
		if(l_str->Equals("EventMouseTrigger"))
		{
			l_pUserControl = gcnew FMBookEditorUI::Event::Trigger::EventMouseTrigger();
		}
		else
		{
			WARNING_MSG(l_str+" doest not supprt for now");
		}
		if( l_pUserControl )
		{
			m_pTriggerEventWrapPanel->Children->Add(l_pUserControl);
			System::Windows::Controls::ListBoxItem^l_pSelectedItem = gcnew System::Windows::Controls::ListBoxItem();
			l_pSelectedItem->Content = l_str;
			//m_pEventListBox->SelectionMode = System::Windows::Controls::SelectionMode::Single;
			m_pEventListBox->Items->Add(l_pSelectedItem);
			m_pEventListHashtable[l_pSelectedItem] = l_pUserControl;
			if( l_pUIImplementClass )
				m_pEventWithImplementHashtable[l_pSelectedItem] = l_pUIImplementClass;
			m_pWrapTabControl->SelectedIndex = 0;
		}
	}


	void		cEventInstance_Editor::Clear()
	{
		m_pEventListBox->Items->Clear();
		m_pEventListHashtable->Clear();
		for each( System::Object^l_pKey in m_pEventWithImplementHashtable->Keys )
		{
			System::Object^l_pValue = m_pEventWithImplementHashtable[l_pKey];
			delete l_pValue;
		}
		m_pEventWithImplementHashtable->Clear();
		if( sFMBookEditorData::m_spEventInstance )
		{
			sFMBookEditorData::m_spEventInstance->m_TriggerEventList.Destroy();
			sFMBookEditorData::m_spEventInstance->m_ActiveEventList.Destroy();
		}
		m_pActiverEventWrapPanel->Children->Clear();
		m_pTriggerEventWrapPanel->Children->Clear();
		m_pEventListBox->Items->Clear();
	}

	bool		cEventInstance_Editor::IsLegal()
	{
		String^l_str = gcnew String("");
		for each( EventBase_Editor^l_pEventBase_Editor in m_pEventWithImplementHashtable->Values )
		{
			String^l_strErrorMsg = l_pEventBase_Editor->IsLegal();
			if( l_strErrorMsg )
			{
				l_str += l_pEventBase_Editor->ToString();
				l_str += "\t";
				l_str += l_strErrorMsg;
				l_str += "\n";
			}
		}
		if(l_str->Length)
		{
			WARNING_MSG(l_str);
			return false;
		}
		return true;
	}
}