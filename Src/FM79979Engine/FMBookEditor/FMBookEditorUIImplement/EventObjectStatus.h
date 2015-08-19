#pragma once

#include "../FatmingUtilities.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMBookEditor 
{
	public ref class EventObjectStatus
	{
	public:
		EventObjectStatus(FMBookEditorUI::FMBookUI^e_pUserControl1,cFatmingUtilitiesData^e_pFatmingUtilitiesData)
		{
			m_pFatmingUtilitiesData = e_pFatmingUtilitiesData;
			m_pCurrentSelectObject = 0;
			m_pEditorEventObjectStatu = 0;
			m_pFMBookUI = e_pUserControl1;
			FMBookEditorUI::EventObjectStatus^l_pStatusUI = e_pUserControl1->m_EventObjectStatus;
			m_pFatmingUtilities = e_pUserControl1->m_FatmingUtilities;
			m_pEventObjectStatus = e_pUserControl1->m_EventObjectStatus;
			m_pEventObjectStatusList = e_pUserControl1->m_EventObjectStatusList;
			m_pStatuslistBox = l_pStatusUI->m_Status_listBox;
			m_pStatusNameTextBox = l_pStatusUI->m_StatusName_textBox;
			m_pShowObjectTextBox = l_pStatusUI->m_ShowObject_TextBox;

			m_pTabControl = e_pUserControl1->m_FatmingUtilities->m_TabControl;

			m_pEventObjectStatus->f_ApplyTestButton_Click		+= gcnew EventHandler(this,&EventObjectStatus::StatusApplyTest_button_Click);
			m_pEventObjectStatus->f_AddStatus					+= gcnew EventHandler(this,&EventObjectStatus::AddEventObjectStatus_button_Click);
			m_pEventObjectStatus->f_Revert						+= gcnew EventHandler(this,&EventObjectStatus::RevertEventObjectStatus_button_Click);
			m_pEventObjectStatus->f_Del							+= gcnew EventHandler(this,&EventObjectStatus::DeleteEventObjectStatus_button_Click);

			//m_pEventObjectStatusList->f_Add						+= gcnew EventHandler(this,&EventObjectStatus::AddEventObjectStatusList_button_Click);
			//m_pEventObjectStatusList->f_Del						+= gcnew EventHandler(this,&EventObjectStatus::DeleteEventObjectStatusList_button_Click);
			//m_pEventObjectStatusList->f_Revert					+= gcnew EventHandler(this,&EventObjectStatus::RevertEventObjectStatusList_button_Click);

			m_pEventObjectStatus->f_TabPageControlSelectChange	+= gcnew EventHandler(this,&EventObjectStatus::TabControlSelectionChanged);
			m_pFatmingUtilities->f_TabControlSelectionChanged	+= gcnew EventHandler(this,&EventObjectStatus::TabControlSelectionChanged);
			m_pFatmingUtilitiesData->m_pXMLText->TextChanged	+= gcnew EventHandler(this,&EventObjectStatus::TabControlSelectionChanged);

			this->m_pEventObjectStatus->m_TextText->TextChanged += gcnew System::Windows::Controls::TextChangedEventHandler(this,&EventObjectStatus::TextStatusTextChanged);

			m_eEventObjectType = eEOT_MAX;

			this->m_pEventObjectStatus->m_ImagePosXYZNumeric->SetMinimum(-9999999.f);
			this->m_pEventObjectStatus->m_ImagePosXYZNumeric->SetMaximum(9999999.f);
			this->m_pEventObjectStatus->m_ImageSizeXYNumeric->SetMinimum(0.f);
			this->m_pEventObjectStatus->m_ImageSizeXYNumeric->SetMaximum(99999.f);
		}
	protected:
		~EventObjectStatus();
    //my
		cFatmingUtilitiesData^m_pFatmingUtilitiesData;
		eEventObjectType	m_eEventObjectType;
		NamedTypedObject*m_pCurrentSelectObject;
		System::Collections::Generic::List<System::Windows::Controls::ListBox^>^ m_pListData;;
		FMBookEditorUI::FMBookUI^							m_pFMBookUI;
		FMBookEditorUI::EventObjectStatus^					m_pEventObjectStatus;
		FMBookEditorUI::EventObjectStatusList^				m_pEventObjectStatusList;
		//store temp EventObjectStatus
		public: cEventObjectStatus*							m_pEditorEventObjectStatu;
		System::Windows::Controls::TextBox^					m_pStatusNameTextBox;
		System::Windows::Controls::ListBox^					m_pStatuslistBox;

		CoreWPF::FMC::FatmingUtilities^						m_pFatmingUtilities;
		System::Windows::Controls::TabControl^				m_pTabControl;
		private: System::Windows::Controls::TextBox^		m_pShowObjectTextBox;
		//
		private: System::Void StatusApplyTest_button_Click(System::Object^  sender, System::EventArgs ^  e);
		private: System::Void AddEventObjectStatus_button_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void DeleteEventObjectStatus_button_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void RevertEventObjectStatus_button_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void EventObjectStatus_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
		public:  System::Void TabControlSelectionChanged(System::Object^  sender, System::EventArgs^  e);
		//
		public:  System::Void TextStatusTextChanged(System::Object^  sender, System::Windows::Controls::TextChangedEventArgs ^  e);
		//
		public:  System::Void UICombineRemove();
		public:  System::Void UICombine(cEventObject*e_pEventObject);
		//public:	 void	 FileOpen();
	};
}
