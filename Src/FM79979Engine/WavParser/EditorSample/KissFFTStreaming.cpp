#include "stdafx.h"
#include "KissFFTStreaming.h"

cKissFFTStreamingConvert::sTimeAndFFTData::sTimeAndFFTData(int*e_piLeftFFTData,int*e_piRightFFTData,float e_fStartTime,float e_fEndTime,int e_iFFTDataOneSample,int e_iTotalFFTDataCount,int e_iNumChannel)
{
	iNumChannel = e_iNumChannel;
	//pPCMData = nullptr;
	piLeftChannelFFTData = e_piLeftFFTData;
	piRightChannelFFTData = e_piRightFFTData;
	fStartTime = e_fStartTime;
	fEndTime = e_fEndTime;
	iFFTDataOneSample = e_iFFTDataOneSample;
	iTotalFFTDataCount = e_iTotalFFTDataCount;
	assert(e_iFFTDataOneSample<1000&&"I didn't expect this...");
}
cKissFFTStreamingConvert::sTimeAndFFTData::~sTimeAndFFTData()
{
	//SAFE_DELETE(pPCMData); 
}

bool	cKissFFTStreamingConvert::sTimeAndFFTData::GenerateFFTLines(float e_fTargetTime,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale,float e_fNextChannelYGap)
{
	float l_fFPS = 1.f/ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;
	if(e_fTargetTime+l_fFPS>fEndTime)
	{
		return false;
	}
	int l_IndexToJump = (int)((e_fTargetTime-e_fTargetTime)/l_fFPS);
	l_IndexToJump = 0;
	if(iTotalFFTDataCount<=l_IndexToJump*iFFTDataOneSample)
		return false;
	float l_fXGap = e_vChartResolution.x/iFFTDataOneSample;
	float l_fYGap = e_fNextChannelYGap;
	float l_fStartXPos = e_vShowPos.x;
	float l_fYStartPos = e_vShowPos.y;

	
	for(int l_iChannelIndex = 0;l_iChannelIndex<this->iNumChannel;++l_iChannelIndex)
	{
		int*l_pChannelData = l_iChannelIndex==0?piLeftChannelFFTData:piRightChannelFFTData;
		int*l_pStartFFTData = &l_pChannelData[iFFTDataOneSample*l_IndexToJump];
		int l_iNumFFTData = iFFTDataOneSample/2;
		for(int i=0;i<l_iNumFFTData;++i)
		{
			int l_iIndex = i*2+l_iChannelIndex*(l_iNumFFTData*2);//one line 2 points
			vFFTDataToPoints[l_iIndex+1].x = vFFTDataToPoints[l_iIndex].x = l_fStartXPos+l_fXGap*i;
			vFFTDataToPoints[l_iIndex].y = l_fYStartPos+l_fYGap*l_iChannelIndex;
			int l_iValue = l_pStartFFTData[i];
			vFFTDataToPoints[l_iIndex+1].y = vFFTDataToPoints[l_iIndex].y-(l_iValue*e_fScale);
		}
	}
	return true;
	
}

void	KissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri)
{
	cKissFFTStreamingConvert*l_pKissFFTStreamingConvert = (cKissFFTStreamingConvert*)_workParameter;
	l_pKissFFTStreamingConvert->m_bThreadAlreadyStop = false;
	float l_fLastTimeHaveNewData = 0.f;
	UT::sTimeAndFPS	l_Timer;
	l_Timer.Update();
	int l_iFFTCount = (int)l_pKissFFTStreamingConvert->m_iNFrameFFTDataCount;
	l_pKissFFTStreamingConvert->m_pOpanalOgg->Play(true);
	while( !l_pKissFFTStreamingConvert->m_bThreadStop && l_pKissFFTStreamingConvert->m_pOpanalOgg )
	{
		if( cGameApp::m_sbGamePause )
		{
			Sleep(10);
			continue;
		}
		//do fft
//		if( l_pKissFFTStreamingConvert->m_SoundThreadTimeAndPCMDataVector.size() )
		{
		
		}
		l_pKissFFTStreamingConvert->m_pOpanalOgg->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
		if( l_pKissFFTStreamingConvert->m_bNewData )
		{
			l_Timer.Update();
			//do FFT convert
			//
			float l_fCurrentNewDataTime = l_pKissFFTStreamingConvert->m_pOpanalOgg->GetCurrentStreamingTime();
			//if(l_fCurrentNewDataTime<=l_pKissFFTStreamingConvert->m_fCurrentTime)
			{//how come this happen!?
				//cGameApp::OutputDebugInfoString(L"this is should not happen,but if this happen make time to a bit futher than we expect to make data smooth");
				//l_pKissFFTStreamingConvert->m_pOpanalOgg->GoTo(l_pKissFFTStreamingConvert->m_fCurrentTime+0.016f);
				//l_iFFTCount /= 2;
			}
			//else
			{//do this in another thread?
				l_pKissFFTStreamingConvert->ProcessFFTData(l_iFFTCount,l_Timer.fElpaseTime);
			}
			l_pKissFFTStreamingConvert->m_bNewData = false;
		}
	}
}

