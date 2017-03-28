#include "stdafx.h"
#include "KissFFTStreaming.h"
#include "../../kiss_fft130/_kiss_fft_guts.h"

void	SoundUpdateThread(size_t _workParameter, size_t _pUri)
{
	cKissFFTStreamingConvert*l_pKissFFTStreamingConvert = (cKissFFTStreamingConvert*)_workParameter;
	float l_fLastTimeHaveNewData = 0.f;
	UT::sTimeAndFPS	l_Timer;
	l_Timer.Update();
	int l_iFFTCount = (int)l_pKissFFTStreamingConvert->m_iNFrameFFTDataCount;
	l_pKissFFTStreamingConvert->m_pOpanalOgg->Play(true);
	while( !l_pKissFFTStreamingConvert->m_bThreadStop && l_pKissFFTStreamingConvert->m_pOpanalOgg )
	{
		if( cGameApp::m_sbGamePause || l_pKissFFTStreamingConvert->m_bPause )
		{
			Sleep(10);
			l_pKissFFTStreamingConvert->m_bThreadInPause[0] = true;
			continue;
		}
		l_pKissFFTStreamingConvert->m_bThreadInPause[0] = false;
		l_pKissFFTStreamingConvert->m_pOpanalOgg->Update(l_Timer.fElpaseTime);
		l_Timer.Update();
		Sleep(1);
	}
}

void	SoundUpdateThreadDone(size_t _workParameter, size_t _pUri)
{
	cKissFFTStreamingConvert*l_pKissFFTStreamingConvert = (cKissFFTStreamingConvert*)_workParameter;
	l_pKissFFTStreamingConvert->m_bThreadAlreadyStop[0] = true;
}

void	KissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri)
{
	cKissFFTStreamingConvert*l_pKissFFTStreamingConvert = (cKissFFTStreamingConvert*)_workParameter;
	if( !l_pKissFFTStreamingConvert->m_pOpanalOgg )
		return;
	int l_iNumChannel = l_pKissFFTStreamingConvert->m_pOpanalOgg->GetChannel();
	int l_iNumFFTDataCount = l_pKissFFTStreamingConvert->m_iNFrameFFTDataCount;
	while( !l_pKissFFTStreamingConvert->m_bThreadStop && l_pKissFFTStreamingConvert->m_pOpanalOgg )
	{
		if( cGameApp::m_sbGamePause ||l_pKissFFTStreamingConvert->m_bPause)
		{
			Sleep(10);
			l_pKissFFTStreamingConvert->m_bThreadInPause[1] = true;
			continue;
		}
		l_pKissFFTStreamingConvert->m_bThreadInPause[1] = false;
		sTimeAndPCMData*l_pTimeAndPCMData = nullptr;
		{
			cFUSynchronizedHold	l_cFUSynchronizedHold(&l_pKissFFTStreamingConvert->m_FUSynchronizedForTimeAndPCMDataVector);
			if(l_pKissFFTStreamingConvert->m_TimeAndPCMDataVector.size())
			{
				l_pTimeAndPCMData = l_pKissFFTStreamingConvert->m_TimeAndPCMDataVector[0];
				l_pKissFFTStreamingConvert->m_TimeAndPCMDataVector.erase(l_pKissFFTStreamingConvert->m_TimeAndPCMDataVector.begin());
			}
		}
		if( l_pTimeAndPCMData != nullptr )
			l_pKissFFTStreamingConvert->m_PCMToFFTDataConvertr.ProcessFFTData(l_pTimeAndPCMData,l_pKissFFTStreamingConvert->m_TimeToUpdateFFTData.fTargetTime,l_iNumChannel,l_iNumFFTDataCount);
	}
}

void	KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri)
{
	cKissFFTStreamingConvert*l_pKissFFTStreamingConvert = (cKissFFTStreamingConvert*)_workParameter;
	l_pKissFFTStreamingConvert->m_bThreadAlreadyStop[1] = true;
}


