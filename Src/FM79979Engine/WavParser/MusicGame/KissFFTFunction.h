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


void	ProcessFFT(sTimeAndPCMData*e_pTimeAndPCMData,kiss_fft_state*e_pkiss_fft_state,kiss_fft_cpx*e_pkiss_fft_In,kiss_fft_cpx*e_pkiss_fft_Out,
				   float* e_pfPreCompiledWindowFunctionValue,int*e_piFFTOutData);


float*	GenerateWindowsFunctionValue(int e_iFFTBinCoutn);