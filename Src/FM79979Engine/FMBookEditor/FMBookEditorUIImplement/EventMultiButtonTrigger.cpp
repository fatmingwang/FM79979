#include "StdAfx.h"
#include "EventMultiButtonTrigger.h"
namespace FMBookEditor
{
		EventMultiButtonTrigger_Editor::EventMultiButtonTrigger_Editor(FMBookEditorUI::Event::Trigger::EventMultiButtonTrigger^e_pEventMultiButtonTrigger)
		{
			m_pEventMultiButtonTrigger = e_pEventMultiButtonTrigger;
			m_pEventMultiButtonTrigger->f_Add += gcnew EventHandler(this,&EventMultiButtonTrigger_Editor::Add);
			m_pEventMultiButtonTrigger->f_Del += gcnew EventHandler(this,&EventMultiButtonTrigger_Editor::Del);
			m_pEventMultiButtonTrigger->f_Revert += gcnew EventHandler(this,&EventMultiButtonTrigger_Editor::Revert);
			m_pEventMultiButton = new cEventMultiButton();
			sFMBookEditorData::m_spEventInstance->m_TriggerEventList.AddObjectNeglectExist(m_pEventMultiButton);
			//
			m_pCurrentEventButtonTrigger_Editor = gcnew EventButtonTrigger_Editor(m_pEventMultiButtonTrigger->m_Button);
		}

		EventMultiButtonTrigger_Editor::EventMultiButtonTrigger_Editor(FMBookEditorUI::Event::Trigger::EventMultiButtonTrigger^e_pEventMultiButtonTrigger,cEventMultiButton*e_pEventMultiButton)
		{
			m_pEventMultiButtonTrigger = e_pEventMultiButtonTrigger;
			m_pEventMultiButtonTrigger->f_Add += gcnew EventHandler(this,&EventMultiButtonTrigger_Editor::Add);
			m_pEventMultiButtonTrigger->f_Del += gcnew EventHandler(this,&EventMultiButtonTrigger_Editor::Del);
			m_pEventMultiButtonTrigger->f_Revert += gcnew EventHandler(this,&EventMultiButtonTrigger_Editor::Revert);
			m_pEventMultiButton = dynamic_cast<cEventMultiButton*>(e_pEventMultiButton->Clone());
			sFMBookEditorData::m_spEventInstance->m_TriggerEventList.AddObjectNeglectExist(m_pEventMultiButton);
			//
			m_pCurrentEventButtonTrigger_Editor = gcnew EventButtonTrigger_Editor(m_pEventMultiButtonTrigger->m_Button);
			int	l_iCount = m_pEventMultiButton->Count();
			for( int i =0;i<l_iCount;++i )
			{
				this->m_pEventMultiButtonTrigger->m_ButtonList->Items->Add(DNCT::WcharToGcstring(m_pEventMultiButton->GetObject(i)->GetName()));
			}
		}

		EventMultiButtonTrigger_Editor::~EventMultiButtonTrigger_Editor()
		{
			int	l_iIndex = sFMBookEditorData::m_spEventInstance->m_TriggerEventList.GetObjectIndexByPointer(m_pEventMultiButton);
			if( l_iIndex != -1 )
			{
				sFMBookEditorData::m_spEventInstance->m_TriggerEventList.RemoveObject(l_iIndex);
			}
			else
			{
				SAFE_DELETE(m_pEventMultiButton);
			}
			delete m_pCurrentEventButtonTrigger_Editor;
		}


		System::Void EventMultiButtonTrigger_Editor::Revert(System::Object^  sender, System::EventArgs^  e)
		{
			if( this->m_pEventMultiButtonTrigger->m_ButtonList->SelectedIndex != -1 )
			{
				cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(m_pEventMultiButton->GetObject(this->m_pEventMultiButtonTrigger->m_ButtonList->SelectedIndex));
				if( !l_pEventButton )
				{
					WARNING_MSG(this->m_pEventMultiButtonTrigger->m_ButtonList->Items[this->m_pEventMultiButtonTrigger->m_ButtonList->SelectedIndex]->ToString()+" not exist! ");
				}
				else
				{
					m_pCurrentEventButtonTrigger_Editor->SetData(l_pEventButton,false);
					l_pEventButton->SetName(l_pEventButton->GetName());
				}
			}
		}

		System::Void EventMultiButtonTrigger_Editor::Add(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pEventMultiButtonTrigger->m_ButtonName->Text->Length == 0 )
			{
				WARNING_MSG("please fill Button's name");
				return;
			}
			String^l_strErrorMsg = m_pCurrentEventButtonTrigger_Editor->IsLegal();
			if(!l_strErrorMsg)
			{
				//m_pCurrentEventButtonTrigger_Editor->m_pButton
				//m_pEventMultiButton->AddObject();
				auto l_strEventName = DNCT::GcStringToWchar(m_pEventMultiButtonTrigger->m_ButtonName->Text);
				if(this->m_pEventMultiButton->GetObject(l_strEventName.c_str()))
				{
					WARING_YES_NO_TO_NO(m_pEventMultiButtonTrigger->m_ButtonName->Text+"this name exits")
						return;
					int	l_iIndex = m_pEventMultiButton->GetObjectIndexByName(l_strEventName.c_str());
					m_pEventMultiButton->RemoveObject(l_strEventName);
					m_pEventMultiButtonTrigger->m_ButtonList->Items->RemoveAt(l_iIndex);
				}
				this->m_pEventMultiButtonTrigger->m_ButtonList->Items->Add(m_pEventMultiButtonTrigger->m_ButtonName->Text);
				cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(m_pCurrentEventButtonTrigger_Editor->m_pButton->Clone());
				l_pEventButton->SetName(l_strEventName);
				m_pEventMultiButton->AddObject(l_pEventButton);
			}
			else
			{
				WARNING_MSG(l_strErrorMsg);
			}
		}

		System::Void EventMultiButtonTrigger_Editor::Del(System::Object^  sender, System::EventArgs^  e)
		{
		
		}
//end namespace FMBookEditor
}