void	KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri)
{
	cKissFFTStreamingConvert*l_pKissFFTStreamingConvert = (cKissFFTStreamingConvert*)_workParameter;
	l_pKissFFTStreamingConvert->m_bThreadAlreadyStop = true;
}

cKissFFTStreamingConvert::cKissFFTStreamingConvert()
{
	m_bReceivedBuffer = false;
	m_pFUThreadPool = nullptr;
	m_pOpanalOgg = nullptr;
	m_bThreadStop = false;
	m_bThreadAlreadyStop = true;
	m_bNewData = false;
	m_iCurrentFFTDataSwapBufferIndex = 0;
	m_iNumFFTDataSwapBuffer = 3;
}

cKissFFTStreamingConvert::~cKissFFTStreamingConvert()
{
	Destroy();
}

void	cKissFFTStreamingConvert::Destroy()
{
	m_bThreadStop = true;
	while( !m_bThreadAlreadyStop ){
		//wait for thread end...
	}
	SAFE_DELETE(m_pFUThreadPool);
	SAFE_RELEASE(m_pOpanalOgg,this);
	while(m_TimeAndPCMDataVector.size())
	{
		delete m_TimeAndPCMDataVector[0];
		m_TimeAndPCMDataVector.erase(m_TimeAndPCMDataVector.begin());
	}
	//DELETE_VECTOR(m_TimeAndPCMDataVector,sTimeAndPCMData*);
	//DELETE_VECTOR(m_SoundThreadTimeAndPCMDataVector,sTimeAndPCMData*);
}
//need a other thread to do this?
void	cKissFFTStreamingConvert::ProcessFFTData(int e_iOneFrameFFTDataCount,float e_fElpaseTime)
{
	//int l_iFFTCount = (int)(1.f/l_pKissFFTStreamingConvert->m_iDivideFFTDataToNFrame*l_pKissFFTStreamingConvert->m_pOpanalOgg->GetPCMDataSize());
	int l_iCount = e_iOneFrameFFTDataCount;
	float l_f30FPSTime = 1/30.f;
	if(e_fElpaseTime>l_f30FPSTime)
	{//performance is too bad...just make FFT smaller again
		//float l_fPercent = l_f30FPSTime/e_fElpaseTime;
		//l_iCount *=e_iOneFrameFFTDataCount;
	}
	if( !this->m_pOpanalOgg||!m_bReceivedBuffer )
		return;
	int  l_iChannels = m_pOpanalOgg->GetChannel();
	UT::sTimeAndFPS	l_Timer;
	l_Timer.Update();
	const int l_iFetchDataCount = m_iNFrameFFTDataCount;//*this->m_pSoundFile->m_iChannel;
	assert(l_iFetchDataCount>=this->m_iNFrameFFTDataCount&&"frenquence is too high,is this okay?");
	//ogg format is float
	size_t l_uiSameCount = e_iOneFrameFFTDataCount;//this->m_pOpanalOgg->GetPCMDataSize()/sizeof(float);
	kiss_fft_cpx* l_pKiss_FFT_In = this->m_Kiss_FFT_In;
	kiss_fft_cpx* l_pKiss_FFT_Out = this->m_Kiss_FFT_Out;
	kiss_fft_state* l_pkiss_fft_state = kiss_fft_alloc(l_iFetchDataCount, 0/*is_inverse_fft*/, NULL, NULL);
	bool l_bDoFFT = true;
	assert(l_iChannels == 2&&"sorry now only support 2 channel");
	for(int l_iCurrentChannelIndex = 0;l_iCurrentChannelIndex<l_iChannels;++l_iCurrentChannelIndex)
	{
		size_t l_uiCurrentSoundDataIndex = 0;
		int	l_iNumFFTData = 0;
		short*l_pucSoundData = (short*)this->m_StreamingBufferData;
		if( l_bDoFFT )
		{
			int l_iRestCountToFFT = l_uiSameCount-l_uiCurrentSoundDataIndex;
			int l_iStartIndex = 0;
			if(l_iCurrentChannelIndex == 1)
				l_iStartIndex = 1;
			for( size_t l_uiCount = 0;l_uiCount <l_uiSameCount;++l_uiCount)
			{
				//https://en.wikipedia.org/wiki/Window_function#Hann_window
				//Apply window function on the sample,Hann window
				double multiplier = 0.5 * (1 - cos(2*D3DX_PI*l_uiCount/(l_iFetchDataCount-1)));
				int l_iSoundDataOfChannel = l_iStartIndex+(l_uiCount*2);
				short l_fValue = l_pucSoundData[l_iSoundDataOfChannel];
				l_pKiss_FFT_In[l_uiCount].r = (float)(multiplier * l_fValue);
				l_pKiss_FFT_In[l_uiCount].i = 0;  //stores N samples 
			}
			//GetFft(l_iFetchDataCount,l_pKiss_FFT_In,l_pKiss_FFT_Out);
			kiss_fft(l_pkiss_fft_state, l_pKiss_FFT_In, l_pKiss_FFT_Out);
			// calculate magnitude of first n/2 FFT
			for(int i = 0; i < l_iFetchDataCount/2; i++ )
			{
				int val;
				float l_Msg = sqrt((l_pKiss_FFT_Out[i].r * l_pKiss_FFT_Out[i].r) + (l_pKiss_FFT_Out[i].i * l_pKiss_FFT_Out[i].i));
				// N/2 Log magnitude values.
				//for (i = 0; i < N/2 ; ++i){
				//  x =   10 * log10(mag[i]) ;
				//  printf("  log x= %g ", log(x));
				//val = l_Msg;
				val = (int)(log(l_Msg) *10); 
				//l_FFTDataVector[i] = val;
				m_FFTData[m_iCurrentFFTDataSwapBufferIndex][l_iCurrentChannelIndex][l_iNumFFTData] = val;
				++l_iNumFFTData;
				//l_pFFTDataVector->push_back(val);
			}
		}
		else
		{
			m_fScale = 0.1f;
			size_t l_uiCount = 0;
			for( l_uiCount = l_uiCurrentSoundDataIndex;l_uiCount < l_uiSameCount;l_uiCount++)
			{
			//	l_pFFTDataVector->push_back(l_pfSoundData[l_uiCount]);
			}
			//size_t l_uFFTDataVectoriSize = l_pFFTDataVector->size();
			//m_FFTDataVectorChannelVector.push_back(l_pFFTDataVector);
			break;	
		}
		//size_t l_uiSize = l_pFFTDataVector->size();
	}
	free(l_pkiss_fft_state);
	l_Timer.Update();
	double secs = l_Timer.fElpaseTime;
	std::string l_strDebugInfo = "do fft spent ";
	l_strDebugInfo += ValueToString(secs);
	cGameApp::OutputDebugInfoString(l_strDebugInfo);

	//sTimeAndFFTData(float*e_pFFTData,float e_fStartTime,float e_fEndTime,int e_iFFTDataOneSample,int e_iTotalFFTDataCount,int e_iNumChannel);
	float l_fCurrentNewDataTime = m_pOpanalOgg->GetCurrentStreamingTime();
	float l_fDuring = (float)OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)/(float)m_pOpanalOgg->GetPCMDataSize()*m_pOpanalOgg->GetTimeLength();
	float l_fEndTime = l_fDuring+l_fCurrentNewDataTime;
	sTimeAndFFTData*l_pTimeAndFFTData = new sTimeAndFFTData(m_FFTData[m_iCurrentFFTDataSwapBufferIndex][0],m_FFTData[m_iCurrentFFTDataSwapBufferIndex][1],l_fCurrentNewDataTime,l_fEndTime,e_iOneFrameFFTDataCount,OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short),m_pOpanalOgg->GetChannel());
	cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronized);
	m_TimeAndPCMDataVector.push_back(l_pTimeAndFFTData);

	m_iCurrentFFTDataSwapBufferIndex++;
	if( m_iCurrentFFTDataSwapBufferIndex >= FFT_DATA_SWAP_BUFFER_COUNT )
		m_iCurrentFFTDataSwapBufferIndex = 0;
}
//1.find proper data by time
//2.compare time and check performance
//3.check how many fft data need to do
//4.push data into m_SoundThreadTimeAndPCMDataVector
//5.calculate fft data in sound thread.
void	cKissFFTStreamingConvert::StreamingBuffer(int e_iCount,char*e_pData,size_t e_iCurrentPCMDataPosIndex)
{
	if(m_pOpanalOgg)
	{
		//m_TimeAndPCMDataVector.clear();
		float	l_fTimeLength = m_pOpanalOgg->GetTimeLength();
		ALsizei	l_iPCMDataLength = m_pOpanalOgg->GetPCMDataSize();
		float	l_fCurrentTime = ((float)e_iCurrentPCMDataPosIndex/l_iPCMDataLength)*l_fTimeLength;
		float	l_fDuring = (float)e_iCount/l_iPCMDataLength*l_fTimeLength;
		if(this->m_fCurrentTime >= l_fCurrentTime+l_fDuring )
		{
			//machine is too slow...
			//cGameApp::OutputDebugInfoString(L"this machine just too slow...");
			//return;
		}
		m_bReceivedBuffer = true;
		memcpy(m_StreamingBufferData,e_pData,e_iCount);
		//m_StreamingBufferector.push_back(l_pData);
	}
	m_bNewData = true;
}


