#pragma once

#include "KissFFTConvert.h"


void	KissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri);
void	KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri);

void	SoundUpdateThread(size_t _workParameter, size_t _pUri);
void	SoundUpdateThreadDone(size_t _workParameter, size_t _pUri);


#define	PCM_SWAP_BUFFER_COUNT	10
#define	FFT_DATA_SWAP_BUFFER_COUNT	10

#define	FFT_DATA_LINE_POINTS_COUNT	2000

class cKissFFTStreamingConvert;
struct sTimeAndPCMData
{
	int		iDebugForOccupyArrayIndex;
	char*	pPCMData;
	int		iNumPCMData;
	float	fStartTime;
	float	fEndTime;
	sTimeAndPCMData(float e_fStartTime,float e_fEndTime,char*e_pPCMData,int	e_iNumPCMData,int e_iOccupyIndex)
	{
		iDebugForOccupyArrayIndex = e_iOccupyIndex;
		pPCMData = e_pPCMData;
		iNumPCMData = e_iNumPCMData;
		fStartTime = e_fStartTime;
		fEndTime = e_fEndTime;
	}
	~sTimeAndPCMData(){}
};
class cPCMToFFTDataConvertr
{
public:
	struct sTimeAndFFTData
	{
		//char*	pPCMData;
		float	fStartTime;
		float	fEndTime;
		float	fTimeGap;
		//only store the index where time is okay for to do fft
		//std::map<float,int>	TimeAndFFTDataIndex;
		int*	piLeftChannelFFTData;
		int*	piRightChannelFFTData;
		int		iFFTDataOneSample;
		int		iTotalFFTDataCount;
		int		iNumChannel;
		sTimeAndFFTData(int*e_piLeftFFTData,int*e_piRightFFTData,float e_fStartTime,float e_fEndTime,int e_iFFTDataOneSample,int e_iTotalFFTDataCount,int e_iNumChannel,float e_fNextFFTTimeGap);
		~sTimeAndFFTData();
		bool	GenerateFFTLines(Vector2*e_pLinePoints,float e_fTargetTime,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale,float e_fNextChannelYGap);
		bool	GenerateFFTLinesByFFTSampleTargetIndex(Vector2*e_pLinePoints,int e_iFFTSampleTargetIndex,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale,float e_fNextChannelYGap);
		bool	ForceGenerateLastFFTLines(Vector2*e_pLinePoints,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale,float e_fNextChannelYGap);
	};
	//give a big enough array instead new and delete
	kiss_fft_cpx			m_Kiss_FFT_In[OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)];
	kiss_fft_cpx			m_Kiss_FFT_Out[OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)];
	kiss_fft_state*			m_pkiss_fft_state;
	float*					m_pfWindowFunctionConstantValue;
	//2 for channel
	int						m_FFTData[FFT_DATA_SWAP_BUFFER_COUNT][2][OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)];
	int						m_iNumFFTDataSwapBuffer;
	int						m_iCurrentFFTDataSwapBufferIndex;
	std::vector<sTimeAndFFTData*>	m_TimeAndFFTDataVector;
	float							m_fFFTSampleScale;
public:
	cFUSynchronized			m_FUSynchronizedForTimeAndFFTDataVector;
	cPCMToFFTDataConvertr();
	~cPCMToFFTDataConvertr();
	void					SetNFrameFFTDataCount(int e_iNFrameFFTDataCount);
	void					Destroy();
	//if performance is not good enough,e_iOneFrameFFTDataCount should be small to improve performance.
	void					ProcessFFTData(sTimeAndPCMData*e_pTimeAndPCMData,float e_fTimeToUpdateFFTData,int e_iNumChannel,int e_iNFrameFFTDataCount);
};

class cKissFFTStreamingConvert:public cKissFFTConvertBase
{
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
	void							StreamingBuffer(int e_iCount,char*e_pData,size_t e_iCurrentPCMDataPosIndex);
	int								m_iCurrentStreamingBufferDataIndex;
	char							m_StreamingBufferData[PCM_SWAP_BUFFER_COUNT][OGG_STREAMING_SOUND_BUFFER_SIZE];
	std::vector<sTimeAndPCMData*>	m_TimeAndPCMDataVector;
	Vector2							m_vFFTDataToPoints[FFT_DATA_LINE_POINTS_COUNT];
	//
	cOpanalOgg*						m_pOpanalOgg;
	//
	virtual void					Destroy();
public:
	cKissFFTStreamingConvert();
	~cKissFFTStreamingConvert();
	//now only support wav
	virtual bool					FetchSoundDataStart(const char*e_strFileName);
	//need another thread to do this?
	virtual void					Update(float e_fElpaseTime);
	virtual void					Render();
	virtual float					GetCurrentTimePercentage();
	virtual float					GetTimeLength();
	virtual	void					GoToTime(float e_fTime);
};