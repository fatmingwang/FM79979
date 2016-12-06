#pragma once

#include "BasicSound.h"

//use another thread to record sound
class cFUThreadPool;

class cSoundCapture
{
	cFUThreadPool*			m_pFUThreadPool;
	bool					m_bPause;
	std::string				m_strSaveFileName;
	ALCdevice *m_pDevice;
	static cSoundCapture*m_spSoundCapture;
	cSoundCapture(ALCuint frequency = 44100, ALCenum format = AL_FORMAT_STEREO16, ALCsizei buffersize = 1024);
public:
	~cSoundCapture();
	static	cSoundCapture* CreateSoundCapture();
	void	DeleteSoundCapture();
	bool	StartRecord(std::string e_strFileName);
	void	PauseRecord();
	void	StopRecord();
};