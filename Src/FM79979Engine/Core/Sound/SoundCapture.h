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

	class cSoundCapture;


	class cSounRecordCallBackObject:public NamedTypedObject
	{
		friend class	cSoundCapture;
		friend void		RecordingThread(size_t _workParameter, size_t _pUri);
		friend void		RecordingDoneThread(size_t _workParameter, size_t _pUri);
	private:
		void			PreCaptureSoundStartCallBack(cSoundCapture*e_pSoundCapture);
		virtual void	CaptureSoundStartCallBack() = 0;
		virtual void	CaptureSoundNewDataCallBack(ALCint e_iSamplesIn,char*e_pData) = 0;
		virtual void	CaptureSoundEndCallBack() = 0;
	protected:
		cSoundCapture*	m_pSoundCapture;
	public:
		cSounRecordCallBackObject(){m_pSoundCapture = nullptr;}
		virtual ~cSounRecordCallBackObject(){}
	};

	//===================================
	//DO NOT ADD ANY cSounRecordCallBackObject AFTER StartRecord
	//
	//
	//===================================
	void	RecordingThread(size_t _workParameter, size_t _pUri);
	void	RecordingDoneThread(size_t _workParameter, size_t _pUri);
	class cSoundCapture:public cNamedTypedObjectVector<cSounRecordCallBackObject>
	{
		friend void	RecordingThread(size_t _workParameter, size_t _pUri);
		friend void	RecordingDoneThread(size_t _workParameter, size_t _pUri);
	protected:
		cFUThreadPool*			m_pFUThreadPool;
		bool					m_bPause;
		bool					m_bStop;
		bool					m_bThreadExitStop;
		//std::string				m_strSaveFileName;
		//cSoundFile*				m_pSoundFile;
		ALCdevice*				m_pDevice;
		int						m_iSampleRate;
		int						m_iWriteChannel;
		int						m_iWriteBitpersample;
		int						m_iBufferSize;
		static cSoundCapture*	m_spSoundCapture;
		//cFUSynchronized			m_FUSynchronized;
		bool					m_bIsRecording;
		//no header file size
		int						m_iFileSize;
		float					m_fCurrntTime;
	public:
		cSoundCapture(ALCuint frequency = SOUND_CAPTURE_FREQUENCE, ALCenum format = SOUND_CAPTURE_FORMAT, ALCsizei buffersize = SOUND_CAPTURE_BUFFSIZE);
		~cSoundCapture();
		bool							StartRecord();
		void							PauseRecord(bool e_bPause);
		void							StopRecord();
		bool							IsStop(){return m_bStop;}
		bool							IsPause(){return m_bPause;}
		void							Update(float e_fElpaseTime);
		ALCdevice*						GetDevice(){return m_pDevice;}
		int								GetSampleRate(){return m_iSampleRate;}
		int								GetWriteChannel(){return m_iWriteChannel;}
		int								GetWriteBitpersample(){return m_iWriteBitpersample;}
		bool							IsRecording(){return m_bIsRecording;}
		void							SetStop(bool e_bStop){m_bStop = e_bStop;}
		void							AddFileSize(int e_iFileSize);
		int								GetFileSize(){return m_iFileSize;}
		int								GetBuffersize(){return m_iBufferSize;}
		float							GetCurrntTime(){return m_fCurrntTime;}
		//
		bool							AddSoundRecord(std::string e_strFileName = "Record.ogg",eCaptureSoundFileFormat	e_eCaptureSoundFileFormat = eCSFF_OGG);
		//
	};
	//================================
	//
	//================================
	class cSounRecordToFileCallBackObject:public cSounRecordCallBackObject
	{
		//
		eCaptureSoundFileFormat	m_eCaptureSoundFileFormat;
		std::string				m_strSaveFileName;
		cSoundFile*				m_pSoundFile;
		virtual void			CaptureSoundStartCallBack();
		virtual void			CaptureSoundNewDataCallBack(ALCint e_iSamplesIn,char*e_pData);
		virtual void			CaptureSoundEndCallBack();
		void					Destory();
	public:
		DEFINE_TYPE_INFO();
		cSounRecordToFileCallBackObject(std::string e_strFileName,eCaptureSoundFileFormat	e_eCaptureSoundFileFormat = eCSFF_OGG);
		~cSounRecordToFileCallBackObject();
		void					SetSaveFileName(const char*e_strFileName);
		const char*				GetSaveFileName();
		const cSoundFile*		GetSoundfile();
		eCaptureSoundFileFormat	GetCaptureSoundFileFormat(){return m_eCaptureSoundFileFormat;}
	};
}
//end def USE_SOUND_CAPTURE
#endif

//http://stackoverflow.com/questions/4281058/how-to-let-user-select-a-audio-recording-device-with-openal
//list all device to choice to recod but I am lazy to do this now.
//http://stackoverflow.com/questions/16329081/is-it-possible-in-openal-to-directly-choose-a-recording-device
//String[] capDevices = ALC11.alcGetString(null, ALC11.ALC_CAPTURE_DEVICE_SPECIFIER).split("\0");
//for (int i = 0; i < capDevices.length; i++) {
//    System.out.println("Capture device "+ i + ": " + capDevices[i]);
//}
////Selection code goes here
//String chosenDevice = ...;
//
//ALCdevice device = ALC11.alcCaptureOpenDevice(chosenDevice, freq, format, bufferSize);

