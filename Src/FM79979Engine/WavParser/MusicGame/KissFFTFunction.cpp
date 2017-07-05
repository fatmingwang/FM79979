#include "stdafx.h"
#include "../kiss_fft130/kiss_fft.h"
#include "../kiss_fft130/_kiss_fft_guts.h"
#include "KissFFTFunction.h"
#include "Parameters.h"

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
	l_dbTotalFFTValue /= l_iEndFilterIndex;//(l_iEndFilterIndex/e_iFilterStrengthValue+1);
	//cGameApp::OutputDebugInfoString(L"Start");
	for(int i = 0; i < e_iTransformLength; i++ )
	{
		int l_iValue = e_pFFTDataSrc[l_iStartIndex];
		//l_iValue -= (int)l_dbTotalFFTValue;
		if(l_iValue < l_dbTotalFFTValue )
			l_iValue /= e_iFilterStrengthValue;
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

template<class T>void	PCMBufferToOneChannel(int e_iNumChannel,int e_iCPMBufferCount,T*e_pInPCMBuffer)
{
	if( e_iNumChannel == 1 )
		return;
	for( int l_iPCMBufferIndex=0;l_iPCMBufferIndex<e_iCPMBufferCount;++l_iPCMBufferIndex )
	{
		double l_dbResult = 0;
		int l_iIndexForChannel = l_iPCMBufferIndex*e_iNumChannel;
		for( int l_iCurrentChannel=0;l_iCurrentChannel<e_iNumChannel;++l_iCurrentChannel )
		{
			int l_iIndex = l_iIndexForChannel+l_iCurrentChannel;
			l_dbResult += e_pInPCMBuffer[l_iIndex];
		}
		e_pInPCMBuffer[l_iPCMBufferIndex] = (T)(l_dbResult/e_iNumChannel);
	}
}

template <class T>void	ProcessFFTWithType(int e_iNumChannel,char*e_pPCMBuffer,int e_iPCMBufferCount,kiss_fft_state*e_pkiss_fft_state,kiss_fft_cpx*e_pkiss_fft_In,kiss_fft_cpx*e_pkiss_fft_Out,
				   float* e_pfPreCompiledWindowFunctionValue,int*e_piFFTOutData,
				   bool e_bDoFilter,int e_iFilterStrength,float e_fFilterEndScaleValue)
{
	kiss_fft_cpx* l_pKiss_FFT_In = e_pkiss_fft_In;
	kiss_fft_cpx* l_pKiss_FFT_Out = e_pkiss_fft_Out;
	kiss_fft_state* l_pkiss_fft_state = e_pkiss_fft_state;
	l_pkiss_fft_state->nfft = e_iPCMBufferCount;
	//16bit 2 byte	,32 bit 4 byte
	T*l_pucSoundData = (T*)e_pPCMBuffer;
	// calculate magnitude of first n/2 FFT
	int l_iDidgitalWindownFunctionCount = e_iPCMBufferCount/WINDOWN_FUNCTION_FRUSTRUM;
	for(int l_iSampleIndex = 0;l_iSampleIndex<e_iPCMBufferCount;++l_iSampleIndex)
	{
		//Apply window function on the sample,Hann window
		double multiplier = e_pfPreCompiledWindowFunctionValue[l_iSampleIndex];
		T l_fValue = l_pucSoundData[l_iSampleIndex];
		l_pKiss_FFT_In[l_iSampleIndex].r = (float)(multiplier * l_fValue);
		l_pKiss_FFT_In[l_iSampleIndex].i = 0;  //stores N samples
	}
	kiss_fft(l_pkiss_fft_state, l_pKiss_FFT_In, l_pKiss_FFT_Out);
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
				double l_dbNormBinMag = 2.*l_Msg / e_iPCMBufferCount;
				/* convert to dB value */
				double l_dbDecebile = 20. * log10( l_dbNormBinMag );
			}
#endif
			//
			val = (int)(log(l_Msg) *10);
			e_piFFTOutData[i] = val;
			//m_fFFTResultPhase[l_iCurrentChannelIndex][l_iNumFFTData] = atan2(l_pKiss_FFT_Out[i].i, l_pKiss_FFT_Out[i].r);
		}
	}
	else
	{
		DoFilter(e_fFilterEndScaleValue,l_iDidgitalWindownFunctionCount,e_iPCMBufferCount,e_piFFTOutData,l_pKiss_FFT_Out,e_iFilterStrength);
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
}

void	ProcessFFT(sTimeAndPCMData*e_pTimeAndPCMData,kiss_fft_state*e_pkiss_fft_state,kiss_fft_cpx*e_pkiss_fft_In,kiss_fft_cpx*e_pkiss_fft_Out,
				   float* e_pfPreCompiledWindowFunctionValue,int*e_piFFTOutData,
				   bool e_bDoFilter,int e_iFilterStrength,float e_fFilterEndScaleValue)
{
	if( e_pTimeAndPCMData->eBitPerSameplType == eDataType::eDT_SHORT )
	{
		PCMBufferToOneChannel<short>(e_pTimeAndPCMData->iNumChannel,e_pTimeAndPCMData->iNumPCMData,(short*)e_pTimeAndPCMData->pPCMData);
		ProcessFFTWithType<short>(e_pTimeAndPCMData->iNumChannel,e_pTimeAndPCMData->pPCMData,e_pTimeAndPCMData->iNumPCMData,e_pkiss_fft_state,e_pkiss_fft_In,e_pkiss_fft_Out,
					   e_pfPreCompiledWindowFunctionValue,e_piFFTOutData,
					   e_bDoFilter,e_iFilterStrength,e_fFilterEndScaleValue);
	}
	else
	if( e_pTimeAndPCMData->eBitPerSameplType == eDataType::eDT_FLOAT )
	{
		PCMBufferToOneChannel<float>(e_pTimeAndPCMData->iNumChannel,e_pTimeAndPCMData->iNumPCMData,(float*)e_pTimeAndPCMData->pPCMData);
		ProcessFFTWithType<float>(e_pTimeAndPCMData->iNumChannel,e_pTimeAndPCMData->pPCMData,e_pTimeAndPCMData->iNumPCMData,e_pkiss_fft_state,e_pkiss_fft_In,e_pkiss_fft_Out,
					   e_pfPreCompiledWindowFunctionValue,e_piFFTOutData,
					   e_bDoFilter,e_iFilterStrength,e_fFilterEndScaleValue);
	}
	else
	if( e_pTimeAndPCMData->eBitPerSameplType == eDataType::eDT_INT )
	{
		PCMBufferToOneChannel<int>(e_pTimeAndPCMData->iNumChannel,e_pTimeAndPCMData->iNumPCMData,(int*)e_pTimeAndPCMData->pPCMData);
		ProcessFFTWithType<int>(e_pTimeAndPCMData->iNumChannel,e_pTimeAndPCMData->pPCMData,e_pTimeAndPCMData->iNumPCMData,e_pkiss_fft_state,e_pkiss_fft_In,e_pkiss_fft_Out,
					   e_pfPreCompiledWindowFunctionValue,e_piFFTOutData,
					   e_bDoFilter,e_iFilterStrength,e_fFilterEndScaleValue);
	}
}