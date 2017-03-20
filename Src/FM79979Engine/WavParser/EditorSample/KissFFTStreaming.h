#pragma once

#include "KissFFTConvert.h"


void	KissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri);
void	KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri);

#define	PCM_SWAP_BUFFER_COUNT	5
class cKissFFTStreamingConvert:public cKissFFTConvertBase
{
	struct sTimeAndPCMData
	{
		char*	pPCMData;
		float	fTime;
		int*	pFFTData;
		sTimeAndPCMData(char*e_pPCMData,float e_fTime){pPCMData = e_pPCMData;fTime = e_fTime; }
		sTimeAndPCMData(){SAFE_DELETE(pPCMData); }
	};
	//this for sound thread,once data 
	std::vector<sTimeAndPCMData*>m_SoundThreadTimeAndPCMDataVector;
	std::vector<sTimeAndPCMData*>m_TimeAndPCMDataVector;
	//
	cFUThreadPool*			m_pFUThreadPool;
	friend			void	KissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri);
	friend			void	KissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri);
	//
	//char			m_StreamingBufferData[PCM_SWAP_BUFFER_COUNT][OGG_STREAMING_SOUND_BUFFER_SIZE];
	char			m_StreamingBufferData[OGG_STREAMING_SOUND_BUFFER_SIZE];
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
};