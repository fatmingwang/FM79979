#include "stdafx.h"
#include "KissFFTStreaming.h"
#include "../kiss_fft130/_kiss_fft_guts.h"
#include <math.h>
#include "KissFFTFunction.h"
//#include "SoundBufferSwaper.h"
//suggest e_iFilterStrengthValue:6
extern int DoFilter(float e_fFilterEndScaleValue,int e_iTransformLength,int e_iStartArrayIndex,int*e_pFFTDataSrc,kiss_fft_cpx*e_pKiss_FFT_Out,int e_iFilterStrengthValue);
void	SoundUpdateThread(size_t _workParameter, size_t _pUri)
{
	cKissFFTStreamingConvert*l_pKissFFTStreamingConvert = (cKissFFTStreamingConvert*)_workParameter;
	float l_fLastTimeHaveNewData = 0.f;
	UT::sTimeAndFPS	l_Timer;
	l_Timer.Update();
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
	int l_iNumFFTDataCount = l_pKissFFTStreamingConvert->m_iOneFrameFFTDataCount;
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
			l_pKissFFTStreamingConvert->m_PCMToFFTDataConvertr.ProcessFFTData(l_pTimeAndPCMData,l_pKissFFTStreamingConvert->m_TimeToUpdateFFTData.fTargetTime,l_iNumFFTDataCount,l_pKissFFTStreamingConvert->IsFilter(),l_pKissFFTStreamingConvert->GetFrenquenceFilterEndScaleValue(),l_pKissFFTStreamingConvert->GetFilterStrengthValue());
	}
}

void	KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri)
{
	cKissFFTStreamingConvert*l_pKissFFTStreamingConvert = (cKissFFTStreamingConvert*)_workParameter;
	l_pKissFFTStreamingConvert->m_bThreadAlreadyStop[1] = true;
}


cPCMToFFTDataConvertr::sTimeAndFFTData::sTimeAndFFTData(int*e_piFFTData,float e_fStartTime,float e_fEndTime,int e_iFFTDataOneSample,float e_fNextFFTTimeGap)
{
	bUpdated = false;
	iBiggestAmplitude = 0;
	iCurrentTimeStampIndex = -1;
	iBiggestFFTDataValueOfIndex = 0;
	//pPCMData = nullptr;
	pFFTData = e_piFFTData;
	fStartTime = e_fStartTime;
	fEndTime = e_fEndTime;
	iFFTDataOneSample = e_iFFTDataOneSample/WINDOWN_FUNCTION_FRUSTRUM;
	fTimeGap = e_fNextFFTTimeGap;
	//FFT_DATA_LINE_POINTS_COUNT
	//assert(e_iFFTDataOneSample<1000&&"I didn't expect this...");
}
cPCMToFFTDataConvertr::sTimeAndFFTData::~sTimeAndFFTData()
{
	//SAFE_DELETE(pPCMData); 
}

bool	cPCMToFFTDataConvertr::sTimeAndFFTData::GenerateFFTLines(Vector2*e_pLinePoints,float e_fTargetTime,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale)
{
	if(e_fTargetTime >= this->fEndTime )
		return false;
	if( bUpdated )
		return true;
	bUpdated = true;
	float l_fXGap = e_vChartResolution.x/iFFTDataOneSample;
	float l_fYGap = 0;
	float l_fStartXPos = e_vShowPos.x;
	float l_fYStartPos = e_vShowPos.y;


	int l_iMax = -79979;
	this->iBiggestFFTDataValueOfIndex = -1;
	int*l_pStartFFTData = pFFTData;
	int l_iFFTDataOneSample = iFFTDataOneSample;
	for(int i=0;i<iFFTDataOneSample;++i)
	{
		int l_iIndex = i*2;//one line 2 points
		e_pLinePoints[l_iIndex+1].x = e_pLinePoints[l_iIndex].x = l_fStartXPos+l_fXGap*i;
		e_pLinePoints[l_iIndex].y = l_fYStartPos+l_fYGap;
		int l_iValue = l_pStartFFTData[i];
		e_pLinePoints[l_iIndex+1].y = e_pLinePoints[l_iIndex].y-(l_iValue*e_fScale);
		if( l_iMax < l_iValue )
		{
			l_iMax = l_iValue;
			this->iBiggestFFTDataValueOfIndex = i;
		}
	}
	iBiggestAmplitude = l_iMax;
#ifdef DEBUG
	std::wstring l_strDebugInfo = L"GenerateFFTLines:";
	l_strDebugInfo += ValueToStringW(this->fStartTime);
	cGameApp::OutputDebugInfoString(l_strDebugInfo);
#endif
	return true;
}