cPCMToFFTDataConvertr::sTimeAndFFTData::sTimeAndFFTData(int*e_piLeftFFTData,int*e_piRightFFTData,float e_fStartTime,float e_fEndTime,int e_iFFTDataOneSample,int e_iTotalFFTDataCount,int e_iNumChannel,float e_fNextFFTTimeGap)
{
	iNumChannel = e_iNumChannel;
	//pPCMData = nullptr;
	piLeftChannelFFTData = e_piLeftFFTData;
	piRightChannelFFTData = e_piRightFFTData;
	fStartTime = e_fStartTime;
	fEndTime = e_fEndTime;
	iFFTDataOneSample = e_iFFTDataOneSample;
	iTotalFFTDataCount = e_iTotalFFTDataCount;
	fTimeGap = e_fNextFFTTimeGap;
	assert(e_iFFTDataOneSample<1000&&"I didn't expect this...");
}
cPCMToFFTDataConvertr::sTimeAndFFTData::~sTimeAndFFTData()
{
	//SAFE_DELETE(pPCMData); 
}

bool	cPCMToFFTDataConvertr::sTimeAndFFTData::ForceGenerateLastFFTLines(Vector2*e_pLinePoints,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale,float e_fNextChannelYGap)
{
	int l_iIndex = (this->iTotalFFTDataCount/this->iFFTDataOneSample)-1;
	GenerateFFTLinesByFFTSampleTargetIndex(e_pLinePoints,l_iIndex,e_vShowPos,e_vChartResolution,e_fScale,e_fNextChannelYGap);
	return true;
}

bool	cPCMToFFTDataConvertr::sTimeAndFFTData::GenerateFFTLinesByFFTSampleTargetIndex(Vector2*e_pLinePoints,int e_iFFTSampleTargetIndex,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale,float e_fNextChannelYGap)
{
	float l_fXGap = e_vChartResolution.x/iFFTDataOneSample;
	float l_fYGap = e_fNextChannelYGap;
	float l_fStartXPos = e_vShowPos.x;
	//float l_fNextChannelXPos = 100+e_vChartResolution.x/2;
	float l_fNextChannelXPos = 0;
	float l_fYStartPos = e_vShowPos.y;

	int l_iFFTDataLength = iFFTDataOneSample/WINDOWN_FUNCTION_FRUSTRUM;
	for(int l_iChannelIndex = 0;l_iChannelIndex<this->iNumChannel;++l_iChannelIndex)
	{
		int*l_pChannelData = l_iChannelIndex==0?piLeftChannelFFTData:piRightChannelFFTData;
		int*l_pStartFFTData = &l_pChannelData[l_iFFTDataLength*e_iFFTSampleTargetIndex];
		for(int i=0;i<l_iFFTDataLength;++i)
		{
			int l_iIndex = i*2+l_iChannelIndex*(l_iFFTDataLength*2);//one line 2 points
			e_pLinePoints[l_iIndex+1].x = e_pLinePoints[l_iIndex].x = l_fStartXPos+l_fXGap*i+l_fNextChannelXPos*l_iChannelIndex;
			e_pLinePoints[l_iIndex].y = l_fYStartPos+l_fYGap*l_iChannelIndex;
			int l_iValue = l_pStartFFTData[i];
			e_pLinePoints[l_iIndex+1].y = e_pLinePoints[l_iIndex].y-(l_iValue*e_fScale);
		}
	}
	return true;
}



