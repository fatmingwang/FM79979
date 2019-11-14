#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMBookEditor
{


	public ref class Sound:public cFatmingUtilitiesBase
	{
	public:
		Sound(CoreWPF::FMC::Sound^e_pUserControl1)
		{
            m_pSoundParser = 0;
            m_pCurrentBasicSound = 0;
            m_pFileIsUsing = 0;

			m_pCurrentSelectedObject = m_pCurrentBasicSound;

			Sound_listBox = e_pUserControl1->m_Sound_Listbox;
			Loop_checkBox = e_pUserControl1->m_Loop_Checkbox;

			

			e_pUserControl1->f_OpenSoundFile += gcnew EventHandler(this,&Sound::OpenSoundFile_button_Click);
			e_pUserControl1->f_AddSound += gcnew EventHandler(this,&Sound::AddSound_button_Click);
			e_pUserControl1->f_DelSound += gcnew EventHandler(this,&Sound::DeleteSound_button_Click);
			e_pUserControl1->f_SoundChangwSelection += gcnew EventHandler(this,&Sound::Sound_listBox_SelectedIndexChanged);
			e_pUserControl1->f_LoopCheckboxChecked += gcnew EventHandler(this,&Sound::Loop_checkBox_CheckedChanged);
			e_pUserControl1->f_SaveoundFile += gcnew EventHandler(this,&Sound::SaveSound_button_Click);
			e_pUserControl1->f_PlaySound += gcnew EventHandler(this,&Sound::Play_button_Click);
			e_pUserControl1->f_StopSound += gcnew EventHandler(this,&Sound::Stop_button_Click);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Sound()
		{//from cGameApp::m_spSoundParser
            //SAFE_DELETE(m_pSoundParser);
		}

    private: System::Windows::Controls::ListBox^	Sound_listBox;
	private: System::Windows::Controls::CheckBox^   Loop_checkBox;
    //my
        cSoundParser*m_pSoundParser;
	public:cBasicSound*m_pCurrentBasicSound;
        FileIsUsing	m_pFileIsUsing;
    public:void    ReflashSoundList(){DumpObjectListIntoListBox<cSoundParser*>(Sound_listBox,m_pSoundParser,true,false);}
    public:void    SetIsFileUsing(void*e_pFileIsUsing){ m_pFileIsUsing = (FileIsUsing)e_pFileIsUsing; }
    public:void    SetSoundParser(void*e_pSoundParser){m_pSoundParser = (cSoundParser*)e_pSoundParser;}
    public:void*   GetSoundParser(){ return m_pSoundParser;}
    public:void*   GetCurrentSound(){ return m_pCurrentBasicSound; }
    
private: System::Void OpenSoundFile_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
            if( m_pSoundParser )
            {
                if( m_pFileIsUsing )
                {
                    for( int i=0;i<m_pSoundParser->Count();++i )
                    {
                        if( m_pFileIsUsing(m_pSoundParser->GetObject(i)) )
                        {
                            WARNING_MSG("sound file index "+i.ToString()+" is in using!");
                            return;
                        }
                    }
                }
                String^l_strFiles = DNCT::OpenFileAndGetName("Sound Files(*.xml;*.fat)|*.xml;*.fat;|All files (*.*)|*.*");
                if( l_strFiles )
                {
                    bool    l_b = m_pSoundParser->Parse(DNCT::GcStringToChar(l_strFiles));
                    ReflashSoundList();
                }
            }
        }

private: System::Void AddSound_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
            if( m_pSoundParser )
            {
                cli::array<String^>^l_strFiles = DNCT::OpenFileAndGetNames("Sound Files(*.wav;*.ogg)|*.wav;*.ogg;|All files (*.*)|*.*");
                if( l_strFiles )
                {
                    for each( String^l_strFileName in l_strFiles )
                    {
                        m_pSoundParser->AddStaticSound(m_pSoundParser,DNCT::GcStringToChar(l_strFileName).c_str());
                    }
                    ReflashSoundList();
                }
            }
        }

private: System::Void DeleteSound_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
            if( Sound_listBox->SelectedIndex != -1 )
            {
                if( m_pFileIsUsing )
                {
                    if( m_pFileIsUsing(m_pSoundParser->GetObject(Sound_listBox->SelectedIndex)) )
                    {
                        WARNING_MSG("sound file index "+Sound_listBox->SelectedItem->ToString()+" is in using!");
                        return;
                    }
                }            
                m_pSoundParser->RemoveObject(Sound_listBox->SelectedIndex);
                m_pCurrentBasicSound = 0;
                ReflashSoundList();
            }
        }
private: System::Void Sound_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
        {
            m_pCurrentBasicSound = 0;
            if( Sound_listBox->SelectedIndex != -1 )
            {
                m_pCurrentBasicSound = m_pSoundParser->GetObject(Sound_listBox->SelectedIndex);
				Loop_checkBox->IsChecked = m_pCurrentBasicSound->IsLoop();
            }
        }
private: System::Void Loop_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
        {
            if( Sound_listBox->SelectedIndex != -1 )
            {
				bool	l_b = Loop_checkBox->IsChecked.ToString()->Equals("bool");
				if( l_b )
					m_pSoundParser->GetObject(Sound_listBox->SelectedIndex)->SetLoop(true);
				else
					m_pSoundParser->GetObject(Sound_listBox->SelectedIndex)->SetLoop(false);
            }
        }
private: System::Void SaveSound_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
            if( m_pSoundParser&&m_pSoundParser->Count() )
            {
                String^l_strFileName = DNCT::SaveFileAndGetName("XML	Files(.xml;)|;*.xml;|All files (*.*)|*.*");
                if( l_strFileName )
                {
                    m_pSoundParser->Export(DNCT::GcStringToChar(l_strFileName).c_str());
                }
            }
        }
private: System::Void Play_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
            if( Sound_listBox->SelectedIndex != -1 )
            {
                m_pSoundParser->GetObject(Sound_listBox->SelectedIndex)->Play(true);
            }
        }
private: System::Void Stop_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
            m_pSoundParser->Stop();
        }    
	};
}