int	cPCMToFFTDataConvertr::sTimeAndFFTData::GetCurrentFFTData(int*e_piOutputData,float e_fTargetTime)
{
	int l_iFFTDataLength = iFFTDataOneSample;
	memcpy(e_piOutputData,pFFTData,sizeof(int)*l_iFFTDataLength);
	return l_iFFTDataLength;
}


cPCMToFFTDataConvertr::cPCMToFFTDataConvertr()
{
//	m_fFFTSampleScale = 1.0f;
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

void	cPCMToFFTDataConvertr::SetupWindowsFunctionConstValue(int e_iCount)
{
	SAFE_DELETE(m_pfWindowFunctionConstantValue);
	m_pfWindowFunctionConstantValue = GenerateWindowsFunctionValue(e_iCount);
}

void	cPCMToFFTDataConvertr::SetNFrameFFTDataCount(int e_iNFrameFFTDataCount)
{
	Destroy();
//	this->m_pkiss_fft_state = kiss_fft_alloc((int)(e_iNFrameFFTDataCount*this->m_fFFTSampleScale), 0/*is_inverse_fft*/, NULL, NULL);
		this->m_pkiss_fft_state = kiss_fft_alloc((int)(e_iNFrameFFTDataCount), 0/*is_inverse_fft*/, NULL, NULL);
	SetupWindowsFunctionConstValue(e_iNFrameFFTDataCount);
}

//need a other thread to do this?
int*	cPCMToFFTDataConvertr::ProcessFFTData(sTimeAndPCMData*e_pTimeAndPCMData,float e_fTimeToUpdateFFTData,int e_iNFrameFFTDataCount,bool e_bDoFilter,float e_fFilterEndScaleValue,int e_iFilterStrength)
{
	if( !e_pTimeAndPCMData)
		return nullptr;
	assert(e_iNFrameFFTDataCount*sizeof(short) <= (size_t)e_pTimeAndPCMData->iNumPCMData);
//	float l_fFFTSampleScale = m_fFFTSampleScale;
	sTimeAndPCMData*l_pTimeAndPCMData = e_pTimeAndPCMData;
	UT::sTimeAndFPS	l_Timer;
	l_Timer.Update();
	int*l_pOutData = m_FFTData[m_iCurrentFFTDataSwapBufferIndex];
	ProcessFFT(l_pTimeAndPCMData,this->m_pkiss_fft_state,this->m_Kiss_FFT_In,this->m_Kiss_FFT_Out,
	this->m_pfWindowFunctionConstantValue,m_FFTData[m_iCurrentFFTDataSwapBufferIndex],e_bDoFilter,e_iFilterStrength,e_fFilterEndScaleValue);
	l_Timer.Update();
#ifdef DEBUG
	std::wstring l_strDebugString = L"processFFTData:";
	l_strDebugString += ValueToStringW(e_pTimeAndPCMData->fStartTime);
	l_strDebugString += L",fft calculate time:";
	l_strDebugString  += ValueToStringW(l_Timer.fElpaseTime);
	l_strDebugString +=L"\n";
	cGameApp::OutputDebugInfoString(l_strDebugString );
#endif
	sTimeAndFFTData*l_pTimeAndFFTData = new sTimeAndFFTData(m_FFTData[m_iCurrentFFTDataSwapBufferIndex],l_pTimeAndPCMData->fStartTime,l_pTimeAndPCMData->fEndTime,e_iNFrameFFTDataCount,e_fTimeToUpdateFFTData);
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
//FINISHED:
	delete l_pTimeAndPCMData;
	return l_pOutData;
}

cKissFFTStreamingConvert::cKissFFTStreamingConvert()
{
	m_pvFFTDataToPoints = new Vector2[cSoundCompareParameter::FFT_DATA_LINE_POINTS_COUNT];
	m_bThreadInPause[0] = m_bThreadInPause[1] = false;
	m_iCurrentStreamingBufferDataIndex = 0;
	m_pFUThreadPool = nullptr;
	m_pOpanalOgg = nullptr;
	m_bThreadStop = false;
	m_bThreadAlreadyStop[0] = m_bThreadAlreadyStop[1] = true;
	m_TimeToUpdateFFTData.SetTargetTime(1.f/m_iDivideFFTDataToNFrame);

	this->m_vChartShowPos.x = cGameApp::m_svGameResolution.x/2-200;
	this->m_vChartShowPos.y = cGameApp::m_svGameResolution.y/2+200;
}

cKissFFTStreamingConvert::~cKissFFTStreamingConvert()
{
	SAFE_DELETE(m_pvFFTDataToPoints);
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
void	cKissFFTStreamingConvert::StreamingBuffer(int e_iCount,char*e_pData,size_t e_iCurrentPCMDataPosIndex,int e_iBirPersample)
{
	assert(e_iCount>=this->m_iOneFrameFFTDataCount&&"PCM buffer could must gerat than m_iOneFrameFFTDataCount,because I am lazy to fix this!.");
	if(m_pOpanalOgg)
	{
		//the performance is too bad...skip this one
		if( m_TimeAndPCMDataVector.size() >= PCM_SWAP_BUFFER_COUNT)
		{
			cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronizedForTimeAndPCMDataVector);
			DELETE_VECTOR(m_TimeAndPCMDataVector,sTimeAndPCMData*);
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
			cGameApp::OutputDebugInfoString(L"this machine just too slow...");
			return;
		}
		memcpy(m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex],e_pData,e_iCount);
		int l_iNumFFTDataIsComing = (int)(l_fDuring/m_TimeToUpdateFFTData.fTargetTime);
		float	l_fTimeGap = l_fDuring/l_iNumFFTDataIsComing;
		for( int i=0;i<l_iNumFFTDataIsComing;++i )
		{
			short*l_pDataPos = (short*)m_StreamingBufferData[m_iCurrentStreamingBufferDataIndex];
			l_pDataPos += m_iOneFrameFFTDataCount*i;
			sTimeAndPCMData*l_pTimeAndPCMData = new sTimeAndPCMData(l_fCurrentTime,l_fCurrentTime+l_fTimeGap,m_pOpanalOgg->GetChannelCount(),(char*)l_pDataPos,m_iOneFrameFFTDataCount*e_iBirPersample,e_iBirPersample==2?eDataType::eDT_SHORT:eDataType::eDT_INT);
			{
				cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronizedForTimeAndPCMDataVector);
				m_TimeAndPCMDataVector.push_back(l_pTimeAndPCMData);
				if( m_TimeAndPCMDataVector.size()>=PCM_SWAP_BUFFER_COUNT )
				{
#ifdef DEBUG
					cGameApp::OutputDebugInfoString("streaming buffer is too small...");
#endif
				}
#ifdef DEBUG
				std::wstring l_strDebugString = L"StreamingBuffer:";
				l_strDebugString += ValueToStringW(l_fCurrentTime);
				cGameApp::OutputDebugInfoString(l_strDebugString );
#endif
			}
			l_fCurrentTime += l_fTimeGap;
			//m_StreamingBufferector.push_back(l_pData);
		}
		++m_iCurrentStreamingBufferDataIndex;
		if( m_iCurrentStreamingBufferDataIndex >= PCM_SWAP_BUFFER_COUNT)
			m_iCurrentStreamingBufferDataIndex = 0;
	}
}