bool	cPCMToFFTDataConvertr::sTimeAndFFTData::GenerateFFTLines(Vector2*e_pLinePoints,float e_fTargetTime,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale,float e_fNextChannelYGap)
{
	if( fEndTime-fTimeGap < e_fTargetTime )
	{
		//return false;
	}
	int l_IndexToJump = (int)((e_fTargetTime-this->fStartTime)/fTimeGap);
	if(this->fStartTime>e_fTargetTime)
	{//how come this happen...fucking sad...
		if(abs(this->fStartTime-e_fTargetTime)>=0.018f)
		{
#ifdef DEBUG
			std::wstring l_strDebugInfo = L"1:S:";
			l_strDebugInfo += ValueToStringW(this->fStartTime);
			l_strDebugInfo += L",E:";
			l_strDebugInfo += ValueToStringW(this->fEndTime);
			l_strDebugInfo += L",TargetTime:";
			l_strDebugInfo += ValueToStringW(e_fTargetTime);
			l_strDebugInfo += L":how come this happen,someone call go to time???...fucking sad,GenerateFFTLines";
			cGameApp::OutputDebugInfoString(l_strDebugInfo);
#endif
			//anyway take a quick fetch avoid screen flash for go to time has been called
			GenerateFFTLinesByFFTSampleTargetIndex(e_pLinePoints,0,e_vShowPos,e_vChartResolution,e_fScale,e_fNextChannelYGap);
			return false;
		}
		l_IndexToJump = 0;
	}
	int l_iFFTDataLength = iFFTDataOneSample/WINDOWN_FUNCTION_FRUSTRUM;
	if(iTotalFFTDataCount<=l_IndexToJump*l_iFFTDataLength)
	{
		//cGameApp::OutputDebugInfoString(L"GenerateFFTLines,here should not happen,fft data count is not enough,,cKissFFTStreamingConvert::ProcessFFTData() need more accurate the performance goes!.");
		//here should not happen,cKissFFTStreamingConvert::ProcessFFTData() need more accurate the performance goes!.
		return false;
	}
	GenerateFFTLinesByFFTSampleTargetIndex(e_pLinePoints,l_IndexToJump,e_vShowPos,e_vChartResolution,e_fScale,e_fNextChannelYGap);
	return true;
}

cPCMToFFTDataConvertr::cPCMToFFTDataConvertr()
{
	m_fFFTSampleScale = 1.0f;
	m_pfWindowFunctionConstantValue = nullptr;
	m_iCurrentFFTDataSwapBufferIndex = 0;
	m_iNumFFTDataSwapBuffer = 3;
	m_pkiss_fft_state = nullptr;
}

cPCMToFFTDataConvertr::~cPCMToFFTDataConvertr()
{

}
void	cPCMToFFTDataConvertr::Destroy()
{
	DELETE_VECTOR(m_TimeAndFFTDataVector,sTimeAndFFTData*);
	SAFE_DELETE(m_pfWindowFunctionConstantValue);
	if( m_pkiss_fft_state )
	{
		free(m_pkiss_fft_state);
		m_pkiss_fft_state = nullptr;
	}
}

void	cPCMToFFTDataConvertr::SetNFrameFFTDataCount(int e_iNFrameFFTDataCount)
{
	Destroy();
	this->m_pkiss_fft_state = kiss_fft_alloc((int)(e_iNFrameFFTDataCount*this->m_fFFTSampleScale), 0/*is_inverse_fft*/, NULL, NULL);
	m_pfWindowFunctionConstantValue = new float[e_iNFrameFFTDataCount];

	for( int l_iCount = 0;l_iCount <e_iNFrameFFTDataCount;++l_iCount)
	{
		double multiplier = 0.5 * (1 - cos(2*D3DX_PI*l_iCount/(e_iNFrameFFTDataCount-1)));
		m_pfWindowFunctionConstantValue[l_iCount] = (float)multiplier;
	}
}

