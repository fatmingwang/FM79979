#include "../stdafx.h"
#include "SoundFile.h"
#include "SoundCapture.h"
#if defined(USE_SOUND_CAPTURE)

#include "../GameplayUT/BinaryFile.h"
//#include <thread>
#include "../Synchronization/FUThreadPool.h"
#include "../GameplayUT/GameApp.h"
namespace	FATMING_CORE
{
	cSoundCapture*cSoundCapture::m_spSoundCapture = nullptr;
	extern float g_fTest;
	extern float g_fTest2;
	#define CAP_SIZE 2048 // How much to capture at a time (affects latency)
	void	RecordingThread(size_t _workParameter, size_t _pUri)
	{
		cSoundCapture*l_pSoundCapture = (cSoundCapture*)_workParameter;
		cSoundFile*l_pSoundFile = l_pSoundCapture->GetSoundFile();
		if( !l_pSoundCapture->GetDevice() )
		{
			UT::ErrorMsg("no capture device exists","ERROR!");
			return;
		}
		alcCaptureStart(l_pSoundCapture->GetDevice());
		ALCenum l_ErrorCode = alcGetError(l_pSoundCapture->GetDevice());
		if(  l_ErrorCode != AL_NO_ERROR)
		{
			UT::ErrorMsg("capture start failed ","ERROR!");
			return;
		}
		
		unsigned char* l_pBuffer = new unsigned char[l_pSoundCapture->GetSampleRate()*2]; // A buffer to hold captured audio
		ALCint l_iSamplesIn = 0;  // How many samples are captured
		int l_iTest = 0;
		while(!l_pSoundCapture->IsStop())
		{
			if(l_pSoundCapture->IsPause())
			{
				Sleep(1);
				continue;
			}
			else
			{
				// Poll for captured audio
				alcGetIntegerv(l_pSoundCapture->GetDevice(),ALC_CAPTURE_SAMPLES,1,&l_iSamplesIn);
				if (l_iSamplesIn>CAP_SIZE) {
#define SRROUND_SOUND_TEST
#ifdef SRROUND_SOUND_TEST
					g_fTest = sin((float)l_iTest/360*20);
					g_fTest2 = cos((float)l_iTest/360*20);
					// Grab the sound
					++l_iTest;
#endif
					//because the open al give short data here need to conver to char size
					alcCaptureSamples(l_pSoundCapture->GetDevice(),l_pBuffer,l_iSamplesIn);
					l_iSamplesIn = l_iSamplesIn*sizeof(short)/sizeof(char)*l_pSoundCapture->GetWriteChannel();
					//cFUSynchronizedHold	l_cFUSynchronizedHold(l_pSoundCapture->GetSynchronized());
					if(l_pSoundCapture->GetCaptureSoundFileFormat() == eCSFF_OGG)
						l_pSoundFile->WriteOggData(l_iSamplesIn,(char*)l_pBuffer,l_pSoundCapture->GetWriteChannel());
					else
						l_pSoundFile->WriteWavData(l_iSamplesIn,(unsigned char*)l_pBuffer);
					//***** Process/filter captured data here *****//
					//for (int ii=0;ii<CAP_SIZE;++ii) {
					//  buffer[ii]*=0.1; // Make it quieter
					//}
				}
			}
		}
		delete l_pBuffer;
		// Stop capture
		alcCaptureStop(l_pSoundCapture->GetDevice());
		if(l_pSoundCapture->GetCaptureSoundFileFormat() == eCSFF_OGG)
			l_pSoundFile->EndWriteOggData();
		else
			l_pSoundFile->EndWriteWavFile();
		l_pSoundCapture->SetStop(false);
	}
	void	RecordingDoneThread(size_t _workParameter, size_t _pUri)
	{

	}
	//const int SRATE = 44100;
	//const int SSIZE = 1024;
	//
	//ALbyte buffer[22050];
	//ALint sample;
	cSoundCapture*g_pSoundCapture = nullptr;
	cSoundCapture::cSoundCapture(ALCuint frequency, ALCenum format, ALCsizei buffersize)
	{
		m_bIsRecording = false;
		m_pSoundFile = nullptr;
		m_pFUThreadPool = nullptr;
		m_bPause = false;
		m_bStop = false;
		m_pDevice = nullptr;

		if( format == AL_FORMAT_MONO16 || format == AL_FORMAT_STEREO16 )
			m_iWriteBitpersample = 16;
		else
		if( format == AL_FORMAT_MONO8 || format == AL_FORMAT_STEREO8 )
			m_iWriteBitpersample = 8;
		else
		{
			UT::ErrorMsg("not support format!",ValueToString(format).c_str());
		}
		if( format == AL_FORMAT_STEREO8 || format == AL_FORMAT_STEREO16 )
			m_iWriteChannel = 2;
		else
		if( format == AL_FORMAT_MONO16 || format == AL_FORMAT_MONO8 )
			m_iWriteChannel = 1;
		else
		{
			UT::ErrorMsg("not support format!",ValueToString(format).c_str());
		}
		if(!cGameApp::m_spSoundParser)
		{
			UT::ErrorMsg("please init GameApp","Error!");
			return;
		}
		if( g_pSoundCapture )
		{
			UT::ErrorMsg("multi sound capture is not allowed!","Error!");
			return;
		}
		g_pSoundCapture = this;
		m_iSampleRate = frequency;
		m_pDevice = alcCaptureOpenDevice(NULL, frequency, format, buffersize);
		if (alGetError() != AL_NO_ERROR)
		{
			if( m_pDevice )
				alcCaptureCloseDevice(m_pDevice);
			m_pDevice = nullptr;
		}
	}

