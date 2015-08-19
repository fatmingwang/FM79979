#include "stdafx.h"
#include "MPDIThread.h"

namespace MPDI 
{

	cMPDIThread::cMPDIThread()
	{
		//m_pMPDIVideo_Editor = System::IntPtr::Zero;
		m_pSoundParser = new cSoundParser();
	}

	cMPDIThread::~cMPDIThread()
	{
		SAFE_DELETE(m_pSoundParser);
		if( m_pMPDIVideo_Editor )
			delete m_pMPDIVideo_Editor;
	}
	void	cMPDIThread::SetVideoPlayer(CoreWPF::VideoPlayer::VideoPlayer^e_pVideoPlayer)
	{
		if( m_pMPDIVideo_Editor )
			delete m_pMPDIVideo_Editor;
		if( e_pVideoPlayer )
		{
			m_pMPDIVideo_Editor = gcnew cMPDIVideo_Editor(e_pVideoPlayer);
		}
	}
	void	cMPDIThread::Update(float e_fElpaseTime)
	{
		if( m_pMPDIVideo_Editor )
			m_pMPDIVideo_Editor->Update(e_fElpaseTime);
	}
	void	cMPDIThread::Render()
	{
		if( m_pMPDIVideo_Editor )
			m_pMPDIVideo_Editor->Render();
	}
//end namesapce
}