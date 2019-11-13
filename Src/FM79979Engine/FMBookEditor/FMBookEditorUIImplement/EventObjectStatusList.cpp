#include "StdAfx.h"
#include "EventObjectStatusList.h"

namespace FMBookEditor
{

	System::Void EventObjectStatusList_Editor::Revert(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iSelectedIndex = m_pEventObjectStatusList->m_EventObjectStatusList_listBox->SelectedIndex;
		if( l_iSelectedIndex == -1 )
			return;
		m_pEventObjectStatus->m_Status_listBox->Items->Clear();
		m_pEditorEventObjectStatus->m_pEditorEventObjectStatu = g_pEditorMode->m_spEventManager->m_pEventObjectStatusList->GetObject(l_iSelectedIndex);
		for( int i=0;i<m_pEditorEventObjectStatus->m_pEditorEventObjectStatu->Count();++i )
			m_pEventObjectStatus->m_Status_listBox->Items->Add(DNCT::WcharToGcstring(m_pEditorEventObjectStatus->m_pEditorEventObjectStatu->GetObject(i)->GetName()));	

	}

	System::Void EventObjectStatusList_Editor::Add(System::Object^  sender, System::EventArgs^  e)
	{
		if( !m_pEditorEventObjectStatus )
		{
			return ;
		}
		if( m_pEventObjectStatusList->m_EventObjectStatusName_textBox->Text->Length == 0)
			return;
		auto l_strName = DNCT::GcStringToWchar(m_pEventObjectStatusList->m_EventObjectStatusName_textBox->Text);
		cEventObjectStatus*l_pEventObjectStatus = g_pEditorMode->m_spEventManager->m_pEventObjectStatusList->GetObject(l_strName.c_str());
		if( l_pEventObjectStatus )
		{
			WARNING_MSG("status already exist!");
			return;
		}
		else
		{
			m_pEditorEventObjectStatus->m_pEditorEventObjectStatu->SetName(l_strName.c_str());
			g_pEditorMode->m_spEventManager->m_pEventObjectStatusList->AddObject(m_pEditorEventObjectStatus->m_pEditorEventObjectStatu);
			m_pEditorEventObjectStatus->m_pEditorEventObjectStatu = 0;
			SAFE_DELETE(g_pEditorEventObject);
			m_pEventObjectStatusList->m_EventObjectStatusList_listBox->Items->Add(m_pEventObjectStatusList->m_EventObjectStatusName_textBox->Text);
			m_pEventObjectStatus->m_Status_listBox->Items->Clear();
		}
	}

	System::Void EventObjectStatusList_Editor::Del(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iSelectedIndex = m_pEventObjectStatusList->m_EventObjectStatusList_listBox->SelectedIndex;
		if( l_iSelectedIndex == -1 )
			return;
		g_pEditorMode->m_spEventManager->m_pEventObjectStatusList->RemoveObject(l_iSelectedIndex);
		m_pEventObjectStatusList->m_EventObjectStatusList_listBox->Items->RemoveAt(l_iSelectedIndex);	
	}

	void	 EventObjectStatusList_Editor::Refresh()
	{
		m_pEventObjectStatusList->m_EventObjectStatusList_listBox->Items->Clear();
		cEventManager*l_pEventManager = g_pEditorMode->m_spEventManager;
		cNodeISAX*l_pNodeISAX = l_pEventManager->m_pEventObjectStatusXml;
		if( !l_pNodeISAX )
			return;
		TiXmlElement*l_pTiXmlElement = l_pNodeISAX->GetRootElement();//->ChildElementCount()
		if( l_pTiXmlElement )
		{
			l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
			while(l_pTiXmlElement)
			{
				const wchar_t*l_strName = l_pTiXmlElement->Attribute(L"Name");
				m_pEventObjectStatusList->m_EventObjectStatusList_listBox->Items->Add(DNCT::WcharToGcstring(l_strName));
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
			}
		}
		//l_pNodeISAX
	}

	System::Void EventObjectStatusList_Editor::SelectionChanged(System::Object^  sender, System::Windows::Controls::SelectionChangedEventArgs^  e)
	{
		if(this->m_pEventObjectStatusList->m_EventObjectStatusList_listBox->SelectedItem)
		{
			m_pEventObjectInstanceList->m_EventStatus_textBox->Text = this->m_pEventObjectStatusList->m_EventObjectStatusList_listBox->SelectedItem->ToString();
		}
	}
}