//need a other thread to do this?
void	cPCMToFFTDataConvertr::ProcessFFTData(sTimeAndPCMData*e_pTimeAndPCMData,float e_fTimeToUpdateFFTData,int e_iNumChannel,int e_iNFrameFFTDataCount)
{
	if( !e_pTimeAndPCMData)
		return;
	sTimeAndPCMData*l_pTimeAndPCMData = e_pTimeAndPCMData;
	UT::sTimeAndFPS	l_Timer;
	l_Timer.Update();
	//
	float	l_fCurrentNewDataTime = l_pTimeAndPCMData->fStartTime;
	float	l_fEndTime = l_pTimeAndPCMData->fEndTime;
	float	l_fDuring = l_fEndTime-l_fCurrentNewDataTime;
	int		l_iNumFFTDataNeed = (int)(l_fDuring/e_fTimeToUpdateFFTData);//this->m_TimeToUpdateFFTData.fTargetTime);
	if( l_iNumFFTDataNeed == 0 )
	{
		if( abs(l_fDuring-e_fTimeToUpdateFFTData) <= 0.00001f )
			l_iNumFFTDataNeed = 1;
		else
			goto FINISHED;
	}
	//float	l_fOneFFTDataPeriod = (float)this->m_iNFrameFFTDataCount/this->m_pOpanalOgg->GetPCMDataSize()*this->m_pOpanalOgg->GetTimeLength();

	int		l_iNextPCMDataStep = l_pTimeAndPCMData->iNumPCMData/sizeof(short)/l_iNumFFTDataNeed;
	size_t	l_uiEndIndex = l_iNextPCMDataStep*l_iNumFFTDataNeed;
	int  l_iChannels = e_iNumChannel;//m_pOpanalOgg->GetChannel();
	//assert(m_fFFTSampleScale<=1.f&&"m_fFFTSampleScale cant not over 1");
	assert(m_fFFTSampleScale<=1.f&&"m_fFFTSampleScale cant not over 1");
	const int l_iFetchDataCount = (int)(e_iNFrameFFTDataCount*m_fFFTSampleScale);//m_iNFrameFFTDataCount*m_fFFTSampleScale;//*this->m_pSoundFile->m_iChannel;
	//assert(l_iFetchDataCount>=this->m_iNFrameFFTDataCount&&"frenquence is too high,is this okay?");
	kiss_fft_cpx* l_pKiss_FFT_In = this->m_Kiss_FFT_In;
	kiss_fft_cpx* l_pKiss_FFT_Out = this->m_Kiss_FFT_Out;
	kiss_fft_state* l_pkiss_fft_state = this->m_pkiss_fft_state;
	//auto l_pData = this->m_pkiss_fft_state;
	//l_pData->nfft = l_iFetchDataCount;
	bool l_bDoFFT = true;
	int l_iNumFFTData2 = 0;
	for(int l_iCurrentChannelIndex = 0;l_iCurrentChannelIndex<l_iChannels;++l_iCurrentChannelIndex)
	{
		size_t l_uiCurrentSoundDataIndex = 0;
		int	l_iNumFFTData = 0;
		short*l_pucSoundData = (short*)l_pTimeAndPCMData->pPCMData;
		int l_iChannelIndex = 0;
		if(l_iCurrentChannelIndex == 1)
			l_iChannelIndex = 1;
		if( l_bDoFFT )
		{
			//for how many FFT data
			for(int l_iSampleIndex = 0;l_iSampleIndex<l_iNumFFTDataNeed;++l_iSampleIndex)
			{
				int l_iStartIndex = l_iSampleIndex*l_iNextPCMDataStep;
				if( ((l_iStartIndex+l_iFetchDataCount*l_iChannels)*(int)sizeof(short))>=l_pTimeAndPCMData->iNumPCMData )
					break;
				int l_iSoundDataOfChannel1 = 0;
				for(int l_iCurrentIndex = 0;l_iCurrentIndex<l_iFetchDataCount;++l_iCurrentIndex)
				{
					int l_iTargetPCMIndex = l_iStartIndex+(l_iCurrentIndex*l_iChannels);
					//Apply window function on the sample,Hann window
					double multiplier = m_pfWindowFunctionConstantValue[l_iCurrentIndex];
					int l_iSoundDataOfChannel = l_iChannelIndex+l_iTargetPCMIndex;
					l_iSoundDataOfChannel1 = l_iSoundDataOfChannel;
					if( l_iSoundDataOfChannel>=l_pTimeAndPCMData->iNumPCMData/l_iChannels )
					{//out of range...how come this happen!.
						int a=0;
					}
					short l_fValue = l_pucSoundData[l_iSoundDataOfChannel];
					l_pKiss_FFT_In[l_iCurrentIndex].r = (float)(multiplier * l_fValue);
					l_pKiss_FFT_In[l_iCurrentIndex].i = 0;  //stores N samples 

				}
				kiss_fft(l_pkiss_fft_state, l_pKiss_FFT_In, l_pKiss_FFT_Out);
				// calculate magnitude of first n/2 FFT
				int l_iDidgitalWindownFunctionCount = l_iFetchDataCount/WINDOWN_FUNCTION_FRUSTRUM;
				for(int i = 0; i < l_iDidgitalWindownFunctionCount; i++ )
				{
					int val;
					float l_Msg = sqrt((l_pKiss_FFT_Out[i].r * l_pKiss_FFT_Out[i].r) + (l_pKiss_FFT_Out[i].i * l_pKiss_FFT_Out[i].i));
					val = (int)(log(l_Msg) *10); 
					if( val < -3000 )//this is mute.
					{
						val = 0;
					}
					m_FFTData[m_iCurrentFFTDataSwapBufferIndex][l_iCurrentChannelIndex][l_iNumFFTData] = val;
					++l_iNumFFTData;
					assert(l_iNumFFTData<=OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)&&"m_FFTData out of range");
				}
			}
			l_iNumFFTData2 = l_iNumFFTData;
		}
		else
		{
			//m_fScale = 0.1f;
			size_t l_uiCount = 0;
			for( l_uiCount = l_uiCurrentSoundDataIndex;l_uiCount < l_uiEndIndex;l_uiCount++)
			{
				int l_iSoundDataOfChannel = l_iChannelIndex+(l_uiCount*2);
				m_FFTData[m_iCurrentFFTDataSwapBufferIndex][l_iCurrentChannelIndex][l_uiCount] = l_pucSoundData[l_iSoundDataOfChannel];
			}
			break;	
		}
	}
