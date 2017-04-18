#include "stdafx.h"
#include "SoundFFTCapture.h"

void	SoundFFTCaptureKissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri)
{
	cSoundFFTCapture*l_pSoundFFTCapture = (cSoundFFTCapture*)_workParameter;
	while( !l_pSoundFFTCapture->m_bThreadStop )
	{
		if( cGameApp::m_sbGamePause ||l_pSoundFFTCapture->m_bThreadPause)
		{
			Sleep(10);
			l_pSoundFFTCapture->m_bThreadInPause = true;
			continue;
		}
		l_pSoundFFTCapture->m_bThreadInPause = false;
		sTimeAndPCMData*l_pTimeAndPCMData = nullptr;
		{
			cFUSynchronizedHold	l_cFUSynchronizedHold(&l_pSoundFFTCapture->m_FUSynchronizedForTimeAndPCMDataVector);
			if(l_pSoundFFTCapture->m_TimeAndPCMDataVector.size())
			{
				float l_fCurrentTime = l_pSoundFFTCapture->m_pSoundCapture->GetCurrntTime();
				float l_fEndTime = l_fCurrentTime+l_pSoundFFTCapture->m_fNextSampleTime;
				l_pTimeAndPCMData = l_pSoundFFTCapture->m_TimeAndPCMDataVector[0];
				l_pSoundFFTCapture->m_TimeAndPCMDataVector.erase(l_pSoundFFTCapture->m_TimeAndPCMDataVector.begin());
			}
		}
		if( l_pTimeAndPCMData != nullptr )
			l_pSoundFFTCapture->m_PCMToFFTDataConvertr.ProcessFFTData(l_pTimeAndPCMData,l_pSoundFFTCapture->m_fNextSampleTime,l_pSoundFFTCapture->m_pSoundCapture->GetWriteChannel(),l_pSoundFFTCapture->m_iNFrameFFTDataCount);
	}
}

void	SoundFFTCaptureKissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri)
{
	cSoundFFTCapture*l_pSoundFFTCapture = (cSoundFFTCapture*)_workParameter;
	l_pSoundFFTCapture->m_bThreadAlreadyStop = true;
}

cSoundFFTCapture::cSoundFFTCapture()
{
	m_bThreadInPause = false;
	m_bThreadStop = true;
	m_bThreadPause = false;
	m_bThreadAlreadyStop = true;
	m_pFUThreadPool = nullptr;
	//m_iFFTSampleForOneFrame = 0;
	m_fNextSampleTime = 1.f/ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;
	this->m_vChartShowPos.x += 800;
}

cSoundFFTCapture::~cSoundFFTCapture()
{
	Destroy();
	m_bThreadInPause = false;
	m_iCurrentStreamingBufferDataIndex = 0;
	m_pFUThreadPool = nullptr;
	m_bThreadStop = false;
	m_bThreadAlreadyStop = true;
}

void	cSoundFFTCapture::Destroy()
{
	m_bThreadStop = true;
	while( !m_bThreadAlreadyStop){
		//wait for thread end...
		Sleep(1);
	}
	m_PCMToFFTDataConvertr.Destroy();
	DELETE_VECTOR(m_TimeAndPCMDataVector,sTimeAndPCMData*);
	SAFE_DELETE(m_pFUThreadPool);
}


void	cSoundFFTCapture::CaptureSoundStartCallBack()
{
	Destroy();
	m_bThreadAlreadyStop = false;
	m_bThreadInPause = false;
	m_bThreadPause = false;
	m_bThreadStop = false;
	m_iCurrentStreamingBufferDataIndex = 0;
	int l_iSampleRate = m_pSoundCapture->GetSampleRate();
	float l_fFPSTime = 1.f/ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;
	m_fNextSampleTime = l_fFPSTime;
	//m_iFFTSampleForOneFrame = (int)(l_iSampleRate * l_fFPSTime);
	m_iNFrameFFTDataCount = (int)(l_iSampleRate * l_fFPSTime)/sizeof(short);
	if( m_iNFrameFFTDataCount >= this->m_pSoundCapture->GetBuffersize()/2)
	{
		m_iNFrameFFTDataCount = this->m_pSoundCapture->GetBuffersize()/2;
	}
	this->m_PCMToFFTDataConvertr.SetNFrameFFTDataCount(m_iNFrameFFTDataCount);
	this->m_pFUThreadPool = new cFUThreadPool();
	this->m_pFUThreadPool->Spawn(1);
	FUStaticFunctor2<size_t, size_t, void>* workFunctor1 = new FUStaticFunctor2<size_t, size_t, void>(&SoundFFTCaptureKissFFTStreamingConvertThread);
	FUStaticFunctor2<size_t, size_t, void>* doneFunctor1 = new FUStaticFunctor2<size_t, size_t, void>(&SoundFFTCaptureKissFFTStreamingConvertThreadDone);
	this->m_pFUThreadPool->ExecuteWork(workFunctor1,doneFunctor1,(size_t)this,0);
}

