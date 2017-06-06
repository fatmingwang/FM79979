#include "stdafx.h"
#include "SoundFFTCapture.h"
#include "QuickFFTDataFrequencyFinder.h"
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
			l_pSoundFFTCapture->m_PCMToFFTDataConvertr.ProcessFFTData(l_pTimeAndPCMData,l_pSoundFFTCapture->m_fNextSampleTime,l_pSoundFFTCapture->m_pSoundCapture->GetWriteChannel(),l_pSoundFFTCapture->m_iOneFrameFFTDataCount,l_pSoundFFTCapture->IsFilter(),l_pSoundFFTCapture->GetFrenquenceFilterEndScaleValue(),l_pSoundFFTCapture->GetFilterStrengthValue());
	}
}

void	SoundFFTCaptureKissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri)
{
	cSoundFFTCapture*l_pSoundFFTCapture = (cSoundFFTCapture*)_workParameter;
	l_pSoundFFTCapture->m_bThreadAlreadyStop = true;
}

cSoundFFTCapture::cSoundFFTCapture()
{
	m_pQuickFFTDataFrequencyFinder = nullptr;
	m_bThreadInPause = false;
	m_bThreadStop = true;
	m_bThreadPause = false;
	m_bThreadAlreadyStop = true;
	m_pFUThreadPool = nullptr;
	m_fNextSampleTime = 1.f/ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;
	//m_vChartResolution.x *= 2;
	//this->m_vChartShowPos.x += 0;
	//m_vChartResolution.x = 5000;
	m_iUpdateFlag = SOUNS_FFT_CAPTURE_UPDATE_DRAW_LINES_FLAG;
	m_piFFTData = nullptr;
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
	int l_iSampleRate = m_pSoundCapture->GetSampleRate();
	float l_fFPSTime = 1.f/ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;
	m_fNextSampleTime = l_fFPSTime;
	cKissFFTConvertBase::SetOneFrameFFTDataCount(l_iSampleRate);
	if( m_iOneFrameFFTDataCount > this->m_pSoundCapture->GetBuffersize())
	{
		m_iOneFrameFFTDataCount = this->m_pSoundCapture->GetBuffersize();
	}

	m_piFFTData = new int[m_iOneFrameFFTDataCount];
	m_pQuickFFTDataFrequencyFinder = new cQuickFFTDataFrequencyFinder(m_iOneFrameFFTDataCount/WINDOWN_FUNCTION_FRUSTRUM,l_iSampleRate);
	m_pQuickFFTDataFrequencyFinder->SetFFTData(m_piFFTData);
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
//#ifdef DEBUG
//		float l_pfshortToFloatArray[3000];
//		short*l_pSrouce = (short*)m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex];
//		for(size_t i=0;i<e_iSamplesIn/sizeof(short);++i)
//		{
//			l_pfshortToFloatArray[i] = (float)l_pSrouce[i];
//		}
//		//double l_dbValue = GoertzelFilter(l_pfshortToFloatArray,this->m_pSoundCapture->GetSampleRate(),20,60,int SIGNAL_SAMPLE_RATE);
//#endif

}

