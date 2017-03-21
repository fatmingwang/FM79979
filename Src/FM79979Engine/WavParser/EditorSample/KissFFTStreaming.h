#pragma once

#include "KissFFTConvert.h"


void	KissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri);
void	KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri);

#define	PCM_SWAP_BUFFER_COUNT	5
#define	FFT_DATA_SWAP_BUFFER_COUNT	3
class cKissFFTStreamingConvert:public cKissFFTConvertBase
{
	cFUSynchronized				m_FUSynchronized;
	//
	struct sTimeAndFFTData
	{
		//char*	pPCMData;
		float	fStartTime;
		float	fEndTime;
		int*	piLeftChannelFFTData;
		int*	piRightChannelFFTData;
		int		iFFTDataOneSample;
		int		iTotalFFTDataCount;
		int		iNumChannel;
		Vector2	vFFTDataToPoints[2000];
		sTimeAndFFTData(int*e_piLeftFFTData,int*e_piRightFFTData,float e_fStartTime,float e_fEndTime,int e_iFFTDataOneSample,int e_iTotalFFTDataCount,int e_iNumChannel);
		~sTimeAndFFTData();
		bool	GenerateFFTLines(float e_fTargetTime,Vector2 e_vShowPos,Vector2 e_vChartResolution,float e_fScale,float e_fNextChannelYGap);
	};
	//this for sound thread,once data 
	//std::vector<sTimeAndFFTData*>m_SoundThreadTimeAndPCMDataVector;
	//std::vector<sTimeAndFFTData*>m_TimeAndPCMDataVector;
	//
	//give a big enough array instead new and delete
	kiss_fft_cpx			m_Kiss_FFT_In[OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)];
	kiss_fft_cpx			m_Kiss_FFT_Out[OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)];
	//2 for channel
	int						m_FFTData[FFT_DATA_SWAP_BUFFER_COUNT][2][OGG_STREAMING_SOUND_BUFFER_SIZE/sizeof(short)];
	int						m_iNumFFTDataSwapBuffer;
	int						m_iCurrentFFTDataSwapBufferIndex;
	std::vector<sTimeAndFFTData*>	m_TimeAndPCMDataVector;
	//
	cFUThreadPool*			m_pFUThreadPool;
	friend			void	KissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri);
	friend			void	KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri);
	//
	//char			m_StreamingBufferData[PCM_SWAP_BUFFER_COUNT][OGG_STREAMING_SOUND_BUFFER_SIZE];
	char			m_StreamingBufferData[OGG_STREAMING_SOUND_BUFFER_SIZE];
	bool			m_bReceivedBuffer;
	void			StreamingBuffer(int e_iCount,char*e_pData,size_t e_iCurrentPCMDataPosIndex);
	//if performance is not good enough,e_iOneFrameFFTDataCount should be small to improve performance.
	void			ProcessFFTData(int e_iOneFrameFFTDataCount,float e_fElpaseTime);
	//
	cOpanalOgg*		m_pOpanalOgg;
	bool			m_bNewData;
	bool			m_bThreadStop;
	bool			m_bThreadAlreadyStop;
	//
	virtual void	Destroy();
public:
	cKissFFTStreamingConvert();
	~cKissFFTStreamingConvert();
	//now only support wav
	virtual bool	FetchSoundDataStart(const char*e_strFileName);
	//need another thread to do this?
	virtual void	Update(float e_fElpaseTime);
	virtual void	Render();
	virtual float	GetCurrentTimePercentage();
	virtual float	GetTimeLength();
};