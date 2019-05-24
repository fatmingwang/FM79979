#pragma once

#define USE_SOUND_CAPTURE
#if defined(USE_SOUND_CAPTURE)


#include "BasicSound.h"
#include "../GameplayUT/EventSender/MessageSender.h"
//use another thread to record sound
class cFUThreadPool;
class cSoundFile;
#ifdef ANDROID
	// (requires the RECORD_AUDIO permission)
	extern void CreateAndroidAudioEngine();
	extern unsigned char NativeAudioCreateAudioRecorder(int e_iFrequency,int e_iSampleBitm,int e_iChannel);
	extern void StartAndroidRecording(int e_iBufferSize);
	extern void AndroidRecordShutdown();
	extern void AndroidRecordPause(bool e_bPause);
	extern void AndroidRecordStop();

#endif
namespace	FATMING_CORE
{

	class cGameApp;
	class cBinaryFile;
	#define SOUND_CAPTURE_FREQUENCE	22050
	#define	SOUND_CAPTURE_BUFFSIZE	SOUND_CAPTURE_FREQUENCE
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
#ifdef ANDROID
		friend class	cOpenSLRecord;
#endif
		friend class	cSoundCapture;
		friend void		RecordingThread(size_t _workParameter, size_t _pUri);
		friend void		RecordingDoneThread(size_t _workParameter, size_t _pUri);
	private:
		//ensure callback is start
		bool			m_bStartCallBackCalled;
		bool			m_bEndCallBackCalled;
		void			PreCaptureSoundStartCallBack(cSoundCapture*e_pSoundCapture);
		virtual void	CaptureSoundStartCallBack() = 0;
		void			PreCaptureSoundEndCallBack();
		virtual void	CaptureSoundEndCallBack() = 0;
	protected:
		cSoundCapture*	m_pSoundCapture;
	public:
		cSounRecordCallBackObject(){m_pSoundCapture = nullptr; m_bStartCallBackCalled = false;m_bEndCallBackCalled = false; }
		virtual ~cSounRecordCallBackObject(){}
		//the data size is short,so the total size is e_iSamplesIn*sizeof(short)
		virtual void	CaptureSoundNewDataCallBack(ALCint e_iSamplesIn,char*e_pData) = 0;
	};

	//===================================
	//DO NOT ADD ANY cSounRecordCallBackObject AFTER StartRecord
	//
	//
	//===================================
	void	RecordingThread(size_t _workParameter, size_t _pUri);
	void	RecordingDoneThread(size_t _workParameter, size_t _pUri);
	class cSoundCapture:public cNamedTypedObjectVector<cSounRecordCallBackObject>,public cMessageSender
	{
		friend void	RecordingThread(size_t _workParameter, size_t _pUri);
		friend void	RecordingDoneThread(size_t _workParameter, size_t _pUri);

		bool					GamePauseEventFromGameApp(void*e_pData);
	protected:
		cFUThreadPool*			m_pFUThreadPool;
		bool					m_bPause;
		bool					m_bStop;
		bool					m_bThreadExitStop;
		//std::string				m_strSaveFileName;
		//cSoundFile*				m_pSoundFile;
#ifndef ANDROID
		ALCdevice*				m_pDevice;
#endif
		int						m_iFormat;
		int						m_iFrequency;
		int						m_iWriteChannel;
		int						m_iWriteBitpersample;
		int						m_iBufferSize;
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
		int								GetFrequency(){return m_iFrequency;}
		int								GetWriteChannel(){return m_iWriteChannel;}
		int								GetWriteBitpersample(){return m_iWriteBitpersample;}
		bool							IsRecording(){return m_bIsRecording;}
		void							AddFileSize(int e_iFileSize);
		int								GetFileSize(){return m_iFileSize;}
		int								GetBuffersize(){return m_iBufferSize;}
		float							GetCurrntTime(){return m_fCurrntTime;}
		//record sound to file
		bool							AddSoundRecord(std::string e_strFileName = "Record.ogg",eCaptureSoundFileFormat	e_eCaptureSoundFileFormat = eCSFF_OGG,int e_iOggWriteChannel = 1);
		static std::vector<std::string>	GetAvalibeRecordDevice();
		bool							IsDeviceOpen();
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
		int						m_iOggWriteChannel;
		cSoundFile*				m_pSoundFile;
		virtual void			CaptureSoundStartCallBack()override;
		virtual void			CaptureSoundEndCallBack()override;
		void					Destory();
	public:
		DEFINE_TYPE_INFO()
		//e_iOggWriteChannel only for ogg
		cSounRecordToFileCallBackObject(std::string e_strFileName,eCaptureSoundFileFormat	e_eCaptureSoundFileFormat = eCSFF_OGG,int e_iOggWriteChannel = 2);
		~cSounRecordToFileCallBackObject();
		virtual void			CaptureSoundNewDataCallBack(ALCint e_iSamplesIn,char*e_pData)override;
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
//http://stackoverflow.com/questions/4281058/how-to-let-user-select-a-audio-recording-device-with-openal
//// Get list of available Capture Devices
//const ALchar *pDeviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
//if (pDeviceList)
//{
//    ALFWprintf("\nAvailable Capture Devices are:-\n");
//
//    while (*pDeviceList)
//    {
//        ALFWprintf("%s\n", pDeviceList);
//        pDeviceList += strlen(pDeviceList) + 1;
//    }
//}
//
//// Get the name of the 'default' capture device
//szDefaultCaptureDevice = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
//ALFWprintf("\nDefault Capture Device is '%s'\n\n", szDefaultCaptureDevice);