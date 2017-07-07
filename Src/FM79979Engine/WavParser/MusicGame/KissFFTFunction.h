#pragma once

struct sTimeAndPCMData
{
	eDataType		eBitPerSameplType;
	char*			pPCMData;
	int				iPCMDataSampleCount;
	int				iNumChannel;
	float			fStartTime;
	float			fEndTime;
	sTimeAndPCMData(float e_fStartTime,float e_fEndTime,int e_iNumChannel,char*e_pPCMData,int	e_iPCMDataSampleCount,eDataType e_eBitPerSameplType)
	{
		iNumChannel = e_iNumChannel;
		eBitPerSameplType = e_eBitPerSameplType;
		pPCMData = e_pPCMData;
		iPCMDataSampleCount = e_iPCMDataSampleCount;
		fStartTime = e_fStartTime;
		fEndTime = e_fEndTime;
	}
	~sTimeAndPCMData(){}
};

int DoFilter(int e_iNumFFT,int*e_pFFTDataSrc,kiss_fft_cpx*e_pKiss_FFT_Out);

int DoFilter(float e_fFilterEndScaleValue,int e_iTransformLength,int e_iStartArrayIndex,int*e_pFFTDataSrc,kiss_fft_cpx*e_pKiss_FFT_Out,int e_iFilterStrengthValue);

void	ProcessFFT(sTimeAndPCMData*e_pTimeAndPCMData,kiss_fft_state*e_pkiss_fft_state,kiss_fft_cpx*e_pkiss_fft_In,kiss_fft_cpx*e_pkiss_fft_Out,
				   float* e_pfPreCompiledWindowFunctionValue,int*e_piFFTOutData,
				   bool e_bDoFilter,int e_iFilterStrength,float e_fFilterEndScaleValue);


float*	GenerateWindowsFunctionValue(int e_iFFTBinCoutn);