void	cSoundFFTCapture::CaptureSoundEndCallBack()
{
	
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
		//if(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLinesByFFTSampleTargetIndex(this->m_vFFTDataToPoints,0,this->m_vChartShowPos,this->m_vChartResolution,this->m_fScale,this->m_fNextChannelYGap))
		if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GetCurrentFFTData(this->m_piFFTData,m_fCurrentTime) == -1 )
		//if( !m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLines(this->m_vFFTDataToPoints,m_fCurrentTime,this->m_vChartShowPos,this->m_vChartResolution,this->m_fScale,this->m_fNextChannelYGap) )
		{
			static int l_iTest = 0;
			++l_iTest;
			if( l_iTest % 10 == 0 )
			{
				const int l_iFetchDataCount = (int)(m_iOneFrameFFTDataCount*m_PCMToFFTDataConvertr.GetFFTSampleScale());
				this->GetCurrentMaxFrequence(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->iBiggestFFTDataValueOfIndex,this->m_pSoundCapture->GetSampleRate(),l_iFetchDataCount,m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->iBiggestAmplitude);
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
	if( 1 )
	{
		//bool
		static std::vector<std::vector<int> >	l_iDebugInfoVectorVector;
		//for debug
		static bool l_bQKeyPress = false;
		if( cGameApp::m_sucKeyData['Q'] )
		{
			l_bQKeyPress = true;
		}
		//if( l_bQKeyPress && !cGameApp::m_sucKeyData['Q'] && m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() )
		if( cGameApp::m_sucKeyData['Q'] && m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() )
		{
			l_bQKeyPress = false;
			auto*l_pTimeAndFFTData = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0];
			int l_iFFTDataLength = l_pTimeAndFFTData->iFFTDataOneSample/WINDOWN_FUNCTION_FRUSTRUM;
			int l_iSampleRate = this->m_pSoundCapture->GetSampleRate();
			int l_iSampleGap = l_iSampleRate/l_iFFTDataLength;
			std::vector<int> l_Result;
			std::wstring l_strDebugInfo;
			for( int i=0;i<l_iFFTDataLength;++i )
			{
				if( this->m_piFFTData[i] >= cSoundCompareParameter::m_siDebugAmplitudeValue )
				{
					l_strDebugInfo += ValueToStringW(l_iSampleGap*i);
					l_strDebugInfo += L",";
					l_Result.push_back(l_iSampleGap*i);
				}
			}
			cGameApp::OutputDebugInfoString(l_strDebugInfo);
			l_iDebugInfoVectorVector.push_back(l_Result);
		}
		static bool l_bWKeyPress = false;
		if( cGameApp::m_sucKeyData['W'] )
		{
			l_bWKeyPress = true;
		}
		if( l_bWKeyPress && !cGameApp::m_sucKeyData['W'])
		{
			l_bWKeyPress = false;
			std::wstring l_strDebugInfo;
			std::map<int,int> l_FrequencyAndCount;
			for(auto l_ResultVector: l_iDebugInfoVectorVector)
			{
				for(auto l_Frequency: l_ResultVector)
				{
					if(l_FrequencyAndCount.find(l_Frequency) == l_FrequencyAndCount.end() )
					{
						l_FrequencyAndCount[l_Frequency] = 1;
					}
					else
					{
						l_FrequencyAndCount[l_Frequency] = l_FrequencyAndCount[l_Frequency]+1;
					}
				}
			}
			int l_iTotalCount = 0;
			int l_iFrequencyCount = 0;
			int l_iAverage = 0;
			for(std::map<int,int>::iterator l_Iterator = l_FrequencyAndCount.begin();l_Iterator!=l_FrequencyAndCount.end();++l_Iterator)
			{
				l_iTotalCount += l_Iterator->second;
				++l_iFrequencyCount;
			}
			l_iAverage = l_iTotalCount/l_iFrequencyCount;
			std::vector<int> l_FinalResultFrequencyVector;
			for(std::map<int,int>::iterator l_Iterator = l_FrequencyAndCount.begin();l_Iterator!=l_FrequencyAndCount.end();++l_Iterator)
			{
				if( l_Iterator->second >= l_iAverage )
				{
					l_FinalResultFrequencyVector.push_back(l_Iterator->first);

					std::wstring l_strLazyOutput = L"<FrequenceAndAmplitudeAndTime StartTime=\"0.01667\" KeepTime=\"0.11667\" LastMatchTime=\"0.13333\" Frequency=\"";
					l_strLazyOutput += ValueToStringW(l_Iterator->first);
					l_strLazyOutput += L"\" Amplitude=\"118\" />";
					cGameApp::OutputDebugInfoString(l_strLazyOutput);
				}
			}
			//cGameApp::OutputDebugInfoString(ValueToStringW(l_FinalResultFrequencyVector));
		}
	}
}
void	cSoundFFTCapture::UpdateWithDrawFFTData(float e_fElpaseTime)
{
	//dont need to do synchronzied,because only here will delete
	while(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size())
	{
		//if(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLinesByFFTSampleTargetIndex(this->m_vFFTDataToPoints,0,this->m_vChartShowPos,this->m_vChartResolution,this->m_fScale,this->m_fNextChannelYGap))
		if( !m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLines(this->m_vFFTDataToPoints,m_fCurrentTime,this->m_vChartShowPos,this->m_vChartResolution*m_fChartScale,this->m_fScale,this->m_fNextChannelYGap) )
		//if( !m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLines(this->m_vFFTDataToPoints,m_fCurrentTime,this->m_vChartShowPos,this->m_vChartResolution,this->m_fScale,this->m_fNextChannelYGap) )
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
			break;
		}
	}
}

void	cSoundFFTCapture::Render()
{
	if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() )
	{
		if( cSoundCompareParameter::m_sbDebugRender )
		{
			cPCMToFFTDataConvertr::sTimeAndFFTData*l_pTimeAndFFTData = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0];
			int l_iNumPointd = l_pTimeAndFFTData->iFFTDataOneSample/2*2;//*2 for one line 2 points,divide 2 for fft only have half count
			//left channel
			GLRender::RenderLine((float*)m_vFFTDataToPoints,l_iNumPointd,Vector4::One,2);
			//right channel
			if( l_pTimeAndFFTData->iNumChannel == 2 )
			{
				int l_iRightChannelIndex = l_iNumPointd;
				GLRender::RenderLine((float*)m_vFFTDataToPoints[l_iRightChannelIndex],l_iNumPointd,Vector4::One,2);
			}
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
		//		cGameApp::RenderFont(l_LinePos[i*2].x,l_LinePos[i*2+1].y+30,ValueToStringW(i*this->m_pSoundCapture->GetSampleRate()/l_ciFreqNeeded).c_str());
		//	}
		//	GLRender::RenderLine((float*)l_LinePos,l_ciFreqNeeded*2,Vector4::Red,2);
		//}
		RenderMaxAmplitudeAndFrequencyInfo(1000,200);
		RenderDebugAmplitudeLine((float)cSoundCompareParameter::m_siDebugAmplitudeValue);
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

void	cSoundFFTCapture::SetFFTSampleScale(float e_fScale)
{
	if(e_fScale>1.f)
		e_fScale = 1.f;
	if(e_fScale<=0.f)
		e_fScale = 0.1f;
	this->m_PCMToFFTDataConvertr.SetFFTSampleScale(e_fScale);
}