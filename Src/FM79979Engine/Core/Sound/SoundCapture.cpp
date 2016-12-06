#include "../stdafx.h"
#include "SoundCapture.h"
//#include <thread>
#include "../Synchronization/FUThreadPool.h"
cSoundCapture*cSoundCapture::m_spSoundCapture = nullptr;

//const int SRATE = 44100;
//const int SSIZE = 1024;
//
//ALbyte buffer[22050];
//ALint sample;
cSoundCapture::cSoundCapture(ALCuint frequency, ALCenum format, ALCsizei buffersize)
{
	m_pFUThreadPool = nullptr;
	m_bPause = false;
	m_pDevice = alcCaptureOpenDevice(NULL, frequency, format, buffersize);
	if (alGetError() != AL_NO_ERROR) {
		if( m_pDevice )
			alcCaptureCloseDevice(m_pDevice);
        m_pDevice = nullptr;
    }
}

cSoundCapture::~cSoundCapture()
{
	SAFE_DELETE(m_pFUThreadPool);
	if( m_pDevice )
	{
		alcCaptureStop(m_pDevice);
		alcCaptureCloseDevice(m_pDevice);
	}
}

cSoundCapture* cSoundCapture::CreateSoundCapture()
{
	return nullptr;
}

void	cSoundCapture::DeleteSoundCapture()
{

}
//http://stackoverflow.com/questions/4087727/openal-how-to-create-simple-microphone-echo-programm
bool	cSoundCapture::StartRecord(std::string e_strFileName)
{
	m_strSaveFileName = e_strFileName;

	if(m_pDevice)
	{
		alcCaptureStart(this->m_pDevice);
		return true;
	}
	return false;
    //while (true) {
    //    alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &sample);
    //    alcCaptureSamples(device, (ALCvoid *)buffer, sample);

    //    // ... do something with the buffer 
    //}
	
}
void	cSoundCapture::PauseRecord()
{
	
}

void	cSoundCapture::StopRecord()
{
	
}


void	RecordingThread(void*e_pData)
{

}