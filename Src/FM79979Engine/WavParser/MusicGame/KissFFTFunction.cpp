#include "stdafx.h"
#include "../kiss_fft130/kiss_fft.h"
#include "../kiss_fft130/_kiss_fft_guts.h"
#include "KissFFTFunction.h"
#include "Parameters.h"

//http://users.ece.utexas.edu/~valvano/Starterfiles/FFT.CPP
//https://groups.google.com/forum/#!topic/comp.dsp/cZsS1ftN5oI
//https://www.kvraudio.com/forum/viewtopic.php?t=276092
// display 15 bins around the frequency of interest 
//    for (long k = 80; k < 110; k += 2) {
//
//    /* real */
//        double re = fftBuffer[k];
//
//    /* imaginary */
//        double im = fftBuffer[k+1];
//
//    /* get normalized bin magnitude */
//        double normBinMag = 2.*sqrt(re*re + im*im) / FFT_SIZE;
//
//    /* convert to dB value */
//        double amplitude = 20. * log10( normBinMag );
//
//    /* and display */
//        printf("bin: %d,\tfreq: %f [Hz],\tmag: %f,\t ampl.: %f [dB]\n", \
//               k/2, sampleRate*.5*(double)k/FFT_SIZE, normBinMag, amplitude);
//    }
inline float AmplitudeTodB(float e_fAmplitude,int e_iNUMFFTBin)
{
	double l_dbNormBinMag = 2 * e_fAmplitude / e_iNUMFFTBin;
	double l_dbAmplitude = 20. * log10(l_dbNormBinMag);
	if( l_dbAmplitude <0 )
		l_dbAmplitude = 0;
	return (float)l_dbAmplitude;
}
 
