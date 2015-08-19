#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMBookEditor
{

	public ref class MPDI:public cFatmingUtilitiesBase
	{
	public:
		MPDI(CoreWPF::FMC::MPDI^e_pUserControl1)
		{
			e_pUserControl1->f_AddMPDI += gcnew EventHandler(this,&MPDI::AddMPDI_button_Click);
			e_pUserControl1->f_DelMPDI += gcnew EventHandler(this,&MPDI::DeleteMPDI_button_Click);
			e_pUserControl1->f_MPDIListChangwSelection += gcnew EventHandler(this,&MPDI::MPDIList_listBox_SelectedIndexChanged);
			e_pUserControl1->f_MPDIChangwSelection += gcnew EventHandler(this,&MPDI::MPDI_listBox_SelectedIndexChanged);

			MPDIList_listBox = e_pUserControl1->m_MPDIList_listBox;
			MPDI_listBox = e_pUserControl1->m_MPDI_listBox;

			m_pMPDIListList = new cNamedTypedObjectVector<cMPDIList>;
			m_pMPDIListList->SetFromResource(true);
			m_pCurrentSelectedObject = m_pCurrentMPDI;
		}
		~MPDI()
		{
			SAFE_DELETE(m_pMPDIListList);
		}
		System::Windows::Controls::ListBox^MPDIList_listBox;
		System::Windows::Controls::ListBox^MPDI_listBox;
		cAnimationParser*m_pAnimationParser;
		cNamedTypedObjectVector<cMPDIList>*m_pMPDIListList;
		public:cMultiPathDynamicImage*m_pCurrentMPDI;
		public:cMPDIList*m_pCurrentMPDIList;
		public:void    SetMPDIListFileIsUsing(void* e_pMPDIFileIsUsing){m_pMPDIListFileIsUsing = (FileIsUsing)e_pMPDIFileIsUsing;}
		public:FileIsUsing	m_pMPDIListFileIsUsing;
		public:void*GetCurrentMPDI(){return (void*)m_pCurrentMPDI;}
		public:void SetAnimationParser(void*e_pAnimationParser){ m_pAnimationParser = (cAnimationParser*)e_pAnimationParser; RefreshMPDIList(); }
		public:void    RefreshMPDIList();
		private: System::Void AddMPDI_button_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void DeleteMPDI_button_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MPDIList_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MPDI_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	};
}
