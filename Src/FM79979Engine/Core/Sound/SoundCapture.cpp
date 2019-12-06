#include "SoundFile.h"
#include "SoundCapture.h"
#if defined(USE_SOUND_CAPTURE)
#include "../Common/Utility.h"
#include "../Common/BinaryFile/BinaryFile.h"
#include "../Common/CommonApp.h"
#include "../Common/StringToStructure.h"
//#include <thread>
#include "../Synchronization/FUThreadPool.h"
#include "SoundManager.h"
#ifndef WIN32
#include "unistd.h"
#endif
#ifdef IOS
#import <AVFoundation/AVFoundation.h>
#endif
namespace	FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cSounRecordToFileCallBackObject);
//#define SRROUND_SOUND_TEST
#ifdef SRROUND_SOUND_TEST
	extern float g_fTest;
	extern float g_fTest2;
#endif
#ifndef ANDROID
	void	RecordingThread(size_t _workParameter, size_t _pUri)
	{
		cSoundCapture*l_pSoundCapture = (cSoundCapture*)_workParameter;
//		cSoundFile*l_pSoundFile = l_pSoundCapture->GetSoundFile();
		if( !l_pSoundCapture->m_pDevice )
		{
			UT::ErrorMsg("no capture device exists","ERROR!");
			return;
		}
		alcCaptureStart(l_pSoundCapture->m_pDevice);
		ALCenum l_ErrorCode = alcGetError(l_pSoundCapture->m_pDevice);
		if(  l_ErrorCode != AL_NO_ERROR)
		{
			UT::ErrorMsg("capture start failed ","ERROR!");
			return;
		}
		UT::sTimeAndFPS	l_Timer;
		l_Timer.Update();
		auto l_CallbackObjectVector = l_pSoundCapture->GetList();
		int l_iNumCount = l_pSoundCapture->Count();
		for( int i=0;i<l_iNumCount;++i )
		{
			(*l_CallbackObjectVector)[i]->PreCaptureSoundStartCallBack(l_pSoundCapture);
		}
		l_pSoundCapture->m_bThreadExitStop = false;
		//2 for 
		unsigned char* l_pBuffer = new unsigned char[l_pSoundCapture->GetFrequency()*sizeof(short)]; // A buffer to hold captured audio
		ALCint l_iSamplesIn = 0;  // How many samples are captured
		int l_iTest = 0;		
		int l_iTargetSampleCount = l_pSoundCapture->GetBuffersize()/l_pSoundCapture->m_iWriteChannel/(l_pSoundCapture->m_iWriteBitpersample/8);
		int l_iOneSampleSize = l_pSoundCapture->m_iWriteChannel/(l_pSoundCapture->m_iWriteBitpersample/8);
		while(!l_pSoundCapture->IsStop())
		{
			l_pSoundCapture->Update(l_Timer.fElpaseTime);
			if(l_pSoundCapture->IsPause() || cCommonApp::m_sbGamePause)
			{
				Sleep(1);
				continue;
			}
			else
			{
				// Poll for captured audio
				alcGetIntegerv(l_pSoundCapture->m_pDevice,ALC_CAPTURE_SAMPLES,1,&l_iSamplesIn);
				if (l_iSamplesIn >= l_iTargetSampleCount) {
#ifdef SRROUND_SOUND_TEST
					g_fTest = sin((float)l_iTest/360*20);
					g_fTest2 = cos((float)l_iTest/360*20);
					// Grab the sound
					++l_iTest;
#endif
					//because the open al give short data here need to conver to char size
					alcCaptureSamples(l_pSoundCapture->m_pDevice,l_pBuffer,l_iSamplesIn);
					l_pSoundCapture->AddFileSize(l_iSamplesIn*l_iOneSampleSize);
					l_Timer.Update();
					FMLog::LogWithFlag(l_Timer.GetFPS(), CORE_LOG_FLAG);
					for( int i=0;i<l_iNumCount;++i )
					{
						(*l_CallbackObjectVector)[i]->CaptureSoundNewDataCallBack(l_iSamplesIn,(char*)l_pBuffer);
					}
				}
			}
		}
		delete[] l_pBuffer;
		// Stop capture
		alcCaptureStop(l_pSoundCapture->m_pDevice);

		for( int i=0;i<l_iNumCount;++i )
		{
			(*l_CallbackObjectVector)[i]->PreCaptureSoundEndCallBack();
		}
		l_pSoundCapture->m_bStop = false;
	}
	void	RecordingDoneThread(size_t _workParameter, size_t _pUri)
	{
		cSoundCapture*l_pSoundCapture = (cSoundCapture*)_workParameter;
		l_pSoundCapture->m_bThreadExitStop = true;
	}
