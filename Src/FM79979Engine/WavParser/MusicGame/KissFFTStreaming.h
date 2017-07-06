#pragma once

#include "KissFFTConvert.h"
#include "Parameters.h"

void	KissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri);
void	KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri);

void	SoundUpdateThread(size_t _workParameter, size_t _pUri);
void	SoundUpdateThreadDone(size_t _workParameter, size_t _pUri);


class cSoundBufferSwaper;
class cKissFFTStreamingConvert;
struct sTimeAndPCMData;
class cPCMToFFTDataConvertr
{
public:
	struct sTimeAndFFTData
	{
		bool	bUpdated;
		int		iBiggestAmplitude;
		int		iBiggestFFTDataValueOfIndex;
		float	fStartTime;
		float	fEndTime;
		float	fTimeGap;
		//only store the index where time is okay for to do fft
		//std::map<float,int>	TimeAndFFTDataIndex;
		//it's possible be wrong here,because the pointer could be reset very offen then the lines would bewrong...
		//best way to avoid this happen is malloc new memory or give a pure data won't be changed,fuck.
		int*	pFFTData;
		int		iFFTDataOneSample;//one fft data length
		int		iCurrentTimeStampIndex;
		int		iNumPointsDraw;
		sTimeAndFFTData(int*e_piFFTData,float e_fStartTime,float e_fEndTime,int e_iFFTDataOneSample,float e_fNextFFTTimeGap);
		~sTimeAndFFTData();
		//if false time over
		bool	GenerateFFTLines(Vector2*e_pLinePoints,float e_fTargetTime,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale);
		//return fft data length(how many bins)
		int		GetCurrentFFTData(int*e_piOutputData,float e_fTargetTime);
	};
	//give a big enough array instead new and delete,because I am lazy...
	kiss_fft_cpx					m_Kiss_FFT_In[OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)];
	kiss_fft_cpx					m_Kiss_FFT_Out[OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)];
	kiss_fft_state*					m_pkiss_fft_state;
	float*							m_pfWindowFunctionConstantValue;
	void							SetupWindowsFunctionConstValue(int e_iCount);
	//2 for channel
	int								m_FFTData[FFT_DATA_SWAP_BUFFER_COUNT][OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)];
	int								m_iNumFFTDataSwapBuffer;
	int								m_iCurrentFFTDataSwapBufferIndex;
	std::vector<sTimeAndFFTData*>	m_TimeAndFFTDataVector;
public:
	cFUSynchronized			m_FUSynchronizedForTimeAndFFTDataVector;
	cPCMToFFTDataConvertr();
	~cPCMToFFTDataConvertr();
	void					SetNFrameFFTDataCount(int e_iNFrameFFTDataCount);
	void					Destroy();
	//if performance is not good enough,e_iOneFrameFFTDataCount should be small to improve performance.
	int*					ProcessFFTData(sTimeAndPCMData*e_pTimeAndPCMData,float e_fTimeToUpdateFFTData,int e_iNFrameFFTDataCount,bool e_bDoFilter = false,float e_fFilterEndScaleValue = 1.f,int e_iFilterStrength = 6);
};

class cKissFFTStreamingConvert:public cKissFFTConvertBase
{
	//cSoundBufferSwaper*				m_pSoundBufferSwaper;
	cPCMToFFTDataConvertr			m_PCMToFFTDataConvertr;
	//make sure 2 thread all in	pause status
	bool							m_bThreadInPause[2];
	bool							m_bThreadAlreadyStop[2];
	bool							m_bThreadStop;
	cFUSynchronized					m_FUSynchronizedForTimeAndPCMDataVector;
	//
	//this for sound thread,once data 
	//std::vector<sTimeAndFFTData*>m_SoundThreadTimeAndPCMDataVector;
	//
	//
	cFUThreadPool*					m_pFUThreadPool;
	friend			void			KissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri);
	friend			void			KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri);
	friend			void			SoundUpdateThread(size_t _workParameter, size_t _pUri);
	friend			void			SoundUpdateThreadDone(size_t _workParameter, size_t _pUri);
	//
	void							StreamingBuffer(int e_iCount,char*e_pData,size_t e_iCurrentPCMDataPosIndex,int e_iBirPersample);
	int								m_iCurrentStreamingBufferDataIndex;
	char							m_StreamingBufferData[PCM_SWAP_BUFFER_COUNT][OGG_STREAMING_SOUND_BUFFER_SIZE];
	std::vector<sTimeAndPCMData*>	m_TimeAndPCMDataVector;
	Vector2*						m_pvFFTDataToPoints;
	//
	cOpanalOgg*						m_pOpanalOgg;
	//
	virtual void					Destroy();
public:
	cKissFFTStreamingConvert();
	virtual ~cKissFFTStreamingConvert();
	//now only support wav
	virtual bool					FetchSoundDataStart(const char*e_strFileName,bool e_bPlaySound = true);
	//need another thread to do this?
	virtual void					Update(float e_fElpaseTime);
	virtual void					Render();
	virtual void					RenderFrequenceNumber();
	virtual float					GetCurrentTimePercentage();
	virtual float					GetTimeLength();
	virtual	void					GoToTime(float e_fTime);
	int								GetFrequenceAmplitiude(int e_iFrenquence);
};