void	cSoundFFTCapture::CaptureSoundNewDataCallBack(ALCint e_iSamplesIn,char*e_pData)
{
		//the performance is too bad...skip this one
		if( m_TimeAndPCMDataVector.size()+1 >= PCM_SWAP_BUFFER_COUNT)
		{
#ifdef DEBUG
			cGameApp::OutputDebugInfoString(L"StreamingBuffer is over size...fuck");
#endif
			//
			return;
		}
		//memset(m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex],0,OGG_STREAMING_SOUND_BUFFER_SIZE);
		memcpy(m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex],e_pData,e_iSamplesIn);
		//if( e_iSamplesIn%2 )
		//{
		//	int a=0;
		//}
		//short*l_pData = (short*)m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex];
		//int l_iNum = 0;
		//int l_iNum2 = 0;
		//for(ALCint i=0;i<e_iSamplesIn/2;++i)
		//{
		//	if( l_pData[i]<10 )
		//	{
		//		++l_iNum;
		//	}
		//	if( l_pData[i]<100 )
		//	{
		//		++l_iNum2;
		//		l_pData[i] = 0;
		//	}
		//}
		float l_fCurrentTime = m_fCurrentTime+this->m_fNextSampleTime;
		sTimeAndPCMData*l_pTimeAndPCMData = new sTimeAndPCMData(l_fCurrentTime,l_fCurrentTime+this->m_fNextSampleTime,m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex],e_iSamplesIn,m_iCurrentStreamingBufferDataIndex);
		{
			cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronizedForTimeAndPCMDataVector);
			m_TimeAndPCMDataVector.push_back(l_pTimeAndPCMData);
			if( m_TimeAndPCMDataVector.size()>=PCM_SWAP_BUFFER_COUNT )
			{
#ifdef DEBUG
				cGameApp::OutputDebugInfoString("streaming buffer is too small...");
#endif
			}
		}
		++m_iCurrentStreamingBufferDataIndex;
		if( m_iCurrentStreamingBufferDataIndex >= PCM_SWAP_BUFFER_COUNT)
			m_iCurrentStreamingBufferDataIndex = 0;
}

void	cSoundFFTCapture::CaptureSoundEndCallBack()
{
	
}

void	cSoundFFTCapture::Update(float e_fElpaseTime)
{
	if( !this->m_bPause )
		this->m_fCurrentTime += e_fElpaseTime;
	//dont need to do synchronzied,because only here will delete
	while(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size())
	{
		if( !m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLines(this->m_vFFTDataToPoints,m_fCurrentTime,this->m_vChartShowPos,this->m_vChartResolution,this->m_fScale,this->m_fNextChannelYGap) )
		{
			cFUSynchronizedHold	l_cFUSynchronizedHold(&m_PCMToFFTDataConvertr.m_FUSynchronizedForTimeAndFFTDataVector);
			//wait for next new one.
			if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() == 1 )
				break;
			float l_fStartTime = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->fStartTime;
			float l_fEndime = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->fEndTime;
			delete m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0];
			m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.erase(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.begin());
#ifdef DEBUG
			std::wstring l_strDebugInfo = L"2:S:";
			l_strDebugInfo += ValueToStringW(l_fStartTime);
			l_strDebugInfo += L",E:";
			l_strDebugInfo += ValueToStringW(l_fEndime);
			l_strDebugInfo += L":old data";
			cGameApp::OutputDebugInfoString(l_strDebugInfo);
#endif
		}
		else
		{
			static int l_iTest = 0;
			++l_iTest;
			if( l_iTest % 10 == 0 )
			{
				const int l_iFetchDataCount = (int)(m_iNFrameFFTDataCount*m_PCMToFFTDataConvertr.GetFFTSampleScale());
				this->m_iMaxFrequence = this->GetCurrentMaxFrequence(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->iBiggestFFTDataValueOfIndex,this->m_pSoundCapture->GetSampleRate(),l_iFetchDataCount);
			}
			break;
		}
	}
}

void	cSoundFFTCapture::Render()
{
	if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() )
	{
		cPCMToFFTDataConvertr::sTimeAndFFTData*l_pTimeAndFFTData = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0];
		int l_iNumPointd = l_pTimeAndFFTData->iFFTDataOneSample/2*2;
		//left channel
		GLRender::RenderLine((float*)m_vFFTDataToPoints,l_iNumPointd,Vector4::One,2);
		std::wstring l_strInfo = L"Frequence:";
		l_strInfo += ValueToStringW(this->m_iMaxFrequence);
		cGameApp::m_spGlyphFontRender->SetScale(3.f);
		cGameApp::RenderFont(1000,200,l_strInfo);
		cGameApp::m_spGlyphFontRender->SetScale(1.f);
		//right channel
		if( l_pTimeAndFFTData->iNumChannel == 2 )
		{
			int l_iRightChannelIndex = l_iNumPointd;
			GLRender::RenderLine((float*)m_vFFTDataToPoints[l_iRightChannelIndex],l_iNumPointd,Vector4::One,2);
		}
	}	
}

int		cSoundFFTCapture::GetOpanalCaptureBufferSize(int e_iFPS,int e_iFrequence)
{
	float l_fFPSTime = 1.f/e_iFPS;
	int l_iBufferSize = (int)(e_iFrequence*l_fFPSTime);
	return l_iBufferSize;
}

void	cSoundFFTCapture::SetFFTSampleScale(float e_fScale)
{
	if(e_fScale>1.f)
		e_fScale = 1.f;
	if(e_fScale<=0.f)
		e_fScale = 0.1f;
	this->m_PCMToFFTDataConvertr.SetFFTSampleScale(e_fScale);
}