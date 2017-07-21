#include "stdafx.h"
#include "SoundFFTCapture.h"
#include "FFTFrequency.h"
#include "KissFFTFunction.h"
void	SoundFFTCaptureKissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri)
{
	cSoundFFTCapture*l_pSoundFFTCapture = (cSoundFFTCapture*)_workParameter;
	float	l_fTimeElpasee = 0.f;
	float	l_fPreviousTime = 0.f;
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
		{
			l_fTimeElpasee = l_pTimeAndPCMData->fStartTime-l_fPreviousTime;
			l_fPreviousTime = l_pTimeAndPCMData->fStartTime;
			int*l_pFFTData = l_pSoundFFTCapture->m_PCMToFFTDataConvertr.ProcessFFTData(l_pTimeAndPCMData,l_pSoundFFTCapture->m_fNextSampleTime,l_pSoundFFTCapture->m_iOneFrameFFTDataCount);
		}
	}
}

void	SoundFFTCaptureKissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri)
{
	cSoundFFTCapture*l_pSoundFFTCapture = (cSoundFFTCapture*)_workParameter;
	l_pSoundFFTCapture->m_bThreadAlreadyStop = true;
}

cSoundFFTCapture::cSoundFFTCapture()
{
	m_pvFFTDataToPoints = new Vector2[cSoundCompareParameter::FFT_DATA_LINE_POINTS_COUNT];
	m_pQuickFFTDataFrequencyFinder = nullptr;
	m_bThreadInPause = false;
	m_bThreadStop = true;
	m_bThreadPause = false;
	m_bThreadAlreadyStop = true;
	m_pFUThreadPool = nullptr;
	m_fNextSampleTime = 1.f/BEST_RECORD_FPS;
	//m_vResolution.x *= 2;
	//this->m_vShowPos.x += 0;
	m_vShowPos.y = 670;
	m_vShowPos.x = 100;
	//m_vResolution.x = 5000;
	m_iUpdateFlag = SOUNS_FFT_CAPTURE_UPDATE_DRAW_LINES_FLAG;
	m_piFFTData = nullptr;
	m_iDivideFFTDataToNFrame = BEST_RECORD_FPS;
}

cSoundFFTCapture::~cSoundFFTCapture()
{
	Destroy();
	SAFE_DELETE(m_pvFFTDataToPoints);
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
	SAFE_DELETE(m_piFFTData);
	DELETE_VECTOR(m_TimeAndPCMDataVector,sTimeAndPCMData*);
	SAFE_DELETE(m_pFUThreadPool);
	SAFE_DELETE(m_pQuickFFTDataFrequencyFinder);
}


void	cSoundFFTCapture::CaptureSoundStartCallBack()
{
	Destroy();
	m_bThreadAlreadyStop = false;
	m_bThreadInPause = false;
	m_bThreadPause = false;
	m_bThreadStop = false;
	m_iCurrentStreamingBufferDataIndex = 0;
	int l_iFrequency = m_pSoundCapture->GetFrequency();
	float l_fFPSTime = 1.f/BEST_RECORD_FPS;
	m_fNextSampleTime = l_fFPSTime;
	cKissFFTConvertBase::SetOneFrameFFTDataCount(l_iFrequency);
	int l_iMaxFFTCount = this->m_pSoundCapture->GetBuffersize()/sizeof(short);
	if( m_iOneFrameFFTDataCount > l_iMaxFFTCount )
	{
		m_iOneFrameFFTDataCount = l_iMaxFFTCount;
	}
	m_piFFTData = new int[m_iOneFrameFFTDataCount];
	m_pQuickFFTDataFrequencyFinder = new cQuickFFTDataFrequencyFinder(m_iOneFrameFFTDataCount/WINDOWN_FUNCTION_FRUSTRUM,l_iFrequency);
	m_pQuickFFTDataFrequencyFinder->SetFFTData(m_piFFTData);

	if( cSoundCompareParameter::m_seMusicGameMode == eMGM_EDITOR )
	{
		this->m_pFFTDataStore = new cFFTDecibelsAnalyzer();
		this->m_pFFTDataStore->Start(l_iFrequency);
	}

	this->m_PCMToFFTDataConvertr.SetNFrameFFTDataCount(m_iOneFrameFFTDataCount);
	this->m_pFUThreadPool = new cFUThreadPool();
	this->m_pFUThreadPool->Spawn(1);
	FUStaticFunctor2<size_t, size_t, void>* workFunctor1 = new FUStaticFunctor2<size_t, size_t, void>(&SoundFFTCaptureKissFFTStreamingConvertThread);
	FUStaticFunctor2<size_t, size_t, void>* doneFunctor1 = new FUStaticFunctor2<size_t, size_t, void>(&SoundFFTCaptureKissFFTStreamingConvertThreadDone);
	this->m_pFUThreadPool->ExecuteWork(workFunctor1,doneFunctor1,(size_t)this,0);
}