inline float dBToAmplitude(float e_fDecebiles)
{
  return pow(10.0f, e_fDecebiles/20.0f);
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

template <class T>bool	ProcessFFTWithType(sTimeAndPCMData*e_pTimeAndPCMData,kiss_fft_state*e_pkiss_fft_state,kiss_fft_cpx*e_pkiss_fft_In,kiss_fft_cpx*e_pkiss_fft_Out,
				   float* e_pfPreCompiledWindowFunctionValue,int*e_piFFTOutData)
{
	kiss_fft_cpx* l_pKiss_FFT_In = e_pkiss_fft_In;
	kiss_fft_cpx* l_pKiss_FFT_Out = e_pkiss_fft_Out;
	kiss_fft_state* l_pkiss_fft_state = e_pkiss_fft_state;
	assert(e_pTimeAndPCMData->eBitPerSameplType==eDataType::eDT_SHORT&&"sorry I am lazy now only support short!");
	int l_iNumPCMBuffer = e_pTimeAndPCMData->iPCMDataSampleCount;
	//16bit 2 byte	,32 bit 4 byte
	T*l_pucSoundData = (T*)e_pTimeAndPCMData->pPCMData;
	// calculate magnitude of first n/2 FFT
	int l_iDidgitalWindownFunctionCount = l_iNumPCMBuffer/WINDOWN_FUNCTION_FRUSTRUM;
	for(int l_iSampleIndex = 0;l_iSampleIndex<l_iNumPCMBuffer;++l_iSampleIndex)
	{
		//Apply window function on the sample,Hann window
		float l_fMultiplier = e_pfPreCompiledWindowFunctionValue[l_iSampleIndex];
		T l_fValue = l_pucSoundData[l_iSampleIndex];
		//l_pKiss_FFT_In[l_iSampleIndex].r = l_fMultiplier * l_fValue;
		l_pKiss_FFT_In[l_iSampleIndex].r = l_fMultiplier * l_fValue;
		//l_pKiss_FFT_In[l_iSampleIndex].r = cos(2 * l_iSampleIndex * 2 * D3DX_PI / l_iNumPCMBuffer);
		l_pKiss_FFT_In[l_iSampleIndex].i = 0;
	}
	//for (int l_iSampleIndex = 0; l_iSampleIndex < l_iNumPCMBuffer / 2; l_iSampleIndex += 2)
	//{
	//	//Apply window function on the sample,Hann window
	//	float l_fMultiplier1 = e_pfPreCompiledWindowFunctionValue[l_iSampleIndex];
	//	float l_fMultiplier2 = e_pfPreCompiledWindowFunctionValue[l_iSampleIndex + 1];
	//	T l_fValue1 = l_pucSoundData[l_iSampleIndex];
	//	T l_fValue2 = l_pucSoundData[l_iSampleIndex + 1];
	//	l_pKiss_FFT_In[l_iSampleIndex].r = l_fMultiplier1 * l_fValue1;
	//	l_pKiss_FFT_In[l_iSampleIndex].i = l_fMultiplier2 * l_fValue2;  //stores N samples
	//}
	kiss_fft(l_pkiss_fft_state, l_pKiss_FFT_In, l_pKiss_FFT_Out);
	float l_fScale = 1. / ((float)(l_iDidgitalWindownFunctionCount / 2 + 1));
	for(int i = 0; i < l_iDidgitalWindownFunctionCount; i++ )
	{
		if (1)
		{
			float l_Amplitude = sqrt((l_pKiss_FFT_Out[i].r * l_pKiss_FFT_Out[i].r) + (l_pKiss_FFT_Out[i].i * l_pKiss_FFT_Out[i].i));
			auto l_Result1 = AmplitudeTodB(l_Amplitude, l_iNumPCMBuffer);;
			e_piFFTOutData[i] = (int)l_Result1;
		}
		else
		{
			//from GLMViz-master\src\FFT.cpp
			//void FFT::magnitudes(const size_t w_size){
			auto l_Result = 20 * std::log10(std::hypot(l_pKiss_FFT_Out[i].r, l_pKiss_FFT_Out[i].i) * l_fScale);
			e_piFFTOutData[i] = l_Result;
		}
	}
	return false;
}

void	ProcessFFT(sTimeAndPCMData*e_pTimeAndPCMData,kiss_fft_state*e_pkiss_fft_state,kiss_fft_cpx*e_pkiss_fft_In,kiss_fft_cpx*e_pkiss_fft_Out,
				   float* e_pfPreCompiledWindowFunctionValue,int*e_piFFTOutData)
{
	if( e_pTimeAndPCMData->eBitPerSameplType == eDataType::eDT_SHORT )
	{
		PCMBufferToOneChannel<short>(e_pTimeAndPCMData->iNumChannel,e_pTimeAndPCMData->iPCMDataSampleCount,(short*)e_pTimeAndPCMData->pPCMData);
		ProcessFFTWithType<short>(e_pTimeAndPCMData,e_pkiss_fft_state,e_pkiss_fft_In,e_pkiss_fft_Out,
					   e_pfPreCompiledWindowFunctionValue,e_piFFTOutData);
	}
	else
	if( e_pTimeAndPCMData->eBitPerSameplType == eDataType::eDT_FLOAT )
	{
		PCMBufferToOneChannel<float>(e_pTimeAndPCMData->iNumChannel,e_pTimeAndPCMData->iPCMDataSampleCount,(float*)e_pTimeAndPCMData->pPCMData);
		ProcessFFTWithType<float>(e_pTimeAndPCMData,e_pkiss_fft_state,e_pkiss_fft_In,e_pkiss_fft_Out,
					   e_pfPreCompiledWindowFunctionValue,e_piFFTOutData);
	}
	else
	if( e_pTimeAndPCMData->eBitPerSameplType == eDataType::eDT_INT )
	{
		PCMBufferToOneChannel<int>(e_pTimeAndPCMData->iNumChannel,e_pTimeAndPCMData->iPCMDataSampleCount,(int*)e_pTimeAndPCMData->pPCMData);
		ProcessFFTWithType<int>(e_pTimeAndPCMData,e_pkiss_fft_state,e_pkiss_fft_In,e_pkiss_fft_Out,
					   e_pfPreCompiledWindowFunctionValue,e_piFFTOutData);
	}
}

float*	GenerateWindowsFunctionValue(int e_iFFTBinCoutn)
{
	/* Hann window */
	bool l_bHannWindow = true;
	if (l_bHannWindow)
	{
		float*l_pfWindowFunctionConstantValue = new float[e_iFFTBinCoutn];
		int l_iConstantValue = e_iFFTBinCoutn-1;
		for( int l_iCount = 0;l_iCount <e_iFFTBinCoutn;++l_iCount)
		{
			double multiplier = 0.5 * (1 - cos(2*D3DX_PI*l_iCount/l_iConstantValue));
			l_pfWindowFunctionConstantValue[l_iCount] = (float)multiplier;
		}
		return l_pfWindowFunctionConstantValue;
	}
	//https://github.com/hannesha/GLMViz/blob/master/src/FFT.cpp
	//from GLMViz-master\src\FFT.cpp
	//void FFT::calculate_window(const size_t w_size){
//#ifndef M_PI
//#  define M_PI (3.1415926536f)
//#endif
	float*l_pfWindowFunctionConstantValue = new float[e_iFFTBinCoutn];
	float N_1 = 1.0 / (float)(e_iFFTBinCoutn - 1);
	// compensated Blackman window constants
	const float a1 = 4620.0 / 3969.0;
	const float a2 = 715.0 / 3969.0;
	for( int l_iCount = 0;l_iCount <e_iFFTBinCoutn;++l_iCount)
	{
		double multiplier = 1.0 - a1 * cos(2 * 3.1415926536f * l_iCount * N_1) + a2 * cos(4 * 3.1415926536f * l_iCount * N_1);
		l_pfWindowFunctionConstantValue[l_iCount] = (float)multiplier;
	}
	return l_pfWindowFunctionConstantValue;
}