#ifdef DEBUG
	{
		l_Timer.Update();
		double secs = l_Timer.fElpaseTime;
		static int l_iTest = 0;
		++l_iTest;
		std::wstring l_strDebugInfo = L"do fft spent  ";
		l_strDebugInfo += ValueToStringW(secs);
		l_strDebugInfo += L":";
		l_strDebugInfo += ValueToStringW(l_iTest);
		cGameApp::OutputDebugInfoString(l_strDebugInfo);
	}
#endif
	//
	sTimeAndFFTData*l_pTimeAndFFTData = new sTimeAndFFTData(m_FFTData[m_iCurrentFFTDataSwapBufferIndex][0],m_FFTData[m_iCurrentFFTDataSwapBufferIndex][1],l_fCurrentNewDataTime,l_fEndTime,l_iFetchDataCount,l_iNumFFTData2,l_iChannels,e_fTimeToUpdateFFTData);
	{
		cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronizedForTimeAndFFTDataVector);
		m_TimeAndFFTDataVector.push_back(l_pTimeAndFFTData);
		if( m_TimeAndFFTDataVector.size()>=FFT_DATA_SWAP_BUFFER_COUNT )
		{
#ifdef DEBUG
			cGameApp::OutputDebugInfoString(L"FFT_DATA_SWAP_BUFFER_COUNT is too small...the fps is set over than 30?");
#endif
		}
	}
	m_iCurrentFFTDataSwapBufferIndex++;
	if( m_iCurrentFFTDataSwapBufferIndex >= FFT_DATA_SWAP_BUFFER_COUNT )
		m_iCurrentFFTDataSwapBufferIndex = 0;