bool	cKissFFTStreamingConvert::FetchSoundDataStart(const char*e_strFileName)
{
	Destroy();
	this->m_fCurrentTime = 0.f;
	m_pOpanalOgg = new cOpanalOgg(this,e_strFileName,true,std::bind(&cKissFFTStreamingConvert::StreamingBuffer,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
	m_iNFrameFFTDataCount = m_pOpanalOgg->GetFreq()/m_iDivideFFTDataToNFrame;
	//m_pOpanalOgg->SetUpdateNewBufferCallbackFunction(std::bind(&cKissFFTStreamingConvert::StreamingBuffer,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
	this->m_pFUThreadPool = new cFUThreadPool();
	this->m_pFUThreadPool->Spawn(1);
	FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&KissFFTStreamingConvertThread);
	FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&KissFFTStreamingConvertThreadDone);
	m_bThreadStop = false;
	this->m_pFUThreadPool->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
	return true;
}

void	cKissFFTStreamingConvert::Update(float e_fElpaseTime)
{
	if( this->m_pOpanalOgg )
	{
		this->m_fCurrentTime += e_fElpaseTime;
		while(m_TimeAndPCMDataVector.size())
		{
			if( !m_TimeAndPCMDataVector[0]->GenerateFFTLines(m_fCurrentTime,this->m_vChartShowPos,this->m_vChartResolution,this->m_fScale,this->m_fNextChannelYGap) )
			{
				cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronized);
				delete m_TimeAndPCMDataVector[0];
				m_TimeAndPCMDataVector.erase(m_TimeAndPCMDataVector.begin());
				cGameApp::OutputDebugInfoString(L"old data");
			}
			else
				break;
		}
		//if( m_TimeAndPCMDataVector.size() )
		{//fo fft to line.
		
		}
	}
}

void	cKissFFTStreamingConvert::Render()
{
	if( m_TimeAndPCMDataVector.size() )
	{
		sTimeAndFFTData*l_pTimeAndFFTData = m_TimeAndPCMDataVector[0];
		int l_iNumPointd = l_pTimeAndFFTData->iFFTDataOneSample/2*2;
		//left channel
		GLRender::RenderLine((float*)l_pTimeAndFFTData->vFFTDataToPoints,l_iNumPointd,Vector4::One,2);
		//right channel
		if( l_pTimeAndFFTData->iNumChannel == 2 )
		{
			int l_iRightChannelIndex = l_iNumPointd;
			GLRender::RenderLine((float*)l_pTimeAndFFTData->vFFTDataToPoints[l_iRightChannelIndex],l_iNumPointd,Vector4::One,2);
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