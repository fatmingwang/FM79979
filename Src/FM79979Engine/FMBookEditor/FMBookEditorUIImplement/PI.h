#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMBookEditor
{

	public ref class PI :public cFatmingUtilitiesBase
	{
	public:
		PI(CoreWPF::FMC::PI^e_pUserControl1)
		{
			e_pUserControl1->f_PIChangeSelection  += gcnew EventHandler(this,&PI::PI_listBox_SelectedIndexChanged);
			e_pUserControl1->f_PIUnitChangeSelection  += gcnew EventHandler(this,&PI::PIUnitList_listBox_SelectedIndexChanged);
			e_pUserControl1->f_AddPIButtonClick += gcnew EventHandler(this,&PI::AddPI_button_Click);
			e_pUserControl1->f_DelPIButtonClick += gcnew EventHandler(this,&PI::DeletePI_button_Click);
			
			PIUnitList_listBox = e_pUserControl1->m_PIUnit_listBox;
			PIList_listBox = e_pUserControl1->m_PI_listBox;
			m_pImageParser = 0;
			m_pPuzzleImageList = new cNamedTypedObjectVector<cPuzzleImage>;
			m_pPuzzleImageList->SetFromResource(true);
			m_pCurrentPI = 0;
            m_pCurrentPIUnit = 0;
            m_pPuzzleImageFileIsUsing = 0;
			m_pCurrentSelectedObject = m_pCurrentPIUnit;
		}

	protected:
		~PI()
		{
		    SAFE_DELETE(m_pPuzzleImageList);
		}
    //my
	private: System::Windows::Controls::ListBox^  PIUnitList_listBox;
    private: System::Windows::Controls::ListBox^  PIList_listBox;
    public:void    SetPIFileIsUsing(void* e_pPuzzleImageFileIsUsing){m_pPuzzleImageFileIsUsing = (FileIsUsing)e_pPuzzleImageFileIsUsing;}
    FileIsUsing	m_pPuzzleImageFileIsUsing;
    public:void*   GetCurrentPIUnit(){ return m_pCurrentPIUnit; }
    void    SetImageParser(void*e_pImageParser){ m_pImageParser = (cImageParser*)e_pImageParser; RefreshPIList(); }
    //because it come from DLL,not same instance
	public:void    RefreshTextureID(){ if(m_pCurrentPIUnit)m_pCurrentPIUnit->GetTexture()->m_suiLastUsingImageIndex = -1; }
    private:cNamedTypedObjectVector<cPuzzleImage>*m_pPuzzleImageList;
    private:cImageParser*m_pImageParser;
    public: cPuzzleImage*m_pCurrentPI;
    public: cPuzzleImageUnit*m_pCurrentPIUnit;
    public: void    RefreshPIList()
    {
        PIList_listBox->Items->Clear();
        PIUnitList_listBox->Items->Clear();
		m_pPuzzleImageList->Destroy();
        if( m_pImageParser )
        {
            int l_iCount = m_pImageParser->Count();
            for( int i=0;i<l_iCount;++i )
            {
                if( m_pImageParser->GetObject(i)->Type() == cPuzzleImage::TypeID )
                {
                    PIList_listBox->Items->Add(DNCT::WcharToGcstring(m_pImageParser->GetObject(i)->GetName()));
                    m_pPuzzleImageList->AddObject(m_pImageParser->GetPuzzleImage(i));
                }
            }
            PIUnitList_listBox->Items->Clear();
            if( l_iCount>0 )
            {
                PIList_listBox->SelectedIndex = PIList_listBox->Items->Count-1;
            }
        }
    }
	
    private: System::Void AddPI_button_Click(System::Object^  sender, System::EventArgs^  e)
    {
        if( !m_pImageParser )
        {
            WARNING_MSG("SetImageParser........");
            return;
        }
	    cli::array<String^>^l_strFileNames = OpenFileAndGetNames("pi files (*.pi)|*.pi|All files (*.*)|*.*");
		if( l_strFileNames )
		{
			for each(String^l_strFileName in l_strFileNames)
			{
				String^l_strPIName = System::IO::Path::GetFileNameWithoutExtension(l_strFileName);
				if(!PIList_listBox->Items->Contains(l_strPIName))
				{
					bool    l_b = m_pImageParser->GetPuzzleImageByFileName(DNCT::GcStringToChar(l_strFileName).c_str());
					m_pPuzzleImageList->AddObject(dynamic_cast<cPuzzleImage*>(m_pImageParser->GetObject(m_pImageParser->Count()-1)));
					PIList_listBox->Items->Add(l_strPIName);
				}
			}
		}
    }
	private: System::Void DeletePI_button_Click(System::Object^  sender, System::EventArgs^  e)
    {
        while(PIList_listBox->SelectedItems->Count)
        {
            int l_iIndex  = PIList_listBox->Items->IndexOf(PIList_listBox->SelectedItems[0]);
            auto l_strName = DNCT::GcStringToWchar(PIList_listBox->Items[l_iIndex]->ToString());
            PIList_listBox->SelectedItems->Remove(l_iIndex );
            if( m_pPuzzleImageFileIsUsing  )
            {
                if( m_pPuzzleImageFileIsUsing(m_pPuzzleImageList->GetObject(l_strName.c_str())) )
                {
                    WARNING_MSG("this file is using");
                    continue;
                }
            }        
            PIList_listBox->Items->RemoveAt(l_iIndex );
            m_pPuzzleImageList->RemoveObjectWithoutDelete(l_strName.c_str());
            m_pImageParser->RemoveObject(l_strName);
        }
        PIList_listBox->SelectedIndex = -1;
        m_pCurrentPI = 0;
        m_pCurrentPIUnit = 0;
    }

	private: System::Void PI_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
    {
        PIUnitList_listBox->Items->Clear();
        m_pCurrentPI = 0;
        if(PIList_listBox->SelectedIndex != -1)
        {
            cPuzzleImage*l_pPI = m_pPuzzleImageList->GetObject(PIList_listBox->SelectedIndex);
            m_pCurrentPI = l_pPI;
            for( int i=0;i<l_pPI->Count();++i )
            {
                PIUnitList_listBox->Items->Add( DNCT::WcharToGcstring(l_pPI->GetObject(i)->GetName()) );
            }
            if( l_pPI->Count() )
                PIUnitList_listBox->SelectedIndex = 0;
        }
    }
	private: System::Void PIUnitList_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
    {
        m_pCurrentPIUnit = 0;
        if( PIUnitList_listBox->SelectedIndex != -1 )
        {
            if( m_pCurrentPI )
            {
                m_pCurrentPIUnit = m_pCurrentPI->GetObject(PIUnitList_listBox->SelectedIndex);
            }
        }
    }
	};
}
