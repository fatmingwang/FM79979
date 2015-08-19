#pragma once
#include "MPDIVideo.h"

namespace MPDI 
{

	public ref class	cMPDIThread
	{
		cMPDIVideo_Editor^m_pMPDIVideo_Editor;
		cSoundParser*		m_pSoundParser;
	public:
		cMPDIThread();
		~cMPDIThread();
		cMPDIVideo_Editor^	GetVideo(){return m_pMPDIVideo_Editor;}
		void	SetVideoPlayer(CoreWPF::VideoPlayer::VideoPlayer^e_pVideoPlayer);
		void	Update(float e_fElpaseTime);
		void	Render();
	};
//end namespace;
}