bool	cKissFFTStreamingConvert::FetchSoundDataStart(const char*e_strFileName,bool e_bPlaySound)
{
	Destroy();
	this->m_fCurrentTime = 0.f;
	m_pOpanalOgg = new cOpanalOgg(this,e_strFileName,true,std::bind(&cKissFFTStreamingConvert::StreamingBuffer,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4),false);
	if( m_pOpanalOgg->GetTimeLength() == 0.f )
	{//file open failed
		Destroy();
		return false;
	}
	float l_fTimeAndFreqAmountWithChannel = m_pOpanalOgg->GetFreq()*m_pOpanalOgg->GetTimeLength();
	float l_PCMSizeDivideFrenquency = m_pOpanalOgg->GetPCMDataSize()/l_fTimeAndFreqAmountWithChannel/m_pOpanalOgg->GetChannelCount();
	if( abs(2-l_PCMSizeDivideFrenquency)>0.01 )
	{
		UT::ErrorMsg(ValueToString(l_PCMSizeDivideFrenquency).c_str(),"ogg file sample Size is not 2 byte!?ignore this and have no idea will happen.");
	}
	//
	//
	cKissFFTConvertBase::SetOneFrameFFTDataCount(m_pOpanalOgg->GetFreq());
	//m_iOneFrameFFTDataCount = m_pOpanalOgg->GetFreq()/m_iDivideFFTDataToNFrame;
	if(m_iOneFrameFFTDataCount*2 > cSoundCompareParameter::FFT_DATA_LINE_POINTS_COUNT )
	{//too big?give it one more chance
		m_iOneFrameFFTDataCount /= 2;
	}
	if(m_iOneFrameFFTDataCount*2 > cSoundCompareParameter::FFT_DATA_LINE_POINTS_COUNT)
	{
		Destroy();
		UT::ErrorMsg(L"m_iDivideFFTDataToNFrame is too small please incerase m_iDivideFFTDataToNFrame or increase FFT_DATA_LINE_POINTS_COUNT",L"Error!");
		return false;
	}
	this->m_PCMToFFTDataConvertr.SetNFrameFFTDataCount(m_iOneFrameFFTDataCount);
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
		if( !this->m_bPause && m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() )
			this->m_fCurrentTime += e_fElpaseTime;
		if( this->m_fCurrentTime > this->m_pOpanalOgg->GetTimeLength() )
		{
			this->Destroy();
			return;
		}
		//dont need to do synchronzied,because only here will delete
		while(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size())
		{
			if( !m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLines(this->m_pvFFTDataToPoints,m_fCurrentTime,this->m_vChartShowPos,this->m_vChartResolution*m_fChartScale,this->m_fScale) )
			{
				cFUSynchronizedHold	l_cFUSynchronizedHold(&m_PCMToFFTDataConvertr.m_FUSynchronizedForTimeAndFFTDataVector);
				//wait for next new one.
				if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() == 1 )
				{
#ifdef DEBUG
					std::wstring l_strDebugInfo = L"NoMore data,Current Time is :";
					l_strDebugInfo += ValueToStringW(m_fCurrentTime);
					cGameApp::OutputDebugInfoString(l_strDebugInfo);
#endif
					break;
				}
				float l_fStartTime = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->fStartTime;
				float l_fEndime = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->fEndTime;
				delete m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0];
				m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.erase(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.begin());
#ifdef DEBUG
				std::wstring l_strDebugInfo = L"CurrentTime:";
				l_strDebugInfo += ValueToStringW(m_fCurrentTime);
				l_strDebugInfo += L",S:";
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
					//const int l_iFetchDataCount = (int)(m_iOneFrameFFTDataCount*m_PCMToFFTDataConvertr.GetFFTSampleScale());
					const int l_iFetchDataCount = (int)(m_iOneFrameFFTDataCount);
					this->GetCurrentMaxFrequence(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->iBiggestFFTDataValueOfIndex,this->m_pOpanalOgg->GetFreq(),l_iFetchDataCount,m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->iBiggestAmplitude);
				}
				break;
			}
		}
	}
}
//base on freq = max_index * Fs / N
void	cKissFFTStreamingConvert::RenderFrequenceNumber()
{
	if(this->m_pOpanalOgg)
	{
		//const int l_iFetchDataCount = (int)(m_iOneFrameFFTDataCount*m_PCMToFFTDataConvertr.GetFFTSampleScale());
		const int l_iFetchDataCount = (int)(m_iOneFrameFFTDataCount);
		int l_iOneStepFrequence = this->m_pOpanalOgg->GetFreq()/l_iFetchDataCount;
		//just need 10 so
		l_iOneStepFrequence =this->m_pOpanalOgg->GetFreq()/10;
	}
}

