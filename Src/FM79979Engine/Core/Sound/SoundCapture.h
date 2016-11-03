#pragma once

#include "BasicSound.h"

class cSoundCapture
{
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