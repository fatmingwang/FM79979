#include "stdafx.h"
#include "KissFFTStreaming.h"
#include "../kiss_fft130/_kiss_fft_guts.h"
#include <math.h>
//suggest e_iFilterStrengthValue:6
int DoFilter(float e_fFilterEndScaleValue,int e_iTransformLength,int e_iStartArrayIndex,int*e_pFFTDataSrc,kiss_fft_cpx*e_pKiss_FFT_Out,int e_iFilterStrengthValue)
{
	double l_dbTotalFFTValue = 0;
	int l_iEndFilterIndex = (int)(e_iTransformLength*e_fFilterEndScaleValue);
	int l_iStartIndex = e_iStartArrayIndex;
	int l_iNumFFTData = e_iStartArrayIndex;
	for(int i = 0; i < e_iTransformLength; i++ )
	{
		float l_Msg = sqrt((e_pKiss_FFT_Out[i].r * e_pKiss_FFT_Out[i].r) + (e_pKiss_FFT_Out[i].i * e_pKiss_FFT_Out[i].i));
		if( i<l_iEndFilterIndex )
			l_dbTotalFFTValue += l_Msg;
		e_pFFTDataSrc[l_iNumFFTData] = (int)l_Msg;
		assert(l_iNumFFTData<=OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)&&"m_FFTData out of range");
		++l_iNumFFTData;
	}
	l_dbTotalFFTValue /= (l_iEndFilterIndex/e_iFilterStrengthValue+1);
	//cGameApp::OutputDebugInfoString(L"Start");
	for(int i = 0; i < e_iTransformLength; i++ )
	{
		int l_iValue = e_pFFTDataSrc[l_iStartIndex];
		l_iValue -= (int)l_dbTotalFFTValue;
		if( l_iValue <= 1 )
			l_iValue = AFTER_FILTER_MIN_VALUE;
		else
		{
			l_iValue = (int)(log(l_iValue) *10);
			//cGameApp::OutputDebugInfoString(ValueToString(l_iValue).c_str());
			//OutputDebugString(ValueToStringW(l_iValue).c_str());
			//OutputDebugString(L",");
		}
		e_pFFTDataSrc[l_iStartIndex] = l_iValue;
		++l_iStartIndex;
	}
	//cGameApp::OutputDebugInfoString(L"\n");
	return l_iNumFFTData;
}
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
	int l_iNumChannel = l_pKissFFTStreamingConvert->m_pOpanalOgg->GetChannelCount();
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
			l_pKissFFTStreamingConvert->m_PCMToFFTDataConvertr.ProcessFFTData(l_pTimeAndPCMData,l_pKissFFTStreamingConvert->m_TimeToUpdateFFTData.fTargetTime,l_iNumChannel,l_iNumFFTDataCount,l_pKissFFTStreamingConvert->IsFilter(),l_pKissFFTStreamingConvert->GetFrenquenceFilterEndScaleValue(),l_pKissFFTStreamingConvert->GetFilterStrengthValue());
	}
}

void	KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri)
{
	cKissFFTStreamingConvert*l_pKissFFTStreamingConvert = (cKissFFTStreamingConvert*)_workParameter;
	l_pKissFFTStreamingConvert->m_bThreadAlreadyStop[1] = true;
}


