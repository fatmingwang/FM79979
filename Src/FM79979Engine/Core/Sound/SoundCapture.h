#pragma once

#define USE_SOUND_CAPTURE
#if defined(USE_SOUND_CAPTURE)


#include "BasicSound.h"
//use another thread to record sound
class cFUThreadPool;
class cSoundFile;
namespace	FATMING_CORE
{
	class cGameApp;
	class cBinaryFile;
	#define SOUND_CAPTURE_FREQUENCE	22050
	#define	SOUND_CAPTURE_BUFFSIZE	SOUND_CAPTURE_FREQUENCE/2
	#define	SOUND_CAPTURE_FORMAT	AL_FORMAT_MONO16
	enum eCaptureSoundFileFormat
	{
		eCSFF_WAV = 0,
		eCSFF_OGG,
		eCSFF_MAX
	};
	class cSoundCapture
	{
		cFUThreadPool*			m_pFUThreadPool;
		bool					m_bPause;
		bool					m_bStop;
		std::string				m_strSaveFileName;
		cSoundFile*				m_pSoundFile;
		int						m_iSampleRate;
		ALCdevice*				m_pDevice;
		static cSoundCapture*	m_spSoundCapture;
		//cFUSynchronized			m_FUSynchronized;
		bool					m_bIsRecording;
		int						m_iWriteChannel;
		int						m_iWriteBitpersample;
		//
		eCaptureSoundFileFormat	m_eCaptureSoundFileFormat;
		//no header file size
		int						m_iFileSize;
	public:
		cSoundCapture(ALCuint frequency = SOUND_CAPTURE_FREQUENCE, ALCenum format = SOUND_CAPTURE_FORMAT, ALCsizei buffersize = SOUND_CAPTURE_BUFFSIZE);
		~cSoundCapture();
		bool							StartRecord(std::string e_strFileName,eCaptureSoundFileFormat	e_eCaptureSoundFileFormat = eCSFF_OGG);
		void							PauseRecord(bool e_bPause);
		void							StopRecord();
		bool							IsStop(){return m_bStop;}
		bool							IsPause(){return m_bPause;}
		void							Update(float e_fElpaseTime);
		ALCdevice*						GetDevice(){return m_pDevice;}
		//const cFUSynchronized*			GetSynchronized(){return &m_FUSynchronized;}
		cSoundFile*						GetSoundFile(){return m_pSoundFile;}
		int								GetSampleRate(){return m_iSampleRate;}
		bool							IsRecording(){return m_bIsRecording;}
		eCaptureSoundFileFormat			GetCaptureSoundFileFormat(){return m_eCaptureSoundFileFormat;}
		void							SetStop(bool e_bStop){m_bStop = e_bStop;}
		int								GetWriteChannel(){return m_iWriteChannel;}
		void							AddFileSize(int e_iFileSize);
		int								GetFileSize(){return m_iFileSize;}
		//
	};
}
//end def USE_SOUND_CAPTURE
#endif