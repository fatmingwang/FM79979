#include "StdAfx.h"
#include "EventPackages.h"

namespace FMBookEditor
{

	System::Void EventPackages_Editor::Add(System::Object^  sender, System::EventArgs^  e)
	{
		if(this->m_pEventPackages->m_PackageName_textBox->Text->Length)
		{
			if(!m_pEventPackages->m_Packages_listBox->Items->Contains(this->m_pEventPackages->m_PackageName_textBox->Text))
			{
				m_pEventPackages->m_Packages_listBox->Items->Add(this->m_pEventPackages->m_PackageName_textBox->Text);
				sFMBookEditorData::AddEventPackage(sFMBookEditorData::m_spEventPackage);
			}
		}
	}

	System::Void EventPackages_Editor::Del(System::Object^  sender, System::EventArgs^  e)
	{
		if( this->m_pEventPackages->m_Packages_listBox->SelectedIndex != -1)
		{
			String^l_strName = (String^)this->m_pEventPackages->m_Packages_listBox->SelectedItem;
			WCHAR*l_strDeleteName = DNCT::GcStringToWchar(l_strName);
			sFMBookEditorData::DelEventPackage(l_strDeleteName);
			this->m_pEventPackages->m_Packages_listBox->Items->RemoveAt(this->m_pEventPackages->m_Packages_listBox->SelectedIndex);
		}	
	}

	System::Void EventPackages_Editor::Revert(System::Object^  sender, System::EventArgs^  e)
	{
		if(m_pEventPackages->m_Packages_listBox->SelectedIndex != -1)
		{
			//m_pFMBookUI->m_EventObjectInstanceList->m_EventObjectInstance_listBox->m_ListBox->Items->Clear();
			m_pFMBookUI->m_EventObjectInstanceList->m_EventObjectInstance_listBox->Items->Clear();
			WCHAR*	l_strPackageName = DNCT::GcStringToWchar(m_pEventPackages->m_Packages_listBox->SelectedItem->ToString());
			cEventPackage*l_pEventPackage = g_pEditorMode->m_spEventManager->GetEventPackage(l_strPackageName);
			cEventObjectInstance*l_pEventObjectInstance = 0;
			if(l_pEventPackage)
			{
				g_pEditorMode->m_spEventPackageRunner->SetCurrentEventPackage(l_pEventPackage);
				l_pEventPackage->Init();
				sFMBookEditorData::DestroyResource();
				FMBookEditorUI::EventObjectInstanceList^l_pEventObjectInstanceLis = m_pFMBookUI->m_EventObjectInstanceList;
				if( l_pEventPackage )
				{
					for( int i=0;i<l_pEventPackage->Count();++i )
					{
						l_pEventObjectInstance = l_pEventPackage->GetObject(i);
						FMBookEditorUI::EventObjectInstanceContentForListBoxUI^l_pEventObjectInstanceContentForListBoxUI = gcnew FMBookEditorUI::EventObjectInstanceContentForListBoxUI();
						l_pEventObjectInstanceContentForListBoxUI->m_EventObjecInstanceName->Text = DNCT::WcharToGcstring(l_pEventObjectInstance->GetName());
						if(l_pEventObjectInstance->GetEventObjectStatus())
							l_pEventObjectInstanceContentForListBoxUI->m_EventInstance->Text = DNCT::WcharToGcstring(l_pEventObjectInstance->GetEventObjectStatus()->GetName());
						if(l_pEventObjectInstance->GetEventInstanceList()->Count())
							l_pEventObjectInstanceContentForListBoxUI->m_EventObjecStatus->Text = DNCT::WcharToGcstring(l_pEventObjectInstance->GetEventInstanceList()->GetObject(0)->GetName());
						if(l_pEventObjectInstance->GetStatusEventVariable())
							l_pEventObjectInstanceContentForListBoxUI->m_StatusEventVariable->Text = DNCT::WcharToGcstring(l_pEventObjectInstance->GetStatusEventVariable()->GetName());
						//l_pEventObjectInstanceLis->m_EventObjectInstance_listBox->AddListBoxItem(DNCT::WcharToGcstring(l_pEventPackage->GetObject(i)->GetName()));
						//l_pEventObjectInstanceLis->m_EventObjectInstance_listBox->Items->Add(DNCT::WcharToGcstring(l_pEventPackage->GetObject(i)->GetName()));
						l_pEventObjectInstanceLis->m_EventObjectInstance_listBox->Items->Add(l_pEventObjectInstanceContentForListBoxUI);
					}
				}
			}
		}
	}

	void	 EventPackages_Editor::Refresh()
	{
		m_pEventPackages->m_Packages_listBox->Items->Clear();
		TiXmlElement*l_pTiXmlElement = g_pEditorMode->m_spEventManager->GetRootElement();//->ChildElementCount()
		if( l_pTiXmlElement )
		{
			l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
			while(l_pTiXmlElement)
			{
				const WCHAR*l_strName = l_pTiXmlElement->Attribute(L"Name");
				m_pEventPackages->m_Packages_listBox->Items->Add(DNCT::WcharToGcstring(l_strName));
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
			}
		}
	}
}