cPCMToFFTDataConvertr::sTimeAndFFTData::sTimeAndFFTData(int*e_piLeftFFTData,int*e_piRightFFTData,float e_fStartTime,float e_fEndTime,int e_iFFTDataOneSample,int e_iTotalFFTDataCount,int e_iNumChannel,float e_fNextFFTTimeGap)
{
	iCurrentTimeStampIndex = -1;
	iCurrentFetchFFTDataTimeStampIndex = -1;
	iBiggestFFTDataValueOfIndex = 0;
	iNumChannel = e_iNumChannel;
	//pPCMData = nullptr;
	piLeftChannelFFTData = e_piLeftFFTData;
	piRightChannelFFTData = e_piRightFFTData;
	fStartTime = e_fStartTime;
	fEndTime = e_fEndTime;
	iFFTDataOneSample = e_iFFTDataOneSample;
	iTotalFFTDataCount = e_iTotalFFTDataCount;
	fTimeGap = e_fNextFFTTimeGap;
	//FFT_DATA_LINE_POINTS_COUNT
	//assert(e_iFFTDataOneSample<1000&&"I didn't expect this...");
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

bool	cPCMToFFTDataConvertr::sTimeAndFFTData::GenerateFFTLines(Vector2*e_pLinePoints,float e_fTargetTime,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale,float e_fNextChannelYGap)
{
	int l_IndexToJump = GetIndexToJumpByTime(e_fTargetTime);
	if( l_IndexToJump != -1 )
	{
		GenerateFFTLinesByFFTSampleTargetIndex(e_pLinePoints,l_IndexToJump,e_vShowPos,e_vChartResolution,e_fScale,e_fNextChannelYGap);
		return true;
	}
	return false;
}

int	cPCMToFFTDataConvertr::sTimeAndFFTData::GetCurrentFFTData(int*e_piOutputData,float e_fTargetTime)
{
	int l_IndexToJump = GetIndexToJumpByTime(e_fTargetTime);
	if( l_IndexToJump != -1 )
	{
		int l_iFFTDataLength = iFFTDataOneSample/WINDOWN_FUNCTION_FRUSTRUM;
		if( this->iCurrentFetchFFTDataTimeStampIndex != l_IndexToJump )
		{
			this->iCurrentFetchFFTDataTimeStampIndex = l_IndexToJump;
	//		this->iTotalFFTDataCount
		//	e_piOutputData
			//only one channel
			if( this->iNumChannel == 1 )
			{
				int*l_pChannelData = piLeftChannelFFTData?piLeftChannelFFTData:piRightChannelFFTData;
				int*l_pStartFFTData = &l_pChannelData[l_iFFTDataLength*l_IndexToJump];
				memcpy(e_piOutputData,l_pStartFFTData,sizeof(int)*l_iFFTDataLength);
			}
			else
			{
				for(int i=0;i<l_iFFTDataLength;++i)
				{
					e_piOutputData[i] = 0;
					for(int l_iChannelIndex = 0;l_iChannelIndex<this->iNumChannel;++l_iChannelIndex)
					{
						int*l_pChannelData = l_iChannelIndex==0?piLeftChannelFFTData:piRightChannelFFTData;
						int*l_pStartFFTData = &l_pChannelData[l_iFFTDataLength*l_IndexToJump];
						int l_iValue = l_pStartFFTData[i];
						e_piOutputData[i] += l_iValue;
					}
					e_piOutputData[i] /= 2;
				}
			}
		}
		return l_iFFTDataLength;
	}
	return -1;
}

bool	cPCMToFFTDataConvertr::sTimeAndFFTData::GenerateFFTLinesByFFTSampleTargetIndex(Vector2*e_pLinePoints,int e_iFFTSampleTargetIndex,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale,float e_fNextChannelYGap)
{
	if( iCurrentTimeStampIndex != e_iFFTSampleTargetIndex)
		iCurrentTimeStampIndex = e_iFFTSampleTargetIndex;
	else//same time do nothing
		return true;
	float l_fXGap = e_vChartResolution.x/iFFTDataOneSample/2;
	float l_fYGap = e_fNextChannelYGap;
	float l_fStartXPos = e_vShowPos.x;
	//float l_fNextChannelXPos = 100+e_vChartResolution.x/2;
	float l_fNextChannelXPos = 0;
	float l_fYStartPos = e_vShowPos.y;

	int l_iFFTDataLength = iFFTDataOneSample/WINDOWN_FUNCTION_FRUSTRUM;
	//
	//if( e_iFFTSampleTargetIndex*l_iFFTDataLength+l_iFFTDataLength > this->iTotalFFTDataCount )
	//{
	//	l_iFFTDataLength = this->iTotalFFTDataCount-e_iFFTSampleTargetIndex*l_iFFTDataLength+l_iFFTDataLength;
	//	l_iFFTDataLength /= 2;
	//	return false;
	//}
	int l_iMax = -79979;
	this->iBiggestFFTDataValueOfIndex = -1;
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
			if( l_iMax < l_iValue )
			{
				l_iMax = l_iValue;
				this->iBiggestFFTDataValueOfIndex = i;
			}
		}
	}
	return true;
}



