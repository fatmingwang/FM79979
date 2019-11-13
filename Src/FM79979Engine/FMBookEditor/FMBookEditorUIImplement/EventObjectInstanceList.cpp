#include "StdAfx.h"
#include "EventObjectInstanceList.h"
namespace FMBookEditor
{

		//System::Void cEventObjectInstance_Editor::Revert(System::Object^  sender, System::EventArgs^  e)
		//{
		//	if( m_EventObjectInstance_listBox->SelectedIndex != -1 )
		//	{
		//		FMBookEditorUI::EventObjectInstanceContentForListBoxUI^l_pEventObjectInstanceContentForListBoxUI = (FMBookEditorUI::EventObjectInstanceContentForListBoxUI^)m_EventObjectInstance_listBox->SelectedItem;
		//		WCHAR*l_strName = DNCT::GcStringToWchar(l_pEventObjectInstanceContentForListBoxUI->m_EventObjecInstanceName->Text);
		//		cEventObjectInstance*l_pEventObjectInstance = g_pEditorMode->m_spEventPackageRunner->GetCurrentEventPackage()->GetObject(l_strName);
		//		m_pEventObjectInstanceName->Text = l_pEventObjectInstanceContentForListBoxUI->m_EventObjecInstanceName->Text;
		//		String^l_strEventInstanceName = DNCT::WcharToGcstring(l_pEventObjectInstance->GetEventInstanceList()->GetObject(0)->GetName());
		//		if(l_pEventObjectInstance->GetEventInstanceList()->Count() >= 1)
		//			this->m_pEventInstanceName->Text = l_strEventInstanceName;
		//		else
		//			this->m_pEventInstanceName->Text = "";
		//		if(l_pEventObjectInstance->GetEventObjectStatus())
		//			this->m_pEventObjectStatusName->Text = DNCT::WcharToGcstring(l_pEventObjectInstance->GetEventObjectStatus()->GetName());
		//		else
		//			this->m_pEventObjectStatusName->Text = "";
		//		if(l_pEventObjectInstance->GetStatusEventVariable())
		//			this->m_pEventObjectStatusWorkingIndexByEventVariableName->Text = DNCT::WcharToGcstring(l_pEventObjectInstance->GetStatusEventVariable()->GetName());
		//		else
		//			this->m_pEventObjectStatusWorkingIndexByEventVariableName->Text = "";
		//		int	l_iIndex = m_pEventInstanceList_Editor->m_pEventInstanceList->m_EventList_listBox->IndexOf(l_strEventInstanceName);
		//		if( l_iIndex != -1 )
		//		{
		//			m_pEventInstanceList_Editor->m_pEventInstanceList->m_EventList_listBox->m_ListBox->SelectedIndex = l_iIndex;
		//		}
		//		this->m_pEventInstanceList_Editor->Revert();
		//		
		//		//eventobjectstatus data assign.
		//		//eventinstance data assign.
		//		//g_pEditorMode->m_spEventManager->GetCurrentPackage()->GetObjectm_EventObjectInstance_listBox->SelectedIndex
		//	}
		//}

		System::Void cEventObjectInstance_Editor::Add(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pEventObjectInstanceName->Text->Length == 0 )
			{
				WARNING_MSG("please fill EventObjectInstanceName's name");
				return;
			}
			if( g_pEditorMode->m_spEventPackageRunner->GetCurrentEventPackage()->GetObject(DNCT::GcStringToWchar(m_pEventObjectInstanceName->Text)) )
			//if( m_EventObjectInstance_listBox->IsContain(m_pEventObjectInstanceName->Text) )
			{
				WARNING_MSG("name exists");
				return;
			}

			bool	l_bAllStatusWorking = (bool)m_pAllStatusWorkingAtSameTime_CheckBox->IsChecked;
			TiXmlElement*l_pEventObjectStatusElement = 0;
			TiXmlElement*l_pEventInstanceElement = 0;
			cEventObjectStatus*l_pEventObjectStatus = 0;
			cEventInstance*l_pEventInstance = 0;
			if(m_pEventObjectStatusName->Text->Length)
			{
				auto l_strName = DNCT::GcStringToWchar(m_pEventObjectStatusName->Text);
				l_pEventObjectStatus = g_pEditorMode->m_spEventManager->m_pEventObjectStatusList->GetObject(l_strName);
				if( !l_pEventObjectStatus )
				{
					WARNING_MSG("there is no "+m_pEventObjectStatusName->Text+" Event Object status");
					return;
				}
			}

			if(m_pEventInstanceName->Text->Length)
			{
				auto l_strEventInstance = DNCT::GcStringToWchar(m_pEventInstanceName->Text);
				l_pEventInstance = g_pEditorMode->m_spEventManager->GetEventInstance(l_strEventInstance.c_str());
				if( l_pEventInstance)
				{
					WARNING_MSG("there is no "+m_pEventInstanceName->Text+" cEventInstance");
					return;
				}
			}
			if( !l_pEventObjectStatus && !l_pEventInstance )
			{
					WARNING_MSG("there is no cEventInstance or Event Object status");
					return;
			}
			cEventObjectInstance*l_pEventObjectInstance = new cEventObjectInstance();
			g_pEditorMode->m_spEventPackageRunner->GetCurrentEventPackage()->AddObject(l_pEventObjectInstance);
			l_pEventObjectInstance->SetName(DNCT::GcStringToWchar(m_pEventObjectInstanceName->Text));

			//m_EventObjectInstance_listBox->AddListBoxItem(m_pEventObjectInstanceName->Text);
			//m_pEventObjectInstanceName->Text
			FMBookEditorUI::EventObjectInstanceContentForListBoxUI^l_pEventObjectInstanceContentForListBoxUI = gcnew FMBookEditorUI::EventObjectInstanceContentForListBoxUI();
			l_pEventObjectInstanceContentForListBoxUI->m_EventObjecInstanceName->Text = m_pEventObjectInstanceName->Text;
			l_pEventObjectInstanceContentForListBoxUI->m_EventInstance->Text = m_pEventInstanceName->Text;
			l_pEventObjectInstanceContentForListBoxUI->m_EventObjecStatus->Text = m_pEventObjectStatusName->Text;
			m_EventObjectInstance_listBox->Items->Add(l_pEventObjectInstanceContentForListBoxUI);
			if( l_pEventObjectStatus )
			{
				l_pEventObjectInstance->SetTargetEventObjectStatus(l_pEventObjectStatus);
				//l_pEventObjectStatusElement = l_pEventObjectStatus->ToTiXmlElement();
			}
			if( l_pEventInstance )
			{
				//l_pEventInstanceElement = l_pEventInstance->ToTiXmlElement();
				l_pEventObjectInstance->AddEventInstance(l_pEventInstance);
			}
			
			if(m_pEventObjectStatusWorkingIndexByEventVariableName->Text->Length)
			{
			
			}
		}

		System::Void cEventObjectInstance_Editor::Del(System::Object^  sender, System::EventArgs^  e)
		{
		
		}

		System::Void cEventObjectInstance_Editor::SelectedChanged(System::Object^  sender, System::EventArgs^  e)
		{

			//if(m_EventObjectInstance_listBox->SelectedName())
			{
			
			}
		}

		void	 cEventObjectInstance_Editor::Refresh()
		{
			m_pEventObjectInstanceList->m_EventObjectInstance_listBox->Items->Clear();
		}
//end namespace FMBookEditor
}