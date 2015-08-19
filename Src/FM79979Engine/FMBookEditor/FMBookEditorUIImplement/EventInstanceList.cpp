#include "StdAfx.h"
#include "EventInstanceList.h"
#include "EventRevertFunction.h"

namespace FMBookEditor
{
		EventInstanceList_Editor::EventInstanceList_Editor(FMBookEditorUI::FMBookUI^e_pUserControl1,cEventInstance_Editor^e_pEventSelector)
		{
			m_pEventObjectInstanceList = e_pUserControl1->m_EventObjectInstanceList;
			m_pEventInstanceEditor = e_pEventSelector;
			m_pEventInstanceList = e_pUserControl1->m_EventInstanceList;
			m_pEventInstanceList->m_EventList_listBox->m_ListBox->SelectionChanged += gcnew System::Windows::Controls::SelectionChangedEventHandler(this,&EventInstanceList_Editor::SelectionChanged);
			m_pEventInstanceList->f_Add += gcnew EventHandler(this,&EventInstanceList_Editor::Add);
			//m_pEventInstanceList->f_Del += gcnew EventHandler(this,&EventInstanceList_Editor::Del);
			m_pEventInstanceList->m_EventList_listBox->f_DeleteSelectedItem += gcnew EventHandler(this,&EventInstanceList_Editor::Del);
			m_pEventInstanceList->f_Revert += gcnew EventHandler(this,&EventInstanceList_Editor::Revert);
		}

		EventInstanceList_Editor::~EventInstanceList_Editor()
		{
		}

		System::Void EventInstanceList_Editor::Revert()
		{
			m_pEventInstanceEditor->Clear();
			String^l_strName = m_pEventInstanceList->m_EventList_listBox->SelectedName();
			if( l_strName )
			{
				cEventInstance*l_pEventInstance = g_pEditorMode->m_spEventManager->GetEventInstance(DNCT::GcStringToWchar(l_strName));
				if( !l_pEventInstance )
				{
					WARNING_MSG(l_strName+ " not exist!");
					return;
				}
				EventInstanceRevert(l_pEventInstance,m_pEventInstanceEditor);
			}
		}
		System::Void EventInstanceList_Editor::Revert(System::Object^  sender, System::EventArgs^  e)
		{
			Revert();
		}

		System::Void EventInstanceList_Editor::SelectionChanged(System::Object^  sender, System::Windows::Controls::SelectionChangedEventArgs^  e)
		{
			String^l_strName = m_pEventInstanceList->m_EventList_listBox->SelectedName();
			if(l_strName)
			{
				m_pEventObjectInstanceList->m_EventInstance_textBox->Text = l_strName;
			}
		}

		System::Void EventInstanceList_Editor::Add(System::Object^  sender, System::EventArgs^  e)
		{
			if(m_pEventInstanceList->m_EventInstanceListName_textBox->Text->Length>0)
			{
				if( sFMBookEditorData::m_spEventInstance->m_TriggerEventList.Count()+sFMBookEditorData::m_spEventInstance->m_ActiveEventList.Count() == 0 )
				{
					WARNING_MSG("there is no event to add");
					return;
				}
				if(this->m_pEventInstanceEditor->IsLegal())
				{
					if(m_pEventInstanceList->m_EventList_listBox->AddListBoxItem(m_pEventInstanceList->m_EventInstanceListName_textBox->Text))
					{
						//fuck copy button crash!.
						cEventInstance*l_pEventInstance = sFMBookEditorData::m_spEventInstance->Clone();
						l_pEventInstance->SetName(DNCT::GcStringToWchar(m_pEventInstanceList->m_EventInstanceListName_textBox->Text));
						g_pEditorMode->m_spEventManager->m_pEventInstanceList->AddObject(l_pEventInstance);
						m_pEventInstanceEditor->Clear();
						//TiXmlElement*l_pTiXmlement = sFMBookEditorData::m_spEventInstance->ToTiXmlElement();
						//l_pTiXmlement;
					}
				}
			}
		}

		System::Void EventInstanceList_Editor::DelListBoxItem(System::Object^  sender, System::EventArgs^  e)
		{
			System::Windows::Controls::TextBox^l_pTextBox = dynamic_cast<System::Windows::Controls::TextBox^>(sender);
			if( l_pTextBox )
			{
				String^l_str = l_pTextBox->Text;
				g_pEditorMode->m_spEventManager->m_pEventInstanceList->RemoveObject(DNCT::GcStringToWchar(l_pTextBox->Text));		
			}
		}

		System::Void EventInstanceList_Editor::Del(System::Object^  sender, System::EventArgs^  e)
		{
			DelListBoxItem(sender,e);
		}

		void	 EventInstanceList_Editor::Refresh()
		{
			m_pEventInstanceList->m_EventList_listBox->m_ListBox->Items->Clear();
			cEventManager*l_pEventManager = g_pEditorMode->m_spEventManager;
			cNodeISAX*l_pNodeISAX = l_pEventManager->m_pEventInstanceXml;
			if( !l_pNodeISAX )
				return;
			TiXmlElement*l_pTiXmlElement = l_pNodeISAX->GetRootElement();//->ChildElementCount()
			if( l_pTiXmlElement )
			{
				l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
				while(l_pTiXmlElement)
				{
					const WCHAR*l_strName = l_pTiXmlElement->Attribute(L"Name");
					m_pEventInstanceList->m_EventList_listBox->AddListBoxItem(DNCT::WcharToGcstring(l_strName));
					l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
				}
			}
			//l_pNodeISAX
		}
//end namespace FMBookEditor
}