void	cKissFFTStreamingConvert::Render()
{
	if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() )
	{
		cPCMToFFTDataConvertr::sTimeAndFFTData*l_pTimeAndFFTData = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0];
		int l_iNumPointd = l_pTimeAndFFTData->iFFTDataOneSample;//*2 for one line 2 points,divide 2 for fft only have half count
		//left channel
		GLRender::RenderLine((float*)m_pvFFTDataToPoints,l_iNumPointd,Vector4::One,2);

		if( m_fChartScale >= 2 )
		{
			//int l_iFreqGap = this->m_pOpanalOgg->GetFreq()/l_pTimeAndFFTData->iFFTDataOneSample;
			const int l_ciFreqNeeded = 100;
			//int l_iFreqGap = this->m_pOpanalOgg->GetFreq()/l_ciFreqNeeded;
			float l_fXGap = m_vChartResolution.x/l_ciFreqNeeded/4*this->m_fChartScale;
			Vector2 l_LinePos[l_ciFreqNeeded*2];
			for(int i=0;i<l_ciFreqNeeded;i++)
			{
				l_LinePos[i*2].x = i*l_fXGap+m_vChartShowPos.x;
				l_LinePos[i*2+1].x = i*l_fXGap+m_vChartShowPos.x;
				l_LinePos[i*2].y = m_vChartShowPos.y;
				l_LinePos[i*2+1].y = 20*m_fScale+m_vChartShowPos.y;

				cGameApp::RenderFont(l_LinePos[i*2].x,l_LinePos[i*2+1].y+30,ValueToStringW(i*this->m_pOpanalOgg->GetFreq()/l_ciFreqNeeded).c_str());
			}
			GLRender::RenderLine((float*)l_LinePos,l_ciFreqNeeded*2,Vector4::Red,2);
		}
		RenderMaxAmplitudeAndFrequencyInfo(100,200);
		RenderDebugAmplitudeLine((float)cSoundCompareParameter::m_siDebugAmplitudeValue);
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