FINISHED:
	delete l_pTimeAndPCMData;
}

cKissFFTStreamingConvert::cKissFFTStreamingConvert()
{
	m_bThreadInPause[0] = m_bThreadInPause[1] = false;
	m_iCurrentStreamingBufferDataIndex = 0;
	m_pFUThreadPool = nullptr;
	m_pOpanalOgg = nullptr;
	m_bThreadStop = false;
	m_bThreadAlreadyStop[0] = m_bThreadAlreadyStop[1] = true;
	m_TimeToUpdateFFTData.SetTargetTime(1.f/30.f);
}

cKissFFTStreamingConvert::~cKissFFTStreamingConvert()
{
	Destroy();
}

void	cKissFFTStreamingConvert::Destroy()
{
	m_bThreadStop = true;
	while( !m_bThreadAlreadyStop[0] || !m_bThreadAlreadyStop[1] ){
		//wait for thread end...
		Sleep(1);
	}
	SAFE_DELETE(m_pFUThreadPool);
	m_PCMToFFTDataConvertr.Destroy();
	SAFE_RELEASE(m_pOpanalOgg,this);
	DELETE_VECTOR(m_TimeAndPCMDataVector,sTimeAndPCMData*);
}

//1.find proper data by time
//2.compare time and check performance
//3.check how many fft data need to do
//4.push data into m_TimeAndPCMDataVector
//5.calculate fft data in sound thread.
void	cKissFFTStreamingConvert::StreamingBuffer(int e_iCount,char*e_pData,size_t e_iCurrentPCMDataPosIndex)
{
	if(m_pOpanalOgg)
	{
#ifdef DEBUG
		static int l_iTest = 0;
		++l_iTest;
		std::wstring l_strDebugInfo = L"StreamingBuffer  ";
		l_strDebugInfo += ValueToStringW(l_iTest);
		cGameApp::OutputDebugInfoString(l_strDebugInfo);
#endif
		//the performance is too bad...skip this one
		if( m_TimeAndPCMDataVector.size()+1 >= PCM_SWAP_BUFFER_COUNT)
		{
#ifdef DEBUG
			cGameApp::OutputDebugInfoString(L"StreamingBuffer is over size...fuck");
#endif
			//
			return;
		}

		float	l_fTimeLength = m_pOpanalOgg->GetTimeLength();
		ALsizei	l_iPCMDataLength = m_pOpanalOgg->GetPCMDataSize();
		float	l_fCurrentTime = ((float)e_iCurrentPCMDataPosIndex/l_iPCMDataLength)*l_fTimeLength;
		float	l_fDuring = (float)e_iCount/l_iPCMDataLength*l_fTimeLength;
		float	l_fEndTime = l_fCurrentTime+l_fDuring;

		if(this->m_fCurrentTime >= l_fCurrentTime )
		{//performance too bad?
			this->m_fCurrentTime = l_fCurrentTime;
		}

		if(this->m_fCurrentTime >= l_fCurrentTime+l_fDuring )
		{
			//machine is too slow...
			//cGameApp::OutputDebugInfoString(L"this machine just too slow...");
			//return;
		}
		memcpy(m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex],e_pData,e_iCount);
		sTimeAndPCMData*l_pTimeAndPCMData = new sTimeAndPCMData(l_fCurrentTime,l_fEndTime,m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex],e_iCount,m_iCurrentStreamingBufferDataIndex);
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
		//m_StreamingBufferector.push_back(l_pData);
	}
}


