#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace System::Speech::Synthesis;


namespace FMBookEditor
{

	public ref class cObjectStatusTrigger
	{
public:
		cObjectStatusTrigger(FMBookEditorUI::FMBookUI^e_pUserControl1)
		{
			m_pFMBookUI = e_pUserControl1;
			m_pEventPackages = m_pFMBookUI->m_EventPackages;
			//m_pEventPackages->f_EventObjectInstanceInvert_listBoxSelectionChanged += gcnew EventHandler(this,&cObjectStatusTrigger::Pages_listBoxSelectionChanged);
		}
		~cObjectStatusTrigger()
		{
		
		}
		//void	SetObjectStatusTrigger(FMBookEditorUI::Event::Trigger::ObjectStatusTrigger^e_pObjectStatusTrigger)
		//{
			//m_pObjectStatusTrigger = e_pObjectStatusTrigger;
			//if( m_pEventPackages->m_Packages_listBox->SelectedItem )
			//{
			//	System::Windows::Controls::ListBoxItem^l_pItemBoxItem = (System::Windows::Controls::ListBoxItem^)m_pEventPackages->m_Packages_listBox->SelectedItem;
			//	m_pObjectStatusTrigger->m_ObjectStatusName_Textbox->Text = l_pItemBoxItem->Content->ToString();
			//}
			//else
			//	m_pObjectStatusTrigger->m_ObjectStatusName_Textbox->Text = "";
		//}
		//private: System::Void Pages_listBoxSelectionChanged(System::Object^sender, System::EventArgs^ e)
		//{
		//	System::Windows::Controls::ListBox^l_pListBox = (System::Windows::Controls::ListBox^)sender;
		//	if( l_pListBox->SelectedItem )
		//	{
		//		System::Windows::Controls::ListBoxItem^l_pItemBoxItem = (System::Windows::Controls::ListBoxItem^)l_pListBox->SelectedItem;
		//		m_pObjectStatusTrigger->m_ObjectStatusName_Textbox->Text = l_pItemBoxItem->Content->ToString();
		//	}
		//	else
		//		m_pObjectStatusTrigger->m_ObjectStatusName_Textbox->Text = "";

		//}
		FMBookEditorUI::EventPackages^m_pEventPackages;
		//FMBookEditorUI::Event::Trigger::ObjectStatusTrigger^m_pObjectStatusTrigger;
		FMBookEditorUI::FMBookUI^m_pFMBookUI;
		//end FMBookEditor
	};
//end FMBookEditor
}