int		cPCMToFFTDataConvertr::sTimeAndFFTData::GetIndexToJumpByTime(float e_fTime)
{
	//if( fEndTime-fTimeGap < e_fTargetTime )
	//{
	//	return false;
	//}
	int l_IndexToJump = (int)((e_fTime-this->fStartTime)/fTimeGap);
	if(this->fStartTime>e_fTime)
	{//how come this happen...fucking sad...
		if(abs(this->fStartTime-e_fTime)>=1.f/ONE_FRAME_NEED_NUM_FFT_DATA_COUNT)
		{
#ifdef DEBUG
			std::wstring l_strDebugInfo = L"1:S:";
			l_strDebugInfo += ValueToStringW(this->fStartTime);
			l_strDebugInfo += L",E:";
			l_strDebugInfo += ValueToStringW(this->fEndTime);
			l_strDebugInfo += L",TargetTime:";
			l_strDebugInfo += ValueToStringW(e_fTime);
			l_strDebugInfo += L":how come this happen,someone call go to time???...fucking sad,GenerateFFTLines";
			cGameApp::OutputDebugInfoString(l_strDebugInfo);
#endif
			//anyway take a quick fetch avoid screen flash for go to time has been called
			//GenerateFFTLinesByFFTSampleTargetIndex(e_pLinePoints,0,e_vShowPos,e_vChartResolution,e_fScale,e_fNextChannelYGap);
			return -1;
		}
		l_IndexToJump = 0;
	}
	int l_iFFTDataLength = iFFTDataOneSample/WINDOWN_FUNCTION_FRUSTRUM;
	if(iTotalFFTDataCount<l_IndexToJump*l_iFFTDataLength+l_iFFTDataLength)
	{
		//cGameApp::OutputDebugInfoString(L"GenerateFFTLines,here should not happen,fft data count is not enough,,cKissFFTStreamingConvert::ProcessFFTData() need more accurate the performance goes!.");
		//here should not happen,cKissFFTStreamingConvert::ProcessFFTData() need more accurate the performance goes!.
		return -1;
	}
	return l_IndexToJump;
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
void	cPCMToFFTDataConvertr::ProcessFFTData(sTimeAndPCMData*e_pTimeAndPCMData,float e_fTimeToUpdateFFTData,int e_iNumChannel,int e_iNFrameFFTDataCount,bool e_bDoFilter,float e_fFilterEndScaleValue,int e_iFilterStrength)
{
	if( !e_pTimeAndPCMData)
		return;
	//do fft by data length not elpase time.
	bool	l_bForceDoAllFFT = false;
	float l_fFFTSampleScale = m_fFFTSampleScale;
	sTimeAndPCMData*l_pTimeAndPCMData = e_pTimeAndPCMData;
	UT::sTimeAndFPS	l_Timer;
	l_Timer.Update();
	//
	float	l_fTimeToUpdateFFT = e_fTimeToUpdateFFTData;
	float	l_fCurrentNewDataTime = l_pTimeAndPCMData->fStartTime;
	float	l_fEndTime = l_pTimeAndPCMData->fEndTime;
	float	l_fDuring = l_fEndTime-l_fCurrentNewDataTime;
	int		l_iNumFFTDataNeed = (int)(l_fDuring/l_fTimeToUpdateFFT);//this->m_TimeToUpdateFFTData.fTargetTime);
	int l_iNumPCMDataByShort = l_pTimeAndPCMData->iNumPCMData/sizeof(short);
	if( l_bForceDoAllFFT )
	{
		l_iNumFFTDataNeed = l_iNumPCMDataByShort/e_iNFrameFFTDataCount;
		if( l_iNumPCMDataByShort%e_iNFrameFFTDataCount )
		{//sorry I am lazy to do more...
			//++l_iNumFFTDataNeed;
		}
		l_fTimeToUpdateFFT = l_fDuring/l_iNumFFTDataNeed;
		l_fFFTSampleScale = 1.f;
	}
	if( l_iNumFFTDataNeed == 0 )
	{
		if( abs(l_fDuring-l_fTimeToUpdateFFT) <= 0.01f )
			l_iNumFFTDataNeed = 1;
		else
		{
			//fuck please increas the ogg cache size(OGG_STREAMING_SOUND_BUFFER_SIZE)
			UT::ErrorMsg(L"the m_iDivideFFTDataToNFrameis too small increase m_iDivideFFTDataToNFrame or increas the ogg cache size(OGG_STREAMING_SOUND_BUFFER_SIZE)",L"Error!!");
			goto FINISHED;
		}
	}
	//float	l_fOneFFTDataPeriod = (float)this->m_iOneFrameFFTDataCount/this->m_pOpanalOgg->GetPCMDataSize()*this->m_pOpanalOgg->GetTimeLength();

	int		l_iNextPCMDataStep = l_iNumPCMDataByShort/l_iNumFFTDataNeed;
	size_t	l_uiEndIndex = l_iNextPCMDataStep*l_iNumFFTDataNeed;
	int  l_iChannels = e_iNumChannel;//m_pOpanalOgg->GetChannel();
	//assert(l_fFFTSampleScale<=1.f&&"l_fFFTSampleScale cant not over 1");
	assert(l_fFFTSampleScale<=1.f&&"m_fFFTSampleScale cant not over 1");
	int l_iFetchDataCount = (int)(e_iNFrameFFTDataCount*l_fFFTSampleScale);//m_iOneFrameFFTDataCount*l_fFFTSampleScale;//*this->m_pSoundFile->m_iChannel;
	const int l_iOriginalFetchDataCount = (int)(e_iNFrameFFTDataCount*l_fFFTSampleScale);//m_iOneFrameFFTDataCount*l_fFFTSampleScale;//*this->m_pSoundFile->m_iChannel;
	//assert(l_iFetchDataCount>=this->m_iOneFrameFFTDataCount&&"frenquence is too high,is this okay?");
	kiss_fft_cpx* l_pKiss_FFT_In = this->m_Kiss_FFT_In;
	kiss_fft_cpx* l_pKiss_FFT_Out = this->m_Kiss_FFT_Out;
	kiss_fft_state* l_pkiss_fft_state = this->m_pkiss_fft_state;
	l_pkiss_fft_state->nfft = l_iFetchDataCount;
	//auto l_pData = this->m_pkiss_fft_state;
	//l_pData->nfft = l_iFetchDataCount;
	bool l_bDoFFT = true;
	int l_iNumFFTData2 = 0;
	for(int l_iCurrentChannelIndex = 0;l_iCurrentChannelIndex<l_iChannels;++l_iCurrentChannelIndex)
	{
		size_t l_uiCurrentSoundDataIndex = 0;
		int	l_iNumFFTData = 0;
		//16bit 2 byte
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
				int l_iEndIndex = (l_iStartIndex+l_iOriginalFetchDataCount*l_iChannels);
				if( l_iEndIndex > l_iNumPCMDataByShort )
				{
					if( l_bForceDoAllFFT )
					{
						int l_iRestCount = (l_iNumPCMDataByShort-l_iStartIndex)/l_iChannels;
						l_iFetchDataCount = l_pkiss_fft_state->nfft = l_iRestCount;
						if( l_iRestCount%2 )
						{
							int a=0;
						}
						if( l_iFetchDataCount == 0 )
							break;
					}
					else
					{
						break;
					}
				}
				else
				{
					l_iFetchDataCount = l_iOriginalFetchDataCount;
					l_pkiss_fft_state->nfft = l_iFetchDataCount;
				}
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
					//if( l_fValue < 20 )
						//l_fValue = 0;
					l_pKiss_FFT_In[l_iCurrentIndex].r = (float)(multiplier * l_fValue);
					l_pKiss_FFT_In[l_iCurrentIndex].i = 0;  //stores N samples 

				}
				kiss_fft(l_pkiss_fft_state, l_pKiss_FFT_In, l_pKiss_FFT_Out);
				// calculate magnitude of first n/2 FFT
				int l_iDidgitalWindownFunctionCount = l_iFetchDataCount/WINDOWN_FUNCTION_FRUSTRUM;
				bool	l_bdoFilter = e_bDoFilter;
				if( !l_bdoFilter )
				{
					for(int i = 0; i < l_iDidgitalWindownFunctionCount; i++ )
					{
						int val;
						float l_Msg = sqrt((l_pKiss_FFT_Out[i].r * l_pKiss_FFT_Out[i].r) + (l_pKiss_FFT_Out[i].i * l_pKiss_FFT_Out[i].i));
#ifdef DEBUG
						{//https://groups.google.com/forum/#!topic/comp.dsp/cZsS1ftN5oI
							/* get normalized bin magnitude */
							double l_dbNormBinMag = 2.*l_Msg / l_iFetchDataCount;
							/* convert to dB value */
							double l_dbDecebile = 20. * log10( l_dbNormBinMag );
						}
#endif
						//
						val = (int)(log(l_Msg) *10);
						m_FFTData[m_iCurrentFFTDataSwapBufferIndex][l_iCurrentChannelIndex][l_iNumFFTData] = val;
						//m_fFFTResultPhase[l_iCurrentChannelIndex][l_iNumFFTData] = atan2(l_pKiss_FFT_Out[i].i, l_pKiss_FFT_Out[i].r);
						assert(l_iNumFFTData<=OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)&&"m_FFTData out of range");
						++l_iNumFFTData;
						//int l_iPhase = 180*atan2(l_pKiss_FFT_Out[i].r,l_pKiss_FFT_Out[i].i)/D3DX_PI-90;
						//int l_iPhase2 = 180*atan2(l_pKiss_FFT_Out[i].i,l_pKiss_FFT_Out[i].r)/D3DX_PI-90;
					}
				}
				else
				{
					l_iNumFFTData = DoFilter(e_fFilterEndScaleValue,l_iDidgitalWindownFunctionCount,l_iNumFFTData,m_FFTData[m_iCurrentFFTDataSwapBufferIndex][l_iCurrentChannelIndex],l_pKiss_FFT_Out,e_iFilterStrength);
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
		//l_Timer.Update();
		//double secs = l_Timer.fElpaseTime;
		//static int l_iTest = 0;
		//++l_iTest;
		//std::wstring l_strDebugInfo = L"do fft spent  ";
		//l_strDebugInfo += ValueToStringW(secs);
		//l_strDebugInfo += L":";
		//l_strDebugInfo += ValueToStringW(l_iTest);
		//cGameApp::OutputDebugInfoString(l_strDebugInfo);
	}
#endif
	//
	sTimeAndFFTData*l_pTimeAndFFTData = new sTimeAndFFTData(m_FFTData[m_iCurrentFFTDataSwapBufferIndex][0],m_FFTData[m_iCurrentFFTDataSwapBufferIndex][1],l_fCurrentNewDataTime,l_fEndTime,l_iOriginalFetchDataCount,l_iNumFFTData2,l_iChannels,l_fTimeToUpdateFFT);
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
	m_TimeToUpdateFFTData.SetTargetTime(1.f/m_iDivideFFTDataToNFrame);
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
		const int l_iFetchDataCount = (int)(this->m_iOneFrameFFTDataCount*this->m_PCMToFFTDataConvertr.GetFFTSampleScale());//m_iOneFrameFFTDataCount*m_fFFTSampleScale;//*this->m_pSoundFile->m_iChannel;
		if( e_iCount <= l_iFetchDataCount )
		{
#ifdef DEBUG
			UT::ErrorMsg(L"Error",L"ogg streaming input count is too small cannt satisfied m_iDivideFFTDataToNFrame,please increase file cached size or ogg cached size(OGG_STREAMING_SOUND_BUFFER_SIZE now is 32k)");
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


bool	cKissFFTStreamingConvert::FetchSoundDataStart(const char*e_strFileName,bool e_bPlaySound)
{
	Destroy();
	this->m_fCurrentTime = 0.f;
	m_pOpanalOgg = new cOpanalOgg(this,e_strFileName,true,std::bind(&cKissFFTStreamingConvert::StreamingBuffer,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),false);
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
	m_iOneFrameFFTDataCount = power_of_two(m_pOpanalOgg->GetFreq()/m_iDivideFFTDataToNFrame);
	//m_iOneFrameFFTDataCount = m_pOpanalOgg->GetFreq()/m_iDivideFFTDataToNFrame;
	if(m_iOneFrameFFTDataCount*2>FFT_DATA_LINE_POINTS_COUNT)
	{//too big?give it one more chance
		m_iOneFrameFFTDataCount /= 2;
	}
	if(m_iOneFrameFFTDataCount*2>FFT_DATA_LINE_POINTS_COUNT)
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
			if( !m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->GenerateFFTLines(this->m_vFFTDataToPoints,m_fCurrentTime,this->m_vChartShowPos,this->m_vChartResolution*m_fChartScale,this->m_fScale,this->m_fNextChannelYGap) )
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
					const int l_iFetchDataCount = (int)(m_iOneFrameFFTDataCount*m_PCMToFFTDataConvertr.GetFFTSampleScale());
					this->m_iMaxAmplitudeFrequence = this->GetCurrentMaxFrequence(m_PCMToFFTDataConvertr.m_TimeAndFFTDataVector[0]->iBiggestFFTDataValueOfIndex,this->m_pOpanalOgg->GetFreq(),l_iFetchDataCount);
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
		const int l_iFetchDataCount = (int)(m_iOneFrameFFTDataCount*m_PCMToFFTDataConvertr.GetFFTSampleScale());
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
		int l_iNumPointd = l_pTimeAndFFTData->iFFTDataOneSample/2*2;//*2 for one line 2 points,divide 2 for fft only have half count
		//left channel
		GLRender::RenderLine((float*)m_vFFTDataToPoints,l_iNumPointd,Vector4::One,2);

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

		std::wstring l_strInfo = L"Frequence:";
		l_strInfo += ValueToStringW(this->m_iMaxAmplitudeFrequence);
		cGameApp::m_spGlyphFontRender->SetScale(2.f);
		cGameApp::RenderFont(100,200,l_strInfo);
		cGameApp::m_spGlyphFontRender->SetScale(1.f);
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

void	cKissFFTStreamingConvert::SetFFTSampleScale(float e_fScale,bool e_bForceSet)
{
	if(e_fScale>1.f)
		e_fScale = 1.f;
	if(e_fScale<=0.f)
		e_fScale = 0.1f;

	cGameApp::m_sbGamePause = true;
	if( e_bForceSet == false )
	{
		while( !m_bThreadInPause[0] || !m_bThreadInPause[1] )
		{//wait for pause
			Sleep(1);
		}
	}

	int l_iNum = (int)(this->m_iOneFrameFFTDataCount*e_fScale);
	SAFE_DELETE(this->m_PCMToFFTDataConvertr.m_pfWindowFunctionConstantValue);
	this->m_PCMToFFTDataConvertr.m_pfWindowFunctionConstantValue = new float[l_iNum];

	for( int l_iCount = 0;l_iCount <l_iNum;++l_iCount)
	{

		//const float £k = 3.1415926f;
		// apply hanning window to buffer
		//below one is possible divide by 0 so dont use this,fuck
		//double multiplier  = 0.54f - 0.46f * cos( (float)l_iCount * 2.0f * D3DX_PI / (l_iCount-1) );
		double multiplier = 0.5 * (1 - cos(2*D3DX_PI*l_iCount/(l_iNum-1)));
		// apply hanning window to buffer
		//double multiplier = 0.54-0.46 * cos(2*D3DX_PI*l_iCount/(l_iNum));
		this->m_PCMToFFTDataConvertr.m_pfWindowFunctionConstantValue[l_iCount] = (float)multiplier;
	}
	if( this->m_PCMToFFTDataConvertr.m_pkiss_fft_state )
	{
		free(this->m_PCMToFFTDataConvertr.m_pkiss_fft_state);
		this->m_PCMToFFTDataConvertr.m_pkiss_fft_state = nullptr;
	}
	this->m_PCMToFFTDataConvertr.m_pkiss_fft_state = kiss_fft_alloc(l_iNum, 0/*is_inverse_fft*/, NULL, NULL);
	this->m_PCMToFFTDataConvertr.SetFFTSampleScale(e_fScale);

	cGameApp::m_sbGamePause = false;
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
		int l_iValueLeft1 = l_pTimeAndFFTData->piLeftChannelFFTData[l_FirstFerequence];
		int l_iValueRightt1 = l_pTimeAndFFTData->piRightChannelFFTData[l_SecondFerequence];
		int l_iValueLeft2 = l_pTimeAndFFTData->piLeftChannelFFTData[l_SecondFerequence];
		int l_iValueRightt2 = l_pTimeAndFFTData->piRightChannelFFTData[l_SecondFerequence];
		int l_iValue1 = (l_iValueLeft1+l_iValueRightt1)/2;
		int l_iValue2 = (l_iValueRightt2+l_iValueLeft2)/2;
		//float l_fFinalFrequenceValue
		float l_fGap = (l_iValue2-l_iValue1)*l_fLerpValue;
		int l_iFinalValue = (int)l_fGap+l_iValue1;
		return l_iFinalValue;
	}
	return 0;
}