void	cSoundFFTCapture::CaptureSoundNewDataCallBack(ALCint e_iSamplesIn,char*e_pData)
{
		//the performance is too bad...skip this one
		assert(e_iSamplesIn >= this->m_iOneFrameFFTDataCount && "PCM buffer could must gerat than m_iOneFrameFFTDataCount,because I am lazy to fix this!.");
		e_iSamplesIn = this->m_iOneFrameFFTDataCount;
		if( m_TimeAndPCMDataVector.size()+1 >= PCM_SWAP_BUFFER_COUNT)
		{
#ifdef DEBUG
			cGameApp::OutputDebugInfoString(L"StreamingBuffer is over size...fuck");
#endif
			//
			return;
		}
		float l_fCurrentTime = m_fCurrentTime+this->m_fNextSampleTime;
		eDataType l_eDataType = eDataType::eDT_SHORT;
		if(this->m_pSoundCapture->GetWriteBitpersample() == 16)
			l_eDataType = eDataType::eDT_SHORT;
		else
		if(this->m_pSoundCapture->GetWriteBitpersample() == 8)
			l_eDataType = eDataType::eDT_BYTE;
		else
		{
			UT::ErrorMsg(L"Error",L"sound capture bit persample is not 8 or 16");
		}
		int l_iOneSampleSize = this->m_pSoundCapture->GetWriteChannel()*l_eDataType == eDataType::eDT_BYTE?1:2;
		memcpy(m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex],e_pData,e_iSamplesIn*l_iOneSampleSize);
		sTimeAndPCMData*l_pTimeAndPCMData = new sTimeAndPCMData(l_fCurrentTime,l_fCurrentTime+this->m_fNextSampleTime,this->m_pSoundCapture->GetWriteChannel(),m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex],e_iSamplesIn,l_eDataType);
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
		//memcpy(m_PreviousStreamingBufferData,e_pData,e_iSamplesIn);
//#ifdef DEBUG
//		float l_pfshortToFloatArray[3000];
//		short*l_pSrouce = (short*)m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex];
//		for(size_t i=0;i<e_iSamplesIn/sizeof(short);++i)
//		{
//			l_pfshortToFloatArray[i] = (float)l_pSrouce[i];
//		}
//		//double l_dbValue = GoertzelFilter(l_pfshortToFloatArray,this->m_pSoundCapture->GetFrequency(),20,60,int SIGNAL_SAMPLE_RATE);
//#endif

}

void	cSoundFFTCapture::CaptureSoundEndCallBack()
{
	cFUSynchronizedHold	l_cFUSynchronizedHold(&m_PCMToFFTDataConvertr.m_FUSynchronizedForTimeAndFFTDataVector);
	Destroy();
}


void	cSoundFFTCapture::Update(float e_fElpaseTime)
{
	if( !this->m_bPause )
		this->m_fCurrentTime += e_fElpaseTime;
	if(	m_iUpdateFlag && SOUNS_FFT_CAPTURE_UPDATE_DRAW_LINES_FLAG )
	{
		UpdateWithFetchFFTData(e_fElpaseTime);
	}
	if(	m_iUpdateFlag && SOUNS_FFT_CAPTURE_UPDATE_FETCH_FFT_DATA_FLAG )
	{
		UpdateWithDrawFFTData(e_fElpaseTime);
	}
}

void	cSoundFFTCapture::UpdateWithFetchFFTData(float e_fElpaseTime)
{
	//dont need to do synchronzied,because only here will delete
	while(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size())
	{
		//if(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLinesByFFTSampleTargetIndex(this->m_vFFTDataToPoints,0,this->m_vShowPos,this->m_vResolution,this->m_fScale,this->m_fNextChannelYGap))
		if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GetCurrentFFTData(this->m_piFFTData,m_fCurrentTime) == -1 )
		//if( !m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLines(this->m_vFFTDataToPoints,m_fCurrentTime,this->m_vShowPos,this->m_vResolution,this->m_fScale,this->m_fNextChannelYGap) )
		{
			static int l_iTest = 0;
			++l_iTest;
			if( l_iTest % 10 == 0 )
			{
				//const int l_iFetchDataCount = (int)(m_iOneFrameFFTDataCount*m_PCMToFFTDataConvertr.GetFFTSampleScale());
				const int l_iFetchDataCount = (int)(m_iOneFrameFFTDataCount);
				this->GetCurrentMaxFrequence(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->iBiggestFFTDataValueOfIndex,this->m_pSoundCapture->GetFrequency(),l_iFetchDataCount,m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->iBiggestAmplitude);
			}
			cFUSynchronizedHold	l_cFUSynchronizedHold(&m_PCMToFFTDataConvertr.m_FUSynchronizedForTimeAndFFTDataVector);
			//wait for next new one.
			if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() == 1 )
				break;
			delete m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0];
			m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.erase(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.begin());
		}
		else
		{
			break;
		}
	}
}
void	cSoundFFTCapture::UpdateWithDrawFFTData(float e_fElpaseTime)
{
	//dont need to do synchronzied,because only here will delete
	while(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size())
	{
		//if(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLinesByFFTSampleTargetIndex(this->m_vFFTDataToPoints,0,this->m_vShowPos,this->m_vResolution,this->m_fScale,this->m_fNextChannelYGap))
		if( !m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLines(this->m_pvFFTDataToPoints,m_fCurrentTime,this->m_vShowPos,this->m_vResolution*m_fChartScale,this->m_fChartAmplitudeScale) )
		//if( !m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLines(this->m_vFFTDataToPoints,m_fCurrentTime,this->m_vChartShowPos,this->m_vChartResolution,this->m_fScale,this->m_fNextChannelYGap) )
		{
			cFUSynchronizedHold	l_cFUSynchronizedHold(&m_PCMToFFTDataConvertr.m_FUSynchronizedForTimeAndFFTDataVector);
			//wait for next new one.
			if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() == 1 )
				break;
			if( m_pFFTDataStore )
				m_pFFTDataStore->UpdateFFTData(e_fElpaseTime,this->m_piFFTData,m_iOneFrameFFTDataCount/WINDOWN_FUNCTION_FRUSTRUM);
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
			break;
		}
	}
}

