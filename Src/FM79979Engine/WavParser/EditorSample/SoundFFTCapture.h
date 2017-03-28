#pragma once
#include "KissFFTStreaming.h"

void	SoundFFTCaptureKissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri);
void	SoundFFTCaptureKissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri);

class cSoundFFTCapture:public FATMING_CORE::cSounRecordCallBackObject,public cKissFFTConvertBase
{
	friend void	SoundFFTCaptureKissFFTStreamingConvertThread(size_t _workParameter, size_t _pUri);
	friend void	SoundFFTCaptureKissFFTStreamingConvertThreadDone(size_t _workParameter, size_t _pUri);
	//int								m_iFFTSampleForOneFrame;
	cFUSynchronized					m_FUSynchronizedForTimeAndPCMDataVector;
	bool							m_bThreadInPause;
	bool							m_bThreadAlreadyStop;
	bool							m_bThreadStop;
	bool							m_bThreadPause;
	float							m_fNextSampleTime;
	cPCMToFFTDataConvertr			m_PCMToFFTDataConvertr;
	cFUThreadPool*					m_pFUThreadPool;
	//
	int								m_iCurrentStreamingBufferDataIndex;
	char							m_StreamingBufferData[PCM_SWAP_BUFFER_COUNT][OGG_STREAMING_SOUND_BUFFER_SIZE];
	std::vector<sTimeAndPCMData*>	m_TimeAndPCMDataVector;
	Vector2							m_vFFTDataToPoints[FFT_DATA_LINE_POINTS_COUNT];
public:
	cSoundFFTCapture();
	~cSoundFFTCapture();
	void			Destroy();
	virtual void	CaptureSoundStartCallBack();
	virtual void	CaptureSoundNewDataCallBack(ALCint e_iSamplesIn,char*e_pData);
	virtual void	CaptureSoundEndCallBack();
	void			Update(float e_fElpaseTime);
	void			Render();
	virtual void	GoToTime(float e_fElpaseTime){}
	//use this one for cSoundCapture(buffersize)
	static int		GetOpanalCaptureBufferSize(int e_iFPS,int e_iFrequence);
};