	cSoundCapture::~cSoundCapture()
	{
		SAFE_DELETE(m_pFUThreadPool);
		SAFE_DELETE(m_pSoundFile);
		if( m_pDevice )
		{
			alcCaptureStop(m_pDevice);
			alcCaptureCloseDevice(m_pDevice);
		}
		g_pSoundCapture = nullptr;
	}

	void	cSoundCapture::Update(float e_fElpaseTime)
	{

	}

	//this is echo sample.
	//http://stackoverflow.com/questions/4087727/openal-how-to-create-simple-microphone-echo-programm
	bool	cSoundCapture::StartRecord(std::string e_strFileName,eCaptureSoundFileFormat	e_eCaptureSoundFileFormat)
	{
		if(this->m_bStop||this->m_bPause)
			return false;
		if(m_pDevice)
		{
			m_eCaptureSoundFileFormat = e_eCaptureSoundFileFormat;
			m_bIsRecording = true;
			this->m_bStop = false;
			this->m_bPause = false;
			m_strSaveFileName = e_strFileName;
			SAFE_DELETE(m_pSoundFile);
			m_pSoundFile = new cSoundFile();
			if(GetCaptureSoundFileFormat() == eCSFF_OGG)
				m_pSoundFile->StartWriteOggData(m_strSaveFileName.c_str(),m_iSampleRate,m_iWriteChannel);
			else
			{
				//   ByteRate         == SampleRate * NumChannels * BitsPerSample/8
				int l_iByteRate = m_iSampleRate*m_iWriteChannel*m_iWriteBitpersample/8;
				m_pSoundFile->SetWAVFmtHdr(1,this->m_iWriteChannel,m_iSampleRate,l_iByteRate,4,m_iWriteBitpersample);
				m_pSoundFile->StartWriteWavFile(m_strSaveFileName.c_str());
			}
			//m_pSoundFile->Writefile(e_strFileName.c_str(),true,false);
			//
			SAFE_DELETE(m_pFUThreadPool);
			this->m_pFUThreadPool = new cFUThreadPool();
			this->m_pFUThreadPool->Spawn(1);
			FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&RecordingThread);
			FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&RecordingDoneThread);
			this->m_pFUThreadPool->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);

			return true;
		}
		return false;
	
	}
	void	cSoundCapture::PauseRecord(bool e_bPause)
	{
		m_bPause = e_bPause;
	}

	void	cSoundCapture::StopRecord()
	{
		this->m_bStop = true;;
		m_bIsRecording = false;
	}
}
//end defined #if defined(USE_SOUND_CAPTURE)
#endif