void	cSoundFFTCapture::Render()
{
	if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() )
	{
		if( cGameApp::m_sbDebugFunctionWorking )
		{
			cPCMToFFTDataConvertr::sTimeAndFFTData*l_pTimeAndFFTData = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0];
			int l_iNumPointd = l_pTimeAndFFTData->iFFTDataOneSample*2;//*2 for one line 2 points
			//left channel
			GLRender::RenderLine((float*)m_pvFFTDataToPoints,l_iNumPointd,Vector4::One,2);
		}

		//if( m_fChartScale >= 2 )
		//{
		//	//int l_iFreqGap = this->m_pOpanalOgg->GetFreq()/l_pTimeAndFFTData->iFFTDataOneSample;
		//	const int l_ciFreqNeeded = 100;
		//	//int l_iFreqGap = this->m_pOpanalOgg->GetFreq()/l_ciFreqNeeded;
		//	float l_fXGap = m_vChartResolution.x/l_ciFreqNeeded/4*this->m_fChartScale;
		//	Vector2 l_LinePos[l_ciFreqNeeded*2];
		//	for(int i=0;i<l_ciFreqNeeded;i++)
		//	{
		//		l_LinePos[i*2].x = i*l_fXGap+m_vChartShowPos.x;
		//		l_LinePos[i*2+1].x = i*l_fXGap+m_vChartShowPos.x;
		//		l_LinePos[i*2].y = m_vChartShowPos.y;
		//		l_LinePos[i*2+1].y = 20*m_fScale+m_vChartShowPos.y;
		//		
		//		cGameApp::RenderFont(l_LinePos[i*2].x,l_LinePos[i*2+1].y+30,ValueToStringW(i*this->m_pSoundCapture->GetFrequency()/l_ciFreqNeeded).c_str());
		//	}
		//	GLRender::RenderLine((float*)l_LinePos,l_ciFreqNeeded*2,Vector4::Red,2);
		//}
		const int l_iFetchDataCount = (int)(m_iOneFrameFFTDataCount);
		this->GetCurrentMaxFrequence(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->iBiggestFFTDataValueOfIndex,this->m_pSoundCapture->GetFrequency(),l_iFetchDataCount,m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->iBiggestAmplitude);
		RenderMaxAmplitudeAndFrequencyInfo(1000,200);
		RenderDebugAmplitudeLine((float)cSoundCompareParameter::m_siDebugAmplitudeValue);

		if( m_pFFTDataStore )
				m_pFFTDataStore->RenderCurrentData();
	}
}

int		cSoundFFTCapture::GetOpanalCaptureBufferSize(int e_iFPS,int e_iFrequence,bool e_bFFTCompatibility,ALCenum e_Fromat)
{
	int l_OneSampleNumByte = 1;
	int l_iChannel = 1;
	if( e_Fromat == AL_FORMAT_STEREO8 || e_Fromat == AL_FORMAT_STEREO16 )
	{
		l_iChannel = 2;
	}
	if( e_Fromat == AL_FORMAT_MONO16 || e_Fromat == AL_FORMAT_STEREO16 )
		l_OneSampleNumByte = 2;
	float l_fFPSTime = 1.f/e_iFPS;
	int l_iBufferSize = (int)(e_iFrequence*l_fFPSTime)*l_iChannel*l_OneSampleNumByte;
	if( e_bFFTCompatibility )
		l_iBufferSize = power_of_two(l_iBufferSize);
	return l_iBufferSize;
}