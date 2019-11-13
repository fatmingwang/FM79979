#include "StdAfx.h"
#include "MPDI.h"

namespace FMBookEditor
{
	void    MPDI::RefreshMPDIList()
	{
		MPDIList_listBox->Items->Clear();
		MPDI_listBox->Items->Clear();
		m_pMPDIListList->Destroy();
		if(m_pAnimationParser)
		{
			int l_iCount = m_pAnimationParser->Count();
			for( int i=0;i<l_iCount;++i )
			{
				System::Windows::Controls::ListBoxItem^l_pListBoxItem = gcnew System::Windows::Controls::ListBoxItem();
				l_pListBoxItem->Content = DNCT::WcharToGcstring(m_pAnimationParser->GetObject(i)->GetName());
				MPDIList_listBox->Items->Add(l_pListBoxItem);
				m_pMPDIListList->AddObject(m_pAnimationParser->GetMPDIList(i));
			}
			if( l_iCount )
			{
				MPDIList_listBox->SelectedIndex = l_iCount-1;
				//cMPDIList*l_pMPDIList =  m_pAnimationParser->GetMPDIList(MPDIList_listBox->SelectedIndex);
				//l_iCount = l_pMPDIList->Count();
				//for( int i=0;i<l_iCount;++i )
				//{
				//    MPDI_listBox->Items->Add(DNCT::WcharToGcstring(l_pMPDIList->GetObject(i)->GetName()));
				//}
			}
		}
	}

    System::Void MPDI::AddMPDI_button_Click(System::Object^  sender, System::EventArgs^  e)
    {
        if( !m_pAnimationParser )
        {
            //WARNING_MSG("SetAnimationParser!!!");
            return;
        }
	    cli::array<String^>^l_strFileNames = OpenFileAndGetNames("mpdi files (*.mpdi)|*.mpdi|All files (*.*)|*.*");
	    if( l_strFileNames )
	    for each(String^l_strFileName in l_strFileNames)
	    {
	        String^l_strMPDIName = System::IO::Path::GetFileNameWithoutExtension(l_strFileName);
			if(cGameApp::GetMPDIListByFileName(DNCT::GcStringToWchar(l_strFileName).c_str()))
	        //if(!MPDIList_listBox->Items->Contains(l_strMPDIName))
	        {
                //bool    l_b = m_pAnimationParser->Parse(DNCT::GcStringToChar(l_strFileName));
                //if( l_b )
                {
                    m_pMPDIListList->AddObject(dynamic_cast<cMPDIList*>(m_pAnimationParser->GetObject(m_pAnimationParser->Count()-1)));
					System::Windows::Controls::ListBoxItem^l_pListBoxItem = gcnew System::Windows::Controls::ListBoxItem();
					l_pListBoxItem->Content = l_strMPDIName;
                    MPDIList_listBox->Items->Add(l_pListBoxItem);
                }
            }
        }    
    }
	System::Void MPDI::DeleteMPDI_button_Click(System::Object^  sender, System::EventArgs^  e)
    {
		while(MPDIList_listBox->SelectedItems->Count)
        {
            int l_iIndex  = MPDIList_listBox->Items->IndexOf(MPDIList_listBox->SelectedItems[0]);
            auto l_strName = DNCT::GcStringToWchar(MPDIList_listBox->Items[l_iIndex]->ToString());
            MPDIList_listBox->SelectedItems->Remove(l_iIndex );
            if( m_pMPDIListFileIsUsing  )
            {
                if( m_pMPDIListFileIsUsing(m_pMPDIListList->GetObject(l_strName.c_str())) )
                {
                    WARNING_MSG("this file is using");
                    continue;
                }
            }
            MPDIList_listBox->Items->RemoveAt(l_iIndex);
            m_pMPDIListList->RemoveObjectWithoutDelete(l_strName.c_str());
            m_pAnimationParser->RemoveObject(l_strName);
        }
        m_pCurrentMPDI = 0;
        MPDIList_listBox->SelectedIndex = -1;
    }
	System::Void MPDI::MPDIList_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
    {
        m_pCurrentMPDIList = 0;
        MPDI_listBox->Items->Clear();
        if(MPDIList_listBox->SelectedIndex != -1)
        {
            cMPDIList*l_pMPDIList = m_pMPDIListList->GetObject(MPDIList_listBox->SelectedIndex);
			if( MPDIList_listBox->SelectedIndex >= m_pMPDIListList->Count() )
			{
				WARNING_MSG("this MPDI is wrong!!");
				return;
			}
            m_pCurrentMPDIList = l_pMPDIList;
            for( int i=0;i<l_pMPDIList->Count();++i )
            {
				System::Windows::Controls::ListBoxItem^l_pListBoxItem = gcnew System::Windows::Controls::ListBoxItem();
				l_pListBoxItem->Content = DNCT::WcharToGcstring(l_pMPDIList->GetObject(i)->GetName());
                MPDI_listBox->Items->Add( l_pListBoxItem );
            }
            if( l_pMPDIList->Count() )
                MPDI_listBox->SelectedIndex = 0;
        }    
    }

	System::Void MPDI::MPDI_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
    {
        m_pCurrentMPDI = 0;
        if( MPDI_listBox->SelectedIndex != -1 )
        {
            if( m_pCurrentMPDIList )
            {
                m_pCurrentMPDI = m_pCurrentMPDIList->GetObject(MPDI_listBox->SelectedIndex);
            }
        }        
    }
}