#pragma once

struct sTimeAndPCMData
{
	eDataType		eBitPerSameplType;
	int				iDebugForOccupyArrayIndex;
	char*			pPCMData;
	int				iNumPCMData;
	int				iNumChannel;
	float			fStartTime;
	float			fEndTime;
	sTimeAndPCMData(float e_fStartTime,float e_fEndTime,int e_iNumChannel,char*e_pPCMData,int	e_iNumPCMData,int e_iOccupyIndex,eDataType e_eBitPerSameplType)
	{
		iNumChannel = e_iNumChannel;
		eBitPerSameplType = e_eBitPerSameplType;
		iDebugForOccupyArrayIndex = e_iOccupyIndex;
		pPCMData = e_pPCMData;
		iNumPCMData = e_iNumPCMData;
		fStartTime = e_fStartTime;
		fEndTime = e_fEndTime;
	}
	~sTimeAndPCMData(){}
};

void	ProcessFFT(sTimeAndPCMData*e_pTimeAndPCMData,kiss_fft_state*e_pkiss_fft_state,kiss_fft_cpx*e_pkiss_fft_In,kiss_fft_cpx*e_pkiss_fft_Out,
				   float* e_pfPreCompiledWindowFunctionValue,int*e_piFFTOutData,
				   bool e_bDoFilter,int e_iFilterStrength,float e_fFilterEndScaleValue);