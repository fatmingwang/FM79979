#include "stdafx.h"
#include "MPDIVideo.h"
#include "../../Core/Network/Network.h"
namespace MPDI 
{
	cMPDIVideo_Editor::cMPDIVideo_Editor(CoreWPF::VideoPlayer::VideoPlayer^e_pVideoPlayer)
	{
		m_pVideoPlayer = e_pVideoPlayer;
		m_pVideoSubMPDI = new cVideoSubMPDI();
		m_pVideoSubMPDI->SetAnimationLoop(true);
		m_pAVIPlayer = m_pVideoSubMPDI->GetAVIPlayer();
		m_TimeLabel = e_pVideoPlayer->m_TimeText;
		m_pSliderTime = e_pVideoPlayer->m_SliderTime;
		m_pVideoPlayer->f_FileOpen += gcnew EventHandler(this,&cMPDIVideo_Editor::FileOpen);
		m_pVideoPlayer->f_Play += gcnew EventHandler(this,&cMPDIVideo_Editor::Play);
		m_pVideoPlayer->f_Pause += gcnew EventHandler(this,&cMPDIVideo_Editor::Pause);
		m_pVideoPlayer->f_Stop += gcnew EventHandler(this,&cMPDIVideo_Editor::Stop);

		m_pVideoPlayer->f_SliderValueChange += gcnew EventHandler(this,&cMPDIVideo_Editor::SliderToTime);
		//m_pVideoPlayer->f_MouseUp += gcnew EventHandler(this,&cMPDIVideo_Editor::SliderToTime);



		m_pVideoPlayer->m_StartH->f_ValueChanged += gcnew EventHandler(this,&cMPDIVideo_Editor::StartTimeValueChanged);
		m_pVideoPlayer->m_StartM->f_ValueChanged += gcnew EventHandler(this,&cMPDIVideo_Editor::StartTimeValueChanged);
		m_pVideoPlayer->m_StartS->f_ValueChanged += gcnew EventHandler(this,&cMPDIVideo_Editor::StartTimeValueChanged);

		m_pVideoPlayer->m_EndH->f_ValueChanged += gcnew EventHandler(this,&cMPDIVideo_Editor::EndTimeValueChanged);
		m_pVideoPlayer->m_EndM->f_ValueChanged += gcnew EventHandler(this,&cMPDIVideo_Editor::EndTimeValueChanged);
		m_pVideoPlayer->m_EndS->f_ValueChanged += gcnew EventHandler(this,&cMPDIVideo_Editor::EndTimeValueChanged);
	}

	cMPDIVideo_Editor::~cMPDIVideo_Editor()
	{
		//Destroy();
	}

	void	cMPDIVideo_Editor::Destroy()
	{
		if( m_pVideoSubMPDI )
			delete m_pVideoSubMPDI;
		//if( m_pVideoPlayer )
		//	delete m_pVideoPlayer;
	}

	System::Void cMPDIVideo_Editor::StartTimeValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		//m_pVideoSubMPDI->GetStartTimePointer();
		//if(sender == m_pVideoPlayer->m_StartH )
		//{
		//
		//}
		//else
		//if(sender == m_pVideoPlayer->m_StartM )
		//{
		//
		//}
		//else
		//if(sender == m_pVideoPlayer->m_StartS )
		//{

		//}
	}

	System::Void cMPDIVideo_Editor::EndTimeValueChanged(System::Object^  sender, System::EventArgs^  e)
	{

	}

	void	cMPDIVideo_Editor::FileOpen(const char*e_strFileName)
	{
		m_pVideoSubMPDI->OpenFile(e_strFileName);
		m_pSliderTime->Minimum = 0;
		m_pSliderTime->Maximum = 0;
		if( m_pAVIPlayer->IsFileOpen() )
		{
			m_pSliderTime->Maximum = m_pAVIPlayer->GetFileTimeLength();
		}
		m_pSliderTime->Value = 0;
	}

	System::Void cMPDIVideo_Editor::FileOpen(System::Object^  sender, System::EventArgs^  e)
	{	
		String^l_strFileName = DNCT::OpenFileAndGetName("avi files (*.avi)|*.avi|All files (*.*)|*.*");
		if( l_strFileName )
		{
			FileOpen(DNCT::GcStringToChar(l_strFileName).c_str());
			m_pVideoSubMPDI->Init();
			this->m_pAVIPlayer->SetPause(false);
			this->m_pAVIPlayer->SetLoop(true);
		}
	}

	System::Void cMPDIVideo_Editor::Play(System::Object^  sender, System::EventArgs^  e)
	{
		this->m_pAVIPlayer->SetPause(false);
	}
	System::Void cMPDIVideo_Editor::Pause(System::Object^  sender, System::EventArgs^  e)
	{
		m_pAVIPlayer->SetPause(!m_pAVIPlayer->GetPause());
	}

	System::Void cMPDIVideo_Editor::Stop(System::Object^  sender, System::EventArgs^  e)
	{
		m_pVideoSubMPDI->Init();
		m_pAVIPlayer->SetPause(true);
		m_pSliderTime->Value = 0;
	}

	System::Void cMPDIVideo_Editor::SetCurrentVideoTime(float e_fTime)
	{
		m_pSliderTime->Value = e_fTime;
	}

	System::Void cMPDIVideo_Editor::SliderToTime(System::Object^  sender, System::EventArgs^  e)
	{
		if(m_pAVIPlayer->GetPause())
			return;
		//m_pAVIPlayer->SetPause(true);
		m_pAVIPlayer->SetCurrentFrameByTime((float)m_pSliderTime->Value);
		m_pAVIPlayer->SetPause(false);
	}

	void	cMPDIVideo_Editor::UpdateTrackBar()
	{
		int	l_iH = (int)(m_pSliderTime->Value/60/60);
		int	l_iM = (int)((m_pSliderTime->Value-(l_iH*60*60))/60);
		int	l_iS = (int)(m_pSliderTime->Value-(l_iH*60*60)-(l_iM*60));
		m_TimeLabel->Content = l_iH.ToString()+"/"+l_iM.ToString()+"/"+l_iS.ToString();
	}

	void	cMPDIVideo_Editor::Update(float e_fElpaseTime)
	{
		if( !m_pAVIPlayer->IsFileOpen() )
			return;
		if(m_pAVIPlayer->GetPause())
			return;
		m_pSliderTime->Value += e_fElpaseTime;
		UpdateTrackBar();
	}

	void	cMPDIVideo_Editor::Render()
	{
		POINT	l_Size = {m_pVideoSubMPDI->GetAVIPlayer()->GetWidth(),m_pVideoSubMPDI->GetAVIPlayer()->GetHeight()};
		m_pVideoSubMPDI->GetAVIPlayer()->RenderByTime((float)m_pSliderTime->Value,0,0,l_Size);
		//m_pVideoSubMPDI->Render();
	}
}