bool	cKissFFTStreamingConvert::FetchSoundDataStart(const char*e_strFileName)
{
	Destroy();
	this->m_fCurrentTime = 0.f;
	m_pOpanalOgg = new cOpanalOgg(this,e_strFileName,true,std::bind(&cKissFFTStreamingConvert::StreamingBuffer,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),false);
	if( m_pOpanalOgg->GetTimeLength() == 0.f )
	{//file open failed
		Destroy();
		return false;
	}
	m_iNFrameFFTDataCount = m_pOpanalOgg->GetFreq()/m_iDivideFFTDataToNFrame;
	this->m_PCMToFFTDataConvertr.SetNFrameFFTDataCount(m_iNFrameFFTDataCount);
	//m_pOpanalOgg->SetUpdateNewBufferCallbackFunction(std::bind(&cKissFFTStreamingConvert::StreamingBuffer,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
	this->m_pFUThreadPool = new cFUThreadPool();
	this->m_pFUThreadPool->Spawn(2);
	FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&KissFFTStreamingConvertThread);
	FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&KissFFTStreamingConvertThreadDone);
	FUStaticFunctor2<size_t, size_t, void>* workFunctor1 = new FUStaticFunctor2<size_t, size_t, void>(&SoundUpdateThread);
	FUStaticFunctor2<size_t, size_t, void>* doneFunctor1 = new FUStaticFunctor2<size_t, size_t, void>(&SoundUpdateThreadDone);
	m_bThreadStop = false;
	m_bThreadAlreadyStop[0] = m_bThreadAlreadyStop[1] = false;
	this->m_pFUThreadPool->ExecuteWork(workFunctor1,doneFunctor1,(size_t)this,0);
	this->m_pFUThreadPool->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
	return true;
}

void	cKissFFTStreamingConvert::Update(float e_fElpaseTime)
{
	if( this->m_pOpanalOgg )
	{
		if( !this->m_bPause )
			this->m_fCurrentTime += e_fElpaseTime;
		if( this->m_fCurrentTime > this->m_pOpanalOgg->GetTimeLength() )
		{
			this->Destroy();
			return;
		}
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
				break;
		}
	}
}

void	cKissFFTStreamingConvert::Render()
{
	if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() )
	{
		cPCMToFFTDataConvertr::sTimeAndFFTData*l_pTimeAndFFTData = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0];
		int l_iNumPointd = l_pTimeAndFFTData->iFFTDataOneSample/2*2;
		//left channel
		GLRender::RenderLine((float*)m_vFFTDataToPoints,l_iNumPointd,Vector4::One,2);
		//right channel
		if( l_pTimeAndFFTData->iNumChannel == 2 )
		{
			int l_iRightChannelIndex = l_iNumPointd;
			GLRender::RenderLine((float*)m_vFFTDataToPoints[l_iRightChannelIndex],l_iNumPointd,Vector4::One,2);
		}
	}
}

float	cKissFFTStreamingConvert::GetCurrentTimePercentage()
{
	if( this->m_pOpanalOgg && this->m_pOpanalOgg->GetTimeLength()>0.f )
	{
		return this->m_fCurrentTime/this->m_pOpanalOgg->GetTimeLength();
	}
	return 0.f;
}

float	cKissFFTStreamingConvert::GetTimeLength()
{
	if(this->m_pOpanalOgg)
	{
		return this->m_pOpanalOgg->GetTimeLength();
	}
	return 0.f;
}

void	cKissFFTStreamingConvert::GoToTime(float e_fTime)
{
	if(this->m_pOpanalOgg)
	{
		cGameApp::m_sbGamePause = true;
		while( !m_bThreadInPause[0] || !m_bThreadInPause[1] )
		{//wait for pause
			Sleep(1);
		}
		//keep one avoid screen flash
		while(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size()>1)
		{
			delete m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[1];
			m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.erase(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.begin()+1);
		}
		DELETE_VECTOR(m_TimeAndPCMDataVector,sTimeAndPCMData*);
		this->m_pOpanalOgg->GoTo(e_fTime);
		this->m_fCurrentTime = e_fTime;
		cGameApp::m_sbGamePause = false;
	}
}