int	cKissFFTStreamingConvert::GetFrequenceAmplitiude(int e_iFrenquence)
{
	assert(0&&"not finish yet");
	if( m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector.size() )
	{
		cPCMToFFTDataConvertr::sTimeAndFFTData*l_pTimeAndFFTData = m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0];
		if( e_iFrenquence == this->m_pOpanalOgg->GetFreq() )
		{
			l_pTimeAndFFTData->iCurrentTimeStampIndex;
			//l_pTimeAndFFTData->piLeftChannelFFTData[l_pTimeAndFFTData]
		}
		float l_fFreqLerp = e_iFrenquence/(float)this->m_pOpanalOgg->GetFreq();
		float l_fFreqIndex = l_fFreqLerp*l_pTimeAndFFTData->iFFTDataOneSample;
		int l_FirstFerequence = (int)l_fFreqIndex;
		if( abs(l_fFreqIndex) <= 0.0001 )
		{
		}
		int l_SecondFerequence = l_FirstFerequence+1;
		float l_fLerpValue = 0.f;
		//get proper 2 frequence and do lerp;
		int l_iValueLeft1 = l_pTimeAndFFTData->pFFTData[l_FirstFerequence];
		//int l_iValueRightt1 = l_pTimeAndFFTData->piRightChannelFFTData[l_SecondFerequence];
		//int l_iValueLeft2 = l_pTimeAndFFTData->piLeftChannelFFTData[l_SecondFerequence];
		//int l_iValueRightt2 = l_pTimeAndFFTData->piRightChannelFFTData[l_SecondFerequence];
		//int l_iValue1 = (l_iValueLeft1+l_iValueRightt1)/2;
		//int l_iValue2 = (l_iValueRightt2+l_iValueLeft2)/2;
		////float l_fFinalFrequenceValue
		//float l_fGap = (l_iValue2-l_iValue1)*l_fLerpValue;
		//int l_iFinalValue = (int)l_fGap+l_iValue1;
		//return l_iFinalValue;
		return 0;
	}
	return 0;
}