#endif
	void	cSounRecordCallBackObject::PreCaptureSoundStartCallBack(cSoundCapture*e_pSoundCapture)
	{
		m_pSoundCapture = e_pSoundCapture;
		CaptureSoundStartCallBack();
		m_bStartCallBackCalled = true;
	}

	void	cSounRecordCallBackObject::PreCaptureSoundEndCallBack()
	{
		CaptureSoundEndCallBack();
		m_bEndCallBackCalled = true;
	}

	cSoundCapture*g_pSoundCapture = nullptr;
	cSoundCapture::cSoundCapture(ALCuint frequency, ALCenum format, ALCsizei buffersize)
	{
#ifdef IOS
        //http://www.cocoachina.com/bbs/read.php?tid=220163
        NSError *l_pError;
        [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayAndRecord withOptions:AVAudioSessionCategoryOptionMixWithOthers error: &l_pError];
        [[AVAudioSession sharedInstance] overrideOutputAudioPort: AVAudioSessionPortOverrideSpeaker error:&l_pError];
        [[AVAudioSession sharedInstance] setActive: YES error: nil];
#endif
		m_iFormat = format;
		m_bThreadExitStop = true;
		m_iBufferSize = buffersize;
		m_fCurrntTime = 0.f;
		m_bIsRecording = false;
		m_pFUThreadPool = nullptr;
#ifndef ANDROID
		m_pDevice = nullptr;
#endif
		m_bPause = false;
		m_bStop = false;
		m_iFileSize = 0;
		if( format == AL_FORMAT_MONO16 || format == AL_FORMAT_STEREO16 )
			m_iWriteBitpersample = 16;
		else
		if( format == AL_FORMAT_MONO8 || format == AL_FORMAT_STEREO8 )
			m_iWriteBitpersample = 8;
		else
		{
			UT::ErrorMsg("not support format!",FATMING_CORE::ValueToString(format).c_str());
		}
		if( format == AL_FORMAT_STEREO8 || format == AL_FORMAT_STEREO16 )
			m_iWriteChannel = 2;
		else
		if( format == AL_FORMAT_MONO16 || format == AL_FORMAT_MONO8 )
			m_iWriteChannel = 1;
		else
		{
			UT::ErrorMsg("not support format!", FATMING_CORE::ValueToString(format).c_str());
		}
		if( g_pSoundCapture )
		{
			UT::ErrorMsg("multi sound capture is not allowed!","Error!");
			return;
		}
		g_pSoundCapture = this;
		m_iFrequency = frequency;
		REG_EVENT(GAME_PAUSE_EVENT_ID,&cSoundCapture::GamePauseEventFromGameApp);
#ifdef ANDROID
		CreateAndroidAudioEngine();
		NativeAudioCreateAudioRecorder(frequency,m_iWriteBitpersample,m_iWriteChannel);
#endif
	}

	cSoundCapture::~cSoundCapture()
	{
		StopRecord();
		this->Destroy();
		SAFE_DELETE(m_pFUThreadPool);
#ifdef ANDROID
		AndroidRecordShutdown();
#else
		if( m_pDevice )
		{
			alcCaptureStop(m_pDevice);
			alcCaptureCloseDevice(m_pDevice);
		}
#endif
		g_pSoundCapture = nullptr;
	}

	void	cSoundCapture::Update(float e_fElpaseTime)
	{
			m_fCurrntTime += e_fElpaseTime;
	}

	//this is echo sample.
	//http://stackoverflow.com/questions/4087727/openal-how-to-create-simple-microphone-echo-programm
	bool	cSoundCapture::StartRecord()
	{
		if(this->m_bStop||this->m_bPause)
			return false;
#ifdef ANDROID
		auto l_CallbackObjectVector = GetList();
		int l_iNumCount = this->Count();
		for( int i=0;i<l_iNumCount;++i )
		{
			(*l_CallbackObjectVector)[i]->PreCaptureSoundStartCallBack(this);
		}
		StartAndroidRecording(this->m_iBufferSize);

#else
		if( !m_pDevice )
		{
			m_pDevice = alcCaptureOpenDevice(NULL, this->m_iFrequency, m_iFormat,this->m_iBufferSize*sizeof(short));
            auto l_Value = alGetError() ;
			if (l_Value != AL_NO_ERROR)
			{
				if( m_pDevice )
					alcCaptureCloseDevice(m_pDevice);
				m_pDevice = nullptr;
			}
		}
		if(m_pDevice)
		{
			this->m_fCurrntTime = 0.f;
			m_bIsRecording = true;
			m_iFileSize = 0;
			this->m_bStop = false;
			this->m_bPause = false;
			SAFE_DELETE(m_pFUThreadPool);
			this->m_pFUThreadPool = new cFUThreadPool();
			this->m_pFUThreadPool->Spawn(1);
			FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&RecordingThread);
			FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&RecordingDoneThread);
			this->m_pFUThreadPool->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
			return true;
		}
		else
		{
			UT::ErrorMsg("no record device found!","Error!");
		}
