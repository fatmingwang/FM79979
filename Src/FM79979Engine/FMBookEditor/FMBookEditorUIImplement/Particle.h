#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMBookEditor
{

	public ref class Particle:public cFatmingUtilitiesBase
	{
	public:
		Particle(CoreWPF::FMC::Particle^e_pUserControl1)
		{
			e_pUserControl1->f_ParticleGroupListChangeSelection	+= gcnew EventHandler(this,&Particle::ParticleGroupFile_listBox_SelectedIndexChanged);
			e_pUserControl1->f_ParticleGroupChangeSelection		+= gcnew EventHandler(this,&Particle::ParticleGroup_listBox_SelectedIndexChanged);
			e_pUserControl1->f_ParticleChangeSelection			+= gcnew EventHandler(this,&Particle::Particle_listBox_SelectedIndexChanged);
			e_pUserControl1->f_AddParticle += gcnew EventHandler(this,&Particle::AddPrtGroup_button_Click);
			e_pUserControl1->f_DelParticle += gcnew EventHandler(this,&Particle::DelPrtGroup_button_Click);

			ParticleGroupFile_listBox = e_pUserControl1->m_ParticleGroupList_listBox;
			ParticleGroup_listBox = e_pUserControl1->m_ParticleGroup_listBox;
			Particle_listBox = e_pUserControl1->m_Particle_listBox;
			m_pPrtGList = 0;
			if( g_pEditorMode )
				m_pPrtGList = g_pEditorMode->m_spEventManager->m_pPrtGList;
		    m_iCurrentSelectDataType = -1;
		    m_pSelectedParticleEmitterGroup = 0;
		    m_pSelectedPrtEmitter = 0;
			m_pCurrentSelectedObject = m_pSelectedPrtEmitter;
		}

	protected:
		~Particle()
		{
		}
    //my
	private: System::Windows::Controls::ListBox^  ParticleGroupFile_listBox;
	private: System::Windows::Controls::ListBox^  ParticleGroup_listBox;
	private: System::Windows::Controls::ListBox^  Particle_listBox;    
    //my
    cPrtGList*m_pPrtGList;
    public: cPrtGList*GetBehaviorPaticleManager(){ return m_pPrtGList; }
    //-1 for non selected,0 for particle,1 for prt group
    int     m_iCurrentSelectDataType;
    cParticleEmitterGroup*m_pSelectedParticleEmitterGroup;
    cPrtEmitter*m_pSelectedPrtEmitter;
    public: int    GetCurrentSelectDataType(){ return m_iCurrentSelectDataType; }
    public: void*    GetCurrentSelectedData(){switch(m_iCurrentSelectDataType){case 0:return m_pSelectedPrtEmitter;case 1:return m_pSelectedParticleEmitterGroup;}return 0;}
    void    RefreshList()
    {
        Particle_listBox->Items->Clear();
        ParticleGroup_listBox->Items->Clear();
        ParticleGroupFile_listBox->Items->Clear();
        if( m_pPrtGList )
        {
            int l_iCount = m_pPrtGList->Count();
            for( int i=0;i<l_iCount;++i )
            {
                ParticleGroupFile_listBox->Items->Add(DNCT::WcharToGcstring(m_pPrtGList->GetObject(i)->GetName()));
            }
        }
    }

private: System::Void Particle_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
        {
            m_pSelectedParticleEmitterGroup = 0;
            if( sender == Particle_listBox )
            {
                m_iCurrentSelectDataType = -1;
                if( ParticleGroupFile_listBox->SelectedIndex != -1 )
                {
                    m_iCurrentSelectDataType = 0;
                    cBehaviorPaticleManager*l_pBehaviorPaticleManager = this->m_pPrtGList->GetObject(ParticleGroupFile_listBox->SelectedIndex);
                    m_pSelectedPrtEmitter = l_pBehaviorPaticleManager->GetPaticleManager()->GetObject(Particle_listBox->SelectedIndex);
                    //ParticleGroup_listBox->SelectedIndex = -1;
                }
            }
        }
private: System::Void ParticleGroup_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
        {
            m_pSelectedPrtEmitter = 0;
            if( sender == ParticleGroup_listBox )
            {
                m_iCurrentSelectDataType = -1;
                if( ParticleGroupFile_listBox->SelectedIndex != -1 )
                {
                    m_iCurrentSelectDataType = 1;
                    cBehaviorPaticleManager*l_pBehaviorPaticleManager = this->m_pPrtGList->GetObject(ParticleGroupFile_listBox->SelectedIndex);
                    m_pSelectedParticleEmitterGroup = dynamic_cast<cParticleEmitterGroup*>(l_pBehaviorPaticleManager->GetObject(ParticleGroup_listBox->SelectedIndex));
                    //Particle_listBox->SelectedIndex = -1;
                }
            }
        }
private: System::Void ParticleGroupFile_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
        {
            m_iCurrentSelectDataType = -1;            
            ParticleGroup_listBox->Items->Clear();
            Particle_listBox->Items->Clear();
            if( ParticleGroupFile_listBox->SelectedIndex != -1 )
            {
                cBehaviorPaticleManager*l_pBehaviorPaticleManager = this->m_pPrtGList->GetObject(ParticleGroupFile_listBox->SelectedIndex);
                int l_iCount = l_pBehaviorPaticleManager->Count();
                for( int i=0;i<l_iCount;++i )
                {
                    ParticleGroup_listBox->Items->Add(DNCT::WcharToGcstring(l_pBehaviorPaticleManager->GetObject(i)->GetName()));
                }
                cPaticleManager*l_pPaticleManager = l_pBehaviorPaticleManager->GetPaticleManager();
                l_iCount = l_pPaticleManager->Count();
                for( int i=0;i<l_iCount;++i )
                {
                    Particle_listBox->Items->Add(DNCT::WcharToGcstring(l_pPaticleManager->GetObject(i)->GetName()));
                }
            }
        }

private: System::Void AddPrtGroup_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
            String^l_strFileName = DNCT::OpenFileAndGetName("prtg files (*.prtg)|*.prtg");
            if( l_strFileName )
            {
                WCHAR*l_strObjectName = DNCT::GcStringToWchar(System::IO::Path::GetFileNameWithoutExtension(l_strFileName));
				String^l_strExtension = System::IO::Path::GetExtension(l_strFileName);
				if(!m_pPrtGList->GetObject(l_strObjectName))
				{
					cBehaviorPaticleManager*l_pBehaviorPaticleManager = new cBehaviorPaticleManager();
					l_pBehaviorPaticleManager->SetName(l_strObjectName);
					bool    l_b = l_pBehaviorPaticleManager->Parse(DNCT::GcStringToChar(l_strFileName));
					if( l_b && l_pBehaviorPaticleManager->Count() )
					{
						m_pPrtGList->AddObjectNeglectExist(l_pBehaviorPaticleManager);
						ParticleGroupFile_listBox->Items->Add(System::IO::Path::GetFileNameWithoutExtension(l_strFileName));
						ParticleGroupFile_listBox->SelectedIndex = ParticleGroupFile_listBox->Items->Count-1;
					}
					else
					{
						delete l_pBehaviorPaticleManager;
					}
				}
            }
        }
private: System::Void DelPrtGroup_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
            m_iCurrentSelectDataType = -1;
            if( ParticleGroupFile_listBox->SelectedIndex != -1 )
            {
                this->m_pPrtGList->RemoveObject(ParticleGroupFile_listBox->SelectedIndex);
                RefreshList();
            }
        }
	};
}