#endif
		return false;
	
	}


	bool	cSoundCapture::GamePauseEventFromGameApp(void*e_pData)
	{
		PauseRecord(cCommonApp::m_sbGamePause);
		return true;
	}

	void	cSoundCapture::PauseRecord(bool e_bPause)
	{
		m_bPause = e_bPause;
#ifdef ANDROID
		AndroidRecordPause(m_bPause);
#endif
	}


	void	cSoundCapture::StopRecord()
	{
#ifndef ANDROID
		this->m_bStop = true;;
		m_bIsRecording = false;
		if( !m_bThreadExitStop )
		{
			Sleep(100);
		}
		if( m_pDevice )
		{
			alcCaptureStop(m_pDevice);
			//alcCaptureCloseDevice(m_pDevice);
			//m_pDevice = nullptr;
		}
#else
		AndroidRecordStop();
		auto l_CallbackObjectVector = GetList();
		int l_iNumCount = this->Count();
		for( int i=0;i<l_iNumCount;++i )
		{
			(*l_CallbackObjectVector)[i]->PreCaptureSoundEndCallBack();
		}
#endif
		this->m_bStop = false;
        this->m_bPause = false;
	}


	void	cSoundCapture::AddFileSize(int e_iFileSize)
	{
		this->m_iFileSize += e_iFileSize;
	}

	bool	cSoundCapture::AddSoundRecord(std::string e_strFileName,eCaptureSoundFileFormat	e_eCaptureSoundFileFormat,int e_iOggWriteChannel)
	{
		cSounRecordToFileCallBackObject*l_pSounRecordToFileCallBackObject = new cSounRecordToFileCallBackObject(e_strFileName,e_eCaptureSoundFileFormat,e_iOggWriteChannel);
		return this->AddObject(l_pSounRecordToFileCallBackObject);
	}

	bool	cSoundCapture::IsDeviceOpen()
	{
#ifdef ANDROID
		return true;
#else
		return m_pDevice?true:false;
#endif
	}

	std::vector<std::string>	cSoundCapture::GetAvalibeRecordDevice()
	{
		std::vector<std::string> l_Result;
		// Get list of available Capture Devices
		const ALchar *pDeviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
		if (pDeviceList)
		{
			while (*pDeviceList)
			{
				std::string l_strDeviceName = pDeviceList;
				l_Result.push_back(l_strDeviceName);
				pDeviceList += strlen(pDeviceList) + 1;
			}
		}

		// Get the name of the 'default' capture device
		//const char*l_strDefualtDevice = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
		//std::string l_strDefualt = l_strDefualtDevice;
		//l_Result.push_back(l_strDefualt);
		return l_Result;
	}

	cSounRecordToFileCallBackObject::cSounRecordToFileCallBackObject(std::string e_strFileName,eCaptureSoundFileFormat	e_eCaptureSoundFileFormat,int e_iOggWriteChannel )
	{
		this->SetName(e_strFileName.c_str());
		m_iOggWriteChannel = e_iOggWriteChannel;
		m_pSoundFile = nullptr;	
		m_eCaptureSoundFileFormat = e_eCaptureSoundFileFormat;
		this->m_strSaveFileName = e_strFileName;
	}
	cSounRecordToFileCallBackObject::~cSounRecordToFileCallBackObject()
	{
		Destory();
	}

	void	cSounRecordToFileCallBackObject::Destory()
	{
		SAFE_DELETE(m_pSoundFile);
	}

	void	cSounRecordToFileCallBackObject::CaptureSoundStartCallBack()
	{
		Destory();
		if( m_strSaveFileName.length() == 0 )
		{
			return;
		}
		
		m_pSoundFile = new cSoundFile();
		int l_iFrequency = this->m_pSoundCapture->GetFrequency();
		int l_iWrtteChannel = this->m_pSoundCapture->GetWriteChannel();
		int l_iWriteBitpersample = this->m_pSoundCapture->GetWriteBitpersample();
		if(m_eCaptureSoundFileFormat == eCSFF_OGG)
		{
			if( this->m_pSoundCapture->GetWriteBitpersample() == 8)
			{
				UT::ErrorMsg(L"sorry ogg format only support for 16 bit now",L"Because I am lazy to fix this!");
			}
			m_pSoundFile->StartWriteOggData(m_strSaveFileName.c_str(),l_iFrequency,m_iOggWriteChannel,1);
		}
		else
		{
			//   ByteRate         == SampleRate * NumChannels * BitsPerSample/8
			int l_iByteRate = l_iFrequency*l_iWrtteChannel*l_iWriteBitpersample/8;
			m_pSoundFile->SetWAVFmtHdr(1,l_iWrtteChannel,l_iFrequency,l_iByteRate,4,l_iWriteBitpersample);
			m_pSoundFile->StartWriteWavFile(m_strSaveFileName.c_str());
		}
	}

	void	cSounRecordToFileCallBackObject::CaptureSoundNewDataCallBack(ALCint e_iSamplesIn,char*e_pData)
	{
		if( m_pSoundFile )
		{
			//the channel is 2,data size is double.
			int l_iChannel = this->m_pSoundCapture->GetWriteChannel();
			int l_iOneSampleSize = this->m_pSoundCapture->GetWriteChannel()*this->m_pSoundCapture->GetWriteBitpersample()/8;
			int l_iBufferSize = e_iSamplesIn*l_iOneSampleSize;
			if(m_eCaptureSoundFileFormat == eCSFF_OGG)
				this->m_pSoundFile->WriteOggData(l_iBufferSize,(char*)e_pData,l_iChannel);
			else
			{
				this->m_pSoundFile->WriteWavData(l_iBufferSize,(unsigned char*)e_pData);
			}
		}
	}

	void	cSounRecordToFileCallBackObject::CaptureSoundEndCallBack()
	{
		if( m_pSoundFile )
		{
			if(m_eCaptureSoundFileFormat == eCSFF_OGG)
				m_pSoundFile->EndWriteOggData();
			else
			{
				m_pSoundFile->EndWriteWavFile();
				//{
				//	cSoundFile*l_pSoundFile = new cSoundFile();
				//	l_pSoundFile->WavToOggFile(m_strSaveFileName.c_str(),"tt.ogg",0.5f,1);
				//}
			}
			SAFE_DELETE(m_pSoundFile);
		}
	}
}
//end defined #if defined(USE_SOUND_CAPTURE)
#endif
